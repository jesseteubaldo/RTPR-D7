<?php
/**
 * @file
 * dexp_pressa_commerce.entity.inc
 */

/**
 * Implements hook_entity_view_mode_info().
 */
function dexp_pressa_commerce_entity_view_mode_info() {
  $info = array();

  $info['node']['portfolio_classic'] = array(
    'label' => 'Portfolio Classic',
    'custom settings' => 1,
  );

  $info['node']['portfolio_grid'] = array(
    'label' => 'Portfolio Grid',
    'custom settings' => 1,
  );

  $info['node']['portfolio_masonry'] = array(
    'label' => 'Portfolio Masonry',
    'custom settings' => 1,
  );

  $info['node']['portfolio_text'] = array(
    'label' => 'Portfolio Text',
    'custom settings' => 1,
  );

  $info['node']['product_sidebar'] = array(
    'label' => 'Product Sidebar',
    'custom settings' => 1,
  );

  $info['node']['teaser_alt'] = array(
    'label' => 'Teaser Alt',
    'custom settings' => 1,
  );

  $info['node']['teaser_grid'] = array(
    'label' => 'Teaser Grid',
    'custom settings' => 1,
  );

  $info['node']['teaser_grid_alter'] = array(
    'label' => 'Teaser Grid Alter',
    'custom settings' => 1,
  );

  $info['node']['timeline'] = array(
    'label' => 'Timeline',
    'custom settings' => 1,
  );

  return $info;
}
