<?php
/**
 * @file
 * rtpr_commerce.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function rtpr_commerce_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_affiliate_activated';
  $strongarm->value = '0';
  $export['commerce_post_affiliate_pro_affiliate_activated'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_merchant_id';
  $strongarm->value = 'admin@rtpr.com';
  $export['commerce_post_affiliate_pro_merchant_id'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_merchant_password';
  $strongarm->value = 'XSW@3edc';
  $export['commerce_post_affiliate_pro_merchant_password'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_merchant_url_prefix';
  $strongarm->value = 'http://rtprtest.com/affiliate/';
  $export['commerce_post_affiliate_pro_merchant_url_prefix'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_merchant_username';
  $strongarm->value = 'admin@rtpr.com';
  $export['commerce_post_affiliate_pro_merchant_username'] = $strongarm;

  return $export;
}
