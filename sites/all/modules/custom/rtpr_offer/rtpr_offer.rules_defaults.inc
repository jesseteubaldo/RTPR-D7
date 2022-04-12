<?php
/**
 * @file
 * rtpr_offer.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rtpr_offer_default_rules_configuration() {
  $items = array();
  $items['rtpr_user_set_sample_role_commission_groups'] = entity_import('rules_config', '{ "rtpr_user_set_sample_role_commission_groups" : {
      "LABEL" : "Set Sample Role Commission Groups",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "sampler" ],
      "REQUIRES" : [ "rules", "rtpr_user" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "248",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430202419,
                          "datetime_end" : 1430202419
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "248",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430202384,
                          "datetime_end" : 1430202384
                        }
                      }
                    ]
                  }
                ]
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430256699,
                          "datetime_end" : 1430256699
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430256661,
                          "datetime_end" : 1430256661
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and $49 \\u003C MSP (last month or previous month) \\u003C $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430188323,
                          "datetime_end" : 1430188323
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430188292,
                          "datetime_end" : 1430188292
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "22" : "22", "23" : "23" } }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $449 and MSP (last month or previous month) \\u003E $449"
          }
        }
      ]
    }
  }');
  $items['rules_add_free_tube_to_cart'] = entity_import('rules_config', '{ "rules_add_free_tube_to_cart" : {
      "LABEL" : "Add Free Tube To Cart",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "cart" ],
      "REQUIRES" : [
        "rules",
        "rtpr_offer",
        "commerce_discount",
        "commerce_order",
        "commerce_cart",
        "entity"
      ],
      "ON" : {
        "commerce_cart_product_add" : [],
        "rtpr_offer_commerce_line_update" : [],
        "commerce_order_update" : []
      },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "7" : "7" } }
          }
        },
        { "rtpr_offer_first_time_buyer" : { "account" : [ "site:current-user" ] } },
        { "commerce_order_compare_order_amount" : {
            "commerce_order" : [ "commerce_order" ],
            "operator" : "\\u003E=",
            "total" : { "value" : { "amount" : 1000, "currency_code" : "USD" } },
            "line_item_types" : { "value" : { "product" : "product" } }
          }
        },
        { "commerce_order_contains_product" : {
            "commerce_order" : [ "commerce_order" ],
            "product_id" : "FREE-TUBE-FOR-NEW",
            "operator" : "\\u003C",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "commerce_cart_product_add_by_sku" : {
            "USING" : {
              "user" : [ "site:current-user" ],
              "sku" : "FREE-TUBE-FOR-NEW",
              "quantity" : "1",
              "combine" : "1"
            },
            "PROVIDE" : { "product_add_line_item" : { "product_add_line_item" : "Added product line item" } }
          }
        }
      ]
    }
  }');
  $items['rules_limit_product_for_sampler'] = entity_import('rules_config', '{ "rules_limit_product_for_sampler" : {
      "LABEL" : "Limit product for Sampler",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "rtpr" ],
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : { "commerce_line_item_insert" : [], "commerce_line_item_update" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "37" : "37" } }
          }
        },
        { "data_is" : {
            "data" : [ "commerce-line-item:quantity" ],
            "op" : "\\u003E",
            "value" : "5"
          }
        },
        { "entity_has_field" : { "entity" : [ "commerce-line-item" ], "field" : "commerce_product" } },
        { "text_matches" : {
            "text" : [ "commerce-line-item:commerce-product:sku" ],
            "match" : "-SP"
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "commerce-line-item:quantity" ], "value" : "5" } }
      ]
    }
  }');
  $items['rules_remove_free_tube_from_cart'] = entity_import('rules_config', '{ "rules_remove_free_tube_from_cart" : {
      "LABEL" : "Remove free tube from cart",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "cart" ],
      "REQUIRES" : [ "commerce_discount", "rtpr_offer", "commerce_cart" ],
      "ON" : {
        "commerce_cart_product_remove" : [],
        "rtpr_offer_commerce_line_update" : []
      },
      "IF" : [
        { "commerce_order_compare_order_amount" : {
            "commerce_order" : [ "commerce_order" ],
            "operator" : "\\u003C",
            "total" : { "value" : { "amount" : 1000, "currency_code" : "USD" } },
            "line_item_types" : { "value" : { "product" : "product" } }
          }
        }
      ],
      "DO" : [
        { "rtpr_offer_product_remove_by_sku" : { "string" : "FREE-TUBE-FOR-NEW" } }
      ]
    }
  }');
  $items['rules_sampler_control_panel_redirect'] = entity_import('rules_config', '{ "rules_sampler_control_panel_redirect" : {
      "LABEL" : "Sampler control panel Redirect",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "redirects" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "37" : "37" } },
            "operation" : "OR"
          }
        },
        { "NOT text_matches" : {
            "text" : [ "site:current-page:path" ],
            "match" : "^user\\/reset\\/",
            "operation" : "regex"
          }
        }
      ],
      "DO" : [ { "redirect" : { "url" : "backoffice\\/sampler-control-panel" } } ]
    }
  }');
  $items['rules_set_item_in_cart_to_1_quantity'] = entity_import('rules_config', '{ "rules_set_item_in_cart_to_1_quantity" : {
      "LABEL" : "Set item in cart to 1 quantity",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : { "commerce_line_item_update" : [] },
      "IF" : [
        { "data_is" : {
            "data" : [ "commerce-line-item:quantity" ],
            "op" : "\\u003E",
            "value" : "1"
          }
        },
        { "data_is" : { "data" : [ "commerce-line-item:type" ], "value" : "product" } },
        { "text_matches" : {
            "text" : [ "commerce-line-item:commerce-product:sku" ],
            "match" : "FREE-TUBE-FOR-NEW"
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "commerce-line-item:quantity" ], "value" : "1" } }
      ]
    }
  }');
  $items['rules_update_user_role_to_sampler'] = entity_import('rules_config', '{ "rules_update_user_role_to_sampler" : {
      "LABEL" : "Update User Role to Sampler",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "8",
      "OWNER" : "rules",
      "TAGS" : [ "sampler" ],
      "REQUIRES" : [ "rules", "commerce_order", "rtpr_user", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "IF" : [
        { "AND" : [
            { "user_has_role" : {
                "account" : [ "commerce-order:owner" ],
                "roles" : { "value" : { "4" : "4" } }
              }
            },
            { "OR" : [
                { "commerce_order_contains_product" : {
                    "commerce_order" : [ "commerce_order" ],
                    "product_id" : "BRONZE-PACKAGE",
                    "operator" : "\\u003E=",
                    "value" : "1"
                  }
                },
                { "commerce_order_contains_product" : {
                    "commerce_order" : [ "commerce_order" ],
                    "product_id" : "SILVER-PACKAGE",
                    "operator" : "\\u003E=",
                    "value" : "1"
                  }
                }
              ]
            }
          ]
        }
      ],
      "DO" : [
        { "user_add_role" : {
            "account" : [ "commerce-order:owner" ],
            "roles" : { "value" : { "30" : "30", "37" : "37" } }
          }
        },
        { "rtpr_user_action_remove_commissiongroup" : {
            "account" : [ "commerce-order:owner" ],
            "commissiongroup" : { "value" : {
                "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                "c602cb08|7720362e" : "c602cb08|7720362e",
                "c602cb08|d7df1364" : "c602cb08|d7df1364",
                "c602cb08|7c51e99a" : "c602cb08|7c51e99a",
                "39e673de|8a65e5be" : "39e673de|8a65e5be",
                "39e673de|98f04a65" : "39e673de|98f04a65",
                "39e673de|ae95c184" : "39e673de|ae95c184",
                "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                "11111111|fd3dc104" : "11111111|fd3dc104",
                "11111111|bd4a45e0" : "11111111|bd4a45e0",
                "11111111|e928564e" : "11111111|e928564e"
              }
            }
          }
        },
        { "rtpr_user_action_add_commissiongroup" : {
            "account" : [ "commerce-order:owner" ],
            "commissiongroup" : { "value" : {
                "c5d380ba|defe8bfe" : "c5d380ba|defe8bfe",
                "c602cb08|cf1768d2" : "c602cb08|cf1768d2",
                "39e673de|2d158699" : "39e673de|2d158699",
                "11111111|2f3f03da" : "11111111|2f3f03da"
              }
            },
            "status" : "A"
          }
        },
        { "user_remove_role" : {
            "account" : [ "commerce-order:owner" ],
            "roles" : { "value" : { "4" : "4", "30" : "30" } }
          }
        }
      ]
    }
  }');
  $items['rules_update_vendor_role_to_sampler'] = entity_import('rules_config', '{ "rules_update_vendor_role_to_sampler" : {
      "LABEL" : "Update Vendor Role to Sampler",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "8",
      "OWNER" : "rules",
      "TAGS" : [ "sampler" ],
      "REQUIRES" : [
        "rules",
        "commerce_order",
        "rtpr_user",
        "rtpr_extras",
        "commerce_checkout"
      ],
      "ON" : { "commerce_checkout_complete" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "commerce-order:owner" ],
            "roles" : { "value" : { "8" : "8", "23" : "23", "24" : "24", "36" : "36" } },
            "operation" : "OR"
          }
        },
        { "commerce_order_contains_product" : {
            "commerce_order" : [ "commerce_order" ],
            "product_id" : "BRONZE-PACKAGE",
            "operator" : "\\u003E=",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "rtpr_user_action_remove_commissiongroup" : {
            "account" : [ "commerce-order:owner" ],
            "commissiongroup" : { "value" : {
                "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                "c602cb08|7720362e" : "c602cb08|7720362e",
                "c602cb08|d7df1364" : "c602cb08|d7df1364",
                "c602cb08|7c51e99a" : "c602cb08|7c51e99a",
                "39e673de|8a65e5be" : "39e673de|8a65e5be",
                "39e673de|98f04a65" : "39e673de|98f04a65",
                "39e673de|ae95c184" : "39e673de|ae95c184",
                "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                "11111111|fd3dc104" : "11111111|fd3dc104",
                "11111111|bd4a45e0" : "11111111|bd4a45e0",
                "11111111|e928564e" : "11111111|e928564e"
              }
            }
          }
        },
        { "rtpr_user_action_add_commissiongroup" : {
            "account" : [ "commerce-order:owner" ],
            "commissiongroup" : { "value" : {
                "c5d380ba|defe8bfe" : "c5d380ba|defe8bfe",
                "c602cb08|cf1768d2" : "c602cb08|cf1768d2",
                "39e673de|2d158699" : "39e673de|2d158699",
                "11111111|2f3f03da" : "11111111|2f3f03da"
              }
            },
            "status" : "A"
          }
        },
        { "rtpr_extras_remove_role_by_id" : { "number" : "8", "user" : [ "commerce-order:owner" ] } },
        { "rtpr_extras_remove_role_by_id" : { "number" : "23", "user" : [ "commerce-order:owner" ] } },
        { "rtpr_extras_remove_role_by_id" : { "number" : "24", "user" : [ "commerce-order:owner" ] } },
        { "rtpr_extras_add_role_by_id" : { "number" : "37", "user" : [ "commerce-order:owner" ] } },
        { "rtpr_extras_remove_role_by_id" : { "number" : "36", "user" : [ "commerce-order:owner" ] } }
      ]
    }
  }');
  $items['rules_upgrade_sampler_to_platinum_winner_circle'] = entity_import('rules_config', '{ "rules_upgrade_sampler_to_platinum_winner_circle" : {
      "LABEL" : "Upgrade Sampler to Platinum Winner-circle",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "10",
      "OWNER" : "rules",
      "TAGS" : [ "sampler" ],
      "REQUIRES" : [ "rules", "commerce_order", "rtpr_extras", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "commerce-order:owner" ],
            "roles" : { "value" : { "37" : "37" } }
          }
        },
        { "commerce_order_contains_product" : {
            "commerce_order" : [ "commerce_order" ],
            "product_id" : "PLT-K-WS",
            "operator" : "\\u003E=",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "rtpr_extras_remove_role_by_id" : { "number" : "37", "user" : [ "commerce-order:owner" ] } },
        { "rtpr_extras_add_role_by_id" : { "number" : "22", "user" : [ "commerce-order:owner" ] } }
      ]
    }
  }');
  return $items;
}
