<?php
  
/**
 * @file
 *
 * History for incentive program activities.
 */

function rtpr_reward_history_form($form, &$form_state) {
  drupal_set_title(t('RTPR Points - History/Export'));
  $form['options'] = array(
    '#title'=> t('Search Options'),
    '#type' => 'fieldset',
    '#attributes' => array(
        'class' => array('container-inline'),
    ),
  '#collapsible' => TRUE, 
  '#collapsed' => FALSE,

  );
 
  $form['options']['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => rtpr_reward_month_list()

  );
  
  $form['options']['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => rtpr_reward_year_list()
  );
  
  $form['options']['batch'] = array(
    '#title' => t('Batch'),
    '#type' => 'select',
    '#options' => rtpr_reward_batch_list(),
  );
  
  $form['options']['group'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => rtpr_reward_point_types(),
    '#default_value' => 'reward',
  );
  
  $form['options']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#autocomplete_path' => 'rtprcc/autocomplete',
    '#attributes' => array('size' => '40')
  );
  
  $form['options']['submit'] = array(
    '#title' => t('Search'),
    '#type' => 'submit',
    '#value' => 'Search',

  );
  
  $form['options']['reset'] = array(
    '#title' => t('Reset'),
    '#type' => 'submit',
    '#value' => 'Reset',
    '#submit' => array('rtpr_reward_history_form_reset'),
  );
  
  $header = array(
    'id' => t('ID'),
    'batch' => t('Batch #'),
    'email' => t('Email'),
    'point_type' => t('Type'),
    'point' => t('Point'),
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'timestamp' => t('Date'),
  );
  
  // $selected_value will only have a value after submit (see submit function)
  $rows = array();
  $per_page = 50;
  
  if (isset($form_state['month'])) {
    $values = $form_state['values'];
    $month = isset($values['month']) ? $values['month'] : '';
    $year = isset($values['year']) ? $values['year'] : '';
    $email = isset($values['email']) ? $values['email'] : '';
    $batch = isset($values['batch']) ? $values['batch'] : '';
    $group = isset($values['group']) ? $values['group'] : '';
    
    $query = db_select('rtpr_reward','r');
    $query->fields('p', array('field_pap_affiliate_id_value'));
    $query->fields('r', array_keys($header))
    ->orderBy('id', 'DESC');
    $query->leftJoin('field_data_field_pap_affiliate_id', 'p', 'p.entity_id = r.uid');
    if ($month)
      $query->condition('month', $month, '=');
    if ($year)
      $query->condition('year', $year, '=');
    if ($email)
      $query->condition('email', $email, '=');
    if ($batch)
      $query->condition('batch', $batch, '=');
    if($group)
      $query->condition('point_type', $group, '=');

    
    $results = $query->execute()->fetchAllAssoc('id');

    foreach($results as $result) {
      $result->timestamp = date('m/d/Y H:i a', $result->timestamp);
      $rows[$result->id] = (array)$result;
    }
  } else {
    $query = db_select('rtpr_reward','r');
    $query->leftJoin('field_data_field_pap_affiliate_id', 'p', 'p.entity_id = r.uid');
    $query->fields('r', array_keys($header))
    ->orderBy('id', 'DESC');
    $query->fields('p', array('field_pap_affiliate_id_value'));
    $results = $query->execute()->fetchAllAssoc('id');

    foreach($results as $result) {
      $result->timestamp = date('m/d/Y H:i a', $result->timestamp);
      $rows[$result->id] = (array)$result;
    }

  }
  $header = array('field_pap_affiliate_id_value' => t('PAP ID')) + $header;

  $form_state['header'] = $header;
  $form_state['rows'] = $rows;

  $current_page = pager_default_initialize(count($rows), $per_page);
  // split array into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);
  
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => isset($chunks[$current_page]) ? $chunks[$current_page] : array(),
    '#empty' => t('No history available for selected criteria.'),
  );
  
  $form['pager'] = array(
    '#theme' => 'pager',
  );

  $form['export'] = array(
    '#type' => 'submit',
    '#value' => 'Export Selected',
    '#submit' => array('rtpr_reward_history_export_submit'),
  );

  $form['export_all'] = array(
    '#type' => 'submit',
    '#value' => 'Export All',
    '#submit' => array('rtpr_reward_history_export_submit'),
  );
  
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#submit' => array('rtpr_reward_history_delete_submit'),
  );
    
  return $form;
}

function rtpr_reward_history_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $form_state['month'] = $values['month'];

// Keep submitted value
  $form_state['rebuild'] = TRUE;

}

function rtpr_reward_history_form_reset($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/rtpr/reward/history';
}


// Export data
function rtpr_reward_history_export_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $values = $form_state['values']['table'];
  $all_rows = $form_state['rows'];

  $rows = array();

  if ($op == 'Export All') {
    foreach($all_rows as $key => $value) {
      $lastvalue = end($value);
      $lastkey = key($value);
      $arr1 = array($lastkey=>$lastvalue);
      array_pop($value);
      $arr1 = array_merge($arr1,$value);
      $rows[$key] = $arr1;
    }
  } else {
    foreach($values as $key => $value) {
      if(is_string($value)) {
        $rows[$key] = $all_rows[$key];
      }
    }
  }

  $header = $form_state['header'];
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=export.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();  
}

function rtpr_reward_history_delete_submit($form, &$form_state) {
  $values = $form_state['values']['table'];
  $all_rows = $form_state['rows'];
  $rows = array();

  foreach($values as $key => $value) {

    if(is_string($value)) {
      $rows[$key] = $all_rows[$key];
    }
  }
  
  $ids = array_keys($rows);

  if(!empty($ids) && is_array($ids)) {
    $num_deleted = db_delete('rtpr_reward')
    ->condition('id', $ids, 'IN')
    ->execute();
  }
  
}