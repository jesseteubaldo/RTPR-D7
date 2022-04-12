<?php

/**
 * @file
 * SKU search features.
 */

/**
 * User SKU search form.
 */
function rtpr_cc_user_sku_search_form($form, &$form_state) {
  $form['options'] = array(
      '#type' => 'container',
      '#attributes' => array(
          'class' => array('container-inline'),
      ),
  );

  $form['options']['sku'] = array(
    '#type' => 'select',
    '#title' => 'SKU',
    '#multiple' => TRUE,
    '#options' => _rtpr_cc_sku_list(),
    '#default_value' => isset($_GET['sku']) ? $_GET['sku'] : NULL,
    '#required' => TRUE,
  );

  $form['options']['start'] = array(
    '#title' => t('To'),
    '#type' => 'date_popup',
    '#date_year_range' => '-3:+1',
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => isset($_GET['start']) ? $_GET['start'] : NULL,
    '#required' => TRUE,
  );

  $form['options']['end'] = array(

    '#type' => 'date_popup',
    '#date_year_range' => '-3:+1',
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => isset($_GET['end']) ? $_GET['end'] : NULL,
    '#required' => TRUE,
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),

  );

  $header = array(
    'uid' => t('UID'),
    'first' => t('First Name'),
    'last' => t('Last Name'),
    'email' => t('Email'),
    'pap_id' => t('PAP ID'),
    'count' => t('SKU Purchased'),
    'sku' => t('SKU'),
    'date' => t('Date'),
    'last_purchased' => t('Paid'),
    'ref_cust_uid' => t('REF CUST UID'),
    'role' => t('Role'),
    'sponsor_uid' => t('Sponsor UID'),
    'sponsor_fname' => t('Sponsor First Name'),
    'sponsor_lname' => t('Sponsor Last Name'),
    'sponsor_username' => t('Sponsor Username'),
    'sponsor_pap_id' => t('Sponsor PAP ID'),
    'sponsor_role' => t('Sponsor Role'),
    'physical_offer' =>  t('Physical'),
    'email_offer' =>  t('Email'),
    'operation' => t('Operation'),
  );
  $form_state['header'] = $header;
  $rows = array();
  if (isset($_GET['batchid'])) {
    $batch_id = 'rtpr_cc_sku_report_' . $_GET['batchid'];
    $rows = variable_get($batch_id, array());
    // Clean up batch.
    db_delete('batch')->condition('batch', '%rtpr_cc_user_sku_search_finished%', 'LIKE')->execute();

    $rows[] = array(
      'uid' => '',
      'first' => '',
      'last' => '',
      'email' => '',
      'pap_id' => 'Total',
      'count' => $rows['total_purchased'],
      'sku' => '',
      'date' => '',
      'last_purchased' => '',
      'ref_cust_uid' => '',
      'role' => '',
      'sponsor_uid' => '',
      'sponsor_fname' => '',
      'sponsor_lname' => '',
      'sponsor_username' => '',
      'sponsor_pap_id' => '',
      'sponsor_role' => '',
      'physical_offer' => '',
      'email_offer' => '',
      'operation' => '',
    );
    
    $non_rows = array('total_purchased', 'sku', 'start', 'end');
    foreach ($non_rows as $key) {
      unset($rows[$key]);
    }
    
    $form_state['rows'] = $rows;
    $form['download'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      '#submit' => array('rtpr_cc_user_sku_search_download_callback'),
    );
    
  }
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No reports available.'),
  );

  return $form;
}

/**
 *
 */
function rtpr_cc_user_sku_search_download_callback($form, &$form_state) {
  $values = $form_state['values'];
  $start_date = $values['start'];
  $end_date = $values['end'];

  $file_name = 'sku-' . $start_date . '-' . $end_date . '.csv';
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $file_name);
  $out = fopen('php://output', 'w');

  $header = $form_state['header'];
  unset($header['operation']);
  $rows = $form_state['rows'];

  fputcsv($out, $header);

  foreach ($rows as $row) {
    unset($row['operation']);
    fputcsv($out, (array) $row);
  }

  fclose($out);
  exit();
}

