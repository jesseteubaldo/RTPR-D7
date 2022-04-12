<?php

/**
 * @file
 * Report relate to Co-Op Shares.
 */

/**
 * Co-Op Shares report form.
 */
function rtpr_reward_reports_shares_form($form, &$form_state) {
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download Co-Op Shares Report'),
  );

  drupal_set_message(variable_get('rtpr_reward_coop_shares_report'));
  return $form;
}

/**
 * Submit handler.
 */
function rtpr_reward_reports_shares_form_submit($form, &$form_state) {
  $query = db_select('rtpr_reward', 'rr')
    ->fields('rr', array('uid'))
    ->condition('point_type', 'coop_share');
    //->range(0,10);

  $uids = $query->execute()->fetchCol();
  $rows = array();
  if ($uids) {
    $uids = array_unique($uids);
    foreach ($uids as $uid) {
      $operations[] = array(
        'rtpr_reward_reports_shares_batch_op',
        array(
          $uid,
        ),
      );
    }

    $batch = array(
      'operations' => $operations,
      'finished' => 'rtpr_reward_reports_shares_finished',
      'file' => drupal_get_path('module', 'rtpr_reward') . '/includes/shares/rtpr_reward.shares.report.inc',
    );

    batch_set($batch);
  }

}

/**
 * Process each user.
 */
function rtpr_reward_reports_shares_batch_op($uid, &$context) {
  $account = user_load($uid);

  try {
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $account_wrapper->field_pap_affiliate_id->value();
    $parent = array();
    if ($pap_id) {

      $affiliate = rtpr_pap_get_uesr_session();
      $affiliate->setUserid($pap_id);
      try {
        $affiliate->load();
        $parent_pap_id = $affiliate->getParentUserId();

        if ($parent_pap_id) {
          $parent['pap_id'] = $parent_pap_id;
          $affiliate->setUserid($parent_pap_id);
          $affiliate->load();
          $parent['email'] = $affiliate->getUsername();
          $parent['name'] = $affiliate->getFirstname() . ' ' . $affiliate->getLastName();
        }
      } catch (Exception $e) {

      }
    }
    module_load_include('inc', 'rtpr_reward', 'includes/shares/user');
    $account_status = $account_wrapper->field_coop_status->value();
    $shares = rtpr_reward_points_earned($uid, 'coop_share');
    $status = _rtpr_reward_coop_status($shares, $account_status);
    if (!empty($parent['email'])) {
      $parent_account = user_load_by_mail($parent['email']);
      $account_status = $account_wrapper->field_coop_status->value();
      $shares = rtpr_reward_points_earned($parent_account->uid, 'coop_share');
      $parent_status = _rtpr_reward_coop_status($shares, $account_status);

      $after_2019 = rtpr_reward_points_earned_after($uid, 'coop_share', strtotime('first day of January'));
      $shares_2019 = $shares - $after_2019;
      $status_2019 = _rtpr_reward_coop_status($shares_2019, $account_status);
    }

    $row = array(
      'uid' => $account->uid,
      'pap_id' => $account_wrapper->field_pap_affiliate_id->value(),
      'email' => $account->mail,
      'name' => $account_wrapper->field_first_name->value() . ' ' . $account_wrapper->field_last_name->value(),
      'club' => $status['current'],
      'last_club' => $status_2019['current'],
      'club_override' => $account_status ? 'YES' : 'NO',
      'parent_pap_id' => isset($parent['pap_id']) ? $parent['pap_id'] : 'N/A',
      'parent_email' => isset($parent['email']) ? $parent['email'] : 'N/A',
      'parent_name' => isset($parent['name']) ? $parent['name'] : 'N/A',
      'parent_club' => isset($parent_status['current']) ? $parent_status['current'] : 'N/A',
      'total' => rtpr_reward_points_available($uid, 'coop_share'),
      'last_points' => rtpr_reward_points_available_before_year($uid, 'coop_share', 2020)
    );

    $context['message'] = t('Processing user %$uid', array('%$uid' => $uid));
    $context['results'][$uid] = $row;
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_reward',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
}

/**
 * End of batch.
 */
function rtpr_reward_reports_shares_finished($success, $rows, $operations) {
  $header = array(
    'uid' => t('UID'),
    'pap_id' => t('PAP ID'),
    'email' => t('Email'),
    'name' => t('Name'),
    'club' => t('2021 User Club Level'),
    'last_club' => t('2020 User Club Level'),
    'club_override' => t('Club Override'),
    'parent_pap_id' => t('Parent PAP ID'),
    'parent_email' => t('Parent Email'),
    'parent_name' => t('Parent Name'),
    'parent_club' => t('Parent Club Level'),
    'total' => t('Total Points'),
    'last_points' => t('Thru 2019'),
  );

  $results = db_query("SELECT month, year FROM rtpr_reward WHERE point_type = 'coop_share' GROUP BY month, year Order by year, month")->fetchAll();
  $dates = array();
  foreach($results as $date) {
    $month = $date->month;
    $year = $date->year;
    $dates[$month.$year] = $month . '/' . $year;
    $header[$month.$year] = $month . '/' . $year;
  }

  foreach ($rows as $uid => $row) {
    foreach($dates as $date) {
      $date_parse = explode('/', $date);
      if (!empty($date_parse[0])) {
        $rows[$uid][$date] = rtpr_reward_points_available($uid, 'coop_share', $date_parse[0], $date_parse[1]);
      }
      else {
        $rows[$uid][$date] = 0;
      }
    }
  }

  $file_path = 'public://private/' . date('m_d_Y', time()) . '_coop_report.csv';
  $fp = fopen($file_path, 'w');

  fputcsv($fp, $header);

  foreach ($rows as $row) {
    fputcsv($fp, $row);
  }

  fclose($fp);
  $download_path = file_create_url($file_path);
  $message = t('Click <a href="@download_path">here</a> to download the report.', array('@download_path' => $download_path));
  drupal_set_message($message);
  variable_set('rtpr_reward_coop_shares_report', 'Last report was generated on ' . date('m/d/Y H:i:s') . '. ' . $message);
}