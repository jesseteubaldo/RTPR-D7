<?php

/**
 * @file
 * Provides Inline Conditions integration for the RTPR User module.
 */

/**
 * Implements hook_inline_conditions_info().
 */
function rtpr_user_inline_conditions_info() {
  $conditions = array();

  if (module_exists('commerce_order')) {
    $conditions['rtpr_user_check_pmsp'] = array(
      'label' => t('User PMSP'),
      'entity type' => 'commerce_order',
      'callbacks' => array(
        'configure' => 'rtpr_user_check_pmsp_configure',
        'build' => 'rtpr_user_check_pmsp_build',
      ),
    );
  }
  
  return $conditions;
}

/**
 * Configuration callback for rtpr_user_user_pmsp.
 *
 * @param array $settings
 *   An array of rules condition settings.
 *
 * @return array
 *   A form element array.
 */
function rtpr_user_check_pmsp_configure($settings) {
  $form = array();

  // Ensure we've default settings set.
  $settings += array(
    'pmsp' => 0,
    'operator' => '>',
    'date' => date('m/Y'),
  );
  
  module_load_include('inc', 'commerce_discount', 'commerce_discount.rules');
  $form['operator'] = array(
    '#type' => 'select',
    '#title' => t('PMSP'),
    '#title_display' => 'invisible',
    '#options' => _commerce_discount_operator_options(),
    '#required' => TRUE,
    '#default_value' => !empty($settings['operator']) ? $settings['operator'] : '>',
  );
  $form['pmsp'] = array(
    '#type' => 'textfield',
    '#title' => t('in'),
    '#title_display' => 'after',
    '#default_value' => !empty($settings['pmsp']) ? $settings['pmsp'] : 0,
    '#required' => TRUE,
    '#size' => 15, 
  );
  $form['date'] = array(
    '#title' => t('Date'),
    '#title_display' => 'invisible',
    '#type' => 'date_popup',
    '#date_year_range' => '-3:+1',
    '#date_label_position' => 'within',
    '#date_format' => 'm/Y',
    '#default_value' => !empty($settings['date']) ? $settings['date'] : date('m/Y'),
    '#required' => TRUE,
  );

  return $form;
}
