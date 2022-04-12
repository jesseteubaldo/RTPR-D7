<?php

/**
 * @file
 * Implement various views hooks.
 */

function _rtpr_cc_user_buck_data($account, $sku) {
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $uid = $account->uid;
  $orders = commerce_order_load_multiple(array(), array('uid' => $uid, 'status' => array('pending', 'completed', 'processing', 'waiting')));
  $buck_offer_purchased = FALSE;
  $sum = 0;
  $total = 0;
  $data = array();
  $skus = array();
  $pre_buck_date = strtotime('07/31/2018');
  $pre_buck_sum = 0;
  $post_buck_sum = 0;
  if (!empty($orders)) {
    foreach ($orders as $order) {
      $buck_order = FALSE;
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
        $line_item = $line_item_wrapper->value();
        if ($line_item->type == 'product') {
          $commerce_product = $line_item_wrapper->commerce_product->value();
          if (in_array($commerce_product->sku, _rtpr_commerce_buck_sku_list())) {
            $buck_order = TRUE;

            if ($sku == 1) {
              if ($commerce_product->sku == 'ALTERNATE-PAIN-RELIEF-OFFER') {
                $skus = array($commerce_product->sku);
                break;
              }
            }
            else {
              $skus[] = $commerce_product->sku;
            }
          }
        }
      }
      if (!$buck_order) {
        $total = $order_wrapper->commerce_order_total->value();
        if ($order->created <= $pre_buck_date) {
          $pre_buck_sum += $total['amount'];
        }
        else {
          $post_buck_sum += $total['amount'];
        }
        $sum += $total['amount'];
      }
    }
    $buck_offer_purchased = implode(',', $skus);

    $data = array(
      'purchased' => $buck_offer_purchased,
      'total' => $total,
      'pre_buck_sum' => $pre_buck_sum,
      'post_buck_sum' => $post_buck_sum,
      'sum' => $sum,
      'additional' => count($orders) > 1 ? TRUE : FALSE,
    );
  }

  return $data;
}

/**
 * Implements hook_views_pre_render().
 */
