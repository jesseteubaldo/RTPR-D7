(function ($) {
  Drupal.behaviors.rtpr_commerce = {
    attach: function (context, settings) {
      $('#edit-status').change(function(e) {
          $('#edit-field-order-warehouse-und').trigger("chosen:updated");
      });

      $('.form-disabled .chosen-enable.form-select').trigger("chosen:updated");
      $('#views-form-commerce-backoffice-orders-admin-page .order-status').change(function(e) {
        if ($(this).val()) {
          if ($(this).val() == 'processing') {
            $(this).parents().parents().find('.order-warehouse').removeAttr('disabled');
          }
          else {
            $(this).parents().parents().find('.order-warehouse').attr("disabled","disabled");
          }
        }
        $('.form-item-warehouse .chosen-enable.form-select').trigger("chosen:updated");
      });

    }
  };
})(jQuery);