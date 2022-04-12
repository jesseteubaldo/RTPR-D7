<?php
  
/**
 * @file
 * User page for Co-Op
 */
 
function rtpr_coop_user_summary_form($form, $form_state, $uid) {
  
   $form['options'] = array(
      '#type' => 'container',
      '#attributes' => array(
          'class' => array('container-inline'),
      ),
  );
  
  $form['options']['year'] = array(
      '#type' => 'select',
      '#title' => t('Year'),
      '#options' => rtpr_reward_year_list(FALSE),
      '#default_value' => date('Y'),
      '#required' => TRUE,
  );

  $form['options']['month'] = array(
      '#type' => 'select',
      '#title' => t('Month'),
      '#options' => array(
          '01' => t('January'),
          '02' => t('February'),
          '03' => t('March'),
          '04' => t('April'),
          '05' => t('May'),
          '06' => t('June'),
          '07' => t('July'),
          '08' => t('August'),
          '09' => t('September'),
          '10' => t('October'),
          '11' => t('November'),
          '12' => t('December'),
      ),
      '#default_value' => date('m'),
      '#required' => TRUE,
  );
  
  $form['options']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
  );

  if (isset($form_state['search'])) {
    $month = $form_state['input']['month'];
    $year = $form_state['input']['year'];
  } else {
    $month = date('m');
    $year = date('Y');
  }

  $options = array(
    'month' => $month,
    'year' => $year,
  );
  
  $type = 'coop';
  $account = user_load($uid);
  $total = rtpr_reward_get_total($uid, $type, $options);
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Customers'),
  );
  
  $rows = rtpr_reward_get_rows($uid, array($type), $options);
  $form_state['uid'] = $uid;
  $form_state['header'] = $header;

  $form['summary'] = array(
    '#prefix' => '<div id="ebp_summary_form">',
    '#suffix' => '</div>',
  );

  $form['summary']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );
  
  return $form;
}


function rtpr_coop_user_summary_form_submit($form, &$form_state) {
  $form_state['search'] = TRUE;
  $form_state['rebuild'] = TRUE;
}