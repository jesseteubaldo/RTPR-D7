<?php
  
/**
 * Cron related features.
 */
define('RTPR_AFFILIATE_RID',4);
define('RTPR_GOLD_RID',9);
define('RTPR_PLATINUM_RID',22);
define('RTPR_VENDOR_RID',8);
define('RTPR_EXTRAS_LIGHT_CRON_GAP', 86400); // one day

function rtpr_extras_cron_report_form($form, $form_state) {
  $form['full'] = array(
    '#type' => 'fieldset',
    '#title' => t('Full cron reports'), 
    '#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );

  $form['full']['last'] = array(
    '#markup' => '<p>Last run: ' . format_interval(time() - variable_get('_rtpr_extras_cron_last_ran')) . ' ' . t('ago') . '</p>',
  );
  
  $form['full']['left'] = array(
    '#markup' => '<p>User left: ' . count(variable_get('_rtpr_extras_user_list')) .'</p>',
  );
/*
  
  $form['full']['run'] = array(
    '#type' => 'submit',
    '#value' => t('Run cron'),
    '#submit' => array('_rtpr_extras_run_cron'),
  );
  
  
  $form['full']['alert'] = array(
    '#markup' => '<p><i>'. t('Warning: This cron will take lots resource, don\'t run it unless you have to!') . '</i></p>',
  );
  
  
*/
  
  $form['light'] = array(
    '#type' => 'fieldset',
    '#title' => t('Light cron reports'), 
    '#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );

  $form['light']['last'] = array(
    '#markup' => '<p>Last run: ' . format_interval(time() - variable_get('_rtpr_extras_light_cron_last_ran')) . ' ' . t('ago') . '</p>',
  );
  
   $query = db_select('commerce_order','o')
  ->fields('o', array('uid', 'order_id', 'created'))
  ->condition('created', time() - RTPR_EXTRAS_LIGHT_CRON_GAP, '>')
  ->condition('status', array('pending', 'processing', 'completed'), 'IN')
  ->orderBy('created', 'DESC');
  
  $results  = $query->execute()->fetchAllAssoc('order_id');
  
  
  $form['light']['left'] = array(
    '#markup' => '<p>Orders in last 24 hours: ' . count($results) .'</p>',
  );
  
  $form['light']['run'] = array(
    '#type' => 'submit',
    '#value' => t('Run cron'),
    '#submit' => array('_rtpr_extras_run_light_cron'),
  );
/*
  
  $form['platinum'] = array(
    '#type' => 'fieldset',
    '#title' => t('Platinum cron reports'), 
    '#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  
  $form['platinum']['left'] = array(
    '#markup' => '<p>User left: ' . count(variable_get('_rtpr_extras_platinum_user_list')) .'</p>',
  );
*/

  return $form;
}


function _rtpr_extras_pap_upgrade_downgrade_transactions($time_gap = '-1 day') {
  db_set_active('pap');
  $date_inserted = date('Y-m-d H:i:s', strtotime($time_gap));

  $query = db_select('qu_pap_transactions', 't')
    ->fields('t')
    ->condition('tier', 1)
    ->condition('campaignid', array('c5d380ba', '11111111'))
    ->condition('dateinserted',$date_inserted, '>');
  $results = $query->execute()->fetchAllAssoc('userid');
  $pap_ids = array_keys($results);
  db_set_active();

  $uids = db_select('field_data_field_pap_affiliate_id', 'p')
    ->fields('p', array('entity_id'))
    ->condition('field_pap_affiliate_id_value', $pap_ids, 'IN')
    ->execute()->fetchCol();

  $exclude_uids = db_select('users_roles', 'r')
    ->fields('r', array('uid'))
    ->condition('rid', 30, '=')
    ->condition('uid', $uids, 'IN')
    ->execute()->fetchCol();

  
 $final_uids = db_select('users_roles', 'r')
    ->fields('r', array('uid'))
    ->condition('rid', array(4, 8, 9, 22), 'IN')
    ->condition('uid', array_diff($uids, $exclude_uids), 'IN')
    ->execute()->fetchCol();

  return $final_uids;
}

/**
 * Full cron to update all users on the site.
 */
function _rtpr_extras_run_cron() {
  $rids = array(RTPR_GOLD_RID, RTPR_PLATINUM_RID, RTPR_VENDOR_RID);
  if (isset($_GET['type']) && $_GET['type'] == 'affiliate') {
    watchdog('rtpr_cron', 'Running affiliate.');
    $rids = array(RTPR_AFFILIATE_RID);
  }
  $last_ran = variable_get('_rtpr_extras_cron_last_ran');
  if (time() - $last_ran < 2400) {
    return;
  }

  watchdog('rtpr_cron', 'PAP commission group cron run activated', array(), WATCHDOG_NOTICE);
  variable_set('_rtpr_extras_cron_last_ran', time());
  $valid_uids = variable_get('_rtpr_extras_user_list');

  if(!$valid_uids && empty($valid_uids)) {
    $query = db_select('users_roles','r');
    $query->join('users', 'u', 'u.uid = r.uid');
    $query->fields('u', array('uid'))
      ->condition('rid', $rids, 'IN')
      ->orderBy('access', 'DESC');
    $uids = $query->execute()->fetchCol();
    $exclude_uids = db_select('users_roles', 'r')
      ->fields('r', array('uid'))
      ->condition('rid', 30, '=')
      ->condition('uid', $uids, 'IN')
      ->execute()->fetchCol();
    $valid_uids = array_diff($uids, $exclude_uids);
    variable_set('_rtpr_extras_user_list', $valid_uids);
  } 

  foreach($valid_uids as $index => $uid) {
    unset($valid_uids[$index]);
    $account = user_load($uid);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $account_wrapper->field_pap_affiliate_id->value();
    if($pap_id) {
      watchdog('rtpr_cron', 'Processing user @email', array('@email' => $account->mail), WATCHDOG_NOTICE);
      rules_invoke_component('rules_set_commission_group_apv', $account);
    }
    // Update list
    variable_set('_rtpr_extras_user_list', $valid_uids);
  }
 
  $deleted = db_delete('rules_scheduler')->execute();
}

/**
 * Only update commission group for user who order in last RTPR_EXTRAS_LIGHT_CRON_GAP.
 */
function _rtpr_extras_run_light_cron() {

  watchdog('PAP Upgrade', 'PAP lightweight commission group cron run activated', array(), WATCHDOG_NOTICE);
  variable_set('_rtpr_extras_light_cron_last_ran', time());
  $uids = _rtpr_extras_pap_upgrade_downgrade_transactions('-24 days');
  $accounts = user_load_multiple($uids);

  variable_del('_rtpr_extras_run_light');
  $ran_users = array();

  foreach($accounts as $account) {
    $roles = array_keys($account->roles);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $account_wrapper->field_pap_affiliate_id->value();
    if(!in_array($pap_id, $ran_users)) {

      rules_invoke_component('rules_set_commission_group_apv', $account);
      $ran_users[] = $pap_id;
      variable_set('_rtpr_extras_run_light', $ran_users);
    }
  }
  return;

  $query = db_select('commerce_order','o')
  ->fields('o', array('uid', 'order_id', 'created'))
  ->condition('created', time() - RTPR_EXTRAS_LIGHT_CRON_GAP, '>')
  ->condition('status', array('pending', 'processing', 'completed'), 'IN')
  ->orderBy('created', 'DESC');
  
  $results  = $query->execute();
  
  
  foreach($results as $result) {
    $account = user_load($result->uid);
    
    $roles = $account->roles;
    if(array_key_exists(RTPR_AFFILIATE_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_affiliate_role_commission_groups', $account);
    } else if (array_key_exists(RTPR_GOLD_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_gold_role_commission_groups', $account);
    } else if (array_key_exists(RTPR_PLATINUM_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_platinum_role_commission_groups', $account);
    } else if(array_key_exists(RTPR_VENDOR_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_vendor_role_commission_groups', $account);  
    }
  }
  $deleted = db_delete('rules_scheduler')->execute();

  // Remove Sampler role
  _rtpr_extras_sampler_cron();
}

/**
 * Clean up.
 */
function _rtpr_extras_run_platinum_cron() {

  variable_set('_rtpr_extras_user_list', array());
  return;
  $users = variable_get('_rtpr_extras_platinum_user_list');
  
  if(!$users && empty($users)) {
    $query = db_select('users','u');
    $query->leftJoin('users_roles', 'r', 'u.uid = r.uid');
  
    $query->fields('u', array('uid', 'name'))
    ->condition('r.rid', 22, '=');
    
    $results = $query->execute()->fetchAllAssoc('uid');
    variable_set('_rtpr_extras_platinum_user_list', $results);
    $users = variable_get('_rtpr_extras_platinum_user_list');
  } 
  
  $results = $users;
  foreach($results as $uid => $result) {
    
    $account = user_load($uid);
    
    unset($users[$uid]);
    $roles = $account->roles;
    if(array_key_exists(RTPR_AFFILIATE_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_affiliate_role_commission_groups', $account);
    } else if (array_key_exists(RTPR_GOLD_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_gold_role_commission_groups', $account);
    } else if (array_key_exists(RTPR_PLATINUM_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_platinum_role_commission_groups', $account);
    } else if(array_key_exists(RTPR_VENDOR_RID, $roles) && !array_key_exists(RTPR_OVERRIDE_RID, $roles)) {
      rules_invoke_component('rules_set_vendor_role_commission_groups', $account);  
    }
    watchdog('rtpr_cron', 'Processed platinum user @email', array('@email' => $account->mail), WATCHDOG_NOTICE);
    // Update list
    variable_set('_rtpr_extras_platinum_user_list', $users);
  }
}
/**
 * RTPR Light weight cron
 *
 * Housekeeping cron
 */
function _rtpr_extras_light_cron() {
  // Deleted scheduled rules for updating commission group.
  $deleted = db_delete('rules_scheduler')->execute();
  
  // Delete cache filter table.
  $deleted = db_delete('cache_filter')->execute();
  watchdog("light_cron", '<pre>' . print_r( $deleted, true) . '</pre>');
  
  // Delete cache field table.
  $deleted = db_delete('cache_field')->execute();
  
  scheduler_cron();
}

/**
 * RTPR Very Light weight cron - 30 minutes or less
 *
 * Housekeeping cron
 */
function _rtpr_extras_very_light_cron() {
  watchdog('very light cron', 'Very lightweight cron run activated', array(), WATCHDOG_NOTICE);
  // Fetch email
  support_fetch_client_mail();
}

/**
 * Cron to remove Sampler role
 */
function _rtpr_extras_sampler_cron() {
  $query = db_select('users_roles', 'r')
    ->fields('r')
    ->condition('rid', RTPR_SAMPLER_RID, '=');
  $product_skus = array(RTPR_BRONZE_PACKAGE_SKU, RTPR_SILVER_PACKAGE_SKU);
  $users = $query->execute()->fetchAllAssoc('uid');
  $found = FALSE;
  foreach ($users as $uid => $data) {
    $orders = commerce_order_load_multiple(array(), array('uid' => $uid));
    foreach ($orders as $order) {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      // Look for product line items on the order whose products match the
      // specified product types and increment the quantity count accordingly.
      foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
        if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
          // Extract the product type from the line item.
          $line_item_sku = $line_item_wrapper->commerce_product->sku->value();
          if (in_array($line_item_sku, $product_skus) && time() < strtotime("+1 year", $order->created)) {
            $found = TRUE;
          }
        }
        if ($found) {
          break;
        }
      }
      if ($found) {
        break;
      }
    }
    if (!$found) {
      db_delete('users_roles')->condition('rid', 37)->condition('uid', $uid)->execute();
    }
  }
}

