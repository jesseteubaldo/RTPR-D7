(function ($, Drupal, window, document, undefined) {
  'use strict';
  Drupal.behaviors.rtpr_cc = {
    attach: function(context) {
      $('.activation-form', context).once('hideSubmitButton', function () {
        var $form = $(this);
        $form.find('input.form-submit').click(function (e) {
          var el = $(this);
          el.after('<input type="hidden" name="' + el.attr('name') + '" value="' + el.attr('value') + '" />');
          return true;
        });
        $form.submit(function (e) {
          if (!e.isPropagationStopped()) {
            $('input.form-submit', $(this)).css('display', 'none');
            return true;
          }  
        });
      });
    }
  };
  

})(jQuery, Drupal, window, this.document);