<?php

/**
 * @file
 * Allow vendor to submit GCC.
 */
function rtpr_user_gcc_submission_form($form, &$form_state, $account) {
  $roles = array_keys($account->roles);
  $diff = array_intersect($roles, array(22, 43, 44));

  module_load_include('inc', 'rtpr_cc', 'includes/rtpr_cc.redemption.t2.admin');
  $wrapper = entity_metadata_wrapper('user', $account);
  $credits = _rtpr_user_get_available_redemption_credits($account->uid);
  if (!$credits) {
    $form['message'] = array(
      '#markup' =>   t('You need Gift Card Credits to submit Gift Card Receipts. To get credits, you can go to
the GCS Shop by <a href="@gcs" target="_blank" class="bold underline">Clicking Here</a> or if you would like to
learn more about the Gift Card Program and Process, <a href="@link2" target="_blank" class="bold underline">Click Here</a>', array('@gcs' => '/gcs-products', '@link2' => '/backoffice/customer-acquisition-gift-card-program')),
      '#prefix' => '<div class="alert alert-danger message">',
      '#suffix' => '</div>',
    );
    return $form;
  }


  $pap_id = $wrapper->field_pap_affiliate_id->value();
  $form_state['pap_id'] = $pap_id;
  $form_state['uid'] = $account->uid;
  $form += rtpr_cc_redemption_t2_form_step_two($form, $form_state);
  return $form;
}

/**
 * Validate GCC Submission.
 */
function rtpr_user_gcc_submission_form_validate($form, &$form_state) {
  $email = $form_state['values']['email'];
  if (!valid_email_address($email)) {
    form_set_error('mail', t('Invalid email address!'));
  }

  $account = user_load_by_mail($email);
  if ($account) {
    form_set_error('email', t('Email already exist in the system!'));
  }

}

function rtpr_user_gcc_submission_form_submit($form, &$form_state) {
  module_load_include('inc', 'rtpr_cc', 'includes/rtpr_cc.redemption.t2.admin');
  rtpr_cc_redemption_t2_form_submit($form, $form_state);
  $values = $form_state['values'];

  drupal_set_message(t('Gift Card Receipt Processed for @first @last', array(
    '@first' => $values['first_name'],
    '@last' => $values['last_name'],
  )));
}
