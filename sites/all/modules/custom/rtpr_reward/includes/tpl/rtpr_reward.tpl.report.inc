<?php

/**
 * @file
 * Report relate to Co-Op Shares.
 */

/**
 * Co-Op Shares report form.
 */
function rtpr_reward_reports_tpl_form($form, &$form_state) {
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download TPL Report'),
  );

  drupal_set_message(variable_get('rtpr_reward_tpl_report'));
  return $form;
}

/**
 * Submit handler.
 */
function rtpr_reward_reports_tpl_form_submit($form, &$form_state) {
  $query = db_select('rtpr_reward', 'rr')
    ->fields('rr', array('uid'))
    ->condition('point_type', 'tpl');

  $uids = $query->execute()->fetchCol();
  $rows = array();
  if ($uids) {
    $uids = array_unique($uids);
    foreach ($uids as $uid) {
      $operations[] = array(
        'rtpr_reward_reports_tpl_batch_op',
        array(
          $uid,
        ),
      );
    }

    $batch = array(
      'operations' => $operations,
      'finished' => 'rtpr_reward_reports_tpl_finished',
      'file' => drupal_get_path('module', 'rtpr_reward') . '/includes/tpl/rtpr_reward.tpl.report.inc',
    );

    batch_set($batch);
  }
  
}

/**
 * Process each user.
 */
function rtpr_reward_reports_tpl_batch_op($uid, &$context) {
  $account = user_load($uid);
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();

  $shares = rtpr_reward_points_earned($uid, 'tpl');
  $tpl_date = $account_wrapper->field_tpl_date->value();
  $row = array(
    'uid' => $account->uid,
    'pap_id' => $account_wrapper->field_pap_affiliate_id->value(),
    'email' => $account->mail,
    'name' => $account_wrapper->field_first_name->value() . ' ' . $account_wrapper->field_last_name->value(),
    'role' => in_array(49, array_keys($account->roles)) ? 'YES' : 'NO',
    'date' => $tpl_date ? date('m/Y', $tpl_date) : 'N/A',
    'total' => rtpr_reward_points_available($uid, 'tpl')
  );
  
  $context['message'] = t('Processing user %$uid', array('%$uid' => $uid));
  $context['results'][$uid] = $row;
}

/**
 * End of batch.
 */
function rtpr_reward_reports_tpl_finished($success, $rows, $operations) {
  $header = array(
    'uid' => t('UID'),
    'pap_id' => t('PAP ID'),
    'email' => t('Email'),
    'name' => t('Name'),
    'role' => t('TPL Role'),
    'date' => t('TPL Start Date'),
    'total' => t('Total TPL'),
  );
  
  $results = db_query("SELECT month, year FROM rtpr_reward WHERE point_type = 'tpl' GROUP BY month, year Order by year, month")->fetchAll();
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
        $rows[$uid][$date] = rtpr_reward_points_available($uid, 'tpl', $date_parse[0], $date_parse[1]);
      }
      else {
        $rows[$uid][$date] = 0;
      }
    }
  }
  
  $file_path = 'public://private/' . date('m_d_Y', time()) . '_tpl_report.csv';
  $fp = fopen($file_path, 'w');

  fputcsv($fp, $header);

  foreach ($rows as $row) {
    fputcsv($fp, $row);
  }

  fclose($fp);
  $download_path = file_create_url($file_path);
  $message = t('Click <a href="@download_path">here</a> to download the report.', array('@download_path' => $download_path));
  drupal_set_message($message);
  variable_set('rtpr_reward_tpl_report', 'Last report was generated on ' . date('m/d/Y H:i:s') . '. ' . $message);
}