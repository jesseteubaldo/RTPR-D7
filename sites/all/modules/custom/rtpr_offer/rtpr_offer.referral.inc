<?php
/**
 * @file
 * Referral list.
 */

/**
 * My referral page callback.
 */
function rtpr_offer_my_referral_form($form, &$form_state, $account_uid) {
  $type = !empty($_GET['type']) ? $_GET['type'] : FALSE;
  $account_wrapper = entity_metadata_wrapper('user', $account_uid);
  try {
    $pap_id = $account_wrapper->field_pap_affiliate_id->value();

    $referrals = db_query(
      'SELECT u.uid, mail, realname, field_address_locality, field_address_administrative_area
      FROM field_data_field_pap_referral_id
      LEFT JOIN users AS u ON entity_id = u.uid
      LEFT JOIN realname AS r ON r.uid = u.uid
      LEFT JOIN field_data_field_address AS a ON u.uid = a.entity_id
      LEFT JOIN users_roles ur ON ur.uid = u.uid
      WHERE field_pap_referral_id_value = :pap_id
      AND rid IN (7,48)
      GROUP BY u.uid,mail, realname, field_address_locality, field_address_administrative_area',
      array(':pap_id' => $pap_id)
    )->fetchAll();
    
    $rows = array();
    foreach ($referrals as $referral) {
      $uid = $referral->uid;
      unset($referral->uid);
      $referral->source = 'Personal';
      $rows[$uid] = (array)$referral;
    }

    $uids = array_keys($rows);

    $gcap_uids = array();
    if (!empty($uids)) {
      $gcap_uids = db_query(
        'SELECT entity_id FROM field_data_field_gcap WHERE entity_id IN (:uids)',
        array(':uids' => $uids)
      )->fetchCol();
    }

    $order_uids = db_query(
      'SELECT distinct uid FROM commerce_order
      WHERE uid IN (:uids)
      AND status IN (:status)',
      array(
        ':uids' => $uids,
        ':status' => rtpr_offer_order_success_statuses()
      )
    )->fetchCol();
    
    $vgcc_uids = db_query(
      'SELECT DISTINCT uid FROM rtpr_gcc
      WHERE uid IN (:uids)
      AND type = :type',
      array(':uids' => $uids, ':type' => 'VGCC')
    )->fetchCol();

    foreach ($rows as $uid => $row) {
      if (in_array($uid, $vgcc_uids)) {
        $rows[$uid]['source'] = 'Virtual GC';
      }
      elseif (in_array($uid, $gcap_uids)) {
        $rows[$uid]['source'] = 'GCAP';
      }
    }


    if ($type == 'gcap') {
      foreach ($rows as $uid => $row) {
        if ($rows[$uid]['source'] != 'GCAP') {
          unset($rows[$uid]);
        }
      }
    }
    elseif ($type == 'potential') {
      foreach ($rows as $uid => $row) {
        if (in_array($uid, $order_uids)) {
          unset($rows[$uid]);
        }
      }
    }
    elseif ($type == 'purchased') {
      foreach ($rows as $uid => $row) {
        if (!in_array($uid, $order_uids)) {
          unset($rows[$uid]);
        }
      }
    }
    
    if (isset($_GET['sort']) && isset($_GET['order'])) {
      if (strtolower($_GET['order']) == 'source') {
        uasort($rows, 'rtpr_offer_referral_sortby_source');
      }
      
      if($_GET['sort'] == 'desc') {
        $rows = array_reverse($rows);
      }
    }

    $form['retail'] = array(
  	  '#markup' => 
  	    t('<p>You have <b><u>@total</u></b> Customers that have made a purchase, <a href="@link"><u>Click to View</u></a>.</p>',
    	  array(
    	    '@total' => count($order_uids),
    	    '@link' => url('/user/' . $account_uid . '/referral', array('query' => array('type' => 'purchased')))
    	  )
      ),
    );
    
    $form['potential'] = array(
  	  '#markup' => 
  	    t('<p>You have <b><u>@total</u></b> Customers that have not purchased yet, <a href="@link"><u>Click to View</u></a>.</p>',
    	  array(
    	    '@total' => count($uids) - count($order_uids),
    	    '@link' => url('/user/' . $account_uid . '/referral', array('query' => array('type' => 'potential')))
    	  )
      ),
    );
    

    $form['gcap'] = array(
  	  '#markup' => 
  	    t('<p>You have <b><u>@total</u></b> GCAP Customers, <a href="@link"><u>Click to View</u></a>.</p>',
    	  array(
    	    '@total' => count($gcap_uids),
    	    '@link' => url('/user/' . $account_uid . '/referral', array('query' => array('type' => 'gcap')))
    	  )
      ),
    );
      
    $header = array(
      array('data' => t('Email')),
      array('data' => t('Customer Name')),
      array('data' => t('Customer City')),
      array('data' => t('Customer State')),
      array('data' => t('Source'), 'field' => 'source'),
    );

    $form_state['rows'] = $rows;
    $form_state['uid'] = $account_uid;
    $per_page = 50;
    $current_page = pager_default_initialize(count($rows), $per_page);
    // split array into page sized chunks
    $chunks = array_chunk($rows, $per_page, TRUE);

    if ($type) {
      switch ($type) {
        case 'purchased':
          $viewing_text = t('Viewing Customers that have Purchased.');
          break;
        case 'potential':
          $viewing_text = t('Viewing Customers that have Not Purchased yet.');
          break;
        case 'gcap':
          $viewing_text = t('Viewing GCAP Customers.');
          break;
      }
      
      $form['download'] = array(
        '#type' => 'submit',
        '#value' => t('Download'),
        '#submit' => array('rtpr_offer_my_referral_form_download'),
      );
      
      $form['viewing'] = array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => $viewing_text
      );

      $form['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $chunks[$current_page],
        '#sticky' => TRUE,
        '#empty' => t('No transaction available for selected date.'),
      );
  
      $form['pager'] = array (
        '#theme' => 'pager',
      );
    }
    return $form;
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_offer',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
}

/**
 * My referral page callback.
 */
function rtpr_offer_my_referral_form_download($form, &$form_state, $uid) {
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=MyReferral.csv');
  $out = fopen('php://output', 'w');
  $header = array(
    'Email',
    'Customer Name',
    'Customer City',
    'Customer State',
    'Source',
  );
  $rows = $form_state['rows'];
  fputcsv($out, $header);
  foreach($rows as $row){
    fputcsv($out, $row);
  }

  fclose($out);
  exit();
}