<?php

/**
* @file
* 
* Reports for userpoints.
*/

function rtpr_reward_userpoints_report_form($form, $form_state, $rid) {  
  
  $form['rid'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#options' => array(
      7 => 'Retail Customer',
      4 => 'Affiliate',
      38 => 'RCP',
      47 => 'Rewards',
    ),
    '#required' => TRUE,
    '#default_value' => 7,
  );
    
  if ($rid == RTPR_REWARD_REWARDS_RID) {
    $form['options'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Date Range'), 
      '#attributes' => array('class' => array('container-inline')), 
    );

    $form['options']['start_date'] = array(
      '#type' => 'date_popup',
      '#date_format' => 'm-d-Y',
      '#title' => t('to'),
      '#date_label_position' => 'invisible',
      '#date_year_range' => '-2:+0',
      '#default_value' => date('Y-m-d H:i:s', strtotime('first day of last month')),
    );
    $form['options']['end_date'] = array(
      '#type' => 'date_popup',
      '#date_format' => 'm-d-Y',
      '#date_label_position' => 'invisible',
      '#date_year_range' => '-2:+0',
      '#default_value' => date('Y-m-d H:i:s', strtotime('last day of last month')),
    );

    $form['submit'] = array(
     '#type' => 'submit',
     '#value' => t('Download Rewards Report'),
     '#submit' => array('rtpr_reward_userpoints_rewards_report'),
    );
  } else {
    $form['submit'] = array(
     '#type' => 'submit',
     '#value' => t('Download Customers Report'),
     '#submit' => array('rtpr_reward_userpoints_customers_report'),
    );
  }



  return $form;
}

function rtpr_reward_userpoints_rewards_report($form, $form_state) {
  $start_timestamp = NULL;
  $end_timestamp = NULL;
  $filter_by_date = FALSE;
  if (isset($form_state['values']['start_date']) && isset($form_state['values']['start_date'])) {
    $start_timestamp = strtotime($form_state['values']['start_date'] . ' 00:00:00');
    $end_timestamp = strtotime($form_state['values']['end_date'] . ' 23:59:59');
    $filter_by_date = TRUE;
  }
  
  $rewards_uids = db_select('users_roles', 'r')
    ->fields('r', array('uid'))
    ->condition('rid', 47)
    ->execute()->fetchAllAssoc('uid');
  
  $query  = db_select('userpoints' ,'up');
  $query->join('users', 'u', 'u.uid = up.uid');
  $query->leftJoin('field_data_field_pap_affiliate_id', 'p', 'p.entity_id = up.uid');
  $query->leftJoin('field_data_field_first_name', 'f', 'f.entity_id = up.uid');
  $query->leftJoin('field_data_field_last_name', 'l', 'l.entity_id = up.uid');
  $query->fields('up', array('uid'))
   ->condition('tid', 0);
  $query->fields('u', array('mail'));
  $query->fields('p', array('field_pap_affiliate_id_value'));
  $query->fields('f', array('field_first_name_value'));
  $query->fields('l', array('field_last_name_value'));
  
  $accounts = $query->execute()->fetchAllAssoc('uid');
  
  $batch = array(
    'operations' => array(),
    'finished' => 'rtpr_reward_userpoints_reward_report_finished', // runs after batch is finished
    'title' => t('Processing data'),
    'init_message' => t('Export is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Export has encountered an error.'),
  );

  $accounts = array_intersect_key($accounts, $rewards_uids);

  $rows = array();
  foreach($accounts as $index => $account) {
    $batch['operations'][] = array('rtpr_reward_userpoints_reward_report_process', array($account, $start_timestamp, $end_timestamp));
  }

  batch_set($batch);
  batch_process('admin/config/people/userpoints/rewards-report');
}

function rtpr_reward_userpoints_reward_report_process($account, $start_timestamp, $end_timestamp, &$context) {
  $uid = $account->uid;
  $credit = rtpr_reward_get_userpoints_txn($uid);
  $redeemed = rtpr_reward_get_userpoints_txn($uid, RTPR_COMMERCE_REDEEMED_TID, 0);
  
  $credit_total = isset($credit->total) ? $credit->total : 0;
  $redeemed_total = isset($redeemed->total) ? abs($redeemed->total) : 0;
  $available = $credit_total - $redeemed_total;

  $context['results'][$uid] = array(
   $uid,
   $account->field_pap_affiliate_id_value,
   $account->field_first_name_value,
   $account->field_last_name_value,
   $account->mail,
   $credit_total,
   $redeemed_total,
   $available
  );

  $credit = rtpr_reward_get_userpoints_txn($uid, 0, 0, $start_timestamp, $end_timestamp);
  $redeemed = rtpr_reward_get_userpoints_txn($uid, RTPR_COMMERCE_REDEEMED_TID, 0, $start_timestamp, $end_timestamp);
  
  $credit_total = isset($credit->total) ? $credit->total : 0;
  $redeemed_total = isset($redeemed->total) ? abs($redeemed->total) : 0;
  $context['results'][$uid][] = $credit_total;
  $context['results'][$uid][] = $redeemed_total;
  $context['results'][$uid][] = date('m/d/Y', $start_timestamp);
  $context['results'][$uid][] = date('m/d/Y', $end_timestamp);

}

