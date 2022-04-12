(function ($) {
    Drupal.behaviors.rtpr_event = {
        attach: function (context, settings) {
            $('#edit-field-event-date .start-date-wrapper .date-padding  .form-type-textfield:first-child input').change(function(){
                var date = $(this).val();
                var parent = $(this).parents();
                
                $(parent[4]).find('.end-date-wrapper .date-padding  .form-type-textfield:first-child input').val(date);
            });
            
            $('#store-location-node-form input:submit').click(function(event){
                $(this).css("display", "none");
            });
            
            $('.oh-clear-link').click(function(){
      $(this).closest( "td" ).find('.chosen-single').each(function() {
        $(this).addClass('chosen-default');
        $(this).find('span').text('Choose an option');
      });
    });
        }
    };
})(jQuery);