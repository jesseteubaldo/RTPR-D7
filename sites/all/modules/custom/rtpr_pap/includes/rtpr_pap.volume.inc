<?php
/**
 * @file
 */

function rtpr_pap_volume_team_form($form, &$form_state, $uid) {
  $form['#attached']['js'][] = drupal_get_path('module', 'rtpr_pap') . '/includes/js/rtpr_pap.volume.js';
  $filter_by_role = FALSE;
  $form['options'] = array(
    '#type' => 'fieldset',
    '#attributes' => array(
        'class' => array('container-inline'),
    ),
  );
  $form['options']['role'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => 'Any',
      4 => 'Affiliate',
      8 => 'Vendor',
      9 => 'Gold',
      10 => 'HCP',
      11 => 'Merchant',
      22 => 'Platinum Winners Circle',
      37 => 'Sampler',
      38 => 'RCP',
      39 => 'Veterinarian',
      47 => 'REWARDS',
    ),
    '#default_value' => isset($_GET['rid']) ? $_GET['rid'] : 0,
    '#title' => t('Role'),
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('rtpr_pap_volume_team_form_filter_submit'),
  );
  
  if (isset($_GET['rid'])) {
    $filter_by_role = TRUE;
 
    $form['options']['download'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      '#submit' => array('rtpr_pap_volume_team_form_download_submit'),
    );
  }
  
  module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
  $parent = FALSE;
  if (isset($_GET['id'])) {
    $parent = $uid;
    $uid = $_GET['id'] - _rtpr_pap_get_session_as_number();
  }
  $month = date('m');
  $year = date('Y');
  
  if (isset($_GET['month'])) {
    $month = $_GET['month'];
  }
  if (isset($_GET['year'])) {
    $year = $_GET['year'];
  }
  $account = user_load($uid);
  if ($account) {
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $user_id = $account_wrapper->field_pap_affiliate_id->value();
    module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
    if ($user_id) {
      db_set_active('pap');
      $query = db_select('qu_pap_users', 'pu');
      $query->fields('pu', array('userid'));
      $query->condition('pu.parentuserid', $user_id, '=');
      $user_ids = $query->execute()->fetchCol();
      db_set_active();
  
      if ($user_ids) {
        $query = db_select('field_data_field_pap_affiliate_id', 'pai');
        $query->join('field_data_field_first_name', 'first', 'first.entity_id = pai.entity_id ');
        $query->join('field_data_field_last_name', 'last', 'last.entity_id = pai.entity_id ');
        $query->join('users_roles', 'ur', 'ur.uid = pai.entity_id');
        $query->fields('ur', array('uid', 'rid'))
          ->fields('pai', array('field_pap_affiliate_id_value'))
          ->fields('first', array('field_first_name_value'))
          ->fields('last', array('field_last_name_value'))
          ->condition('field_pap_affiliate_id_value', $user_ids, 'IN');
        if (!$parent && empty($_GET['rid'])) {
          $query->condition('rid', array(4), 'NOT IN');
        }
        
        if ($filter_by_role && !empty($_GET['rid'])) {
          $query->condition('rid', $_GET['rid']);
        }
        $results = $query->execute();
  
        $roles = _rtpr_pap_get_primary_roles_list();
        $roles_rids = array_keys($roles);

        $header = array(
          'index' => t('#'),
          'name' => t('Name'),
          'apv' => t('Total APV'),
          'role' => t('Primary Role'),
          'vendor' => t('Vendor-Owner'),
          'diamond' => t('Diamond Club'),
          'detail' => t('Detail'),
        );
  
        $rows = array();
        $count_rows = array();
       
        $session_number = _rtpr_pap_get_session_as_number();
        foreach($results as $index => $row) {
          $user_data = new PAPUserData($row->field_pap_affiliate_id_value, $month, $year);
          $count_rows[$row->uid] = $row->uid;
          $rows[$row->uid]['index'] = count($count_rows);
          $rows[$row->uid]['name'] = substr($row->field_first_name_value . ' ' . $row->field_last_name_value, 0, 20);
          $rows[$row->uid]['apv'] = $user_data->getTotalAPV() * 100;
          
          if (!isset($rows[$row->uid]['role'])){
            $rows[$row->uid]['role'] = FALSE;
          }
          
          // Get primary role.
          if (!$rows[$row->uid]['role'] && in_array($row->rid, $roles_rids)) {
            $rows[$row->uid]['role'] = ucfirst($roles[$row->rid]);
          }

          // Check if user has vendor-owner (36) or Vendor-Owner20 (42) role.
          if (in_array($row->rid, array(36, 42))) {
            $rows[$row->uid]['vendor'] = 'Yes';
          }
          
          if(!isset($rows[$row->uid]['vendor'])) {
            $rows[$row->uid]['vendor'] = 'No';
          }
          
          // Check if user has Diamond Club (40) role.
          if ($row->rid == 40) {
            $rows[$row->uid]['diamond'] = 'Yes';
          }
          
          if(!isset($rows[$row->uid]['diamond'])) {
            $rows[$row->uid]['diamond'] = 'No';
          }
          
          // Detail
          $rows[$row->uid]['detail'] = l(t('View detail'),
              '',
              array(
                'fragment' => ' ',
                'external' => TRUE,
                'attributes' => array(
                  'class' => 'volume-detail',
                  'uid' => $session_number + $row->uid,
                  'month' => $month,
                  'year' => $year,
                  'data-toggle' => "modal",
                  'data-target' => "#myModal",
                  'tier' => isset($_GET['tier']) ? $_GET['tier'] : 1,
                )
              )
            );

        }
        foreach ($rows as $key => $row) {
          $apv[$key]  = $row['apv'];
          $rows[$key]['apv'] = commerce_currency_format($row['apv'], 'USD');
        }

        array_multisort($apv, SORT_DESC, $rows);

        foreach ($rows as $key => $row) {
          $rows[$key]['index'] = $key + 1;
        }
        
        $team_name = $account_wrapper->field_first_name->value() . ' ' . $account_wrapper->field_last_name->value();
        $form['message'] = array(
          '#markup' => '<h4>You are viewing the team of <b>' . $team_name . '</b> for ' . $month . '/' . $year .'</h4>',
        );

        $parent_link = '';
        if (isset($_GET['tier'])) {
          $parent_link = '<b><a href="#" onClick="history.go(-1);return true;">Click Here to view the Team of their Sponsor Or </a></b>';
        }
        $form['goup'] = array(
          '#markup' => '<h4>' . $parent_link . ' Select a Team Member you would like to view from the list below.</h4>',
        );

        $previous_month = strtotime('-1 month', strtotime($month . '/01/' . $year));
        $next_month = strtotime('+1 month', strtotime($month . '/01/' . $year));
        $query = '';
        if (isset($_GET['id'])) {
          $query = '&id=' . $_GET['id'] . '&tier=' . $_GET['tier'];
        }
        $form['prev'] = array(
          '#markup' => '<div class="pull-left"><a href="team?month=' . date('m', $previous_month) .'&year=' . date('Y', $previous_month) . $query .'">Previous Month</a></div>',
        );
        $form['next'] = array(
          '#markup' => '<div class="pull-right"><a href="team?month=' . date('m', $next_month) .'&year=' . date('Y', $next_month) . $query .'">Next Month</a></div>',
        );

        $per_page = 100;
        $current_page = pager_default_initialize(count($rows), $per_page);
        // split array into page sized chunks
        $chunks = array_chunk($rows, $per_page, TRUE);

        $form['table'] = array(
          '#theme' => 'table',
          '#header' => $header,
          '#rows' => $chunks[$current_page],
          '#empty' => t('No data available for this user.'),
        );
        $form['pager'] = array (
          '#theme' => 'pager',
        );

        $form['modal'] = array(
          '#markup' => '<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"><div class="modal-dialog" role="document"><div class="modal-content"><div class="modal-header"><button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button><h4 class="modal-title" id="myModalLabel">Loading</h4></div><div class="modal-body"><div class="progress"><div class="progress-bar progress-bar-striped active" role="progressbar" aria-valuenow="10" aria-valuemin="0" aria-valuemax="100" style="width: 10%"></div></div></div></div></div></div>',
        );
        
        $form_state['header'] = $header;
        $form_state['rows'] = $rows;
      }
    }
  } else {
    drupal_access_denied();
    module_invoke_all('exit');
    drupal_exit();
  }
  return $form;
}

