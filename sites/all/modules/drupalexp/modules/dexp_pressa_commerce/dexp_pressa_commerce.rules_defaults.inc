<?php
/**
 * @file
 * dexp_pressa_commerce.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function dexp_pressa_commerce_default_rules_configuration() {
  $items = array();
  $items['rules_discount'] = entity_import('rules_config', '{ "rules_discount" : {
      "LABEL" : "Discount 5 percent on specific products",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "commerce_line_item", "commerce_product_reference" ],
      "ON" : { "commerce_product_calculate_sell_price" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "commerce-line-item" ], "field" : "commerce_product" } },
        { "component_rules_product_on_sale" : { "product" : [ "commerce-line-item:commerce-product" ] } }
      ],
      "DO" : [
        { "commerce_line_item_unit_price_multiply" : {
            "commerce_line_item" : [ "commerce_line_item" ],
            "amount" : "0.9",
            "component_name" : "discount",
            "round_mode" : "1"
          }
        }
      ]
    }
  }');
  $items['rules_product_on_sale'] = entity_import('rules_config', '{ "rules_product_on_sale" : {
      "LABEL" : "Product On Sale",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "product" : { "label" : "Product", "type" : "commerce_product" } },
      "AND" : [
        { "entity_has_field" : { "entity" : [ "product" ], "field" : "field_sale_event" } },
        { "NOT data_is_empty" : { "data" : [ "product:field-sale-event" ] } }
      ]
    }
  }');
  return $items;
}
