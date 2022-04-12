<?php
  
/**
 * @file
 *
 * User reward page.
 */

// Summary form
function rtpr_reward_user_pib_form($form, $form_state, $uid) {
  
  
  $total = rtpr_ebp_get_total($uid, 'pib');
  if (!$total) {
    $total = 0;
  }
  $form['message'] = array(
    '#markup' => '<h4><strong>Total Passive Income Bonus = ' . commerce_currency_format($total * 100, 'USD'). '</strong></h4>',
  );
  $account = user_load($uid);
  $wrapper = entity_metadata_wrapper('user', $account);
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Amount'),
  );
  
  $rows = rtpr_ebp_get_rows($uid, array('pib'));

  foreach ($rows as $index => $row) {
    $points = $rows[$index]['data']['points'];
    $rows[$index]['data']['points'] = commerce_currency_format($points * 100, 'USD');
  }
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
 * PIB Total Report.
 */
function rtpr_reward_pib_report_form($form, &$form_state) {
  $form['download'] = array(
    '#type' => 'submit',
    '#value'=> t('Download Report'),
  );

  return $form;
}

function rtpr_reward_pib_report_form_submit($form, &$form_state) {
  $header = array(
    'pap_id' => t('PAP ID'),
    'email' => t('Email'),
    'name' => t('Name'),
    'status' => t('Current Status'),
    'total_pib' => t('Total PIB Amount'),
  );

  $start    = new DateTime('06/01/2021');
  $end  = new DateTime();
  $interval = DateInterval::createFromDateString('1 month');
  $period   = new DatePeriod($start, $interval, $end);
  foreach ($period as $dt) {
    $header[$dt->format('mY')] = $dt->format('M-Y');
  }
  
  $query = db_select('rtpr_reward', 'rw')
    ->fields('rw', array('uid'))
    ->condition('point_type', 'pib')
    ->groupBy('uid');

  $uids = $query->execute()->fetchCol();
  $rows = array();
  foreach($uids as $uid) {
    $premium_rid = _rtpr_user_has_rids($uid, array(44,57,59,60,61));
    $account = user_load($uid);
    $roles = $account->roles;
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $rows[$uid] = array(
      'pap_id' => $account_wrapper->field_pap_affiliate_id->value(),
      'email' => $account->mail,
      'name' => $account_wrapper->field_first_name->value() . ' ' . $account_wrapper->field_last_name->value(),
      'status' => $premium_rid? user_role_load($premium_rid)->name : 'N/A',
      'total_pib' => rtpr_ebp_get_total($uid, 'pib'),
    );
    foreach ($period as $dt) {
      $rows[$uid][$dt->format('mY')] = rtpr_ebp_get_total($uid, 'pib', $dt->format('m'), $dt->format('Y')); 
    }
  }

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=pib-report.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();  
}
