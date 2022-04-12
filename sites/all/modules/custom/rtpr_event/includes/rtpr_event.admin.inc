<?php

/**
 * @file
 * Event related admin features.
 */

function rtpr_event_status_count_report($form, $form_state) {
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

/**
 * Submit handler for event report.
 */
function rtpr_event_status_count_report_submit($form, $form_state) {
  $query = db_select('field_data_field_gsl_status', 's');
  $query->join('node', 'n', 'n.nid = s.entity_id');
  $query->join('users', 'u', 'u.uid = n.uid');
  $query->leftJoin('field_data_field_pap_affiliate_id', 'p', 'p.entity_id = n.uid');
  $query->fields('n', array('uid', 'nid'))
    ->fields('u', array('mail'))
    ->fields('s', array('field_gsl_status_value'))
    ->fields('p', array('field_pap_affiliate_id_value'))
    ->condition('s.bundle', 'store_location');
  $results =  $query->execute()->fetchAll();

  $statuses = array(
    'approve' => 'Approved',
    'pending' => 'Pending',
    'declined' => 'Info Needed',
    'resubmit' => 'Resubmitted',
    'fulldecline' => 'Fully Declined',
    'abandoned' => 'Abandoned',
    'released' => 'Released by Vendor',
    'recertification' => 'Recertification',
    'hold' => 'On Hold',
    'recertification_pending' => 'Recertification Pending',
    'level_1_review' => 'Level 1 Review',
    'level_2_review' => 'Level 2 Review',
  );
  $accounts = array();
  
  foreach($results as $result) {
    $accounts[$result->uid]['pap_id'] = $result->field_pap_affiliate_id_value;
    $accounts[$result->uid]['mail'] = $result->mail;
    $accounts[$result->uid]['status'][$result->field_gsl_status_value]++;  
  }

  $header = array(
    'pap_id' => t('PAP ID'),
    'mmail' => t('Email'),
    'approve' => 'Approved',
    'pending' => 'Pending',
    'declined' => 'Info Needed',
    'resubmit' => 'Resubmitted',
    'abandoned' => 'Abandoned',
    'released' => 'Released by Vendor',
    'recertification' => 'Recertification',
    'hold' => 'On Hold',
    'level_1_review' => 'Level 1 Review',
    'level_2_review' => 'Level 2 Review',
  );

  $rows = array();
  foreach($accounts as $uid => $data) {
    $row = array();
    $temp = array_fill_keys(array_keys($statuses), 0);
    $row['pap_id'] = $data['pap_id'];
    $row['mail'] = $data['mail'];
    $row += $temp;
    $row = array_merge($row, $accounts[$uid]['status']);
    $row['declined'] += $rows[$pap_id]['fulldecline'];
    $row['recertification'] += $rows[$pap_id]['recertification_pending'];
    unset($row['fulldecline']);
    unset($row['recertification_pending']);
    $rows[] = $row;
  }


  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=events-report.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    unset($row['operations']);
    fputcsv($out, $row);
  }

  fclose($out);
  exit(); 
}