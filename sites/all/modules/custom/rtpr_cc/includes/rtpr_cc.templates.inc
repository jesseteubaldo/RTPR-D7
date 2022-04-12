<?php

/**
 * Various templates.
 */
function rtpr_cc_templates_form($form, $form_states) {
  $form['#attached']['css'][] = drupal_get_path('module', 'rtpr_cc') . '/includes/css/templates.css';

/*
  $form['primary'] = array(
    '#type' => 'fieldset',
    '#title' => t('Good/Better/Best Promotion'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['primary']['rtpr_commerce_primary_offer_a_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Good Offer Title'),
    '#default_value' => variable_get('rtpr_commerce_primary_offer_a_modal_title', ''),
    '#weight' => 16,
  );
  $default_value = variable_get('rtpr_commerce_primary_offer_a_modal_body', NULL);
  $form['primary']['rtpr_commerce_primary_offer_a_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Good Offer Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 17,
  );
  $form['primary']['rtpr_commerce_primary_offer_b_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Better/Best Offer Title'),
    '#default_value' => variable_get('rtpr_commerce_primary_offer_b_modal_title', ''),
    '#weight' => 18,
  );
  $default_value = variable_get('rtpr_commerce_primary_offer_b_modal_body', NULL);
  $form['primary']['rtpr_commerce_primary_offer_b_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Better/Best Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 19,
  );
*/
  
  $form['promotion']['rtpr_commerce_rewards_promotion_free_product_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Rewards Promotion'),
    '#default_value' => variable_get('rtpr_commerce_rewards_promotion_free_product_modal_title', 'Rewards Promotion'),
    '#weight' => 14,
  );

  $default_value = variable_get('rtpr_commerce_rewards_promotion_free_product_modal_body', NULL);
  $form['promotion']['rtpr_commerce_rewards_promotion_free_product_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Rewards Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 15,
  );
/*
  
  $form['buck'] = array(
    '#type' => 'fieldset',
    '#title' => t('Buck settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['buck']['rtpr_commerce_buck_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Buck Modal Title'),
    '#default_value' => variable_get('rtpr_commerce_buck_modal_title', 'Best Offer'),
    '#weight' => 0,
  );

  $default_value = variable_get('ALTERNATE-PAIN-RELIEF-OFFER', NULL);
  $form['buck']['ALTERNATE-PAIN-RELIEF-OFFER'] = array(
    '#type' => 'text_format',
    '#title' => t('ALTERNATE-PAIN-RELIEF-OFFER'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 1,
  );

  $default_value = variable_get('BETTER-PAIN-RELIEF-OFFER', NULL);
  $form['buck']['BETTER-PAIN-RELIEF-OFFER'] = array(
    '#type' => 'text_format',
    '#title' => t('BETTER-PAIN-RELIEF-OFFER'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 2,
  );

  $default_value = variable_get('BEST-PAIN-RELIEF-OFFER', NULL);
  $form['buck']['BEST-PAIN-RELIEF-OFFER'] = array(
    '#type' => 'text_format',
    '#title' => t('BEST-PAIN-RELIEF-OFFER'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 3,
  );

  $form['buck']['rtpr_commerce_free_buck_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Free Product Promotion Title'),
    '#default_value' => variable_get('rtpr_commerce_free_buck_modal_title', 'Free Product'),
    '#weight' => 10,
  );

  $default_value = variable_get('rtpr_commerce_free_buck_modal_body', NULL);
  $form['buck']['rtpr_commerce_free_buck_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Free Product Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 11,
  );
*/
  $form['introductory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Introductory'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['introductory']['rtpr_commerce_introductory_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Great Confirmation Modal title'),
    '#default_value' => variable_get('rtpr_commerce_introductory_modal_title', 'Great Confirmation Modal'),
    '#weight' => 22,
  );
  $default_value = variable_get('rtpr_commerce_introductory_modal_body', NULL);
  $form['introductory']['rtpr_commerce_introductory_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Great Confirmation Modal Body'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 23,
  );

  $form['introductory']['rtpr_commerce_introductory_add_on_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Better/Best Add-on Modal title'),
    '#default_value' => variable_get('rtpr_commerce_introductory_add_on_modal_title', 'Better/Best Add-on Modal'),
    '#weight' => 24,
  );

  $default_value = variable_get('rtpr_commerce_introductory_add_on_modal_body', NULL);
  $form['introductory']['rtpr_commerce_introductory_add_on_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Better/Best Add-on Modal Body'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 25,
  );

  $form['introductory']['rtpr_commerce_introductory_alternate_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate Modal title'),
    '#default_value' => variable_get('rtpr_commerce_introductory_alternate_modal_title', 'Alternate Modal'),
    '#weight' => 26,
  );

  $default_value = variable_get('rtpr_commerce_introductory_alternate_modal_body', NULL);
  $form['introductory']['rtpr_commerce_introductory_alternate_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Alternate Modal Body'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 27,
  );

  $form['promotion'] = array(
    '#type' => 'fieldset',
    '#title' => t('Promotions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['promotion']['rtpr_commerce_affiliate_promotion_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate Promotion'),
    '#default_value' => variable_get('rtpr_commerce_affiliate_promotion_modal_title', 'Affiliate Promotion'),
    '#weight' => 12,
  );
  $default_value = variable_get('rtpr_commerce_affiliate_promotion_modal_body', NULL);
  $form['promotion']['rtpr_commerce_affiliate_promotion_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Affiliate Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 13,
  );

  $form['promotion']['rtpr_commerce_rewards_promotion_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Rewards Promotion'),
    '#default_value' => variable_get('rtpr_commerce_rewards_promotion_modal_title', 'Rewards Promotion'),
    '#weight' => 14,
  );

  $default_value = variable_get('rtpr_commerce_rewards_promotion_modal_body', NULL);
  $form['promotion']['rtpr_commerce_rewards_promotion_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Rewards Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 15,
  );
