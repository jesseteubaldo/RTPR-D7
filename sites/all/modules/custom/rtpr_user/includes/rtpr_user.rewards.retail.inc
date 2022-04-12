<?php
/**
 * @file
 * Retail Rewards Program.
 */

/**
 * Callback form for /rewards-program.
 */
function _rtpr_user_retail_rewards_program_submit($option) {
  global $user;
  $rids = array_keys($user->roles);


  if (!$user->uid) {
    if ($option === 'decline') {
      drupal_goto('/');
    }
    user_cookie_save(array('rewards_program'=>TRUE));
    drupal_goto('user');
  }
  if (!in_array(7, $rids)) {
    drupal_not_found();
  }
  if ($user->uid && !in_array(48, $rids)) {

    $account_wrapper = entity_metadata_wrapper('user', $user);
  
    $statuses = $account_wrapper->field_account_status->value();
  
    $valid_status = array(
      'join' => 'joined_retail_rewards',
      'decline' => 'declined_retail_rewards'
    );
  
    if (isset($valid_status[$option])) {
      $statuses[] = $valid_status[$option];
      $account_wrapper->field_account_status->set($statuses);
      if ($option == 'join') {
        $account_wrapper->field_account_status->set(array('joined_retail_rewards'));
        $rids[] = 48;
        $account_wrapper->roles = $rids;
        drupal_set_message(t('Congratulations, your account is now activated in the RTPR Rewards Program.'));
        $account_wrapper->save();

        if (in_array($user->uid, variable_get('rtpr_cc_retail_rewards_exception_uids', array()))) {
/*
          $params = array(
            'points' => 1500,
            'uid' => $user->uid,
            'operation' => 'admin',
            'description' => 'New Retail Rewards Bonus',
            'tid' => 0,
            'time_stamp' => time(),
            'moderate' => FALSE,
            'approver_uid' => 1,
            'expirydate' => strtotime("3 months")
          );
          userpoints_userpointsapi($params);
          drupal_set_message(t('Welcome! Your account has been activated and you have been issued a $15 in Online Gift Card Credit to use for any Product in our Shop, <strong><a href="@link">Click Here</a> to Shop Now!</strong>', array('@link' => '/shop')));
*/
        }
        drupal_goto('user/' . $user->uid .'/retail-rewards-itemization');
      } else if ($option == 'decline') {
        $account_wrapper->save();
        drupal_goto('backoffice/rewards-program-declined');
      }

    }
  }
  drupal_not_found();
}