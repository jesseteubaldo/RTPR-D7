<?php

/**
 * @file
 * List of potential customers.
 */

function rtpr_user_potential_customer_form($form, &$form_state, $account) {
  // Filter.
  $form['options'] = array(
    '#type' => 'fieldset', 
    '#attributes' => array('class' => array('container-inline')), 
  );
  $form['options']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      '0' => t('Any'),

    ) + _rtpr_user_gcc_status_list(),
    '#default_value' => (isset($_GET['status']) && $_GET['status']) ? $_GET['status'] : 0,
  );
  $form['options']['entered_by'] = array(
    '#type' => 'select',
    '#title' => t('Entered By'),
    '#default_value' => (!empty($_GET['entered_by'])) ? $_GET['entered_by'] : 0,
    '#options' => array(
      0 => t('Any'),
      1 => t('Me'),
      2 => t('Virtual GC'),
    ),
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#name' => '',
    '#submit' => array('rtpr_user_potential_customer_list'),
  );

  $header = array(
    'name' => t('Name'),
    'email' => t('Email'),
    'city' => t('City'),
    'state' => t('State'),
    'phone' => t('Phone'),
    array('data' => 'GCS', 'field' => 'gcs',  'sort' => 'ASC'), 
    array('data' => 'GCA', 'field' => 'gca',  'sort' => 'ASC'), 
    'status' => t('Status'),
    'submitted' => t('Entered By')
  );
  $rows = array();
  if (isset($form_state['status'])){
    $status = $form_state['status'];
  }

  if (isset($_GET['status']) && $_GET['status']){
    $status = $_GET['status'];
  }

  $account_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  $gcc_users = db_select('rtpr_gcc', 't')
    ->fields('t', array('uid', 'type'))
    ->condition('parent_uid', $account->uid)
    ->execute()->fetchAllAssoc('uid');
  $uids = array_keys($gcc_users);
  $status_counter = array(
    'GCS' => 0,
    'GCA' => 0,
    'Converted' => 0,
    'GCS you should work' => 0,
    'Unsubscribed' => 0,
    'Hard Undeliverable' => 0,
    'Soft Undeliverable' => 0,
  );
  module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
  module_load_include('inc','rtpr_cc','includes/rtpr_cc.gcc');
  $first_day_of_this_month_timestamp = strtotime('first day of this month 00:00:00');
  $last_day_of_this_month_timestamp = strtotime('last day of this month 23:59:59');
  $first_day_of_last_month_timestamp = strtotime('first day of last month 00:00:00');
  $last_day_of_last_month_timestamp = strtotime('last day of last month 23:59:59');
  $this_month_user_data = new PAPUserData($pap_id, date('m', $first_day_of_this_month_timestamp), date('Y', $first_day_of_this_month_timestamp));
  $last_month_user_data = new PAPUserData($pap_id, date('m', $first_day_of_last_month_timestamp), date('Y', $first_day_of_last_month_timestamp));


  $this_month = array(
    'GCS' => 0,
    'GCA' => 0,

    'APV' => commerce_currency_format($this_month_user_data->getTotalAPV() * 100, 'USD'),
  );
  $last_month = array(
    'GCS' => 0,
    'GCA' => 0,

    'APV' => commerce_currency_format($last_month_user_data->getTotalAPV() * 100, 'USD'),
  );

  $submitted_by_me = FALSE;
  $submitted_by_vgcc = FALSE;
  $gcc_query = db_select('rtpr_gcc', 'c')
    ->fields('c', array('uid', 'parent_uid'))
    ->condition('parent_uid', $account->uid);
  if (!empty($_GET['entered_by'])) {
    switch($_GET['entered_by']) {
      case 1:
        $gcc_query->condition('submitted_uid', $account->uid);
        $submitted_by_me = TRUE;
        break;
      case 2:
        $gcc_query->condition('type', 'VGCC');
        $submitted_by_vgcc = TRUE;
        break;
    }
    
  }
  $submitted_by = $gcc_query->execute()->fetchAllAssoc('uid');

  foreach($uids as $uid) {
    $customer = user_load($uid);
    if($customer) {
      $customer_wrapper = entity_metadata_wrapper('user', $customer);
      $name = $customer_wrapper->field_first_name->value() . ' ' . $customer_wrapper->field_last_name->value();
      $address = $customer_wrapper->field_address->value();
      
      $status_index = _rtpr_user_get_gcc_user_status($customer);
      
      $status_text = _rtpr_user_gcc_status_list($status_index);
      $gca = _rtpr_cc_get_gca_date($uid);
 
      $status_counter[$status_text]++;
      if ($customer->created >= $first_day_of_this_month_timestamp) {
        $this_month['GCS']++;
      }
      if ($gca >= $first_day_of_this_month_timestamp && $gca <= $last_day_of_this_month_timestamp && $customer->init != 'unsubscribe') {
        $this_month['GCA']++;
      }
      if ($customer->created >= $first_day_of_last_month_timestamp && $customer->created <= $last_day_of_last_month_timestamp) {
        $last_month['GCS']++;
      }
      if ($gca >= $first_day_of_last_month_timestamp && $gca <= $last_day_of_last_month_timestamp && $customer->init != 'unsubscribe'){
         $last_month['GCA']++;
      }
      $entered_by = 'RTPR';
      if ((isset($submitted_by[$uid]) && $submitted_by[$uid] == $account->uid) || $submitted_by_me) {
        $entered_by = 'ME';
      }
      if (!empty($gcc_users[$uid]->type) && $gcc_users[$uid]->type == 'VGCC') {
        $entered_by = 'Virtual GC';
      } 

      
      $rows[$uid] = array(
        'name' => $name,
        'email' => $status_index != 5 ? $customer->mail : $status_text,
        'city' => $address['locality'],
        'state' => $address['administrative_area'],
        'phone' => $customer_wrapper->field_phone->value(),
        'gcs' => date('m/d/Y', $customer->created),
        'gca' => $gca ? date('m/d/Y', $gca) : 'N/A',
        'status' => $status_text,
        'submitted' => $entered_by,
      );
      $download_rows[$uid] = array(
        'first' => $customer_wrapper->field_first_name->value(),
        'last' => $customer_wrapper->field_last_name->value(),
        'email' =>  $rows[$uid]['email'],
        'address' => $address['thoroughfare'],
        'address2' => $address['premise'],
        'city' => $rows[$uid]['city'],
        'state' => $rows[$uid]['state'],
        'zipcode' => $address['postal_code'],
        'phone' => $rows[$uid]['phone'],
        'gcs' => $rows[$uid]['gcs'],
        'gca' => $rows[$uid]['gca'],
        'status' => $rows[$uid]['status'],
        'submitted' => $entered_by,
      );

      $entered = ($submitted_by_me || $submitted_by_vgcc) && !in_array($uid, array_keys($submitted_by));
      if ((isset($status) && $status != $status_index) || $entered) {
        unset($rows[$uid]);
        unset($download_rows[$uid]);
      }
    }
  }


  $status_counter['GCS'] += $status_counter['GCA']  + $status_counter['Converted'] + $status_counter['GCS you should work'] + $status_counter['Unsubscribed'];
  $status_counter['GCA'] += $status_counter['Converted'];
  unset($status_counter['Converted']);
  unset($status_counter['Unsubscribed']);
  
  if ($rows) {
    
    $form_state['rows'] = $download_rows;
    $form_state['header'] = array(
      'first' => t('First Name'),
      'last' => t('Last Name'),
      'email' => t('Email'),
      'address' => t('Address'),
      'address2' => t('Address 2'),
      'city' => t('City'),
      'state' => t('State'),
      'zipcode' => t('Zip Code'),
      'phone' => t('Phone'),
      'gcs' => t('GCS'),
      'gca' => t('GCA'),
      'status' => t('Status'),
      'submitted' => t('Entered By')
    );
    $form['options']['download'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      '#submit' => array('rtpr_user_potential_customer_download'),
    );

    if (isset($_GET['op']) && $_GET['op'] == 'Download') {
      rtpr_user_potential_customer_download($form, $form_state);
    }
  }

  $summary_markup = '<h4>Summary</h4><hr/><div class="row"><div class="col-md-4"><strong>This Month\'s Stat</strong><ul>';

  foreach($this_month as $key => $total) {
    $summary_markup .= '<li>' . $key . ' : ' . $total;
  }
  $summary_markup .= '</ul></div><div class="col-md-4"><strong>Last Month\'s Stat</strong><ul>';
  foreach($last_month as $key => $total) {
    $summary_markup .= '<li>' . $key . ' : ' . $total;
  }
