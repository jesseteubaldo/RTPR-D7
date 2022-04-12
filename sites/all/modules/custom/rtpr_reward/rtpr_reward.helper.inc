<?php

/**
 * @file
 */

function _rtpr_reward_get_referral_count($pap_id) {
  $count = 0;
  db_set_active('pap');
  if ($pap_id) {
    $children_ids = db_query('SELECT userid FROM qu_pap_users WHERE parentuserid = :parentuserid', array(':parentuserid' => $pap_id))->fetchCol();
    if (!empty($children_ids)) {
      db_set_active();
      $uids = db_query("SELECT entity_id FROM field_data_field_pap_affiliate_id WHERE field_pap_affiliate_id_value in (:children_ids)", array(':children_ids' => $children_ids))->fetchCol();
      if (!empty($uids)) {
        $user_ids = db_query("select distinct uid from commerce_order as co, commerce_line_item as cli where co.order_id = cli.order_id and line_item_label LIKE '%PROMOTIONAL-CO-OP-KIT%' and status in ('processing', 'waiting', 'pending', 'checkout_complete', 'completed') AND uid IN (:uids)", array(':uids' => $uids))->fetchCol();
        $count = count($user_ids);
      }
    }
  }
  db_set_active();
  return $count;
}
