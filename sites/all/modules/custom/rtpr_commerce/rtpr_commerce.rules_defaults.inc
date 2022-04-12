<?php
/**
 * @file
 * rtpr_commerce.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rtpr_commerce_default_rules_configuration() {
  $items = array();
  $items['commerce_shipping_service__100_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__100_flat_rate" : {
      "LABEL" : "Rate $100 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__15_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__15_flat_rate" : {
      "LABEL" : "Rate $15 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__20_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__20_flat_rate" : {
      "LABEL" : "Rate $20 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__35_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__35_flat_rate" : {
      "LABEL" : "Rate $35 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__40_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__40_flat_rate" : {
      "LABEL" : "Rate $40 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__4_95_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__4_95_flat_rate" : {
      "LABEL" : "Rate $4.95 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__50_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__50_flat_rate" : {
      "LABEL" : "Rate $50 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service__70_flat_rate'] = entity_import('rules_config', '{ "commerce_shipping_service__70_flat_rate" : {
      "LABEL" : "Rate $70 Flat rate",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : []
    }
  }');
  $items['commerce_shipping_service_free_shipping'] = entity_import('rules_config', '{ "commerce_shipping_service_free_shipping" : {
      "LABEL" : "Rate Free shipping",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "commerce_order" : { "type" : "commerce_order", "label" : "Order" } },
      "DO" : [
        { "component_rules_delete_shipping_line_item_and_choose_shipping_service" : { "commerce_order" : [ "commerce_order" ] } }
      ]
    }
  }');
  $items['rules_bulk_add_roles'] = entity_import('rules_config', '{ "rules_bulk_add_roles" : {
      "LABEL" : "Bulk add roles",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "user" : { "label" : "User", "type" : "user" },
        "roles" : { "label" : "Roles", "type" : "list\\u003Cinteger\\u003E" }
      },
      "ACTION SET" : [
        { "user_add_role" : { "account" : [ "user" ], "roles" : [ "roles" ] } }
      ]
    }
  }');
  $items['rules_bulk_remove_roles'] = entity_import('rules_config', '{ "rules_bulk_remove_roles" : {
      "LABEL" : "Bulk remove roles",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "user" : { "label" : "User", "type" : "user" },
        "roles" : { "label" : "Roles", "type" : "list\\u003Cinteger\\u003E" }
      },
      "ACTION SET" : [
        { "user_remove_role" : { "account" : [ "user" ], "roles" : [ "roles" ] } }
      ]
    }
  }');
  $items['rules_choose_shipping_service_for_retail'] = entity_import('rules_config', '{ "rules_choose_shipping_service_for_retail" : {
      "LABEL" : "Choose shipping service for Retail customer",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "commerce_shipping" ],
      "USES VARIABLES" : { "commerce_order" : { "label" : "Order", "type" : "commerce_order" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "data_is" : {
                  "data" : [ "commerce-order:commerce-order-total:amount" ],
                  "op" : "\\u003E",
                  "value" : "3995"
                }
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "free_shipping",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if over $39.95 and Retail"
          }
        },
        { "RULE" : {
            "IF" : [
              { "data_is" : {
                  "data" : [ "commerce-order:commerce-order-total:amount" ],
                  "op" : "\\u003C",
                  "value" : "3995"
                }
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_4_95_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if below $39.95 and Retail"
          }
        }
      ]
    }
  }');
  $items['rules_choose_shipping_service_for_wholesale'] = entity_import('rules_config', '{ "rules_choose_shipping_service_for_wholesale" : {
      "LABEL" : "Choose shipping service for Wholesale customer",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "commerce_shipping" ],
      "USES VARIABLES" : { "commerce_order" : { "label" : "Order", "type" : "commerce_order" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "data_is" : {
                  "data" : [ "commerce-order:commerce-order-total:amount" ],
                  "op" : "\\u003C",
                  "value" : "15000"
                }
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_15_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if below $150 and Wholesale"
          }
        },
        { "RULE" : {
            "IF" : [
              { "AND" : [
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003E",
                      "value" : "15100"
                    }
                  },
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003C",
                      "value" : "30000"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_20_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if between $151-$300 and Wholesale"
          }
        },
        { "RULE" : {
            "IF" : [
              { "AND" : [
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003E",
                      "value" : "30100"
                    }
                  },
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003C",
                      "value" : "50000"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_35_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if between $301-$500 and Wholesale"
          }
        },
        { "RULE" : {
            "IF" : [
              { "AND" : [
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003E",
                      "value" : "50100"
                    }
                  },
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003C",
                      "value" : "75000"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_40_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if between $501-$750 and Wholesale"
          }
        },
        { "RULE" : {
            "IF" : [
              { "AND" : [
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003E",
                      "value" : "75100"
                    }
                  },
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003C",
                      "value" : "100000"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_50_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if between $751-$1000 and Wholesale"
          }
        },
        { "RULE" : {
            "IF" : [
              { "AND" : [
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003E",
                      "value" : "100100"
                    }
                  },
                  { "data_is" : {
                      "data" : [ "commerce-order:commerce-order-total:amount" ],
                      "op" : "\\u003E",
                      "value" : "150000"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_70_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if between $1001-$1500 and Wholesale"
          }
        },
        { "RULE" : {
            "IF" : [
              { "data_is" : {
                  "data" : [ "commerce-order:commerce-order-total:amount" ],
                  "op" : "\\u003E",
                  "value" : "150000"
                }
              }
            ],
            "DO" : [
              { "commerce_shipping_service_rate_order" : {
                  "shipping_service_name" : "_100_flat_rate",
                  "commerce_order" : [ "commerce_order" ]
                }
              }
            ],
            "LABEL" : "Add rate for shipping service if over $1500 and Wholesale"
          }
        }
      ]
    }
  }');
  $items['rules_delete_shipping_line_item_and_choose_shipping_service'] = entity_import('rules_config', '{ "rules_delete_shipping_line_item_and_choose_shipping_service" : {
      "LABEL" : "Delete shipping line item and choose shipping service",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_shipping", "rules" ],
      "USES VARIABLES" : { "commerce_order" : { "label" : "Order", "type" : "commerce_order" } },
      "RULES" : [
        { "RULE" : {
            "DO" : [
              { "commerce_shipping_delete_shipping_line_items" : { "commerce_order" : [ "commerce_order" ] } }
            ],
            "LABEL" : "Delete shipping line items"
          }
        },
        { "RULE" : {
            "IF" : [
              { "NOT user_has_role" : {
                  "account" : [ "site:current-user" ],
                  "roles" : { "value" : {
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "13" : "13",
                      "15" : "15",
                      "20" : "20"
                    }
                  },
                  "operation" : "OR"
                }
              }
            ],
            "DO" : [
              { "component_rules_choose_shipping_service_for_retail" : { "commerce_order" : [ "commerce_order" ] } }
            ],
            "LABEL" : "Choose shipping service for Retail customer"
          }
        },
        { "RULE" : {
            "IF" : [
              { "user_has_role" : {
                  "account" : [ "site:current-user" ],
                  "roles" : { "value" : {
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "13" : "13",
                      "15" : "15",
                      "20" : "20"
                    }
                  },
                  "operation" : "OR"
                }
              }
            ],
            "DO" : [
              { "component_rules_choose_shipping_service_for_wholesale" : { "commerce_order" : [ "commerce_order" ] } }
            ],
            "LABEL" : "Choose shipping service for Wholesale customer"
          }
        }
      ]
    }
  }');
  $items['rules_register_completed_cart_sale_with_post_affiliate_pro'] = entity_import('rules_config', '{ "rules_register_completed_cart_sale_with_post_affiliate_pro" : {
      "LABEL" : "Register Completed Cart Sale With Post Affiliate Pro",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rtpr_commerce", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "DO" : [
        { "rtpr_commerce_sales_notify" : { "commerce_order" : [ "commerce_order" ] } }
      ]
    }
  }');
  $items['rules_wholesaler_price'] = entity_import('rules_config', '{ "rules_wholesaler_price" : {
      "LABEL" : "Wholesaler price",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "commerce_line_item", "commerce_product_reference" ],
      "ON" : { "commerce_product_calculate_sell_price" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "commerce-line-item" ], "field" : "commerce_product" } },
        { "entity_has_field" : {
            "entity" : [ "commerce-line-item:commerce-product" ],
            "field" : "field_wholesaler_price"
          }
        },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "3" : "3" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "commerce_line_item_unit_price_amount" : {
            "commerce_line_item" : [ "commerce_line_item" ],
            "amount" : [ "commerce-line-item:commerce-product:field-wholesaler-price:amount" ],
            "component_name" : "base_price",
            "round_mode" : "1"
          }
        }
      ]
    }
  }');
  return $items;
}
