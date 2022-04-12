<?php
/**
 * @file
 * rtpr_commerce.features.inc
 */

/**
 * Implements hook_commerce_flat_rate_default_services().
 */
function rtpr_commerce_commerce_flat_rate_default_services() {
  $items = array(
    '_100_flat_rate' => array(
      'name' => '_100_flat_rate',
      'base' => '_100_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__100_flat_rate',
      'weight' => 1,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_100_flat_rate_details_form',
        'details_form_validate' => '_100_flat_rate_details_form_validate',
        'details_form_submit' => '_100_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$100 Flat rate',
      'base_rate' => array(
        'amount' => 10000,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_15_flat_rate' => array(
      'name' => '_15_flat_rate',
      'base' => '_15_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__15_flat_rate',
      'weight' => 2,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_15_flat_rate_details_form',
        'details_form_validate' => '_15_flat_rate_details_form_validate',
        'details_form_submit' => '_15_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$15 Flat rate',
      'base_rate' => array(
        'amount' => 1500,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_20_flat_rate' => array(
      'name' => '_20_flat_rate',
      'base' => '_20_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__20_flat_rate',
      'weight' => 3,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_20_flat_rate_details_form',
        'details_form_validate' => '_20_flat_rate_details_form_validate',
        'details_form_submit' => '_20_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$20 Flat rate',
      'base_rate' => array(
        'amount' => 2000,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_35_flat_rate' => array(
      'name' => '_35_flat_rate',
      'base' => '_35_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__35_flat_rate',
      'weight' => 4,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_35_flat_rate_details_form',
        'details_form_validate' => '_35_flat_rate_details_form_validate',
        'details_form_submit' => '_35_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$35 Flat rate',
      'base_rate' => array(
        'amount' => 3500,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_40_flat_rate' => array(
      'name' => '_40_flat_rate',
      'base' => '_40_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__40_flat_rate',
      'weight' => 5,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_40_flat_rate_details_form',
        'details_form_validate' => '_40_flat_rate_details_form_validate',
        'details_form_submit' => '_40_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$40 Flat rate',
      'base_rate' => array(
        'amount' => 4000,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_4_95_flat_rate' => array(
      'name' => '_4_95_flat_rate',
      'base' => '_4_95_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__4_95_flat_rate',
      'weight' => 6,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_4_95_flat_rate_details_form',
        'details_form_validate' => '_4_95_flat_rate_details_form_validate',
        'details_form_submit' => '_4_95_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$4.95 Flat rate',
      'base_rate' => array(
        'amount' => 495,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_50_flat_rate' => array(
      'name' => '_50_flat_rate',
      'base' => '_50_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__50_flat_rate',
      'weight' => 7,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_50_flat_rate_details_form',
        'details_form_validate' => '_50_flat_rate_details_form_validate',
        'details_form_submit' => '_50_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$50 Flat rate',
      'base_rate' => array(
        'amount' => 5000,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    '_70_flat_rate' => array(
      'name' => '_70_flat_rate',
      'base' => '_70_flat_rate',
      'display_title' => 'Flat rate shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate__70_flat_rate',
      'weight' => 8,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => '_70_flat_rate_details_form',
        'details_form_validate' => '_70_flat_rate_details_form_validate',
        'details_form_submit' => '_70_flat_rate_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => '$70 Flat rate',
      'base_rate' => array(
        'amount' => 7000,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
    'free_shipping' => array(
      'name' => 'free_shipping',
      'base' => 'free_shipping',
      'display_title' => 'Free shipping',
      'description' => '',
      'shipping_method' => 'flat_rate',
      'rules_component' => TRUE,
      'price_component' => 'flat_rate_free_shipping',
      'weight' => 0,
      'callbacks' => array(
        'rate' => 'commerce_flat_rate_service_rate_order',
        'details_form' => 'free_shipping_details_form',
        'details_form_validate' => 'free_shipping_details_form_validate',
        'details_form_submit' => 'free_shipping_details_form_submit',
      ),
      'module' => 'commerce_flat_rate',
      'title' => 'Free shipping',
      'base_rate' => array(
        'amount' => 0,
        'currency_code' => 'USD',
        'data' => array(),
      ),
      'data' => array(),
      'admin_list' => TRUE,
      'is_features' => TRUE,
    ),
  );
  return $items;
}

/**
 * Implements hook_commerce_tax_default_types().
 */
function rtpr_commerce_commerce_tax_default_types() {
  $items = array(
    'sales_tax' => array(
      'name' => 'sales_tax',
      'display_title' => 'Sales tax',
      'description' => 'A basic type for taxes that do not display inclusive with product prices.',
      'display_inclusive' => 0,
      'round_mode' => 0,
      'rule' => 'commerce_tax_type_sales_tax',
      'module' => 'commerce_tax_ui',
      'title' => 'Sales tax',
      'admin_list' => TRUE,
    ),
    'vat' => array(
      'name' => 'vat',
      'display_title' => 'VAT',
      'description' => 'A basic type for taxes that display inclusive with product prices.',
      'display_inclusive' => 1,
      'round_mode' => 1,
      'rule' => 'commerce_tax_type_vat',
      'module' => 'commerce_tax_ui',
      'title' => 'VAT',
      'admin_list' => TRUE,
    ),
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function rtpr_commerce_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function rtpr_commerce_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
