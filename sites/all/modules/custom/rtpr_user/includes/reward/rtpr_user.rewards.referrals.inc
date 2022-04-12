<?php

/**
 * @file
 * Rewards role related features.
 */

/**
 * Form callback for user/%user/rewards/redeemed-mrc.
 */
function rtpr_user_reward_redeemed_mrc_form($form, &$form_state, $account) {
  $header = array(
    'date' => t('Redeemed Date'),
    'comment' => t('Comment'),
    'points' => t('Redeemed Amount'),
  );

  $rows = array();
   $query = db_select('rtpr_reward', 'r')
  ->fields('r', array('id', 'insertdate', 'comment', 'point'))
  ->condition('uid', $account->uid, '=');
  $query->condition('type', 'Redeem');
  $query->condition('point_type', 'MRC');
  $query->orderBy('insertdate', 'DESC');

  $results = $query->execute();
  $total = 0;
  foreach($results as $result) {
    $point = abs($result->point);
    $rows[] = array(
      'data' => array(
        'date' => date('m/d/Y', $result->insertdate),
        'comment' => $result->comment,
        'points' => $point,
      ),
      'class' => array('table-row'),
    );

    $total += $point;
  }

  $rows[] = array(
    'data' => array(
      'date' =>  '',
      'comment' => 'Total',
      'points' => $total,
    ),
    'class' => array('last-row total'),
  );

  $form['summary']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );

  return $form;
}

/**
 * Form callback for user/%user/rewards/team-referrals.
 */
function rtpr_user_reward_team_referrals_form($form, &$form_state, $account) {
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $address = $account_wrapper->field_address->value();
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  $default_start_date = NULL;
  $default_end_date = NULL;

  $total_mrc_earned = rtpr_reward_points_earned($account->uid, 'mrc') + rtpr_reward_points_earned($account->uid, 'mrc', 1);
  $title = t('MRC Summary');
  $redeemed = abs(rtpr_reward_points_used($account->uid, 'mrc'));
  $redeemed_detail = '';
  if ($redeemed) {
    $redeemed_detail = t('<a href=@link>Click to View</a>', array('@link' => '/user/' . $account->uid .'/rewards/redeemed-mrc'));
  }


  $mrc_types = array('PMRC', 'SMRC', 'TMRC');
  $mrc_results =  array();
  foreach ($mrc_types as $type) {
    $mrc_results[$type] = db_query(
      "SELECT field_pap_affiliate_id_value,realname FROM rtpr_reward,field_data_field_pap_affiliate_id, realname WHERE rtpr_reward.uid = :uid and point_type = 'MRC' and point_subtype = :type AND referral_uid = entity_id AND entity_id = realname.uid",
      array(':uid' => $account->uid, ':type' => $type)
    )->fetchAllAssoc('field_pap_affiliate_id_value');
  }
  
  if (!empty($_GET['type'])) {
    $type = $_GET['type'];
    db_set_active('pap');
    $joined_date = db_select('qu_pap_userincommissiongroup', 'uc')
      ->fields('uc', array('userid', 'dateadded'))
      ->condition('userid', array_keys($mrc_results[$type]) , 'IN')
      ->execute()->fetchAllAssoc('userid');
  
    db_set_active();
    $header = array(
      'name' => t('Name'),
      'id' => t('ID#'),
      'joined' => t('Date Joined')
    );
    $rows = array();
    foreach ($mrc_results[$type] as $pap_id => $data) {
      $rows[$pap_id] = array(
        'name' => $data->realname,
        'id' => $pap_id,
        'joined' => $joined_date[$pap_id]->dateadded
      );
    }
    $form['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No data available.'),
    );
    $data = array(
      'uid' => drupal_render($form['table']),
    );
    drupal_json_output($data);
    drupal_exit();
  }
  $current_path = '/' . $_GET['q'];
  $items = array(
    'Total PMRC = ' . l(count($mrc_results['PMRC']), $current_path, array('attributes' => array('id' => 'rewards-modal', 'data-type' => 'PMRC', 'data-uid' => $account->uid, 'data-toggle' => 'modal', 'data-target' => '#rewardsModal'), 'fragment' => 'showmodal')),
    'Total SMRC = ' . l(count($mrc_results['SMRC']), $current_path, array('attributes' => array('id' => 'rewards-modal', 'data-type' => 'SMRC', 'data-uid' => $account->uid, 'data-toggle' => 'modal', 'data-target' => '#rewardsModal'), 'fragment' => 'showmodal')),
    'Total TMRC = ' . l(count($mrc_results['TMRC']), $current_path, array('attributes' => array('id' => 'rewards-modal', 'data-type' => 'TMRC', 'data-uid' => $account->uid, 'data-toggle' => 'modal', 'data-target' => '#rewardsModal'), 'fragment' => 'showmodal')),
    t('<strong>Total MRC Earned:</strong> <u>' . $total_mrc_earned . '</u>'),
    t('<strong>Total MRC Redeemed:</strong> <u>' . $redeemed . '</u> ' . $redeemed_detail),
    t('<strong>Total MRC Remaining:</strong> <u>' . rtpr_reward_points_available($account->uid, 'mrc') . '</u>'),
  );


  $form['mrc_remain'] = array(
    '#markup' => theme('item_list', array('items' => $items, 'title' => $title)),
    '#weight' => 1,
  );
  
  $form['modal'] = array(
    '#markup' => render(theme('rtpr_user_rewards_modal'))
  );
  
