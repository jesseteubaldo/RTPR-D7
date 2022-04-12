<?php

/**
 * @file
 * File for user redemption credits.
 */

function rtpr_cc_redemption_t2_overview_form($form, &$form_state) {
  $form['add'] = array(
    '#markup' => l('Add Credit', 'admin/config/rtpr/redemption/t2/edit/1'),
  );
  $header = array(
    'uid' => t('User ID'),
    'email' => t('Email'),
    'pap_id' => t('PAP ID'),
    'total' => t('Total Acquired'),
    'purchased' => t('Total Purchased'),
    'manually_add' => t('Total manually add'),
    'manually_deducted' => t('Total manually deducted'),
    'staff_gcs' => t('Staff GCS'),
    'personal_gcs' => t('Personal GCS'),
    'total_deductions' => t('Total Processed'),
    'total_available' => t('Total Available'),
    'edit' => t('Edit'),
    'list' => t('Details'),
  );
  $show_pager = TRUE;
  $rows = array();
  $page = 0;
  $per_page = 25;
  $start = 0;
  $end = $per_page;
  if (isset($_GET['page'])) {
    $page = $_GET['page'];
    $start = $page * $per_page;
  }

  $count = db_select('rtpr_t2_credits', 't2')
    ->distinct()
    ->fields('t2', array('uid'))
    ->execute()->fetchCol();

  $query = db_select('rtpr_t2_credits', 't2');
  $query->join('users', 'u', 'u.uid = t2.uid');
  $query->leftJoin('field_data_field_pap_affiliate_id', 'p', 'p.entity_id = t2.uid');
  $query->fields('t2', array('uid'));
  $query->fields('u', array('mail'));
  $query->fields('p', array('field_pap_affiliate_id_value'))
    ->orderBy('tid', 'DESC');

  if (!empty($form_state['values']['search'])) {
    $show_pager = FALSE;
    if (!empty($form_state['values']['uid'])) {
      $query->condition('u.uid', $form_state['values']['uid']);
    }
    if (!empty($form_state['values']['pap_id'])) {
      $query->condition('field_pap_affiliate_id_value', $form_state['values']['pap_id']);
    }
    if (!empty($form_state['values']['email'])) {
      $query->condition('mail', $form_state['values']['email']);
    }
  }

  $full_accounts = $query->execute()->fetchAllAssoc('uid');
  $accounts = array_slice($full_accounts, $start, $end, TRUE);
  foreach($accounts as $uid => $account) {
    $total = _rtpr_user_get_total_redemption_credits($uid);
    $available = _rtpr_user_get_available_redemption_credits($uid);
    $used = _rtpr_user_get_used_redemption_credits($uid);
    $manual = _rtpr_user_get_manual_redemption_credits($uid);
    $rows[$uid] = array(
      'uid' => $uid,
      'email' => $account->mail,
      'pap_id' => $account->field_pap_affiliate_id_value,
      'total' => $total,
      'purchased' => $total - abs(_rtpr_user_get_manual_redemption_credits($uid, 'add')),
      'manually_add' => _rtpr_user_get_manual_redemption_credits($uid, 'add'),
      'manually_deducted' => _rtpr_user_get_manual_redemption_credits($uid, 'deduct'),
      'staff_gcs' => _rtpr_user_get_used_redemption_credits($uid, 'staff'),
      'personal_gcs' => _rtpr_user_get_used_redemption_credits($uid, 'personal'),
      'total_deductions' => $used,
      'total_available' => $available,
      'edit' => l('edit', 'admin/config/rtpr/redemption/t2/edit/' . $uid),
      'list' => l('details', 'admin/config/rtpr/redemption/t2/edit/' . $uid . '/list'),
    );
  }

  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['search']['uid'] = array(
    '#type' => 'textfield',
    '#title' => 'UID',
    '#size' => 15,
  );

  $form['search']['pap_id'] = array(
    '#type' => 'textfield',
    '#title' => 'PAP ID',
    '#size' => 15,
  );

  $form['search']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#size' => 30,
  );

  $form['search']['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#submit' => array('rtpr_cc_redemption_t2_overview_form_search'),
  );

  $form['accounts'] = array(
    '#type' => 'value',
    '#value' => $full_accounts,
  );

  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );

  $form['rows'] = array(
    '#type' => 'value',
    '#value' => $rows,
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );

  if ($show_pager) {
    $current_page = pager_default_initialize(count($count), $per_page);
    $form['pager'] = array(
      '#theme' => 'pager',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
    '#submit' => array('rtpr_cc_redemption_t2_overview_form_download'),
  );
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download All'),
    '#submit' => array('rtpr_cc_redemption_t2_overview_form_download_all'),
  );

  return $form;
}

/**
 * Search Handler.
 */
function rtpr_cc_redemption_t2_overview_form_search($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['header'] = $form_state['values']['header'];
  $form_state['rows'] = $form_state['values']['rows'];
}

function rtpr_cc_redemption_t2_overview_form_download($form, &$form_state) {
  $header = $form_state['values']['header'];
  $rows = $form_state['values']['rows'];
  unset($header['edit']);
  unset($header['list']);
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=gcc-customers.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    unset($row['edit']);
    unset($row['list']);
    fputcsv($out, $row);
  }

  fclose($out);
  exit();
}

function rtpr_cc_redemption_t2_overview_form_download_all($form, &$form_state) {
  $rows = array();
  $accounts = $form_state['values']['accounts'];
  foreach($accounts as $uid => $account) {
    $total = _rtpr_user_get_total_redemption_credits($uid);
    $available = _rtpr_user_get_available_redemption_credits($uid);
    $used = _rtpr_user_get_used_redemption_credits($uid);
    $manual = _rtpr_user_get_manual_redemption_credits($uid);
    $rows[$uid] = array(
      'uid' => $uid,
      'email' => $account->mail,
      'pap_id' => $account->field_pap_affiliate_id_value,
      'total' => $total,
      'purchased' => $total - abs(_rtpr_user_get_manual_redemption_credits($uid, 'add')),
      'manually_add' => _rtpr_user_get_manual_redemption_credits($uid, 'add'),
      'manually_deducted' => _rtpr_user_get_manual_redemption_credits($uid, 'deduct'),
      'staff_gcs' => _rtpr_user_get_used_redemption_credits($uid, 'staff'),
      'personal_gcs' => _rtpr_user_get_used_redemption_credits($uid, 'personal'),
      'total_deductions' => $used,
      'total_available' => $available,
      'edit' => l('edit', 'admin/config/rtpr/redemption/t2/edit/' . $uid),
      'list' => l('details', 'admin/config/rtpr/redemption/t2/edit/' . $uid . '/list'),
    );
  }

  unset($header['edit']);
  unset($header['list']);
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=gcc-customers.csv');
  $out = fopen('php://output', 'w');
  fputcsv($out, $header);

  foreach($rows as $row){
    unset($row['edit']);
    unset($row['list']);
    fputcsv($out, $row);
  }

  fclose($out);
  exit();
}

