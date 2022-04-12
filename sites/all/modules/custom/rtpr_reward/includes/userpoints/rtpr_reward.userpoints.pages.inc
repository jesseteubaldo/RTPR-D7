<?php

/**
 * @file
 *
 * Customize userpoint.
 */
  
  /**
 * Menu callback; display details about a specific transaction.
 *
 * @param $transaction
 *   Transaction object.
 * @return
 *   Render-able array with all the information about this transaction.
 */
function rtpr_reward_userpoints_view_transaction($transaction) {
  drupal_add_css(drupal_get_path('module', 'userpoints') . '/userpoints.css');

  drupal_set_title(t('View transaction #@txn_id', array('@txn_id' => $transaction->txn_id)));

  $css_stati = array(
    USERPOINTS_TXN_STATUS_APPROVED => 'approved',
    USERPOINTS_TXN_STATUS_DECLINED => 'declined',
    USERPOINTS_TXN_STATUS_PENDING => 'pending',
  );
  $classes = 'userpoints-view-' . $css_stati[$transaction->status] . ' userpoints-view-category-' . $transaction->tid . ' userpoints-view-' . ($transaction->points > 0 ? 'positive' : 'negative');
  if (!empty($transaction->expirydate)) {
    $classes .= $transaction->expired ? ' userpoints-view-expired' : ' userpoints-view-not-expired';
  }

  $content = array(
    '#prefix' => '<div class="userpoints-view-transaction ' . $classes . '">',
    '#suffix' => '</div>',
  );

  $content['details'] = array(
    '#theme' => 'userpoints_view_category',
    '#title' => t('Details'),
    '#weight' => 0,
    '#attributes' => array('class' => array('userpoints-group-details')),
  );

  $content['details']['user'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('User'),
    '#value' => theme('username', array('account' => $transaction->user)),
    '#weight' => 0,
    '#attributes' => array('class' => array('userpoints-item-user')),
  );

  $content['details']['points'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('!Points', userpoints_translation()),
    '#value' => $transaction->points,
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-item-points')),
  );

  $content['details']['category'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Category'),
    '#value' => $transaction->category,
    '#weight' => 20,
    '#attributes' => array('class' => array('userpoints-item-category')),
  );

  $content['details']['reason'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Reason'),
    '#value' => userpoints_create_description($transaction, array('truncate' => FALSE)),
    '#weight' => 30,
    '#attributes' => array('class' => array('userpoints-item-reason')),
  );

  $content['details']['transaction'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Transaction ID'),
    '#value' => $transaction->txn_id,
    '#weight' => 40,
    '#attributes' => array('class' => array('userpoints-item-transaction')),
  );

  $content['status'] = array(
    '#theme' => 'userpoints_view_category',
    '#title' => t('Status'),
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-group-status')),
  );

  $stati = userpoints_txn_status();
  $content['status']['status'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Approval status'),
    '#value' => $stati[$transaction->status],
    '#weight' => 0,
    '#attributes' => array('class' => array('userpoints-item-status')),
  );

  $content['status']['date'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Creation date'),
    '#value' => format_date($transaction->time_stamp),
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-item-date')),
  );

  $content['status']['changed'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Last modified'),
    '#value' => format_date($transaction->changed),
    '#weight' => 20,
    '#attributes' => array('class' => array('userpoints-item-changed')),
  );

  if (!empty($transaction->expirydate)) {
    $content['status']['expiration_status'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Expiration status'),
      '#value' => $transaction->expired ? t('Expired') : t('Not expired'),
      '#weight' => 20,
      '#attributes' => array('class' => array('userpoints-item-expiration-status')),
    );
    $content['status']['expiration_date'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Expiration date'),
      '#value' => format_date($transaction->expirydate),
      '#weight' => 30,
      '#attributes' => array('class' => array('userpoints-item-points-expiration-date')),
    );
  }

  if (!empty($transaction->parent_txn_id)) {
    $parent_transaction = userpoints_transaction_load($transaction->parent_txn_id);
    $parent = l(userpoints_create_description($parent_transaction, array('link' => FALSE)), 'userpoints/view/' . $transaction->parent_txn_id, array('html' => TRUE));
  }

  $child_txn_ids = db_query('SELECT txn_id FROM {userpoints_txn} WHERE parent_txn_id = :txn_id', array(':txn_id' => $transaction->txn_id))->fetchCol();
  $children = array();
  foreach ($child_txn_ids as $child_txn_id) {
    $child_transaction = userpoints_transaction_load($child_txn_id);
    $children[] = l(userpoints_create_description($child_transaction, array('link' => FALSE)), 'userpoints/view/' . $child_txn_id, array('html' => TRUE));
  }
  $children = !empty($children) ? theme('item_list', array('items' => $children)) : '';

  if (!empty($parent) || !empty($children)) {
    $content['related'] = array(
      '#theme' => 'userpoints_view_category',
      '#title' => t('Related !points transactions', userpoints_translation()),
      '#weight' => 20,
      '#attributes' => array('class' => array('userpoints-group-related')),
    );

    if (!empty($parent)) {
      $content['related']['parent'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Prior transaction'),
        '#value' => $parent,
        '#weight' => 0,
        '#attributes' => array('class' => array('userpoints-item-parent')),
      );
    }

    if (!empty($children)) {
      $content['related']['children'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Follow-up transactions'),
        '#value' => $children,
        '#weight' => 10,
        '#attributes' => array('class' => array('userpoints-item-children')),
      );
    }
  }

  if (userpoints_admin_access('edit')) {
    $content['admin'] = array(
      '#theme' => 'userpoints_view_category',
      '#title' => t('Admin'),
      '#weight' => 30,
      '#attributes' => array('class' => array('userpoints-group-admin')),
    );

    if (!empty($transaction->approver_uid)) {
      $content['admin']['moderator'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Moderator'),
        '#value' => theme('username', array('account' => user_load($transaction->approver_uid))),
        '#weight' => 0,
        '#attributes' => array('class' => array('userpoints-item-moderator')),
      );
    }

    if (!empty($transaction->description)) {
      $order = commerce_order_load($transaction->reference);
      $content['admin']['description_manual'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Description (manually entered)'),
        '#value' => l($transaction->description, 'https://rtpr.com/user/' . $order->uid, array('attributes' => array('target' => '_blank'))),
        '#weight' => 10,
        '#attributes' => array('class' => array('userpoints-item-description-manual')),
      );

      $content['admin']['description_generated'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Description (auto generated)'),
        '#value' => userpoints_create_description($transaction, array('skip_description' => TRUE, 'truncate' => FALSE)),
        '#weight' => 20,
        '#attributes' => array('class' => array('userpoints-item-description-generated')),
      );
    }

    $content['admin']['operation'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Operation'),
      '#value' => $transaction->operation,
      '#weight' => 30,
      '#attributes' => array('class' => array('userpoints-item-operation')),
    );

    if (!empty($transaction->reference)) {
      $content['admin']['reference'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Order Number'),
        '#value' => l($transaction->reference, 'https://rtpr.com/admin/commerce/orders/' . $transaction->reference . '/print', array('attributes' => array('target' => '_blank'))),
        '#weight' => 40,
        '#attributes' => array('class' => array('userpoints-item-reference')),
      );
    }

    $content['admin']['actions'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Actions'),
      '#value' => userpoints_get_transaction_actions($transaction, FALSE),
      '#weight' => 50,
      '#attributes' => array('class' => array('userpoints-item-actions')),
    );
  }
  return $content;
}


