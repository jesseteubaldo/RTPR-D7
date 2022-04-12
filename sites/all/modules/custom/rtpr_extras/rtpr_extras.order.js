// Using the closure to map jQuery to $. 
(function ($) {
  Drupal.behaviors.rtpr_extras = {
    attach: function (context, settings) {
      $('.field.order-status .form-submit').on('click', function(e){
         //$('.item-150167.updated').addClass('updated');
         var orderID = $(this).closest('.field.order-status').attr('order');
         $('.item-' + orderID +'.views-row-active').addClass('updated');

      });
    }
  };
})(jQuery);