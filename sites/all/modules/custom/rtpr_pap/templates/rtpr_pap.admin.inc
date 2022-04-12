<?php

/**
 * @file
 * Administrator features for PAP.
 */

/**
 * PAP user report.
 */
function rtpr_pap_user_report_form($form, $form_state) {
  rtpr_pap_impor_users();
  
  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );

  $form['options']['year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => rtpr_reward_year_list(FALSE, 2011),
    '#default_value' => date('Y'),
    '#required' => TRUE,
  );

  $form['options']['month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => array(
      '01' => t('January'),
      '02' => t('February'),
      '03' => t('March'),
      '04' => t('April'),
      '05' => t('May'),
      '06' => t('June'),
      '07' => t('July'),
      '08' => t('August'),
      '09' => t('September'),
      '10' => t('October'),
      '11' => t('November'),
      '12' => t('December'),
    ),
    '#default_value' => date('m'),
    '#required' => TRUE,
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
    //'#disabled' => variable_get('rtpr_pap_user_report_disabled', FALSE),
  );

  if (variable_get('rtpr_pap_user_report_disabled')) {
    $batch_id = variable_get('rtpr_pap_user_batch_id');
    $progress = progress_get_progress('_background_batch:' . $batch_id);
    $percentage = $progress->progress * 100;
    drupal_set_message(t('There\'s already a report started by @mail.', array('@mail' => variable_get('rtpr_pap_user_report_mail'))), 'warning');
    if ($percentage > 98) {
      drupal_set_message(t('You can click <a href="@batch_link">here</a> to download the report.', array('@batch_link'=> '/batch?op=start&id=' . $batch_id)));
    } else {
      drupal_set_message(t('It\'s currently at @percentage%.', array('@percentage' => $percentage)));
    }
  }

  return $form;
}

/**
 *
 */
function rtpr_pap_process_reports($month, $year) {
  $users = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('access', strtotime('-6 months'), '>=')
        ->orderBy('access', 'ASC')
        ->execute()->fetchCol();
  $operations = array();
  foreach ($users as $uid) {
    $data = array(
      'month' => $month,
      'year' => $year,
      'uid' => $uid,
    );
    $operations[] = array(
      'rtpr_pap_process_report_op',
          array(
            $data,
          ),
    );
  }

  // Build the batch instructions.
  $batch = array(
    'operations' => $operations,
    'finished' => 'rtpr_pap_process_reports_finished',
    'file' => drupal_get_path('module', 'rtpr_pap') . '/templates/rtpr_pap.admin.inc',
  );

  return $batch;
}

/**
 *
 */
function rtpr_pap_process_reports_finished($success, $results, $operations) {
  variable_set('rtpr_pap_user_report_disabled', FALSE);
  $header = array(
    'PAP USER ID',
    'USERNAME',
    'First Name',
    'Last Name',
    'Primary Role',
    'FLT Role',
    'Account Manager',
    'Override Role',
    'Total MSP',
    'Total SAV',
    'Total RAV',
    'Total NAV',
    'Total APV',
    'APV Kit',
    'TOTAL Retail Customers',
    'TOTAL Purchasing Customers',
    'Month Customer Purchases',
    'Month Sampler Purchases',
    'Total Sample Program Frontline Volume',
    'Total Sampler Commissions',
    'Month',
    'State',
    '500 Referrals',
    'PAP Parent First Name',
    'PAP Parent Last Name',
    'PAP Parent Username Email',
    'Parent Primary Role',
    'Parent PAP User ID',
  );

  if ($success) {
    // Here we could do something meaningful with the results.
    // We just display the number of nodes we processed...
    drupal_set_message(t('@count user processed.', array('@count' => count($results))));

    // drupal_add_http_header('Content-Type', 'text/csv');
    //        drupal_add_http_header('Content-Disposition', 'attachment;filename=report.csv');
    //        $fp = fopen('php://output', 'w');
    //        // Header
    //        fputcsv($fp, $header);
    //
    //        foreach($results as $row){
    //        fputcsv($fp, $row);
    //        }
    //
    //        fclose($fp);
    //        drupal_exit();
    $file_path = 'public://private/pap/' . time() . '.csv';
    $fp = fopen($file_path, 'w');
    // Header.
    fputcsv($fp, $header);

    foreach ($results as $row) {
      fputcsv($fp, $row);
    }

    fclose($fp);
    $loglink = l("here", file_create_url($file_path));
    $msg = "Click $loglink to download the report.";
    drupal_set_message($msg);

  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }
}