/**
 * Displays a list of transactions.
 *
 */
function rtpr_reward_userpoints_admin_transactions($form, &$form_state, $moderate) {

  // If this is an AJAX request, update $_GET['q'] so that table sorting and
  // similar links are using the correct base path.
  if ($_GET['q'] == 'system/ajax') {
    $_GET['q'] = $moderate ? 'admin/config/people/userpoints/moderate' : 'admin/config/people/userpoints/transaction';
  }

  $settings = array(
    'show_status' => !$moderate,
  );
  $header = userpoints_get_transaction_header($settings);
  $query = db_select('userpoints_txn', 'p')->extend('PagerDefault')->extend('TableSort')
    ->fields('p')
    ->orderByHeader($header)
    // Enforce consistent sort order.
    ->orderBy('p.txn_id', 'DESC')
    ->limit(variable_get(USERPOINTS_REPORT_USERCOUNT, 30));

  if ($moderate) {
    $query->condition('p.status', USERPOINTS_TXN_STATUS_PENDING);
  }

  if (module_exists('taxonomy')) {
    $query->leftJoin('taxonomy_term_data', 't', 'p.tid = t.tid');
  }

  $values = userpoints_filter_parse_input($form_state);
  $active_category = userpoints_filter_query($query, $values);

  if ($moderate) {
    if (isset($active_category)) {
      drupal_set_title(t('Moderation (%category category)', userpoints_translation() + array('%category' => $active_category)), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Moderation'));
    }
  }
  else {
    if (isset($active_category)) {
      drupal_set_title(t('Transactions (%category category)', userpoints_translation() + array('%category' => $active_category)), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Transactions'));
    }
  }
  
  $header[] = array(
    'data' => t('Order #'),
    'class' => array('userpoint-transactions-header-entity-id'),
  );
  $header[] = array(
    'data' => t('Order Total'),
    'class' => array('userpoint-transactions-header-order-total'),
  );
  $rows = array();
  foreach ($query->execute() as $index => $transaction) {
    $rows[$index] = userpoints_get_transaction_row($transaction, $settings);
    $order_number = $transaction->entity_id;
    if (!$order_number) {
      $order_number = $transaction->reference;
    }
    if (is_numeric($order_number)) {
      $rows[$index]['data'][] = array(
        'data' => l($order_number, 'https://rtpr.com/admin/commerce/orders/' . $order_number, array('attributes' => array('target' => '_blank'))),
        'class' => array('entity-id'),
      );
      $order = commerce_order_load($order_number);
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
        $order_total = $order_wrapper->commerce_order_total->value();
      $rows[$index]['data'][] = array(
        'data' => commerce_currency_format($order_total['amount'], $order_total['currency_code']),
        'class' => array('entity-id'),
      );
    }
    else {
      $rows[$index]['data'][] = array();
      $rows[$index]['data'][] = array();
    }
  }

  // Store context in the output array so that modules have access to it.
  $output = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'userpoints') . '/userpoints.css',
      ),
    ),
  );
  $output['form'] = userpoints_filter_form(NULL, $values);

  $output['list'] = array(
    '#type' => 'container',
    '#id' => 'userpoints_list_wrapper',
  );

  $output['list']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => $moderate ? t('No !points awaiting moderation', userpoints_translation()) : t('No !Points earned', userpoints_translation()),
    '#weight' => -5,
    '#attributes' => array('class' => array($moderate ? 'userpoints-moderation-list' : 'userpoints-transactions-list')),
  );
  $output['list']['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 0,
  );

  return $output;
}
