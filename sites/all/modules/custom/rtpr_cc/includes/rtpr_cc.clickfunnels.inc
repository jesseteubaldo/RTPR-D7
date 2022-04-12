<?php

/**
 * @file
 * Features related to click funnels.
 */

function rtpr_cc_click_funnels_form($form, &$form_state) {
  $options = array();
  $dir = '/home/rtprcom/clickfunnels';
  $files = scandir($dir);
  foreach ($files as $file) {
    if (strpos($file, 'csv') !== FALSE) {

      $options[$dir . '/' . $file] = $file;
    }
  }
  $form['files'] = array(
    '#title' => t('Files'),
    '#type' => 'select',
    '#options' => $options,
    '#required' => TRUE,
    '#size'=> 300,
              '#maxlength'=> 255,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

function rtpr_cc_click_funnels_form_submit($form, &$form_state) {
  $file_url = $form_state['values']['files'];
  header('Content-Type: application/octet-stream');
  header("Content-Transfer-Encoding: Binary");
  header("Content-disposition: attachment; filename=\"" . basename($file_url) . "\"");
  readfile($file_url);
  exit;
}