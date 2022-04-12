<?php

/**
 * Administrator features for PAP
 */


/**
 * Display missing orders from PAP.
 */
function rtpr_offer_missing_orders_form($form, &$form_state) {
  $oldest_year = 2011;
  $current_year = date('Y');

  $years = array();

  for($year = $oldest_year; $year <= $current_year; $year++) {
    $years[$year] = $year;
  }

  $months = array();
  for($month = 1; $month <= 12; $month++) {
    $months[$month] = $month;
  }

  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );

  $form['options']['year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => $years,
    '#default_value' => isset($_SESSION['rtpr_offer']['year']) ? $_SESSION['rtpr_offer']['year'] : date('Y'),
  );

  $form['options']['month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => $months,
    '#default_value' => isset($_SESSION['rtpr_offer']['month']) ? $_SESSION['rtpr_offer']['month'] : date('n'),
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('rtpr_offer_missing_orders_search_form_submit'),
  );
  $form['options']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('rtpr_offer_missing_orders_search_form_reset'),
  );

  if(isset($form_state) && isset($form_state['rows'])) {
    $rows = $form_state['rows'];
  }
  else {
    $rows = rtpr_offer_missing_orders_rows(date('n'), date('Y'), $form_state);
  }

  $header = rtpr_offer_missing_order_table_header();

  $form['orders'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No missing orders found.'),
  );

  $form['current'] = array(
    '#type' => 'checkbox',
    '#title' => t('Post to PAP using current date.'),
  );

  $form['notes'] = array(
    '#type' => 'textfield',
    '#title' => t('Notes'),
    '#description' => t('15 characters or less'),
    '#size' => 15,
    '#maxlength' => 15,
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name=current]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['fix'] = array(
    '#type' => 'submit',
    '#value' => t('Fix'),
    '#submit' => array('rtpr_offer_missing_order_fixed'),
  );
  return $form;
}

/**
 * Table header.
 */
function rtpr_offer_missing_order_table_header() {
  return array(
    array('data' => t('#')),
    array('data' => t('Order #'), 'field' => 'o.order_id'),
    array('data' => t('Total')),
    array('data' => t('UID')),
    array('data' => t('Name')),
    array('data' => t('Email')),
    array('data' => t('Primary Role')),
    array('data' => t('Paid Date'), 'field' => 'o.changed'),
    array('data' => t('PAP Referrer')),
  );
}
/**
 * Search reset handler.
 */
function rtpr_offer_missing_orders_search_form_reset($form, &$form_state) {
  unset($_SESSION['rtpr_offer']);
}
/**
 * Search missing order handler
 */
function rtpr_offer_missing_orders_search_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $year = $values['year'];
  $month = $values['month'];
  $_SESSION['rtpr_offer']['month'] = $month;
  $_SESSION['rtpr_offer']['year'] = $year;
  $form_state['rows'] = rtpr_offer_missing_orders_rows($month, $year);
  // Keep submitted value
  $form_state['rebuild'] = TRUE;
}

