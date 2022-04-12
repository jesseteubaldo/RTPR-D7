<?php
  
/**
 * @file
 */

function _rtpr_pap_get_teams($affiliate, $rows, &$context) {
  db_set_active('pap');
  $affiliates = db_query("SELECT * FROM qu_pap_users WHERE parentuserid=:pap_id", array(':pap_id' => $affiliate['userid']))->fetchCol();
  if (!empty($affiliates)) {
    $query = db_select('qu_pap_users','pu');
  	$query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
  	$query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
  	$query->fields('pu', array('userid', 'parentuserid', 'data4', 'data5', 'data8'))
  	->condition('pu.userid', $affiliate['userid'], '=');
  	$query->fields('ga', array('username', 'firstname', 'lastname'));
  	$parent = $query->execute()->fetchObject();
    
  	$query = db_select('qu_pap_users','pu');
  	$query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
  	$query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
  	$query->fields('pu', array('userid', 'parentuserid', 'data4', 'data5', 'data8'))
  	->condition('pu.userid', $affiliates, 'IN');
  	$query->fields('ga', array('username', 'firstname', 'lastname'));
  	$results = $query->execute()->fetchAll();

  	$emails = array();
  	foreach ($results as $affiliate) {
    	$emails[] =  $affiliate->username;
  	}
  	db_set_active();
  	$email_list = array();
  	if (!empty($emails)) {
    	$email_list = db_query("SELECT uid, mail FROM users WHERE mail IN (:emails)", array(':emails' => $emails))->fetchAllAssoc('mail');
  	}
  	
  	$affiliate_list = db_query("SELECT uid, rid FROM users_roles WHERE rid=4")->fetchAllAssoc('uid');
  	$rewards_list = db_query("SELECT uid, rid FROM users_roles WHERE rid=47")->fetchAllAssoc('uid');
    $parent_account = user_load_by_mail($parent->username);
    foreach ($results as $affiliate) {
      $email = $affiliate->username;
      $uid = isset($email_list[$email]) ? $email_list[$email]->uid : NULL;
      $row = array(
        'uid' => $uid ? $uid: 'N/A',
        'userid' => $affiliate->userid,
        'email' => $email,
        'role' => _rtpr_pap_affiliate_or_rewards($affiliate_list, $rewards_list, $uid),
        'name' => $affiliate->firstname . ' ' . $affiliate->lastname,
        'city' => $affiliate->data4,
        'state' => $affiliate->data5,
        'phone' => $affiliate->data8,
        'parent_papid' => $affiliate->parentuserid,
        'parent_name' => $parent->firstname . ' ' . $parent->lastname,
        'parent_role' => _rtpr_pap_affiliate_or_rewards($affiliate_list, $rewards_list, $parent_account->uid),
      );
      $rows[] = $row;
      $rows = _rtpr_pap_get_teams($row, $rows, $context);
    }
  }
  db_set_active();
  return $rows;
}

/**
 * Determine if uid is in the list of affiliate or rewards
 */
function _rtpr_pap_affiliate_or_rewards($affiliate_list, $rewards_list, $uid) {
  if(isset($affiliate_list[$uid])) {
    return 'Affiliate';
  }
  elseif (isset($rewards_list[$uid])) {
    return 'Rewards';
  }
  
  return 'N/A';
}

function rtpr_pap_affiliates_form($form, &$form_state) {
  $form['pap_id'] = array(
    '#type' => 'textfield',
    '#title' => t('PAP ID'),
    '#required' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#submit' => array('rtpr_pap_affiliates_form_search'),
  );
  
  if (!empty($form_state['input'])) {
    
    $form['download'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      '#submit' => array('rtpr_pap_affiliates_form_download'),
    );
    $parentuserid = $form_state['input']['pap_id'];
    $rows = _rtpr_pap_get_teams(array('userid' => $parentuserid), $rows);

    $header = array(
      'uid' => 'UID',
      'userid' => 'PAPID',
      'email' => 'Email',
      'role' => 'Role',
      'name' => 'Name',
      'city' => 'City',
      'state' => 'State',
      'phone' => 'Phone',
      'parent_papid' => 'Parent PAPID',
      'parent_name' => 'Parent Name',
      'parent_role' => 'Parent Role',
    );

    $form_state['rows'] = $rows;
    $form_state['header'] = $header;
    $form['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => array_slice($rows,0, 500),
      '#empty' => t('No data available for selected date.'),
    );


  }
    
  return $form;
}


function rtpr_pap_affiliates_form_search($form, &$form_state) {
 $form_state['rebuild'] = TRUE;
}

function rtpr_pap_affiliates_form_download($form, &$form_state) {
  $pap_id = $form_state['values']['pap_id'];
  $form_state['rebuild'] = TRUE;
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $pap_id . '_affiliates.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $form_state['header']);

  foreach($form_state['rows'] as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();  
}
