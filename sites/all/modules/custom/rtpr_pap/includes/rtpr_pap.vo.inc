<?php

/**
 * @file
 * Vendorn-Owner related features.
 */


/**
 * Callback for admin/config/rtpr/pap/vo-report.
 */
function rtpr_pap_vo_report_form($form, $form_state) {
  module_load_include('inc', 'rtpr_pap', 'includes/helper');
  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );

  $form['options']['year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => _rtpr_pap_get_year_options(2013),
    '#default_value' => date('Y'),
    '#required' => TRUE,
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

function rtpr_pap_vo_report_form_submit($form, $form_state) {
  $year = $form_state['values']['year'];
  
  db_set_active('pap');
  $start_timestamp = $year . '-12-31 23:59:59';
  $user_ids = db_select('qu_pap_users', 'pu')
    ->fields('pu', array('userid'))
    ->condition('dateinserted', $start_timestamp, '<')
    ->execute()->fetchCol();
  db_set_active();
  
  if (!empty($user_ids)) {
    $query = db_select('field_data_field_pap_affiliate_id', 'p');
    $query->join('users_roles', 'r', 'r.uid = p.entity_id');
    $query->fields('p', array('entity_id'))
      ->condition('field_pap_affiliate_id_value', $user_ids, 'IN')
      ->condition('rid', 36);
    $uids = $query->execute()->fetchCol();
  }

  batch_set(rtpr_pap_vo_report_batch($uids, $year));
}

function rtpr_pap_vo_report_batch($uids, $year) {

  foreach ($uids as $uid) {
    $operations[] = array(
      'rtpr_pap_vo_report_batch_op',
      array(
        $uid,
        $year,
      ),
    );
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'rtpr_pap_vo_report_batch_op_finished',
    'file' => drupal_get_path('module', 'rtpr_pap') . '/includes/rtpr_pap.vo.inc', // Required.
  );

  return $batch;
}

function rtpr_pap_vo_report_batch_op($uid, $year, &$context) {
  module_load_include('inc', 'rtpr_pap', 'includes/helper');
  module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
  $account = user_load($uid);
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $email = $account->mail;
  $name = $account_wrapper->field_first_name->value() . ' ' . $account_wrapper->field_last_name->value();
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  $rows = array();
  $rows['pap_id'] = $pap_id;
  $rows['email'] = $email;
  $rows['name'] = $name;
  $rows['year'] = $year;

  foreach (_rtpr_pap_get_month_options() as $month_number => $month_text) {
    $user_data = new PAPUserData($pap_id, $month_number, $year, $account, $account_wrapper);
    $month_name = strtolower($month_text);
    $rows[$month_name . '_msp'] = '$' . number_format($user_data->getVolumeTotal('msp'), 2);
    $rows[$month_name . '_apv'] = '$' . number_format($user_data->getTotalAPV($pap_id), 2);
  }

  $context['message'] = t('Processing user %$uid', array('%$uid' => $uid));
  $context['results'][$uid] = $rows;
}

function rtpr_pap_vo_report_batch_op_finished($success, $rows, $operations) {
  if ($success) {
      $header = array(
        'pap_id' => t('PAP ID'),
        'email' => t('Email'),
        'name' => t('Name'),
        'year' => t('Year'),
        'january_msp' => t('January MSP'),
        'january_apv' => t('January APV'),
        'february_msp' => t('February MSP'),
        'february_apv' => t('February APV'),
        'march_msp' => t('March MSP'),
        'march_apv' => t('March APV'),
        'april_msp' => t('April MSP'),
        'april_apv' => t('April APV'),
        'may_msp' => t('May MSP'),
        'may_apv' => t('May APV'),
        'june_msp' => t('June MSP'),
        'june_apv' => t('June APV'),
        'july_msp' => t('July MSP'),
        'july_apv' => t('July APV'),
        'august_msp' => t('August MSP'),
        'august_apv' => t('August APV'),
        'september_msp' => t('September MSP'),
        'september_apv' => t('Seotember APV'),
        'october_msp' => t('October MSP'),
        'october_apv' => t('October APV'),
        'november_msp' => t('November MSP'),
        'november_apv' => t('November APV'),
        'december_msp' => t('December MSP'),
        'december_apv' => t('December APV'),
      );
    
      $file_path = 'public://private/' . date('m_d_Y', time()) . '_vo_report.csv';
      $fp = fopen($file_path, 'w');
    
      fputcsv($fp, $header);
    
      foreach ($rows as $row) {
        fputcsv($fp, $row);
      }
    
      fclose($fp);
      $loglink = l("here", file_create_url($file_path));
      $msg = "Click $loglink to download the report.";
      drupal_set_message($msg);
  } else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    watchdog('rtpr_cc', t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }

}