/**
 * Form callback for admin/config/rtpr/redemption/t2/edit/%.
 */
function rtpr_cc_redemption_t2_credit_edit_form($form, $form_states, $account) {
  $form['uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#default_value' => $account->uid,
    '#maxlength'=> 30,
    '#size'=> 30,
    '#required' => TRUE,
  );

  $form['new'] = array(
    '#type' => 'textfield',
    '#title' => t('New Credits'),
    '#description' => t('Number of credits to add to current user.'),
    '#maxlength'=> 30,
    '#size'=> 30,
    '#required' => TRUE,
  );

  $form['log'] = array(
    '#type' => 'textfield',
    '#title' => t('Log message'),
    '#description' => t('Provide an explanation of the changes you are making. This will help other authors understand your motivations.'),
    '#size'=> 50,
    '#required' => TRUE,
  );

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => isset($_GET['uid']) ? t('Add') : t('Update'),
  );

  return $form;
}

/**
 * Validate form.
 */
function rtpr_cc_redemption_t2_credit_edit_form_validate($form, $form_states) {
  $new = $form_states['values']['new'];
  if (!is_numeric($new)) {
    form_set_error('new', t('Invalid credits entered!'));
  }
  $uid = $form_states['values']['uid'];
  $account = user_load($uid);
  if (!$account) {
    form_set_error('uid', t('Wrong user id!'));
  }
}

/**
 * Form submit handler.
 */
function rtpr_cc_redemption_t2_credit_edit_form_submit($form, $form_states) {
  global $user;
  $uid = $form_states['values']['uid'];
  $new = $form_states['values']['new'];
  $log = $form_states['values']['log'];
  $account = user_load($uid);
  _rtpr_cc_update_t2_credits($uid, $new, $log, 'manual');
  $available = _rtpr_user_get_available_redemption_credits($uid);
  drupal_set_message(t('Redemption credit updated for @mail. Total available: @available', array('@mail' => $account->mail, '@available' => $available)));
}

/**
 * Callback for admin/config/rtpr/redemption/t2.
 */
function rtpr_cc_redemption_t2_form($form, $form_states) {
  if (isset($form_states['storage']['step_two'])) {
    return rtpr_cc_redemption_t2_form_step_two($form, $form_states);
  }
  if (isset($form_states['storage']['email'])) {
    $account = $form_states['account'];
    drupal_set_message(t('User <strong>@pap_id</strong> does not have enough credits!', array('@pap_id' => $form_states['values']['pap_id'])), 'warning');
    rules_invoke_event('rtpr_cc_notify_event', $account, 'vendor_no_credits', $account, 'extra');
  }
  $form['pap_id'] = array(
    '#type' => 'textfield',
    '#title' => t('PAP ID'),
    '#description' => t('Enter the PAP id of the vendor.'),
    '#required' => TRUE,
    //c7c80fe1
    //'#default_value' => '1db79bfb' ,

  );

  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

function rtpr_cc_redemption_t2_form_validate($form, &$form_states) {
  // Step 2 validate.
  if (isset($form_states['clicked_button']['#id']) && $form_states['clicked_button']['#id'] == 'edit-submit') {
    $email = $form_states['values']['email'];
    $result = rtpr_cc_redemption_t2_validate_email($email);
    if (!$result['email']) {
      form_set_error('email', t('Invalid email address!'));
    }
    if (!$result['valid']) {
      $form_states['valid'] = FALSE;
      drupal_set_message($result['message'], 'warning');
      if ($result['status'] == 'retail') {
        $coupon = commerce_coupon_create('discount_coupon');
        $coupon->code = commerce_coupon_generate_coupon_code('discount_coupon', 8);
        $coupon->commerce_discount_reference[LANGUAGE_NONE][0]['target_id'] = 4;
        $coupon->commerce_coupon_conditions[LANGUAGE_NONE][0] = array(
           'condition_name' => 'commerce_coupon_usage_evaluate_usage',
           'condition_settings' => array(
              'max_usage' => 1
           )
        );
        commerce_coupon_save($coupon);
        rules_invoke_event('rtpr_cc_notify_event', user_load($form_states['uid']), 'redemption_retail', $result['account'], $coupon->code);
      }
      elseif ($result['status'] == 'potential_customer') {
        rules_invoke_event('rtpr_cc_notify_event', user_load($form_states['uid']), 'redemption_potential', $result['account'], 'extra');
      }
      elseif ($result['status'] == 'other') {
        rules_invoke_event('rtpr_cc_notify_event', user_load($form_states['uid']), 'redemption_non_retail', $result['account'], 'extra');
      }

    }
  }

  if (isset($form_states['values']['pap_id'])) {
    $pap_id = trim($form_states['values']['pap_id']);
    $entity_id = db_select('field_data_field_pap_affiliate_id', 'pai')
      ->fields('pai', array('entity_id'))
      ->condition('field_pap_affiliate_id_value', $pap_id)
      ->execute()->fetchField();
    if ($pap_id) {
      $form_states['uid'] = $entity_id;
      $form_states['pap_id'] = $pap_id;
      if (!$entity_id) {
        form_set_error('pap_id', t('PAP ID <strong>@pap_id</strong> does not existed!', array('@pap_id' => $pap_id)));
      }
    }
  }
}

  /**
 * Callback for admin/config/rtpr/redemption/t2.
 */
function rtpr_cc_redemption_t2_form_submit($form, &$form_states) {
  if (isset($form_states['valid'])) {
    $form_states['redirect'] = 'admin/config/rtpr/redemption/t2';
    return $form;
  }
  if ($form_states['clicked_button']['#id'] == 'edit-next') {
    $uid  = $form_states['uid'];
    $account = user_load($uid);
    $form_states['account'] = $account;
    $credits = _rtpr_user_get_available_redemption_credits($uid);

    if ($credits && $credits >= 1) {
      $form_states['storage']['step_two'] = TRUE;
    }
    else {
      $form_states['storage']['email'] = TRUE;

    }

    $form_states['storage']['page_one_values'] = $form_states['values'];
    $form_states["rebuild"] = TRUE;
  }
  // Handle page 2 submissions.
  else {
    $values = $form_states['values'];

    $new_user = array(
      'name' => $values['email'],
      'pass' => time(), // note: do not md5 the password
      'mail' => $values['email'],
      'status' => 1,
      'init' => $values['email'],
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => DRUPAL_AUTHENTICATED_RID,
        RTPR_CC_POTENTIAL_CUSTOMER_RID => RTPR_CC_POTENTIAL_CUSTOMER_RID
      ),
      'field_phone' => array(LANGUAGE_NONE => array(array('value' => $values['phone']))),
      'field_first_name' => array(LANGUAGE_NONE => array(array('value' => $values['first_name']))),
      'field_last_name' => array(LANGUAGE_NONE => array(array('value' => $values['last_name']))),
      'field_address' => array(LANGUAGE_NONE => array(array(
        'country' => $values['country'],
        'administrative_area' => $values['state'],
        'locality' => $values['city'],
        'postal_code' => $values['zip'],
        'thoroughfare' => $values['address_1'],
        'premise' => $values['address_2'],
      ))),
    );
    $account = user_save('', $new_user);
    $account->roles = array(
        DRUPAL_AUTHENTICATED_RID => DRUPAL_AUTHENTICATED_RID,
        RTPR_CC_POTENTIAL_CUSTOMER_RID => RTPR_CC_POTENTIAL_CUSTOMER_RID
    );
    $account->field_pap_referral_id = array(LANGUAGE_NONE => array(array('value' => trim($form_states['pap_id']))));
    $account = user_save($account);

    if ($account) {
      _rtpr_user_post_virtual_transaction_to_pap($account, $form_states['pap_id']);
      $parent_account = user_load($form_states['uid']);
      _rtpr_cc_update_t2_credits($parent_account->uid, -1, 'Used on account ' . $account->mail, 'redeem');
      drupal_set_message(t('Successfully processed <strong>@email</strong>', array('@email' => $account->mail)));
      rules_invoke_event('rtpr_cc_notify_event', $parent_account, 'successfully_redemp', $account, $values['email']);
      $gcc_customers = variable_get('gcc_customers', array());
      $gcc_customers[$parent_account->uid][] = $account->uid;
      variable_set('gcc_customers', $gcc_customers);
      rtpr_cc_insert_gcc($account->uid, $parent_account->uid);
    }

  }
}

