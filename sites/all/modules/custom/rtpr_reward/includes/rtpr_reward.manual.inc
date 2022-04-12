<?php
  
/**
 * @file
 *
 * Allow admin/staff to manually add/substract point for user
 */

function rtpr_reward_manual_entry_form($form, $form_state) {

  drupal_set_title(t('RTPR Points - Manual entry'));
  if (isset($_GET['id'])) {
    $reward = rtpr_reward_get_reward_data($_GET['id']);
  }
  $form['reward_id'] = array(
    '#title' => t('ID'),
    '#type' => 'textfield',
    '#default_value' => isset($reward) ? $reward['id'] : NULL,
    '#disabled' => TRUE,
  );
  
  $form['referral_uid'] = array(
    '#title' => t('Referred'),
    '#type' => 'textfield',
    '#default_value' => isset($reward) ? $reward['referral_uid'] : 0,
    '#states' => array(
      'visible' => array(
        ':input[name="group"]' => array('value' => 'MRC'),
      ),
    ),
  );
  
  $form['group'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => rtpr_reward_point_types() + array('MRC' => t('PMRC')),
    '#default_value' => isset($reward) ? $reward['point_type'] : 'reward',
  );
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#autocomplete_path' => 'rtprcc/autocomplete',
    '#required' => TRUE,
     '#default_value' => isset($reward) ? $reward['email'] : NULL,
  );
  
  $form['options'] = array(
    '#title'=> t('Options'),
    '#type' => 'fieldset',
    '#attributes' => array(
        'class' => array('container-inline'),
    ),
  );
  $form['options']['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => rtpr_reward_month_list(false),
    '#default_value' => isset($reward) ? $reward['month'] : date('n'),
  );
  
  $form['options']['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => rtpr_reward_year_list(false),
    '#default_value' => isset($reward) ? $reward['year'] : date('Y'),
  );
  
  $form['options']['point'] = array(
    '#title' => t('Point'),
    '#type' => 'textfield',
    '#default_value' => isset($reward) ? $reward['point'] : NULL,
  );
  
  $form['options']['type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' =>  rtpr_reward_reward_types(),
    '#default_value' => isset($reward) ? strtolower($reward['type']) : 'earned',
    '#required' => TRUE,
  );
  
  $form['comment'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#required' => TRUE,
    '#default_value' => isset($reward) ? $reward['comment'] : NULL,
  );
  
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($reward) ? t('Update') : t('Submit'),
  );
  
  return $form;
}

function rtpr_reward_manual_entry_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $points = $values['point'];
  $email = $values['email'];

  if ($values['group'] != 'mrc') {
    if (!is_numeric($points)) {
      form_set_error('point', t('Invalid point!'));
    }
  }

  $account = user_load_by_mail($email);
  if (!$account) {
    form_set_error('email', t('User @email does not exist!', array('@email' => $email)));
  } else {
    if ($values['group'] == 'mrc') {
      $referral_pap = rtpr_pap_get_affiliate_by_email($email);
      $form_state['values']['referral_pap'] = $referral_pap;
    }
    $form_state['values']['uid'] = $account->uid;
  }
  
}
function rtpr_reward_manual_entry_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $point_subtype = NULL;
  if ($values['group'] == 'mrc') {
    $point_subtype = 'PMRC';
  }
  
  if ($point_subtype != 'PMRC') {
    $record = array(
      'id' => $values['reward_id'],
      'email' => $values['email'],
      'uid' => $values['uid'],
      'month' => $values['month'],
      'year' => $values['year'],
      'point' => $values['point'],
      'comment' => $values['comment'],
      'type' => ucfirst($values['type']),
      'timestamp' => REQUEST_TIME,
      'point_type' => $values['group'],
      'point_subtype' => $point_subtype,
    );
    
    if (!empty($values['referral_uid'])) {
      $record['referral_uid'] = $values['referral_uid'];
    }
    else {
      $record['referral_uid'] = NULL;
    }
    rtpr_reward_insert($record);
  }
  else {
    $ba_rids = array(50, 51);
    $referral_pap = $values['referral_pap'];
    $comment = $values['comment'];
    if ($referral_pap) {
      $referral_pap_id = $referral_pap->getParentUserId();
      $referral_uid = rtpr_pap_get_uid_by_pap_id($referral_pap_id);

      if ($referral_uid) {
        $parent_account = user_load($referral_uid);
        $parent_account_roles = $parent_account->roles;
        if (array_intersect($ba_rids, array_keys($parent_account_roles))) {
          
          $subtype = 'PMRC';
          $record = array(
            'uid' => $values['uid'],
            'email' => $values['email'],
            'point' => 3,
            'timestamp' => REQUEST_TIME,
            'month' => $values['month'],
            'year' => $values['year'],
            'type' => 'Earned',
            'comment' => $comment,
            'point_type' => 'MRC',
            'point_subtype' => 'PMRC',
            'manual' => 1,
            'referral_uid' =>  $values['uid'],
    
          );
          $rid = db_query('SELECT id FROM {rtpr_reward} WHERE comment = :comment AND point_subtype = :subtype', array(':comment' => $comment, ':subtype' => $subtype))->fetchField();
          if (!$rid) {
            rtpr_reward_insert($record);
          }
        }
        
        $tmrc = _rtpr_reward_get_parent(user_load($values['uid']), $comment, 'SMRC');
        if ($tmrc) {
          _rtpr_reward_get_parent($tmrc, $comment, 'TMRC');
        }

      
      }
    }
  }

  drupal_set_message(t('Successfully update point for user @email', array('@email' => $values['email'])));
}

function rtpr_reward_get_reward_data($id) {
  $query = db_select('rtpr_reward', 'r')
    ->fields('r')
    ->condition('id', $id)
    ->range(0, 1);
  
  return $query->execute()->fetchAssoc();
}

function rtpr_reward_manual_delete_form($form, &$form_state, $id) {

  $form['message'] = array(
    '#markup' => '<p>Are you sure you want to delete this record? This action cannot be undone.</p>'
  );
  
  $form_state['reward_id'] = $id;
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete')
  );
  
  return $form;
}

function rtpr_reward_manual_delete_form_submit($form, $form_state) {
  $reward_id = $form_state['reward_id'];
  db_delete('rtpr_reward')
    ->condition('id', $reward_id)
    ->execute();
  drupal_set_message(t('Record @id has been deleted.', array('@id' => $reward_id)));
  drupal_goto('admin/config/rtpr/points/history');
}