define('RTPR_VENDOR_FLT_RID', 24);
define('RTPR_ACCOUNT_MANAGER_RID', 20);

/**
 *
 */
function rtpr_pap_user_report_form_submit($form, $form_state) {
  global $user;
  $values = $form_state['values'];
  $month = $values['month'];
  $year = $values['year'];
  // batch_set(batch_example_batch_1($month, $year));.
  batch_set(rtpr_pap_process_reports($month, $year));
  background_batch_process_batch();
  variable_set('rtpr_pap_user_report_disabled', TRUE);
  variable_set('rtpr_pap_user_report_mail', $user->mail);
  $batch = batch_get();
  variable_set('rtpr_pap_user_batch_id', $batch['id']);
}

/**
 *
 */
function rtpr_pap_calculate_volume_total($type, $pap_id, $month, $year) {
  db_set_active('pap');
  $bonus = rtpr_pap_volume_calculation_formula($type);
  $tiers = $bonus['tiers'];
  $campaigns = $bonus['campaigns'];
  $start_date = $year . '-' . $month . '-01';
  $end_date = $year . '-' . $month . '-31 23:59:59';

  $results = db_select('qu_pap_transactions', 't')
        ->fields('t')
        ->condition('userid', $pap_id, '=')
        ->condition('tier', $tiers, 'IN')
        ->condition('rtype','R', '!=')
        ->condition('campaignid', $campaigns, 'IN')
        ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
        ->execute();

  $correct_total = 0;
  foreach ($results as $row) {
    if ($row->totalcost) {
      $correct_total += $row->totalcost;
    }
  }

  db_set_active();
  return $correct_total;

}

/**
 *
 */
function rtpr_pap_get_pap_account_info($pap_id) {
  db_set_active('pap');

  $query = db_select('qu_pap_users', 'pu');
  $query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
  $query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
  $query->fields('pu', array('parentuserid'))
        ->condition('pu.userid', $pap_id, '=');

  $user_result = $query->execute()->fetchAssoc();

  $sub_query = db_select('qu_pap_users', 'pu');
  $sub_query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
  $sub_query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
  $sub_query->fields('ga', array('username', 'firstname', 'lastname'))
        ->fields('pu', array('userid'))
        ->condition('pu.userid', $user_result['parentuserid'], '=');
  $result = $sub_query->execute()->fetchAssoc();

  db_set_active();
  return $result;
}

/**
 *
 */
function rtpr_pap_get_drupal_account_info($username) {
  $query = db_select('users', 'u')
        ->fields('u')
        ->condition('mail', $username, '=')
        ->execute()->fetchAssoc();
  $result = array();
  $result['p_role'] = '';
  if (isset($query['uid'])) {
    $uid = $query['uid'];
    $account = user_load($uid);

    $roles = $account->roles;
    $wrapper = entity_metadata_wrapper('user', $account);
    $result['p_first_name'] = $wrapper->field_first_name->value();
    $result['p_last_name'] = $wrapper->field_last_name->value();
    $result['p_username'] = $username;
    $result['p_role'] = rtpr_user_get_primary_role($roles);
  }

  return $result;
}

/**
 * @param $pap_id
 *
 *
 * @return total count of users this user is the parent of that has 499
 * or higher MSP for the selected month and year
 */
function rtpr_pap_get_500_referrals($pap_id, $month, $year) {
  $count = 0;
  db_set_active('pap');

  $query = db_select('qu_pap_users', 'pu')
        ->fields('pu', array('userid'))
        ->condition('pu.parentuserid', $pap_id, '=');

  $results = $query->execute()->fetchAllAssoc('userid');
  $user_ids = array_keys($results);
  if(!empty($user_ids)) {
    $bonus = rtpr_pap_volume_calculation_formula('msp');
    $tiers = $bonus['tiers'];
    $campaigns = $bonus['campaigns'];

    $start_date = $year . '-' . $month . '-01 00:00:00';
    $end_date = $year . '-' . $month . '-31 23:59:59';

    $query = db_select('qu_pap_transactions','t')
    ->fields('t')
    ->condition('userid', $user_ids , 'IN')
    ->condition('tier', $tiers, 'IN')
    ->condition('rtype','R', '!=')
    ->condition('campaignid', $campaigns , 'IN')
    ->condition('dateinserted',array($start_date, $end_date) , 'BETWEEN');
    $query->groupBy('userid');//GROUP BY user ID
    $alias = $query->addExpression('SUM(totalcost)', 'totalcost');
    $results = $query->execute()->fetchAll();
    db_set_active();
    foreach($results as $transaction) {
      if ($transaction->totalcost >= 499) {
        $papuserid = $transaction->userid;
        $query = db_select('field_data_field_pap_affiliate_id', 'p');
        $query->addField('p', 'entity_id');
        $query->condition('p.field_pap_affiliate_id_value', $papuserid);
        $userid = $query->execute()->fetchField();
        if ($userid){
          $account = user_load($userid);
          $account_roles = $account->roles;
          $exclude_rids = array(10, 11);
          if (array_intersect($exclude_rids, array_keys($account_roles))) {
            continue;
          }
        }
       $count++;
      }
    }
  }
  return $count;
}

