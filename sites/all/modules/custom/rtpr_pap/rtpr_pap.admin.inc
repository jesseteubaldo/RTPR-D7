<?php

/**
 * @file
 */

/**
 * Override background_batch_overview_page()
 */
function rtpr_pap_background_process_overview_form($form, $form_state) {
  $data = array();
  $bids = db_select('batch', 'b')
            ->fields('b', array('bid'))
            ->orderBy('b.bid', 'ASC')
            ->execute()
            ->fetchAllKeyed(0, 0);
  require_once DRUPAL_ROOT . '/includes/batch.inc';
  foreach ($bids as $bid) {
    $progress = progress_get_progress('_background_batch:' . $bid);
    $batch = db_query("SELECT batch FROM {batch} WHERE bid = :bid", array(
      ':bid' => $bid,
    ))
    ->fetchField();
    $batch = unserialize($batch);
    $email = db_query("SELECT mail FROM {users} WHERE uid = :uid", array(
      ':uid' => $batch['uid'],
    ))
    ->fetchField();
    if (!$progress) {
      $progress = (object)array(
        'start' => 0,
        'end' => 0,
        'progress' => 0,
        'message' => t('N/A'),
      );
    }

    $eta = progress_estimate_completion($progress);
    $data[] = array(
      $progress->end ? $bid : l($bid, 'batch', array('query' => array('op' => 'start', 'id' => $bid))),
      sprintf("%.2f%%", $progress->progress * 100),
      $progress->message,
      $progress->start ? format_date((int)$progress->start, 'small') : t('N/A'),
      $progress->end ? format_date((int)$progress->end, 'small') : ($eta ? format_date((int)$eta, 'small') : t('N/A')),
      $email,
    );
  }
  $header = array('Batch ID', 'Progress', 'Message', 'Started', 'Finished/ETA', 'UID');

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $data,
    '#empty' => t('No batch found.'),
  );

  $form['bid'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch ID'),
    '#required' => TRUE,
    '#description' => t('Enter batch ID to be cancelled.')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel Batch'),
  );
  return $form;
}

/**
 * Handle batch delete.
 */
function rtpr_pap_background_process_overview_form_submit($form, $form_state) {
  $bid = intval($form_state['values']['bid']);
  if ($bid) {
    $name = 'drupal_batch:' . $bid . ':0';
    $progress = '_background_batch:' . $bid;
    db_delete('batch')->condition('bid', $bid)->execute();
    db_delete('queue')->condition('name', $name)->execute();
    db_delete('progress')->condition('name', $progress)->execute();

    drupal_set_message(t('Batch @bid cancelled.', array('@bid' => $bid)));
  }
}