/*
  $form['brand'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brand Ambassador'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $default_value = variable_get('rtpr_user_rewards_activate_ba', NULL);
  $form['brand']['rtpr_user_rewards_activate_ba'] = array(
    '#type' => 'text_format',
    '#title' => t('Brand Ambassador Activate'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 16,
  );

  $form['gopak'] = array(
    '#type' => 'fieldset',
    '#title' => t('GoPak'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['gopak']['rtpr_commerce_gopak_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('GoPak Promotion Title'),
    '#default_value' => variable_get('rtpr_commerce_gopak_modal_title', 'Special Offe'),
    '#weight' => 16,
  );

  $default_value = variable_get('rtpr_commerce_gopak_modal_body', NULL);
  $form['gopak']['rtpr_commerce_gopak_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('GoPak Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 17,
  );
*/ 
  $form['retail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Retail Promotion'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['retail']['rtpr_commerce_retail_modal_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Retail Promotion Title'),
    '#default_value' => variable_get('rtpr_commerce_retail_modal_title', ''),
    '#weight' => 16,
  );
  $default_value = variable_get('rtpr_commerce_retail_modal_body', NULL);
  $form['retail']['rtpr_commerce_retail_modal_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Retail Promotion Message'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 17,
  );

  $default_value = variable_get('rtpr_extra_tos', NULL);
  $form['rtpr_extra_tos'] = array(
    '#type' => 'text_format',
    '#title' => t('Terms Agreement'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 17,
  );
  
  $form['agreement'] = array(
    '#type' => 'fieldset',
    '#title' => t('Agreement'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $default_value = variable_get('rtpr_cc_template_agreement_affiliate', NULL);
  $form['agreement']['rtpr_cc_template_agreement_affiliate'] = array(
    '#type' => 'text_format',
    '#title' => t('Affiliate'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 20,
  );
  $default_value = variable_get('rtpr_cc_template_agreement_rcp', NULL);
  $form['agreement']['rtpr_cc_template_agreement_rcp'] = array(
    '#type' => 'text_format',
    '#title' => t('RCP'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 25,
  );
  $default_value = variable_get('rtpr_cc_template_agreement_rewards', NULL);
  $form['agreement']['rtpr_cc_template_agreement_rewards'] = array(
    '#type' => 'text_format',
    '#title' => t('Rewards'),
    '#default_value' => $default_value ? $default_value['value'] : '',
    '#format' => 'full_html',
    '#weight' => 30,
  );
  return system_settings_form($form);
}