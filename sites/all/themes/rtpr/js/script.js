        jQuery(document).ready(function() {

            jQuery(".login a").click(function(e) {          
        e.preventDefault(); 
                jQuery("#block-user-login").toggle();
        jQuery(".login a").toggleClass("menu-open");
            });
      
      jQuery("div#block-user-login").mouseup(function() {
        return false
      });
      jQuery(document).mouseup(function(e) {
        if(jQuery(e.target).parent(".login").length==0) {
          jQuery(".login a").removeClass("menu-open");
          jQuery("div#block-user-login").hide();
        }
      }); 
          }); 





(function ($) {
$('#mobnav-btn').click(

function () {
    $('.sf-menu').toggleClass("xactive");


});


$('.mobnav-subarrow').click(

function () {
    $(this).parent().toggleClass("xpopdrop");


});
 
})(jQuery);