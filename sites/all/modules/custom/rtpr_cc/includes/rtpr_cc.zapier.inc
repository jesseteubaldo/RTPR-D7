<?php

/**
 * @file
 * Features to Zapier
 */

function rtpr_cc_resend_zapier_form($form, &$form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Resend Email'),
  );

  return $form;
}

function rtpr_cc_resend_zapier_form_validate($form, &$form_state) {
  $email = $form_state['values']['email'];

  if (!user_load_by_mail($email)) {
    form_set_error('email', t('No user matches the email!'));
  }
}

function rtpr_cc_resend_zapier_form_submit($form, &$form_state) {
  $email = $form_state['values']['email'];
  module_load_include('inc', 'rtpr_user', 'services/rtpr_user.services');
  _rtpr_user_services_zapier_email(array('Email' => $email));
  drupal_set_message(t('Successfully resend email!'));
}