function rtpr_offer_missing_orders_rows($month, $year, $form_state = NULL) {
  if (isset($_SESSION['rtpr_offer']['month'])) {
    $month = $_SESSION['rtpr_offer']['month'];
    $year = $_SESSION['rtpr_offer']['year'];
  }
  $query = $_GET;
  if(isset($query['sort'])) {
    $form_state;
    $sort = $query['sort'];
  }
  if(isset($query['order'])) {
    $order = $query['order'];
    if ($order == 'Order #') {
      $order = 'order_id';
    }
    else {
      $order = 'changed';  
    }
  }

  $header = rtpr_offer_missing_order_table_header();

  $start_date = strtotime($year . '-' . $month . '-01 00:00:01');
  $end_date = strtotime($year . '-' . $month . '-31 23:59:59');

  $query = db_select('commerce_order', 'o');
  $query->join('commerce_payment_transaction', 't', 't.order_id = o.order_id');
  $query->join('users_roles', 'u', 'u.uid = o.uid');
  $query->fields('o', array('uid'));
  $query->fields('t', array('order_id', 'created'))
    ->condition('t.status', 'success')
    ->condition('t.created', array($start_date, $end_date), 'BETWEEN')
    ->condition('o.status', array('pending', 'processing', 'completed'), 'IN')
    ->condition('rid', 7, '=');
    
  $orders = $query->execute()->fetchAllAssoc('order_id');
  $revision_orders_ids = array_keys($orders);
  $commerce_order_ids = $revision_orders_ids;

  // Non-primary role
  $non_primary_roles = array('2' => '2', '30' => '30');

  // Exclude roles
  $exclude_roles = array(7, 5, 3);
  $staff_rid = 5;

  $rows = array();
  if(!empty($commerce_order_ids)){
    db_set_active('pap');
    $pap_orders = db_select('qu_pap_transactions', 't')
    ->fields('t', array('transid', 'userid','orderid'))
    ->condition('orderid', $commerce_order_ids, 'IN')
    ->execute()->fetchAllAssoc('orderid');
    $pap_order_ids = array_keys($pap_orders);

    db_set_active();

    // Missing orders
    $diff = array_diff($commerce_order_ids, $pap_order_ids);

    $index = 1;
    foreach($diff as $order_id){
      if (isset($orders[$order_id])) {
        $order = $orders[$order_id];
        $account = user_load($order->uid);
        if ($account) {
          $roles = $account->roles;
          $roles = array_diff_key($roles, $non_primary_roles);
          // Get the order wrapper
          $commerce_order = commerce_order_load($order_id);
          $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
          $order_total = $order_wrapper->commerce_order_total->value();

          $wrapper = entity_metadata_wrapper('user', $account);
          $name = $wrapper->field_first_name->value() . ' ' . $wrapper->field_last_name->value();
          $updated = $order->created;
          $rows[$order_id] = array(
            $index,
            $account->uid,
            $order_id,
            '$' . number_format($order_total['amount'] / 100, 2),
            $name,
            $account->mail,
            strtoupper(implode(',', $roles)),
            date('m/d/Y', $updated),
            $wrapper->field_pap_referral_id->value(),
          );
          $index++;
        }
      }
    }
  }
  return $rows;
}

/**
 * Get order confirmed date.
 */
function rtpr_offer_get_confirmed_date($order_id) {
  $query = db_select('commerce_order_revision', 'r')
  ->fields('r', array('revision_timestamp'));

  $or = db_or();
  $or->condition('status', 'pending', '=');
  $or->condition('status', 'checkout_complete', '=');

  $result = $query->condition($or)
  ->condition('order_id', $order_id, '=')
  ->orderBy('status', 'DESC')
  ->execute()->fetchField();

  return $result;
}

/**
 * Fixed missing orders
 */
function rtpr_offer_missing_order_fixed($form, &$form_state) {
  $values = $form_state['values'];
  $orders = $values['orders'];
  $use_current_date = $values['current'];
  $notes = $values['notes'];
  foreach($orders as $order_id => $value) {
    if(isset($values) && $value) {
      $order = commerce_order_load($order_id);
      if ($use_current_date) {
        $confirmed_date = time();
      } else {
        $confirmed_date = rtpr_offer_get_confirmed_date($order_id);
        if(!$confirmed_date) {
          $confirmed_date = $order->changed;
        }
      }
      rtpr_offer_missing_order_post($order, $confirmed_date, $notes);
      unset($form_state['rows'][$order_id]);
    }
  }
  $form_state['rebuild'] = TRUE;
}

