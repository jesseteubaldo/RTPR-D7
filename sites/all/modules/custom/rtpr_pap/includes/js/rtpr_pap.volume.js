(function ($) {
  Drupal.behaviors.rtpr_pap = {
    attach: function (context, settings) {
      
      $('.volume-detail').click(function(){
        var uid = $(this).attr('uid');
        var tier = $(this).attr('tier');
        var month = $(this).attr('month');
        var year = $(this).attr('year');
        $('#myModal').modal('show');
        $(".progress-bar").animate({
            width: "100%"
        }, 1000);
        $('#myModal').load( "detail/" + tier + "?uid=" + uid + "&month=" + month + "&year=" + year);
      });
      
      $('#myModal').on('hide.bs.modal', function (e) {
        $('#myModal').html('<div class="modal-dialog" role="document"><div class="modal-content"><div class="modal-header"><button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button><h4 class="modal-title" id="myModalLabel">Loading</h4></div><div class="modal-body"><div class="progress"><div class="progress-bar progress-bar-striped active" role="progressbar" aria-valuenow="10" aria-valuemin="0" aria-valuemax="100" style="width: 10%"></div></div></div></div></div>');
      })
    }
  };
})(jQuery);
