<?php

/**
 * @file
 * Report relate to MRC.
 */

function rtpr_reward_mrc_report_form($form, &$form_state) {
  global $user;
  $data = &drupal_static(__FUNCTION__);
  $is_cached = FALSE;
  if (!isset($data)) {
    $cache = cache_get('rtpr_reward_mrc_data_3');
    if ($cache->expire > REQUEST_TIME) {
      $is_cached = TRUE;
      $data = $cache->data;
    }
  }
  $per_page = 50;
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download MRC report'),
    '#submit' => array('rtpr_reward_current_mrc_report_form_submit'),
  );
  $header = array(
    'uid' => t('UID'),
    'pap_id' => t('PAP ID'),
    'role' => t('Role'),
    'email' => t('Email'),
    'mrc_earned' => t('MRC Earned'),
    'mrc_manual' => t('MRC Manually Added'),
    'total_pmrc' => t('Total PMRC'),
    'total_smrc' => t('Total SMRC'),
    'total_tmrc' => t('Total TMRC'),
    'total_mrc_earned' => t('Total MRC Earned'),
    'total_mrc_redeemed' => t('Total MRC Redeemed'),
    'total_mrc_remaining' => t('Total MRC Remaining'),
  );
  
  $rows = array();

  if (!isset($data['mrc'])) {
    $query = db_select('rtpr_reward', 'rr')
      ->fields('rr', array('uid'))
      ->condition('point_type', 'mrc')
      ->extend('PagerDefault')
      ->limit(30);

    $uids = $query->execute()->fetchCol();

    $account_data = array();
    if ($uids) {
      $rows = _rtpr_reward_current_mrc_rows($uids);  
    }
  }
  $form_state['header'] = $header;
  $form_state['rows'] = $rows;

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No MRC data available.'),
  );
  
  $form['pager'] = array(
    '#theme' => 'pager',
  );

  return $form;
}

/**
 * Helper function to get current MRC.
 */
function _rtpr_reward_current_mrc_rows($uids) {
  $rows = array();
  $uids = array_unique($uids);
  $accounts = user_load_multiple($uids);
  foreach($accounts as $account) {
    $uid = $account->uid;
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $mrc_earned = rtpr_reward_points_earned($uid, 'mrc');
    $mrc_manual = rtpr_reward_points_earned($uid, 'mrc', 1);
    $pmrc_count = db_query('SELECT sum(point) as total FROM rtpr_reward where uid = :uid and point_subtype = :type', array(':uid' => $uid, ':type' => 'PMRC'))->fetchField();
    $smrc_count = db_query('SELECT sum(point) as total FROM rtpr_reward where uid = :uid and point_subtype = :type', array(':uid' => $uid, ':type' => 'SMRC'))->fetchField();
    $tmrc_count = db_query('SELECT sum(point) as total FROM rtpr_reward where uid = :uid and point_subtype = :type', array(':uid' => $uid, ':type' => 'TMRC'))->fetchField();
    $total_mrc_earned = $mrc_earned + $mrc_manual;
    $total_mrc_redeemed = abs(rtpr_reward_points_used($uid, 'mrc'));
    $account_data[$uid] = array(
      'mail' => $account->mail,
      'pap_id' => $account_wrapper->field_pap_affiliate_id->value(),
    );
    $rows[] = array(
      'uid' => $uid,
      'pap_id' => $account_wrapper->field_pap_affiliate_id->value(),
      'role' => rtpr_user_get_primary_role($account->roles),
      'email' => $account->mail,
      'mrc_earned' => $mrc_earned,
      'mrc_manual' => $mrc_manual,
      'total_pmrc' => $pmrc_count ? $pmrc_count : 0,
      'total_smrc' => $smrc_count ? $smrc_count : 0,
      'total_tmrc' => $tmrc_count ? $tmrc_count : 0,
      'total_mrc_earned' => $total_mrc_earned,
      'total_mrc_redeemed' => $total_mrc_redeemed,
      'total_mrc_remaining' => $total_mrc_earned - $total_mrc_redeemed,
    );
  }
  
  return $rows;
}

