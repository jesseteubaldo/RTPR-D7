<?php

/**
 * @file
 *
 */

function rtpr_reward_points_import_form($form, $form_state) {
  $form['type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => _rptr_reward_point_types(),
    '#default_value' => 'purchase',
    '#required' => TRUE,
  );

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#description' => t('Upload a import file, allowed extensions: csv'),
    '#title_display' => 'invisible',
    '#size' => 22,
  );

  $form['comment'] = array(
    '#type' => 'textarea',
    '#title' => 'Comment',
    '#required' => TRUE,
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),

  );

  return $form;
}


function rtpr_reward_points_import_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    // Validate extensions.
    'file_validate_extensions' => array('csv'),
  ));
  // If the file passed validation:
  if ($file) {
    // Move the file into the Drupal file system.
    $imports_dir = 'public://imports/';
    file_prepare_directory($imports_dir, FILE_CREATE_DIRECTORY);
    if ($file = file_move($file, $imports_dir . $file->filename)) {
      // Save the file for use in the submit handler.
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function rtpr_reward_points_import_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $comment = $values['comment'];
  $type = $values['type'];
  $file = $form_state['storage']['file'];
  $handle = fopen($file->uri, 'r');
  fgetcsv($handle, 1024, ",");
  $success = 0;
  $failed = 0;
  $line = 1;
  while (($data = fgetcsv($handle, 1024, ",")) !== FALSE) {
    $pap_id = trim($data[0]);
    $points = $data[1];
    $month = $data[2];
    $year = $data[3];
    if ($type != 'retail') {
      $uid = db_query("SELECT p.entity_id FROM {field_data_field_pap_affiliate_id} p WHERE p.field_pap_affiliate_id_value = :value", array(':value' => $pap_id))->fetchField();
    }
    else {
      $uid = $data[0];
    }
    if ($uid && is_numeric($points)) {
      $batch_number = variable_get('rtpr_cc_rewards_point_batch_number', 0);
      ++$batch_number;
      $params = array(
        'points' => $points,
        'uid' => $uid,
        'operation' => 'admin',
        'description' => $comment,
        'tid' => 0,
        'time_stamp' => strtotime($month . '/15/' . $year),
        'moderate' => FALSE,
        'approver_uid' => $user->uid,
        'batch' => $batch_number,
        'type' => 'rewards_point',
        'subtype' => $type,
        'display' => FALSE,
        'expirydate' => 0,
      );
      $result = userpoints_userpointsapi($params);
      $transaction = $result['transaction'];
      db_update('userpoints_txn')
        ->fields(array(
        'batch' => $batch_number,
        'type' => 'rewards_point',
        'subtype' => $type,
      ))
      ->condition ('txn_id', $transaction['txn_id'], '=')
      ->execute();
      if ($result) {
        $success++;
      }
    }
    else {
      drupal_set_message(t('Record on line #@line (@id) failed to import!', array('@line' => $line, '@id' => $pap_id)), 'error');
    }
    $line++;
  }
  variable_set('rtpr_cc_rewards_point_batch_number', $batch_number);
  drupal_set_message(t('Successfully import @total records.', array('@total' => $success)));
}

/**
 * Helper function to get points status.
 */
function _rtpr_reward_points_report($uid, $start = NULL, $end = NULL) {
  module_load_include('inc', 'rtpr_pap', 'includes/helper');
  $credit = rtpr_reward_get_userpoints_txn($uid);
  $redeemed = rtpr_reward_get_userpoints_txn($uid, RTPR_COMMERCE_REDEEMED_TID);

  $credit_total = isset($credit->total) ? $credit->total : 0;
  $redeemed_total = isset($redeemed->total) ? abs($redeemed->total) : 0;
  $available = $credit_total - $redeemed_total;

  if (empty($start)) {
    $start = strtotime('first day of this month 00:00:00');
    $end = strtotime('last day of this month 23:59:59');
  }

  $qualified_amount = 0;
  $used_points = 0;
  $coop_total = 0;
  $query = db_select('commerce_payment_transaction', 't');
  $query->join('commerce_order', 'o', 't.order_id = o.order_id');
  $query->fields('o', array('order_id'))
    ->condition('t.status', 'success', '=')
    ->condition('o.uid', $uid)
    ->condition('t.created', array($start, $end), 'BETWEEN');
  $completed_order_ids = $query->execute()->fetchCol();

  if (!empty($completed_order_ids)) {
    $order_ids = array_unique($completed_order_ids);
    foreach($order_ids as $order_id) {
      $order = commerce_order_load($order_id);
      if ($order->status != 'canceled') {
        try {
          $coop_items = array();
          $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
          $line_items = array();
          $used_points_amount = commerce_line_items_total($order_wrapper->commerce_line_items->value(), array('userpoints'));
          $used_points += abs($used_points_amount['amount']);
          foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
            $exclude = FALSE;
            $line_item = $line_item_wrapper->value();
            if (strpos($line_item->line_item_label, 'CO-OP') !== FALSE) {
              $coop_items[] = $line_item;
            }
/*
            if (strpos($line_item->line_item_label, 'PROMOTIONAL-CO-OP-KIT') !== FALSE) {
              $exclude = TRUE;
            }
            if (strpos($line_item->line_item_label, 'ONLINE-GCS-T2-Credit') !== FALSE) {
              $exclude = TRUE;
            }
*/
            if (!$exclude) {
              $line_items[] = $line_item;
            }
          }

          if (!empty($coop_items)) {
            $coop_subtotal = commerce_line_items_total($coop_items, array('product'));
            if ($coop_subtotal['amount'] > 0) {
              $coop_total += $coop_subtotal['amount'];
            }
          }

          if (!empty($line_items)) {
            $subtotal = commerce_line_items_total($line_items, array('product'));
            if ($subtotal['amount'] > 0) {
              $qualified_amount += $subtotal['amount'];
            }
          }
        }
        catch (EntityMetadataWrapperException $exc) {
          watchdog(
            'rtpr_reward',
            'EntityMetadataWrapper exception in %function() @trace<',
            array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
            WATCHDOG_ERROR
          );
        }
      }
    }
  }
  $greater_points = $coop_total > $used_points ? $coop_total :$used_points;
  $total = $qualified_amount - $greater_points;
  return array(
    'available' => $available,
    'qualified_amount' => $qualified_amount,
    'multiplication_factor' => _rtpr_reward_get_points_multiplication_factor($total),
    'redeemed_total' => $redeemed_total,
    'used_points' => $used_points,
    'coop_total' => $coop_total,
    'total' => $total,
  );
}
/**
 * Points report for Reward role.
 */