/**
 *
 */
function rtpr_pap_process_report_op($data, &$context) {
  $month = $data['month'];
  $year = $data['year'];
  $uid = $data['uid'];

  $context['message'] = t('Processing user with id "@id"', array('@id' => $uid));
  $account = user_load($uid);
  $wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $wrapper->field_pap_affiliate_id->value();

  $address = $wrapper->field_address->value();

  if ($pap_id) {
    $roles = $account->roles;
    $row['PAP USER ID'] = $pap_id;
    $row['USERNAME'] = $account->mail;
    $row['First Name'] = $wrapper->field_first_name->value();
    $row['Last Name'] = $wrapper->field_last_name->value();
    $row['Primary Role'] = rtpr_user_get_primary_role($roles);
    $row['FLT Role'] = 'NO';
    $row['Account Manager'] = 'NO';
    $row['Override Role'] = 'NO';
    if (array_key_exists(RTPR_VENDOR_FLT_RID, $roles)) {
      $row['FLT Role'] = 'YES';
    }
    if (array_key_exists(RTPR_ACCOUNT_MANAGER_RID, $roles)) {
      $row['Account Manager'] = 'YES';
    }
    if (array_key_exists(RTPR_ACCOUNT_MANAGER_RID, $roles)) {
      $row['Account Manager'] = 'YES';
    }
    $start_date = $year . '-' . $month . '-01';
    $end_date = $year . '-' . $month . '-31';
    $min_timestamp = strtotime($year. '-' . $month . '-01');
    $max_timestamp = strtotime('next month', $min_timestamp) + 3599;

    // Referral.
    $retail_data = rtpr_pap_retail_customer_referral_data($pap_id, $start_date, $end_date);
    $referral_data = rtpr_pap_get_referral_data($pap_id, $min_timestamp, $max_timestamp);

    $row['Total MSP'] = '$' . number_format(rtpr_pap_calculate_volume_total('msp', $pap_id, $month, $year), 2);
    $row['Total SAV'] = '$' . number_format(rtpr_pap_calculate_volume_total('sav', $pap_id, $month, $year), 2);
    $row['Total RAV'] = '$' . number_format(rtpr_pap_calculate_volume_total('rav', $pap_id, $month, $year), 2);
    $row['Total NAV'] = '$' . number_format(rtpr_pap_calculate_volume_total('nav', $pap_id, $month, $year), 2);
    $total_apv = (rtpr_pap_calculate_volume_total('msp', $pap_id, $month, $year) * 100) + ($retail_data['total']/2);
    $row['Total APV'] =  commerce_currency_format($total_apv, 'USD');
    $row['APV Kit'] = rtpr_pap_brought_product($uid, $min_timestamp, $max_timestamp, array('VENDOR-REFILL-KIT', 'VENDOR-SAMPLER-KIT'));

    $row['TOTAL Retail Customers'] = $retail_data['count'];
    $row['TOTAL Purchasing Customers'] = $retail_data['purchase'];
    $row['Month Customer Purchases'] = $retail_data['tier1'];
    $row['Month Sampler Purchases'] = $retail_data['tier2'];
    $row['Total Sample Program Frontline Volume'] = commerce_currency_format($retail_data['total'], 'USD');
    $row['Total Sampler Commissions'] = $retail_data['total_commission'];

    $row['Month'] = $month . '-' . $year;
    $row['State'] = $address['administrative_area'];
    $row['500 Referrals'] = rtpr_pap_get_500_referrals($pap_id, $month, $year);

    $p_pap_info = rtpr_pap_get_pap_account_info($pap_id);

    $row['PAP Parent First Name'] = $p_pap_info['username'];
    $row['PAP Parent Last Name'] = $p_pap_info['firstname'];
    $row['PAP Parent Username Email'] = $p_pap_info['lastname'];

    $p_drupal_info = rtpr_pap_get_drupal_account_info($p_pap_info['username']);

    $row['Parent Primary Role'] = $p_drupal_info['p_role'];
    $row['Parent PAP User ID'] = $p_pap_info['userid'];

    $context['results'][$uid] = $row;
  }
}

