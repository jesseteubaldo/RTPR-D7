<?php
/**
 * @file
 * rtpr_commerce.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function rtpr_commerce_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'customer_report_full';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'users';
  $view->human_name = 'Customer report full';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Customer reports';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'mail' => 'mail',
    'order_id' => 'order_id',
    'commerce_order_total_1' => 'commerce_order_total_1',
    'commerce_order_total' => 'commerce_order_total',
    'created' => 'created',
    'login' => 'login',
    'field_pap_affiliate_id' => 'field_pap_affiliate_id',
    'commerce_customer_address' => 'commerce_customer_address',
    'commerce_customer_address_1' => 'commerce_customer_address_1',
  );
  $handler->display->display_options['style_options']['default'] = 'login';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'login' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_pap_affiliate_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_customer_address' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_customer_address_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: User: Orders */
  $handler->display->display_options['relationships']['uid_commerce_orders']['id'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['uid_commerce_orders']['table'] = 'users';
  $handler->display->display_options['relationships']['uid_commerce_orders']['field'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['uid_commerce_orders']['required'] = TRUE;
  /* Relationship: Commerce Order: Referenced customer profile */
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['id'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['table'] = 'field_data_commerce_customer_shipping';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['field'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['label'] = 'Shipping';
  /* Relationship: Commerce Order: Referenced customer profile */
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['id'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['table'] = 'field_data_commerce_customer_billing';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['field'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['label'] = 'Billing';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Customer';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['link_to_user'] = '0';
  /* Field: User: Phone */
  $handler->display->display_options['fields']['field_phone']['id'] = 'field_phone';
  $handler->display->display_options['fields']['field_phone']['table'] = 'field_data_field_phone';
  $handler->display->display_options['fields']['field_phone']['field'] = 'field_phone';
  $handler->display->display_options['fields']['field_phone']['element_type'] = '0';
  $handler->display->display_options['fields']['field_phone']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_phone']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_phone']['field_api_classes'] = TRUE;
  /* Field: Commerce Customer profile: Address - First name */
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['id'] = 'commerce_customer_address_first_name';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['field'] = 'commerce_customer_address_first_name';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['label'] = 'First name';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_first_name']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Last name */
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['id'] = 'commerce_customer_address_last_name';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['field'] = 'commerce_customer_address_last_name';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['label'] = 'Last name';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_last_name']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Company */
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['id'] = 'commerce_customer_address_organisation_name';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['field'] = 'commerce_customer_address_organisation_name';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['label'] = 'Company';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_organisation_name']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Thoroughfare (i.e. Street address) */
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['id'] = 'commerce_customer_address_thoroughfare';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['field'] = 'commerce_customer_address_thoroughfare';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['label'] = 'Street address';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_thoroughfare']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Premise (i.e. Apartment / Suite number) */
  $handler->display->display_options['fields']['commerce_customer_address_premise']['id'] = 'commerce_customer_address_premise';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['field'] = 'commerce_customer_address_premise';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['label'] = 'Apartment / Suite number';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_premise']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Locality (i.e. City) */
  $handler->display->display_options['fields']['commerce_customer_address_locality']['id'] = 'commerce_customer_address_locality';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['field'] = 'commerce_customer_address_locality';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['label'] = 'City';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_locality']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Postal code */
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['id'] = 'commerce_customer_address_postal_code';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['field'] = 'commerce_customer_address_postal_code';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['label'] = 'Postal code';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_postal_code']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Administrative area (i.e. State / Province) */
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['id'] = 'commerce_customer_address_administrative_area';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['field'] = 'commerce_customer_address_administrative_area';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['label'] = 'State / Province';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_administrative_area']['element_default_classes'] = FALSE;
  /* Field: Commerce Customer profile: Address - Country */
  $handler->display->display_options['fields']['commerce_customer_address_country']['id'] = 'commerce_customer_address_country';
  $handler->display->display_options['fields']['commerce_customer_address_country']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_country']['field'] = 'commerce_customer_address_country';
  $handler->display->display_options['fields']['commerce_customer_address_country']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_country']['label'] = 'Country';
  $handler->display->display_options['fields']['commerce_customer_address_country']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_country']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address_country']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address_country']['display_name'] = 1;
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address']['label'] = 'Billing Address';
  $handler->display->display_options['fields']['commerce_customer_address']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  $handler->display->display_options['fields']['commerce_customer_address']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['commerce_customer_address']['field_api_classes'] = TRUE;
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address_1']['id'] = 'commerce_customer_address_1';
  $handler->display->display_options['fields']['commerce_customer_address_1']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_1']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_1']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_1']['label'] = 'Shipping Address';
  $handler->display->display_options['fields']['commerce_customer_address_1']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address_1']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  $handler->display->display_options['fields']['commerce_customer_address_1']['group_column'] = 'entity_id';
  /* Field: COUNT(Commerce Order: Order ID) */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['order_id']['label'] = 'Orders';
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Total';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Field: AVG(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total']['group_type'] = 'avg';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Average';
  $handler->display->display_options['fields']['commerce_order_total']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  $handler->display->display_options['fields']['commerce_order_total']['field_api_classes'] = TRUE;
  /* Field: User: PAP Affiliate ID */
  $handler->display->display_options['fields']['field_pap_affiliate_id']['id'] = 'field_pap_affiliate_id';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['table'] = 'field_data_field_pap_affiliate_id';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['field'] = 'field_pap_affiliate_id';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['element_type'] = '0';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_pap_affiliate_id']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_pap_affiliate_id']['field_api_classes'] = TRUE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Registered';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['created']['format_date_sql'] = 0;
  /* Field: User: Last login */
  $handler->display->display_options['fields']['login']['id'] = 'login';
  $handler->display->display_options['fields']['login']['table'] = 'users';
  $handler->display->display_options['fields']['login']['field'] = 'login';
  $handler->display->display_options['fields']['login']['date_format'] = 'short';
  $handler->display->display_options['fields']['login']['second_date_format'] = 'long';
  $handler->display->display_options['fields']['login']['format_date_sql'] = 0;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'commerce-reports';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Search customers';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  $handler->display->display_options['path'] = 'rtpr/reports/customers';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Customers';
  $handler->display->display_options['menu']['description'] = 'View customer reports.';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view-%timestamp-full.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(
    'pending' => 'pending',
    'completed' => 'completed',
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'admin/commerce/reports/customers/export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
    'chart_revenue' => 0,
    'chart_orders' => 0,
  );
  $export['customer_report_full'] = $view;

  return $export;
}
