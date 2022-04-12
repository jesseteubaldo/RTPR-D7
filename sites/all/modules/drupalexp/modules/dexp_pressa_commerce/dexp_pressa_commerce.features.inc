<?php
/**
 * @file
 * dexp_pressa_commerce.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dexp_pressa_commerce_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "file_entity" && $api == "file_default_displays") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dexp_pressa_commerce_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function dexp_pressa_commerce_flag_default_flags() {
  $flags = array();
  // Exported flag: "Wishlist".
  $flags['wishlist'] = array(
    'entity_type' => 'commerce_product',
    'title' => 'Wishlist',
    'global' => 0,
    'types' => array(
      0 => 'product',
    ),
    'flag_short' => '<i class=\'fa fa-heart\'></i><span>Add to Wishlist</span>',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => '<i class="fa fa-trash-o"></i><span>Remove Wishlist</span>',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 'full',
      'node_full' => 'node_full',
      'node_teaser' => 'node_teaser',
      'token' => 0,
      'line_item' => 0,
      'commerce_line_item_display' => 0,
      'commerce_line_item_token' => 0,
      'node_rss' => 0,
      'node_search_index' => 0,
      'node_search_result' => 0,
      'node_dexp_portfolio' => 0,
      'node_large_title' => 0,
      'node_large' => 0,
      'node_medium' => 0,
      'node_hover_image' => 0,
      'node_portfolio_bxslider' => 0,
      'node_token' => 0,
    ),
    'show_as_field' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'api_version' => 3,
    'module' => 'dexp_pressa_commerce',
    'locked' => array(
      0 => 'name',
    ),
  );
  return $flags;

}

/**
 * Implements hook_node_info().
 */
