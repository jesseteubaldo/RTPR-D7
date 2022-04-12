<?php

/**
 * Helpful functions.
 */

function _rtpr_pap_get_month_options() {
  return array(
    '01' => t('January'),
    '02' => t('February'),
    '03' => t('March'),
    '04' => t('April'),
    '05' => t('May'),
    '06' => t('June'),
    '07' => t('July'),
    '08' => t('August'),
    '09' => t('September'),
    '10' => t('October'),
    '11' => t('November'),
    '12' => t('December'),
  );
}

function _rtpr_pap_get_year_options($start_year = 2015) {
  $current_year = date('Y');
  $options = array();
  if ($start_year == $current_year) {
    return array($current_year => $current_year);
  }
  
  for ($start_year; $start_year <= $current_year; $start_year++) {
    $options[] = $start_year;
  }

  return array_combine($options, $options);

}

function _rtpr_pap_get_gcc_customer_uids($uid) {
  $list = array();
  $uids = db_select('rtpr_gcc', 't')
    ->fields('t', array('uid'))
    ->condition('parent_uid', $uid)
    ->execute()->fetchCol();
  foreach($uids as $uid) {
    $list[] = str_replace('GCC', '', $uid);
  }

  return $list;
}

function _rtpr_pap_is_valid_pap_id($pap_id) {
  $uid = db_select('field_data_field_pap_affiliate_id', 'pai')
  ->fields('pai', array('entity_id'))
  ->condition('field_pap_affiliate_id_value', $pap_id)
  ->execute()->fetchField();

  return $uid;
}