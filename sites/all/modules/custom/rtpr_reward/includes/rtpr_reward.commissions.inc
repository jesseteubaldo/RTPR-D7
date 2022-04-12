<?php

/**
 * Get PAP commissions.
 */
function rtpr_reward_user_commissions_form($form, $form_state, $account) {
  $rows = array();
  try{
    $user_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $user_wrapper->field_pap_affiliate_id->value();
       // load PAP library
    $path = libraries_get_path('pap');
    require_once ($path . '/PapApi.class.php');
    module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
    $session = commerce_post_affiliate_pro_get_session();
  
    // get recordset of list of transactions
    $request = new Pap_Api_TransactionsGrid($session);
    $request->addFilter('dateinserted', 'D>=', date('Y-m-d', strtotime('2020-04-30')));
    $request->addFilter('userid', Gpf_Data_Filter::EQUALS, $pap_id);
    $request->addFilter('commission', '>', 0);

    $request->setLimit(0, 30000);
    $request->sendNow();
    $grid      = $request->getGrid();
    $recordset = $grid->getRecordset();
    $paid_commissions = 0;
    $unpaid_commissions = 0;
    // iterate through the records
    foreach ($recordset as $rec) {
      $status = $rec->get('payoutstatus');
      $type = 'N/A';
      $campaign_id = $rec->get('campaignid');

      if (in_array($campaign_id, array('11111111', '39e673de', 'e12d3426', '97c267cb'))) {
        $type = 'Retail';
      }
      elseif (in_array($campaign_id, array('c5d380ba', 'c602cb08', '32459dc8'))) {
        $type = 'Wholesale';
      }
      elseif ($campaign_id == '33fadfd9') {
        $type = 'RCP';
      }
      elseif ($campaign_id == 'f001dcb8') {
        $type = 'Bonus';
      }
      $commission = $rec->get('commission');
      if ($status == 'P') {
        $status = 'Paid';
        $paid_commissions += $commission;
      }
      else {
        $status = 'Unpaid';
        $unpaid_commissions += $commission;
      }
      $rows[] = array(
        'date' => $rec->get('dateinserted'),
        'amount' => '$' . number_format($commission, 2),
        'status' => $status,
        'type' => $type,
        'tier' => $rec->get('tier') > 1 ? 'Sub-Affiliate' : 'Personal',
        'name' => $rec->get('data1'),
        'product' => $rec->get('productid')
      );
    }
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_extras',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
  
  $total_commission = number_format($paid_commissions + $unpaid_commissions, 2);
  $paid_commissions = number_format($paid_commissions, 2);
  $unpaid_commissions = number_format($unpaid_commissions, 2);
  $list = array();
  $list[0]['data'] = '<strong>All Commissionable Transactions = </strong>' . l('$'.$total_commission, $_GET['q'], array('query' => array('status' => 'all'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));

  $list[0]['children'][0] = '<strong>Total Paid Transactions = </strong>' . l('$'.$paid_commissions, $_GET['q'], array('query' => array('status' => 'paid'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));

  $list[0]['children'][1] = '<strong>Total UnPaid Transactions = </strong>' . l('$'.$unpaid_commissions, $_GET['q'], array('query' => array('status' => 'unpaid'), 'attributes' => array('class' => array('underline')), 'language' => LANGUAGE_NONE));

  $form['summary'] = array(
    '#theme' => 'item_list',
    '#items' => $list,
  );
  $header = array(
    'date' => array('data' => t('Date'), 'field' => 'date'),
    'amount' => array('data' => t('Commission'), 'field' => 'amount'),
    'status' => array('data' => t('Status')),
    'type' => array('data' => t('Type'), 'field' => 'type'),
    'tier' => array('data' => t('Tier')),
    'name' => array('data' => t('Customer Name'), 'field' => 'name'),
    'product' => array('data' => t('Product')),
  );
  
  if (!empty($_GET['status'])) {
    if ($_GET['status'] != 'all') {
      foreach ($rows as $index => $row) {
        if ($row['status'] !=  ucfirst($_GET['status'])) {
          unset($rows[$index]);
        }
      }
    }
  }
  else {
    foreach ($rows as $index => $row) {
      if ($row['status'] !=  'Unpaid') {
        unset($rows[$index]);
      }
    }
  }
  

  $sort_mapping= array(
    'Date' => 'date',
    'Commission' => 'amount',
    'Type' => 'type',
    'Customer Name' => 'name',
  );

  if (isset($_GET['order']) && !empty($sort_mapping[$_GET['order']])) {
    $index = $sort_mapping[$_GET['order']];
    if ($_GET['sort'] == 'asc') {
      usort($rows, function($a, $b) {
        $sort_mapping= array(
          'Date' => 'date',
          'Commission' => 'amount',
          'Type' => 'type',
          'Customer Name' => 'name',
        );
        $index = $sort_mapping[$_GET['order']];
        
        if ($index == 'amount') {
          $a_amount = str_replace('$', '', $a[$index]);
          $b_amount = str_replace('$', '', $b[$index]);
          return $a_amount > $b_amount;
        }

        return strcmp($a[$index],$b[$index]);
      });
    }
    else {
      usort($rows, function($a, $b) {
        $sort_mapping= array(
          'Date' => 'date',
          'Commission' => 'amount',
          'Type' => 'type',
          'Customer Name' => 'name',
        );
        $index = $sort_mapping[$_GET['order']];
        if ($index == 'amount') {
          $a_amount = str_replace('$', '', $a[$index]);
          $b_amount = str_replace('$', '', $b[$index]);
          return $a_amount < $b_amount;
        }
        return strcmp($b[$index], $a[$index]);
      });
    }
  }
  
  $form['commissions'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );

  return $form;
}