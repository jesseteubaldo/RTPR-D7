(function ($) {
  Drupal.behaviors.rtpr_offer = {
    attach: function(context, settings) {
      jQuery(document).ready(function($) {
        //$("#edit-submit").delay(70000).val('test');
        setTimeout(function(){
          $("#edit-submit").mousedown();
        }, 2000);
      });
    }
  }
}(jQuery));

jQuery(document).ready(function($) {

  $('#rtpr-offer-gc-referrals-form .view-code').click(function() {
    $('#offerGcModal').on('show.bs.modal', function(e) {
    
        //get data-id attribute of the clicked element
        var status = $(e.relatedTarget).data('status');
        var name = $(e.relatedTarget).data('name');
        var email = $(e.relatedTarget).data('email');
        var token = $(e.relatedTarget).data('token');
        if (status == 'sentgiftcard') {
          var link = 'https://rtpr.com/user/register?gctoken=' + token;
          var body = '<p>If your Customer says they did not get the activation email and they searched their email box (including junk) for an email from promotions@rtpr.com then you can copy the link below to send them their activation link directly. Copy the full link below<p>Activation link for '+ name + ' email address ' + email + ':</p><p><a href="'+ link +'"> ' + link + '</a></p></p>Note: this activation link will only work when creating an account with the email address of ' + email + '.</p>';
        }
        else if (status == 'activatedgiftcard') {
          var body = '<p>' + name +' has already activated with email address ' + email + ' and their Gift Card is already applied to their account</p><p>Ask the customer to login to their RTPR.com account with ' + email + ' as the username and the Password they set when they activated. If they do not remember their password, they can click the lost password request on the login page.</p>';
        }
        
        if (body) {
          console.log(body);
          $(e.currentTarget).find('.modal-body').html(body);
        }
    });
  });
});