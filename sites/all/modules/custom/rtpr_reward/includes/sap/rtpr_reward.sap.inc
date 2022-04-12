<?php

/**
 * @file
 * Report relate to SAP.
 */


/**
 * Callback for user/%/sap/earned.
 */
function rtpr_reward_user_sap_earmed_summary_form($form, &$form_state, $uid) {
  $items = array(
    'SAP Earned: <span class="underline">' . rtpr_reward_points_earned($uid, 'sap') . '</span>',
    'SAP Redeemed: <span class="underline">' . abs(rtpr_reward_points_used($uid, 'sap')) . '</span>',
    'SAP Available: <span class="underline">' . rtpr_ebp_get_total($uid, 'sap') . '</span>',
  );

  $form['summary'] = array(
    '#title' => t('SAP Summary'),
    '#theme' => 'item_list',
    '#items' => $items,
  );
  
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Points'),
  );
  
  $rows = rtpr_rewards_get_rows($uid, array('sap'));
  $form['divider'] = array(
    '#markup' => '<hr class="dm"></hr>',
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );
  
  return $form;
}

/**
 * Callback for user/%/sap/redeemed.
 */
function rtpr_reward_user_sap_redeemed_summary_form($form, &$form_state, $uid) {    
  $items = array(
    'SAP Earned: <span class="underline">' . rtpr_reward_points_earned($uid, 'sap') . '</span>',
    'SAP Redeemed: <span class="underline">' . abs(rtpr_reward_points_used($uid, 'sap')) . '</span>',
    'SAP Available: <span class="underline">' . rtpr_ebp_get_total($uid, 'sap') . '</span>',
  );

  $form['summary'] = array(
    '#title' => t('SAP Summary'),
    '#theme' => 'item_list',
    '#items' => $items,
  );

  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Points'),
  );
  
  $rows = rtpr_rewards_get_rows($uid, array('sap'), 'Redeem');
  $form['divider'] = array(
    '#markup' => '<hr class="dm"></hr>',
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );
  
  return $form;
}

/**
 * Callback for admin/config/rtpr/points/reports/sap.
 */
function rtpr_reward_reports_sap_form($form, &$form_state) {

  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download SAP report'),
    '#submit' => array('rtpr_reward_current_sap_report_form_submit'),
  );
  
  return $form;
  
  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'total_sap_earned' => t('Total SAP Earned'),
    'total_sap_redeemed' => t('Total SAP Redeemed'),
    'total_sap_remaining' => t('Total SAP Remaining'),
  );
  
  $rows = array();
  $uids = db_query(
    'SELECT * FROM rtpr_reward WHERE point_type = :point_type',
    array(':point_type' => 'sap')
  )->fetchAllAssoc('uid');
  
  foreach ($uids as $uid => $row) {
    $rows[$uid] = array(
      $row->uid,
      $row->email,
      rtpr_reward_points_earned($uid, 'sap'),
      abs(rtpr_reward_points_used($uid, 'sap')),
      rtpr_ebp_get_total($uid, 'sap'),
    );
  }

  $form_state['header'] = $header;
  $form_state['rows'] = $rows;

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No SAP data available.'),
  );
  
  $form['pager'] = array(
    '#theme' => 'pager',
  );

  return $form;
}

/**
 * Download SAP.
 */
function rtpr_reward_current_sap_report_form_submit($form, &$form_state) {

  $uids = db_query(
    'SELECT * FROM rtpr_reward WHERE point_type = :point_type',
    array(':point_type' => 'sap')
  )->fetchAllAssoc('uid');
  $rows = array();
  if ($uids) {
    foreach ($uids as $row) {
      $operations[] = array(
        'rtpr_reward_reports_sap_batch_op',
        array(
          $row,
        ),
      );
    }

    $batch = array(
      'operations' => $operations,
      'finished' => 'rtpr_reward_reports_sap_finished',
      'file' => drupal_get_path('module', 'rtpr_reward') . '/includes/sap/rtpr_reward.sap.inc',
    );

    batch_set($batch);
  }
  
  
/*
  $header = $form_state['header'];
  $rows = $form_state['rows'];
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=sap-' . date('m-d-Y') . '.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();
*/
}

/**
 * Process each user.
 */
function rtpr_reward_reports_sap_batch_op($row, &$context) {
  $uid = $row->uid;
  $row = array(
    'uid' => $uid,
    'email' => $row->email,
    'total_sap_earned' => rtpr_reward_points_earned($uid, 'sap'),
    'total_sap_redeemed' => abs(rtpr_reward_points_used($uid, 'sap')),
    'total_sap_remaining' => rtpr_ebp_get_total($uid, 'sap'),
  );
  
  $context['message'] = t('Processing user %$uid', array('%$uid' => $uid));
  $context['results'][$uid] = $row;
}

/**
 * End of batch.
 */
function rtpr_reward_reports_sap_finished($success, $rows, $operations) {
  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'total_sap_earned' => t('Total SAP Earned'),
    'total_sap_redeemed' => t('Total SAP Redeemed'),
    'total_sap_remaining' => t('Total SAP Remaining'),
  );

  $file_path = 'public://private/' . date('m_d_Y', time()) . '_sap_report.csv';
  $fp = fopen($file_path, 'w');
  
  
  fputcsv($fp, $header);

  foreach($rows as $row){
    fputcsv($fp, $row);
  }

  fclose($fp);
  $download_path = file_create_url($file_path);
  $message = t('Click <a href="@download_path">here</a> to download the report.', array('@download_path' => $download_path));
  drupal_set_message($message);
}