$summary_markup .= '</ul></div><div class="col-md-4"><strong>TOTAL Gift Card Customer Stats</strong><ul>';
    foreach($status_counter as $key => $total) {
    $summary_markup .= '<li>' . $key . ' : ' . $total;
  }
  $summary_markup .= '</ul></div></div><br/>';
  $form['summary'] = array(
    '#type' => 'markup',
    '#markup' => $summary_markup,
    '#weight' => -1,
  );
  
  uasort($rows, '_rtpr_user_sort_gcc');

  $per_page = 50;
  $current_page = pager_default_initialize(count($rows), $per_page);
  // split array into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $chunks[$current_page],
    '#empty' => t('No GCC customer available.'),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );
  $form['#method'] = 'get';
  $form['#pre_render'][] = 'rtpr_user_potential_customer_form_modify';

  return $form;
}

function rtpr_user_potential_customer_form_modify($form){
  unset($form['form_token']);
  unset($form['form_build_id']);
  unset($form['form_id']);
  return $form;
}

/**
 * Helper function to sort GCA and GCS.
 */
function _rtpr_user_sort_gcc($a, $b) {
  if (!isset($_GET['order'])) {
    return;
  }
  $sort_by = strtolower($_GET['order']);
  $sort_order = strtolower($_GET['sort']);
  if($a[$sort_by] == $b[$sort_by]) {
      return 0;
  }
  if ($sort_order == 'asc') {
    return (strtotime($a[$sort_by]) > strtotime($b[$sort_by])) ? 1 : -1;
  }
  return (strtotime($a[$sort_by]) > strtotime($b[$sort_by])) ? -1 : 1;
}

