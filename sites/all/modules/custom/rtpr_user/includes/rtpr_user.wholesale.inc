<?php

/**
 * @file
 */
 
/**
 * Callback for user/%user/wholesale-referrals.
 */
function rtpr_user_wholesale_referrals_form($form, &$form_state, $account) {
  $path = current_path();
  $rows = array();
  $pap_id = NULL;
  try {
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = trim($account_wrapper->field_pap_affiliate_id->value());
    db_set_active('pap');
    $referrals_pap_ids = db_query(
      "SELECT userid FROM qu_pap_users WHERE parentuserid= :parentuserid",
      array(':parentuserid' => $pap_id)
    )->fetchCol();
    db_set_active();
    if (!empty($referrals_pap_ids)) {
      $rows = array();
      $pending = 0;
      $referrals = db_query(
        "SElECT users.uid, realname, field_pap_affiliate_id_value, field_address_locality, field_address_administrative_area, mail, field_phone_value
          FROM field_data_field_pap_affiliate_id
          LEFT JOIN users ON users.uid = entity_id
          LEFT JOIN users_roles as ur ON ur.uid = users.uid
          LEFT JOIN realname ON realname.uid = users.uid
          LEFT JOIN field_data_field_address as address ON address.entity_id = users.uid
          LEFT JOIN field_data_field_phone as phone ON phone.entity_id = users.uid
          WHERE rid IN (4)
          AND field_pap_affiliate_id_value IN (:referrals_ids)",
        array(':referrals_ids' => $referrals_pap_ids)
      );
      foreach ($referrals as $referral) {
        $rows[$referral->uid] = array(
          'name' => l(
            $referral->realname,
            '/' . $path,
            array(
              'fragment' => 'wholesaleReferralsModal',
              'attributes' => array(
                'class' => 'wholesale ' . $referral->field_pap_affiliate_id_value,
                'data-pap-id' => $referral->field_pap_affiliate_id_value,
                'data-uid' => $referral->uid,
                'data-pid' => $account->uid,
                'data-name' => $referral->realname,
                'data-toggle' => 'modal'
              ),
            )
          ),

          'pap_id' => $referral->field_pap_affiliate_id_value,
          'city' => $referral->field_address_locality,
          'state' => $referral->field_address_administrative_area,
          'email' => $referral->mail,
          'phone' => $referral->field_phone_value,
          'status' => 'Pending WS',
        );
        $pending++;
      }

      $referrals_uids = db_query(
        "SELECT entity_id FROM field_data_field_pap_affiliate_id, users_roles
          WHERE rid IN (37,38, 47)
          AND uid = entity_id
          AND field_pap_affiliate_id_value IN (:referrals_ids)",
        array(':referrals_ids' => $referrals_pap_ids)
      )->fetchCol();

      $total_ba = 0;
      $total_rewards = 0;
      $total_rewards_roles = 0;
      $total_rcp = 0;
      $total_simpler = 0;
      $total_promoter = 0;
      foreach ($referrals_uids as $uid) {
        $child = user_load($uid);
        $rids = array_keys($child->roles);
        if (in_array(38, $rids)) {
          $status = 'Wholesale';
          $total_rcp += 1;
        }
        elseif (in_array(37, $rids)) {
          $status = 'Dispenser';
          $total_rcp += 1;
        }
        elseif (in_array(56, $rids) && !in_array(47, $rids)) {
          $status = 'Promoter';
          $total_promoter += 1;
        }
        else {
          if (in_array(47, $rids)) {
            $total_rewards_roles += 1;
          }
          $is_ba = in_array(50, $rids);
          $is_ba_plus = in_array(51, $rids);
          $is_ba_business = in_array(52, $rids);
          $is_simpler = in_array(37, $rids);
          if ($is_ba_business) {
            $status = 'BA';
            $total_ba += 1;
          }
          elseif ($is_ba_plus) {
            $status = 'BA';
            $total_ba += 1;
          }
          elseif ($is_ba) {
            $status = 'BA';
            $total_ba += 1;
          }
          else {
            $status = 'BA';
            $total_rewards += 1;
          }
        }
        
        
        $child_wrapper = entity_metadata_wrapper('user', $child);
        $child_pap = $child_wrapper->field_pap_affiliate_id->value();
        $phone = $child_wrapper->field_phone_number->value();
        if (!$phone) {
          $phone = $child_wrapper->field_phone->value();
        }
        $address = $child_wrapper->field_address->value();
        $rows[$uid] = array(
          'name' => l(
            $child->realname,
            '/' . $path,
            array(
              'fragment' => 'wholesaleReferralsModal',
              'attributes' => array(
                'class' => 'wholesale ' . $child_pap,
                'data-pap-id' => $child_pap,
                'data-uid' => $uid,
                'data-pid' => $account->uid,
                'data-name' => $child->realname,
                'data-toggle' => 'modal'
              ),
            )
          ),
          'id' => $child_pap,
          'city' => $address['locality'],
          'state' => $address['administrative_area'],
          'email' => $child->mail,
          'phone' => preg_replace('/[^0-9-]/', '', $phone),
          'status' => $status,
        );
        
      }
      
      $header = array(
        'name' => t('Name'),
        'id' => t('ID#'),
        'city' => t('City'),
        'state' => t('State'),
        'email' => t('Email'),
        'phone' => t('Phone #'),
        'status' => t('Status')
      );

      $mapping = array(
        'BAB' => 'ba',
        'BA+' => 'ba',
        'BA' => 'ba',
        'WRM' => 'ba',
        'Rewards' => 'rewards',
        'RCP' => 'retailers',
        'Wholesale' => 'retailers',
        'Dispenser' => 'simpler',
        'Promoter' => 'promoter',
        'Pending WS' => 'pending',
      );

      if (!empty($_GET['status'])) {
        if ($_GET['status'] != 'all') {
          foreach ($rows as $index => $row) {
            if ($mapping[$row['status']] != $_GET['status']) {
              unset($rows[$index]);
            }
          }
        }
      }
      else {
        foreach ($rows as $index => $row) {
          if ($mapping[$row['status']] != 'ba') {
            unset($rows[$index]);
          }
        }
      }

      if (!empty($_GET['download'])) {
        drupal_add_http_header('Content-Type', 'text/csv; utf-8');
        drupal_add_http_header('Content-Disposition', 'attachment;filename=wholesale-referrals.csv');
        $out = fopen('php://output', 'w');
        fputcsv($out, $header);
      
      
        foreach($rows as $row){
          $row['name'] = strip_tags($row['name']);
          fputcsv($out, $row);
        }
      
        fclose($out);
        exit();
      }
      
      $total_referrals = $total_ba + $total_rewards + $total_rcp;
      $list = array();

      $list[0]['data'] = '<strong>Total Wholesale Referrals = </strong>' . l($total_referrals + $pending, $_GET['q'], array('query' => array('status' => 'all'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));
    
      $list[0]['children'][1] = '<strong>Total Brand Ambassadors = </strong>' . l($total_rewards_roles, $_GET['q'], array('query' => array('status' => 'ba'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));
      $list[0]['children'][2] = '<strong>Total Retailers = </strong>' . l($total_rcp, $_GET['q'], array('query' => array('status' => 'retailers'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));
      $list[0]['children'][4] = '<strong>Total Pending Wholesale = </strong>' . l($pending, $_GET['q'], array('query' => array('status' => 'pending'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));
    
  
      
      $form['total'] = array(
        '#theme' => 'item_list',
        '#items' => $list,
      );
      $query = array('download' => 1);
      if(isset($_GET['status'])) {
        $query['status'] = $_GET['status'];
      }
      $form['download'] = array(
        '#prefix' => '<p>',
        '#markup' => l('Download current view', $_GET['q'], array('query' => $query, 'attributes' => array('class' => array('btn btn-primary btn-sm')))),
        '#suffix' => '</p>',
      );

      $form['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => t('No data available.'),
      );
      $form['modal'] = array(
        '#markup' => render(theme('rtpr_user_wholesale_referrals_modal', array('account' => $account)))
      );

    }
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_user',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
  
  db_set_active();
  
  if (empty($rows)) {
    $form['no_referrals'] = array(
      '#markup' => '<div class="alert alert-warning" role="alert">You have not referred anyone to the program. To refer someone to the program, all you need to do is send them to your Wholesale referral site <strong><u><a href="https://EnjoyLivingAgain.com/' . $pap_id .'">https://EnjoyLivingAgain.com/' . $pap_id .'</a></u></strong></div>'
    );
  }
  

  return $form;
}

function _rtpr_user_wholesale_referrals_table($uid) {
   try {
    $account = user_load($uid);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = trim($account_wrapper->field_pap_affiliate_id->value());
    db_set_active('pap');
    $referrals_pap_ids = db_query(
      "SELECT userid FROM qu_pap_users WHERE parentuserid= :parentuserid",
      array(':parentuserid' => $pap_id)
    )->fetchCol();
    db_set_active();
    if (!empty($referrals_pap_ids)) {
      $referrals_uids = db_query(
        "SELECT entity_id FROM field_data_field_pap_affiliate_id, users_roles
          WHERE rid IN (31, 38, 47, 50, 51, 52)
          AND uid = entity_id
          AND field_pap_affiliate_id_value IN (:referrals_ids)",
        array(':referrals_ids' => $referrals_pap_ids)
      )->fetchCol();
      $rows = array();
      foreach ($referrals_uids as $uid) {
        $child = user_load($uid);
        $rids = array_keys($child->roles);
        if (in_array(38, $rids)) {
          $status = 'RCP';
        }
        else {
          $is_ba = in_array(50, $rids);
          $is_ba_plus = in_array(51, $rids);
          $is_ba_business = in_array(52, $rids);
          if ($is_ba_business) {
            $status = 'BAB';
          }
          elseif ($is_ba_plus) {
            $status = 'BA+';
          }
          elseif ($is_ba) {
            $status = 'BA';
          }
          else {
            $status = 'Rewards';
          }
        }

        $child_wrapper = entity_metadata_wrapper('user', $child);
        $child_pap = $child_wrapper->field_pap_affiliate_id->value();
        $rows[$uid] = array(
          'name' => $child->realname,
          'id' => $child_pap,
          'status' => $status,
        );
      }
    }
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_user',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
  
  $header = array(
    'name' => t('Name'),
    'id' => t('ID#'),
    'status' => t('Status')
  );
  
  
  $form['top'] = array(
    '#markup' => '<strong>' . $account->realname . '</strong> has referred <strong>' . count($rows) . '</strong> Wholesale Accounts. These are your Sub-Affiliates'
  );
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );
  return $form;
}

/**
 *  Callback for user/%user/wholesale-referrals/%/data.
 */
function rtpr_user_wholesale_referrals_data_form($account, $uid) {

  $form = _rtpr_user_wholesale_referrals_table($uid);
  
 
  $data = array(
    'uid' => drupal_render($form),
  );
  return drupal_json_output($data);
}

/**
 * Callback for user/%user/wholesale-referrals/affiliates.
 */
function rtpr_user_wholesale_referrals_affiliates_form($account) {
  try {
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = trim($account_wrapper->field_pap_affiliate_id->value());
    db_set_active('pap');
    $referrals_pap_ids = db_query(
    "SELECT userid FROM qu_pap_users WHERE parentuserid= :parentuserid",
    array(':parentuserid' => $pap_id)
    )->fetchCol();
    db_set_active();
    if (!empty($referrals_pap_ids)) {
      $referrals = db_query(
        "SELECT entity_id, realname, mail, field_pap_affiliate_id_value
          FROM field_data_field_pap_affiliate_id, users_roles as ur, users as u, realname
          WHERE rid IN (4)
          AND ur.uid = entity_id
          AND ur.uid = u.uid
          AND u.uid = realname.uid
          AND field_pap_affiliate_id_value IN (:referrals_ids)",
        array(':referrals_ids' => $referrals_pap_ids)
      );
      $rows = array();
      foreach ($referrals as $referral) {
        $rows[] = array(
          'name' => $referral->realname,
          'email' => $referral->mail,
          'pap_id' => $referral->field_pap_affiliate_id_value,
        );
      }
      $header = array(
        'name' => t('Name'),
        'email' => t('Email'),
        'pap_id' => t('ID #')
      );
      drupal_add_http_header('Content-Type', 'text/csv; utf-8');
      drupal_add_http_header('Content-Disposition', 'attachment;filename=affiliates.csv');
      $out = fopen('php://output', 'w');
      fputcsv($out, $header);
    
      foreach($rows as $row){
        fputcsv($out, $row);
      }
    
      fclose($out);
      exit();  
    }
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_user',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
}