function rtpr_pap_volume_team_form_filter_submit($form, &$form_state) {
  $form_state['redirect'] = array(
    current_path(),
    array(
      'query' => array(
        'rid' => $form_state['values']['role'],
      ),
    ),
  );
}

function rtpr_pap_volume_team_form_download_submit($form, &$form_state) {
  $header = $form_state['header'];
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=export.csv');
  $out = fopen('php://output', 'w');
  unset($header['detail']);
  fputcsv($out, $header);

  foreach($form_state['rows'] as $row){
    unset($row['detail']);
    fputcsv($out, $row);
  }

  fclose($out);
  exit(); 
}
/**
 * Helper function to get primary role mapping.
 */
function _rtpr_pap_get_primary_roles_list() {
  $query = db_select('role', 'r')
    ->fields('r', array('rid', 'name'))
    ->condition('rid', array(4, 8, 9, 10, 11, 22, 37, 38, 39, 47), 'IN');

  $results = $query->execute()->fetchAllAssoc('rid');
  $roles = array();

  foreach ($results as $rid => $row) {
    $name = $row->name;
    if (strpos($row->name, ' ') !== FALSE) {
      $name = strstr($row->name, ' ', TRUE);
    }
    $roles[$rid] = $name;
  }

  return $roles;
}

function _rtpr_pap_get_session_as_number() {
 
  return substr(filter_var($_COOKIE['PAPVisitorId'], FILTER_SANITIZE_NUMBER_INT),0,6);
}
function _rtpr_pap_volume_detail_ajax($user, $tier) {
  if (isset($_GET['uid'])) {
  $uid = $_GET['uid'] - _rtpr_pap_get_session_as_number();
  module_load_include('inc','rtpr_pap','templates/PAPUserData.class');
  $account = user_load($uid);
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  $user_data = new PAPUserData($pap_id, $_GET['month'], $_GET['year'], $account, $account_wrapper);
  $data = $user_data->getCustomerReferralData();
  $retail = $user_data->getMonthlyRetailData($tier);
  $address = $account_wrapper->field_address->value();
  $last_login = $account->login ? date('m/d/Y H:i:s', $account->login) : 'Never';
  $full_name = $account_wrapper->field_first_name->value() . ' '  . $account_wrapper->field_last_name->value();
 
  // Only show contact info for personal referr.
  if ($tier == 1) {
    $mail =  "<tr><th>Email</th><td>$account->mail</td></tr>";
    $phone = "<tr><th>Phone</th><td>{$account_wrapper->field_phone->value()}</td></tr>";
  }
  $referral_url = 'team?id=' . $_GET['uid'] . '&tier=' . ++$tier;
  if (isset($_GET['month'])) {
    $referral_url .= '&month=' . $_GET['month'] . '&year=' . $_GET['year'];
  }
  $modal = '
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="myModalLabel">' . $full_name . '</h4>
      </div>
      <div class="modal-body"><table class="table table-condensed">
        <tr>
          <th>Online ID</th>
          <td>' .$pap_id . '</td>
        </tr>
        <tr>
          <th>State</th>
          <td>' . $address['administrative_area'] . '</td>
        </tr>'. $mail . $phone .'
          <th>Total MSP</th>
          <td>' . commerce_currency_format($user_data->getVolumeTotal('msp') * 100, 'USD') . '</td>
        </tr>
        <tr>
          <th>Total APV</th>
         <td>' . commerce_currency_format($user_data->getTotalAPV() * 100, 'USD') . '</td>
        </tr>
        <tr>
          <th>Total Number of Personal Wholesale Referrals</th>
          <td><a href="' .$referral_url . '">' . $user_data->getTotalReferral() . '</td>
        </tr> 
        <tr>
          <th>Total New Personal Wholesale Referrals this month</th>
          <td>' . $user_data->getNewMonthlyReferralTotal() . '</td>
        </tr>
        <tr>
          <th>Total All Time Purchasing Retail Customers</th>
          <td>' .  $data['purchase'] . '</td>
        </tr>
        <tr>
          <th>Total Purchasing Retail Customers this Month</th>
          <td>' . $retail['count'] . '</td>
        </tr> 
        <tr>
          <th>Total New Purchasing Retail Customers this month</th>
          <td>' . $retail['new'] . '</td>
        </tr>
        <tr>
          <th>Total Rebate Redemptions this month</th>
          <td>' . $retail['redemption'] . '</td>
        </tr>

      '.'</table></div>
      <div class="modal-footer">
        <div class="text-left pull-left"><b>Last Login:</b> ' . $last_login . '</div>
        <a class="btn btn-primary" href="' . $referral_url .'">View Referrals</a>
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div>
  </div>';
  } else {
    $modal = '<div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="myModalLabel">Loading</h4>
      </div><div class="modal-footer">
      </div>
    </div>
  </div>';
  }

return $modal;
}

function _rtpr_pap_volume_detail_callback($page_callback_result) {
  print $page_callback_result;
}
