<?php

/**
 * Administrator features for PAP
 */


/**
 * Display missing orders from PAP.
 */
function rtpr_pap_missing_orders_form($form, &$form_state) {

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
    '#default_value' => $current_year,
  );

  $form['options']['month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => $months,
    '#default_value' => date('n'),
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('rtpr_pap_missing_orders_search_form_submit'),
  );

  $form['promotional_sku'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Order With STARTER-KIT Only'),
    '#states' => array(
      'disabled' => array(
        ':input[name="t2_credit_only"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['t2_credit_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Order With ONLINE-GCS-T2-Credit Only'),
    '#states' => array(
      'disabled' => array(
        ':input[name="promotional_sku"]' => array('checked' => TRUE),
      ),
    )
  );

  if(isset($form_state) && isset($form_state['rows'])) {

    $rows = $form_state['rows'];
    $header = array(
      'index' => t('#'),
      'uid' => t('UID'),
      'order_id' => t('Order #'),
      'total' => t('Total'),
      'name' => t('Name'),
      'email' => t('Email'),
      'role' => t('Primary Role'),
      'updated' => t('Paid Date'),
    );

    $form['orders'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No missing orders found.'),

    );

    $form['fix'] = array(
      '#type' => 'submit',
      '#value' => t('Fix'),
      '#submit' => array('rtpr_pap_missing_order_fixed'),
    );
  }
  return $form;
}

/**
 * Search missing order handler
 */
function rtpr_pap_missing_orders_search_form_submit($form, &$form_state) {

  $promotional_sku_only = $form_state['values']['promotional_sku'];
  $t2_credit_only = $form_state['values']['t2_credit_only'];
  $values = $form_state['values'];

  $year = $values['year'];
  $month = $values['month'];

  $start_date = strtotime($year . '-' . $month . '-01 00:00:01');
  $end_date = strtotime($year . '-' . $month . '-31 23:59:59');

  $query = db_select('commerce_order', 'o');
  $query->join('commerce_payment_transaction', 't', 't.order_id = o.order_id');
  $query->fields('o', array('uid'));
  $query->fields('t', array('order_id', 'created'))
    ->condition('t.status', 'success')
    ->condition('t.created', array($start_date, $end_date), 'BETWEEN')
    ->condition('o.status', array('pending', 'processing', 'completed'), 'IN');

  $orders = $query->execute()->fetchAllAssoc('order_id');
  $revision_orders_ids = array_keys($orders);
  //$commerce_order_ids = array_keys($orders);
  $commerce_order_ids = $revision_orders_ids;

  // Non-primary role
  $non_primary_roles = array(2, 30);

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
    $credits_orders = array();
    $promotional_orders = array();
    foreach($diff as $order_id){
      $skip = FALSE;
      $show_order_id = array();
      if (isset($orders[$order_id])) {
        $order = $orders[$order_id];
        $account = user_load($order->uid);
        if ($account) {
          $roles = $account->roles;
          if(!array_intersect($exclude_roles, array_keys($roles))){
            foreach($non_primary_roles as $rid) {
              if(isset($roles[$rid])) {
                unset($roles[$rid]);
              }
            }
            // Get the order wrapper
            $commerce_order = commerce_order_load($order_id);
            $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
            $order_total = $order_wrapper->commerce_order_total->value();

            $products = array();
            foreach($order_wrapper->commerce_line_items as $line_item) {
              if ($line_item->type->value() == 'product') {
                $products[] = $line_item->line_item_label->value();
              }
              if (stripos($line_item->line_item_label->value(), 'STARTER-KIT') !== FALSE) {
                $promotional_orders[] = $order_id;
              }
            }
            if (count($products) == 1 && in_array('ONLINE-GCS-T2-Credit', $products)) {
              $credits_orders[] = $order_id;
            }
            $wrapper = entity_metadata_wrapper('user', $account);
            $name = $wrapper->field_first_name->value() . ' ' . $wrapper->field_last_name->value();
            $updated = $order->created;
            $rows[$order_id] = array(
              'index' => $index,
              'uid' => $account->uid,
              'order_id' => $order_id,
              'total' => '$' . number_format($order_total['amount'] / 100, 2),
              'name' => $name,
              'email' => $account->mail,
              'role' => strtoupper(implode(',', $roles)),
              'updated' => date('m/d/Y', $updated),

            );
            $index++;
          } else {
            //drupal_set_message(t('order @order_id is not wholesale order.', array('@order_id' => $order_id)), 'warning');
          }
        }
      }
    }
  }

 $count = 1;
  foreach ($rows as $order_id => $row) {
    if (in_array($order_id, $credits_orders) != $t2_credit_only || in_array($order_id, $promotional_orders) != $promotional_sku_only) {
      unset($rows[$order_id]);
    } else {
      $rows[$order_id]['index'] = $count;
      $count++;
    }
  }

  $form_state['rows'] = $rows;
  // Keep submitted value
  $form_state['rebuild'] = TRUE;
}

/**
 * Get order confirmed date.
 */
function rtpr_pap_get_confirmed_date($order_id) {
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
function rtpr_pap_missing_order_fixed($form, &$form_state) {


  $values = $form_state['values'];

  $orders = $values['orders'];

  foreach($orders as $order_id => $value) {
    if(isset($values) && $value) {
      $order = commerce_order_load($order_id);
      $confirmed_date = rtpr_pap_get_confirmed_date($order_id);
      if(!$confirmed_date) {
        $confirmed_date = $order->changed;
      }
      rtpr_pap_missing_order_post($order, $confirmed_date);
      unset($form_state['rows'][$order_id]);
    }
  }


  $form_state['rebuild'] = TRUE;
}

function rtpr_pap_missing_order_post($commerce_order, $paid_date = null) {
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

      // Paid date
      $sale->setTimeStamp($paid_date);

    }
  }


  ob_start();
  $sale_tracker->register();
  ob_get_clean();

}