/**
 *
 */
function rtpr_pap_retail_customer_referral_data($pap_id, $start_date, $end_date) {
  ini_set('memory_limit', '1436m');
  $start_date = $start_date . ' 00:00:00';
  $end_date = $end_date . ' 23:59:59';

  $query = db_select('field_data_field_pap_referral_id', 'r');
  $query->innerJoin('users_roles', 'u', 'r.entity_id = u.uid');
  $query->fields('r')->fields('u', array('rid'))
    ->condition('r.field_pap_referral_id_value', $pap_id, '=')
    ->condition('u.rid', 7, '=');
  $results = $query->execute()->fetchAllAssoc('entity_id');

  $total_amount = 0;
  $purchase = array();
  foreach ($results as $entity_id => $result) {
    $query = db_select('commerce_order', 'o');
    $query->innerJoin('field_data_commerce_order_total', 't', 'o.order_id = t.entity_id');
    $query->fields('o', array('order_id'))
    ->condition('o.status', rtpr_offer_order_success_statuses(), 'IN')
    ->condition('o.uid', $entity_id, '=');
    // $alias = $query->addExpression('SUM(commerce_order_total_amount)', 'total_amount');.
    $orders = $query->execute()->fetchAllAssoc('order_id');
    foreach ($orders as $order_id => $data) {
      $order = commerce_order_load($order_id);
      $purchase[$order->uid] = $order->uid;
      $wrapper = entity_metadata_wrapper('commerce_order', $order);
      $line_items = $wrapper->commerce_line_items;
      $total = commerce_line_items_total($line_items, array('product'));
      $total_amount += $total['amount'];
    }
  }

  db_set_active('pap');
  $query = db_select('qu_pap_transactions', 't')
    ->fields('t')
    ->condition('userid', $pap_id)
    ->condition('campaignid', '11111111')
    ->condition('payoutstatus', 'U')
    ->condition('rtype','R', '!=')
    ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
    ->condition('tier', 1, '=');
  $query->addExpression('SUM(totalcost)', 'monthly_unpaid');;
  $transactions = $query->execute()->fetchAll();
  $tier_1_total = 0;

  if (isset($transactions[0])) {
    $tier_1_total = $transactions[0]->monthly_unpaid;
    $tier_1_total *= 100;
  }

  $query = db_select('qu_pap_transactions', 't')
    ->fields('t')
    ->condition('userid', $pap_id)
    ->condition('campaignid', 'c602cb08')
    ->condition('payoutstatus', 'U')
    ->condition('rtype','R', '!=')
    ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
    ->condition('tier', 2, '=');
  $query->addExpression('SUM(totalcost)', 'monthly_unpaid');;
  $transactions = $query->execute()->fetchAll();
  $tier_2_total = 0;

  if (isset($transactions[0])) {
    $tier_2_total = $transactions[0]->monthly_unpaid;
    $tier_2_total *= 100;
  }

  $query = db_select('qu_pap_transactions', 't')
    ->fields('t')
    ->condition('userid', $pap_id)
    ->condition('campaignid', array('11111111', 'c602cb08'), 'IN')
    ->condition('payoutstatus', 'U')
    ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN');
  $query->addExpression('SUM(commission)', 'monthly_commission');;
  $transactions = $query->execute()->fetchAll();
  $total_commission = 0;

  if (isset($transactions[0])) {
    $total_commission = $transactions[0]->monthly_commission;
    $total_commission *= 100;
  }
  
  $query = db_select('qu_pap_transactions', 't')
    ->fields('t')
    ->condition('userid', $pap_id)
    ->condition('campaignid', array('c5d380ba'), 'IN')
    ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN');
  $query->addExpression('SUM(commission)', 'monthly_commission');;
  $transactions = $query->execute()->fetchAll();
  $wholesale_commissions = 0;

  if (isset($transactions[0])) {
    $wholesale_commissions = $transactions[0]->monthly_commission;
    $wholesale_commissions *= 100;
  }

  db_set_active();

  return array(
    'count' => count($results),
    'purchase' => count($purchase),
    'tier1' => commerce_currency_format($tier_1_total, 'USD'),
    'tier2' => commerce_currency_format($tier_2_total, 'USD'),
    'total' => $tier_1_total + $tier_2_total,
    'total_commission' => commerce_currency_format($total_commission, 'USD'),
    'wholesale_commissions' => commerce_currency_format($wholesale_commissions, 'USD'),
  );
}

