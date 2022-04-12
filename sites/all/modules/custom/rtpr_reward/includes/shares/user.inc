<?php

/**
 * @file
 * User page for Co-Op Shares
 */
function rtpr_reward_user_coop_form($form, $form_state, $uid) {
  $stats = _rtpr_reward_user_coop_stats($uid);

  $form['summary']['#markup'] = $stats['summary'];
  $form['coop_status']['#markup'] = $stats['coop_status'];

  $results = db_query("SELECT month, year FROM rtpr_reward WHERE point_type = 'coop_share' AND uid = :uid GROUP BY month, year Order by year DESC, month DESC", array(':uid' => $uid))->fetchAll();
  $rows = array();
  foreach($results as $date) {
    $month = $date->month;
    $year = $date->year;
    $rows[] = array(
      $month,
      $year,
      rtpr_reward_points_available($uid, 'coop_share', $month, $year),
      l('view', 'user/' . $uid . '/coop/' . $month . '/' . $year)
    );
  }

  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'total' => t('Total'),
    'view' => t('view'),
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );

  return $form;
}

/**
 * Helper function to get coop stats.
 */
function _rtpr_reward_user_coop_stats($uid) {
  $orders_id = db_query("SELECT order_id FROM commerce_line_item WHERE line_item_label LIKE '%PROMOTIONAL-CO-OP-KIT'")->fetchCol();
  $account = user_load($uid);

  $account_wrapper = entity_metadata_wrapper('user', $account);
  $account_status = $account_wrapper->field_coop_status->value();
  $orders = commerce_order_load_multiple($orders_id, array('uid' => $uid, 'status' => array('checkout_complete', 'pending', 'completed', 'processing', 'waiting')));
  $shares = rtpr_reward_points_earned($uid, 'coop_share');
  $shares += rtpr_reward_points_earned($uid, 'coop_share', 1);
  $status = _rtpr_reward_coop_status($shares, $account_status);

  $after_2019 = rtpr_reward_points_earned_after($uid, 'coop_share', 1577858400);
  $shares_2019 = $shares - $after_2019;
  $status_2019 = _rtpr_reward_coop_status($shares_2019, $account_status);

  $summary = '<p><strong>Qualification Points Tracking toward your 2021 Status: <span class="underline">' . $shares . '</span></strong></p>';
  $summary .= '<p>2020 Club Status (Achieved in 2019) <strong>' . $status_2019['current'] .'</strong>.</p>';

  if ($shares) {
/*
    if ($shares >= 6) {
      $coop_status = '<p>You have achieved <strong>' . $status['current'] .'</strong> for <strong>' . date('Y') . '</strong> and you are only <strong>' . $status['shares_needed'] . '</strong> Qualification Points away from the <strong>' . $status['next'] .
        '</strong> Level. <a class="underline" href="https://rtpr.com/media/Brand-Ambassador-Clubs.pdf" target="_blank">Click Here to Learn More</a></p>';
    }
*/
    if ($shares >= 100) {
      //$coop_status = '<p>Congratulations you have achieved the Brand Ambassador <strong>' . $status['current'] .'</strong> Level.</p>';
    }
  }
  else {
    if (!empty($orders) && !$account_status) {
      $coop_status = '<p>You have achieved the BRONZE Brand Ambassador Club level, your next Club Level is SILVER, <a class="underline" href="https://rtpr.com/media/Brand-Ambassador-Clubs.pdf" target="_blank">Click Here to Learn More. Club Levels are updated in each month.</a></p>';
    }
    else {
      $coop_status = '<p>You are not in the Brand Ambassador Co-Op Program. <a class="underline" href="https://rtpr.com/media/Brand-Ambassador-Clubs.pdf" target="_blank">Click Here to Learn More about this amazing program.</a></p>';
    }
  }

  if ($account_status) {
    $coop_status = '<p>You have achieved <strong>' . $status['current'] .'</strong> for <strong>' . date('Y') . '</strong> and you are only <strong>' . $status['shares_needed'] . '</strong> Qualification Points away from the <strong>' . $status['next'] .
        '</strong> Level. <a class="underline" href="https://rtpr.com/media/Brand-Ambassador-Clubs.pdf" target="_blank">Click Here to Learn More</a></p>';
  }

  return array(
    'summary' => $summary,
    'coop_status' => $coop_status,
  );
}

function rtpr_reward_user_coop_month_year_form($form, &$form_state, $uid, $month, $year) {
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'shares' => t('Shares'),
  );

  $results = db_query("SELECT * FROM rtpr_reward WHERE point_type = 'coop_share' AND uid = :uid AND month = :month AND year = :year ", array(':uid' => $uid, ':month' => $month, ':year' => $year))->fetchAll();
  $rows = array();
  foreach($results as $result) {
    $rows[] = array(
      $result->month,
      $result->year,
      $result->comment,
      $result->point
    );
  }


   $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );

  return $form;
}

/**
 * Helper function to determine the status of Co-Op.
 */
function _rtpr_reward_coop_status($shares, $account_status = NULL) {
  $field_info = field_info_field('field_coop_status');
  $allowed_values = $field_info['settings']['allowed_values'];
  $coop_status = array(
    100 => 'diamond',
    60 => 'ruby',
    48 => 'sapphire',
    36 => 'emerald',
    24 => 'platinum',
    12 => 'gold',
    6 => 'silver',
    1 => 'bronze',
  );

  $return = array();
  foreach ($coop_status as $share => $status) {
    if ($shares >= $share || $status == $account_status) {
      $return['current'] = $allowed_values[$status];
      break;
    }
    $return['next'] = $allowed_values[$status];
    $return['shares_needed'] = $share - $shares;
  }
  return $return;
}