/**
 * Insert GCC data into rtpr_gcc.
 */
function rtpr_cc_insert_gcc($owner_uid, $parent_uid) {
  global $user;
  db_insert('rtpr_gcc')
  ->fields(array('uid', 'parent_uid', 'submitted', 'submitted_uid'))
  ->values(array(
  		'uid' => $owner_uid,
  		'parent_uid' => $parent_uid,
  		'submitted' => REQUEST_TIME,
  		'submitted_uid' => $user->uid,
  		))
  ->execute();
}

function _rtpr_cc_redemption_t2_submit($account, $pap_id, $uid, $type = '') {
  _rtpr_user_post_virtual_transaction_to_pap($account, $pap_id);
  $parent_account = user_load($uid);
  _rtpr_cc_update_t2_credits($uid, -1, $type, 'redeem');
  drupal_set_message(t('Successfully processed <strong>@email</strong>', array('@email' => $account->mail)));
  //rules_invoke_event('rtpr_cc_notify_event', $parent_account, 'successfully_redemp', $account, 'extra');
  $gcc_customers = variable_get('gcc_customers', array());
  $gcc_customers[$parent_account->uid][] = $account->uid;
  variable_set('gcc_customers', $gcc_customers);
  rtpr_cc_insert_gcc($account->uid, $parent_account->uid);
}

function _rtpr_user_post_virtual_transaction_to_pap($account, $affiliate_id) {
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $account_id = variable_get('commerce_post_affiliate_pro_merchant_id', NULL);
  $path = libraries_get_path('pap');
  require_once $path . '/PapApi.class.php';
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $sale_tracker = new Pap_Api_SaleTracker(variable_get('commerce_post_affiliate_pro_merchant_url_prefix', 'http://demo.qualityunit.com/pax4/') . "scripts/sale.php", TRUE);
  $sale_tracker->setAccountId($account_id);
  $sale = $sale_tracker->createSale();
  $sale->setOrderID('GCC' . $account->uid);
  $sale->setProductID('GCC-Order');
  $sale->setTotalCost(10);
  $sale->setCampaignID('11111111');
  $sale->setAffiliateID($affiliate_id);
  // Ser status to pending,.
  $sale->setStatus('P');
  // Name.
  $sale->setData1(preg_replace("/[^a-zA-Z0-9 ]/", "", $account_wrapper->field_first_name->value()) . ' ' . preg_replace("/[^a-zA-Z0-9 ]/", "", $account_wrapper->field_last_name->value()));
  // Customer email.
  $sale->setData2($account_wrapper->mail->value());
  // Phone number.
  $sale->setData3($account_wrapper->field_phone->value());
  // User ID.
  $sale->setData5($account->uid);
  $result = $sale_tracker->register();
}


/**
 * Step two.
 */
