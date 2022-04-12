<?php

/**
 * @file
 * Administrator features for PAP.
 */

/**
 * PAP user report.
 */
function rtpr_pap_retail_report_form($form, $form_state) {
  drupal_set_title(t('Retail Total Cost 7 Tiers'));

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

  $form['options']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Month 123'),
    '#options' => array(
      '4' => t('Affiliate'),
      '8' => t('Vendor'),
      '9' => t('Gold'),
      '22' => t('Platinum Winners Circle'),
      '37' => t('Sampler'),
      '38' => t('RCP'),
      '47' => t('Rewards'),
      '44' => t('Diamond'),

    ),
    '#default_value' => array(38,47),

    '#multiple' => TRUE,
    '#required' => TRUE,
  );

  $form['fast'] = array(
    '#type' => 'checkbox',
    '#default_value' => TRUE,
    '#title' => t('Ignore non-essential fields.'),
    '#description' => t('Following fields will not have data: Total Purchasing Customers, Total Sampler Commissions, Total Wholesale Commissions.')
  );

  $form['options']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      //'#disabled' => variable_get('rtpr_pap_user_retail_report_disabled', FALSE),
  );


  if (variable_get('rtpr_pap_user_retail_report_disabled')) {
    $batch_id = variable_get('rtpr_pap_user_retail_batch_id');
    $progress = progress_get_progress('_background_batch:' . $batch_id);
    if ($progress) {
      $percentage = $progress->progress * 100;
      drupal_set_message(t('There\'s already a report started by @mail.', array('@mail' => variable_get('rtpr_pap_user_retail_report_mail'))), 'warning');
      if ($percentage > 98) {
        drupal_set_message(t('You can click <a href="@batch_link">here</a> to download the report.', array('@batch_link'=> '/batch?op=start&id=' . $batch_id)));
      } else {
        drupal_set_message(t('It\'s currently at @percentage%.', array('@percentage' => $percentage)));
      }
    }
  }

  return $form;
}

/**
 * Retail report submit handler.
 */
function rtpr_pap_retail_report_form_submit($form, $form_state) {
  global $user;
  $values = $form_state['values'];
  $month = $values['month'];
  $year = $values['year'];
  $rids = array_keys($values['roles']);
  $fast = $values['fast'];

  batch_set(rtpr_pap_process_retail_reports($month, $year, $rids, $fast));
  background_batch_process_batch();
  variable_set('rtpr_pap_user_retail_report_disabled', TRUE);
  variable_set('rtpr_pap_user_retail_report_mail', $user->mail);
  $batch = batch_get();
  variable_set('rtpr_pap_user_retail_batch_id', $batch['id']);

}

/** Get uid for children.
  *
  */
function rptr_pap_get_children_uid($parent_id) {
  db_set_active('pap');
  $query = db_select('qu_pap_users', 'pu')
    ->fields('pu', array('userid'))
    ->condition('pu.parentuserid', $parent_id, '=');
  $results = $query->execute()->fetchAllAssoc('userid');
  $user_ids = array_keys($results);
  db_set_active();

  $query = db_select('field_data_field_pap_affiliate_id', 'pap');
  $query->join('users_roles', 'u', 'pap.entity_id = u.uid');

  $query->fields('pap')
    ->condition('field_pap_affiliate_id_value', $user_ids, 'IN');
  $children = $query->execute()->fetchAllAssoc('entity_id');

  return array_keys($children);
}
/**
 *
 */
function rtpr_pap_process_retail_reports($month, $year, $rids, $fast = FALSE) {
  $query = db_select('users', 'u');
  $query->join('users_roles', 'r', 'r.uid = u.uid');
  $query->fields('u', array('uid'))
//   ->range(0,10)
  ->condition('rid', $rids, 'IN')
  ->condition('u.uid', array(48001,47868,48003,327134,38699,76817,330845,38679), 'NOT IN');
  $results = $query->execute()->fetchCol();
  $operations = array();
  foreach ($results as $uid) {
    $data = array(
        'month' => $month,
        'year' => $year,
        'uid' => $uid,
        'fast' => $fast,
    );
    $operations[] = array(
        'rtpr_pap_process_retail_tier_report_op',
        array(
            $data,
        ),
    );
  }

  // Build the batch instructions.
  $batch = array(
      'operations' => $operations,
      'finished' => 'rtpr_pap_process_retail_reports_finished',
      'file' => drupal_get_path('module', 'rtpr_pap') . '/templates/rtpr_pap.retail.inc',
  );

  return $batch;
}

/**
 *  Callback: batch finish.
 */
