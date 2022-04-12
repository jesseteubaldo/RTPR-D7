<?php
/*
 * @file commerce_post_affiliate_pro.rules.inc
 * Provides rules actions/integration
 * @copyright Copyright(c) 2011 Rowlands Group
 * @license GPL v3 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlandsgroup dot com
 *
 */

/**
 * Implements hook_rules_condition_info().
 */
function rtpr_commerce_rules_condition_info() {
  $conditions = array();

  $conditions['rtpr_commerce_order_contains_product'] = array(
    'label' => t('Order contains a particular SKU wildcard'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order whose line items should be checked for the specified product.'),
      ),
      'product_id' => array(
        'type' => 'text',
        'label' => t('Product SKU'),
        'description' => t('The SKU of the product to look for on the order.'),
      ),
    ),
    'group' => t('RTPR Commerce'),
    'callbacks' => array(
      'execute' => 'rtpr_commerce_order_rules_contains_product',
    ),
  );
  
  $conditions['rtpr_commerce_order_contains_product_between_date'] = array(
    'label' => t('Purchased particular SKU (wildcard) in current/last month'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
      'product_id' => array(
        'type' => 'text',
        'label' => t('Product SKU'),
        'description' => t('The SKU of the product to look for on the order.'),
      ),
      'month' => array(
        'type' => 'text',
        'label' => t('Month'),
        'options list' => 'rtpr_commerce_rules_month_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('RTPR Commerce'),
    'callbacks' => array(
      'execute' => 'rtpr_commerce_order_contains_product_between_date',
    ),
  );

  $conditions['rtpr_commerce_order_only_contains_product'] = array(
    'label' => t('Order only contains a particular SKUs'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order whose line items should be checked for the specified product.'),
      ),
      'skus' => array(
        'type' => 'list<text>',
        'label' => t('List of SKUs'),
        'options list' => '_rtpr_commerce_sku_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('RTPR Commerce'),
    'callbacks' => array(
      'execute' => 'rtpr_commerce_order_only_contains_product',
    ),
  );

    $conditions['rtpr_commerce_order_base_price_compare'] = array(
    'label' => t('Order subtotal compare'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'rules_condition_data_is_operator_options',
        'restriction' => 'input',
      ),
      'amount' => array(
        'type' => 'integer',
        'label' => t('Amount'),
        'description' => t('The value to compare the subtotal with.'),
        'restriction' => 'input',
      ),
    ),
    'group' => t('RTPR Commerce'),
    'callbacks' => array(
      'execute' => 'rtpr_commerce_order_base_price_compare',
    ),
  );
  return $conditions;
}
/**
 * Implements hook_rules_action_info();
 */
function rtpr_commerce_rules_action_info() {
  $actions = array();
  $actions['rtpr_commerce_sales_notify'] = array(
    'label' => t('Notify the Post Affiliate Pro server of a sale.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to notify'),
      ),
    ),
    'group' => t('Commerce Order'),
  );
  return $actions;
}

/**
 * Get the list of options
 */
function rtpr_commerce_rules_month_list() {
  return array(
    'current' => 'Current Month',
    'last' => 'Last Month',
  );
}


/**
 * Condition callback: checks to see if a particular SKU exists on an order
 * in the specified quantity.
 */
