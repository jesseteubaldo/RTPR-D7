<?php

/**
 * @file
 * Redemption features.
 */


/**
 *
 */
function rtpr_cc_redemption_form($form, &$form_state) {

  if (isset($form_state['storage']['step_two'])) {

    return rtpr_cc_redemption_form_step_two($form_state);
  }

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#required' => TRUE,
    '#autocomplete_path' => 'rtprcc/autocomplete',
  );

  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  return $form;
}

/**
 * Step two.
 */
function rtpr_cc_redemption_form_step_two($form_state) {

  $page_one_values = $form_state['storage']['page_one_values'];
  $new_account = $page_one_values['new_account'];
  $email = $page_one_values['email'];

  $account = user_load_by_mail($email);
  $data = array();
  if ($account) {

    $wrapper = entity_metadata_wrapper('user', $account);

    $data['address'] = $wrapper->field_address->value();
    $data['phone'] = $wrapper->field_phone->value();
    $data['first_name'] = $wrapper->field_first_name->value();
    $data['last_name'] = $wrapper->field_last_name->value();
    $data['referral_id'] = $wrapper->field_pap_referral_id->value();

  }


  $form['#attached']['js'][] = drupal_get_path('module', 'rtpr_cc') . '/js/rtpr_cc.js';

  $form['redemption_id'] = array(
    '#title' => 'Redemption ID',
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic information'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['basic']['email'] = array(
    '#title' => 'Email Address',
    '#disabled' => TRUE,
    '#type' => 'textfield',
    '#default_value' => $email,
    '#required' => TRUE,
  );

  $form['basic']['first_name'] = array(
    '#title' => 'First Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['first_name']) ? $data['first_name'] : '',
  );

  $form['basic']['last_name'] = array(
    '#title' => 'Last Name',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['last_name']) ? $data['last_name'] : '',
  );

  $form['basic']['phone'] = array(
    '#title' => 'Phone',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['phone']) ? $data['phone'] : '',
  );

  $form['basic']['pap_referral_id'] = array(
    '#title' => 'PAP Referral ID',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['referral_id']) ? $data['referral_id'] : '',
  );

  $form['address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Address information'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['address']['address_1'] = array(
    '#title' => 'Address 1',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['address']['thoroughfare']) ? $data['address']['thoroughfare'] : '',

  );

  $form['address']['address_2'] = array(
    '#title' => 'Address 2',
    '#type' => 'textfield',
    '#default_value' => isset($data['address']['premise']) ? $data['address']['premise'] : '',
  );

  $form['address']['city'] = array(
    '#title' => 'City',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['address']['locality']) ? $data['address']['locality'] : '',
  );

  $form['address']['state'] = array(
    '#title' => 'State',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => rtpr_pap_get_state_list(),
    '#default_value' => isset($data['address']['administrative_area']) ? $data['address']['administrative_area'] : '',
  );

  $form['address']['zip'] = array(
    '#title' => 'Zip',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($data['address']['postal_code']) ? $data['address']['postal_code'] : '',
  );

  $form['address']['country'] = array(
    '#title' => 'Country',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => country_get_list(),
    '#default_value' => 'US',
  );

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account information'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['account']['password'] = array(
    '#title' => 'Password',
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => '123456',
  );
  $form['account']['role'] = array(
    '#title' => 'Role',
    '#type' => 'select',
    '#options' => array(
      RTPR_CC_RETAIL_RID => 'Retail Customer',
    ),
    '#default_value' => RTPR_CC_RETAIL_RID,
  );

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product information'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['product']['product_id'] = array(
    '#title' => 'Product',
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => rtpr_cc_redemption_product_list(),
  );

  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm Redemption'),
    '#weight' => 5,
  );

  return $form;
}

function rtpr_cc_redemption_product_list() {
  return array(
    111 => '1.5 oz Pain Cream TUBE',
    124 => '1.4 oz Foot Cream Jar',
    154 => '1.5oz Hand Cream Tube',
    230 => 'MAXX 1.5 oz Tube',
  );
}

/**
 *
 */
function rtpr_cc_redemption_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $email = $values['email'];

  if (!_rtpr_cc_domain_exists($email)) {
    form_set_error('email', t('The email address appears to be invalid.'));
  }

  $account = user_load_by_mail($email);

  if ($account) {
    $form_state['values']['new_account'] = FALSE;
    if (!rtpr_cc_is_retail_customer($account->roles)) {
      form_set_error('email', t('Email address is wholesale and a different email address needs to be used to honor a redemption.'));
    }
    else {
      $has_ordered = rtpr_cc_has_order($account->uid);
      if ($has_ordered) {
        form_set_error('email', t('Email address is in the system with prior orders and needs to be processed manually.'));
      }
    }
  }
  else {
    $form_state['values']['new_account'] = TRUE;
  }

  if ($form_state['clicked_button']['#id'] == 'edit-confirm') {
    $pap_referral_id = $values['pap_referral_id'];
    $is_valid = rtpr_cc_is_referral_id_valid($pap_referral_id);
    if (!$is_valid) {
       form_set_error('pap_referral_id', t('The Referring ID is invalid!'));
    }
  }
}

