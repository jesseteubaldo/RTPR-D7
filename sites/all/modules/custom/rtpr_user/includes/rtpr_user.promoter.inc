<?php

/**
 * @file
 */
 
/**
 * Callback for user/%user/promoter-referrals.
 */
function rtpr_user_promoter_referrals_form($form, &$form_state, $account) {
  $header = array(
    'name' => t('Name'),
    'id' => t('ID#'),
    'city' => t('City'),
    'state' => t('State'),
    'email' => t('Email'),
    'phone' => t('Phone #'),
    'status' => t('Status'),
    'date' => t('Join Date'),
  );
  $rows = _rtpr_user_promoter_referrals_rows($account->uid, TRUE);

  $form['summary'] = array(
    '#markup' => '<p><strong>Total Promoter Referrals</strong> = <u>' . count($rows) . '</u></p>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );
  $form['modal'] = array(
    '#markup' => render(theme('rtpr_user_promoter_referrals_modal', array('account' => $account)))
  );
  
  $form_state['header'] = $header;
  $form_state['rows'] = $rows;
  
  return $form;
}

function rtpr_user_promoter_referrals_form_submit($form, &$form_state, $account) {
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=promoter-referrals.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $form_state['header']);

  foreach($form_state['rows'] as $row){
    $row['name'] = strip_tags($row['name']);
    fputcsv($out, $row);
  }

  fclose($out);
  exit();
}

/**
 * Helper function to build promoter referral row.
 */
function _rtpr_user_promoter_referrals_rows($uid, $link_to_child = FALSE) {
  $path = current_path();
  $role = user_role_load_by_name('Promoter');
  $results = db_query(
    'SELECT entity_id,field_pap_affiliate_id_value FROM {field_data_field_pap_affiliate_id}
    WHERE entity_id IN (SELECT uid FROM users_roles WHERE rid = :rid)',
    array(':rid' => $role->rid)
  )->fetchAll();
  
  $uid_pap_mapping = array();
  $promoter_pap_ids = array();
  foreach ($results as $record) {
    $promoter_pap_ids[] = $record->field_pap_affiliate_id_value;
    $uid_pap_mapping[$record->field_pap_affiliate_id_value] = $record->entity_id;
  }
  $rows = array();
  try {
    $account = user_load($uid);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $account_wrapper->field_pap_affiliate_id->value();
    db_set_active('pap');
    
    $referrals = db_query(
      'SELECT userid FROM {qu_pap_users}
      WHERE parentuserid = :pap_id AND userid IN (:promoter)',
      array(':pap_id' => $pap_id, ':promoter' => $promoter_pap_ids)
    )->fetchCol();
    db_set_active();
    if ($referrals) {
      foreach ($referrals as $referral) {
        if (isset($uid_pap_mapping[$referral])) {
          $referral_uid = $uid_pap_mapping[$referral];
          $referral_account = user_load($referral_uid);
          $referral_wrapper = entity_metadata_wrapper('user', $referral_account);
          $rids = $referral_wrapper->roles->value();
          $status = 'N/A';
          if (array_intersect(array(RTPR_USER_AFFILIATES_RID, RTPR_USER_RETAIL_CUSTOMER_RID), $rids)) {
            $status = 'Retail';
          }
          elseif (array_intersect(array(RTPR_USER_REWARDS_RID, RTPR_USER_RCP_RID), $rids)) {
            $status = 'Wholesale';
          }
          $offer_order_id = db_query(
            'SELECT order_id FROM commerce_line_item WHERE line_item_label LIKE :sku AND order_id IN
              (SELECT order_id FROM commerce_order WHERE uid = :uid AND status IN (:status)) ORDER BY order_id ASC',
            array(
              ':sku' => '%NC-OFFER%',
              ':uid' => $referral_uid,
              ':status' => array('waiting','pending','processing','completed','checkout_complete')
            )
          )->fetchField();
          
          $join_date = 'N/A';
          if ($offer_order_id) {
            $payment_date = db_query(
              'SELECT created FROM {commerce_payment_transaction} WHERE order_id = :order_id',
              array(':order_id' => $offer_order_id)
            )->fetchField();
            $join_date = date('m/d/Y', $payment_date);
          }

          $phone = $referral_wrapper->field_phone_number->value();
          if (!$phone) {
            $phone = $referral_wrapper->field_phone->value();
          }
          $address = $referral_wrapper->field_address->value();
          $rows[$referral_uid] = array(
            'name' => $referral_account->realname,
            'id' => $referral,
          );
          
          if ($link_to_child) {
            $rows[$referral_uid]['name'] = l(
              $referral_account->realname,
              '/' . $path,
              array(
                'fragment' => 'promoterReferralsModal',
                'attributes' => array(
                  'class' => 'wholesale ' . $referral,
                  'data-pap-id' => $referral,
                  'data-uid' => $referral_uid,
                  'data-pid' => $account->uid,
                  'data-name' => $referral_account->realname,
                  'data-toggle' => 'modal'
                ),
              )
            );
            $rows[$referral_uid]['city'] = $address['locality'];
            $rows[$referral_uid]['state'] = $address['administrative_area'];
            $rows[$referral_uid]['email'] = $referral_account->mail;
            $rows[$referral_uid]['phone'] = preg_replace('/[^0-9-]/', '', $phone);
          }
          $rows[$referral_uid]['status'] = $status;
          $rows[$referral_uid]['join'] = $join_date;
        }
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
  
  return $rows;
}
/**
 *  Callback for user/%user/promoter-referrals/%/data.
 */
function rtpr_user_promoter_referrals_data_form($account, $uid) {
  $rows = _rtpr_user_promoter_referrals_rows($uid);
  $referral_account = user_load($uid);
  $header = array(
    'name' => t('Name'),
    'id' => t('ID#'),
    'status' => t('Status'),
    'date' => t('Join Date'),
  );
  
  $form['top'] = array(
    '#markup' => '<strong>' . $referral_account->realname . '</strong> has referred <strong>' . count($rows) . '</strong> promoter accounts:'
  );
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );

  $data = array(
    'uid' => drupal_render($form),
  );
  return drupal_json_output($data);
}