/*
  if (isset($_GET['start'])) {
    $default_start_date = $_GET['start']['date'];
    $default_end_date = $_GET['end']['date'];
  }
  $form['markup']= array(
    '#markup' => '<span class="date-label"><strong>Date Joined</strong></span>',
    '#weight' => 5,
  );
  $form['options'] = array(
      '#type' => 'container',
      '#attributes' => array(
          'class' => array('container-inline'),
      ),
      '#weight' => 10,
  );

  $form['options']['start']= array(
    '#type' => 'date_popup',
    '#date_year_range' => '-5:+0',
    '#date_format' => 'Y-m-d',
    '#title' => t('To'),
'#date_label_position' => 'none',
    '#default_value' => $default_start_date ? $default_start_date : NULL,
    '#required' => TRUE,
  );

  $form['options']['end']= array(
    '#type' => 'date_popup',
    '#date_year_range' => '-5:+0',
    '#date_format' => 'Y-m-d',
    '#date_label_position' => 'none',
    '#default_value' => $default_end_date ? $default_end_date : NULL,
    '#required' => TRUE,
  );

  $form['options']['search']= array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['options']['download']= array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  $default_status = array('Rewards', 'BA');
  if (isset($_GET['status'])) {
    $default_status = (array)$_GET['status'];
  }
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => array('rewards', 'ba'),
    '#options' => array(
      'Pending' => 'Pending',
      'Rewards' => 'Rewards',
      'BA' => 'BA'
    ),
    '#multiple' => TRUE,
    '#default_value' => $default_status,
    '#weight' => 11,

  );

  $header = array(
    'name' => t('Name'),
    'id' => t('ID# '),
    'email' => t('Email'),
    'phone' => t('Phone #'),
    'role' => array('data' => t('Status'), 'field' => 'role'),
    'join' => array('data' => t('Date Joined'), 'field' => 'join'),
    'pmrc' => array('data' => t('PMRC'), 'field' => 'pmrc'),
    'smrc' => array('data' => t('SMRC'), 'field' => 'smrc'),
    'tmrc' => array('data' => t('TMRC'), 'field' => 'tmrc'),
    //'coop' => array('data' => t('BA'), 'field' => 'coop'),
  );
  $rows = &drupal_static(__FUNCTION__);
  $cid = 'rtpr_user_rewards_referral_v9_' . $account->uid . time();
  $ba_count = 0;
  if (!isset($rows)) {
    $cache = cache_get($cid);
    if ($cache && $cache->expire > REQUEST_TIME && !isset($_GET['start'])) {
      $rows = $cache->data;
    }
    else {
      $affiliates = _rtpr_user_get_affiliates($pap_id, $default_start_date, $default_end_date);

      $rows = array();
      foreach ($affiliates as $affiliate) {

        $exclude = FALSE;
        $join = date('Y-m-d', strtotime($affiliate->get('dateinserted')));
        $query = db_select('users', 'u');
        $query->join('users_roles', 'ur', 'u.uid = ur.uid');
        $query->fields('ur', array('uid', 'rid'))
        ->condition('mail', $affiliate->get('username'));
        $rids = $query->execute()->fetchAllAssoc('rid');
        $uid = reset($rids)->uid;
        $role = rtpr_user_get_primary_role(array_flip(array_keys($rids)));

        $orders = commerce_order_load_multiple(array(), array('uid' => $uid, 'status' => array('pending', 'completed', 'processing', 'waiting')));
        $pmrc = 'N/A';
        $smrc = 0;
        $purchased = FALSE;
        $coop = FALSE;
        foreach ($orders as $order) {
          try{
            $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
            foreach($order_wrapper->commerce_line_items as $line_item_wrapper){
              $sku = $line_item_wrapper->line_item_label->value();
              $wrp_sku = 'STARTER-KIT';
              if (strpos($sku, $wrp_sku) !== FALSE && $order->created >= strtotime('2018-01-01')) {
                $join = date('Y-m-d', $order->created);
                $purchased = TRUE;
              }
              if (strpos($sku, 'PROMOTIONAL-CO-OP-KIT') !== FALSE) {
                $coop = TRUE;
              }
            }
          }
          catch (EntityMetadataWrapperException $exc) {
            watchdog(
              'rtpr_user',
              'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
              array(
                '%function' => __FUNCTION__,
                '@trace' => $exc->getTraceAsString()
              ),
              WATCHDOG_ERROR
            );
          }
        }

        if ($role == 'BA' || $role == 'Rewards') {
          $pmrc = _rtpr_reward_count_mrc($uid, 'PMRC');

          if (!$purchased) {
            $join = _rtpr_user_get_commission_group_data($affiliate->get('userid'));
          }

          $smrc = _rtpr_reward_count_smrc($uid);
          $smrc = $smrc ? $smrc: 0;
        }

        if ($role == 'Affiliate') {
          $role = 'Pending';

          if (!in_array('Pending', $default_status)) {
            $exclude = TRUE;
          }
        }

        if ($role == 'BA') {
          $coop = TRUE;
          $ba_count++;
        }



        if (!$exclude) {

          $tmrc = _rtpr_reward_count_mrc($uid, 'TMRC');
          $user_id = $affiliate->get('userid');
          $rows[] = array(
            'name' => substr($affiliate->get('firstname') . ' ' . $affiliate->get('lastname'), 0, 25),
            'id' => $user_id,
            'email' => $affiliate->get('username'),
            'phone' => $affiliate->get('data8'),
            'role' => $role,
            'join' => $join,
            'pmrc' => $pmrc,
            'smrc' => $smrc,
            'tmrc' => $tmrc ? $tmrc : 0,

            //'coop' => $coop ? 'YES' : 'NO',
          );

          $address_rows[$user_id] = array(
            'city' => $affiliate->get('data4'),
            'state' => $affiliate->get('data5'),
          );

        }
      }
      if (!isset($_GET['start'])) {
        cache_set($cid, $rows, 'cache', time() + 60*60);
      }

    }
  }

  uasort($rows, "_rtpr_user_reward_sort");

  foreach ($rows as $index => $row) {
    if (!in_array($row['role'], (array)$default_status)) {
      unset($rows[$index]);
    }
  }
  $download = FALSE;
  if (isset($_GET['op']) && $_GET['op'] == 'Download') {
    $download = TRUE;
  }

  if ($download) {

    $header = array(
      'name' => t('Name'),
      'id' => t('ID# '),
      'email' => t('Email'),
      'phone' => t('Phone #'),
      'role' => t('Status'),
      'join' => t('Date Joined'),
      'pmrc' => t('PMRC'),
      'smrc' => t('SMRC'),
      'tmrc' => t('TMRC'),
      'city' => t('City'),
      'state' => t('State'),
    );
    drupal_add_http_header('Content-Type', 'text/csv; utf-8');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=customer-team-referrals.csv');
    $out = fopen('php://output', 'w');
    fputcsv($out, $header);

    foreach($rows as $row){
      $row = $row + $address_rows[$row['id']];

      fputcsv($out, $row);
    }

    fclose($out);
    exit();
  }

  $per_page = 50;
  $current_page = pager_default_initialize(count($rows), $per_page);
  // split array into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $chunks[$current_page],
    '#empty' => t('No affiliate data available.'),
    '#weight' => 20,
  );
  if ($ba_count) {
    $form['ba_count'] = array(
      '#markup' => '<div><strong>You have <span class="underline">' . $ba_count . '</span> Personally Referred Brand Ambassadors</strong></div>',
      '#weight' => 0,
    );
  }

  $form['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 25,
  );
  $form['#method'] = 'get';
*/
  return $form;
}

