<?php

/**
 * Order related features.
 */

function rtpr_cc_abnormal_orders_form($form, $form_state) {
  $fixed_orders = variable_get('rtpr_cc_fixed_abnormal_orders', array(1));
  $orders_ids = db_query('SELECT p.order_id, changed from field_data_commerce_order_total as t, commerce_payment_transaction as p WHERE p.order_id = t.entity_id AND amount != commerce_order_total_amount AND order_id NOT IN (:fixed_orders) AND p.status = :payment_status ORDER BY order_id DESC', array(':payment_status' => 'success', ':fixed_orders' => $fixed_orders))->fetchAll();

  $header = array(
    'order_id' => t('Order ID'),
    'changed' => t('Paid date'),
  );

  $wrong_status = db_query('SELECT o.order_id, t.changed from commerce_order as o, commerce_payment_transaction as t WHERE t.order_id = o.order_id AND t.status = :tstatus AND o.status NOT IN (:status) AND o.order_id NOT IN (:fixed_orders) ORDER BY o.order_id DESC', array(':tstatus' => 'success', ':status' => array('canceled', 'pending', 'processing', 'completed'), ':fixed_orders' => $fixed_orders))->fetchAll();

  $orders_ids = array_merge($orders_ids,$wrong_status);
  $rows = array();
  
  foreach ($orders_ids as $order) {
    $order_id = $order->order_id;
    $rows[$order_id] = array(
      'order_id' => l($order_id, 'admin/commerce/orders/' . $order_id, array('attributes' => array('target'=>'_blank'))),
      'changed' => date('m/d/Y', $order->changed),
    );
  }

  arsort($rows);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Mark selected orders as fixed'),
  );
  $form['orders'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#multiple' => TRUE,
    '#empty' => t('No abnormal orders!'),
  );

  return $form;
}

function rtpr_cc_abnormal_orders_form_submit($form, $form_state) {
  $orders = $form_state['values']['orders'];
  $selected_orders = array_filter($orders);
  $fixed_orders = variable_get('rtpr_cc_fixed_abnormal_orders', array());
  $fixed_orders = array_merge(array_keys($selected_orders), $fixed_orders);
  variable_set('rtpr_cc_fixed_abnormal_orders', array_unique($fixed_orders));

  drupal_set_message('Fixed orders: ' . implode(',', $selected_orders));
}