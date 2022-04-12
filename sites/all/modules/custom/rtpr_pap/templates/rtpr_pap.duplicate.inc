<?php

/**
 * Administrator features for PAP
 */


/**
 * Display missing orders from PAP.
 */
function rtpr_pap_duplicate_orders_form($form, &$form_state) {

  $oldest_year = 2011;
  $current_year = date('Y');
  
  $years = array();
  
  for($year = $oldest_year; $year <= $current_year; $year++) {
    $years[$year] = $year;
  }
  
  $months = array();
  for($month = 1; $month <= 12; $month++) {
    $months[$month] = $month;
  }
  
  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );
    
  $form['options']['year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => $years,
    '#default_value' => $current_year,
  );
  
  $form['options']['month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => $months,
    '#default_value' => date('n'),
  );
  
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('rtpr_pap_duplicate_orders_search_form_submit'),
  );
  
  if(isset($form_state) && isset($form_state['rows'])) {
    $rows = $form_state['rows'];
    $header = array(
      'index' => t('#'),
      'order_id' => t('Order #'),
      'commission' =>  t('Commission'),
    );
    
    $form['orders'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No duplicate orders found.'),
    );
  }

  
  return $form;
}

function rtpr_pap_duplicate_orders_search_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  $year = $values['year'];
  $month = $values['month'];
  $start_date = $year . '-' . $month . '-01 00:00:01';
  $end_date = $year . '-' . $month . '-31 23:59:59';
  
  db_set_active('pap');
  $query = db_select('qu_pap_transactions', 't')
  ->fields('t', array('orderid', 'tier', 'commission'))
  ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
  ->condition('tier' , 1, '=')
  ->groupBy('orderid')
  ->groupBy('productid')
  ->having('ncount > 1');
  $query->addExpression('COUNT(orderid)', 'ncount');

  $results = $query->execute();
  $rows = array();
  $index = 1;
  foreach($results as  $result) {
    if(!preg_match("/[a-z]/i", $result->orderid)){
      $rows[] = array(
        'index' => $index,
        'orderid' => $result->orderid,
        'commission' => $result->commission,
      );
      $index++;
    }
    
  }
  db_set_active();
  
  $form_state['rows'] = $rows;
  
  // Keep submitted value
  $form_state['rebuild'] = TRUE;
  
}