function _rtpr_user_reward_sort($a, $b){
  if (isset($_GET['sort'])) {
    $sort_by = 'join';
    switch ($_GET['order']) {
      case 'PMRC':
        $sort_by = 'pmrc';
        break;
      case 'SMRC':
        $sort_by = 'smrc';
        break;
      case 'Co-Op':
        $sort_by = 'coop';
        break;
      case 'Status':
        $sort_by = 'role';
        break;
      default:
        $sort_by = 'join';
    }

    if (is_numeric($a[$sort_by])) {
      $a[$sort_by]++;
    }
    if (is_numeric($b[$sort_by])) {
      $b[$sort_by]++;
    }


    if ($a[$sort_by] == $b[$sort_by]) {
      return 0;
    }
    if ($_GET['sort'] == 'asc') {
      return ($a[$sort_by] < $b[$sort_by]) ? -1 : 1;

    }
    else {
      return ($a[$sort_by] < $b[$sort_by]) ? 1 : -1;
    }
  }
}

function _rtpr_user_get_commission_group_data($userid) {
  // load PAP library
  db_set_active('pap');
  $joined_date = db_select('qu_pap_userincommissiongroup', 'uc')
    ->fields('uc', array('dateadded'))
    ->condition('userid', $userid)
    ->condition('commissiongroupid', 'ebd943a1')
    ->execute()->fetchField();

  db_set_active();
  if ($joined_date) {
    return date('Y-m-d', strtotime($joined_date));
  }
}