function rtpr_pap_process_retail_reports_finished($success, $results, $operations) {
  variable_set('rtpr_pap_user_retail_report_disabled', FALSE);
  $header = array(
    'id' => t('PAP ID'),
    'email' => t('Email'),
    'first_name' => t('First Name'),
    'last_name' => t('Last Name'),
    'state' => t('State'),
    'primary_roles' => t('Primary Roles'),
//     'override_role' => t('Override Role'),
//     'vendor_owner_role' => t('Vendor-Owner Role'),
//     'diamond_club_role' => t('Diamond Club Role'),
//     'retail_total' => t('Retail Total Cost'),
//     'total_nav' => t('Wholesale Total Cost'),
    'total_msp' => t('Total MSP'),
//     'total_apv' => t('Total APV'),
//     'sampler_commissions' => t('Total Sampler Commissions'),
//     'wholesale_commissions' => t('Total Wholesale Commissions'),
//     'platinum_referrals' => t('Platinum Referrals'),
//     'purchasing_customers' => t('Total Purchasing Customers'),
//     'new_referrals' => t('New Customers This Month'),
//     'gcc_used' => t('GCC Used this Month'),
//     'gcc_available' => t('GCC Available'),
//     'activate_this_month' => t('Activated THIS Month'),
     'rp' => t('RP'),
    'parent_first_name' => t('PAP Parent First Name'),
    'parent_last_name' => t('PAP Parent Last Name'),
    'parent_email' => t('PAP Parent Username Email'),
    'parent_primary_role' => t('Parent Primary Role'),
    'parent_id' => t('PAP Parent User ID'),
    'parent_state' => t('PAP Parent State'),
    'month' => t('Month'),
  );

  if ($success) {

    drupal_set_message(t('@count user processed.', array('@count' => count($results))));

    $file_path = 'public://private/retail/' . time() . '_retail.csv';
    $fp = fopen($file_path, 'w');

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
    watchdog('rtpr_pap', t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }
}


/**
 *  Callback: batch operation
 */
function rtpr_pap_process_retail_tier_report_op($data, &$context) {
  module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
  module_load_include('inc','rtpr_pap','templates/rtpr_pap.admin');
  module_load_include('inc','rtpr_cc','includes/rtpr_cc.gcc');
  $month = $data['month'];
  $year = $data['year'];
  $uid = $data['uid'];
  $fast = $data['fast'];

  $context['message'] = t('Processing user with id "@id"', array('@id' => $uid));
  $account = user_load($uid);
  $wrapper = entity_metadata_wrapper('user', $account);
  $address = $wrapper->field_address->value();
  $pap_id = $wrapper->field_pap_affiliate_id->value();

  if ($pap_id) {

    $user_data = new PAPUserData($pap_id, $month, $year, $account, $wrapper);
    $start_date = $year . '-' . $month . '-01';
    $end_date = $year . '-' . $month . '-31';
    $start_timestamp = strtotime('first day of ' . $year . '-' . $month . ' 00:00:00');
    $end_timestamp = strtotime('last day of ' . $year . '-' . $month . ' 23:59:59');

    // Referral.
    if (!$fast) {
      //$retail_data = rtpr_pap_retail_customer_referral_data($pap_id, $start_date, $end_date);
    }
    else {
      $retail_data['total_commission'] = 'N/A';
      $retail_data['wholesale_commissions'] = 'N/A';
      $retail_data['purchase'] = 'N/A';
    }

    $row['id'] = $pap_id;
    $row['email'] = $account->mail;
    $row['first_name'] = $wrapper->field_first_name->value();
    $row['last_name'] = $wrapper->field_last_name->value();
    $row['state'] = $address['administrative_area'];
    $row['primary_roles'] = $user_data->getPrimaryRole();
//     $row['override_role'] = $user_data->hasRole(PAPUserData::ROLE_OVERRIDE_RID);
//     $row['vendor_owner_role'] = $user_data->hasRole(PAPUserData::ROLE_VENDOR_OWNER_RID);
//     $row['diamond_club_role'] = $user_data->hasRole(PAPUserData::ROLE_DIAMOND_CLUB_RID);
//     $row['retail_total'] = '$' . number_format($user_data->getMonthlyRetailTotal(), 2);
//     $row['total_nav'] = '$' . number_format($user_data->getVolumeTotal('nav'), 2);
    $row['total_msp'] = '$' . number_format(_rtpr_user_get_monthly_pmsp($account->uid, $month, $year), 2);
//     $row['total_apv'] = '$' . number_format($user_data->getTotalAPV($pap_id), 2);
//     $row['sampler_commissions'] = $retail_data['total_commission'];
//     $row['wholesale_commissions'] = $retail_data['wholesale_commissions'];
//     $row['platinum_referrals'] = $user_data->getPlatinumReferrals();
//     $row['purchasing_customers'] = $retail_data['purchase'];
//     $row['new_referrals'] = $user_data->getNewReferrals();
//     $row['gcc_used'] = $user_data->getGccUsed();
//     $row['gcc_available'] = _rtpr_user_get_available_redemption_credits($uid);
//     $row['activate_this_month'] = _rtpr_cc_gca_total_count($uid, $start_timestamp, $end_timestamp);
     $row['rp'] = '$' . number_format(_rtpr_user_get_monthly_credits_used($account->uid, $month, $year), 2);

    $parent_data = $user_data->getParentData();

    $row['parent_first_name'] = $parent_data['firstname'];
    $row['parent_last_name'] = $parent_data['lastname'];
    $row['parent_email'] = $parent_data['username'];
    $row['parent_primary_role'] = $user_data->getPrimaryRole($parent_data['username']);
    $row['parent_state'] = $parent_data['data5'];
    $row['parent_id'] = $parent_data['userid'];
    $row['month'] = $month . ' - ' . $year;
    $context['results'][$uid] = $row;
  }
}

/**
 * PAP retail report download form.
 */
function rtpr_pap_retail_report_download_form($form, $form_state) {
  $report_limit = 10;
  $report_path = 'public://private/retail';
  $files = file_scan_directory($report_path, '/.*.csv$/i');
  krsort($files);
  $header = array(
      'date' => t('Date'),
  );
  $reports = array_slice($files, 0, $report_limit);
  $rows = array();
  foreach($reports as $report) {
    $uri = $report->uri;
    $timestamp = preg_replace("/[^0-9]/", '', $report->name);
    if (strpos($uri, 'retail') !== FALSE) {
      $rows[] = array(
          'date' => l(date('m/d/Y H:i:s',$timestamp), file_create_url($uri)),
      );
    }
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No reports available.'),
  );

  return $form;
}