/**
 * Get referral count.
 */
function rtpr_pap_get_referral_data($pap_id, $min_timestamp, $max_timestamp) {
  $results = db_select('field_data_field_pap_referral_id', 'r')
    ->fields('r')
    ->condition('r.field_pap_referral_id_value', $pap_id, '=')
    ->execute()->fetchAllAssoc('entity_id');

  $total_amount = 0;
  $purchasing_referrals = 0;
  foreach ($results as $entity_id => $result) {
    $query = db_select('commerce_order', 'o');
    $query->join('field_data_commerce_order_total', 't', 'o.order_id = t.entity_id');
    $query->fields('o');
    $query->fields('t')
      ->condition('o.status', rtpr_offer_order_success_statuses(), 'IN')
      ->condition('o.uid', $entity_id, '=')
      ->condition('o.created', array($min_timestamp, $max_timestamp), 'BETWEEN');
    // $alias = $query->addExpression('SUM(commerce_order_total_amount)', 'total_amount');.
    $orders = $query->execute()->fetchAllAssoc('order_id');

    if (!empty($orders)) {
      $purchasing_referrals++;
    }
    foreach ($orders as $order_id => $data) {
      $order = commerce_order_load($order_id);
      $wrapper = entity_metadata_wrapper('commerce_order', $order);
      $line_items = $wrapper->commerce_line_items;
      $total = commerce_line_items_total($line_items, array('product'));
      $total_amount += $total['amount'];
    }
  }

  return array(
    'count' => count($results),
    'amount' => commerce_currency_format($total_amount, 'USD'),
    'purchase' => $purchasing_referrals,
  );
}

/**
 * PAP Report Download Form
 */
function rtpr_pap_user_report_download_form($form, $form_state) {
  $report_limit = 10;
  $report_path = 'public://private/pap';
  $files = file_scan_directory($report_path, '/.*.csv$/i');
  krsort($files);
  $header = array(
    'date' => t('Date'),
  );
  $reports = array_slice($files, 0, $report_limit);

  foreach($reports as $report) {
    $uri = $report->uri;
    $rows[] = array(
      'date' => l(date('m/d/Y H:i:s', $report->name), file_create_url($uri)),
    );
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No reports available.'),
  );

  return $form;
}

/**
 * Import users.
 */
function rtpr_pap_impor_users() {
  global $user;
  
  if ($user->uid != 22269) {
    return;
  }
 
  
  
  $csv_file_path = drupal_get_path('module','rtpr_pap') . '/templates/gcap.csv';
  if (($handle = fopen($csv_file_path, "r")) !== FALSE) {
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {

      if (user_load_by_mail($data[0])) {
        continue;
      }
      $new_user = array(
        'name' => $data[0],
        'mail' => $data[0],
        'init' => $data[0],
        'roles' => array(
          DRUPAL_AUTHENTICATED_RID => 'authenticated user',
          7 => 'retail',
        ),
        'field_first_name' => array(
          'und' => array(
            0 => array(
              'value' => $data[1],
            ),
          ),
        ),
        'field_last_name' => array(
          'und' => array(
            0 => array(
              'value' => $data[2],
            ),
          ),
        ),
        'field_phone' => array(
          'und' => array(
            0 => array(
              'value' => $data[8],
            ),
          ),
        ),
        'field_address' => array(
          'und' => array(
            0 => array(
              'country' => 'US',
              'administrative_area' => $data[6],
              'locality' => $data[5],
              'postal_code' => $data[7],
              'thoroughfare' => $data[3],
              'premise' => $data[4],
              
            ),
          ),
        ),
        'field_gcap' => array(
          'und' => array(
            0 => array(
              'value' => 'gcap_pool',
            ),
          ),
        ),
      );
      
      $account = user_save(NULL, $new_user);
      
      if ($account) {
        $account = user_load($account->uid);
        $account->field_pap_referral_id['und'][0]['value'] = '4ca30282';
        user_save($account);
        dpm($account->mail);
      }

    }
    fclose($handle);
  }
  
  
}