function _rtpr_user_gcc_status_list($index = NULL) {
  
  $list =  array(
    1 => 'GCA',
    2 => 'Converted',
    3 => 'GCS',
    4 => 'GCS you should work',
    5 => 'Unsubscribed',
    6 => 'Hard Undeliverable',
    7 => 'Soft Undeliverable'
  );
  if ($index) {
    return $list[$index];
  }
  return $list;
}
function _rtpr_user_get_gcc_user_status($customer) {
  $status = 3;

  if ($customer->init == 'unsubscribe') {
    return 5;
  }
  
  if ($customer->init == 'hard') {
    return 6;
  }

  if ($customer->init == 'soft') {
    return 7;
  }

  if (user_has_role(7, $customer)) {
    $status = 1;
    $order = db_select('commerce_order', 'o')
      ->fields('o')
      ->condition('status', array('pending', 'processing', 'completed'), 'IN')
      ->condition('uid', $customer->uid)
      ->execute()->fetchObject();
    if ($order) {
      $status = 2;
    }
  }
  elseif ($customer->created < strtotime('-10 days')) {
    return 4;
  }

  return $status;
}

function rtpr_user_potential_customer_list($form, &$form_state) {
  $form_state['me'] = $form_state['values']['me'];
  if(is_numeric($form_state['values']['status'])) {
    $form_state['status'] = $form_state['values']['status'];
  } else {
    unset($form_state['status']);
  }
  
  $form_state['rebuild'] = TRUE;
}

function rtpr_user_potential_customer_download($form, &$form_state) {
  $header = $form_state['header'];
  $rows = $form_state['rows'];
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=gcc-report.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();  
}
