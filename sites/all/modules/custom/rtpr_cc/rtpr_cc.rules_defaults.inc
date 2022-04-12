<?php
/**
 * @file
 * rtpr_cc.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rtpr_cc_default_rules_configuration() {
  $items = array();
  $items['rules_add_commission_group_for_online_affiliate'] = entity_import('rules_config', '{ "rules_add_commission_group_for_online_affiliate" : {
      "LABEL" : "Add commission group for online affiliate",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_user" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "39" : "39", "38" : "38" } },
            "operation" : "OR"
          }
        },
        { "NOT user_has_role" : {
            "account" : [ "account-unchanged" ],
            "roles" : { "value" : { "39" : "39", "38" : "38" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "rtpr_user_action_add_commissiongroup" : {
            "account" : [ "account" ],
            "commissiongroup" : { "value" : { "11111111|dd6d8eec" : "11111111|dd6d8eec" } },
            "status" : "A"
          }
        }
      ]
    }
  }');
  $items['rules_already_a_potential_customer'] = entity_import('rules_config', '{ "rules_already_a_potential_customer" : {
      "LABEL" : "Already a potential customer",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_cc" ],
      "ON" : { "rtpr_cc_notify_event" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "redemption_potential" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "customer:mail",
            "subject" : "RTPR Online Gift Card Activation",
            "message" : "Dear [customer:field-first-name],\\r\\n\\r\\nYou recently purchased Real Time Pain Relief at an event, and as result you have two special \\u201cWelcome to the RTPR Family\\u201d gifts available to you:\\r\\n\\r\\n\\u003Col\\u003E\\r\\n\\u003Cli\\u003E$15.00 Online Gift Card \\u2013 Click the link below to activate your account!\\u003C\\/li\\u003E\\r\\n\\u003Cli\\u003EPlus, you\\u2019ll receive a free $10 Tube when you use your Gift Card.\\u003C\\/li\\u003E\\r\\n\\u003C\\/ol\\u003E\\r\\n\\r\\nWhen you access the link below to Activate your account, you will set your password and then your Online Gift Card will be available for use immediately:\\r\\n\\r\\n\\u003C?php echo user_pass_reset_url($customer) . \\u0027?parent=\\u0027?\\u003E[vendor:uid]\\r\\n\\r\\n\\r\\nReal Time Pain Relief is a family owned business that is committed to providing the highest quality products at affordable prices. We can do this because we sell the majority of our products at events and online. In both cases, we cut out the middleman to give you great value.\\r\\n\\r\\nEnjoy!\\r\\n\\r\\nRTPR Gift Card Processing",
            "from" : "support@rtpr.com",
            "language" : [ "customer:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_gcc_customer_processed'] = entity_import('rules_config', '{ "rules_gcc_customer_processed" : {
      "LABEL" : "GCC Customer Processed",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_cc" ],
      "ON" : { "rtpr_cc_notify_event" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "successfully_redemp" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "vendor:mail" ],
            "subject" : "GCC Customer Processed\\r\\n",
            "message" : "Dear [vendor:field-first-name],\\r\\n\\r\\nWe have processed the Gift Card Receipt for your Customer:\\r\\n\\r\\n\\u003Cstrong\\u003E[customer:field-first-name] [customer:field-last-name]\\u003Cstrong\\u003E\\r\\n\\r\\nYou may access you\\u2019re the Gift Card Processing area of your backoffice to view the current status of your submitted Customers.\\r\\n\\r\\nSincerely,\\r\\n\\r\\nRTPR Gift Card Processing\\r\\n",
            "from" : "support@rtpr.com",
            "language" : [ "vendor:language" ]
          }
        },
        { "mail" : {
            "to" : [ "customer:mail" ],
            "subject" : "RTPR Online Gift Card Activation",
            "message" : "Dear [customer:field-first-name],\\r\\n\\r\\nYou recently purchased Real Time Pain Relief at an event, and as result you have two special \\u201cWelcome to the RTPR Family\\u201d gifts available to you:\\r\\n\\r\\n\\u003Col\\u003E\\r\\n\\u003Cli\\u003E$15.00 Online Gift Card \\u2013 Click the link below to activate your account!\\u003C\\/li\\u003E\\r\\n\\u003Cli\\u003EPlus, you\\u2019ll receive a free $10 Tube when you use your Gift Card.\\u003C\\/li\\u003E\\r\\n\\u003C\\/ol\\u003E\\r\\n\\r\\nWhen you access the link below to Activate your account, you will set your password and then your Online Gift Card will be available for use immediately:\\r\\n\\r\\n\\u003C?php echo user_pass_reset_url($customer) . \\u0027?parent=\\u0027?\\u003E[vendor:uid]\\r\\n\\r\\n\\r\\nReal Time Pain Relief is a family owned business that is committed to providing the highest quality products at affordable prices. We can do this because we sell the majority of our products at events and online. In both cases, we cut out the middleman to give you great value.\\r\\n\\r\\nEnjoy!\\r\\n\\r\\nRTPR Gift Card Processing",
            "from" : "support@rtpr.com",
            "language" : [ "customer:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_gift_card_now_activated'] = entity_import('rules_config', '{ "rules_gift_card_now_activated" : {
      "LABEL" : "Gift Card Now Activated",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_cc" ],
      "ON" : { "rtpr_cc_notify_event" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "potential_customer_activate" } },
        { "user_has_role" : { "account" : [ "customer" ], "roles" : { "value" : { "46" : "46" } } } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "customer:mail" ],
            "subject" : "Gift Card Now Activated",
            "message" : "Dear [customer:field-first-name],\\r\\n\\r\\nYour RTPR Customer Account and Gift Card has now been activated. Your $15 Online Gift Card can be used immediately. Here are a few tips that will help you maximize the value of your Gift Card!\\r\\n\\r\\n\\u003Col\\u003E\\r\\n\\u003Cli\\u003E\\u003Cstrong\\u003EExtra Gift with First Order\\u003C\\/strong\\u003E\\r\\nReal Time Pain Relief is a family owned business that is committed to giving you more pain relief for your dollar. Because of this, a free tube of Real Time Pain Relief will be added to your first order. Welcome to the RTPR family!\\u003C\\/li\\u003E\\r\\n\\u003Cli\\u003E\\u003Cstrong\\u003EFREE $10 Tube when you spend $25 or more\\u003C\\/strong\\u003E\\r\\nEach month Real Time Pain Relief has a featured product of the month \\u2013 When you spend $25 or more, you get a free $10 tube of that featured product.\\u003C\\/li\\u003E\\r\\n\\u003Cli\\u003E\\u003Cstrong\\u003EFree Shipping\\u003C\\/strong\\u003E\\r\\nReceive Free Shipping on all orders $39.95 or over! \\u003C\\/li\\u003E\\r\\n\\u003C\\/ol\\u003E\\r\\n\\r\\n\\u003Ca href=\\u0022https:\\/\\/rtpr.com\\u0022\\u003EClick Here\\u003C\\/a\\u003E to login and shop! \\r\\n\\r\\nSincerely,\\r\\nRTPR\\r\\n",
            "from" : "support@rtpr.com",
            "language" : [ "customer:language" ]
          }
        },
        { "mail" : {
            "to" : [ "vendor:mail" ],
            "subject" : "GCC Customer Activated",
            "message" : "Dear [vendor:field-first-name],\\r\\n\\r\\nA Gift Card Receipt Customer has activated their account\\u201d\\r\\n\\r\\n\\u003Cstrong\\u003E[customer:field-first-name] [customer:field-last-name]\\u003C\\/strong\\u003E\\r\\n\\r\\nYou may access you\\u2019re the Gift Card Processing area of your backoffice to view more information on your submitted Customers.\\r\\n\\r\\nSincerely,\\r\\n\\r\\nRTPR Gift Card Processing\\r\\n",
            "from" : "support@rtpr.com",
            "language" : [ "vendor:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_vendor_gift_card_credits_needed'] = entity_import('rules_config', '{ "rules_vendor_gift_card_credits_needed" : {
      "LABEL" : "Vendor Gift Card Credits Needed",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_cc" ],
      "ON" : { "rtpr_cc_notify_event" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "vendor_no_credits" } }
      ],
      "DO" : [
        { "drupal_message" : { "message" : "Email with subject: \\u003Cstrong\\u003EUrgent: Vendor Gift Card Credits Needed\\u003C\\/strong\\u003E send to vendor." } },
        { "mail" : {
            "to" : "[vendor:mail]",
            "subject" : "Urgent: Vendor Gift Card Credits Needed",
            "message" : "Dear [vendor:field-first-name],\\r\\n\\r\\nWe have received from you one or more Customer Gift Card Receipts for which you do not have the Vendor Gift Card Credits necessary for us to process.  Please go to the Vendor Gift Card Credit Area to purchase packages that include Vendor Gift Card Credits as soon as possible so that we can promptly process your Gift Card Receipts.\\r\\n\\r\\nhttps:\\/\\/rtpr.com\\/backoffice\\/vendor-gift-card-credits\\r\\n\\r\\n\\u003Cstrong\\u003EPlease note:\\u003C\\/strong\\u003E We will reattempt to process your Gift Card Receipts in a few days, however, if you do not have the Vendor Gift Card Credits to process these Customer Gift Card Receipts within 14 days, you will lose these customers as we will be forced to proceed with processing them to meet our obligation to the customer.\\r\\n\\r\\nSincerely,\\r\\nRTPR Gift Card Processing",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_your_rtpr_coupon'] = entity_import('rules_config', '{ "rules_your_rtpr_coupon" : {
      "LABEL" : "Your RTPR Coupon",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_cc" ],
      "ON" : { "rtpr_cc_notify_event" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "redemption_retail" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "customer:mail" ],
            "subject" : "Your RTPR Coupon",
            "message" : "Dear [customer:field-first-name].\\r\\n\\r\\nWe recently received a Free $15 Gift Card Receipt, submitted on your behalf, by an RTPR Vendor you met at an event. As noted on the Gift Card Receipt you received, the free $15 Gift Card Offer is only valid for first time purchasers on RTPR.com. Unfortunately, you are already registered as a customer in our system with the Email Address you provided on the Gift Card form. For this reason, you are ineligible for the $15 Gift Card offer.\\r\\n\\r\\nHowever, we have good news. As a valuable RTPR customer, we are offering you a 10% discount on your next purchase at RTPR.com. To apply this discount to your next order, please enter the following Coupon Code at checkout: \\r\\n\\r\\n\\u003Cstrong\\u003E[extra:value]\\u003C\\/strong\\u003E\\r\\n\\r\\nWhen entered, this coupon code will apply a 10% discount to your order at RPTR.com. This Coupon Code expires on (60 day expiration on the coupon code)\\r\\n\\r\\n\\r\\nSincerely,\\r\\n\\r\\nRTPR Gift Card Processing",
            "from" : "support@rtpr.com",
            "language" : [ "customer:language" ]
          }
        },
        { "mail" : {
            "to" : [ "vendor:mail" ],
            "subject" : "GCC failed to process\\r\\n",
            "message" : "Dear [vendor:field-first-name],\\r\\n\\r\\nWe recently received a Gift Card Receipt from you, on behalf for the following individual: \\r\\n\\r\\n\\u003Cstrong\\u003E[customer:field-first-name] [customer:field-last-name]\\u003C\\/strong\\u003E\\r\\n\\r\\nHowever, this individual is already an existing customer in our system, with one or more previous orders placed at RTPR.com. As a result, this customer is ineligible for the offer, and is already hard-coded to another account. \\r\\n\\r\\nBecause this customer will not be hard-coded to you, we will not deduct a Vendor Gift Card Credit from your account for the Gift Card Receipt you submitted for the customer.  This Gift Card Credit will remain available for processing of Gift Card Receipt for an eligible customer.\\r\\n\\r\\nSincerely,\\r\\n\\r\\nRTPR Gift Card Processing",
            "from" : "support@rtpr.com",
            "language" : [ "vendor:language" ]
          }
        },
        { "drupal_message" : { "message" : "Email sent to vendor and retail customer!" } }
      ]
    }
  }');
  $items['rules_your_rtpr_coupon_non_retail'] = entity_import('rules_config', '{ "rules_your_rtpr_coupon_non_retail" : {
      "LABEL" : "Your RTPR Coupon - Non-retail",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Call Center" ],
      "REQUIRES" : [ "rules", "rtpr_cc" ],
      "ON" : { "rtpr_cc_notify_event" : [] },
      "IF" : [
        { "text_matches" : { "text" : [ "type" ], "match" : "redemption_non_retail" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "customer:mail" ],
            "subject" : "Your RTPR Coupon",
            "message" : "Dear [customer:field-first-name].\\r\\n\\r\\nWe recently received a Free $15 Gift Card Receipt, submitted on your behalf, by an RTPR Vendor you met at an event. As noted on the Gift Card Receipt you received, the free $15 Gift Card Offer is only valid for first time purchasers on RTPR.com. Unfortunately, you are already registered in our system with the Email Address you provided on the Gift Card form. For this reason, you are ineligible for the $15 Gift Card offer.\\r\\n\\r\\nSincerely,\\r\\n\\r\\nRTPR Gift Card Processing\\r\\n",
            "from" : "support@rtpr.com",
            "language" : [ "customer:language" ]
          }
        },
        { "mail" : {
            "to" : [ "vendor:mail" ],
            "subject" : "GCC failed to process\\r\\n",
            "message" : "Dear [vendor:field-first-name],\\r\\n\\r\\nWe recently received a Gift Card Receipt from you, on behalf for the following individual: \\r\\n\\r\\n\\u003Cstrong\\u003E[customer:field-first-name] [customer:field-last-name]\\u003C\\/strong\\u003E\\r\\n\\r\\nHowever, this individual is already an existing customer in our system, with one or more previous orders placed at RTPR.com. As a result, this customer is ineligible for the offer, and is already hard-coded to another account. \\r\\n\\r\\nBecause this customer will not be hard-coded to you, we will not deduct a Vendor Gift Card Credit from your account for the Gift Card Receipt you submitted for the customer.  This Gift Card Credit will remain available for processing of Gift Card Receipt for an eligible customer.\\r\\n\\r\\nSincerely,\\r\\n\\r\\nRTPR Gift Card Processing\\r\\n",
            "from" : "support@rtpr.com",
            "language" : [ "vendor:language" ]
          }
        },
        { "drupal_message" : { "message" : "Email sent to vendor and retail customer!" } }
      ]
    }
  }');
  return $items;
}