function rtpr_cc_redemption_t2_form_step_two($form, &$form_states) {
  module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic information'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['basic']['status'] = array(
    '#type' => '#markup',
    '#prefix' => '<div id="email-status">',
    '#suffix' => '</div>',
  );

  $form['basic']['email'] = array(
    '#title' => 'Email Address',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'rtpr_cc_redemption_t2_validate_email_ajax_callback',
      'wrapper' => 'email-status',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['basic']['first_name'] = array(
    '#title' => 'First Name',
    '#type' => 'textfield',
    '#required' => TRUE,

  );

  $form['basic']['last_name'] = array(
    '#title' => 'Last Name',
    '#type' => 'textfield',
    '#required' => TRUE,

  );

  $form['basic']['phone'] = array(
    '#title' => 'Phone',
    '#type' => 'textfield',
    '#required' => TRUE,

  );

  $form['address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Address information'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['address']['country'] = array(
    '#title' => t('Country'),
    '#type' => 'select',
    '#options' => array(
      'CA' => 'Canada',
      'US' => 'United States',
    ),
    '#default_value' => 'US',
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => '_rtpr_cc_t2_country_callback',
      'method' => 'replace',
      'effect' => 'fade',
      'wrapper' => 'address-country',
    )
  );

  $form['address']['address_1'] = array(
    '#title' => 'Address 1',
    '#type' => 'textfield',
    '#required' => TRUE,


  );

  $form['address']['address_2'] = array(
    '#title' => 'Address 2',
    '#type' => 'textfield',

  );

  $form['address']['city'] = array(
    '#title' => 'City',
    '#type' => 'textfield',
    '#required' => TRUE,

  );

  $form['address']['state'] = array(
    '#prefix' => '<div id="address-country">',
    '#suffix' => '</div>',
    '#title' => 'State',
    '#type' => 'select',
    '#validated' => TRUE,
    '#required' => TRUE,
    '#options' => addressfield_get_administrative_areas('US'),
  );

  $form['address']['zip'] = array(
    '#title' => 'Zip',
    '#type' => 'textfield',
    '#required' => TRUE,

  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Ajax callback for country.
 */
function _rtpr_cc_t2_country_callback($form, $form_state) {
  module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');
  $form['address']['state'][ '#prefix'] = '<div id="address-country">';
  $form['address']['state']['#suffix'] = '</div>';
  $country = isset($form_state['values']['country']) ? $form_state['values']['country'] : 'US';
  $options = addressfield_get_administrative_areas($country);
  $form['address']['state']['#options'] = $options;
  return $form['address']['state'];
}

/**
 * Aajx callback to provide instance feedback on email entered.
 */
function rtpr_cc_redemption_t2_validate_email_ajax_callback($form, $form_states) {
  $form['status'][ '#prefix'] = '<div id="email-status">';
  $form['status']['#suffix'] = '</div>';
  $result = rtpr_cc_redemption_t2_validate_email($form_states['values']['email']);
  $form['status']['#markup'] = $result['markup'];
  return $form['status'];
}

function rtpr_cc_redemption_t2_validate_email($email) {
  $result = array();
  $result['valid'] = FALSE;
  $result['status'] = FALSE;
  $result['account'] = FALSE;
  $result['email'] = FALSE;
  if (_rtpr_cc_domain_exists($email) && valid_email_address($email)) {
    $result['email'] = TRUE;
    $account = user_load_by_mail($email);
    $result['account'] = $account;
    if (!$account) {
      $markup = '<div class="messages status">This email address is not currently in our system.</div>';
      $result['valid'] = TRUE;
    }
    else {
      $roles = $account->roles;
      $rids = array_keys($roles);
      if (in_array(RTPR_CC_RETAIL_RID, $rids)) {
         $result['status'] = 'retail';
         $message = $email . ' is a <strong>Retail Customer</strong>';
         $markup = '<div class="messages warning">' . $message . '</div>';
         $result['message'] = $message;
         // they are already in the system but we gave them a coupon to use on their next order (code included)
         // “Redemption Customer already in the system” email
      }
      elseif (in_array(RTPR_CC_POTENTIAL_CUSTOMER_RID, $rids)) {
        $result['status'] = 'potential_customer';
        $message = $email . ' is already in system';
        $markup = '<div class="messages warning">' . $message . '</div>';
        $result['message'] = $message;
        // Send the Potential Customer the Potential Customer Activation Email
      }
      else {
        $result['status'] = 'other';
        $message = $email . ' is already in system';
        $markup = '<div class="messages warning">' . $message . '</div>';
        $result['message'] = $message;
        //we received their rebate and they are already in the system and do not qualify for the offer”
      }
    }
  }
  else {
    $markup = '<div class="messages error">' . $email . ' is <strong>NOT</strong> an valid email!</div>';
  }

  $result['markup'] = $markup;

  return $result;
}

/**
 * T2 Reassign form.
 */
function rtpr_cc_redemption_t2_reassign_form($form, $form_states) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#required' => TRUE,
    '#autocomplete_path' => 'rtprcc/autocomplete',
  );

  $form['pap_id'] = array(
    '#type' => 'textfield',
    '#title' => 'New PAP Affiliate ID',
    '#required' => TRUE,

  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Re-assign'),
  );

  return $form;
}

/**
 * T2 Reassign form validate.
 */
function rtpr_cc_redemption_t2_reassign_form_validate($form, $form_states) {

  $values = $form_states['values'];
  $new_pap_id = trim($values['pap_id']);
  $account = user_load_by_mail($values['email']);
  $account_wrapper = entity_metadata_wrapper('user', $account);

  $current_parent = db_select('field_data_field_pap_affiliate_id', 'p')
    ->fields('p')
    ->condition('field_pap_affiliate_id_value', $new_pap_id)
    ->execute()->fetchObject();

  $current_parent_account = user_load($current_parent->entity_id);
  $credits = _rtpr_user_get_available_redemption_credits($current_parent_account->uid);

  if (!$credits) {
    form_set_error('pap_id', t('@mail does not have enough credits!', array('@mail' => $current_parent_account->mail)));
  }

  if (!in_array(46, array_keys($account->roles))) {
    form_set_error('email', t('You can only re-assign potential customers!.'));
  }

}

function rtpr_cc_redemption_t2_reassign_form_submit($form, $form_states) {

  $values = $form_states['values'];
  $account = user_load_by_mail($values['email']);
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $new_pap_id = trim($values['pap_id']);
  $orderid = 'GCC' . $account->uid;
  db_set_active('pap');

  db_delete('qu_pap_transactions')
    ->condition('orderid', $orderid)
    ->execute();

  db_set_active();

  $old_pap_id = $account_wrapper->field_pap_referral_id->value();
  $current_parent = db_select('field_data_field_pap_affiliate_id', 'p')
    ->fields('p')
    ->condition('field_pap_affiliate_id_value', $new_pap_id)
    ->execute()->fetchObject();
  $old_parent = db_select('field_data_field_pap_affiliate_id', 'p')
    ->fields('p')
    ->condition('field_pap_affiliate_id_value', $old_pap_id)
    ->execute()->fetchObject();

  $old_parent_account = user_load($old_parent->entity_id);
  $old_parent_wrapper = entity_metadata_wrapper('user', $old_parent_account);

  $current_parent_account = user_load($current_parent->entity_id);
  $current_parent_wrapper = entity_metadata_wrapper('user', $current_parent_account);

  _rtpr_cc_update_t2_credits($old_parent->entity_id, 1, 'Refund on account ' . $account->mail, 'credit');

  $account_wrapper->field_pap_referral_id->set($new_pap_id);
  $account_wrapper->save();
  _rtpr_cc_redemption_t2_submit($account, $new_pap_id, $current_parent_account->uid, 'Used on account ' . $account->mail);
}

