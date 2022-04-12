(function ($) {
  Drupal.behaviors.rtpr_pap = {
    attach: function (context, settings) {
      $('.referral-table').on('click', '.pap-ajax-load', function (event) {
        var pap_id = $(this).attr('pid');
        var tier = $(this).attr('tier');
        var type = $(this).attr('type');
        tier = parseInt(tier) + 1;
        var current_btn = $(this);
       // $('#wrap-'+pap_id).html('<img width="50" height="50" src="http://www.arabianbusiness.com/skins/ab.main/gfx/loading_spinner.gif">');
        $('#wrap-'+pap_id).load('/pap_ajax/' + pap_id + '/' + tier + '/' + type, function(){ 
           current_btn.removeClass('not-selected');
           current_btn.addClass('selected');
           current_btn.removeClass('pap-ajax-load');
           current_btn.addClass('close-pap-ajax-load');
           current_btn.text("-");
        });
        
        event.preventDefault();
      });
      
      $('.referral-table').on('click', '.close-pap-ajax-load', function (event) {
        console.log("close");
        var pap_id = $(this).attr('pid');
        var tier = $(this).attr('tier');
        tier = parseInt(tier) + 1;
        var current_btn = $(this);
        
        $('#wrap-'+pap_id).html("");
         current_btn.addClass('not-selected');
         current_btn.removeClass('selected');
         current_btn.addClass('pap-ajax-load');
         current_btn.removeClass('close-pap-ajax-load');
         current_btn.text("+");
        
        
        event.preventDefault();
      });
      
      $('.referral-table').on('change', '.form-select', function(){
          var val = $(this).val();
         
          if(val == 'all') {
            $('.referral-info.table').css('display', 'block');
          } else {
            
            $('.referral-info').css('display', 'none');
            $('.referral-info.'+val).css('display', 'block');
            
          }
      });

      var $progressBar = $('.progress-bar');
      var $progress = $('.progress');
      $progressBar.animate({width: '100%'}, 15000);
      // Volume Ajax
      console.log('test');
      console.log('/volume/ajax/' + settings.rtpr_pap.uid  + '/' + settings.rtpr_pap.month + '/' + settings.rtpr_pap.year + '?time=' + $.now());
      $("#volume-stat").load('/volume/ajax/' + settings.rtpr_pap.uid  + '/' + settings.rtpr_pap.month + '/' + settings.rtpr_pap.year + '?time=' + $.now());
    }
  };
})(jQuery);