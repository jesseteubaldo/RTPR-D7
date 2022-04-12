<?php

/**
 * @file
 */

/**
 * Form callback for user/%user/ppe.
 */
function rtpr_user_ppe_form($form, $form_state, $account) {
  $total = 0;
  $list = array();
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = NULL;
  try {
    $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_extras',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
  if ($pap_id) {
    db_set_active('pap');
    for ($tier = 2; $tier <= 8; $tier++) {
      $count = db_query('
        SELECT count(DISTINCT data2) FROM qu_pap_transactions WHERE userid=:pap_id
        AND dateinserted > :date AND productid IN (:skus) AND tier = :tier',
        array(
          ':pap_id' => $pap_id,
          ':date' => '2020-08-15',
          ':skus' => array('PPE-PROGRAM-STARTER-KIT', 'INTERNATIONAL-PPE-PROGRAM-STARTER-KIT', 'PPE-PROGRAM-DIGITAL-KIT', 'PPE-PROGRAM-UPGRADE-KIT'),
          ':tier' => $tier
        )
      )->fetchField();
      if ($tier == 2) {
        $key = '<strong>Personal</strong> PPE Referrals';
      }
      else {
        $key = '<strong>Generation ' . ($tier - 1) .'</strong> PPE Referrals';
        
      }
      $list[0]['children'][] = $key . ' = ' . $count;
      $total+=$count;
    }
    db_set_active();
  }
  $list[0]['data'] = '<h5><strong>Total Organization PPE Referrals = ' . $total . '</strong></h5>';

  $form['total'] = array(
    '#theme' => 'item_list',
    '#items' => $list,
  );
  return $form;
} 