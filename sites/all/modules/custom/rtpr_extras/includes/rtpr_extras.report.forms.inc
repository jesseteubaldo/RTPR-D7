<?php

/**
 * @file
 */

function rtpr_extras_report_tracking_form($form, $form_state) {

  $query = db_select('tracking_numbers', 't');
  $query->join('commerce_order', 'c', 'c.order_id = t.order_id');
  $query->condition('c.status', array('completed','canceled'), 'NOT IN');
  $query->fields('t', array('order_id'));
  $query->fields('c', array('status'));

  $results = $query->execute()->fetchAllAssoc('order_id');

  $header = array(
    'id' => t('Order ID'),
    'status' => t('Status'),
  );

  $rows = array();
  foreach($results as $order_id =>  $result) {
    $rows[$order_id] = array(
      'id' => l($order_id, 'admin/commerce/orders/'.$order_id),
      'status' => $result->status,
    );
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('All order with tracking number are in completed status.'),
  );
  return $form;
}

