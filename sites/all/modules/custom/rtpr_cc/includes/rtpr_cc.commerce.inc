<?php

/**
 * @file
 *
 */
 
function rtpr_cc_fix_order_form($form, $form_state) {
  $form['uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User UID'),
    '#required' => TRUE,
  );
  
  $form['fix'] = array(
    '#type' => 'submit',
    '#value' => t('Fix')
  );
  
  return $form;
}

function rtpr_cc_fix_order_form_validate($form, $form_state) {
  $uid = $form_state['values']['uid'];
  if (!user_load($uid)) {
    form_set_error('uid', t('Invalid UID!'));
  }  
}
function rtpr_cc_fix_order_form_submit($form, $form_state) {
  $uid = $form_state['values']['uid'];
  
  if (!empty($uid) && user_load($uid)) {
    $order = commerce_cart_order_load($uid);
    commerce_order_save($order);
    drupal_set_message(t('Fixed order <a target="_blank" href="/admin/commerce/orders/@order_id">@order_id</a>.', array('@order_id' => $order->order_id, '@order_id' => $order->order_id)));
  }
}

/**
 * Callback form for admin/config/rtpr/cc/repost-sp.
 */
function rtpr_cc_repost_to_shipstation_form($form, $form_state) {
  $options = array(
    'method' => 'GET',
    'timeout' => 15,
    'headers' => array(
      'Authorization' => _rtpr_commerce_get_shipstation_key()
    ),
  );
  $result = drupal_http_request('https://ssapi.shipstation.com/orders?sortBy=CreateDate&sortDir=DESC&pageSize=100', $options);
  $json = drupal_json_decode($result->data, TRUE);
  $sp_orders = array();
  foreach ($json['orders'] as $order) {
    $order_number = explode('-', $order['orderNumber']);
    $sp_orders[] = $order_number[0];
  }
  

            
  $order_ids = db_query(
    "SELECT order_id FROM commerce_order,field_data_field_order_warehouse WHERE order_id = entity_id AND status = 'processing' AND  field_order_warehouse_value != 'no_shipping_required' ORDER BY order_id DESC"
  )->fetchCol();
  
  if (!empty($order_ids)) {
    $order_ids = db_query(
      "select order_id from commerce_payment_transaction where created > :created and order_id IN (:ids)",
      array(
        ':created' => strtotime('10/31/2020'),
        ':ids'  => $order_ids,
      )
    )->fetchCol();
  }
  
  $diff = array_diff($order_ids, $sp_orders);
  $rows = array();
  $posted_ids = variable_get('rtpr_cc_ids_posted_to_sp', array());
  foreach ($diff as $id) {
    if (!in_array($id, $posted_ids)) {
      $result = drupal_http_request('https://ssapi.shipstation.com/orders?orderNumber=' . $id, $options);
      $json = drupal_json_decode($result->data, TRUE);
      if (!$json['orders']) {
        $rows[$id] = array(
          'order_id' => $id,
        );
      }
      else {
        $posted_ids[$id] =  $id;
      }
    }
  }

  $headers = array(
    'order_id' => t('Order ID'),
  );
  variable_set('rtpr_cc_ids_posted_to_sp',  $posted_ids);
  $form['order_ids'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $rows,
    '#description' => t('Select an order to post to ShipStation.'),
    '#empty' => t('No processing order found.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Post to ShipStation'),
  );
  
  return $form;
}

/**
 * Submit handler for rtpr_cc_repost_to_shipstation_form().
 */
function rtpr_cc_repost_to_shipstation_form_submit($form, $form_state) {
  $order_ids = $form_state['values']['order_ids'];
  foreach ($order_ids as $order_id) {
    $order =  commerce_order_load($order_id);
    try {
      
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $warehouse = $order_wrapper->field_order_warehouse->value();
      $order_wrapper->field_order_warehouse->set('no_shipping_required');
      $order_wrapper->save();
      
      $order =  commerce_order_load($order_id);
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $order_wrapper->field_order_warehouse->set($warehouse);
      $order_wrapper->save();
      
    }
    catch (EntityMetadataWrapperException $exc) {
      watchdog(
        'rtpt_commerce',
        'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
        array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
        WATCHDOG_ERROR
      );
    }
  }
}
