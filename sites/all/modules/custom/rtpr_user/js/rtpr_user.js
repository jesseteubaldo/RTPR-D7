(function ($) {
  Drupal.behaviors.rtpr_user = {
    attach: function (context, settings) {
      $('#user-register-form input:submit').click(function(event){
        $(this).css("display", "none");
      });
    }
  };
})(jQuery);