function rtpr_reward_points_itemization_form($form, &$form_state, $uid) {
  $points_stats = _rtpr_reward_points_report($uid);
  $total_earned = _rtpr_reward_get_points_for_category($uid);

  $form['title'] = array(
    '#markup' => '<h2 class="text-center">Wholesale Rewards Points Summary</h2>',
  );

  $form['all_time_stats_title'] = array(
    '#markup' => '<h3 class="text-center">All Time Points Stats</h3>',
  );

  $form['all_time_stats'] = array(
  '#markup' => '<div class="row row-eq-height">
  <div class="col-xs-12 col-sm-6 col-md-4 col-lg-4 color-rtpr">
  <p class="text-center">Credits Available</p>
  <h3 class="text-center">' . $points_stats['available'] . '</h3>
  <p class="text-center">Points Never Expire</p>
  <p class="text-center"><a href="/shop" class="bold underline">Click Here to Shop</a></p>
  </div>
  
  <div class="col-xs-12 col-sm-12 col-md-4 col-lg-4 color-green">
   <p class="text-center">Rewards Points</p>
  <p class="text-center">EARNED</p>
  <h3 class="text-center">' . $total_earned . '</h3>
  <p class="text-center"><a href="/user/' . $uid . '/rewards-itemization/earned" class="bold underline">Click Here to View All Earned Points</a></p>
  </div>

  <div class="col-xs-12 col-sm-12 col-md-4 col-lg-4 color-orange">
  <p class="text-center">Total All Time Rewards Points</p>
  <p class="text-center">REDEEMED</p>
  <h3 class="text-center"> ' . $points_stats['redeemed_total'] . '</h3>
  <p class="text-center"><a href="/user/' . $uid . '/rewards-itemization/redeemed" class="bold underline">Click Here to View All Redeemed Points</a></p></div>
  </div>',
  );

  return $form;
}

/**
 * Ajax callback for month and year in rtpr_reward_points_itemization_form.
 */
function rtpr_reward_prior_stats_dropdown_callback($form, &$form_state) {
  $values = $form_state['values'];
  $form['prior_stats_grid']['result'] = _rtpr_reward_points_stats_grid($values['uid'], $values['month'], $values['year']);
  return $form['prior_stats_grid'];

}


