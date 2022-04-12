<?php
  
/**
 * @file
 *
 * Import points from csv file.
 */
 
function rtpr_reward_import_form($form, $form_state) {
  
  drupal_set_title(t('RTPR Points - Import'));

  
  $form['group'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => rtpr_reward_point_types(),
    '#default_value' => 'reward',
  );
  
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#description' => t('Upload a import file, allowed extensions: csv'),
    '#title_display' => 'invisible',
    '#size' => 22,
  );
  
  $form['comment'] = array(
    '#type' => 'textarea',
    '#title' => 'Comment',  
  );
  
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    
  );
  
  return $form;
}

function rtpr_reward_import_form_validate($form, &$form_state) {
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

function rtpr_reward_import_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $group = $values['group'];
  $comment = $values['comment'];

  
  $batch_number = variable_get('rtpr_import_batch_number', 0);
  $file = $form_state['storage']['file'];
  $handle = fopen($file->uri, 'r');
  $success = 0;
  $failed = 0;
  
  fgetcsv($handle, 1024, ",");
  while (($data = fgetcsv($handle, 1024, ",")) !== FALSE) {
      $wrong_role = FALSE;
    $email = $data[0];
    $point = floatval(str_replace('$', '', $data[1]));
    $month = intval($data[2]);
    $year = intval($data[3]);
    if(isset($data[4])) {
      $comment = $data[4];
    }
    $account = user_load_by_mail($email);
    if (!$account) {
      $account = user_load($email);
      // Co-Op Share only for reward role.
      if ($group == 'coop_share') {
        if (!in_array(47, array_keys($account->roles))) {
          $wrong_role = TRUE;
        }
      }
      if ($group == 'tpl') {
        if (!in_array(49, array_keys($account->roles))) {
          $wrong_role = TRUE;
        }  
      }

      
    }
    if($account && $point && $month && $year && !$wrong_role) {
      $record = array(
        'uid' => $account->uid,
        'batch' => $batch_number,
        'email' => $account->mail,
        'point' => $point,
        'month' => $month,
        'year' => $year,
        'timestamp' => REQUEST_TIME,
        'type' => intval($point) > 0 ? 'Earned' : 'Redeem',
        'comment' => !empty($comment) ? $comment : 'Imported on ' . date('m/d/Y H:i a'),
        'point_type' => $group,
      );
      rtpr_reward_insert($record);
      $success++;
    }  else {
      if(!$account){
        drupal_set_message(t('No user with email @email exist!', array('@email' => $email)), 'warning');
      } else if (!$point) {
        drupal_set_message(t('Point is empty for user with email @email!', array('@email' => $email)), 'warning');
      } else if (!$month) {
        drupal_set_message(t('Month is empty for user with email @email!', array('@email' => $email)), 'warning');
      } else if (!$year) {
        drupal_set_message(t('Year is empty for user with email @email!', array('@email' => $email)), 'warning');
      } else if ($wrong_role) {
        drupal_set_message(t('Account @email does not have required role for imported point type <strong>@point_type</strong>!', array('@email' => $email, '@point_type' => $group)), 'warning');
      }
      
      $failed++;
    }
    
  }
  fclose($handle);
  
  drupal_set_message(t('Total imported: @success', array('@success' => $success)));
  drupal_set_message(t('Total failed: @failed', array('@failed' => $failed)), 'error');
  
  variable_set('rtpr_import_batch_number', $batch_number+1);
  
}
