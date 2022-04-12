<?php

/**
 * @file
 * Call Center features related to Retail role.
 */
function rtpr_cc_retail_rewards_exceptions_import_form($form, $form_state) {
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#description' => t('Upload a import file, allowed extensions: csv'),
    '#title_display' => 'invisible',
    '#size' => 22,
  );
  
  $form['overwrite'] = array(
    '#type' => 'checkbox',
    '#title' => t('Assign points to user even if they already have points.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  

/*
  $uids = variable_get('rtpr_cc_retail_rewards_exception_uids', array());
  unset($uids[26590]);
  variable_set('rtpr_cc_retail_rewards_exception_uids', array_unique($uids));
  
*/
  return $form;
}

function rtpr_cc_retail_rewards_exceptions_import_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    // Validate extensions.
    'file_validate_extensions' => array('csv'),
  ));
  // If the file passed validation:
  if ($file) {
    // Move the file into the Drupal file system.
    $imports_dir = 'public://imports/'; 
    file_prepare_directory($imports_dir, FILE_CREATE_DIRECTORY);
    if ($file = file_move($file, $imports_dir . $file->filename)) {
      // Save the file for use in the submit handler.
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function rtpr_cc_retail_rewards_exceptions_import_form_submit($form, &$form_state) {
  $file = $form_state['storage']['file'];
  $handle = fopen($file->uri, 'r');
  $success = 0;
  $failed = 0;
  
  fgetcsv($handle, 1024, ",");
  $uids = variable_get('rtpr_cc_retail_rewards_exception_uids', array());
  $before = count($uids);
  while (($data = fgetcsv($handle, 1024, ",")) !== FALSE) {
    $uid = $data[0];
    $credit = rtpr_reward_get_userpoints_txn($uid);
    if ($form_state['values']['overwrite'] || !$credit->total) {
      $uids[] = $uid;  
    }
    else {
      drupal_set_message(t('User <a href="/user/@uid/points" target="_blank">@uid</a> already have points!', array('@uid' => $uid, '@uid' => $uid)), 'warning');
    }

  }
  $after_count = count($uids);
  variable_set('rtpr_cc_retail_rewards_exception_uids', array_unique($uids));
  
  if ($after_count - $before > 0) {
    drupal_set_message(t('Successfully import @count uids.', array('@count' => $after_count - $before)));
  }
  
}