function dexp_pressa_commerce_node_info() {
  $items = array(
    'article' => array(
      'name' => t('Article'),
      'base' => 'node_content',
      'description' => t('Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'clients' => array(
      'name' => t('Clients'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'dexp_portfolio' => array(
      'name' => t('Portfolio'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'gallery' => array(
      'name' => t('Gallery'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'product_display' => array(
      'name' => t('Product Display'),
      'base' => 'node_content',
      'description' => t('This content type is used for show product on page'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'team' => array(
      'name' => t('Team'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_default_search_api_index().
 */
function dexp_pressa_commerce_default_search_api_index() {
  $items = array();
  $items['default_node_index'] = entity_import('search_api_index', '{
    "name" : "Default node index",
    "machine_name" : "default_node_index",
    "description" : "An automatically created search index for indexing node data. Might be configured to specific needs.",
    "server" : "orane",
    "item_type" : "node",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "data_alter_callbacks" : { "search_api_alter_node_access" : { "status" : 1, "weight" : "0", "settings" : [] } },
      "processors" : {
        "search_api_case_ignore" : { "status" : 1, "weight" : "0", "settings" : { "strings" : 0 } },
        "search_api_html_filter" : {
          "status" : 1,
          "weight" : "10",
          "settings" : {
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\nh2 = 3\\nh3 = 2\\nstrong = 2\\nb = 2\\nem = 1.5\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 1,
          "weight" : "20",
          "settings" : { "spaces" : "[^\\\\p{L}\\\\p{N}]", "ignorable" : "[-]" }
        }
      },
      "fields" : {
        "author" : { "type" : "integer", "entity_type" : "user" },
        "body:value" : { "type" : "text" },
        "changed" : { "type" : "date" },
        "comment_count" : { "type" : "integer" },
        "created" : { "type" : "date" },
        "promote" : { "type" : "boolean" },
        "search_api_language" : { "type" : "string" },
        "sticky" : { "type" : "boolean" },
        "title" : { "type" : "text", "boost" : "5.0" },
        "type" : { "type" : "string" }
      },
      "sorts_widget" : {
        "enabled" : false,
        "title" : "",
        "autosubmit" : false,
        "autosubmit_hide" : false
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function dexp_pressa_commerce_default_search_api_server() {
  $items = array();
  $items['orane'] = entity_import('search_api_server', '{
    "name" : "Pressa",
    "machine_name" : "orane",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "1",
      "partial_matches" : 0,
      "indexes" : {
        "product_display" : {
          "nid" : {
            "table" : "search_api_db_product_display",
            "column" : "nid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "type" : {
            "table" : "search_api_db_product_display",
            "column" : "type",
            "type" : "string",
            "boost" : "1.0"
          },
          "title" : {
            "table" : "search_api_db_product_display",
            "column" : "title",
            "type" : "string",
            "boost" : "1.0"
          },
          "status" : {
            "table" : "search_api_db_product_display",
            "column" : "status",
            "type" : "integer",
            "boost" : "1.0"
          },
          "created" : {
            "table" : "search_api_db_product_display",
            "column" : "created",
            "type" : "date",
            "boost" : "1.0"
          },
          "changed" : {
            "table" : "search_api_db_product_display",
            "column" : "changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "field_product_catalog" : {
            "table" : "search_api_db_product_display",
            "column" : "field_product_catalog",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_brand" : {
            "table" : "search_api_db_product_display",
            "column" : "field_brand",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_product" : {
            "table" : "search_api_db_product_display_field_product",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_product_display",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_aggregation_1" : {
            "table" : "search_api_db_product_display_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "search_api_aggregation_5" : {
            "table" : "search_api_db_product_display_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "search_api_aggregation_6" : {
            "table" : "search_api_db_product_display_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_product_catalog:name" : {
            "table" : "search_api_db_product_display",
            "column" : "field_product_catalog_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_brand:name" : {
            "table" : "search_api_db_product_display",
            "column" : "field_brand_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_product:field_color" : {
            "table" : "search_api_db_product_display_field_product_field_color",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_product:field_size" : {
            "table" : "search_api_db_product_display_field_product_field_size",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_availbility:name" : {
            "table" : "search_api_db_product_display",
            "column" : "field_availbility_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_product:commerce_price:amount_decimal" : {
            "table" : "search_api_db_product_display_field_product_commerce_price_amo",
            "column" : "value",
            "type" : "list\\u003Cdecimal\\u003E",
            "boost" : "1.0"
          },
          "field_product:field_product_characteristics:name" : {
            "table" : "search_api_db_product_display_field_product_field_product_ch_1",
            "column" : "value",
            "type" : "list\\u003Clist\\u003Cstring\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_product:field_color:name" : {
            "table" : "search_api_db_product_display_field_product_field_color_name",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "field_product:field_size:name" : {
            "table" : "search_api_db_product_display_field_product_field_size_name",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "taxonomy_forums:name" : {
            "table" : "search_api_db_product_display",
            "column" : "taxonomy_forums_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_product:field_sale_event:name" : {
            "table" : "search_api_db_product_display_field_product_field_sale_event_n",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "field_tags" : {
            "table" : "search_api_db_product_display_field_tags",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_blog_categories" : {
            "table" : "search_api_db_product_display_field_blog_categories",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_portfolio_categories" : {
            "table" : "search_api_db_product_display_field_portfolio_categories",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_availbility" : {
            "table" : "search_api_db_product_display",
            "column" : "field_availbility",
            "type" : "integer",
            "boost" : "1.0"
          },
          "taxonomy_forums" : {
            "table" : "search_api_db_product_display",
            "column" : "taxonomy_forums",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_tags:name" : {
            "table" : "search_api_db_product_display_field_tags_name",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "field_blog_categories:name" : {
            "table" : "search_api_db_product_display_field_blog_categories_name",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "field_portfolio_categories:name" : {
            "table" : "search_api_db_product_display_field_portfolio_categories_name",
            "column" : "value",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "field_product:field_product_characteristics" : {
            "table" : "search_api_db_product_display_field_product_field_product_char",
            "column" : "value",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_product:field_sale_event" : {
            "table" : "search_api_db_product_display_field_product_field_sale_event",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_product_commerce_price_amount_decimal_asc" : {
            "table" : "search_api_db_product_display",
            "column" : "field_product_commerce_price_amount_decimal_asc",
            "type" : "decimal",
            "boost" : "1.0"
          },
          "field_product_commerce_price_amount_decimal_desc" : {
            "table" : "search_api_db_product_display",
            "column" : "field_product_commerce_price_amount_decimal_desc",
            "type" : "decimal",
            "boost" : "1.0"
          }
        },
        "default_node_index" : {
          "type" : {
            "table" : "search_api_db_default_node_index",
            "column" : "type",
            "type" : "string",
            "boost" : "1.0"
          },
          "title" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "5.0"
          },
          "promote" : {
            "table" : "search_api_db_default_node_index",
            "column" : "promote",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "sticky" : {
            "table" : "search_api_db_default_node_index",
            "column" : "sticky",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "created" : {
            "table" : "search_api_db_default_node_index",
            "column" : "created",
            "type" : "date",
            "boost" : "1.0"
          },
          "changed" : {
            "table" : "search_api_db_default_node_index",
            "column" : "changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "author" : {
            "table" : "search_api_db_default_node_index",
            "column" : "author",
            "type" : "integer",
            "boost" : "1.0"
          },
          "comment_count" : {
            "table" : "search_api_db_default_node_index",
            "column" : "comment_count",
            "type" : "integer",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_default_node_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "body:value" : {
            "table" : "search_api_db_default_node_index_text",
            "type" : "text",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_sort().
 */
function dexp_pressa_commerce_default_search_api_sort() {
  $items = array();
  $items['product_display__created'] = entity_import('search_api_sort', '{
    "index_id" : "product_display",
    "field" : "created",
    "name" : "Date created",
    "enabled" : "1",
    "weight" : "0",
    "identifier" : "product_display__created",
    "default_sort" : "1",
    "default_sort_no_terms" : "0",
    "default_order" : "desc",
    "options" : { "field_name" : "Date created" },
    "rdf_mapping" : []
  }');
  $items['product_display__field_product_commerce_price_amount_decimal_desc'] = entity_import('search_api_sort', '{
    "index_id" : "product_display",
    "field" : "field_product_commerce_price_amount_decimal_desc",
    "name" : "Product \\u00bb Price \\u00bb Amount (decimal) (Max)",
    "enabled" : "1",
    "weight" : "0",
    "identifier" : "product_display__field_product_commerce_price_amount_decimal_desc",
    "default_sort" : "0",
    "default_sort_no_terms" : "0",
    "default_order" : "desc",
    "options" : { "field_name" : "Products Variations \\u00bb Price \\u00bb Amount (decimal) (Max)" },
    "rdf_mapping" : []
  }');
  $items['product_display__title'] = entity_import('search_api_sort', '{
    "index_id" : "product_display",
    "field" : "title",
    "name" : "Title",
    "enabled" : "1",
    "weight" : "0",
    "identifier" : "product_display__title",
    "default_sort" : "0",
    "default_sort_no_terms" : "0",
    "default_order" : "desc",
    "options" : { "field_name" : "Title" },
    "rdf_mapping" : []
  }');
  return $items;
}