/**
 *
 */
function rtpr_cc_is_retail_customer($roles) {
  return in_array(RTPR_CC_RETAIL_RID, array_keys($roles));
}

/**
 * Check if the referral id enter match any of affiliate id on the site.
 */
function rtpr_cc_is_referral_id_valid($referral_id) {
  if ($referral_id) {
    $is_valid = db_select('field_data_field_pap_affiliate_id', 'r')
      ->fields('r', array('field_pap_affiliate_id_value'))
      ->condition('field_pap_affiliate_id_value', $referral_id)
      ->execute()
      ->fetchAssoc();
    if ($is_valid) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 *
 */
function rtpr_cc_has_order($uid) {
  $query = db_select('commerce_order', 'o')
    ->fields('o', array('order_id'))
    ->condition('status', array('checkout_complete', 'waiting', 'pending', 'processing', 'completed'), 'IN')
    ->condition('uid', $uid);

  $result = $query->execute()->fetchCol();

  return !empty($result);
}

/**
 *
 */
function rtpr_cc_redemption_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-next') {
    // We set this to determine.
    $form_state['storage']['step_two'] = TRUE;
    // Which elements to display
    // when the page reloads.
    // Values below in the $form_state['storage'] array are saved
    // to carry forward to subsequent pages in the form.
    $form_state['storage']['page_one_values'] = $form_state['values'];
    // Added.
    $form_state["rebuild"] = TRUE;
  }
  // Handle page 2 submissions.
  else {
    $values = $form_state['values'];
    $account_uid  = 0 ;
    if ($values['new_account']) {
      // Create account
      $new_user = array(
        'name' => $values['email'],
        'pass' => $values['password'], // note: do not md5 the password
        'mail' => $values['email'],
        'status' => 1,
        'init' => $values['email'],
        'roles' => array(
          DRUPAL_AUTHENTICATED_RID => 'authenticated user',
          RTPR_CC_RETAIL_RID => 'Retail Customer',
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
      $account->field_pap_referral_id = array(LANGUAGE_NONE => array(array('value' => trim($values['pap_referral_id']))));
      $account = user_save($account);
      $account_uid = $account->uid;
    } else {
      $account = user_load_by_mail($values['email']);
      $account_uid = $account->uid;
    }

    // Create/update address.
    $default_pid=commerce_addressbook_get_default_profile_id($account_uid,'shipping');
    if ($default_pid) {
      // Update existing profile.
      $profile = commerce_customer_profile_load($default_pid);
      $new_address = $profile->commerce_customer_address['und'][0];
      $new_address['country'] = $values['country'];
      $new_address['administrative_area'] = $values['state'];
      $new_address['locality'] = $values['city'];
      $new_address['postal_code'] = $values['zip'];
      $new_address['thoroughfare'] = $values['address_1'];
      $new_address['premise'] = $values['address_2'];
      $new_address['first_name'] = $values['first_name'];
      $new_address['last_name'] = $values['last_name'];
      $new_address['name_line'] = $values['first_name'] . ' ' . $values['last_name'];

      $profile->commerce_customer_address['und'][0] = $new_address;
      $status = commerce_customer_profile_save($profile);
    } else {
      // Create new profile.
      $new_profile = commerce_customer_profile_new('shipping');
      $wrapper = entity_metadata_wrapper('commerce_customer_profile', $new_profile);
      $wrapper->uid = $account_uid;
      $wrapper->commerce_customer_address->country = $values['country'];
      $wrapper->commerce_customer_address->name_line = $values['first_name'] . ' ' . $values['last_name'];
      $wrapper->commerce_customer_address->first_name = $values['first_name'];
      $wrapper->commerce_customer_address->last_name = $values['last_name'];
      $wrapper->commerce_customer_address->administrative_area = $values['state'];
      $wrapper->commerce_customer_address->sub_administrative_area = $sub_administrative_area;
      $wrapper->commerce_customer_address->locality = $values['city'];
      $wrapper->commerce_customer_address->postal_code = $values['zip'];
      $wrapper->commerce_customer_address->thoroughfare = $values['address_1'];
      $wrapper->commerce_customer_address->premise = $values['address_2'];
      commerce_customer_profile_save($new_profile);
      $default_pid = $new_profile->profile_id;
    }

    // Create/update billing address.
    $billing_pid=commerce_addressbook_get_default_profile_id($account_uid,'billing');
    if ($default_pid) {
      // Update existing profile.
      $profile = commerce_customer_profile_load($billing_pid);
      $new_address = $profile->commerce_customer_address['und'][0];
      $new_address['country'] = $values['country'];
      $new_address['administrative_area'] = $values['state'];
      $new_address['locality'] = $values['city'];
      $new_address['postal_code'] = $values['zip'];
      $new_address['thoroughfare'] = $values['address_1'];
      $new_address['premise'] = $values['address_2'];
      $new_address['first_name'] = $values['first_name'];
      $new_address['last_name'] = $values['last_name'];
      $new_address['name_line'] = $values['first_name'] . ' ' . $values['last_name'];

      $profile->commerce_customer_address['und'][0] = $new_address;
      commerce_customer_profile_save($profile);
    } else {
      // Create new profile.
      $new_profile = commerce_customer_profile_new('billing');
      $wrapper = entity_metadata_wrapper('commerce_customer_profile', $new_profile);
      $wrapper->uid = $account_uid;
      $wrapper->commerce_customer_address->country = $values['country'];
      $wrapper->commerce_customer_address->name_line = $values['first_name'] . ' ' . $values['last_name'];
      $wrapper->commerce_customer_address->first_name = $values['first_name'];
      $wrapper->commerce_customer_address->last_name = $values['last_name'];
      $wrapper->commerce_customer_address->administrative_area = $values['state'];
      $wrapper->commerce_customer_address->sub_administrative_area = $sub_administrative_area;
      $wrapper->commerce_customer_address->locality = $values['city'];
      $wrapper->commerce_customer_address->postal_code = $values['zip'];
      $wrapper->commerce_customer_address->thoroughfare = $values['address_1'];
      $wrapper->commerce_customer_address->premise = $values['address_2'];
      commerce_customer_profile_save($new_profile);
      $billing_pid = $new_profile->profile_id;

    }

    $order = commerce_cart_order_load($account_uid);

    if ($order) {
      // Empty the order.
      commerce_cart_order_empty($order);

    } else {
      // Create a new order
      $order = commerce_order_new($account_uid, 'cart');
    }
    $redemption_id = $values['redemption_id'];
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    $products = array(RTPR_CC_FREE_PRODUCT_ID, $values['product_id']);
    foreach($products as $product_id){
      $product = commerce_product_load($product_id);
      $line_item = commerce_product_line_item_new($product, 1, $order->order_id);
      commerce_line_item_save($line_item);
      $order_wrapper->commerce_line_items[] = $line_item;
    }
    $order_wrapper->commerce_customer_billing = $billing_pid;
    $order_wrapper->commerce_customer_shipping = $default_pid;

    // Add comments
    $order_wrapper->field_order_comments = 'Your Rebate Order for Redemption ID# ' . $redemption_id . ' has been processed and there is NO Charge to you for this order. If you would like to place additional orders in the future, we have setup an online account for you. Simply go to www.RTPR.com and login using your email address ' . $values['email'] . ' and the password 123456 (which can be changed once you login), or you can call our toll-free line at 877-787-7180 and give them your email address.';
    // Agree to TOC
    $order_wrapper->field_agreement = TRUE;

    if (user_has_role(RTPR_CC_SHIPPER_RID)) {
      $order_wrapper->status = 'processing';
      $order->log = t('Processed by redemption shipping.');
    } else {
      // Change status to Confirmed
      $order_wrapper->status = 'pending';
    }

    commerce_order_save($order);

    drupal_set_message(t('Successfully create <a target="_blank" href="/admin/commerce/orders/@id">order #@id</a>', array('@id' => $order->order_id)));
    commerce_checkout_complete($order);

    // Add merchant note to transaction.
    rtpr_cc_add_merchant_note($order->order_id, $redemption_id);
  }
}

function rtpr_cc_add_merchant_note($order_id, $note) {
  db_set_active('pap');
  $query = db_select('qu_pap_transactions', 't')
    ->fields('t', array('transid'))
    ->condition('orderid', $order_id);

  $results = $query->execute()->fetchCol();

  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();

  // loading a transaction with exact id e.g.:  e367f77a
  $sale = new Pap_Api_Transaction($session);

  foreach($results as $transid) {
    $sale->setTransId($transid);
    if ($sale->load()) {
      $sale->setMerchantNote($note);
      $sale->save();
    }
  }

  db_set_active();
}