/**
 * Manually send reminder email.
 */
function rtpr_cc_redemption_t2_reminder_form($form, $form_states) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#required' => TRUE,
    '#autocomplete_path' => 'rtprcc/autocomplete',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Reminder Email'),
  );

  return $form;
}

function rtpr_cc_redemption_t2_reminder_form_validate($form, $form_states) {
  $values = $form_states['values'];
  $account = user_load_by_mail($values['email']);
  if (!in_array(46, array_keys($account->roles))) {
    form_set_error('email', t('You can only re-assign potential customers!.'));
  }
}

function rtpr_cc_redemption_t2_reminder_form_submit($form, $form_states) {
  $values = $form_states['values'];
  $account = user_load_by_mail($values['email']);
  $wrapper = entity_metadata_wrapper('user', $account);
  $referral_id = $wrapper->field_pap_referral_id->value();
  $query = db_select('field_data_field_pap_affiliate_id', 'p')
    ->fields('p', array('entity_id'))
    ->condition('field_pap_affiliate_id_value', $referral_id);
  $parent = $query->execute()->fetchObject();
  db_update('users')
    ->fields(array(
      'login' => REQUEST_TIME,
    ))
  ->condition('uid', $account->uid, '=')
  ->execute();
  $account->login = REQUEST_TIME;
  $one_time_link = user_pass_reset_url($account) . '?parent=' . $parent->entity_id;
  rules_invoke_event('rtpr_cc_notify_event', $account, 'reminder', $account, $one_time_link);
  drupal_set_message(t('Reminder email send to @email' ,array('@email' => $account->mail)));
}

/**
 * Activation report.
 */
function rtpr_cc_redemption_t2_activation_report_form($form, &$form_state) {
  $disabled = !(user_access('administer site configuration') ||variable_get('rtpr_cc_t2_activation_report_finished', TRUE));

  if (!variable_get('rtpr_cc_t2_activation_report_finished')) {
    drupal_set_message(t('New report in progress...'), 'warning');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download Activation Report'),
    '#disabled' => $disabled
  );

  $last_report = variable_get('rtpr_cc_t2_activation_report');
  drupal_set_message(t('Click <a href="@link">here</a> to download report generated from @timestamp ago.' , array('@link' => $last_report['link'], '@timestamp' => format_interval(REQUEST_TIME - $last_report['timestamp']))));

  return $form;
}

function rtpr_cc_redemption_t2_activation_report_form_submit($form, &$form_state) {
  $timestamp = REQUEST_TIME;
  $t2_counter = variable_set('rtpr_cc_t2_counter', $timestamp);
  $file_name = './sites/default/files/private/' . $timestamp . '_gcc.csv';
  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'first' => t('First Name'),
    'last' =>  t('Last Name'),
    'address_1' => t('Address 1'),
    'address_2' => t('Address 2'),
    'city' => t('City'),
    'state' => t('State'),
    'zip_code' => t('Zip Code'),
    'pap_referrer' => t('PAP Referrer'),
    'gcc_date' => t('Date initially entered as a GCC'),
    'initial_amount' => t(' Initial Purchase Amount'),
    'initial_date' => t('Date of Initial Purchase'),
    'balance' => t('Activation Credit Balance'),
    'expiration' => t('Activation Credit Balance Expiration Date'),
    'referred_credits' =>  t('STR Credit Total Earned'),
    'physical_offer' =>  t('Physical'),
    'email_offer' =>  t('Email'),
  );
  $csv_file = fopen($file_name, 'a') or die('Cant open file!');
  fputcsv($csv_file, $header, ',', '"');
  fclose($csv_file);

  variable_set('rtpr_cc_t2_activation_report_finished', FALSE);
  $orders = db_query('SELECT uid,submitted FROM rtpr_gcc')->fetchAllAssoc('uid');
  $retail_uids = db_query('SELECT g.uid FROM rtpr_gcc as g, users_roles as r WHERE r.uid = g.uid AND rid = 7 ORDER BY uid ASC')->fetchCol();
  batch_set(rtpr_cc_redemption_t2_activation_report_batch($retail_uids, $orders, $timestamp));
  background_batch_process_batch();

}

/**
 * Activation report batch.
 */
function rtpr_cc_redemption_t2_activation_report_batch($retail_uids, $orders, $timestamp) {
  $completed_orders = db_select('commerce_order', 'o')
    ->fields('o')
    ->condition('status', array('pending', 'processing', 'completed'))
    ->condition('uid', $retail_uids, 'IN')
    ->orderBy('created', 'DESC')
    ->execute()->fetchAllAssoc('uid');
  $cancelled_orders = db_select('commerce_order', 'o')
    ->fields('o')
    ->condition('status', array('canceled'))
    ->condition('uid', $retail_uids, 'IN')
    ->orderBy('created', 'DESC')
    ->execute()->fetchAllAssoc('uid');

  $operations = array();

  foreach ($retail_uids as $uid) {

    $order_date = isset($orders[$uid]) ? $orders[$uid]->submitted : NULL;
    $completed_order_id = isset($completed_orders[$uid]) ? $completed_orders[$uid]->order_id : NULL;
    $cancelled_order = isset($cancelled_orders[$uid]) ? 'CANCEL' : NULL;
    $operations[] = array(
      'rtpr_cc_redemption_t2_activation_report_op',
      array(
        $uid,
        $order_date,
        $completed_order_id,
        $cancelled_order,
        $timestamp
      ),
    );
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'rtpr_cc_redemption_t2_activation_report_op_finished',
    'file' => drupal_get_path('module', 'rtpr_cc') . '/includes/rtpr_cc.redemption.t2.admin.inc', // Required.
  );

  return $batch;

}

