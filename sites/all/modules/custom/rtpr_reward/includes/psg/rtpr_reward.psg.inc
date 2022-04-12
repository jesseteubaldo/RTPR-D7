<?php

/**
 * @file
 * Report relate to psg.
 */


/**
 * Callback for user/%/psg/earned.
 */
function rtpr_reward_user_psg_earmed_summary_form($form, &$form_state, $uid) {
  $items = array(
        '#markup' => t(
      'You have earned a total of @personal_psg Personal PSG Points and a total of @bonus_psg Bonus PSG Points.',
      array(
        '@personal_psg' => rtpr_reward_points_earned($uid, 'personal_psg'),
        '@bonus_psg' => rtpr_reward_points_earned($uid, 'bonus_psg')
      )
    )
  );

  $form['summary'] = array(
    '#title' => t('PSG Summary'),
    '#theme' => 'item_list',
    '#items' => $items,
  );
  

  
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Points'),
    'point_type' => t('Type'),
  );
  
  $rows = rtpr_rewards_get_rows($uid, array('personal_psg', 'bonus_psg'));
  $form['divider'] = array(
    '#markup' => '<hr class="dm"></hr>',
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
 * Callback for admin/config/rtpr/points/reports/psg.
 */
function rtpr_reward_reports_psg_form($form, &$form_state, $point_type) {
  $form_state['point_type'] = $point_type;

  $type = 'Personal';
  if ($point_type != 'personal_psg') {
    $type = 'Bonus';
  }
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download @type PSG report', array('@type' => $type)),
    '#submit' => array('rtpr_reward_reports_psg_form_submit'),
  );
  
  return $form;
  
}

/**
 * Download PSG.
 */
function rtpr_reward_reports_psg_form_submit($form, &$form_state) {
  $uids = db_query(
    'SELECT * FROM rtpr_reward WHERE point_type = :point_type ORDER BY insertdate',
    array(':point_type' => $form_state['point_type'])
  )->fetchAllAssoc('uid');

  $rows = array();
  if ($uids) {
    $months = array();
    foreach ($uids as $row) {
      $month_year = $row->month . '/' . $row->year;
      $months[$month_year] = $month_year;
    }

    foreach ($uids as $row) {
      $operations[] = array(
        'rtpr_reward_reports_psg_batch_op',
        array(
          $row,
          $months
        ),
      );
    }

  
    $batch = array(
      'operations' => $operations,
      'finished' => 'rtpr_reward_reports_psg_finished',
      'file' => drupal_get_path('module', 'rtpr_reward') . '/includes/psg/rtpr_reward.psg.inc',
    );

    batch_set($batch);
  }
}

/**
 * Process each user.
 */
function rtpr_reward_reports_psg_batch_op($row_data, $months, &$context) {
  $uid = $row_data->uid;
  $row = array(
    'type' => $row_data->point_type,
    'uid' => $uid,
    'email' => $row_data->email,
    'total_psg_earned' => rtpr_reward_points_earned($uid, $row_data->point_type),

  );
  
  foreach ($months as $month_year) {
    list($month, $year) = explode('/', $month_year);
    $row[$month_year] = rtpr_reward_points_earned_for_month_year($uid, $row_data->point_type, $month, $year);
  }

  $context['message'] = t('Processing user %$uid', array('%$uid' => $uid));
  $context['results'][$uid] = $row;
  $context['months'] = $months;
}

/**
 * End of batch.
 */
function rtpr_reward_reports_psg_finished($success, $rows, $operations) {
  $first_row = reset($rows);
  $type = 'Personal';
  if ($first_row['type'] != 'personal_psg') {
    $type = 'Bonus';
  }
  $header = array(
    'uid' => t('UID'),
    'email' => t('Email'),
    'total_psg_earned' => t('Total @type PSG Earned', array('@type' => $type)),
  );
  
  unset($first_row['uid']);
  unset($first_row['email']);
  unset($first_row['total_psg_earned']);
  unset($first_row['type']);
  $header += array_keys($first_row);




  $file_path = 'public://private/' . date('m_d_Y', time()) . '_psg_report.csv';
  $fp = fopen($file_path, 'w');
  
  
  fputcsv($fp, $header);

  foreach($rows as $row){
    unset($row['type']);
    fputcsv($fp, $row);
  }

  fclose($fp);
  $download_path = file_create_url($file_path);
  $message = t('Click <a href="@download_path">here</a> to download the report.', array('@download_path' => $download_path));
  drupal_set_message($message);
}
