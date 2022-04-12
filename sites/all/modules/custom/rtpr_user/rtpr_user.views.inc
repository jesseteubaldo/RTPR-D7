<?php

/**
 * @file
 * Views file for rtpr_user.
 */


/**
 * Implements hook_views_data_alter().
 */
function rtpr_user_views_data_alter(&$data) {
  // mail
  // Note that this field implements field level access control.
  $data['users']['init'] = array(
    'title' => t('Init E-mail'), // The item it appears as on the UI,
    'help' => t('Initial Email address for a given user. This field is normally not shown to users, so be cautious when using it.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_user_mail',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
}