function rtpr_cc_redemption_t2_activation_report_op($uid, $order_date, $completed_order_id, $cancelled_order, $timestamp, &$context) {
  $account = user_load($uid);

  if ($account) {
      // Optional message displayed under the progressbar.
    $context['message'] = t('Processing user %email', array('%email' => $account->mail));
    $roles = array_keys($account->roles);
    // Only retail role we intertested in.
    if (in_array(7, $roles)) {
      $uid  = $account->uid;
      // Default values.
      $gcc_date = 'N/A';
      $initial_date = 'N/A';
      $sub_total = 0;
      $balanace = 0;
      $expiration = 'N/A';
      $total_referral = 0;
      $referred_credits = 0;
      if ($completed_order_id) {
        $order_id = $completed_order_id;

        $order = commerce_order_load($order_id);
        if ($order) {
          $wrapper = entity_metadata_wrapper('commerce_order', $order);
          // This will returns an array that contains amount and currency.
          // array(2) { ["amount"]=> int(4350) ["currency_code"]=> string(3) "USD" }
          $product_total = commerce_line_items_total($wrapper->commerce_line_items, array('product'));
          $sub_total = $product_total['amount'];
          $initial_date = date('Y-m-d H:i:s', $order->created);
          unset($order);
        }
      }
      elseif ($cancelled_order) {
        $initial_date = 'CANCEL';
      }
      $account_wrapper = entity_metadata_wrapper('user', $account);
      $address = $account_wrapper->field_address->value();

      if ($order_date) {
        $gcc_date = date('Y-m-d H:i:s', $order_date);
      }
      $point = rtpr_cc_get_total_userpoints_txn($uid);
      $referred = rtpr_cc_get_total_referred_points($uid);
      if ($referred->total) {
          $referred_credits = $referred->total;
      }

      if ($point->expirydate) {
        $balanace = $point->total;
        $expiration = date('m-d-Y', $point->expirydate);
      }
      $subsciptions = $account_wrapper->field_offer_subscription->value();
      $temp = array(
        'uid' => $uid,
        'email' => $account->mail,
        'first' => $account_wrapper->field_first_name->value(),
        'last' =>  $account_wrapper->field_last_name->value(),
        'address_1' => $address['thoroughfare'],
        'address_2' => $address['premise'],
        'city' => $address['locality'],
        'state' => $address['administrative_area'],
        'zip_code' => $address['postal_code'],
        'pap_referrer' => $account_wrapper->field_pap_referral_id->value(),
        'gcc_date' => $gcc_date,
        'initial_amount' => commerce_currency_format($sub_total, 'USD'),
        'initial_date' => $initial_date,
        'balance' => $balanace,
        'expiration' => $expiration,
        'referred_credits' =>  $referred_credits,
        'physical_offer' => !in_array('no_mail_offer', $subsciptions) ? 'YES' : 'NO',
        'email_offer' => !in_array('no_email_offer', $subsciptions) ? 'YES' : 'NO',
      );

      $file_name = './sites/default/files/private/' . $timestamp . '_gcc.csv';
      $csv_file = fopen($file_name, 'a') or die('Cant open file!');
      fputcsv($csv_file, $temp, ',', '"');
      fclose($csv_file);
    }
  }
}

function rtpr_cc_redemption_t2_activation_report_op_finished($success, $rows, $operations) {
  if ($success) {
      $header = array(
        'uid' => t('UID'),
        'email' => t('Email'),
        'first' => t('First Name'),
        'last' =>  t('Last Name'),
        'address_1' => t('Address 1'),
        'address_2' => t('Address 2'),
        'city' => t('City'),
        'state' => t('State'),
        'zip_code' => t('Zip Code'),
        'pap_referrer' => t('PAP Referrer'),
        'gcc_date' => t('Date initially entered as a GCC'),
        'initial_amount' => t(' Initial Purchase Amount'),
        'initial_date' => t('Date of Initial Purchase'),
        'balance' => t('Activation Credit Balance'),
        'expiration' => t('Activation Credit Balance Expiration Date'),
        'referred_credits' =>  t('STR Credit Total Earned'),
        'physical_offer' =>  t('Physical'),
        'email_offer' =>  t('Email'),
      );
      $timestamp = variable_get('rtpr_cc_t2_counter', time());
      $file_path = 'public://private/' . $timestamp . '_gcc.csv';
      $link = file_create_url($file_path);
      $loglink = l("here", $link);
      $msg = "Click $loglink to download the report.";
      drupal_set_message($msg);
      variable_set('rtpr_cc_t2_activation_report', array('timestamp' => time(), 'link' => $link));
      variable_set('rtpr_cc_t2_activation_report_finished', TRUE);
  } else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    watchdog('rtpr_cc', t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }

}

function rtpr_cc_get_total_userpoints_txn($uid) {
  $query = db_select('userpoints_txn', 't')
    ->fields('t')
    ->condition('status', 2, '!=')
    ->condition('uid', $uid);

  $alias = $query->addExpression('SUM(points)', 'total');

  return $query->execute()->fetch();
}

function rtpr_cc_get_total_referred_points($uid) {
  $query = db_select('userpoints_txn', 't')
    ->fields('t')
    ->condition('status', 2, '!=')
    ->condition('description', 'Credit earned from%', 'LIKE')
    ->condition('uid', $uid);

  $alias = $query->addExpression('SUM(points)', 'total');

  return $query->execute()->fetch();
}

/**
 * Manually activate.
 */
function rtpr_cc_redemption_t2_activate_form($form, &$form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#required' => TRUE,
    '#autocomplete_path' => 'rtprcc/autocomplete',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Activate!'),
  );

  return $form;
}

function rtpr_cc_redemption_t2_activate_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $account = user_load_by_mail($values['email']);

  if (!in_array(46, array_keys($account->roles))) {
    form_set_error('email', t('You can only re-assign potential customers!.'));
  }
}