function rtpr_cc_views_pre_render(&$view) {
  // Webform submissions.
  if (isset($view->name) && $view->name == 'webform_submissions') {
    if ($view->current_display == 'default') {
     if (in_array(78299, $view->args)) {
       $view->field['value']->options['exclude'] = FALSE;
     } 
    }
  }
  if (isset($view->name) && $view->name == 'tpl') {
    $results = &$view->result;
    $end = NULL;
    $start =  NULL;
    $start_date =  NULL;
    $end_date = NULL;
    if ($view->current_display == 'views_data_export_1') {

      $refer_url = parse_url($_SERVER['HTTP_REFERER']);
      $queries = explode('&', urldecode($refer_url['query']));
      foreach ($queries as $query) {
        list($key, $value) = explode('=', $query);
        if ($key == 'start[date]' && $value) {
          $start = $value;
          $start_date = date('Y-m-d H:i:s', strtotime($value));
        }
        elseif ($key == 'end[date]' && $value) {
          $end = $value;
          $end_date = date('Y-m-d H:i:s', strtotime('last day of ' . $value  . ' 23:59:59'));
        }
      }
    }
    if (!empty($view->exposed_raw_input['start'])) {
      $start = $view->exposed_raw_input['start'];
      $start_date = date('Y-m-d H:i:s', strtotime($start));
    }

    if (!empty($view->exposed_raw_input['end'])) {
      $end = $view->exposed_raw_input['end'];
      $end_date = date('Y-m-d H:i:s', strtotime('last day of ' . $end  . ' 23:59:59'));
    }
    $to_be_removed = array();

    foreach ($results as $key => $result) {
      $account = $result->_field_data['uid']['entity'];

      $tpl_role = in_array(49, array_keys($account->roles)) ? 'Yes' : 'No';
      $results[$key]->field_field_pap_affiliate_id_1[0]['rendered']['#markup'] = $tpl_role;
      $pap_id = $result->field_field_pap_affiliate_id[0]['raw']['value'];
      $referrals_count = 0;
      $transactions = NULL;
      $campaign_ids = array('c602cb08', '11111111', 'c5d380ba');
      db_set_active('pap');
      if ($pap_id) {
        $children_ids = db_query('SELECT userid FROM qu_pap_users WHERE parentuserid = :parentuserid', array(':parentuserid' => $pap_id))->fetchCol();
        if (!empty($children_ids)) {
          db_set_active();
            $uids = db_query("SELECT entity_id FROM field_data_field_pap_affiliate_id WHERE field_pap_affiliate_id_value in (:children_ids)", array(':children_ids' => $children_ids))->fetchCol();
            if (!empty($uids)) {
              $user_ids = db_query("select distinct uid from commerce_order as co, commerce_line_item as cli where co.order_id = cli.order_id and line_item_label LIKE '%PROMOTIONAL-CO-OP-KIT' and status in ('processing', 'waiting', 'pending', 'checkout_complete', 'completed') AND uid IN (:uids)", array(':uids' => $uids))->fetchCol();
              $referrals_count = count($user_ids);
            }
            //$user_ids = db_query("SELECT distinct userid FROM qu_pap_transactions WHERE productid LIKE '%PROMOTIONAL-CO-OP-KIT%' AND userid in (:children_ids)", array(':children_ids' => $children_ids))->fetchCol();

        }
        $results[$key]->field_field_pap_affiliate_id_2[0]['rendered']['#markup'] = $referrals_count;
        db_set_active('pap');
        if ($start_date && $end_date) {
          $transactions['c602cb08'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND dateinserted >= :start AND dateinserted <= :end AND campaignid = :campaignid", array(':userid' => $pap_id, 'start' => $start_date, 'end' =>  $end_date, 'campaignid' => 'c602cb08'))->fetchCol();
          $transactions['11111111'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND dateinserted >= :start AND dateinserted <= :end AND campaignid = :campaignid", array(':userid' => $pap_id, 'start' => $start_date, 'end' =>  $end_date, 'campaignid' => '11111111'))->fetchCol();
          $transactions['c5d380ba'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND dateinserted >= :start AND dateinserted <= :end AND campaignid = :campaignid", array(':userid' => $pap_id, 'start' => $start_date, 'end' =>  $end_date, 'campaignid' => 'c5d380ba'))->fetchCol();
          $transactions['14'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND dateinserted >= :start AND dateinserted <= :end AND campaignid = :campaignid AND tier = 1", array(':userid' => $pap_id, 'start' => $start_date, 'end' =>  $end_date, 'campaignid' => '11111111'))->fetchCol();
        }
        else {
          //db_set_active('pap');
          $transactions['c602cb08'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND campaignid = :campaignid", array(':userid' => $pap_id, 'campaignid' => 'c602cb08'))->fetchCol();
          $transactions['11111111'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND campaignid = :campaignid", array(':userid' => $pap_id, 'campaignid' => '11111111'))->fetchCol();
          $transactions['c5d380ba'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND campaignid = :campaignid", array(':userid' => $pap_id, 'campaignid' => 'c5d380ba'))->fetchCol();
          $transactions['14'] = db_query("SELECT sum(totalcost) FROM qu_pap_transactions WHERE userid = :userid AND campaignid = :campaignid AND tier = 1", array(':userid' => $pap_id, 'campaignid' => '11111111'))->fetchCol();
         // db_set_active();
        }
      }

     if (!$referrals_count && !$transactions['c602cb08'][0]) {
       $to_be_removed[] = $key;
     }
     $results[$key]->field_field_pap_affiliate_id_3[0]['rendered']['#markup'] = commerce_currency_format($transactions['c602cb08'][0] *100, 'USD');
     $results[$key]->field_field_pap_affiliate_id_12[0]['rendered']['#markup'] = commerce_currency_format($transactions['11111111'][0] * 100, 'USD');
     $results[$key]->field_field_pap_affiliate_id_13[0]['rendered']['#markup'] = commerce_currency_format($transactions['c5d380ba'][0] * 100, 'USD');
     $results[$key]->field_field_pap_affiliate_id_14[0]['rendered']['#markup'] = commerce_currency_format($transactions['14'][0] * 100, 'USD');
     $parent_pap_id = db_query('SELECT parentuserid FROM qu_pap_users WHERE userid = :userid', array(':userid' => $pap_id))->fetchField();

     db_set_active();
     $results[$key]->field_field_pap_affiliate_id_4[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_5[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_6[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_7[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_8[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_9[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_10[0]['rendered']['#markup'] = '';
     $results[$key]->field_field_pap_affiliate_id_11[0]['rendered']['#markup'] = '';
     if ($parent_pap_id) {
       $uid = db_query('SELECT entity_id FROM field_data_field_pap_affiliate_id WHERE field_pap_affiliate_id_value = :parent_pap_id' , array(':parent_pap_id' => $parent_pap_id))->fetchField();
       if ($uid) {
         $parent_account = user_load($uid);
         if ($parent_account) {
           $parent_wrapper = entity_metadata_wrapper('user', $parent_account);
           $tpl_date = $parent_wrapper->field_tpl_date->value();
           $results[$key]->field_field_pap_affiliate_id_4[0]['rendered']['#markup'] = $uid;
           $results[$key]->field_field_pap_affiliate_id_5[0]['rendered']['#markup'] = $parent_pap_id;
           $results[$key]->field_field_pap_affiliate_id_6[0]['rendered']['#markup'] = $parent_account->mail;
           $results[$key]->field_field_pap_affiliate_id_7[0]['rendered']['#markup'] = $parent_wrapper->field_first_name->value() . ' ' . $parent_wrapper->field_last_name->value();
           $results[$key]->field_field_pap_affiliate_id_8[0]['rendered']['#markup'] = in_array(49, array_keys($parent_account->roles)) ? 'Yes' : 'No';
           $results[$key]->field_field_pap_affiliate_id_9[0]['rendered']['#markup'] = $tpl_date ? date('Y-m', $parent_wrapper->field_tpl_date->value()) : 'N/A';
           $results[$key]->field_field_pap_affiliate_id_10[0]['rendered']['#markup'] = $start;
           $results[$key]->field_field_pap_affiliate_id_11[0]['rendered']['#markup'] = $end;
         }
       }
     }
    }

    if (!empty($to_be_removed)) {
      foreach($to_be_removed as $key) {
        if (count($results) > 1) {
          unset($results[$key]);
        }
        else {
          $results[$key]->field_field_pap_affiliate_id[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_1[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_2[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_3[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_4[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_5[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_6[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_7[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_8[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_9[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_10[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_pap_affiliate_id_11[0]['rendered']['#markup'] = '';
          $results[$key]->field_field_tpl_date[0]['rendered']['#markup'] = '';
          $results[$key]->uid = NULL;
          $results[$key]->users_mail = NULL;
        }
      }
    }

  }
  if (isset($view->name) && $view->name == 'rtpr_cc_buck_report') {
    $results = &$view->result;
    foreach ($results as $key => $result) {
      $remove = FALSE;
      $uid = $result->uid;
      $payment_date = NULL;
      $order_ids = db_query("SELECT order_id FROM commerce_order WHERE uid = :uid AND status in ('pending', 'completed', 'processing', 'waiting')", array(':uid' => $uid))->fetchCol();
      if ($order_ids) {
        $query = db_select('commerce_line_item', 'item')
          ->fields('item', array('order_id', 'line_item_label'))
          ->condition('order_id', $order_ids, 'IN')
          ->condition('line_item_label', _rtpr_commerce_buck_sku_list(), 'IN');

        $bo_order_ids = $query->execute()->fetchAll();
        if ($bo_order_ids) {
          foreach($bo_order_ids as $boi) {
            if (strpos($boi->line_item_label, 'PAIN-RELIEF-OFFER') !== FALSE) {
              $payment_date = db_query('SELECT created FROM commerce_payment_transaction WHERE order_id = :order_id AND status = :status', array(':order_id' => $boi->order_id, ':status' => 'success'))->fetchField();
              break;
            }
          }
        }
      }

      $ref_pap_id = $result->field_field_pap_referral_id[0]['raw']['value'];
      $name = '';
      $mail = '';
      $ref_uid = db_query('SELECT entity_id FROM field_data_field_pap_affiliate_id WHERE field_pap_affiliate_id_value = :ref_pap_id', array(':ref_pap_id' => $ref_pap_id))->fetchField();
      if ($ref_uid) {
        $ref_account = user_load($ref_uid);
        if ($ref_account) {
          $ref_wrapper = entity_metadata_wrapper('user', $ref_account);
          $name = $ref_wrapper->field_first_name->value() . ' ' . $ref_wrapper->field_last_name->value();
          $mail = $ref_account->mail;
        }
      }
      $data = _rtpr_cc_user_buck_data($result->_field_data['uid']['entity'], $view->exposed_raw_input['line_item_label']);

      if ($view->exposed_raw_input['line_item_label'] != 'All') {
        if (empty($data['purchased'])) {
          unset($results[$key]);
          continue;
        }
      }
      $results[$key]->field_field_pap_affiliate_id[0]['rendered']['#markup'] = _rtpr_user_get_available_redemption_credits($uid);
      $results[$key]->field_field_pap_affiliate_id_1[0]['rendered']['#markup'] = $data['purchased'];
      $results[$key]->field_field_pap_affiliate_id_2[0]['rendered']['#markup'] = $data['additional'] ? 'YES' : 'NO';
      $results[$key]->field_field_pap_affiliate_id_3[0]['rendered']['#markup'] = commerce_currency_format($data['sum'], 'USD');
      $results[$key]->field_field_pap_affiliate_id_10[0]['rendered']['#markup'] = commerce_currency_format($data['pre_buck_sum'], 'USD');
      $results[$key]->field_field_pap_affiliate_id_11[0]['rendered']['#markup'] = commerce_currency_format($data['post_buck_sum'], 'USD');
      $results[$key]->field_field_pap_affiliate_id_4[0]['rendered']['#markup'] = $data['purchased'] ? 'YES' : 'NO';
      $results[$key]->field_field_pap_affiliate_id_5[0]['rendered']['#markup'] = $name;
      $results[$key]->field_field_pap_affiliate_id_6[0]['rendered']['#markup'] = $mail;
      $results[$key]->field_field_pap_affiliate_id_7[0]['rendered']['#markup'] = $payment_date ? date('m/d/Y', $payment_date) : 'N/A';
      $results[$key]->field_field_pap_affiliate_id_8[0]['rendered']['#markup'] = rtpr_user_get_primary_role($result->_field_data['uid']['entity']->roles);
      $results[$key]->field_field_pap_affiliate_id_9[0]['rendered']['#markup'] = in_array($data['purchased'], _rtpr_commerce_buck_primary_sku_list()) ? 'YES' : 'NO';
    }
  }
}

/**
 * Implements hook_views_query_alter().
 */
function rtpr_cc_views_query_alter(&$view, &$query) {
  if (isset($view->name) && $view->name == 'rtpr_cc_buck_report') {
    $query->table_queue['commerce_payment_transaction_commerce_order']['join']->left_query = 'SELECT commerce_payment_transactionINNER.transaction_id AS transaction_idINNER
FROM
{commerce_payment_transaction} commerce_payment_transactionINNER
LEFT JOIN {commerce_order} commerce_orderINNER ON commerce_payment_transactionINNER.order_id = commerce_orderINNER.order_id
LIMIT 1 OFFSET 0';
    $sku = $view->exposed_raw_input['line_item_label'];

    if ($sku == 'All') {
      unset($query->where[1]['conditions'][2]);
    }
    if ($sku == 1) {
      unset($query->where[1]['conditions'][1]);
    }
  }
}

/**
 * Implements hook_views_data().
 */
function rtpr_cc_views_data() {
/*
  $data = array();
  // Data field
  $data['users']['data'] = array(
    'title' => t('User Data'), // The item it appears as on the UI,
    'help' => t("The user's data."), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
*/
}

/**
 * Implements hook_views_data_alter().
 */
function rtpr_cc_views_data_alter(&$data) {
  $data['users']['data'] = array(
    'title' => t('User Data'), // The item it appears as on the UI,
    'help' => t("The user's data."), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
}
