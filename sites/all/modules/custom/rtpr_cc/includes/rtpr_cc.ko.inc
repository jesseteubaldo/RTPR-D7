<?php

/**
 * @file
 * Reports related to KO.
 */
 
/**
 * Callback for admin/config/rtpr/ko/new-customers.
 */
function rtpr_cc_ko_new_customers_form($form, &$form_state) {
  $order_ids = db_query(
    "SELECT order_id FROM commerce_line_item WHERE line_item_label LIKE :sku",
    array(':sku' => '%-NC-OFFER%')
  )->fetchCol();
  $results = db_query(
    "SELECT order_id, created FROM commerce_payment_transaction WHERE order_id IN (:order_ids)",
    array(':order_ids' => $order_ids)
  )->fetchAll();

  $results = db_query(
    "select commerce_line_item.order_id,commerce_payment_transaction.created  from commerce_line_item,commerce_payment_transaction where line_item_label LIKE :sku and commerce_payment_transaction.order_id = commerce_line_item.order_id and status = :success",
    array(
      ':sku' => '%-NC-OFFER%',
      ':success' => 'success',
    )
  )->fetchAll();
  
  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'name' => t('Name'),
    'pap_referrer' => t('PAP Referrer'),
    'str' => t('STR'),
    'date' => t('Date of Sale'),
    'upgrade' => t('Upgrade'),
    'promoter' => t('Promoter'),
  );

  $rows = array();

  foreach($results as $result) {
    $order = commerce_order_load($result->order_id);
    $account = user_load($order->uid);
    
    try {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $account_wrapper = entity_metadata_wrapper('user', $account);
      
      $str = $account_wrapper->field_customer_referral_id->value();
      $upgrade = FALSE;
      foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
        $sku = $line_item_wrapper->line_item_label->value();
        if (strpos($sku, 'NC-ADD-ON') !== FALSE) {
          $upgrade = TRUE;
        }
      }
      $rows[] = array(
        'uid' => $account->uid,
        'email' => $account->mail,
        'name' => $account->realname,
        'pap_referrer' => $account_wrapper->field_pap_referral_id->value(),
        'str' => $str ? $str : 'N/A',
        'date' => date('M/d/Y', $result->created),
        'upgrade' => $upgrade ? 'YES' : 'NO',
        'promoter' => user_has_role(56, $account) ? 'YES' : 'NO',
        
      );
    }
    catch (EntityMetadataWrapperException $exc) {
      watchdog(
        'rtpr_cc',
        'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
        array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
        WATCHDOG_ERROR
      );
    }
  }

  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );
  $form['rows'] = array(
    '#type' => 'value',
    '#value' => $rows,
  );
  $form['file_name'] = array(
    '#type' => 'value',
    '#value' => 'ko_new_customers.csv',
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No KO orders found.'),
  );
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No KO orders found.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
    '#submit' => array('rtpr_cc_ko_new_customers_form_submit'),
  );
  return $form;
}

function rtpr_cc_ko_new_customers_form_submit($form, &$form_state) {
  rtpr_cc_ko_report_download($form, $form_state);
  
}

/**
 * Callback for admin/config/rtpr/ko/promoter.
 */
