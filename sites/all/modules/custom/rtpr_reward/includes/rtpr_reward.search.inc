<?php
  
/**
 * @file
 *
 * Ability to look in up a user and see their Total Earned Incentive Points, Used Points, Remaining Points.
 */
 
function rtpr_reward_search_form($form, &$form_state) {
  drupal_set_title(t('RTPR Points - Search'));
  $form['options'] = array(
    '#title'=> t('Search Options'),
    '#type' => 'fieldset',
    '#attributes' => array(
        'class' => array('container-inline'),
    ),
  '#collapsible' => TRUE, 
  '#collapsed' => FALSE,

  );
  
  $form['options']['group'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => rtpr_reward_point_types(),
    '#default_value' => 'reward',
  );
  
  
  $form['options']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#autocomplete_path' => 'rtprcc/autocomplete',
    '#required' => TRUE,
  );
  
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  $header = array();
  $rows = array();
  
  if (isset($form_state['uid'])) {
      $uid = $form_state['uid'];
      $account = user_load($uid);
      $wrapper = entity_metadata_wrapper('user', $account);
      if($form_state['group'] == 'reward'){
        
        $header = array(
          'PAP' => t('PAP ID'),
          'email' => t('Email'),
          'first' => t('First Name'),
          'last' => t('Last Name'),
          'total' => t('EarnedPoints '),
          'user' => t('Used Points'),
          'remain' => t('Remaining points'),
        );
        $rows[] = array(
          'pap' => $wrapper->field_pap_affiliate_id->value(),
          'email' => $account->mail,
          'first' => $wrapper->field_first_name->value(),
          'last' => $wrapper->field_last_name->value(),
          'total' => rtpr_reward_points_earned($uid),
          'user' => abs(rtpr_reward_points_used($uid)),
          'remain' => rtpr_reward_points_available($uid)
        );
      } else if ($form_state['group'] == 'ebp'){
        $header = array(
          'PAP' => t('PAP ID'),
          'email' => t('Email'),
          'first' => t('First Name'),
          'last' => t('Last Name'),
          'total' => t('Total EBP '),

        );
        $rows[] = array(
          'pap' => $wrapper->field_pap_affiliate_id->value(),
          'email' => $account->mail,
          'first' => $wrapper->field_first_name->value(),
          'last' => $wrapper->field_last_name->value(),
          'total' => rtpr_reward_total_ebp($uid),
        );
      }
      elseif($form_state['group'] == 'afl'){
        $header = array(
          'PAP' => t('PAP ID'),
          'email' => t('Email'),
          'first' => t('First Name'),
          'last' => t('Last Name'),
          'total' => t('Total AFL')
        );
        $rows[] = array(
          'pap' => $wrapper->field_pap_affiliate_id->value(),
          'email' => $account->mail,
          'first' => $wrapper->field_first_name->value(),
          'last' => $wrapper->field_last_name->value(),
          'total' => rtpr_ebp_get_total($uid, 'afl'),
        );
      }
      elseif($form_state['group'] == 'coop_share'){
        $header = array(
          'PAP' => t('PAP ID'),
          'email' => t('Email'),
          'first' => t('First Name'),
          'last' => t('Last Name'),
          'total' => t('Total Co-Op')
        );
        $rows[] = array(
          'pap' => $wrapper->field_pap_affiliate_id->value(),
          'email' => $account->mail,
          'first' => $wrapper->field_first_name->value(),
          'last' => $wrapper->field_last_name->value(),
          'total' => rtpr_reward_points_available($uid, 'coop_share'),
        );
      }
    }
  
    if(!empty($rows)) {
      $form['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => t('No history available for selected date.'),
      );
      
      $form['export'] = array(
        '#type' => 'submit',
        '#value' => 'Export',
        '#submit' => array('rtpr_reward_search_export_submit'),
      );
      
      $form_state['rows'] = $rows;
      $form_state['header'] = $header;
    }
        
      

  
  return $form;
}


function rtpr_reward_search_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  $email = $values['email'];
  $account = user_load_by_mail($email);
  if (!$account) {
    form_set_error('email', t('User @email does not exist!', array('@email' => $email)));
  } else {
    $form_state['values']['uid'] = $account->uid;
  }
}


function rtpr_reward_search_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $form_state['uid'] = $values['uid'];
  $form_state['group'] = $values['group'];
// Keep submitted value
  $form_state['rebuild'] = TRUE;
 
}

// Export data
function rtpr_reward_search_export_submit($form, &$form_state) {
  $rows = $form_state['rows'];
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