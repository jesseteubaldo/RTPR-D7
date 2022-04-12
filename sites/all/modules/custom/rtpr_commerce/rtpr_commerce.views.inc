<?php

/**
 * @file
 * Coupon views extra data.
 */

/**
 * Alter the views data to enable some additional features for coupons in views.
 */
function rtpr_commerce_views_data_alter(&$data) {
  // coupon_id relationship.
  $data['commerce_coupon']['coupon_id']['relationship'] = array(
    'handler' => 'views_handler_relationship',
    'base' => 'commerce_coupon_usage_transaction',
    'base field' => 'coupon_id',
    'label' => t('Coupon ID'),
  );
}

/**
 * Implements hook_views_pre_render().
 */
function rtpr_commerce_views_pre_render(&$view) {
  if ($view->name == 'commerce_backoffice_orders') {
    $order_ids = array();
    foreach ($view->result as $index => $result) {
      $order_ids[] = $result->order_id;
    }

    if (!empty($order_ids)) {
      $bo_order_ids = db_select('commerce_line_item', 'item')
        ->fields('item', array('order_id'))
        ->condition('line_item_label', _rtpr_commerce_buck_sku_list(), 'IN')
        ->condition('order_id', $order_ids, 'IN')
        ->execute()->fetchCol();

      foreach ($view->result as $index => $result) {
        if (in_array($result->order_id, $bo_order_ids)) {
          $view->result[$index]->commerce_order_order_number = 'BO' . $result->order_id;
        }
      }
    }
  }
}

/**
 * 	Implements hook_views_query_alter().
 */
function rtpr_commerce_views_query_alter(&$view, &$query) {
  if ($view->name == 'commerce_reports_products') {
      $exposed_raw_input = $view->exposed_raw_input;
      $date = array();

      if (!empty($exposed_raw_input['date_filter'])) {
        $min = strtotime($exposed_raw_input['date_filter']['min']);
        $max = strtotime($exposed_raw_input['date_filter']['max'] . ' 23:59:59');
        $date = array($min, $max);
      }

      $conditions = $query->where[1]['conditions'];
      foreach($conditions as $index => $condition) {
        if ($condition['operator'] == 'formula') {
          unset($query->where[1]['conditions'][$index]);
        }
      }

      $join = new views_join();
      $join->table = 'commerce_payment_transaction';
      $join->field = 'order_id';
      $join->left_table = 'commerce_line_item';
      $join->left_field = 'order_id';
      $join->type = 'left';

      $query->add_relationship('commerce_payment_transaction', $join, 'commerce_order');

      $query->add_where(1,'commerce_payment_transaction.created', $date, 'BETWEEN');
      $query->add_where(1,'commerce_payment_transaction.status','success');
  }
  elseif ($view->name == 'commerce_reports_customers') {
      $exposed_raw_input = $view->exposed_raw_input;
      $date = array();

      if (!empty($exposed_raw_input['date_filter'])) {
        $min = strtotime($exposed_raw_input['date_filter']['min']);
        $max = strtotime($exposed_raw_input['date_filter']['max'] . ' 23:59:59');
        $date = array($min, $max);
      }

      $conditions = $query->where[1]['conditions'];
      foreach($conditions as $index => $condition) {
        if ($condition['operator'] == 'formula') {
          unset($query->where[1]['conditions'][$index]);
        }
      }

      $join = new views_join();
      $join->table = 'commerce_payment_transaction';
      $join->field = 'order_id';
      $join->left_table = 'commerce_order';
      $join->left_field = 'commerce_order_users.order_id';
      $join->type = 'left';

      $query->add_relationship('commerce_payment_transaction', $join, 'commerce_order');

      $query->add_where(1,'commerce_payment_transaction.created', $date, 'BETWEEN');
      $query->add_where(1,'commerce_payment_transaction.status','success');
  }
  elseif ($view->name == 'commerce_backoffice_orders') {
    $exposed_raw_input = $view->exposed_raw_input;
    if (!empty($exposed_raw_input['sanitizer'])) {
      $sanitizer_order_ids = db_query(
        "select distinct order_id from commerce_line_item where line_item_label like '%HAND-SNITIZER%' OR line_item_label like '%HAND-SANITIZER%' OR line_item_label like '%CLEANSE-and-NOURISH-KIT%'"
      )->fetchCol();
      $condition = array(
        'field' => 'commerce_order.order_id',
        'value' => $sanitizer_order_ids,
        'operator' => 'NOT IN',
      );
      $query->where[1]['conditions'][] = $condition;
    }
  }
}