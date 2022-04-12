<?php

/**
 * @file
 * VGCC Report
 */

function rtpr_cc_vgcc_report_form($form, $form_state) {
  $header = _rtpr_cc_vgcc_report_header();

  $rows = _rtpr_cc_vgcc_report(100);
  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );

  $form['rows'] = array(
    '#type' => 'value',
    '#value' => $rows,
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );

  $form['pager_pager'] = array(
    '#theme' => 'pager',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

function rtpr_cc_vgcc_report_form_submit($form, $form_states) {
  $query = db_select('userpoints_txn', 'ut')
    ->fields('ut', array('uid', 'expirydate'))
    ->condition('description', 'New Virtual GCC bonus')
    ->orderBy('time_stamp', 'DESC');

  $vgcc = $query->execute()->fetchAllAssoc('uid');

  $uids = array_keys($vgcc);
  $purchase_uids = array();
  if ($uids) {
    $purchased_orders = db_select('commerce_order', 'o')
      ->fields('o', array('uid', 'order_id', 'created'))
      ->condition('uid', $uids, 'IN')
      ->condition('status', array(
        'completed',
        'pending',
        'processing',
        'waiting',
        'checkout_complete',
      ))
      ->execute()->fetchAllAssoc('uid');
    $purchase_uids = array_keys($purchased_orders);
  }

  $operations = array();

  foreach ($uids as $uid) {
    $operations[] = array(
      'rtpr_cc_vgcc_report_process',
      array(
        $uid,
        $vgcc[$uid]->expirydate,
        in_array($uid, $purchase_uids),
        in_array($uid, $purchase_uids) ? $purchased_orders[$uid] : NULL,
      ),
    );

  }
  $batch = array(
    'operations' => $operations,
    'finished' => '_rtpr_cc_vgcc_report_finished',
    'title' => t('Processing VGCC Report'),
    'init_message' => t('VGCC Report is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('VGCC Report has encountered an error.'),
    'file' => drupal_get_path('module', 'rtpr_cc') . '/includes/rtpr_cc.redemption.vgcc.admin.inc',
  );
  batch_set($batch);
}

/**
 * Batch process.
 */
function rtpr_cc_vgcc_report_process($uid, $expirydate, $has_purchased, $purchased_order, &$context) {
  $context['message'] = t('Processing user %$uid', array('%$uid' => $uid));
  $context['results'][$uid] = _rtpr_cc_vgcc_report_get_row($uid, $expirydate, $has_purchased, $purchased_order);
}

/**
 * Get VGCC result.
 */
function _rtpr_cc_vgcc_report($limit = 100) {
  $query = db_select('userpoints_txn', 'ut')
    ->fields('ut', array('uid', 'expirydate'))
    ->condition('description', 'New Virtual GCC bonus')
    ->orderBy('time_stamp', 'DESC');

  if ($limit) {
    $vgcc = $query->extend('PagerDefault')->limit(100)->execute()->fetchAllAssoc('uid');
  }
  else {
    $vgcc = $query->execute()->fetchAllAssoc('uid');
  }
  $uids = array_keys($vgcc);

  $purchase_uids = array();
  if ($uids) {
    $purchased_orders = db_select('commerce_order', 'o')
      ->fields('o', array('uid', 'order_id', 'created'))
      ->condition('uid', $uids, 'IN')
      ->condition('status', array(
        'completed',
        'pending',
        'processing',
        'waiting',
        'checkout_complete',
      ))
      ->execute()->fetchAllAssoc('uid');
    $purchase_uids = array_keys($purchased_orders);
  }
  $rows = array();
  foreach($uids as $uid) {
    $expiry_date = $vgcc[$uid]->expirydate;
    $has_purchased = in_array($uid, $purchase_uids);
    $purchased_order = $has_purchased ? $purchased_orders[$uid] : NULL;
    $rows[$uid] = _rtpr_cc_vgcc_report_get_row($uid, $expiry_date, $has_purchased, $purchased_order);
  }

  return $rows;
}

/**
 * Batch 'finished' callback
 */
function _rtpr_cc_vgcc_report_finished($success, $rows, $operations) {
  if ($success) {
    $header = _rtpr_cc_vgcc_report_header();
    
    $file_path = 'public://private/' . date('m_d_Y', time()) . '_vgcc_report_report.csv';
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

function _rtpr_cc_vgcc_report_header() {
  return array(
    'uid' => t('UID'),
    'first_name' => 'First name',
    'last_name' => 'Last name',
    'address' => 'address',
    'city' => 'city',
    'state' => 'state',
    'zip' => 'zip',
    'email' => 'email',
    'phone' => 'phone',
    'referring_pap' => 'referring PAP ID',
    'purchase' => 'Purchased',
    'purchase_date' => 'Date of First Purchase',
    'purchase_subtotal' => t('Purchase subtotal'),
    'purchase_total' => t('Purchase total'),
    'gc_balance' => 'GC Balance',
    'gc_exp' => 'GC Exp Date',
  );
}

function _rtpr_cc_vgcc_report_get_row($uid, $expirydate, $has_purchased, $purchased_order) {
  $account = user_load($uid);
  $credit = rtpr_reward_get_userpoints_txn($uid);
  $redeemed = rtpr_reward_get_userpoints_txn($uid, RTPR_COMMERCE_REDEEMED_TID);

  $credit_total = isset($credit->total) ? $credit->total : 0;
  $redeemed_total = isset($redeemed->total) ? abs($redeemed->total) : 0;
  $available = $credit_total - $redeemed_total;

  $account_wrapper = entity_metadata_wrapper('user', $account);
  $address = $account_wrapper->field_address->value();
  $purchased = FALSE;
  $purchase_date = 'N/A';
  $purchase_total = 0;
  $purchase_subtotal = 0;
  if ($has_purchased) {
    $purchased = TRUE;
    $purchase_date = date('m/d/Y', $purchased_order->created);
    $commerce_order = commerce_order_load($purchased_order->order_id);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
    $product = commerce_line_items_total($order_wrapper->commerce_line_items, array('product'));
    $purchase_subtotal = '$' . number_format($product['amount'] / 100, 2);
    $total_amount = $order_wrapper->commerce_order_total->amount->value();
    $purchase_total = '$' . number_format($total_amount / 100, 2);
  }
  $row = array(
    'uid' => $uid,
    'first_name' => $account_wrapper->field_first_name->value(),
    'last_name' => $account_wrapper->field_last_name->value(),
    'address' => $address['thoroughfare'] . ' ' . $address['premise'],
    'city' => $address['locality'],
    'state' => $address['administrative_area'],
    'zip' => $address['postal_code'],
    'email' => $account->mail,
    'phone' => $account_wrapper->field_phone->value(),
    'referring_pap' => $account_wrapper->field_pap_referral_id->value(),
    'purchase' => $purchased ? 'YES' : 'NO',
    'purchase_date' => $purchase_date,
    'purchase_total' => $purchase_total,
    'purchase_subtotal' => $purchase_subtotal,
    'gc_balance' => $available,
    'gc_exp' => $available ? date('m/d/Y', $expirydate) : 'N/A',
  );

  return $row;
}