/**
 * Helper function to get past MRC.
 */
function _rtpr_reward_past_mrc_rows($rewards) {
  $rows = array();
  foreach($rewards as $reward) {
    $uid = $reward->uid;
    $ref_uid = $reward->referral_uid;
    $ref_account = user_load($ref_uid);
    $account = user_load($uid);
    if ($ref_account) {
      try {
        $ref_wrapper = entity_metadata_wrapper('user', $ref_account);
        $account_data[$ref_uid] = array(
          'mail' => $ref_account->mail,
          'pap_id' => $ref_wrapper->field_pap_affiliate_id->value(),
        );
      }
      catch (EntityMetadataWrapperException $exc) {
        watchdog(
          'rtpr_extras',
          'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
          array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
          WATCHDOG_ERROR
        );
      }
    }
    if ($account) {
      try {
        $account_wrapper = entity_metadata_wrapper('user', $account);
        $account_data[$uid] = array(
          'mail' => $ref_account->mail,
          'pap_id' => $ref_wrapper->field_pap_affiliate_id->value(),
        );
      }
      catch (EntityMetadataWrapperException $exc) {
        watchdog(
          'rtpr_extras',
          'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
          array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
          WATCHDOG_ERROR
        );
      }
    }

    $rows[] = array(
      'uid' => $uid,
      'email' => $account_data[$uid]['mail'],
      'pap_id' => $account_data[$uid]['pap_id'],
      'point' => $reward->point,
      'type' => $reward->point_subtype,
      'order' => (int) filter_var($reward->comment, FILTER_SANITIZE_NUMBER_INT),
      'referral_uid' => $ref_uid,
      'referral_pap' => $account_data[$ref_uid]['pap_id'],
      'referral_email' => $account_data[$ref_uid]['mail'],
      'date' => date('m/d/Y', $reward->timestamp)
    );
  }
  return $rows;
}
function rtpr_reward_past_mrc_report_form($form, &$form_state) {
  global $user;

  $rows = array();
  $query = db_select('rtpr_reward', 'rr')
  ->fields('rr')
  ->condition('point_type', 'mrc')
  ->extend('PagerDefault')
  ->limit(30);

  $rewards = $query->execute()->fetchAll();
  if ($rewards) {
    $rows = _rtpr_reward_past_mrc_rows($rewards);  
  }

  $past_header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'pap_id' => t('PAP ID'),
    'point' => t('Point'),
    'type' => t('Type'),
    'order' => t('Order ID'),
    'referral_uid' => t('Referrer UID'),
    'referral_pap' => t('Referrer PAP'),
    'referral_email' => t('Referrer Email'),
    'date' => t('Date')
  );

  $form['download_past'] = array(
    '#type' => 'submit',
    '#value' => t('Download Past MRC report'),
    '#submit' => array('rtpr_reward_past_mrc_report_form_submit'),
  );

  $form['data'] = array(
    '#theme' => 'table',
    '#header' => $past_header,
    '#rows' =>   $rows,
    '#empty' => t('No MRC data available.'),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );

  $form_state['mrc_header'] = $past_header;


  return $form;
}
function rtpr_reward_past_mrc_report_form_submit($form, &$form_state) {
  $header = $form_state['mrc_header'];
  $rows = array();
  $query = db_select('rtpr_reward', 'rr')
  ->fields('rr')
  ->condition('point_type', 'mrc');

  $rewards = $query->execute()->fetchAll();
  if ($rewards) {
    $rows = _rtpr_reward_past_mrc_rows($rewards);  
  }
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=past-mrc-' . date('m-d-Y') . '.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();

}
function rtpr_reward_current_mrc_report_form_submit($form, &$form_state) {
  $header = $form_state['header'];
  $rows = array();
  
  $query = db_select('rtpr_reward', 'rr')
  ->fields('rr', array('uid'))
  ->condition('point_type', 'mrc');

  $uids = $query->execute()->fetchCol();
  $rows = _rtpr_reward_current_mrc_rows($uids); 
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=mrc-' . date('m-d-Y') . '.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();
}