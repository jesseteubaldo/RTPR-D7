<?php

/**
 * Implementation of hook_rules_event_info().
 */
function rtpr_user_rules_event_info() {
  // return array(
  //   'rtpr_user_action_register_presave_user' => array(
  //     'label' => t('Before saving a new user account'),
  //     'group' => t('User'),
  //     'variables' => array(
  //       'account' => array('type' => 'user', 'label' => t('registered user')),
  //     ),
  //   ),
  // );
}

/**
 * Implements hook_rules_condition_info().
 */
function rtpr_user_rules_condition_info() {
  $inline_conditions = inline_conditions_get_info();
  $conditions = array();
  $conditions['rtpr_user_condition_msp'] = array(
    'label' => t('Check user\'s MSP'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
      'campaigns' => array(
        'type' => 'list<text>',
        'label' => t('Campaigns'),
        'options list' => 'rtpr_user_rules_campaigns',
        'restriction' => 'input',
      ),
      'tier' => array(
        'type' => 'text',
        'label' => t('Tier'),
        'options list' => 'rtpr_user_rules_tier_list',
        'restriction' => 'input',
      ),
      'msp' => array(
        'type' => 'text',
        'label' => t('MSP value'),
        'description' => t('Input MSP value to be checked.'),
        'restriction' => 'input',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'rtpr_user_rules_op_list',
        'restriction' => 'input',
      ),
      'daterange' => array(
        'type' => 'text',
        'label' => t('Date Preset'),
        'options list' => 'rtpr_user_rules_daterange_list',
        'restriction' => 'input',
      ),
      'datetime_start' => array(
        'label' => t('Date/Time Start'),
        'type' => 'date',
      ),
      'datetime_end' => array(
        'label' => t('Date/Time End'),
        'type' => 'date',
      ),
    ),
    'group' => t('Post Affiliate Pro'),
  );
  $conditions['rtpr_user_condition_apv'] = array(
    'label' => t('Check user APV'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
      'msp' => array(
        'type' => 'text',
        'label' => t('APV value'),
        'description' => t('Input APV value to be checked.'),
        'restriction' => 'input',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'rtpr_user_rules_op_list',
        'restriction' => 'input',
      ),
      'datetime_start' => array(
        'label' => t('Date/Time Start'),
        'type' => 'date',
      ),
      'datetime_end' => array(
        'label' => t('Date/Time End'),
        'type' => 'date',
      ),
    ),
      'group' => t('Post Affiliate Pro'),
  );
  
  $conditions['rtpr_user_check_available_points'] = array(
    'label' => t('Check available points'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
      'available' => array(
        'type' => 'text',
        'label' => t('Available points'),
        'description' => t('User points to be checked.'),
        'restriction' => 'input',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'rtpr_user_rules_op_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('RTPR User'),
    'callbacks' => array(
    'execute' => 'rtpr_user_check_available_points',
    ),
  );
  
  $conditions['rtpr_user_condition_pmsp'] = array(
    'label' => t('Check user\'s PMSP'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
      'msp' => array(
        'type' => 'text',
        'label' => t('MSP value'),
        'description' => t('Input MSP value to be checked.'),
        'restriction' => 'input',
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'rtpr_user_rules_op_list',
        'restriction' => 'input',
      ),
      'month' => array(
        'type' => 'text',
        'label' => t('Month'),
        'options list' => 'rtpr_user_rules_pmsp_month_list',
        'restriction' => 'input',
      ),
    ),
    'group' => t('RTPR User'),
  );
  
  $conditions['rtpr_user_check_pmsp'] = array(
    'label' => t('Check User PMSP'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order.'),
        'wrapped' => TRUE,
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The operator used with the PMSP value below.'),
        'default value' => '>=',
        'options list' => '_commerce_discount_operator_options',
      ),
      'pmsp' => array(
        'type' => 'integer',
        'label' => t('PMSP'),
        'description' => t('PMSP to check.'),
      ),
      'date' => array(
        'type' => 'text',
        'label' => t('Date'),
      ),
    ),
    'module' => 'rtpr_user',
    'group' => t('Commerce Discount'),
    'callbacks' => array(
      'execute' => $inline_conditions['rtpr_user_check_pmsp']['callbacks']['build'],
    ),
  );

  return $conditions;
}

/**
 * Get the list of options
 */
function rtpr_user_rules_tier_list() {
  return array(
    1 => 'Tier1',
    2 => 'Tier2 - Experimental',
  );
}

/**
 * Get the list of options
 */
function rtpr_user_rules_op_list() {
  return array(
    '<' => '<',
    '<=' => '<=',
    '=' => '=',
    '>=' => '>=',
    '>' => '>',
  );
}

/**
 * Get the list of options
 */
function rtpr_user_rules_pmsp_month_list() {
  return array(
    'current' => 'Current Month',
    'last' => 'Last Month',
  );
}



/**
 * Get the list of options
 */
function rtpr_user_rules_daterange_list() {
  return array(
    'none' => 'Use custom date range',
    Gpf_Data_Filter::RANGE_TODAY => 'Today',
    Gpf_Data_Filter::RANGE_YESTERDAY => 'Yesterday',
    Gpf_Data_Filter::RANGE_LAST_7_DAYS => 'Last 7 days',
    Gpf_Data_Filter::RANGE_LAST_30_DAYS => 'Last 30 days',
    Gpf_Data_Filter::RANGE_LAST_90_DAYS => 'Last 90 days',
    Gpf_Data_Filter::RANGE_THIS_WEEK => 'This week (Mon - Sun)',
    Gpf_Data_Filter::RANGE_LAST_WEEK => 'Last week (Mon - Sun)',
    Gpf_Data_Filter::RANGE_LAST_2WEEKS => 'Last 2 weeks',
    Gpf_Data_Filter::RANGE_LAST_WORKING_WEEK => 'Last working week (Mon - Fri)',
    Gpf_Data_Filter::RANGE_THIS_MONTH => 'This month',
    Gpf_Data_Filter::RANGE_LAST_MONTH => 'Last month',
    Gpf_Data_Filter::RANGE_THIS_YEAR => 'This year',
    Gpf_Data_Filter::RANGE_LAST_YEAR => 'Last year',
  );
}

/**
 * Calculate the total PMSP
 */
function rtpr_user_condition_pmsp($account, $msp, $op, $month_selected) {
  $month = 0;
  $year = 0;
  if ($month_selected == 'current') {
    $month = date('m');
    $year = date('Y');
  }
  elseif ($month_selected == 'last') {
    $month = date('m', strtotime('last month'));
    $year = date('Y', strtotime('last month'));
  }
  
  $monthly_msp = _rtpr_user_get_monthly_pmsp($account->uid, $month, $year, TRUE);

    switch ($op) {
    case '<':
      return $monthly_msp < $msp;

    case '<=':
      return $monthly_msp <= $msp;

    case '=':
      return $monthly_msp == $msp;

    case '>=':
      return $monthly_msp >= $msp;

    case '>':
      return $monthly_msp > $msp;
  }
  return FALSE;
}

/**
 * Calculate the total MSP
 */
function rtpr_user_condition_msp($account, $campaigns, $tier = 1, $msp, $op, $daterange, $datetime_start, $datetime_end) {
  // get affiliate id
  $msp = trim($msp);
  $user_wrapper = entity_metadata_wrapper('user', $account);
  $papaid = $user_wrapper->field_pap_affiliate_id->value();
  // load PAP library
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();

  // get recordset of list of transactions
  $request = new Pap_Api_TransactionsGrid($session);
  if($daterange != 'none'){
    //watchdog('rtpr_user', 'Calculated MSP range for @range.', array('@range' => $daterange));
    $request->addFilter('dateinserted', Gpf_Data_Filter::DATERANGE_IS, $daterange); // to use the pre defined date ranges
  }
  else {
    //watchdog('rtpr_user', 'Calculated MSP range for @start - @end.', array('@start' => date('Y-m-d', $datetime_start), '@end' => date('Y-m-d', $datetime_end)));
    $request->addFilter('dateinserted', 'D>=', date('Y-m-d', $datetime_start));
    $request->addFilter('dateinserted', 'D<=', date('Y-m-d', $datetime_end));
  }
  // example 9b9a3106
  $request->addFilter('userid', Gpf_Data_Filter::EQUALS, $papaid);
  $campaigns_list = implode(',', $campaigns);
  $request->addFilter('campaignid', 'IN', $campaigns_list);
  // for the commission = 0, self purchase
  $request->addFilter('tier', Gpf_Data_Filter::EQUALS, $tier);
  $request->setLimit(0, 30);
  $request->sendNow();
  $grid      = $request->getGrid();
  $recordset = $grid->getRecordset();
  $totalcost = 0;
  // iterate through the records
  foreach ($recordset as $rec) {
    $totalcost += $rec->get('totalcost');
  }
  $totalRecords = $grid->getTotalCount();
  $maxRecords = $recordset->getSize();
  if ($maxRecords > 0) {
    $cycles = ceil($totalRecords / $maxRecords);
    for ($i = 1; $i < $cycles; $i++) {
      $request->setLimit($i * $maxRecords, $maxRecords);
      $request->sendNow();
      $recordset = $request->getGrid()->getRecordset();
      // iterate through the records
      foreach ($recordset as $rec) {
        $totalcost += $rec->get('totalcost');
      }
    }
  }
  //watchdog('rtpr_user', 'Calculated MSP is @total for @user.', array('@total' => $totalcost, '@user' => $account->uid));
  // rule condition to see the range
  switch ($op) {
    case '<':
      return $totalcost < $msp;

    case '<=':
      return $totalcost <= $msp;

    case '=':
      return $totalcost == $msp;

    case '>=':
      return $totalcost >= $msp;

    case '>':
      return $totalcost > $msp;
  }
  return FALSE;
}

/**
 * Implements hook_rules_action_info().
 */
function rtpr_user_rules_action_info() {
  $actions = array();
  $actions['rtpr_user_action_create_affiliate'] = array(
    'label' => t('Add an affiliate account'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('Account')),
      'first_name' => array(
        'type' => 'text',
        'label' => t('First Name'),
      ),
      'last_name' => array(
        'type' => 'text',
        'label' => t('Last Name'),
      ),
      'email' => array(
        'type' => 'text',
        'label' => t('E-mail'),
      ),
      'password' => array(
        'type' => 'text',
        'label' => t('Password'),
      ),
      'phone' => array(
        'type' => 'text',
        'label' => t('Phone'),
      ),
      'street' => array(
        'type' => 'text',
        'label' => t('Street'),
      ),
      'city' => array(
        'type' => 'text',
        'label' => t('City'),
      ),
      'states' => array(
        'type' => 'text',
        'label' => t('State'),
      ),
      'zip' => array(
        'type' => 'text',
        'label' => t('Zip code'),
      ),
      'country' => array(
        'type' => 'text',
        'label' => t('Country'),
      ),
      'status' => array(
        'type' => 'list<text>',
        'label' => t('Status'),
        'options list' => 'rtpr_user_rules_user_status',
        'restriction' => 'input',
      ),
    ),
    'provides' => array(
      'affiliate_id' => array('type' => 'text', 'label' => t('Affiliate id')),
    ),
    'group' => t('Post Affiliate Pro'),
  );
  $actions['rtpr_user_action_update_affiliate_id'] = array(
    'label' => t('Get affiliate id'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('Account')),
    ),
    'provides' => array(
      'affiliate_id' => array('type' => 'text', 'label' => t('Affiliate id')),
    ),
    'group' => t('Post Affiliate Pro'),
  );
  $actions['rtpr_user_action_get_referring_user'] = array(
    'label' => t('Get referring user'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('Account')),
    ),
    'provides' => array(
      'referring_user' => array('type' => 'user', 'label' => t('Referring user')),
    ),
    'group' => t('user referral'),
  );
  $actions['rtpr_user_action_add_affiliate_relations'] = array(
    'label' => t('Add affiliate relations'),
    'parameter' => array(
      'referring-user' => array('type' => 'user', 'label' => t('Referring User')),
      'new-user' => array('type' => 'user', 'label' => t('New User')),
    ),
    'group' => t('Post Affiliate Pro'),
  );
  // $actions['rtpr_user_action_add_campaign'] = array(
  //   'label' => t('Add user to a campaign'),
  //   'parameter' => array(
  //     'account' => array('type' => 'user', 'label' => t('Account')),
  //     'campaigns' => array(
  //       'type' => 'list<text>',
  //       'label' => t('Campaigns'),
  //       'options list' => 'rtpr_user_rules_campaigns',
  //       'restriction' => 'input',
  //     ),
  //   ),
  //   'group' => t('Post Affiliate Pro'),
  // );
  $actions['rtpr_user_action_add_commissiongroup'] = array(
    'label' => t('Add user to a commission group'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('Account')),
      'commissiongroup' => array(
        'type' => 'list<text>',
        'label' => t('Commission Groups to Add'),
        'options list' => 'rtpr_user_rules_commissiongroup',
        'restriction' => 'input',
      ),
      'status' => array(
        'type' => 'text',
        'label' => t('Status'),
        'options list' => 'rtpr_user_rules_commissiongroup_status',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Post Affiliate Pro'),
  );
  $actions['rtpr_user_action_remove_commissiongroup'] = array(
    'label' => t('Remove user from a commission group'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('Account')),
      'commissiongroup' => array(
        'type' => 'list<text>',
        'label' => t('Commission Groups to Remove'),
        'options list' => 'rtpr_user_rules_commissiongroup',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Post Affiliate Pro'),
  );

  $actions['rtpr_user_action_remove_all_commissiongroup'] = array(
    'label' => t('Remove all commission group for an user'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('Account')),
    ),
    'group' => t('Post Affiliate Pro'),
  );

  return $actions;
}