function _rtpr_user_get_affiliates($pap_id, $default_start_date = NULL, $default_end_date = NULL) {
  // load PAP library
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  $request = new Pap_Api_AffiliatesGrid($session);

  $request->addFilter('parentuserid', Gpf_Data_Filter::EQUALS, trim($pap_id));

 // $request->setSorting('dateinserted', false);
  // sets limit to 10000 rows.
  $request->setLimit(0, 10000);

  // sets columns, use it only if you want retrieve other as default columns
  $request->addParam('columns', new Gpf_Rpc_Array(array(array('id'), array('refid'), array('userid'),
  array('username'), array('firstname'), array('lastname'), array('rstatus'), array('parentuserid'),
  array('dateinserted'), array('data8'), array('data4'),array('data5'))));

  // send request
  try {
    $request->sendNow();
  } catch(Exception $e) {
    die("API call error: ".$e->getMessage());
  }

  // request was successful, get the grid result
  $grid = $request->getGrid();

  // get recordset from the grid
  $recordset = $grid->getRecordset();

  return $recordset;
}

function rtpr_user_reward_team_referrals_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 *
 */
function rtpr_user_reward_customer_referrals_form($form, &$form_state, $account) {
  drupal_goto('user/' . $account->uid . '/referral');
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  $default_start_date = NULL;
  $default_end_date = NULL;
  $status = 'all';

  if (isset($_GET['status'])) {
    $status = $_GET['status'];
  }

  if (isset($_GET['start'])) {
    $default_start_date = $_GET['start']['date'];
    $default_end_date = $_GET['end']['date'];
  }
  $form['markup']= array(
    '#markup' => '<span class="date-label"><strong>Date Joined</strong></span>',
  );
  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array(
        'class' => array('container-inline'),
    ),
  );
  $form['options']['start']= array(
    '#type' => 'date_popup',
    '#date_year_range' => '-5:+0',
    '#date_format' => 'Y-m-d',
    '#title' => t('To'),
    '#date_label_position' => 'none',
    '#default_value' => $default_start_date ? $default_start_date : NULL,
  );

  $form['options']['end']= array(
    '#type' => 'date_popup',
    '#date_year_range' => '-5:+0',
    '#date_format' => 'Y-m-d',
    '#date_label_position' => 'none',
    '#default_value' => $default_end_date ? $default_end_date : NULL,
    '#required' => TRUE,
  );


  $form['options']['status']= array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      'all' => t('All'),
      'Pending' => t('Pending'),
      'NO' => t('NO'),
      'YES' => t('YES'),
    ),
    '#default_value' => $status,
  );

  $form['options']['search']= array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['options']['download']= array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  $download = FALSE;
  if (isset($_GET['op']) && $_GET['op'] == 'Download') {
    $download = TRUE;
  }
  $query = db_select('field_data_field_pap_referral_id', 'p');
  $query->join('users', 'u', 'u.uid = p.entity_id');
  $query->fields('p', array('entity_id'))
    ->condition('field_pap_referral_id_value', $pap_id)
    ->orderBy('created', 'DESC');
  if ($status == 'all' && !$download) {
    $query = $query->extend('PagerDefault')->limit(50);
  }

  $uids = $query->execute()->fetchCol();

  $accounts = user_load_multiple($uids);
  $header = array(
    array('data' => t('Name')),
    array('data' => t('Email')),
    array('data' => t('Phone #')),
    array('data' => t('Status')),
    array('data' => t('Date'), 'field' => 'source'),
    array('data' => t('Has Purchased')),
  );

  $rows = array();
  foreach ($accounts as $user_account) {

    $join = date('Y-m-d', $user_account->created);
    $wrapper = entity_metadata_wrapper('user', $user_account);
    $purchased = 'NO';

    $orders = commerce_order_load_multiple(array(), array('uid' => $user_account->uid, 'status' => array('pending', 'completed', 'processing', 'waiting')));
    if ($orders) {
      $purchased = 'YES';
      $order = reset($orders);
      $join = date('Y-m-d', $order->created);
    }
    $role = rtpr_user_get_primary_role($user_account->roles);

    if ($role == 'Potential Customer' || $role == 'Retail' || $role == 'Retail Rewards') {
      if ($role == 'Potential Customer') {
        $purchased = 'Pending';
        $role = 'GCS';
      }

      if ($status != 'all' && $purchased != $status) {
        continue;
      }

      if ($status = _rtpr_commerce_has_buck_order_and_others($user_account->uid)) {
        if ($status === 'BO') {
          $role = 'Buck Offer';
        }
      }
      $rows[] = array(
        'name' => substr($wrapper->field_first_name->value() . ' ' . $wrapper->field_last_name->value(),0, 25),
        'email' => $user_account->mail,
        'phone' => $wrapper->field_phone->value(),
        'role' => $role,
        'join' => $join,
        'purchased' => $purchased,
      );
      }
  }

  if (isset($_GET['sort'])) {
    if ($_GET['sort'] == 'asc') {
      usort($rows, function($a, $b) {
        return strtotime($a['join']) > strtotime($b['join']);
      });
    }
    else {
      usort($rows, function($a, $b) {
        return strtotime($a['join']) < strtotime($b['join']);
      });
    }
  }

  $clone_rows = $rows;
  foreach($clone_rows as $key => $row) {
    if (strtotime($row['join']) < strtotime($default_start_date)) {
      unset($rows[$key]);
    }
  }
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No affiliate data available.'),
  );


  $form_state['rows'] = $rows;
  $form_state['header'] = $header;
  $form['pager'] = array(
    '#theme' => 'pager',
  );

  if ($download) {
    $header = array(
      t('Name'),
      t('Email'),
      t('Phone #'),
      t('Status'),
      t('Date'),
      t('Has Purchased'),
    );
    drupal_add_http_header('Content-Type', 'text/csv; utf-8');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=customer-referrals.csv');
    $out = fopen('php://output', 'w');
    fputcsv($out, $header);

    foreach($rows as $row){
      fputcsv($out, $row);
    }

    fclose($out);
    exit();
  }

  $form['#method'] = 'get';
  return $form;
}

function rtpr_user_reward_customer_referrals_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
