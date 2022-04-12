<?php

/**
 * @file
 *
 * Features related to Rewards role upgrade.
 */

define('RTPR_USER_BRAND_AMBASSADOR_RID', 50);
define('RTPR_USER_BA_PLUS_RID', 51);
define('RTPR_USER_BA_BUSINESS_RID', 52);

/**
 * Rewards Brand Ambassador upgrade form.
 */
function rtpr_user_activate_ba_form($form, $form_state) {
  global $user;
  
  if ($user->uid) {
    if (!array_intersect(array(RTPR_USER_BRAND_AMBASSADOR_RID, RTPR_USER_BA_PLUS_RID, RTPR_USER_BA_BUSINESS_RID), array_keys($user->roles))) {
      $message = variable_get('rtpr_user_rewards_activate_ba');
      $form['message'] = array(
        '#markup' => $message['value'],
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Activate My Brand Ambassador Status'),
        '#attributes' => array(
          'class' => array(
            'btn-primary',
          )
        )
      );
    } else {
      drupal_set_message(t('You are already activated into the Brand Ambassador Program'), 'warning');
    }
  }
  
  return $form;
}

function rtpr_user_activate_ba_form_submit($form, $form_state) {
  global $user;
  
  if ($user->uid) {
    $role = user_role_load(RTPR_USER_BRAND_AMBASSADOR_RID);
    user_multiple_role_edit(array($user->uid), 'add_role', RTPR_USER_BRAND_AMBASSADOR_RID);
    drupal_set_message(t('Welcome to Brand Ambassador Program!'));
    drupal_goto('ba-activated');
  }
}