/**
 * Get the list of options
 */
function rtpr_user_rules_user_status() {
  return array(
    'A' => t('Approved'),
    'P' => t('Pending'),
    'D' => t('Declined'),
  );
}

/**
 * Get the list of field instances
 */
function rtpr_user_rules_user_fields_list() {
  $options = array();
  $fields = field_info_instances('user', 'user');
  foreach ($fields as $machine_name => $field) {
    $options[$machine_name] = $field['label'];
  }
  return $options;
}

/**
 * Get the list of commission group status
 */
function rtpr_user_rules_commissiongroup_status() {
  // A = Approved, R = Ascending, F = Descending, X = Fixed
  return array(
    'A' => 'Approved',
    'R' => 'Ascending',
    'F' => 'Descending',
    'X' => 'Fixed',
  );
}

/**
 * Action callback rtpr_user_action_create_affiliate
 */
function rtpr_user_action_create_affiliate($account, $first_name, $last_name, $email, $password, $phone, $street, $city, $states, $zip, $country, $status, $field_affiliate_id) {
  if (empty($password)) {
    $password = $account->password;
  }
  $account_id = variable_get('commerce_post_affiliate_pro_merchant_id', NULL);
  if (!$account_id) {
    watchdog('rtpr_user', 'Could not create the user account on PAP @user, merchant id is not set, please visit admin pages.', array('@user' => $account->mail));
    return;
  }
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  $affiliate = new Pap_Api_Affiliate($session);
  $affiliate->setUsername($email);
  $affiliate->setFirstname($first_name);
  $affiliate->setLastname($last_name);
  $affiliate->setPassword($password);
  $affiliate->setData(3, $street);
  $affiliate->setData(4, $city);
  $affiliate->setData(5, $states);
  $affiliate->setData(6, $country);
  $affiliate->setData(7, $zip);
  $affiliate->setData(8, $phone);
  $affiliate->setStatus(reset($status));

  $pap_affiliate_id = '671d0049';
  if (isset($_COOKIE['Drupal_visitor_referral_aff_id'])) {
    $pap_affiliate_id = $_COOKIE['Drupal_visitor_referral_aff_id'];
  }
  if (isset($_COOKIE['Drupal_visitor_referral_aff_id'])) {
    $affiliate->setParentUserId($pap_affiliate_id);
  }
  
  $account_wrapper = entity_metadata_wrapper('user', $account);
  try {
    $referral_id = $account_wrapper->field_pap_referral_id->value();
    if ($referral_id) {
      $affiliate->setParentUserId(trim($referral_id));
    }
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'rtpr_user',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
  try {
    if ($affiliate->add()) {
      watchdog('rtpr_user', '@user created an affiliate on PAP.', array('@user' => $account->mail));
    }
    else {
      watchdog('rtpr_user', '@error', array('@error' => $affiliate->getMessage()), WATCHDOG_ERROR);
    }
  }
  catch(Exception$e) {
    watchdog('rtpr_user', '@error, occurred while trying to create affiliate with rules.', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
  }
  $affiliate_id = rtpr_user_action_update_affiliate_id($account);
  return $affiliate_id;
}

/**
 * Action callback rtpr_user_action_update_affiliate_id
 */
function rtpr_user_action_update_affiliate_id($account) {
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  //create request object
  $request = new Pap_Api_AffiliatesGrid($session);
  $request->addFilter('username', Gpf_Data_Filter::EQUALS, $account->mail);
  $request->setLimit(0, 1);
  // sets columns, use it only if you want retrieve other as default columns
  $request->addParam('columns', new Gpf_Rpc_Array(array(array('id'), array('refid'), array('userid'),
        array('username'), array('firstname'), array('lastname'), array('rstatus'), array('parentuserid'),
        array('dateinserted'), array('salesCount'), array('clicksRaw'), array('clicksUnique'),
      )));

  // send request
  try {
    $request->sendNow();
  }
  catch(Exception$e) {
    watchdog('rtpr_user', '@error, occurred while trying to fetch an affiliate account.', array('@error' => ''), WATCHDOG_ERROR);
  }

  // request was successful, get the grid result
  $grid = $request->getGrid();

  // get recordset from the grid
  $recordset = $grid->getRecordset();
  foreach ($recordset as $rec) {
    $userid = $rec->get('userid');
  }
  if (!empty($userid)) {
    return array('affiliate_id' => $userid);
    watchdog('rtpr_user', '@user updated with the affiliate id.', array('@user' => $account->mail));
  }
  else {
    watchdog('rtpr_user', '@error, occurred while trying to update the @user an affiliate account.', array('@user' => $account->mail, '@error' => ''), WATCHDOG_ERROR);
  }
}

/**
 * Action callback rtpr_user_action_get_referring_user
 */
function rtpr_user_action_get_referring_user($account) {
  $uid = referral_get_user($account->uid);
  return array('referring_user' => user_load($uid));
}

/**
 * Action callback rtpr_user_action_add_parent_user
 */
function rtpr_user_action_add_affiliate_relations($referring_user, $new_user) {
  // load the PAP library
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  // get user objects
  $new_user_wrapper = entity_metadata_wrapper('user', $new_user);
  $new_user_papaid = $new_user_wrapper->field_pap_affiliate_id->value();
  $referring_user_wrapper = entity_metadata_wrapper('user', $referring_user);
  $referring_user_papaid = $referring_user_wrapper->field_pap_affiliate_id->value();
  // connect with PAP
  $affiliate = new Pap_Api_Affiliate($session);
  $affiliate->setUserid($new_user_papaid);
  try {
    $affiliate->load();
  }
  catch(Exception $e) {
    watchdog('rtpr_user', '@error, occurred while loading the affiliate user object.', array('@error' => $affiliate->getMessage()), WATCHDOG_ERROR);
    return;
  }

  $affiliate->setParentUserId($referring_user_papaid);

  try {
    if ($affiliate->save()) {
      //watchdog('rtpr_user', '@user1 affiliate relations to @user2 added.', array('@user1' => $new_user->mail, '@user2' => $referring_user->mail));
    }
    else {
      watchdog('rtpr_user', '@error, occurred while trying to save the affiliate account.', array('@error' => $affiliate->getMessage()), WATCHDOG_ERROR);
    }
  }
  catch(Exception$e) {
    watchdog('rtpr_user', '@error, occurred while communicating with PAP.', array('@error' => $affiliate->getMessage()), WATCHDOG_ERROR);
  }
}

/**
 * Action callback rtpr_user_action_add_campaign
 */
function rtpr_user_action_add_campaign($account, $campaigns) {}

/**
 * Action callback rtpr_user_action_add_commissiongroup
 */
function rtpr_user_action_add_commissiongroup($account, $commissiongroups, $rstatus) {
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $papaid          = $account_wrapper->field_pap_affiliate_id->value();
  $path            = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  $request = new Gpf_Rpc_FormRequest("Pap_Features_CommissionGroups_AffiliateGroupForm", "add", $session);
  $request->setField('Id', '');
  $request->setField('userid', $papaid);
  // A = Approved, R = Ascending, F = Descending, X = Fixed
  $request->setField('rstatus', $rstatus);
  $request->setField('note', '');
  foreach ($commissiongroups as $commissiongroup) {
    
    list($campaignid, $commissiongroupid) = explode('|', $commissiongroup);
    $request->setField('campaignid', $campaignid);
    $request->setField('commissiongroupid', $commissiongroupid);
    try {
      $request->sendNow();
      $response = $request->getStdResponse();
      //watchdog('rtpr_user', 'Add @commissiongroup to @user.', array('@user' => $account->mail, '@commissiongroup' => $commissiongroupid));
    }
    catch(Exception$e) {
      watchdog('rtpr_user', 'API call error: @error.', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    }
  }
}

/**
 * Action callback rtpr_user_action_remove_commissiongroup
 */
function rtpr_user_action_remove_commissiongroup($account, $commissiongroups) {
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $papaid          = $account_wrapper->field_pap_affiliate_id->value();
  $ids             = array();
  // load the PAP library
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();
  foreach ($commissiongroups as $commissiongroup) {
    $request = new Gpf_Rpc_Gridrequest("Pap_Features_Common_AffiliateGroupGrid", "getRows", $session);
    $request->addFilter('userid', '=', $papaid);
    list($campaignid, $commissiongroupid) = explode('|', $commissiongroup);
    $request->addFilter('campaignid', '=', $campaignid);
    $request->addFilter('commissiongroupid', '=', $commissiongroupid);
    try {
      $request->sendNow();
    }
    catch(Exception$e) {
      watchdog('rtpr_user', 'API call error: @error.', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    }
    $grid = $request->getGrid();
    $recordset = $grid->getRecordset();
    foreach ($recordset as $rec) {
      $ids[] = $rec->get('id');
    }
  }
  
  if (!empty($ids)) {
    // delete the entry
    $request = new Gpf_Rpc_Request("Pap_Features_CommissionGroups_AffiliateGroupForm", "deleteRows", $session);
    $request->addParam('ids', new Gpf_Rpc_Array($ids));
    try {
      $request->sendNow();
    }
    catch(Exception$e) {
      watchdog('rtpr_user', 'API call error: @error.', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    }

    // Get the response as stdClass Object
    $response = $request->getStdResponse();
    watchdog('rtpr_user', '@id removed from commission groups @group', array('@id' => $papaid, '@group' => implode(', ', $commissiongroups)));
  }
}

/**
 * Action callback rtpr_user_action_remove_all_commissiongroup
 */
function rtpr_user_action_remove_all_commissiongroup($account) {
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $papaid          = $account_wrapper->field_pap_affiliate_id->value();
  $ids             = array();
  // load the PAP library
  $path = libraries_get_path('pap');
  require_once ($path . '/PapApi.class.php');
  module_load_include('php', 'commerce_post_affiliate_pro', 'DrupalPapApi.class');
  $session = commerce_post_affiliate_pro_get_session();

  $request = new Gpf_Rpc_Gridrequest("Pap_Features_Common_AffiliateGroupGrid", "getRows", $session);
  $request->addFilter('userid', '=', $papaid);
  try {
    $request->sendNow();
  }
  catch(Exception$e) {
    watchdog('rtpr_user', 'API call error: @error.', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
  }
  $grid = $request->getGrid();
  $recordset = $grid->getRecordset();
  foreach ($recordset as $rec) {
    $ids[] = $rec->get('id');
  }

  if (!empty($ids)) {
    // delete the entry
    $request = new Gpf_Rpc_Request("Pap_Features_CommissionGroups_AffiliateGroupForm", "deleteRows", $session);
    $request->addParam('ids', new Gpf_Rpc_Array($ids));
    try {
      $request->sendNow();
    }
    catch(Exception$e) {
      watchdog('rtpr_user', 'API call error: @error.', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    }

    // Get the response as stdClass Object
    $response = $request->getStdResponse();
    watchdog('rtpr_user', 'Removed all commission groups for @id', array('@id' => $papaid));
  }
}

/**
 * Calculate the total APV
 */
function rtpr_user_condition_apv($account, $apv, $op, $datetime_start, $datetime_end) {
  // get affiliate id
  $apv = trim($apv);
  $user_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $user_wrapper->field_pap_affiliate_id->value();

  $datetime_start = date('Y-m-d H:i:s', $datetime_start);
  $datetime_end = date('Y-m-d H:i:s', $datetime_end);

  db_set_active('pap');
  $totalcost = 0;
  // Tier 1 Retail (campaign 11111111) total cost divided by 2.
  $results = db_select('qu_pap_transactions', 't')
        ->fields('t')
        ->condition('userid', $pap_id, '=')
        ->condition('tier', '1', '=')
        ->condition('rtype', 'R', '!=')
        ->condition('campaignid', '11111111', '=')
        ->condition('dateinserted', array($datetime_start, $datetime_end), 'BETWEEN')
        ->execute();
  foreach ($results as $row) {
    if ($row->totalcost) {
      $totalcost += $row->totalcost;
    }
  }
  $totalcost = $totalcost / 2;
  $results = db_select('qu_pap_transactions', 't')
        ->fields('t')
        ->condition('userid', $pap_id, '=')
        ->condition('tier', '1', '=')
        ->condition('rtype', 'R', '!=')
        ->condition('campaignid', 'c5d380ba', '=')
        ->condition('dateinserted', array($datetime_start, $datetime_end), 'BETWEEN')
        ->execute();

  foreach ($results as $row) {
    if ($row->totalcost) {
      $totalcost += $row->totalcost;
    }
  }

  $results = db_select('qu_pap_transactions', 't')
        ->fields('t')
        ->condition('userid', $pap_id, '=')
        ->condition('tier', '2', '=')
        ->condition('rtype', 'R', '!=')
        ->condition('campaignid', 'c602cb08', '=')
        ->condition('dateinserted', array($datetime_start, $datetime_end), 'BETWEEN')
        ->execute();

  foreach ($results as $row) {
    if ($row->totalcost) {
      $totalcost += ($row->totalcost / 2);
    }
  }
  db_set_active();
 // dpm($datetime_start . ' - ' . $datetime_end . ' = ' . $totalcost . ' APV  ' . $op . ' ' . $apv);

  switch ($op) {
    case '<':
      return $totalcost < $apv;

    case '<=':
      return $totalcost <= $apv;

    case '=':
      return $totalcost == $apv;

    case '>=':
      return $totalcost >= $apv;

    case '>':
      return $totalcost > $apv;
  }
  return FALSE;
}

/**
 * Condition callback: checks available points. 
 */
function rtpr_user_check_available_points($account, $points, $op) {
  $available_points = rtpr_reward_get_available_credits($account->uid);
  switch ($op) {
    case '<':
      return $available_points < $points;
  
    case '<=':
      return $available_points <= $points;
  
    case '=':
      return $available_points == $points;
  
    case '>=':
      return $available_points >= $points;
  
    case '>':
      return $available_points > $points;
  }

  return FALSE;
}
