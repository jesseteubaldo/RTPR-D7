<?php
/**
 * @file
 */

/**
 * Implements hook_rules_action_info().
 */
function rtpr_extras_rules_action_info() {
  $actions = array();
  
  $actions['rtpr_extras_remove_role_by_id'] = array(
    'label' => t('Remove user role by rid'),
    'parameter' => array(
      'number' => array(
        'type' => 'integer',
        'label' => t('Role ID'),
        'description' => t('The rid to be removed from user.'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      )
    ),
    'group' => t('User'),
  );
  
  $actions['rtpr_extras_add_role_by_id'] = array(
    'label' => t('Add user role by rid'),
    'parameter' => array(
      'number' => array(
        'type' => 'integer',
        'label' => t('Role ID'),
        'description' => t('The rid to be added to user.'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      )
    ),
    'group' => t('User'),
  );
  
  return $actions;
}