function rtpr_cc_redemption_t2_activate_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $account = user_load_by_mail($values['email']);
  $account_wrapper = entity_metadata_wrapper('user', $account);

  $new_pap_id = $account_wrapper->field_pap_referral_id->value();
  if ($new_pap_id) {
    $current_parent = db_select('field_data_field_pap_affiliate_id', 'p')
      ->fields('p')
      ->condition('field_pap_affiliate_id_value', $new_pap_id)
      ->execute()->fetchObject();
    $vendor = user_load($current_parent->entity_id);
    if ($vendor) {
      rules_invoke_event('rtpr_cc_notify_event', $vendor, 'potential_customer_activate', $account, 'extra');
      // Issue $15 credits
      if (!_rtpr_user_has_bonus($account->uid)) {
        $params = array(
          'points' => 1500,
          'uid' => $account->uid,
          'operation' => 'admin',
          'description' => 'New customer bonus',
          'tid' => 0,
          'time_stamp' => time(),
          'moderate' => FALSE,
          'approver_uid' => 1,
          'expirydate' => strtotime("3 months")
        );
        userpoints_userpointsapi($params);
        drupal_set_message(t('Welcome! Your account has been activated and you have been issued a $15 in Online Gift Card Credit to use for any Product in our Shop, <a href="@link">Click Here</a> to Shop Now!', array('@link' => '/shop')));
      }
      $account_wrapper->roles = array(
        2 => 2,
        7 => 7,
      );
      $account_wrapper->save();
      $manual_t2_activated = variable_get('rtpr_cc_t2_manual_activate', array());
      $manual_t2_activated[$account->uid] = $user->uid;
      variable_set('rtpr_cc_t2_manual_activate', $manual_t2_activated);
    }
  }
}


/**
 * Unsubscribe user.
 */
function rtpr_cc_redemption_t2_unsubscribe_form($form, $form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#required' => TRUE,
    '#autocomplete_path' => 'rtprcc/autocomplete',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unsubscribe!'),
  );

  return $form;
}

function rtpr_cc_redemption_t2_unsubscribe_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $account = user_load_by_mail($values['email']);

  if ($account->init == 'unsubscribe') {
    form_set_error('email', t('User already unsubscribe!'));
  }
}


/**
 * Unsubscribe user form submit handler.
 */
function rtpr_cc_redemption_t2_unsubscribe_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $account = user_load_by_mail($values['email']);
  if ($account) {
      db_update('users')
    ->fields(array(
      'init' => 'unsubscribe',
    ))
    ->condition('uid', $account->uid, '=')
    ->execute();
    drupal_set_message(t('Successfully unsubscribe @email.', array('@email' => $account->mail)));
  }
}


function rtpr_cc_redemption_t2_manual_activated_report_form($form, $form_state) {
  $manual_users = variable_get('rtpr_cc_t2_manual_activate', array());
  $header = array(
    'uid' => t('UID'),
    'name' => t('Name'),
    'referral' => t('Referral PAP ID'),
    'email' => t('Email'),
    'admin' => t('Activated By'),
  );
  $rows = array();

  foreach($manual_users as $user_uid => $admin_uid) {
    $customer = user_load($user_uid);
    $admin = user_load($admin_uid);

    if ($customer) {
      $customer_wrapper = entity_metadata_wrapper('user', $customer);
      $rows[$user_uid] = array(
        'uid' => $user_uid,
        'name' => $customer_wrapper->field_first_name->value() . ' ' . $customer_wrapper->field_last_name->value(),
        'referral' => $customer_wrapper->field_pap_referral_id->value(),
        'email' => $customer->mail,
        'admin' => $admin->mail,
      );
    }
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );


  return $form;
}

function rtpr_cc_redemption_t2_missing_gcs_report_form($form, &$form_state) {

  db_set_active('pap');
  $uids = db_select('qu_pap_transactions', 't')
    ->fields('t', array('data5'))
    ->distinct('orderid')
    ->condition('orderid', 'GCC%', 'LIKE')
    ->execute()->fetchCol();

  db_set_active();

  $uids_by_bonus = db_select('userpoints_txn', 'p')
    ->fields('p', array('uid'))
    ->condition('description', 'New customer bonus')
    ->execute()->fetchCol();

  $uids_by_role = db_select('users_roles', 'r')
    ->fields('r', array('uid'))
    ->condition('rid', 46)
    ->execute()->fetchCol();

  $drupal_uids = array_unique(array_merge($uids_by_role, $uids_by_bonus));

  $missing_uids = array_diff($drupal_uids, $uids);

  $accounts = user_load_multiple(array_values($missing_uids));
  $form_state['accounts'] = $accounts;
  $header = array(
    'uid' => t('UID'),
    'name' => t('Name'),
    'email' => t('Email'),
    'referral' => t('Referral'),
  );

  $options = array();

  foreach($accounts as $account) {
    $wrapper = entity_metadata_wrapper('user', $account);
    $options[$account->uid] = array(
      'uid' => $account->uid,
      'name' => $wrapper->field_first_name->value() . ' ' . $wrapper->field_last_name->value(),
      'email' => $account->mail,
      'referral' => $wrapper->field_pap_referral_id->value(),
    );
  }


 //Build the tableselect.
  $form['gcs'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#required' => TRUE,
    '#multiple' => TRUE,
    '#empty' => t('No Missing GCS!'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Post to PAP'),
  );

  return $form;

}

function rtpr_cc_redemption_t2_missing_gcs_report_form_submit($form, &$form_state) {
  $uids = array_filter($form_state['values']['gcs']);
  $accounts = $form_state['accounts'];
  foreach($uids as $uid) {
    $account = $accounts[$uid];
    $wrapper = entity_metadata_wrapper('user', $account);
    $pap_id = $wrapper->field_pap_referral_id->value();
    _rtpr_user_post_virtual_transaction_to_pap($account, $pap_id);
    drupal_set_message(t('Successfully post %email to PAP.', array('%email' => $account->mail)));
  }
}

/**
 * Form to add list of user to allow access to pre-release features.
 */
function rtpr_cc_redemption_t2_allow_list_form($form, $form_state) {

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add user to allow list to access /gcc-submission'),
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('container-inline')),
  );


  $form['options']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'User',
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['options']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'pap' => t('PAP ID'),
  );

  $rows = array();
  $list = variable_get('rtpr_cc_redemption_allowed_list', array());
  $new = array();
  foreach($list as $uid) {
    $account = user_load($uid);

    if ($account) {
      $account_wrapper = entity_metadata_wrapper('user', $account);

      $rows[$account->uid] = array(
        'uid' => $account->uid,
        'email' => $account->mail,
        'pap' => $account_wrapper->field_pap_affiliate_id->value(),
      );
    }
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No data available.'),
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

/**
 * Validate rtpr_cc_redemption_t2_allow_list_form().
 */
function rtpr_cc_redemption_t2_allow_list_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $op = $form_state['input']['op'];
  if ($op == 'Add') {
    $account = user_load_by_mail($values['email']);
    if (!$account) {
      form_set_error('email', t('Invalid email!'));
    }
    $form_state['account'] = $account;
  } else if ($op == 'Delete') {
    $table =  array_filter($values['table']);
    if (empty($table)) {
       form_set_error('table', t('At least one row must selected for delete!'));
    }
  }

}

