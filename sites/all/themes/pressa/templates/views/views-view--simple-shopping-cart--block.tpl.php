<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>
<?php
global $user;
$quantity = 0;
$total = '$0.00';
$order = commerce_cart_order_load($user->uid);
if ($order) {
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  try {
    $line_items = $wrapper->commerce_line_items;
    $quantity = commerce_line_items_quantity($line_items, commerce_product_line_item_types());
    $order_total = commerce_line_items_total($line_items);
  }
  catch (EntityMetadataWrapperException $exc) {
    watchdog(
      'pressa',
      'EntityMetadataWrapper exception in %function() <pre>@trace</pre>',
      array('%function' => __FUNCTION__, '@trace' => $exc->getTraceAsString()),
      WATCHDOG_ERROR
    );
  }
}
?>

<p>
  <a href="<?php print url('cart'); ?>" alt="View cart"  class="dtooltip" data-toggle="tooltip" data-placement="bottom" data-original-title="<?php print $quantity; ?> item in your cart">
	(<?php print $quantity; ?>) Items
   <span>
      <i class="fa fa-shopping-cart"></i>
   </span>
  </a>
</p>