function rtpr_reward_userpoints_reward_report_finished($success, $rows, $operations) {
  $header['pap_id'] = t('PAP Affiliate ID');
  $header['earned_filtered'] = t('Total Points Earned During This Period');
  $header['redeemed_filtered'] = t('Total Points Redeemed During This Period');
  $header['start'] = t('Start');
  $header['end'] = t('End');

  rtpr_reward_userpoints_generate_report($rows, $header, 'reward');
}

function rtpr_reward_userpoints_customers_report($form, $form_state) {
  
  $rid = $form_state['values']['rid'];
  $retail_uids = db_select('users_roles', 'r')
    ->fields('r', array('uid'))
    ->condition('rid', $rid)
    ->execute()->fetchAllAssoc('uid');
  
  $query  = db_select('userpoints' ,'up');
  $query->join('users', 'u', 'u.uid = up.uid');
  $query->leftJoin('field_data_field_pap_referral_id', 'p', 'p.entity_id = up.uid');
  $query->leftJoin('field_data_field_first_name', 'f', 'f.entity_id = up.uid');
  $query->leftJoin('field_data_field_last_name', 'l', 'l.entity_id = up.uid');
  $query->fields('up', array('uid'))
   ->condition('tid', 0);
  $query->fields('u', array('mail'));
  $query->fields('p', array('field_pap_referral_id_value'));
  $query->fields('f', array('field_first_name_value'));
  $query->fields('l', array('field_last_name_value'));
  
  $accounts = $query->execute()->fetchAllAssoc('uid');

  $accounts = array_intersect_key($accounts, $retail_uids);
  $batch = array(
    'operations' => array(),
    'finished' => 'rtpr_reward_userpoints_customers_report_finished', // runs after batch is finished
    'title' => t('Processing data'),
    'init_message' => t('Export is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Export has encountered an error.'),
  );

  $rows = array();
  foreach($accounts as $index => $account) {
    $batch['operations'][] = array('rtpr_reward_userpoints_customers_report_process', array($account));
  }

  batch_set($batch);
  batch_process('admin/config/people/userpoints/customer-report');
}

function rtpr_reward_userpoints_customers_report_process($account, &$context) {

  $uid = $account->uid;
  $credit = rtpr_reward_get_userpoints_txn($uid);
  $redeemed = rtpr_reward_get_userpoints_txn($uid, RTPR_COMMERCE_REDEEMED_TID, 0);
  
  $credit_total = isset($credit->total) ? $credit->total : 0;
  $redeemed_total = isset($redeemed->total) ? abs($redeemed->total) : 0;
  $available = $credit_total - $redeemed_total;

  $context['results'][$uid] = array(
   $uid,
   $account->field_pap_referral_id_value,
   $account->field_first_name_value,
   $account->field_last_name_value,
   $account->mail,
   $credit_total,
   $redeemed_total,
   $available
  );

  $context['message'] = 'Now processing ' . $uid;
}

function rtpr_reward_userpoints_customers_report_finished($success, $rows, $operations) {
  rtpr_reward_userpoints_generate_report($rows, array('pap_id' => t('PAP Referral ID')), 'customer');
}

function rtpr_reward_userpoints_generate_report($rows, $header = array(), $type) {
  $local_header = array(
   'uid' => 'User ID',
   'pap_id' => 'PAP ID',
    'first' => t('First Name'),
   'last' => t('Last Name'),
   'mail' => 'Email',
   'earned' => 'Total Points Earned',
   'redeemed' => 'Total Points Redeemed',
   'available' => 'Total Points Available',
  );
  
  $header = array_merge($local_header, $header);
  $file_path = 'public://private/' . time() . '_userpoints_' . $type . '_report.csv';
  $fp = fopen($file_path, 'w');

  fputcsv($fp, $header);

  foreach ($rows as $row) {
    fputcsv($fp, $row);
  }

  fclose($fp);
  $loglink = l("here", file_create_url($file_path));
  $msg = "Click $loglink to download the report.";
  drupal_set_message($msg);
}
