<?php

/**
 * @file
 * Services related to user.
 */

 function rtpr_user_services_zapier() {
  $post_data = file_get_contents('php://input');
  $json = drupal_json_decode($post_data);
  if (!empty($json['API_KEY']) && !empty($json['Email'])) {
    if ($json['API_KEY'] == variable_get('rtpr_user_zapier_api_key', NULL)) {
      return _rtpr_user_services_zapier_email($json);
    }
  }
  return drupal_json_output('Invalid API KEY');
 }

 function _rtpr_user_services_zapier_email($json) {
     $email = $json['Email'];
      $existing_account = user_load_by_mail($json['Email']);
      if (!$existing_account) {
        // Create account
        $password = user_password();
        $new_user = array(
          'name' => $email,
          'pass' => $password, // note: do not md5 the password
          'mail' => $email,
          'status' => 1,
          'init' => $email,
          'roles' => array(
            DRUPAL_AUTHENTICATED_RID => 'authenticated user',
            7 => 'Retail Customer',
          ),
          'field_phone' => array(LANGUAGE_NONE => array(array('value' => $json['Phone']))),
          'field_first_name' => array(LANGUAGE_NONE => array(array('value' => $json['First']))),
          'field_last_name' => array(LANGUAGE_NONE => array(array('value' => $json['Last']))),
          'field_address' => array(LANGUAGE_NONE => array(array(
            'country' => !empty($json['Country']) ? $json['Country'] : 'US',
            'administrative_area' => $json['State'],
            'locality' => $json['City'],
            'postal_code' => $json['Postal'],
            'thoroughfare' => $json['Address'],
            //'premise' => $json['address_2'],
            ))),
          'field_funnel_order_id' => array(LANGUAGE_NONE => array(array('value' => $json['OrderNum']))),
          'field_funnel_order_sku' => array(LANGUAGE_NONE => array(array('value' => $json['Sku']))),
        );

        $json['status'] = t('Created account: @email', array('@email' => $json['Email']));
        watchdog('rtpr_user', '<pre>' . print_r($json, TRUE) . '</pre>');
        $account = user_save('', $new_user);
        if ($account) {
          $referral_id = $json['pap-referral-id'];
          if ($referral_id != 'undefined') {
            $account->field_pap_referral_id = array(LANGUAGE_NONE => array(array('value' => $referral_id)));
            user_save($account);
          }
          $message = "
Hello!

Thank you for choosing from our <strong>George Foreman™ GoPak Offers!</strong> Within a few days, you will receive shipment confirmation from RTPR.com.

<strong>eBook Available Now</strong>
Your virtual copy of George Foreman's™ Home Remedy Guide to Natural Pain Relief is available today! In this eBook, George Foreman™ is excited to share the following informative articles with you:

<ul>
<li>The Connection Between Sleep and Chronic Pain </li>
<li>Do’s & Don’ts for Knee Pain</li>
<li>Menthol for Topical Pain Relief</li>
<li>7 Tips to Get Relief for Arthritis Pain</li>
<li>Pain Relief Tips for Children</li>
<li>10 Tips to Relieve Muscle Tension</li>
<li>8 Tips to Cope with Headaches</li>
<li>And much more
</li>
</ul>

To access your free eBook, simply log into your newly created RTPR.com account (see details below).
ID#: $email
Password: $password

<strong>Personalized Account on RTPR.com</strong>
We have created a new customer account for you on RTPR.com. Use the following login details to access your eBook and to place future orders.

<u><a href='https://rtpr.com/user'>Click here</a></u> to log in and Get Your Free eBook!

<strong>Welcome!</strong>
While you wait for your order to arrive, you can learn more about Real Time Pain Relief by visiting <u><a href='https://rtpr.com'>RTPR.com</a></u>. What will you discover? For over 20 years, Real Time Pain Relief has delivered PAIN RELIEF YOU CAN TRUST® to families across the USA. With targeted formulas that are rich in nature's ingredients, Real Time Pain Relief has lifestyle essentials to suit your every need.

Discover what others have to say about George Foreman's KNOCKOUT Formula™ and more by visiting the Real Time Pain Relief <u><a href='https://www.facebook.com/realtimepainrelief'>Facebook</a></u> page. We look forward to hearing your feedback soon!

<strong>Sincerely</strong>,
The Real Time Pain Relief Processing Team
          ";
          $params = array(
            'subject' => t('George’s Home Ready Guide'),
            'body' => $message,
          );
          drupal_mail('rtpr_user', 'zapier_user', $email, language_default(), $params);
        }
      }
      else {
        include("includes/password.inc");
        if (in_array(46, array_keys($existing_account->roles))) {
          $existing_account->roles = array(7 => 7);
        }
        $user_wrapper = entity_metadata_wrapper('user', $existing_account);

        $user_wrapper->field_funnel_order_id->set($json['OrderNum']);
        $user_wrapper->field_funnel_order_sku->set($json['Sku']);
        $user_wrapper->save();
        $password = user_password();
        $existing_account->pass = user_hash_password($password);
        user_save($existing_account);
        $message = "
Hello!

Thank you for choosing from our <strong>George Foreman™ GoPak Offers!</strong> Within a few days, you will receive shipment confirmation from RTPR.com.

<strong>eBook Available Now</strong>
Your virtual copy of George Foreman's™ Home Remedy Guide to Natural Pain Relief is available today! In this eBook, George Foreman™ is excited to share the following informative articles with you:

<ul>
<li>The Connection Between Sleep and Chronic Pain </li>
<li>Do’s & Don’ts for Knee Pain</li>
<li>Menthol for Topical Pain Relief</li>
<li>7 Tips to Get Relief for Arthritis Pain</li>
<li>Pain Relief Tips for Children</li>
<li>10 Tips to Relieve Muscle Tension</li>
<li>8 Tips to Cope with Headaches</li>
<li>And much more
</li>
</ul>

<strong>Log Into Your RTPR.com Account to Access</strong>
To access your free eBook, simply log into your RTPR.com account.
<u><a href='https://rtpr.com/user'>Click here</a></u> to log in and Get Your Free eBook!

<strong>Learn More!</strong>
While you wait for your order to arrive, you can learn more about Real Time Pain Relief by visiting <u><a href='https://rtpr.com'>RTPR.com</a></u>. What will you discover? For over 20 years, Real Time Pain Relief has delivered PAIN RELIEF YOU CAN TRUST® to families across the USA. With targeted formulas that are rich in nature's ingredients, Real Time Pain Relief has lifestyle essentials to suit your every need.

Discover what others have to say about George Foreman's KNOCKOUT Formula™ and more by visiting the Real Time Pain Relief <u><a href='https://www.facebook.com/realtimepainrelief'>Facebook</a></u> page. We look forward to hearing your feedback soon!

<strong>Sincerely</strong>,
The Real Time Pain Relief Processing Team
";
        $params = array(
          'subject' => t('George’s Home Ready Guide'),
          'body' => $message,
        );
        drupal_mail('rtpr_user', 'zapier_user', $email, language_default(), $params);
        $json['status'] = t('Updated account: @email', array('@email' => $email));
      }
 }