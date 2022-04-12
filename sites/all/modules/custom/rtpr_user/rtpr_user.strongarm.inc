<?php
/**
 * @file
 * rtpr_user.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function rtpr_user_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_avatax_install_time';
  $strongarm->value = 1427252817.426578;
  $export['commerce_avatax_install_time'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_avatax_validate_address';
  $strongarm->value = FALSE;
  $export['commerce_avatax_validate_address'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_affiliate_auto_login';
  $strongarm->value = 1;
  $export['commerce_post_affiliate_pro_affiliate_auto_login'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_post_affiliate_pro_affiliate_auto_login_roles';
  $strongarm->value = array(
    8 => '8',
    9 => '9',
    10 => '10',
    11 => '11',
    4 => '4',
    22 => '22',
    23 => '23',
    24 => '24',
  );
  $export['commerce_post_affiliate_pro_affiliate_auto_login_roles'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'commerce_reports_tax_batch_finished';
  $strongarm->value = 1427298778;
  $export['commerce_reports_tax_batch_finished'] = $strongarm;

  return $export;
}