function rtpr_offer_missing_order_post($commerce_order, $paid_date = null, $notes = '') {
  //$paid_date = $paid_date==null ? date('Y-m-d', time()) : date('Y-m-d', $paid_date);
  $paid_date = $paid_date==null ? time() : $paid_date;
  $account_id = variable_get('commerce_post_affiliate_pro_merchant_id', NULL);
  if (!$account_id) {
    watchdog('commerce_post_affiliate_pro', 'Could not register sale for order @order, merchant id is not set, please visit admin pages.', array('@order' => $commerce_order->order_number));
    return;
  }
  $transaction_exists = rtpr_commerce_pap_transaction_load($commerce_order->order_number);
  if ($transaction_exists) {
    watchdog('commerce_post_affiliate_pro', 'Could not register sale for order @order, transactoin already exists.', array('@order' => $commerce_order->order_number));
    return;
  }
  
  if (!user_access('administer shipment') && !user_access('administer rptr cc')){
    $transactions = commerce_payment_transaction_load_multiple(array(), array('order_id' => $commerce_order->order_number), TRUE);
    if (!$transactions) {
      watchdog('commerce_post_affiliate_pro', 'No transaction found in order @order!', array('@order' => $commerce_order->order_number), 'warning');
      //return;
    }
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
  $line_items = $order_wrapper->commerce_line_items->value();

  if(!$line_items) {
    drupal_set_message(t('No items found in order @order!', array('@order' => $commerce_order->order_number)), 'error');
    return;
  }
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');

  $sale_tracker = new Pap_Api_SaleTracker(variable_get('commerce_post_affiliate_pro_merchant_url_prefix', 'http://demo.qualityunit.com/pax4/') . "scripts/sale.php", TRUE);

  $sale_tracker->setAccountId($account_id);
  if(isset($_COOKIE['PAPVisitorId']))
    $sale_tracker->setVisitorId($_COOKIE['PAPVisitorId']);

  // get the customer details
  $account = user_load($commerce_order->uid);
  $account_wrapper = entity_metadata_wrapper('user', $account);

  $coupons = $order_wrapper->commerce_coupons->value();

  // iterate through each line item
  foreach ($line_items as $line_item) {
    // skip other itesm
    if ($line_item->type == 'product') {
      $sale              = $sale_tracker->createSale();
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $amount            = $line_item_wrapper->commerce_total->value();
      // ser the product cost
      $price = $line_item->commerce_total['und'][0]['amount'];

      $sale->setTotalCost($price / 100);
      // set the order ID
      $sale->setOrderID($commerce_order->order_number);
      $commerce_product = $line_item_wrapper->commerce_product->value();
      $commerce_product_wrapper = entity_metadata_wrapper('commerce_product', $commerce_product);
      $sale->setProductID($commerce_product->sku);
      // Override product cost
      if (in_array($commerce_product->sku, array('VENDOR-STARTER-BASIC', 'VENDOR-STARTER-ADVANCED'))) {
        $sale->setTotalCost(100);
      }
      $campaign = $commerce_product_wrapper->field_product_campaign->value();

      // Set campagin base on role and uid.
      if ($account->uid > 384432 || user_has_role(25, $account)) {
        $premuim_campaign = $commerce_product_wrapper->field_product_campaign_premuim->value();
        if ($premuim_campaign && $premuim_campaign != '_none') {
          $campaign = $premuim_campaign;
        }
      }

      if (!empty($campaign) && $campaign != '_none') {
        // set the campaign id
        $sale->setCampaignID($campaign);
      }

      if(isset($coupons) && isset($coupons[0])){
        $coupon = $coupons[0];
        $sale->setCouponCode($coupon->code);
      }

      $referral_id = $account_wrapper->field_pap_referral_id->value();
 
      if (user_has_role(7, $account)) {
        if (!$referral_id) {
          $sale->setAffiliateID(RTPR_OFFER_DEFAULT_PAP_ID);
        } else {
          $sale->setAffiliateID(trim($referral_id));
        }
      } else {
        $pap_affiliate_id = $account_wrapper->field_pap_affiliate_id->value();
        if ($pap_affiliate_id) {
          $sale->setAffiliateID(trim($pap_affiliate_id));
        }
      }

      // ser status to pending,
      $sale->setStatus('P');
      // name
      $sale->setData1(preg_replace("/[^a-zA-Z0-9 ]/", "", $account_wrapper->field_first_name->value()) . ' ' . preg_replace("/[^a-zA-Z0-9 ]/", "", $account_wrapper->field_last_name->value()));
      // customer email
      $sale->setData2($account_wrapper->mail->value());
      // phone number
      $sale->setData3($account_wrapper->field_phone->value());
      // Notes
      $sale->setData4($notes);
      // Paid date
      $sale->setTimeStamp($paid_date);
      // User ID.
      $sale->setData5($account->uid);
    }
    dpm($sale);
    
  }
  
  ob_start();
  $result = $sale_tracker->register();
    dpm($result);
  ob_get_clean();

}
