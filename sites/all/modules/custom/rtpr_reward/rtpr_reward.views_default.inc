<?php
/**
 * @file
 * rtpr_reward.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function rtpr_reward_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'rtpr_reward';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'rtpr_reward';
  $view->human_name = 'RTPR Reward';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'RTPR Reward';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    5 => '5',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: RTPR Reward: User ID */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'rtpr_reward';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: PAP Affiliate ID */
  $handler->display->display_options['fields']['field_pap_affiliate_id']['id'] = 'field_pap_affiliate_id';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['table'] = 'field_data_field_pap_affiliate_id';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['field'] = 'field_pap_affiliate_id';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['label'] = 'PAP ID';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['element_type'] = '0';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_pap_affiliate_id']['field_api_classes'] = TRUE;
  /* Field: RTPR Reward: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: RTPR Reward: Batch */
  $handler->display->display_options['fields']['batch']['id'] = 'batch';
  $handler->display->display_options['fields']['batch']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['batch']['field'] = 'batch';
  $handler->display->display_options['fields']['batch']['label'] = 'Batch #';
  /* Field: RTPR Reward: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  /* Field: RTPR Reward: Point Type */
  $handler->display->display_options['fields']['point_type']['id'] = 'point_type';
  $handler->display->display_options['fields']['point_type']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['point_type']['field'] = 'point_type';
  /* Field: RTPR Reward: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: RTPR Reward: Point */
  $handler->display->display_options['fields']['point']['id'] = 'point';
  $handler->display->display_options['fields']['point']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['point']['field'] = 'point';
  /* Field: RTPR Reward: Month */
  $handler->display->display_options['fields']['month']['id'] = 'month';
  $handler->display->display_options['fields']['month']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['month']['field'] = 'month';
  /* Field: RTPR Reward: Year */
  $handler->display->display_options['fields']['year']['id'] = 'year';
  $handler->display->display_options['fields']['year']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['year']['field'] = 'year';
  $handler->display->display_options['fields']['year']['separator'] = '';
  /* Field: RTPR Reward: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  /* Field: RTPR Reward: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'rtpr_reward';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'medium';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['timestamp']['format_date_sql'] = 0;
  /* Sort criterion: RTPR Reward: ID */
  $handler->display->display_options['sorts']['id']['id'] = 'id';
  $handler->display->display_options['sorts']['id']['table'] = 'rtpr_reward';
  $handler->display->display_options['sorts']['id']['field'] = 'id';
  $handler->display->display_options['sorts']['id']['order'] = 'DESC';
  /* Filter criterion: RTPR Reward: Point Type */
  $handler->display->display_options['filters']['point_type']['id'] = 'point_type';
  $handler->display->display_options['filters']['point_type']['table'] = 'rtpr_reward';
  $handler->display->display_options['filters']['point_type']['field'] = 'point_type';
  $handler->display->display_options['filters']['point_type']['group'] = 1;
  $handler->display->display_options['filters']['point_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['point_type']['expose']['operator_id'] = 'point_type_op';
  $handler->display->display_options['filters']['point_type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['point_type']['expose']['operator'] = 'point_type_op';
  $handler->display->display_options['filters']['point_type']['expose']['identifier'] = 'point_type';
  $handler->display->display_options['filters']['point_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    3 => 0,
    4 => 0,
    22 => 0,
    20 => 0,
    25 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
  );
  $handler->display->display_options['filters']['point_type']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['point_type']['group_info']['label'] = 'Point Type';
  $handler->display->display_options['filters']['point_type']['group_info']['identifier'] = 'point_type';
  $handler->display->display_options['filters']['point_type']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Reward',
      'operator' => '=',
      'value' => 'reward',
    ),
    2 => array(
      'title' => 'EBP',
      'operator' => '=',
      'value' => 'ebp',
    ),
    3 => array(
      'title' => 'Super Bonus Points',
      'operator' => '=',
      'value' => 'super_bonus',
    ),
    4 => array(
      'title' => 'Multiplication Factor Points',
      'operator' => '=',
      'value' => 'multi_factor',
    ),
    5 => array(
      'title' => 'Co-Op',
      'operator' => '=',
      'value' => 'coop',
    ),
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    3 => 0,
    4 => 0,
    22 => 0,
    20 => 0,
    25 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
  );
  /* Filter criterion: RTPR Reward: Batch */
  $handler->display->display_options['filters']['batch']['id'] = 'batch';
  $handler->display->display_options['filters']['batch']['table'] = 'rtpr_reward';
  $handler->display->display_options['filters']['batch']['field'] = 'batch';
  $handler->display->display_options['filters']['batch']['group'] = 1;
  $handler->display->display_options['filters']['batch']['exposed'] = TRUE;
  $handler->display->display_options['filters']['batch']['expose']['operator_id'] = 'batch_op';
  $handler->display->display_options['filters']['batch']['expose']['label'] = 'Batch #';
  $handler->display->display_options['filters']['batch']['expose']['operator'] = 'batch_op';
  $handler->display->display_options['filters']['batch']['expose']['identifier'] = 'batch';
  $handler->display->display_options['filters']['batch']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    3 => 0,
    4 => 0,
    22 => 0,
    20 => 0,
    25 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
  );
  /* Filter criterion: Date: Date (rtpr_reward) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'rtpr_reward';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = 'between';
  $handler->display->display_options['filters']['date_filter']['group'] = 1;
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Date';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    3 => 0,
    4 => 0,
    22 => 0,
    20 => 0,
    25 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
  );
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'rtpr_reward.timestamp' => 'rtpr_reward.timestamp',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/config/rtpr/points/history';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'History';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'admin/config/rtpr/points/history/export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '100';
  $translatables['rtpr_reward'] = array(
    t('Master'),
    t('RTPR Reward'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('User'),
    t('PAP ID'),
    t('ID'),
    t('.'),
    t('Batch #'),
    t(','),
    t('Email'),
    t('Point Type'),
    t('Type'),
    t('Point'),
    t('Month'),
    t('Year'),
    t('Comment'),
    t('Timestamp'),
    t('Name'),
    t('Date'),
    t('Page'),
    t('Data export'),
  );
  $export['rtpr_reward'] = $view;

  return $export;
}
