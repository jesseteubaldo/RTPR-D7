<?php


/**
 * Implements hook_token_info().
 */
function rtpr_extras_token_info() {
  $types['pap'] = array(
    'name' => t("PAP Tokens"),
    'description' => t("Tokens for PAP."),
  );

  $pap['pap_session'] = array(
    'name' => t("PAP Session"),
    'description' => t("The PAP session id for current user."),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'pap' => $pap,
    ),
  );
}

/**
 * hook_token_info_alter().
 */
function rtpr_extras_token_info_alter(&$data) {
  $data['tokens']['commerce-line-item']['line-item-title'] = array(
    'name' => t('Line item title'),
    'description' => t('The title of the product.'),
  );
  
}

/**
 * Implements hook_tokens().
 */
function rtpr_extras_tokens($type, $tokens, array $data = array(), array $options = array()) {
  global $user;

  $replacements = array();

  if ($type == 'pap') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'pap_session':
          $replacements[$original] = isset($_SESSION['pap_sid']) ? $_SESSION['pap_sid'] : '1';
          break;
      }
    }
  }
  
  if ($type == 'commerce-line-item' && !empty($data['commerce-line-item'])) {
  $line_item = $data['commerce-line-item'];

  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'line-item-title':
        $product = commerce_product_load_by_sku($line_item->line_item_label);
        $replacements[$original] = $sanitize ? check_plain($product->title) : $product->title;
        break;
      }
    }
  }

  return $replacements;
}
