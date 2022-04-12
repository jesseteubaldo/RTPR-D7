jQuery.fn.rtpr_extras_remove_all = function() {
  jQuery("div").removeClass("not-selected");
}
jQuery.fn.rtpr_extras_remove_this = function() {
  var selector = this.selector;
  jQuery(selector + ' .product-image-wrapper').addClass("product-selected");
  jQuery(selector + ' .product-image-wrapper').removeClass("not-selected");
  jQuery(selector + ' .product-price.not-selected').addClass("product-selected");
  jQuery(selector + ' .product-price.not-selected').removeClass("not-selected");
  jQuery(selector + ' .product-item-links.not-selected').removeClass("not-selected");
}