/**
 * Submit handler for rtpr_cc_redemption_t2_allow_list_form().
 */
function rtpr_cc_redemption_t2_allow_list_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $account = $form_state['account'];
  $allow_list = variable_get('rtpr_cc_redemption_allowed_list', array());
  $op = $form_state['input']['op'];
  if ($op == 'Add') {
    $allow_list[$account->uid] = $account->uid;
  } else {
    $table =  array_filter($values['table']);
    foreach($table as $uid) {
      unset($allow_list[$uid]);
    }
  }

  variable_set('rtpr_cc_redemption_allowed_list', $allow_list);
}

/**
 * Failed Activation Report.
 */
function rtpr_cc_redemption_t2_failed_activation_form($form, $form_state) {
  $query = db_select('users_roles', 'r');
  $query->join('userpoints_txn', 't', 't.uid=r.uid');
  $query->fields('r', array('uid'))->condition('r.rid', 46);

  $uids = $query->execute()->fetchCol();
  $accounts = user_load_multiple($uids);
  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'refer_pap_id' => t('PAP Referral ID'),
  );
  $rows = array();
  foreach($accounts as $account) {
    $wrapper = entity_metadata_wrapper('user', $account);
    $ref_pap_id =$wrapper->field_pap_referral_id->value();
    $rows[] = array(
      'uid' => $account->uid,
      'email' => $account->mail,
      'refer_pap_id' => $ref_pap_id,
    );
  }

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data available.'),
  );

  return $form;
}

/**
 * Form for admin/config/rtpr/redemption/t2/edit/%/list.
 */
function rtpr_cc_redemption_t2_credits_transaction_form($form, $form_state, $account) {
  $header = array(
    'created' => t('Date'),
    'credits' => t('Credits'),
    'type' => t('Type'),
    'log' => t('Log'),
    'update' => t('Entered By'),
  );
  $rows = array();
  $transactions = db_select('rtpr_t2_credits', 't2')
    ->fields('t2')
    ->condition('uid', $account->uid)
    ->orderBy('tid', 'DESC')
    ->execute()->fetchAll();

  foreach($transactions as $transaction) {
    $submitter = user_load($transaction->submitter);
    $rows[$transaction->tid] = array(
      'created' => date('m/d/Y H:i:s', $transaction->created),
      'credits' => $transaction->credits,
      'type' => $transaction->type,
      'log' => $transaction->log,
      'update' => $submitter->mail,
    );
  }
  $form['header'] = array(
    '#type' => 'value',
    '#value' => $header,
  );

  $form['rows'] = array(
    '#type' => 'value',
    '#value' => $rows,
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
 * Bulk change potential customer status.
 */
function rtpr_cc_redemption_t2_bad_emails_form($form, $form_state) {
  $form['helper'] = array(
    '#markup' => t('<strong>Use this form to bulk updating the status of potential customers that have undeliverable email addresses.</strong>'),
  );

  $path = drupal_get_path('module', 'rtpr_cc') . '/includes/bad-emails-sample.csv';

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#description' => t('Upload a import file, allowed extensions: csv. Download a<a href="/@path"> sample </a>file', array('@path' => $path)),
    '#size' => 22,
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),

  );

  return $form;
}

function rtpr_cc_redemption_t2_bad_emails_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    // Validate extensions.
    'file_validate_extensions' => array('csv'),
  ));
  // If the file passed validation:
  if ($file) {
    // Move the file into the Drupal file system.
    if ($file = file_move($file, 'private://')) {
      // Save the file for use in the submit handler.
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function rtpr_cc_redemption_t2_bad_emails_form_submit($form, $form_state) {
  $file = $form_state['storage']['file'];
  $handle = fopen($file->uri, 'r');
  $success = 0;
  $failed = 0;

  fgetcsv($handle, 1024, ",");
  $valid_statuses = array('soft', 'hard');
  while (($data = fgetcsv($handle, 1024, ",")) !== FALSE) {
    $email = $data[0];
    $status = $data[1];
    if (in_array($status, $valid_statuses)) {
      $account = user_load_by_mail($email);
      if ($account) {
        db_update('users')
        ->fields(array(
          'init' => $status,
        ))
        ->condition('uid', $account->uid, '=')
        ->execute();
        $success++;
        continue;
      }
    }
    $failed++;
  }

  drupal_set_message(t('Total imported: @success', array('@success' => $success)));
  drupal_set_message(t('Total failed: @failed', array('@failed' => $failed)), 'error');

}

/**
 * Duplicate Credit Report.
 */
function rtpr_cc_redemption_t2_duplicate_redemption_form($form, $form_state) {
  $header = array(
    'name' => t('Name'),
    'email' => t('Email'),
    'link' => t('Link'),
  );


  $issues = array();
  $fixed = array();
  $query = db_select('userpoints_txn', 'ut')
    ->fields('ut', array('uid', 'txn_id'))
    ->condition('status', 2)
    ->condition('description', 'New customer bonus')
    ->groupBy('uid');
  $query->addExpression('COUNT(txn_id)', 'tcount');

  // Add the HAVING condition
  $query->havingCondition('tcount', 2);

  $results = $query->execute();

  foreach($results as $result) {
    $point = db_query('SELECT points FROM userpoints WHERE uid = :uid', array(':uid' => $result->uid))->fetchField();
    $account = user_load($result->uid);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $row = array(
      'name' => $account_wrapper->field_first_name->value() . ' ' . $account_wrapper->field_last_name->value(),
      'email' => $account->mail,
      'link' => l('View Points', '/user/' . $account->uid . '/points', array('attributes' => array('target' => '_blank'))),
    );

    if ($point > 1500) {
      $issues[] = $row;
    }
    else {
      $fixed[] = $row;
    }
  }

  $form['issue'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $issues,
    '#empty' => t('No data available.'),
  );

  $form['fixed_message'] = array(
    '#markup' => '<h2>Records in following tables are already fixed.</h2>',
  );
  $form['fixed'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $fixed,
    '#empty' => t('No data available.'),
  );

  return $form;
}