function _rtpr_reward_points_stats_grid($uid, $month, $year) {
  $stats['posted_stats_row_1'] = array(
    '#markup' => '<div class="row">
<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-cryan">
<p class="text-center">Total Points Earned</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year) . '</h3>
<p class="text-center">Total Rewards Points Earned from all categories</p>
</div>

<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">Purchase Rewards Points</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "purchase") . '</h3>
<p class="text-center">Points Earned from purchases last month based on your Multiplication Factor</p>
</div>

<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">Customer Referral Points</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "customer") . '</h3>
<p class="text-center">Points Earned from referring Retail Customers to RTPR.com</p>
</div>

<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">Team Referral Points</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "team") . '</h3>
<p class="text-center">Points Earned from referring others to the Wholesale Rewards Program</p>
</div>
</div>',
  );

  $fast = _rtpr_reward_get_points_for_category($uid, $month, $year, "fast");
  $commission = _rtpr_reward_get_points_for_category($uid, $month, $year, "commission");
  $misc = _rtpr_reward_get_points_for_category($uid, $month, $year, "misc");
  $afl = _rtpr_reward_get_points_for_category($uid, $month, $year, "afl");
  if ($fast || $commission || $misc || $afl) {
  $stats['posted_stats_row_2'] = array(
    '#markup' => '<div class="row">
<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">New Member Bonus</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "fast") . '</h3>
</div>

<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">Bonus Points</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "commission") . '</h3>
</div>

<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">Miscellaneous Points</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "misc") . '</h3>
</div>

<div class="col-xs-12 col-sm-3 col-md-3 col-lg-3 color-blue">
<p class="text-center">AFL Contribution</p>
<h3 class="text-center">'  . _rtpr_reward_get_points_for_category($uid, $month, $year, "afl") . '</h3>
</div>
</div>',
  );
  }

  return $stats;

}


function _rtpr_reward_get_points_for_category($uid, $month = NULL, $year = NULL, $category = NULL) {
  $point_query = db_select('userpoints_txn', 't')
    ->fields('t', array('points'))
    ->condition('type', 'rewards_point')
    ->condition('uid', $uid);

  if ($category) {
    $point_query->condition('subtype', $category);
  }

  if ($month) {

  	$dateObj   = DateTime::createFromFormat('!m', $month);
    $monthName = $dateObj->format('F');
    $start = strtotime('first day of ' . $monthName . ' ' . $year . '00:00:00');
    $end = strtotime('last day of' . $monthName . ' ' . $year . '23:59:59');

    $point_query->condition('time_stamp', array($start, $end), 'BETWEEN');
  }
  $point_query->addExpression('SUM(points)', 'total');
  $points = $point_query->execute()->fetch();
  return (int)$points->total;
}
/**
 * Form callback for /user/%/rewards-itemization/earned.
 */
function rtpr_reward_points_itemization_earned_form($form, &$form_state, $uid, $type) {
  if ($type == 'rewards-itemization') {
    $form['type'] = array(
      '#type' => 'select',
      '#options' => array('all' => t('All')) + _rptr_reward_point_types(),
      '#default_value' => 'all',
      '#ajax' => array(
        'callback' => 'rtpr_reward_point_dropdown_callback',
        'wrapper' =>  'reward_earned_summary_form',
      )
    );
  }

  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'type' => t('Type'),
    'comment' => t('Comment'),
    'points' => t('Points'),
  );

  $rows = _rtpr_reward_get_point_rows($uid);

  $form_state['uid'] = $uid;
  $form_state['header'] = $header;

  $form['summary'] = array(
    '#prefix' => '<div id="reward_earned_summary_form">',
    '#suffix' => '</div>',
  );

  $form['summary']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );

  return $form;
}

/**
 * Ajax callback when reward drop-down change.
 */
function rtpr_reward_point_dropdown_callback($form, &$form_state) {
  $values = $form_state['values'];
  $type = $values['type'];
  $uid = $form_state['uid'];
  $header = $form_state['header'];

  $form['summary']['table']['#rows'] =  _rtpr_reward_get_point_rows($uid, $type);

  return $form['summary'];
}

/**
 * Get reward point rows.
 */
