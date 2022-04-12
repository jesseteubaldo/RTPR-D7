<?php

/**
 * @file
 * GCC Virtual
 */

/**
 * GCC Virtual Activation form.
 */
function rtpr_offer_virtual_gcc_activation_form($form, &$form_state) {

  if (isset($_GET['submitted'])) {
    return;
  }
  $form['#attributes'] = array('class' => 'activation-form');
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'rtpr_cc') . '/includes/js/rtpr_cc.js',
  );
  $form['message_intro'] = array(
    '#markup' => '<div><h2>Virtual Gift Card Code</h2><p>If you have received a Real Time Pain Relief Gift Card, please use the activation code displayed on the
card to claim your Virtual Gift Card, and follow the instructions.</p></div>',
  );

  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter Virtual Activation Code'),
    '#description' => t('Virtual Gifts are valid for product during online purchases only'),
    '#required' => TRUE,
  );

  $form['activate'] = array(
    '#type' => 'submit',
    '#value' => t('Activate'),
    '#attributes' => array('class' => array('btn-primary activation-code')),
/*
    '#ajax' => array(

      'callback' => 'rtpr_offer_virtual_gcc_activation_code_callback',
      'wrapper' => 'register-form',
      
    ),
    '#prefix' => '<div class="activate">',
    '#suffix' => '</div>',
*/
  );

/*
  $form['register'] = array(
    '#prefix' => '<div id="register-form">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
  );

  $register = drupal_get_form('user_register_form');

  if (!empty($form_state['valid'])) {
    unset($register['account']['mail']['#value']);
    unset($register['account']['pass']['#value']);
    $form['register']['mail'] = $register['account']['mail'];
    $form['register']['pass'] = $register['account']['pass'];
    $form['register']['field_first_name'] = $register['field_first_name'];
    $form['register']['field_last_name'] = $register['field_last_name'];
    $form['register']['phone'] = $register['field_phone'];
    $form['register']['field_address'] = $register['field_address'];

    _rtpr_offer_recursive_unset($form['register'], '#value');
    $form['register']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#weight' => 100,
      '#submit' => array('rtpr_offer_virtual_gcc_activation_register'),
      '#validate' => array('rtpr_offer_virtual_gcc_activation_validate'),
    );

  }
*/

  return $form;
}

function rtpr_offer_virtual_gcc_activation_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $pap_id = trim($values['code']);
  
  $form_state['redirect'] = 'gc-request/' . $pap_id;
}
/**
 * Helper function to recursively unset a key in an array.
 */
function _rtpr_offer_recursive_unset(&$array, $unwanted_key) {
  unset($array[$unwanted_key]);
  foreach ($array as &$value) {
    if (is_array($value)) {
      _rtpr_offer_recursive_unset($value, $unwanted_key);
    }
  }
}

/**
 * Ajax callback to validate the activation code.
 */
function rtpr_offer_virtual_gcc_activation_code_callback($form, &$form_state) {
  $code =  trim($form_state['values']['code']);  

  if ($code){
    return $form['register'];
  }

}

function ajax_example_add_more_add_one($form, &$form_state) {
  $code =  trim($form_state['values']['code']);  
  $result = db_select('field_data_field_pap_affiliate_id', 'pai')
  ->fields('pai', array('entity_id'))
  ->condition('field_pap_affiliate_id_value', $code)
  ->execute()->fetchField();
  if ($result){
    $form_state['valid'] = TRUE;
    $form_state['pid'] = $result;

  } else {
    form_set_error('code', t('Invalid activation code!'));
  }
    $form_state['rebuild'] = TRUE;

  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('.activate', 'test'),
    ),
  );
}
function rtpr_offer_virtual_gcc_activation_validate($form, $form_state) {
  $values = $form_state['values'];
  $username = $values['mail'];
  if (user_load_by_mail($username)) {
    form_set_error('mail', t('This offer is only available for new customer!'));
  }
  if (!valid_email_address($username)) {
    form_set_error('mail', t('Invalid email address!'));
  }
}
function rtpr_offer_virtual_gcc_activation_register($form, &$form_state) {
  $values = $form_state['values'];
  $pap_id = trim($values['code']);
  $address = $values['field_address'][LANGUAGE_NONE][0];
  $username = $values['mail'];
  if (user_load_by_mail($username)) {
    
    return;
  }
  $password = $values['pass'];
  $first_name = $values['field_first_name'][LANGUAGE_NONE][0]['value'];;
  $last_name = $values['field_last_name'][LANGUAGE_NONE][0]['value'];;
  $phone = $values['field_phone'][LANGUAGE_NONE][0]['value'];
  $pap_id = trim($values['code']);
  $fields = array(
    'name' => $username,
    'mail' => $username,
    'pass' => $password,
    'status' => 1,
    'init' => $username,
    'roles' => array(
      DRUPAL_AUTHENTICATED_RID => TRUE,
      7 => TRUE,
    ),
    'field_pap_referral_id' => array(LANGUAGE_NONE => array(array('value' => $pap_id))),
    'field_first_name' => array(LANGUAGE_NONE => array(array('value' => $first_name))),
    'field_last_name' => array(LANGUAGE_NONE => array(array('value' => $last_name))),
    'field_phone' => array(LANGUAGE_NONE => array(array('value' => $phone))),
    'field_address' => array(LANGUAGE_NONE => array(array(
      'country' => $address['country'],
      'administrative_area' => $address['administrative_area'],
      'locality' => $address['locality'],
      'postal_code' => $address['postal_code'],
      'thoroughfare' => $address['thoroughfare'],
      'premise' => $address['premise'],
    ))),
  );

  //the first parameter is left blank so a new user is created
  $account = user_save('', $fields);
  if ($account) {
    // Issue $15 credits
    $params = array(
      'points' => 1500,
      'uid' => $account->uid,
      'operation' => 'admin',
      'description' => 'New Virtual GCC bonus',
      'tid' => 0,
      'time_stamp' => time(),
      'moderate' => FALSE,
      'approver_uid' => 1,
      'expirydate' => strtotime("3 months")
    );
    userpoints_userpointsapi($params);

    $account_wrapper = entity_metadata_wrapper('user', $account);
    $account_wrapper->field_pap_referral_id->set($pap_id);
    $account_wrapper->field_customer_referral_id->set();
    $account_wrapper->save();
    
    $vendor = user_load($form_state['pid']);
    drupal_set_message(t('Please check <strong>@email</strong> and verify your email address to complete your Gift Card Activation.', array('@email' => $account->mail)), 'warning');
    rules_invoke_event('rtpr_cc_notify_event', $vendor, 'virtual_gcc', $account, 'virtual_gcc');
    drupal_goto('activate/submitted', array('query' => array('submitted' => 'true')));
  }

}
