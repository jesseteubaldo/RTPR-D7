<?php

/**
 * @file
 * Custom files for RTPR support.
 */

function rtpr_support_rules_action_info() {
  $actions = array();
  $actions['rtpr_support_update_ticket_client'] = array(
    'label' => t('Update ticket client.'),
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Created ticket'),
      ),
    ),
    'group' => t('Support'),
  );
  return $actions;
}

/**
 * Rules action: Update ticket client
 */
function rtpr_support_update_ticket_client($node) {
  if(isset($node->uid)) {
    $account = user_load($node->uid);
    $clid = rtpr_support_get_department_id($account);
    $num_updated = db_update('support_ticket') // Table name no longer needs {}
    ->fields(array(
    'client' => $clid,
    ))
    ->condition('nid', $node->nid, '=')
    ->execute();
    field_cache_clear();

  }
}