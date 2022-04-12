<?php

/**
 * @file
 * 
 * Reports related to T2.
 */

/**
 * Reminder email sent report.
 */
function rtpr_cc_redemption_t2_report_reminder_form($form, $form_state) {

  $user_logins = variable_get('rptr_cc_reminder_users');
  $users = variable_get('rptr_cc_reminder_users');

  $header = array(
    'date' => t('Date'),
    'count' => t('Count'),
  );
  
  $data = array();
  $ten_days_in_second = 10 * 24 * 60 * 60;
  foreach($users as $uid => $timestamp) {
    $sent_date = $timestamp - $ten_days_in_second;
    if ($sent_date > strtotime('-7 days')) {
      $date = date('m/d/Y', $sent_date);
      if (!isset($data[$date])) {
        $data[$date] = 0;
      }
      $data[$date]++;
    }
  }

  $rows = array();
  foreach($data as $date => $count) {
    // No data available prior to the request.
    if ($count > 2000) {
      $timestamp = strtotime('-10 days', strtotime($date));
      $next_day = strtotime('+1 day', $timestamp);
      $user_ids = (!empty($user_logins)) ? array_keys($user_logins) : array('0');
      $query = db_select('users_roles', 'r');
      $query->join('users', 'u', 'u.uid = r.uid');
      $query->fields('r');
      $query->fields('u', array('login'))
        ->condition('created', $timestamp, '>' )
        ->condition('created', $next_day, '<' )
        ->condition('init', 'unsubscribe', '!=')
        ->condition('access', 0)
        ->condition('rid', RTPR_CC_POTENTIAL_CUSTOMER_RID);
  
      $results = $query->execute()->fetchAll();
      $count = count($results);
    }
    $rows[] = array(
      'date' => $date,
      'count' => $count,
    );
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );

  return $form;
}