/**
 * Validation handler for rtpr_cc_user_sku_search_form().
 */
function rtpr_cc_user_sku_search_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $skus = $values['sku'];
  if (count($skus) > 1) {
    $max = 86400 * 30;
    $time_elapsed = strtotime($values['end']) - strtotime($values['start']);
    if ($time_elapsed > $max) {
      form_set_error('sku', t('Date range cannot exceed 90 days with multiple SKU selected.'));
    }
  }
}

/**
 * Submit handler for rtpr_cc_user_sku_search_form().
 */
function rtpr_cc_user_sku_search_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $start_date = $values['start'];
  $end_date = $values['end'];
  $skus = $values['sku'];
  $rows = array();


  $start_timestamp = strtotime($start_date);
  $end_timestamp = strtotime($end_date . ' 23:59:59');
  $operations = array();
     $datas = array();
  foreach ($skus as $search_sku) {
    $product_id =  db_query(
      'SELECT product_id FROM commerce_product WHERE sku = :sku',
      array(':sku' => $search_sku)
    )->fetchField();


    // Account for old sku.
    $search_skus = db_query(
      'SELECT distinct line_item_label FROM field_data_commerce_product,commerce_line_item WHERE entity_id = line_item_id AND commerce_product_product_id = :product_id',
      array(':product_id' => $product_id)
    )->fetchCol();
  
    if (empty($search_skus)) {
      $search_skus = array($search_sku);
    }
    $query = db_select('commerce_line_item', 'i');
    $query->join('commerce_payment_transaction', 'v', 'v.order_id = i.order_id');
    $query->join('commerce_order', 'o', 'o.order_id = v.order_id');
    $query->fields('i', array('quantity'));
    $query->fields('v', array('order_id', 'created'));
    $query->fields('o', array('uid'));
    $query->condition('v.status', 'success')
        ->condition('line_item_label', $search_skus, 'IN')
        ->condition('o.status', array('pending', 'waiting', 'hold', 'processing', 'completed'), 'IN')
        ->condition('v.created', array($start_timestamp, $end_timestamp), 'BETWEEN');
    $orders = $query->execute();
  
 
    foreach ($orders as $order) {
      if (!isset($datas[$order->uid][$search_sku]['count'])) {
        $datas[$order->uid][$search_sku]['count'] = 0;
      }
      if (empty($datas[$order->uid][$order->order_id][$search_sku])) {
        $datas[$order->uid][$order->order_id][$search_sku] = TRUE;
        $datas[$order->uid][$search_sku]['count'] += (int) $order->quantity;
        $datas[$order->uid][$search_sku]['last_purchased'] = $order->created;
        $datas[$order->uid][$search_sku]['uid'] = $order->uid;
        $datas[$order->uid][$search_sku]['start_date'] = $start_date;
        $datas[$order->uid][$search_sku]['end_date'] = $end_date;
        $datas[$order->uid][$search_sku]['sku'] = $search_sku;
        
      }
    }


  }
  foreach ($datas as $uid => $data) {
    foreach ($data as $sku => $param) {
      if (isset($param['sku'])) {
        $operations[] = array(
            'rtpr_cc_user_sku_search_op',
            array(
                $param,
            ),
        );
      }
    }
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'rtpr_cc_user_sku_search_finished',
    'file' => drupal_get_path('module', 'rtpr_cc') . '/includes/rtpr_cc.sku.inc',
  );
  batch_set($batch);
}

/**
 *  Callback: batch operation
 */
