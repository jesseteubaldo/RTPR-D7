<?php

/**
 * @file
 * EBP Report.
 */

function rtpr_reward_annual_report_form() {
  $form['options'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Options'), 
    '#attributes' => array('class' => array('container-inline')), 
  );
  $form['options']['start_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'm-Y',
    '#title' => t('to'),
    '#date_label_position' => 'invisible',
    '#date_year_range' => '-2:+0',
    '#required' => TRUE,
  );
  $form['options']['end_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'm-Y',
    '#date_label_position' => 'invisible',
    '#date_year_range' => '-2:+0',
    '#required' => TRUE,
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Download Report',
  );

  return $form;
}


function rtpr_reward_annual_report_form_submit($form, $form_state) {
  $start_date = strtotime($form_state['values']['start_date']);
  $end_date = strtotime($form_state['values']['end_date']);

  $query = db_select('rtpr_reward', 'reward');
  $query->join('users_roles', 'role', 'role.uid = reward.uid');
  $query->fields('reward')
    ->condition('insertdate', array($start_date, $end_date), 'BETWEEN')
    //->condition('id', 30899)
    ->condition('point_type', 'ebp')
    ->condition('role.rid', 36);
  
  $result = $query->execute()->fetchAll();
}

// Export data
function rtpr_reward_report_form_submit($form, &$form_state) {
  
  $header = array(
    'id' => t('PAP ID'),
    'email' => t('Email'),
    'primary' => t('Current Status'),
    'override' => t('Override'),
    'vendor' => t('Vendor-Own'),
    'ebp' => t('Total EBP'),
    'multi' => t('Total Multiplication Factor'),
    'bonus' => t('Total Super Bonus points'),
    'total' => t('Points Total'),
  );
  
  
  $query = db_select('rtpr_reward', 'r')
    ->fields('r', array('uid'))
    ->condition('point_type', 'ebp');
    
  $uids = $query->distinct()->execute()->fetchCol();
  
  foreach($uids as $uid) {
    $account = user_load($uid);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    
    $primary_role = rtpr_user_get_primary_role($account->roles);
    $is_vendor_owner = rtpr_ebp_has_vendor_owner_role($account->roles);
    $is_override = rtpr_ebp_has_override_role($account->roles);
    $ebp = rtpr_ebp_get_total($uid, 'ebp');
    $multi_factor = rtpr_ebp_get_total($uid, 'multi_factor');
    if (!$multi_factor) {
      $multi_factor = 1;
    }
    $super_bonus = rtpr_ebp_get_total($uid, 'super_bonus');
    $total = ($ebp * $multi_factor) + $super_bonus;
    $rows[$uid] = array(
      $account_wrapper->field_pap_affiliate_id->value(),
      $account->mail,
      $primary_role,
      $is_override,
      $is_vendor_owner,
      $ebp,
      $multi_factor,
      $super_bonus,
      $total,
    );
  }

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=ebp_export.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit(); 

}