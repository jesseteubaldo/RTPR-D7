<?php

/**
 * @file
 * Features for premium report.
 */

/**
 * Form callback for admin/config/rtpr/cc/premium-report.
 */
function rtpr_cc_premium_report_form($form, &$form_state) {
  $form['options'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
  );

  $form['options']['year'] = array(
      '#type' => 'select',
      '#title' => t('Year'),
      '#options' => rtpr_reward_year_list(FALSE, 2021),
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
  $form['options']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#options' => array(
      '47' => t('Rewards'),
      '44' => t('Diamond'),
      '57' => t('Premium'),
      '58' => t('Premium Trial'),
      '59' => t('Premium Plus'),
      '60' => t('Diamond Select'),
      '61' => t('Diamond Elite'),
    ),
    '#multiple' => TRUE,
    '#required' => TRUE,
    '#default_value' => array(44),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('rtpr_cc_premium_report_form_view'),
  );

  $rows = array();
  if (!empty($form_state['rows'])) {
    $form['download'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
      '#submit' => array('rtpr_cc_premium_report_form_download'),
    );
    $form['table'] = array(
      '#theme' => 'table',
      '#header' => $form_state['header'],
      '#rows' => $form_state['rows'],
      '#empty' => t('No data available.'),
    );

  }


  return $form;
}


/**
 * Custom submit handler.
 */
function rtpr_cc_premium_report_form_download($form, &$form_state) {
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=premium-report.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $form_state['header']);
  
  foreach($form_state['rows'] as $row){
    fputcsv($out, $row);
  }
  
  fclose($out);
  exit();
}

/**
 * Custom submit handler.
 */
function rtpr_cc_premium_report_form_view($form, &$form_state) {
  $values = $form_state['values'];
  $year = $values['year'];
  $month = $values['month'];
  
  $current_month = date('m');
  $current_year = date('Y');
  $rids = $values['roles'];
  
  $start_date = strtotime($year . '-' . $month . '-01');
  $end_date = strtotime('last day of ' . $year . '-' . $month) + 82800;

  $rows = array();
  $uids = db_query(
    'SELECT uid FROM users_roles WHERE rid IN (:rids)',
    array(':rids' => array_keys($rids))
  )->fetchCol();
  
  if (!empty($uids)) {
    $accounts = db_query(
      'SELECT uid,mail,field_pap_affiliate_id_value FROM users
      LEFT JOIN field_data_field_pap_affiliate_id ON entity_id = uid
      WHERE uid IN (:uids)',
      array(':uids' => $uids)
    )->fetchAll();

    if (!empty($accounts)) {
      $start_kits = array();
      
      $names = db_query(
        'SELECT uid, realname FROM realname WHERE uid in (:uids)',
        array(':uids' => $uids)
      )->fetchAllAssoc('uid');
      
      $orders = db_query(
        'SELECT order_id,uid FROM commerce_order
        WHERE uid IN (:uids)
        AND status IN (:status)',
        array(
          ':uids' => $uids,
          ':status' => array('processing','completed','waiting','pending','checkout_complete'),
        )
      )->fetchAllAssoc('order_id');
      

      $created = db_query(
        "SELECT p.order_id, p.uid, p.created FROM commerce_payment_transaction AS p
        LEFT JOIN commerce_order AS o ON p.order_id = o.order_id
        LEFT JOIN commerce_line_item AS item ON item.order_id = p.order_id
        WHERE p.order_id IN (:order_ids) AND p.status = 'success' and o.status IN ('processing','completed','waiting','pending','checkout_complete') and line_item_label LIKE '%STARTER-KIT%'",
        array(':order_ids' => array_keys($orders))
      )->fetchAll();

      foreach ($created as $start_kit) {
        $uid = $orders[$start_kit->order_id]->uid;
        if (!isset($start_kits[$uid]) && $start_kit->created > strtotime('01/01/2021')) {
          $start_kits[$uid] = $start_kit->created;
        }
      }
      $previous_month_timestamp = strtotime('-1 month', strtotime($year . '-' . $month . '-15'));
      $previous_month = date('m', $previous_month_timestamp);
      $previous_year = date('Y', $previous_month_timestamp);
      
      $gcaps = array();
      $gcap = db_query(
        "SELECT  distinct line_item_id,p.order_id, quantity FROM commerce_payment_transaction AS p
        LEFT JOIN commerce_order AS o ON p.order_id = o.order_id
        LEFT JOIN commerce_line_item AS item ON item.order_id = p.order_id
        WHERE p.order_id IN (:order_ids) 
        AND p.status = 'success' and o.status IN ('processing','completed','waiting','pending','checkout_complete')
        AND line_item_label LIKE '%GCAP%'
        AND p.created > :start_date AND p.created < :end_date
        ",
        array(
          ':order_ids' => array_keys($orders),
          ':start_date' => $start_date,
          ':end_date' => $end_date,
        )
      )->fetchAll();
      foreach ($gcap as $data) {
        $uid = $orders[$data->order_id]->uid;
        $gcaps[$uid] += $data->quantity;
      }

      foreach ($accounts as $data) {
        $uid = $data->uid;
        $premium_rid = _rtpr_user_has_rids($uid, array(44,57,59,60,61));
        $rows[$uid] = array(
          'uid' => $uid,
          'mail' => $data->mail,
          'name' => $names[$uid]->realname,
          'pap_id' => $data->field_pap_affiliate_id_value,
          'start_kit' => isset($start_kits[$uid]) ? date('m/d/Y', $start_kits[$uid]) : 'Pre21',
          'current_msp' => _rtpr_user_get_monthly_pmsp($uid, $current_month, $current_year),
          'msp' => _rtpr_user_get_monthly_pmsp($uid, $month, $year),
          'previous_msp' => _rtpr_user_get_monthly_pmsp($uid, $previous_month, $previous_year),
          'trial' => _rtpr_user_has_rids($uid, array(58)) ? 'YES' : 'NO',
          'status' => $premium_rid? user_role_load($premium_rid)->name : 'N/A',
          'gcaps' => isset($gcaps[$uid]) ? $gcaps[$uid] : 0,
        );
      }
    }
  }

  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'name' => t('Name'),
    'pap_id' => t('PAP ID'),
    'start_kit' => t('Start Kit'),
    'current_msp' => $current_month . '/' . $current_year,
    'msp' => $month . '/' . $year,
    'previous_msp' => $previous_month . '/' . $previous_year,
    'trial' => t('Premium Trial'),
    'status' => t('Premium Status'),
    'gcaps' => $month . '/' . $year . ' GCAPS',
  );
  
  $form_state['rows'] = $rows;
  $form_state['header'] = $header;
  $form_state['rebuild'] = TRUE;
}