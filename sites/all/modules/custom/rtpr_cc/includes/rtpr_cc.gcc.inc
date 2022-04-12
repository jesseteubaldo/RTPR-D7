<?php

/**
 * @file GCC related helper functions.
 */


/**
 * Get user GCA.
 */
function _rtpr_cc_gca_total_count($uid, $start_timestamp, $end_timestamp) {
  $uids = db_select('rtpr_gcc', 't')
    ->fields('t', array('uid'))
    ->condition('parent_uid', $uid)
    ->execute()->fetchCol();

  module_load_include('inc','rtpr_user','includes/rtpr_user.redemption.t2.user');
  $gca_count = 0;
  foreach($uids as $uid) {
    $gca = _rtpr_cc_get_gca_date($uid);
    if ($gca >= $start_timestamp && $gca <= $end_timestamp) {
      $gca_count++;
    }
  }

  return $gca_count;
}

function _rtpr_cc_get_gca_date($uid) {
  $or = db_or();
  $or->condition('description', 'New customer bonus');
  $or->condition('description', 'New Virtual GCC bonus');

  $query = db_select('userpoints_txn', 'p');
  $query->join('users', 'u', 'u.uid = p.uid');
  $query->fields('p', array('time_stamp'))
    ->condition('p.uid', $uid)
    ->condition($or)
    ->condition('init', 'unsubscribe', '!=');
  $created = $query->execute()->fetchField();
  $gca = NULL;
  if ($created) {
    $gca = $created;
  }
  return $gca;
}

function _rtpr_cc_gcs_total_count($uid, $start_timestamp, $end_timestamp) {
  $uids = db_select('rtpr_gcc', 't')
    ->fields('t', array('uid'))
    ->condition('parent_uid', $uid)
    ->execute()->fetchCol();
  module_load_include('inc','rtpr_user','includes/rtpr_user.redemption.t2.user');
  $gcs_count = 0;
  $custmomers = user_load_multiple($uids);
  foreach($custmomers as $custmomer) {
   if ($custmomer->created >= $start_timestamp && $custmomer->created <= $end_timestamp) {
      $gcs_count++;
    }
  }

  return $gcs_count;
}