function _rtpr_reward_get_point_rows($uid, $subtype = NULL) {
  $query = db_select('userpoints_txn', 't')
    ->fields('t', array('time_stamp', 'subtype', 'description', 'points'))
    ->condition('uid', $uid)
    ->condition('points', 0, '>')
    ->condition('type', 'rewards_point');

  if ($subtype && $subtype != 'all') {
    $query->condition('subtype', $subtype);
  }

  $transactions = $query->execute()->fetchAll();

  $rows = array();
  $total = 0;
  foreach($transactions as $transaction) {
    $total += $transaction->points;
    $point_type = _rptr_reward_point_types($transaction->subtype);
    $rows[] = array(
      'month' => date('m', $transaction->time_stamp),
      'year' => date('Y', $transaction->time_stamp),
      'type' => $point_type ? $point_type : 'Bonus',
      'comment' => $transaction->description,
      'points' => $transaction->points,
    );
  }

  $rows[] = array(
    'data' => array(
      'month' =>  '',
      'year' => '',
      'type' => '',
      'comment' => 'Total',
      'points' => $total,
    ),
    'class' => array('last-row total'),
  );
  return $rows;
}

function rtpr_reward_points_itemization_redeemed_form($form, $form_state, $uid) {
  $form['title'] = array(
    '#markup' => '<p class="text-center">Redeemed Summary</p>',
  );

  $query = db_select('userpoints_txn', 't')
    ->fields('t', array('time_stamp', 'subtype', 'description', 'points', 'entity_id'))
    ->condition('uid', $uid)
    ->condition('status', 0)
    ->condition('tid', 101);
  $transactions = $query->execute()->fetchAll();

  $header = array(
    'points' => t('Points'),
    'date' => t('Date'),
    'comment' => t('Comment'),
    'used_on' => t('Used on'),
  );

  foreach($transactions as $transaction) {
    $rows[] = array(
      'points' => $transaction->points,
      'date' => date('m/d/Y H:i:s', $transaction->time_stamp),
      'comment' => $transaction->description,
      'used_on' => $transaction->entity_id ? l($transaction->entity_id, 'user/' . $uid . '/orders/' . $transaction->entity_id, array('attributes' => array('target'=>'_blank'))) : 'N/A'

    );
  }

  $form['summary']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );

  return $form;
}

/**
 * Return Multiplication Factor base on qualify purchase.
 */
function _rtpr_reward_get_points_multiplication_factor($amount) {
  if ($amount >= 100 && $amount <= 24999) {
    return 5;
  }
  elseif ($amount > 24999 && $amount <= 49999) {
    return 10;
  }
  elseif ($amount > 49999 && $amount <= 99999) {
    return 15;
  }
  elseif ($amount > 99999 && $amount <= 199999) {
    return 20;
  }
  elseif ($amount > 199999) {
    return 25;
  }

  return 0;
}


/**
 * Callback for user/%/retail-rewards-itemization
 */
function rtpr_reward_reatil_points_itemization_form($form, $form_state, $uid) {
  module_load_include('inc', 'rtpr_pap', 'includes/helper');
  $credit = rtpr_reward_get_userpoints_txn($uid);
  $redeemed = rtpr_reward_get_userpoints_txn($uid, RTPR_COMMERCE_REDEEMED_TID);

  $credit_total = isset($credit->total) ? $credit->total : 0;
  $redeemed_total = isset($redeemed->total) ? abs($redeemed->total) : 0;
  $available = $credit_total - $redeemed_total;

 $form['stats_summary'] = array(
    '#markup' => '<div class="row row-eq-height"><div class="col-xs-12 col-sm-4 col-md-4 col-lg-4 color-rtpr">
<p class="text-center">Purchases Credits Available</p>
<h3 class="text-center">' . $available . '</h3>
<p>&nbsp;</p>
<p class="text-center"><a href="/shop" class="bold underline">Click Here to Shop</a></p></div>
<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4 color-green">
<p class="text-center">Total Rewards Points Earned</p>
<h3 class="text-center"> ' . $credit_total . '</h3>
<p>&nbsp;</p>
<p class="text-center"><a href="/user/'. $uid . '/retail-rewards-itemization/earned" class="bold underline">Click Here to View All Earned Points</a></p></div>
<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4 color-orange">
<p class="text-center">Total Rewards Points Redeemed</p>
<h3 class="text-center"> ' . $redeemed_total . '</h3>
<p>&nbsp;</p>
<p class="text-center"><a href="/user/'. $uid . '/retail-rewards-itemization/redeemed" class="bold underline">Click Here to View All Redeemed Points</a></p></div></div>',
  );

  return $form;
}
