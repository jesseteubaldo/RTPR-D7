<?php

/**
 * @file
 *
 * Features related to Gift Card Submission.
 */

/**
 * Callback for /gc-submission.
 */
function rtpr_offer_gc_submission_form($form, &$form_state) {
  global $base_url;
  
  $current_paths = explode('/', current_path());
  if (isset($current_paths[1])) {
    user_cookie_save(array('referral_aff_id' => $current_paths[1]));
  }
  
  drupal_page_is_cacheable(FALSE);
  $path = $base_url . '/' . current_path();
  $form['sharethis'] = array(
    '#markup' => '<div class="sharethis-inline-share-buttons" data-url="' . $path . '"></div>'
  );
  $block =block_load('block',108);
  $block_output = drupal_render(_block_get_renderable_array(_block_render_blocks(array($block)))); 
  $form['block'] = array(
    '#markup' => $block_output,
  );
  $form['gc_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'), 
    '#required' => TRUE,
    '#attributes' => array('required' => array(''))

  );
  
  $form['gc_email'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Email'),
    '#attributes' => array('required' => array(''))
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

function rtpr_offer_gc_submission_form_validate(&$form, &$form_state) {

  $name = $form_state['values']['gc_name'];
  $email = $form_state['values']['gc_email'];
  
  if (!valid_email_address($email)) {
    form_set_error('gc_email', t('Invalid email address!'));
  }

  if (user_load_by_mail($email)) {
    form_set_error('gc_email', t('Email existed in the system!'));
  }
  
  if (_rtpr_offer_gc_check_contact_in_campaign($email, 'V')) {
    form_set_error('gc_email', t('Email existed in the system (GetResponse)!'));
  }
      
}

function _rtpr_offer_gc_check_contact_in_campaign($email, $campaign_id) {
  $campaigns_endpoint = 'campaigns/' . $campaign_id . '/contacts';
  $data = array(
    'query[email]' => $email,
  );

  $getresponse_api_url = url(RTPR_OFFER_GETRESPONSE_BASE_URL . $campaigns_endpoint, array('query' => $data));
  $options = array(
     'headers' => array('X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY),
  );

  $result = drupal_http_request($getresponse_api_url, $options);

  if ($result->code == 200) {
    $response = drupal_json_decode($result->data);
    if ($response) {
      return $response[0]['contactId'];
    }
  }
  return FALSE;
}


function _rtpr_offer_gc_get_contacts_in_campaign($campaign_id) {
  $campaigns_endpoint = 'campaigns/' . $campaign_id . '/contacts';

  $data = array(
    'perPage' => 1000,
  );
  $getresponse_api_url = url(RTPR_OFFER_GETRESPONSE_BASE_URL . $campaigns_endpoint, array('query' => $data));
  $options = array(
     'headers' => array('X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY),
  );

  $result = drupal_http_request($getresponse_api_url, $options);

  if ($result->code == 200) {
    $response = drupal_json_decode($result->data);
    if ($response) {
      return $response;
    }
  }
  return FALSE;
}

function _rtpr_offer_gc_get_contact($contact_id) {
  $campaigns_endpoint = 'contacts/' . $contact_id;

  $getresponse_api_url = RTPR_OFFER_GETRESPONSE_BASE_URL . $campaigns_endpoint;
  $options = array(
     'headers' => array('X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY),
  );

  $result = drupal_http_request($getresponse_api_url, $options);

  if ($result->code == 200) {
    $response = drupal_json_decode($result->data);
    if ($response) {
      return $response;
    }
  }
  return FALSE;
}

function _rtpr_offer_gc_update_contact($email, $campaign_id, $updates) {
  $contact_id = _rtpr_offer_gc_check_contact_in_campaign($email, $campaign_id);
  $contacts_endpoint = 'contacts/' . $contact_id;
  $getresponse_api_url = RTPR_OFFER_GETRESPONSE_BASE_URL . $contacts_endpoint;
  
  $options = array(
    'method' => 'POST',
    'data' => json_encode($updates),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );

  $result = drupal_http_request($getresponse_api_url, $options);


}

function _rtpr_offer_gc_add_contact($email, $updates) {
  $contacts_endpoint = 'contacts/' . $contact_id;
  $getresponse_api_url = RTPR_OFFER_GETRESPONSE_BASE_URL . $contacts_endpoint;
  
  $options = array(
    'method' => 'POST',
    'data' => json_encode($updates),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );

  $result = drupal_http_request($getresponse_api_url, $options);
}

function _rtpr_offer_gc_update_contact_custom_fields($email, $campaign_id, $updates) {
  $contact_id = _rtpr_offer_gc_check_contact_in_campaign($email, $campaign_id);
  $contacts_endpoint = 'contacts/' . $contact_id . '/custom-fields';
  $getresponse_api_url = RTPR_OFFER_GETRESPONSE_BASE_URL . $contacts_endpoint;

  $options = array(
    'method' => 'POST',
    'data' => json_encode($updates),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );

  $result = drupal_http_request($getresponse_api_url, $options);
}

function _rtpr_offer_gc_update_contact_tags($email, $campaign_id, $updates) {
  $contact_id = _rtpr_offer_gc_check_contact_in_campaign($email, $campaign_id);
  $contacts_endpoint = 'contacts/' . $contact_id . '/tags';
  $getresponse_api_url = RTPR_OFFER_GETRESPONSE_BASE_URL . $contacts_endpoint;
  
  var_dump($getresponse_api_url);
  $options = array(
    'method' => 'POST',
    'data' => json_encode($updates),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );

  drupal_http_request($getresponse_api_url, $options);
}

function rtpr_offer_gc_submission_form_submit(&$form, &$form_state) {
  global $user;
  $pap_referring_id = 'N/A';
  if (isset($_COOKIE['Drupal_visitor_referral_aff_id'])) {
    $pap_referring_id = $_COOKIE['Drupal_visitor_referral_aff_id'];
  }
  
  if ($user->uid) {
    $account = user_load($user->uid);
    $account_wrapper = entity_metadata_wrapper('user', $account);
    $account_referral_id = $account_wrapper->field_pap_referral_id->value();
    if ($account_referral_id) {
      $pap_referring_id = $account_referral_id;
    }
  } 
  $current_paths = explode('/', current_path());
  if (isset($current_paths[1])) {
    $pap_referring_id = $current_paths[1];
    user_cookie_save(array('referral_aff_id' => $pap_referring_id));
  }
    
  $name = $form_state['values']['gc_name'];
  $email = $form_state['values']['gc_email'];
  $campaigns_id = 'V';
  $contact_endpoint = 'contacts';
  $data = array(
    'name' => $name,
    'email' => $email,
    'campaign' => array(
      'campaignId' => $campaigns_id,
    ),
    'tags' => array(
      array(
        'tagId' => 'Z',
      ),
    ),
    'customFieldValues' => array(
      array(
        'customFieldId' => 'C',
        'value' => array($pap_referring_id),
      ),
      array(
        'customFieldId' => '22',
        'value' => array(md5($email)),
      ),
    )
  );
  $options = array(
    'method' => 'POST',
    'data' => json_encode($data),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );
  $result = drupal_http_request(RTPR_OFFER_GETRESPONSE_BASE_URL . $contact_endpoint, $options);
  if ($result->code == 202) {
    drupal_set_message(t('Thank you for requesting a Gift Card. Please check shortly for an Activation email from promotions@rtpr.com.'));
  }

}

/**
 * Callback for user/%user/giftcard-referrals.
 */
function rtpr_offer_gc_referrals_form($form, &$form_state, $account) {
  drupal_page_is_cacheable(FALSE);
  $form['#attached']['js'][] = drupal_get_path('module', 'rtpr_offer') . '/js/rtpr_offer.js';
  $account_wrapper = entity_metadata_wrapper('user', $account);
  $pap_id = $account_wrapper->field_pap_affiliate_id->value();
  

  $search_contact =_rtpr_offer_create_search_contact_by_referral_id($pap_id, 'V');
  
  dpm($search_contact);

  $contacts = _rtpr_offer_get_contacts_by_search_contact_id($search_contact['searchContactId']);
  $referrals = array();
  $total_request = 0;
  $total_activated = 0;
  $total_used = 0;
  $rows = array();
  $tags = array('sentgiftcard', 'activatedgiftcard', 'usedgiftcard');
  $cached_contacts = array();
  $cid = 'rtpr_offer_gr_contacts_072021_v2_' . $account->uid;
  $cache = cache_get($cid);
  if (REQUEST_TIME < $cache->expire) {
    $form['cache'] = array(
      '#markup' => t(
        '<p><strong>Last update: @created.</strong></p>',
        array('@created' => date('m/d/Y H:i:s', $cache->created))
      ),
      '#weight' => 11,
    );
    $cached_contacts = $cache->data;
  }

  foreach ($contacts as $contact) {
   if ($cached_contacts) {
     $contact = $cached_contacts[$contact['contactId']];
   }
   else {
     $contact = _rtpr_offer_gc_get_contact($contact['contactId']);
     $data[$contact['contactId']] = $contact;
   }

    $gc_token = '';
    foreach ($contact['customFieldValues'] as $custom_field) {
      if ($custom_field['customFieldId'] == 22) {
        $gc_token = $custom_field['value'][0];
      }
    }
    foreach ($contact['customFieldValues'] as $custom_field) {
      if ($custom_field['value'][0] == $pap_id) {
        $referral = array(
          'name' => $contact['name'],
          'email' => $contact['email'],
          'sentgiftcard' => FALSE,
          'activatedgiftcard' => FALSE,
          'usedgiftcard' => FALSE,
        );
        
        
        foreach ($contact['tags'] as $tag) {
          $tag_name = $tag['name'];
          if (isset($referral[$tag_name])) {
            $referral[$tag_name] = TRUE;
            if ($tag_name == 'sentgiftcard') {
              $total_request++;
            }
            elseif ($tag_name == 'activatedgiftcard') {
              $total_activated++;
            }
            elseif ($tag_name == 'usedgiftcard') {
              $total_used++;
            }
          }
          
          $status = FALSE;
          if ($referral['usedgiftcard']) {
            $status = 'usedgiftcard';
          }
          elseif ($referral['activatedgiftcard']) {
            $status = 'activatedgiftcard';
          }
          elseif ($referral['sentgiftcard']) {
            $status = 'sentgiftcard';
          }
          
          if ($status != 'usedgiftcard') {
            $referral['code'] = l(
              t('View Code'),
              '',
              array(
                'attributes' => array(
                  'data-status' => $status,
                  'class' => 'view-code',
                  'data-toggle' => 'modal',
                  'data-target' => '#offerGcModal',
                  'data-name' => $contact['name'],
                  'data-email' => $contact['email'],
                  'data-token' => $gc_token,
                ),
                'fragment' => 'code',
                'external' => TRUE,
                
              )
            );
          }
          else {
            $referral['code'] = '';
          }
        }
        $referral['created'] = date('m/d/Y', strtotime($contact['createdOn']));
        $referrals[] = $referral;
        break;
      }
    }
  }
  
 

  if (!$cached_contacts) {
    cache_set($cid, $data, 'cache',  REQUEST_TIME  + (3600));
  }
  
  foreach ($referrals as $referral) {
    foreach ($tags as $tag) {
      $referral[$tag] = $referral[$tag] ? '<span class="glyphicon glyphicon-ok"></span>' : '<span class="glyphicon glyphicon-remove"></span>';
    }
    $rows[] = $referral;
  }

  $items = array(
    'Total Gift Card Request = ' . $total_request,
    'Total Gift Cards Activated = ' . $total_activated,
    'Total Gift Cards Used = ' . $total_used,
  );

  $form['summary'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#weight' => 5,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
    '#weight' => 10,
  );
  $header = array(
    'name' => t('Name'),
    'email' => t('Email'),
    'sentgiftcard' => t('SENT Gift Card'),
    'activatedgiftcard' => t('ACTIVATED Gift Card'),
    'usedgiftcard' => t('USED Gift Card'),
    'code' => t('Code'),
    'created' => t('Date Added'),
  );
    
  $form_state['header'] = $header;
  $form_state['rows'] = $referrals;
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No referrals data available.'),
    '#weight' => 15,
  );
  
  $form['modal'] = array(
    '#markup' => theme('rtpr_offer_gc_modal')
  );

  return $form;
}

/**
 * Submit handler for user/%user/giftcard-referrals.
 */
function rtpr_offer_gc_referrals_form_submit($form, &$form_state, $account) {
  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=gc-referrals.csv');

  $fp = fopen('php://output', 'w');
  unset($form_state['header']['code']);
  fputcsv($fp, $form_state['header']);
  foreach ($form_state['rows'] as $row) {
    foreach ($row as $index => $value) {
      if ($value === TRUE) {
        $row[$index] = 'Yes';
      }
      elseif ($value === FALSE) {
        $row[$index] = 'No';
      }
    }
    unset($row['code']);
    fputcsv($fp, $row);
  }
  fclose($fp);
  drupal_exit();
}

/**
 * Helper function get contact by gctoken.
 */
function _rtpr_offer_get_gr_contact_by_token($token, $campaign_id) {
  $data = array(
    'subscribersType' => array('subscribed'),
    'sectionLogicOperator' => 'and',
    'section' => array(
      array(
        'campaignIdsList' => array ($campaign_id),
        'logicOperator' => 'or',
        'subscriberCycle' => array(
          'receiving_autoresponder',
          'not_receiving_autoresponder',
        ),
        'subscriptionDate' => 'all_time',
        'conditions' => array(
          array(
            'conditionType' => 'custom',
            'scope' => '22',
            'operatorType' => 'string_operator',
            'operator' => 'is',
            'value' => $token,
          )
        )
      )
    )
  );
  $options = array(
    'method' => 'POST',
    'data' => json_encode($data),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );
  $result = drupal_http_request(RTPR_OFFER_GETRESPONSE_BASE_URL . 'search-contacts/contacts', $options);
  if ($result->code == 200) {
    $contacts = json_decode($result->data);
    $contact_id = $contacts[0]->contactId;
    return _rtpr_offer_gc_get_contact($contact_id);

  }

}

/**
 * Helper function to create search segements.
 */
function _rtpr_offer_create_search_contact_by_referral_id($referral_id, $campaign_id) {
    $data = array(
    'subscribersType' => array('subscribed'),
    'sectionLogicOperator' => 'and',
    'section' => array(
      array(
        'campaignIdsList' => array ($campaign_id),
        'logicOperator' => 'or',
        'subscriberCycle' => array(
          'receiving_autoresponder',
          'not_receiving_autoresponder',
        ),
        'subscriptionDate' => 'all_time',
        'conditions' => array(
          array(
            'conditionType' => 'custom',
            'scope' => 'C',
            'operatorType' => 'string_operator',
            'operator' => 'is',
            'value' => $referral_id,
          )
        )
      )
    ),
    'name' => 'referral for ' . $referral_id
  );
  $options = array(
    'method' => 'POST',
    'data' => json_encode($data),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );
  $url = RTPR_OFFER_GETRESPONSE_BASE_URL . 'search-contacts';
  $result = drupal_http_request($url, $options);
  

  if ($result->code == 201) {
    return drupal_json_decode($result->data);
  }
  elseif ($result->code == 409) {
    $options['method'] = 'GET';
    $data = array(
      'query' => array('name' => $data['name']),
    );
    $full_url = url($url, array('query' => $data));
    $result = drupal_http_request($full_url, $options);
    return drupal_json_decode($result->data)[0];
  }
}

/**
 * Helper function get contact by gctoken.
 */
function _rtpr_offer_get_gr_contact_by_referral_id($referral_id, $campaign_id) {
  $data = array(
    'subscribersType' => array('subscribed'),
    'sectionLogicOperator' => 'and',
    'section' => array(
      array(
        'campaignIdsList' => array ($campaign_id),
        'logicOperator' => 'or',
        'subscriberCycle' => array(
          'receiving_autoresponder',
          'not_receiving_autoresponder',
        ),
        'subscriptionDate' => 'all_time',
        'conditions' => array(
          array(
            'conditionType' => 'custom',
            'scope' => 'C',
            'operatorType' => 'string_operator',
            'operator' => 'is',
            'value' => $referral_id,
          )
        )
      )
    )
  );
  $options = array(
    'method' => 'POST',
    'data' => json_encode($data),
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );
  $result = drupal_http_request(RTPR_OFFER_GETRESPONSE_BASE_URL . 'search-contacts/contacts', $options);
  if ($result->code == 200) {
    return drupal_json_decode($result->data);
  }

}

/**
 * Helper function get contact by search id.
 */
function _rtpr_offer_get_contacts_by_search_contact_id($search_contact_id) {
  $options = array(
    'headers' => array(
      'Content-Type' => 'application/json',
      'X-Auth-Token' => 'api-key ' . RTPR_OFFER_GETRESPONSE_API_KEY,
    ),
  );
  $search_contacts_url = url(
    RTPR_OFFER_GETRESPONSE_BASE_URL . 'search-contacts/' . $search_contact_id . '/contacts',
    array('query' => array('perPage' => 1000))
  );

  $result = drupal_http_request($search_contacts_url, $options);

  $options['method'] = 'DELETE';
  // Delete search segment.
  drupal_http_request(RTPR_OFFER_GETRESPONSE_BASE_URL . 'search-contacts/' . $search_contact_id, $options);

  if ($result->code == 200) {
    return drupal_json_decode($result->data);
  }

}
