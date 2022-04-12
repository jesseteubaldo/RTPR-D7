<?php
  
/**
 * @file
 *
 * User reward page.
 */

// Summary form
function rtpr_reward_user_afl_summary_form($form, $form_state, $uid) {
  $total = rtpr_ebp_get_total($uid, 'afl');
  $form['message'] = array(
    '#markup' => '<h4><strong>Total Contribution to your AFL = ' . ($total). '</strong></h4>',
  );
  $account = user_load($uid);
  $wrapper = entity_metadata_wrapper('user', $account);
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Points'),
  );
  
  $rows = rtpr_ebp_get_rows($uid, array('afl'));
  $form['divider'] = array(
    '#markup' => '<hr class="dm"></hr>',
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );
  
  return $form;
}
// Summary form
function rtpr_reward_user_summary_form($form, $form_state, $uid) {
  $header = array(
    'total' => t('Total Points Earned'),
    'user' => t('Total Used Points'),
    'remain' => t('Total Remaining Points'),
  );
  $rows[] = array(
    'total' => rtpr_reward_points_earned($uid),
    'user' => abs(rtpr_reward_points_used($uid)),
    'remain' => rtpr_reward_points_available($uid)
  );
  
  $form['divider'] = array(
    '#markup' => '<hr class="dm"></hr>',
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );
  
  $form['divider2'] = array(
    '#markup' => '<hr class="dm"></hr>',
  );
  
  $form['request'] = array(
    '#markup' => t('<p>Click <a href="@link" target="_blank">here</a> to submit a redemption request.</p>', array('@link' => '/tickets/new')),
  );
  
  return $form;
}

// Points earned history
function rtpr_reward_earned_history_form($form, $form_state, $uid) {
  return rtpr_reward_history_form($uid);
}

// Points redeem history
function rtpr_reward_redeem_history_form($form, $form_state, $uid) {
  return rtpr_reward_history_form($uid, 'redeem');
}


function rtpr_reward_history_form($uid, $type = 'earned') {
  $rows = array();
  $per_page = 10;

  $header = array(
    'id' => t('ID'),
    'point' => t('Points'),
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'timestamp' => t('Date'),
  );
  
  $results = db_select('rtpr_reward','r')
  ->fields('r', array_keys($header))
  ->condition('type', $type, '=')
  ->condition('point_type', 'reward', '=')
  ->condition('uid', $uid, '=')
  ->orderBy('id', 'DESC')
  ->execute();
  
  foreach($results as $result) {
    $result->timestamp = date('m/d/Y H:i a', $result->timestamp);
    $rows[] = (array)$result;
  }
  
  $current_page = pager_default_initialize(count($rows), $per_page);
  // split array into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);
  
  $form['divider'] = array(
    '#markup' => '<hr class="dm"></hr>',
  );
  
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => isset($chunks[$current_page]) ? $chunks[$current_page] : array(),
    '#empty' => t('No history available.'),
  );
  
  $form['pager'] = array(
    '#theme' => 'pager',
  );
  
  return $form;

}