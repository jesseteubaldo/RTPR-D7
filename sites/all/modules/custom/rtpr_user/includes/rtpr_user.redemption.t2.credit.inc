<?php
  
/**
 * @file
 * File for user redemption credits.
 */

function rtpr_user_redemption_credit_form($form, $form_states, $account) {
  $uid = $account->uid;
  $total = _rtpr_user_get_total_redemption_credits($uid);
  $used = _rtpr_user_get_used_redemption_credits($uid);
  $available = _rtpr_user_get_available_redemption_credits($uid);
  $form['total'] = array(
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#markup' => 'Total Redemption Credits Acquired = ' . $total,
  );

  $form['used'] = array(
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#markup' => 'Total Redemption Credits Used = ' . $used,
  );

  $form['available'] = array(
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#markup' => 'Total Remaining Redemption Credits Available = ' . $available,
  );

  return $form;
}