<?php

/**
 * GCC Report.
 */

function rtpr_pap_gcc_report_form($form, $form_state) {
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
    '#options' => _rtpr_pap_get_year_options(2015),
    '#default_value' => date('Y'),
    '#required' => TRUE,
  );

  $form['options']['month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => _rtpr_pap_get_month_options(),
    '#default_value' => date('m'),
    '#required' => TRUE,
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

function rtpr_pap_gcc_report_form_submit($form, &$form_state) {
  $parent_uids = db_query('SELECT DISTINCT parent_uid FROM {rtpr_gcc} WHERE parent_uid NOT IN (48001,47868,48003,327134,38699,76817,330845,38679)')->fetchCol();
  $vendors = array();
  foreach($parent_uids as $uid) {
    $uids = db_query('SELECT uid FROM {rtpr_gcc} g WHERE g.parent_uid = :parent_uid', array(':parent_uid' => $uid))->fetchCol();
    $vendors[$uid] = $uids;
  }

  $values = $form_state['values'];
  $data['month'] = $values['month'];
  $data['year'] = $values['year'];

  batch_set(rtpr_pap_gcc_report_batch($vendors, $data));
}

/**
 * Activation report batch.
 */
function rtpr_pap_gcc_report_batch($vendors, $data) {
  $operations = array();
  foreach($vendors as $uid => $customers) {
    $operations[] = array(
      'rtpr_pap_gcc_report_op',
      array(
        $uid,
        $data['month'],
        $data['year'],
        $customers,
      ),
    );
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'rtpr_pap_gcc_report_op_finished',
    'file' => drupal_get_path('module', 'rtpr_pap') . '/templates/rtpr_pap.report.gcc.inc', // Required.
  );

  return $batch;

}

function rtpr_pap_gcc_report_op($uid, $month, $year, $customers, &$context) {

  module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
  module_load_include('inc','rtpr_cc','includes/rtpr_cc.gcc');
  module_load_include('inc', 'rtpr_pap', 'includes/helper');

  $account = user_load($uid);
  // Optional message displayed under the progressbar.
  $context['message'] = t('Processing user %email', array('%email' => $account->mail));
  if ($account) {

    $start_timestamp = strtotime('first day of ' . $year . '-' . $month . ' 00:00:00');
    $end_timestamp = strtotime('last day of ' . $year . '-' . $month . ' 23:59:59');
    $wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $wrapper->field_pap_affiliate_id->value();
    $customers = _rtpr_pap_get_gcc_customer_uids($account->uid);
    $user_data = new PAPUserData($pap_id, $month, $year, $account, $wrapper);
    $total_acquired = _rtpr_user_get_total_redemption_credits($account->uid);
    $total_available = _rtpr_user_get_available_redemption_credits($account->uid);
    $conversion_data =  _rtpr_pap_gcc_conversion_data($customers);
    $monthly_conversion_data =  _rtpr_pap_gcc_conversion_data($customers, $start_timestamp, $end_timestamp);

    $context['results'][$uid] = array(
      'pap_id' => $pap_id,
      'email' => $account->mail,
      'name' => $wrapper->field_first_name->value() . ' ' . $wrapper->field_last_name->value(),
      'primary' => $user_data->getPrimaryRole(),
      'total_credits' => $total_acquired,
      'total_used' => $total_acquired - $total_available,
      'total_available' => $total_available,
      'non_activated' => _rtpr_pap_gcc_customer_status_count($customers, 46),
      'activated' => _rtpr_pap_gcc_customer_status_count($customers, 7),
      'converted' => $conversion_data['converted'],
      'init_conversion' => $conversion_data['init_conversion'],
      'total_conversion' => $conversion_data['total_conversion'],
      'canceled_converted' => $conversion_data['canceled_converted'],
      'canceled_conversion' => $conversion_data['canceled_conversion'],
      'date' => $month . '/' . $year,
      'monthly_used' => _rtpr_pap_gcc_monthly_credit_used($customers, $start_timestamp, $end_timestamp),
      'monthly_activation' =>_rtpr_pap_gcc_monthly_activation($customers, $start_timestamp, $end_timestamp),
      'activated_this_month' =>  _rtpr_cc_gca_total_count($uid, $start_timestamp, $end_timestamp),
      'monthly_conversion' => $monthly_conversion_data['init_conversion']
    );
  }
}


function rtpr_pap_gcc_report_op_finished($success, $rows, $operations) {

  if ($success) {
    $header = array(
      'pap_id' => t('PAP ID'),
      'email' => t('Email'),
      'name' => t('Name'),
      'primary' => t('Primary Role'),
      'total_credits' => t('Total Credit Acquired'),
      'total_used' => t('Total Credit Used'),
      'total_available' => t('Total Credit Available'),
      'non_activated' => t('Non-activated'),
      'activated' => t('Activated'),
      'converted' => t('Converted'),
      'init_conversion' => t('Initial Conversion'),
      'total_conversion' => t('Total Conversion'),
      'canceled_converted' => t('Canceled Converted'),
      'canceled_conversion' => t('Canceled Conversion'),
      'date' => t('Date'),
      'monthly_used' => t('Monthly Credit Used'),
      'monthly_activation' =>t('Monthly Activated'),
      'activated_this_month' => t('Activated THIS Month'),
      'monthly_conversion' => t('Monthly Conversion'),
    );

      $file_path = 'public://private/' . time() . '_gcc.csv';
      $fp = fopen($file_path, 'w');

      fputcsv($fp, $header);

      foreach ($rows as $row) {
        fputcsv($fp, $row);
      }

      fclose($fp);
      $loglink = l("here", file_create_url($file_path));
      $msg = "Click $loglink to download the PAP GCC report.";
      drupal_set_message($msg);
  } else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    watchdog('rtpr_cc', t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }

}

function _rtpr_pap_gcc_monthly_activation($uids, $start_timestamp, $end_timestamp) {
  if (!is_array($uids) || empty($uids)) {
    return 0;
  }
  $query = db_select('users', 'u');
  $query->join('users_roles', 'r', 'r.uid=u.uid');
  $query->fields('u')
    ->condition('u.uid', $uids, 'IN')
    ->condition('rid', 46, '!=')
    ->condition('created', array($start_timestamp, $end_timestamp), 'BETWEEN');
  $results = $query->execute();
  $count = array();
  foreach($results as $result) {
    $count[] = $result->uid;
  }
  return count(array_unique($count));

}

function _rtpr_pap_gcc_monthly_credit_used($uids, $start_timestamp, $end_timestamp) {
  if (!is_array($uids) || empty($uids)) {
    return 0;
  }
  $query = db_select('users', 'u')
    ->fields('u')
    ->condition('uid', $uids, 'IN')
    ->condition('created', array($start_timestamp, $end_timestamp), 'BETWEEN');

  return count($query->execute()->fetchAll());

}
function _rtpr_pap_gcc_conversion_data($uids, $start_timestamp = NULL, $end_timestamp = NULL) {
  $data = array(
    'converted' => 0,
    'canceled_converted' => 0,
    'canceled_conversion' => '$0.00',
    'init_conversion' => '$0.00',
    'total_conversion' => '$0.00',
  );
  if (!is_array($uids) || empty($uids)) {
    return $data;
  }
  $query = db_select('commerce_order', 'o');
  $query->fields('o', array('uid', 'order_id', 'status'))
    ->condition('uid', $uids, 'IN')
    ->condition('status', array('pending', 'processing', 'completed', 'canceled'), 'IN')
    ->orderBy('created', 'ASC');

  if ($start_timestamp) {
    $query->condition('created', array($start_timestamp, $end_timestamp), 'BETWEEN');
  }

  $results = $query->execute()->fetchAllAssoc('order_id');
  $init_conversion = array();
  $total_conversion = 0;
  $unique_uids = array();
  $canceled_converted = array();
  $canceled_conversion = 0;
  foreach($results as $order_id => $result) {
    $uid = $result->uid;
    $order = commerce_order_load($order_id);
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $product_total = 	commerce_line_items_total($wrapper->commerce_line_items, array('product'));
    if ($result->status == 'canceled') {
      $canceled_converted[$uid] = $uid;
      $canceled_conversion += $product_total['amount'];
    } else {
      $unique_uids[$uid] = $uid;
      if (!isset($init_conversion[$uid])) {
        $init_conversion[$uid] = $product_total['amount'];
      }
      $total_conversion += $product_total['amount'];
    }
  }

  $data['converted'] = count($unique_uids);
  $data['canceled_converted'] = count($canceled_converted);
  $data['init_conversion'] = commerce_currency_format(array_sum($init_conversion), 'USD');
  $data['total_conversion'] = commerce_currency_format($total_conversion, 'USD');
  $data['canceled_conversion'] = commerce_currency_format($canceled_conversion, 'USD');
  return $data;
}
function _rtpr_pap_gcc_customer_status_count($uids, $rid) {
  if (!is_array($uids) || empty($uids)) {
    return 0;
  }
  $query = db_select('users_roles', 'r')
    ->fields('r')
    ->condition('uid', $uids, 'IN')
    ->condition('rid', $rid, '=');

  return count($query->execute()->fetchAll());
}