function rtpr_commerce_order_rules_contains_product($order, $sku) {

  // If we actually received a valid order...
  if (!empty($order)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    // Populate the array of the quantities of the products on the order.
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        // Extract a product ID and quantity from the line item.
        $line_item_sku = $line_item_wrapper->commerce_product->sku->value();
        $quantity = $line_item_wrapper->quantity->value();
        if ($quantity >= 1 && strpos($line_item_sku, $sku) !== FALSE) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}
  
/**
 * Condition callback: checks to see if a particular SKU exists on an order
 * in the specified quantity.
 */
function rtpr_commerce_order_contains_product_between_date($account, $sku, $month_selected) {
  $month = 0;
  $year = 0;
  if ($month_selected == 'current') {
    $month = date('m');
    $year = date('Y');
  }
  elseif ($month_selected == 'last') {
    $month = date('m', strtotime('last month'));
    $year = date('Y', strtotime('last month'));
  }
  
  $start_date = strtotime($year . '-' . $month . '-01');
  $end_date = strtotime($year . '-' . $month . '-31');
  
  $validate_order_ids = db_query(
    'SELECT order_id FROM commerce_order WHERE uid = :uid AND status IN (:status)',
    array(
      ':uid' => $account->uid,
      ':status' => array('completed', 'processing', 'pending')
    )
  )->fetchCol();
  
  $order_ids = db_query(
    'SELECT distinct order_id FROM commerce_payment_transaction WHERE order_id IN (:order_ids) AND created > :start_date AND created < :end_date AND status = :status',
    array(
      ':order_ids' => $validate_order_ids,
      ':start_date' => $start_date,
      ':end_date' => $end_date,
      ':status' => 'success',
    )
  )->fetchCol();

  $total = 0;
  if (!empty($order_ids)) {
    foreach ($order_ids as $order_id) {
      $order = commerce_order_load($order_id);
      if (in_array($order->status, array('completed', 'processing', 'pending'))) {
        if (rtpr_commerce_order_rules_contains_product($order, $sku)) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}


  
/**
 * Condition callback: checks to see if order ONLY a particular SKU.
 */
function rtpr_commerce_order_only_contains_product($order, $skus) {
  // If we actually received a valid order...
  $result = TRUE;
  if (!empty($order)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
     foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        // Extract a product ID and quantity from the line item.
        $line_item_sku = $line_item_wrapper->commerce_product->sku->value();
        if (!in_array($line_item_sku, $skus) && !in_array($line_item_sku, _rtpr_commerce_sku_excluded_from_shipping())) {
          $result = FALSE;
        }
      }
    }
  }

  return $result;
}

function _rtpr_commerce_sku_excluded_from_shipping() {
  return  variable_get('rtpr_commerce_exclude_skus_from_shipping', array(
      'ONLINE-GCS-T2-Credit',
      'ONLINE-SSP-GCS-T2-Credit',
      'JANUARY-GCAP-PREMIUM-PROGRAM',
      'FEBRUARY-GCAP-PREMIUM-PROGRAM',
      'MARCH-GCAP-PREMIUM-PROGRAM',
      'APRIL-GCAP-PREMIUM-PROGRAM',
      'MAY-GCAP-PREMIUM-PROGRAM',
      'JUNE-GCAP-PREMIUM-PROGRAM',
      'JULY-GCAP-PREMIUM-PROGRAM',
      'AUGUST-GCAP-PREMIUM-PROGRAM',
      'SEPTEMBER-GCAP-PREMIUM-PROGRAM',
      'OCTOBER-GCAP-PREMIUM-PROGRAM',
      'NOVEMBER-GCAP-PREMIUM-PROGRAM',
      'DECEMBER-GCAP-PREMIUM-PROGRAM',
      'FOREMAN-GCAP-PREMIUM-PROGRAM',
      'JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'HDSAFL-JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'HDSAFL-JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'HOP-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'DOUBLE-HOP-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-PLUS-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'KNOCKOUT-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'FOREMAN-COMBO-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-SPRAY-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-ROLL-On-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'HOP-SPRAY-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'HOP-ROLL-On-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'KNOCKOUT-SPRAY-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'KNOCKOUT-ROLL-On-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'Special-One-GCAP-PREMIUM-PROGRAM',
      'Special-Two-GCAP-PREMIUM-PROGRAM',
      'Special-Three-GCAP-PREMIUM-PROGRAM',
      'Special-Four-GCAP-PREMIUM-PROGRAM',
      'Special-Five-GCAP-PREMIUM-PROGRAM',
      'Special-Six-GCAP-PREMIUM-PROGRAM',
      'Special-Seven-GCAP-PREMIUM-PROGRAM',
      'Special-Eight-GCAP-PREMIUM-PROGRAM',
      'Special-Nine-GCAP-PREMIUM-PROGRAM',
      'Special-Ten-GCAP-PREMIUM-PROGRAM',
      'Special-Eleven-GCAP-PREMIUM-PROGRAM',
      'Special-Twelve-GCAP-PREMIUM-PROGRAM',
      'SEPTEMBER-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'FREE-100-MAXX-TRAVEL-PACKS',
      'FREE-10-KNOCKOUT-GOPAKS-PACKS'
    ));
}

/**
 * Condition callback: compare subtotal of the order with given amount.
 */
function rtpr_commerce_order_base_price_compare($order, $op, $amount) {
  if (!empty($order)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $subtotal = 0;
    $exclude_skus = variable_get('rtpr_commerce_exclude_skus_from_shipping', array(
      'ONLINE-GCS-T2-Credit',
      'ONLINE-SSP-GCS-T2-Credit',
      'JANUARY-GCAP-PREMIUM-PROGRAM',
      'FEBRUARY-GCAP-PREMIUM-PROGRAM',
      'MARCH-GCAP-PREMIUM-PROGRAM',
      'APRIL-GCAP-PREMIUM-PROGRAM',
      'MAY-GCAP-PREMIUM-PROGRAM',
      'JUNE-GCAP-PREMIUM-PROGRAM',
      'JULY-GCAP-PREMIUM-PROGRAM',
      'AUGUST-GCAP-PREMIUM-PROGRAM',
      'SEPTEMBER-GCAP-PREMIUM-PROGRAM',
      'OCTOBER-GCAP-PREMIUM-PROGRAM',
      'NOVEMBER-GCAP-PREMIUM-PROGRAM',
      'DECEMBER-GCAP-PREMIUM-PROGRAM',
      'FOREMAN-GCAP-PREMIUM-PROGRAM',
      'JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'HDSAFL-JUNE-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'HDSAFL-JULY-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'HDSAFL-GCAP-PREMIUM-PROGRAM-PACKAGE-ONE',
      'HOP-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'DOUBLE-HOP-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-PLUS-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'KNOCKOUT-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'FOREMAN-COMBO-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-SPRAY-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'MAXX-ROLL-On-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'HOP-SPRAY-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'HOP-ROLL-On-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'KNOCKOUT-SPRAY-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'KNOCKOUT-ROLL-On-GCAP-PREMIUM-PROGRAM-PACKAGE',
      'Special-One-GCAP-PREMIUM-PROGRAM',
      'Special-Two-GCAP-PREMIUM-PROGRAM',
      'Special-Three-GCAP-PREMIUM-PROGRAM',
      'Special-Four-GCAP-PREMIUM-PROGRAM',
      'Special-Five-GCAP-PREMIUM-PROGRAM',
      'Special-Six-GCAP-PREMIUM-PROGRAM',
      'Special-Seven-GCAP-PREMIUM-PROGRAM',
      'Special-Eight-GCAP-PREMIUM-PROGRAM',
      'Special-Nine-GCAP-PREMIUM-PROGRAM',
      'Special-Ten-GCAP-PREMIUM-PROGRAM',
      'Special-Eleven-GCAP-PREMIUM-PROGRAM',
      'Special-Twelve-GCAP-PREMIUM-PROGRAM',
      'SEPTEMBER-GCAP-PREMIUM-PROGRAM-PACKAGE-TWO',
      'FREE-100-MAXX-TRAVEL-PACKS'
    ));
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      $valid_types = commerce_product_line_item_types();
      if (in_array($line_item_wrapper->type->value(), $valid_types)) {
        $line_item = $line_item_wrapper->value();
        $sku = $line_item->line_item_label;
        if (!in_array($sku, $exclude_skus)){
          $line_item_amount = $line_item_wrapper->commerce_total->amount->value();
          $subtotal += $line_item_amount;
        }
      }
    }
  }

  switch ($op) {
    case '<':
      return $subtotal < $amount;

    case '<=':
      return $subtotal <= $amount;

    case '=':
      return $subtotal == $amount;

    case '>=':
      return $subtotal >= $amount;

    case '>':
      return $subtotal > $amount;
  }
  return FALSE;
}

/**
 * Rules action: Notifies PAP of a sale
 */
function rtpr_commerce_sales_notify($commerce_order) {
  $account_id = variable_get('commerce_post_affiliate_pro_merchant_id', NULL);
  if (!$account_id) {
    watchdog('rtpr_commerce', 'Could not register sale for order @order, merchant id is not set, please visit admin pages.', array('@order' => $commerce_order->order_number));
    return;
  }
  $transaction_exists = rtpr_commerce_pap_transaction_load($commerce_order->order_number);
  if ($transaction_exists) {
    watchdog('rtpr_commerce', 'Could not register sale for order @order, transactoin already exists.', array('@order' => $commerce_order->order_number));
    return;
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
  $line_items = $order_wrapper->commerce_line_items->value();


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
  $referral_id = $account_wrapper->field_pap_referral_id->value();
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

      $pap_affiliate_id = $account_wrapper->field_pap_affiliate_id->value();
      if (user_has_role(7, $account)) {
        if (!$referral_id) {
          $sale->setAffiliateID(RTPR_OFFER_DEFAULT_PAP_ID);
        } else {
          $sale->setAffiliateID(trim($referral_id));
        }
      } else {
        if ($pap_affiliate_id) {
          $sale->setAffiliateID(trim($pap_affiliate_id));
        }
      }
      
      // RTPR-264: post the credit for the sale to the purchaser.
      if (strpos($commerce_product->sku, 'KO-PP-KIT-') === 0) {
        $sale->setAffiliateID(trim($pap_affiliate_id));
        $needs_repost = variable_get('rtpr_commerce_ko_orders_repost');
        $needs_repost[$commerce_order->order_number] = $commerce_order->order_number;
        variable_set('rtpr_commerce_ko_orders_repost', $needs_repost);
      }
      // ser status to pending,
      $sale->setStatus('P');
      // name
      $sale->setData1(preg_replace("/[^a-zA-Z0-9 ]/", "", $account_wrapper->field_first_name->value()) . ' ' . preg_replace("/[^a-zA-Z0-9 ]/", "", $account_wrapper->field_last_name->value()));
      // customer email
      $sale->setData2($account_wrapper->mail->value());
      // phone number
      $sale->setData3($account_wrapper->field_phone->value());
      // User ID.
      $sale->setData5($account->uid);

    }
  }
  $result = $sale_tracker->register();

  //watchdog('commerce_post_affiliate_pro', 'Result: sale registration for order @order: @result', array('@order' => $commerce_order->order_number, '@result' => $result));
}

