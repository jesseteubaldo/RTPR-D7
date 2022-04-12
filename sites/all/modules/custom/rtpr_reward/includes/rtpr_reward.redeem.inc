<?php
  
/**
 * @file
 *
 * Allow admin/staff to redem point for user
 */

/**
 * Returns the list of steps and their associated forms.
 */
function _rtpr_reward_redeem_steps() {
  return array(
    1 => array(
      'form' => 'rtpr_reward_redeem_step_one',
    ),
    2 => array(
      'form' => 'rtpr_reward_redeem_step_two',
    )
  );
}

function rtpr_reward_redeem_form($form, &$form_state) {

  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = _rtpr_reward_redeem_steps();
  }
  $step = &$form_state['step'];
  drupal_set_title(t('Redeem Points: Step @step', array('@step' => $step)));

  // Call the function named in $form_state['step_information'] to get the
  // form elements to display for this step.
  $form = $form_state['step_information'][$step]['form']($form, $form_state);


  if ($step > 1) {
    $form['prev'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#name' => 'prev',
      '#submit' => array('rtpr_reward_redeem_previous_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  // Show the Next button only if there are more steps defined.
  if ($step < count($form_state['step_information'])) {
    // The Next button should be included on every step.
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#name' => 'next',
      '#submit' => array('rtpr_reward_redeem_next_submit'),
    );
  }
  else {
    // Just in case there are no more steps, we use the default submit handler
    // of the form wizard. Call this button Finish, Submit, or whatever you
    // want to show. When this button is clicked, the
    // form_example_wizard_submit handler will be called.
    $form['finish'] = array(
      '#type' => 'submit',
      '#value' => t('Finish'),
    );
  }

  // Include each validation function defined for the different steps.
  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    $form['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
  }

  return $form;
}

/**
 * Submit handler for the "previous" button.
 */
function rtpr_reward_redeem_previous_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];
  if ($current_step > 1) {
    $current_step--;
    $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
  }
  $form_state['rebuild'] = TRUE;
}


/**
 * Submit handler for the 'next' button.
 */
function rtpr_reward_redeem_next_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  if ($current_step < count($form_state['step_information'])) {
    $current_step++;
    if (!empty($form_state['step_information'][$current_step]['stored_values'])) {
      $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    // Force rebuild with next step.
    $form_state['rebuild'] = TRUE;
    return;
  }
}


/**
 * Redeem form submit handler.
 */
function rtpr_reward_redeem_form_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $values = $form_state['values'];
  $record = array(
    'email' => $values['email'],
    'uid' => $values['uid'],
    'month' => $values['month'],
    'year' => $values['year'],
    'point' => 0 - intval($values['point']),
    'comment' => $values['comment'],
    'type' => 'Redeem',
    'timestamp' => REQUEST_TIME,
    'point_type' => $values['point_type'],
    'manual' => 1,
  );
  rtpr_reward_insert($record);

  drupal_set_message(t('Successfully redeem point for user @email', array('@email' => $values['email'])));
}


function rtpr_reward_redeem_step_one($form, &$form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#autocomplete_path' => 'rtprcc/autocomplete',
    '#required' => TRUE,
  );

  $form['point_type'] = array(
    '#title' => t('Point type'),
    '#type' => 'select',
    '#options' =>  array('mrc' => t('MRC'), 'sap' => t('SAP')),
    '#default_value' => 'reward',
    '#required' => TRUE,
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  
  return $form;
}
function rtpr_reward_redeem_step_one_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  $email = $values['email'];
  $account = user_load_by_mail($email);
  if (!$account) {
    form_set_error('email', t('User @email does not exist!', array('@email' => $email)));
  } else {
    $form_state['values']['uid'] = $account->uid;
  }
}
function rtpr_reward_redeem_step_two($form, &$form_state) {
  $values = $form_state['step_information'][1]['stored_values'];

  $email = $values['email'];
  $uid = $values['uid'];
  $point_type = $values['point_type'];
  $available_point = rtpr_reward_points_available($uid, $point_type);
  $options = array();
  
  for($point = 1; $point <= $available_point; $point++) {
    $options[$point] = $point;
  }
  
  $form['email'] = array(
    '#type' => 'value',
    '#value' => $email,
  );
  
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $uid,
  );
  
  $form['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => rtpr_reward_month_list(),
    '#default_value' => date('n'),
  );
  
  $form['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => rtpr_reward_year_list(),
    '#default_value' => date('Y'),
  );

  $form['point_type'] = array(
    '#type' => 'value',
    '#value' =>  $point_type,
  );

  $form['point'] = array(
    '#title' => t('Point cost'),
    '#type' => 'select',
    '#options' =>  $options,
    '#required' => TRUE,
  );
  
  $form['comment'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#required' => TRUE,
  );
  
  return $form;
}