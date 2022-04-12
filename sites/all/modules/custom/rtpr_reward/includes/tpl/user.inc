<?php
  
/**
 * @file
 *
 * User reward page.
 */

// Summary form
function rtpr_rewards_tpl_user_summary_form($form, &$form_state, $uid) {
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Points'),
  );
  $form_state['uid'] = $uid;
  $form_state['header'] = $header;
  $form['summary']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => rtpr_reward_get_rows($uid, array('tpl')),
    '#empty' => t('No available data.'),
  );
  
  return $form;
}
