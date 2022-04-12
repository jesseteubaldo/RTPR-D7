<?php

/**
 * @file
 * Features relate to MRC.
 */

function rtpr_reward_mrc_reassign_form($form, &$form_state) {
  $form['options'] = array(
    '#type' => 'fieldset',
    '#attributes' => array(
        'class' => array('container-inline'),
    ),
  '#collapsible' => TRUE,
  '#collapsed' => FALSE,

  );

  $form['options']['pap_id'] = array(
    '#title' => t('PAP ID'),
    '#type' => 'textfield',
  );

  $form['options']['email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Reassign MRC',
  );



  return $form;
}

function rtpr_reward_mrc_reassign_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $account = NULL;

  if (!empty($values['email'])) {
    $account = user_load_by_mail($values['email']);
  }
  if (!$account && !empty($values['pap_id'])) {

    $pap_id = $values['pap_id'];
    $uids = rtpr_pap_get_affiliate_uids(array($pap_id=>$pap_id));

    if (!empty($uids)) {
      $uid = reset($uids);
      $account = user_load($uid);
    }
  }

  $form_state['account'] = $account;
  if (!$account) {
    form_set_error('pap_id', t('No record found for PAP ID/Email entered!'));
    form_set_error('email', t('No record found for Email entered!'));
  }
}

function rtpr_reward_mrc_reassign_form_submit($form, &$form_state) {
  $account = $form_state['account'];
  rtpr_reward_mrc_reassign($account->uid);
}

function rtpr_reward_mrc_reassign($uid) {
  $rewards = db_query('SELECT * FROM {rtpr_reward} WHERE referral_uid = :uid', array(':uid' => $uid))->fetchAll();
  $account = user_load($uid);
  $referral_pap = rtpr_pap_get_affiliate_by_email($account->mail);
  $referral_pap_id = $referral_pap->getParentUserId();
  $referral_uid = rtpr_pap_get_uid_by_pap_id($referral_pap_id);

  if (!$referral_uid) {
    return;
  }
  $ba_rids = array(50, 51);
  foreach($rewards as $reward) {
    if ($reward->uid !== $referral_uid) {
      $timestamp = $reward->timestamp;
      $comment = $reward->comment;
      db_delete('rtpr_reward')
        ->condition('comment', $comment)
        ->condition('timestamp', $timestamp)
        ->execute();
      $old_parent = user_load($reward->uid);
      $parent_account = user_load($referral_uid);
      $parent_account_roles = $parent_account->roles;
      if (array_intersect($ba_rids, array_keys($parent_account_roles))) {
        $record = array(
          'uid' => $referral_uid,
          'email' => $parent_account->mail,
          'point' => 3,
          'timestamp' => $timestamp,
          'month' => date('m', $timestamp),
          'year' => date('Y', $timestamp),
          'type' => 'Earned',
          'comment' => $comment,
          'point_type' => 'MRC',
          'point_subtype' => 'PMRC',
          'referral_uid' => $account->uid,
        );
        rtpr_reward_insert($record);
      }
      $tmrc = _rtpr_reward_get_parent($parent_account, $comment, 'SMRC');
      if ($tmrc) {
        _rtpr_reward_get_parent($tmrc, $comment, 'TMRC');
      }
      drupal_set_message(t('Reassigned MRC credits from @old to @new', array('@old' => $old_parent->mail, '@new' => $parent_account->mail)));
    }
  }
}
