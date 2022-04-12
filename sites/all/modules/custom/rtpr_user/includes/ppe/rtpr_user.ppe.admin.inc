<?php

/**
 * @file
 */

/**
 * Callback for admin/config/rtpr/ppe/report.
 */
function rtpr_user_ppe_report_form($form, &$form_state) {
  $header = array(
    'uid' => t('UID'),
    'pap_id' => t('PAP ID'),
    'firstname' => t('First Name'),
    'lastname' => t('Last Nam'),
    'email' => t('Email'),
    'self' => t('Self Purchase'),
    'total' => t('Total PPE Organization'),
    'personal' => t('Personal PPE'),
    'gen2' => ('Gen 2'),
    'gen3' => ('Gen 3'),
    'gen4' => ('Gen 4'),
    'gen5' => ('Gen 5'),
    'gen6' => ('Gen 6'),
    'gen6' => ('Gen 7'),
  );

  $transactions = array();
  
  $accounts = db_query(
    'select uid, field_pap_affiliate_id_value, mail, field_first_name_value as first_name, field_last_name_value as last_name from users,field_data_field_pap_affiliate_id as p, field_data_field_first_name as f, field_data_field_last_name as l where uid = p.entity_id  and uid = f.entity_id and uid=l.entity_id'
  )->fetchAllAssoc('field_pap_affiliate_id_value');

  db_set_active('pap');
  for ($tier = 1; $tier <= 8; $tier++) {
    $count = db_query('
      SELECT userid, count(DISTINCT data2) as count FROM qu_pap_transactions WHERE 
      dateinserted > :date AND productid IN (:skus) AND tier = :tier GROUP BY userid',
      array(
        ':date' => '2020-08-15',
        ':skus' => array('PPE-PROGRAM-STARTER-KIT', 'INTERNATIONAL-PPE-PROGRAM-STARTER-KIT', 'PPE-PROGRAM-DIGITAL-KIT', 'PPE-PROGRAM-UPGRADE-KIT'),
        ':tier' => $tier
      )
    )->fetchAllAssoc('userid');
    
    foreach ($count as $pap_id => $row) {
      $transactions[$pap_id][$tier] = $row->count ? $row->count : 0;
      if ($tier > 1) {
        $transactions[$pap_id]['total'] += $transactions[$pap_id][$tier];
      }
    }
  }
  db_set_active();

  $rows = array();
  foreach ($transactions as $pap_id => $transaction) {
    if (isset($accounts[$pap_id])) {
      $account = $accounts[$pap_id];
      $rows[$pap_id] = array(
        $account->uid,
        $pap_id,
        $account->first_name,
        $account->last_name,
        $account->mail,
        empty($transaction[1]) ? 'NO' : 'YES',
        $transaction['total'],
        $transaction[2] ? $transaction[2] : 0,
        $transaction[3] ? $transaction[3] : 0,
        $transaction[4] ? $transaction[4] : 0,
        $transaction[5] ? $transaction[5] : 0,
        $transaction[6] ? $transaction[6] : 0,
        $transaction[7] ? $transaction[7] : 0,
      );
    }
  }
  
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  $form['rows'] = array(
    '#type' => 'value',
    '#value' => $rows,
  );
  
  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No reports available.'),
  );
  
  return $form;
}

/**
 * Download PPE report.
 */
function rtpr_user_ppe_report_form_submit($form, $form_state) {
  $values = $form_state['values'];

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=ppe.csv');
  $out = fopen('php://output', 'w');

  fputcsv($out, $values['header']);

  foreach ($values['rows'] as $row) {
    fputcsv($out, (array) $row);
  }

  fclose($out);
  exit();
}