function rtpr_cc_user_sku_search_op($data, &$context) {
  // Load PAP library.
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  $affiliate = new Pap_Api_Affiliate($session);

  $uid = $data['uid'];
  $start_date = $data['start_date'];
  $end_date = $data['end_date'];
  $search_sku = $data['sku'];
  $context['message'] = t('Processing user with id "@id"', array('@id' => $uid));
   
  $sponsor = array(
    'username' => 'N/A',
    'pap' => 'N/A',
    'role' => 'N/A',
    'first_name' => 'N/A',
    'last_name' => 'N/A'
  );

  $account = user_load($uid);
  if ($account && $uid) {
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $subsciptions = $account_wrapper->field_offer_subscription->value();
    $ref_custom_uid = 'N/A';
    if (!in_array(7, array_keys($account->roles))) {
      if ($account_wrapper->field_pap_affiliate_id->value()) {
        $affiliate->setUserid($account_wrapper->field_pap_affiliate_id->value());
        try {
          $affiliate->load();
          $sponsor['pap'] = $affiliate->getParentUserId();
        } catch (Exception $e) {
          watchdog('rtpr_cc', t('Cannot load record: ' .$e->getMessage()));
        }
        if ($sponsor['pap']) {
          $sponsor_uid = db_query("SELECT p.entity_id FROM {field_data_field_pap_affiliate_id} p WHERE p.field_pap_affiliate_id_value = :value", array(
            ':value' => $sponsor['pap']
          ))->fetchField();
          $sponsor_account = user_load($sponsor_uid);
          try {
            $sponsor_wrapper = entity_metadata_wrapper('user', $sponsor_account);
            $sponsor['first_name'] = $sponsor_wrapper->field_first_name->value();
            $sponsor['last_name'] = $sponsor_wrapper->field_last_name->value();
            $sponsor['username'] = $sponsor_account->mail;
            $sponsor['role'] = rtpr_user_get_primary_role($sponsor_account->roles);
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
      }
    }
    if ($account_wrapper->field_pap_referral_id->value()) {
      $ref_custom_uid = db_query(
        'SELECT entity_id FROM field_data_field_pap_affiliate_id WHERE field_pap_affiliate_id_value = :pap_referral_id',
        array(':pap_referral_id' => $account_wrapper->field_pap_referral_id->value())
      )->fetchField();
    }
    $row = array(
      'uid' => $account->uid,
      'first' => $account_wrapper->field_first_name->value(),
      'last' => $account_wrapper->field_last_name->value(),
      'email' => $account->mail,
      'pap_id' => $account_wrapper->field_pap_affiliate_id->value(),
      'count' => $data['count'],
      'sku' => $search_sku,
      'date' => $start_date . ' - ' . $end_date,
      'last_purchased' => date('m/d/Y', $data['last_purchased']),
      'ref_cust_uid' => $ref_custom_uid,
      'role' => rtpr_user_get_primary_role($account->roles),
      'sponsor_uid' => $sponsor['first_name'] == 'N/A' ? 'N/A' : $sponsor_uid,
      'sponsor_fname' => $sponsor['first_name'],
      'sponsor_lname' => $sponsor['last_name'],
      'sponsor_username' => $sponsor['username'],
      'sponsor_pap_id' => $sponsor['pap'],
      'sponsor_role' => $sponsor['role'],
      'physical_offer' => !in_array('no_mail_offer', $subsciptions) ? 'YES' : 'NO',
      'email_offer' => !in_array('no_email_offer', $subsciptions) ? 'YES' : 'NO',
      'operation' => l('edit', '/user/' . $uid . '/edit', array('attributes' => array('target'=>'_blank'))),
    );
    $context['results']['total_purchased'] += $data['count'];
    $context['results']['start'] = $start_date;
    $context['results']['end'] = $end_date;
    $context['results']['sku'][$search_sku] = $search_sku;
  
    $context['results'][$uid . $search_sku] = $row;
  }
}

function rtpr_cc_user_sku_search_finished($success, $results, $operations) {
  $batch_id = time();
  variable_set('rtpr_cc_sku_report_' . $batch_id, $results);
  
  drupal_goto('admin/config/rtpr/sku',
    array(
      'query' => array(
        'batchid' => $batch_id,
        'sku' => $results['sku'],
        'start' => $results['start'],
        'end' => $results['end'],
      )
    )
  );
}
/**
 *
 */
function rtpr_cc_get_year_options() {
  $current_year = date('Y');
  $years = array();
  for ($year = $current_year - 3; $year <= $current_year; $year++) {
    $years[$year] = $year;
  }

  return $years;
}
