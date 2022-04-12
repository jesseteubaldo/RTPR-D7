<?php

/**
 * Implements hook_rules_event_info().
 */
function rtpr_offer_rules_event_info() {
  $events = array();

  $events['rtpr_offer_commerce_line_update'] = array(
    'label' => t('After update a product in the cart'),
    'group' => t('Commerce Cart'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Shopping cart order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function rtpr_offer_rules_condition_info() {
  $conditions = array();

  $conditions['rtpr_offer_first_time_buyer'] = array(
    'label' => t('User is first time buyer'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
    ),
    'group' => t('RTPR'),
    'access callback' => 'rules_user_integration_access',
    'base' => 'rtpr_offer_first_time_buyer',
  );
  
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function rtpr_offer_rules_action_info() {
  $actions = array();
  
  $actions['rtpr_offer_product_remove_by_sku'] = array(
    'label' => t('Remove a product from the cart'),
    'parameter' => array(
      'string' => array(
        'type' => 'text',
        'label' => t('Product SKU'),
        'description' => t('The SKU of the product to remove from the cart.'),
      ),
    ),
    'group' => t('Commerce Cart'),
  );
  
  return $actions;
}