function rtpr_cc_ko_promoter_form($form, $form_state) {
  $results = db_query(
    'SELECT u.uid, mail, realname, field_pap_affiliate_id_value
     FROM users_roles as ur, users as u, realname as r , field_data_field_pap_affiliate_id as p where rid = :rid  and u.uid = ur.uid and r.uid = u.uid and  p.entity_id = u.uid',
    array(':rid' => 56)
  )->fetchAll();
  
  $accounts = array();
  $pap_ids = array();
  $pap_id_mapping = array();
  foreach($results as $account) {
    $account->sku = 'N/A';
    $account->promoter_referrals = 0;
    $account->offer_referrals = 0;
    $account->parent = 'N/A';
    $accounts[$account->uid] = $account;
    $pap_ids[] = $account->field_pap_affiliate_id_value;
    $pap_id_mapping[$account->field_pap_affiliate_id_value] = $account->uid;
  }

  // Get all paid orders from promoters.
  $transactions = db_query(
    'SELECT uid, order_id FROM commerce_payment_transaction
     WHERE uid IN (:uids) AND status = :status',
    array(':uids' => array_keys($accounts), ':status' => 'success')
  )->fetchAll();
  $orders = array();
  $order_ids = array();
  foreach ($transactions as $transaction) {
    $orders[$transaction->order_id] = $transaction->uid;
    $order_ids[] = $transaction->order_id;
  }

  // Get all paid orders from promoters containing KO-PP-KIT- SKU.
  $ko_orders = db_query(
    'SELECT order_id,line_item_label FROM commerce_line_item
     WHERE order_id IN (:orders_id) AND line_item_label LIKE :sku',
     array(':orders_id' => $order_ids, ':sku' => 'KO-PP-KIT-%')
  )->fetchAll();
  
  $ko_orders_list = array();
  foreach ($ko_orders as $order) {
    if (isset($orders[$order->order_id])) {
      $uid  = $orders[$order->order_id];
      $accounts[$uid]->sku = $order->line_item_label;
    }
  }
  
  $rows = json_decode(json_encode($accounts), TRUE);
  db_set_active('pap');
  
  $promoter_referrals = db_query(
    'SELECT userid, count(*) AS total FROM qu_pap_transactions
     WHERE campaignid = :campaignid AND tier = :tier AND userid IN (:pap_ids) GROUP BY userid',
     array(
       ':campaignid' => '06c32fff',
       ':tier' =>  2,
       ':pap_ids' => $pap_ids
      )
  )->fetchAll();
  
  $offer_referrals = db_query(
    'SELECT userid, count(*) AS total FROM qu_pap_transactions
     WHERE campaignid = :campaignid AND tier = :tier AND userid IN (:pap_ids) GROUP BY userid',
     array(
       ':campaignid' => '4aaf14f2',
       ':tier' =>  1,
       ':pap_ids' => $pap_ids
      )
  )->fetchAll();
  
  $parents = db_query(
    'SELECT userid, parentuserid FROM qu_pap_users WHERE userid IN (:pap_ids)',
    array(':pap_ids' => $pap_ids)
  )->fetchAll();
  
  db_set_active();
  
  foreach ($promoter_referrals as $result) {
    if (isset($pap_id_mapping[$result->userid])) {
      $uid = $pap_id_mapping[$result->userid];
      $rows[$uid]['promoter_referrals'] = $result->total;
    }
  }
  foreach ($offer_referrals as $result) {
    if (isset($pap_id_mapping[$result->userid])) {
      $uid = $pap_id_mapping[$result->userid];
      $rows[$uid]['offer_referrals'] = $result->total;
    }
  }
  foreach ($parents as $parent) {
    if (isset($pap_id_mapping[$parent->userid])) {
      $uid = $pap_id_mapping[$parent->userid];
      $rows[$uid]['parent'] = $parent->parentuserid;
    }
  }
  $header = array(
    'uid' => t('UID'),
    'email'  => t('Email'),
    'pap_id' => t('PAP ID'),
    'name' => t('Name'),
    'sku' => t('Product ID'),
    'promoter_referrals' => t('Promoter Referrals'),
    'offer_referrals' => t('Offer Referrals'),
    'parent' => t('Parent'),
  );
  
  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );
  $form['rows'] = array(
    '#type' => 'value',
    '#value' => $rows,
  );
  $form['file_name'] = array(
    '#type' => 'value',
    '#value' => 'ko_promoter.csv',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
    '#submit' => array('rtpr_cc_ko_new_customers_form_submit'),
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No promoter found.'),
  );
  
  return $form;
}

/**
 * Callback for admin/config/rtpr/ko/inquiry.
 */
function rtpr_cc_ko_inquiry_form($form, $form_state) {
  $form['txt'] = array(
    '#markup' => views_embed_view('webform_submissions', 'default', 80389),
  );
  
  return $form;
}

/**
 * Submit handler to download report.
 */
function rtpr_cc_ko_report_download($form, &$form_state) {
  $header = $form_state['values']['header'];
  $rows = $form_state['values']['rows'];
  $file_name = $form_state['values']['file_name'];

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $file_name);
  $out = fopen('php://output', 'w');


  fputcsv($out, $header);

  foreach ($rows as $row) {
    fputcsv($out, (array) $row);
  }

  fclose($out);
  exit();
}