jQuery(document).ready(function($) {
    "use strict";
    $('.dexp-container').find('.block-big-title').wrap('<div class="container">');
    $('.search-toggle').click(function() {
        $(this).parent().find('.search-form-block-wrapper').toggleClass('open').find('input[type=text]').focus();
    });
    $(document).keyup(function(e) {
        if (e.keyCode === 27) {
            $('.search-form-block-wrapper').removeClass('open');
        }
    });
    // Auto clear default value field
    //$('.form-text,.form-textarea').cleardefault();
    // Tooltips
    $('.bs-example-tooltips').tooltip({
        selector: "[data-toggle=tooltip]",
        container: "body"
    });
    $('.dtooltip').tooltip({
        container: 'body'
    });
    $("#bs-example a").popover();

    //Go to top
    $(window).scroll(function() {
        if ($(this).scrollTop() > 100) {
            $('#go-to-top').css({
                bottom: "25px"
            });
        } else {
            $('#go-to-top').css({
                bottom: "-100px"
            });
        }
    });
    $('#go-to-top').click(function() {
        $('html, body').animate({
            scrollTop: '0px'
        }, 800);
        return false;
    });
    var animate = "flipUp";
    if (detectIE() > 1) {
        animate = "fade";
        $('body').addClass('ie');
    }
    $(".rotate").textrotator({
        animation: animate,
        speed: 2000
    });
    $("a[rel^='prettyPhoto']").prettyPhoto();

    var url = window.location.pathname;
    var paths = url.split("/");
    if(paths[1] == 'support' && paths[2]){
      var activeTab = "/" + paths[1] + "/" + paths[2];
      console.log(activeTab);
      $('.support.nav a[href="'+ activeTab +'"]').parent().addClass('active');
    }

    $('#edit-commerce-fieldgroup-pane-group-order-feedback-field-agreement-und').change(function() {

      if(this.checked) {
          $('#myModal').modal('show')
      }
    });
    var numItems = $('.page-taxonomy-term .node-blog-post').length;

    if(numItems && numItems < 10) {
      $('.page-taxonomy-term .pagination').css('display', 'none');
    }

    $('.switcher .option a').click(function(e){
      if($(this).hasClass('selected')){
        $(this).addClass('hide');
      } else {
        $('.switcher .option a').removeClass('hide');
        $('.switcher .option a').removeClass('selected');
        $(this).addClass('selected');
        $(this).addClass('hide');
      }
    });

    $( ".switcher .option a" ).each(function( index ) {
      if($(this).hasClass('selected')){
        $(this).addClass('hide');
      }
    });

    jQuery.fn.outerHTML = function() {
       return (this[0]) ? this[0].outerHTML : '';
    };

    $( ".services-box-animated" ).hover(

      function() {
        var height = $(this).find('.inner').find('.front').height() + 10;
        $( this ).removeClass( "services-box" );
        $( this ).addClass( "back-seleted" );

        if (height < 250) {
          var link = $(this).find('.inner').find('.back p:last-child a').outerHTML();

          $(this).find('.inner').find('.back p').addClass('hide');//remove();
          $(this).find('.inner').find('.back').append(link);
          $(this).find('.inner .back a').addClass('tablet');
        } else {
          $(this).find('.inner').find('.back p').removeClass('hide');
          $(this).find('.inner').find('.back p .tablet').addClass('hide');
        }
        $( this ).find('.back').height(height);
      }, function() {
        $( this ).addClass( "services-box" );
        $( this ).removeClass( "back-seleted" );

      }
    );

    $('.dexp-dropdown  li.expanded .nolink').click(function() {
      $(this).parent().find('.menu').toggleClass('menu-open');
      $(this).parent().find('.menu-toggler').toggleClass('fa-angle-down');

    });

    // Disable coupon.
    if ( $(".view-order-coupon-list.view-id-order_coupon_list .view-content").length ) {
      $('#commerce-checkout-coupon-ajax-wrapper .form-submit').prop('disabled', true);
    }

    if ( $('.page-activate .form-item-mail').length) {
        $('.activate .form-submit').prop('disabled', true);
        $('.page-activate .form-item-code .form-text').prop('readonly', true);

    }
    $('.page-activate .activate').hover(function() {
      if ( $('.page-activate .form-item-mail').length) {
        $('.activate .form-submit').prop('disabled', true);
        $('.page-activate .form-item-code .form-text').prop('readonly', true);
      }
    });

    // Show Rewards TOS.
    $('.page-user-edit #myModal').modal('show');
    $('.page-user-edit #myModal .form-select').change(function() {
      var business = $(this).val();
      if (business == 107 || business == 108) {
        $('.page-user-edit #myModal .other').removeClass('hide');
      }
      else {
        $('.page-user-edit #myModal .other').addClass('hide');
      }
    });
    $('.page-user-edit #myModal').on('hide.bs.modal', function (e) {

      var business = $(this).find('.form-select').val();
      var other = '0';
      if (business == '_none') {
        alert('Please describe your business!');
        return false;
      }
      else {
        if (business == 107 || business == 108) {
          other = $('.page-user-edit #myModal #edit-business-other').val();
          if (!other) {
            alert('Please describe your business!');
            return false;
          }
        }
      }

      var url = '/rewards-terms/' + $(this).data('uid');
      //
      var posting = $.post( url, { tid: business, other:other } )
        .done(function( data ) {
          self.location = "https://rtpr.com/backoffice/wholesale-rewards-cp";
        });


    });
    // Display modal for retail reward.
    $('#retail-reward-modal').modal('show');

    // Commerce buck modal.
    $('#buckModal').modal('show');

    // Commerce GOPAK modal.
    $('#gopakModal').modal('show');
    
    // Commerce Introductory modal.
    $('#introductoryModal').modal('show');

    // Commerce buck modal.
    $('#freeProductModal').modal('show');

    // Affiliate buck modal.
    $('#affiliatePromotionModal').modal('show');

    // Reward buck modal.
    $('#rewardsPromotionModal').modal('show');
    
    // Good/Better/Best modal.
    $('#primaryOfferModalLabel').modal('show');
    
    // Agreement modal.
    $('#userAgreementModal').modal('show');
    
    $('#userAgreementModal .modal-footer button').on('click', function(event) {
      var $button = $(event.target); // The clicked button
    
      $(this).closest('.modal').one('hidden.bs.modal', function() {
        if ($button.text() == 'I agree to the terms') {
          var rid = $button.attr('data-rid');
          $.post('/rtpr-user-agreement', { rid: rid} )
          .done(function( data ) {
            if (rid == 4 ) {
              self.location = "https://rtpr.com/backoffice/pending-wholesale-panel";
            }
            else if (rid == 38) {
              self.location = "https://rtpr.com/backoffice/retail-center-program";
            }
            else if (rid == 47) {
              self.location = "https://rtpr.com/backoffice/wholesale-rewards-cp";
            }
          });
        }
      });
    });


    // GetReponse modal.
    setTimeout(function() {
        $('#getresponseModal').modal();
    }, 6000);

    $('#wholesaleReferralsModal').on('show.bs.modal', function(e) {
      //get data-id attribute of the clicked element
      var uid = $(e.relatedTarget).data('uid');
      var pid = $(e.relatedTarget).data('pid');
      var name = $(e.relatedTarget).data('name');
      $(e.currentTarget).find('.modal-title').text('Wholesale Referrals of ' + name);
      
      $.get('/user/'  + pid + '/wholesale-referrals/' + uid + '/data')
        .done(function( data ) {
          $(e.currentTarget).find('.modal-body').html(data.uid);
          console.log( "Data Loaded: " + data.uid );
        });
    });
    
    $('#wholesaleReferralsModal').on('hide.bs.modal', function(e) {
      $(e.currentTarget).find('.modal-body').text('Loading...');
    });
    
    $('#promoterReferralsModal').on('show.bs.modal', function(e) {
      //get data-id attribute of the clicked element
      var uid = $(e.relatedTarget).data('uid');
      var pid = $(e.relatedTarget).data('pid');
      var name = $(e.relatedTarget).data('name');
      $(e.currentTarget).find('.modal-title').text('Promoter Referrals of ' + name);
      
      $.get('/user/'  + pid + '/promoter-referrals/' + uid + '/data')
        .done(function( data ) {
          $(e.currentTarget).find('.modal-body').html(data.uid);
          console.log( "Data Loaded: " + data.uid );
        });
    });
    
    $('#promoterReferralsModal').on('hide.bs.modal', function(e) {
      $(e.currentTarget).find('.modal-body').text('Loading...');
    });
    
    $('#rewardsModal').on('show.bs.modal', function(e) {
      var uid = $(e.relatedTarget).data('uid');
      var type = $(e.relatedTarget).data('type');
      console.log(type);
      $(e.currentTarget).find('.modal-title').text(type + ' List');
      $.get('/user/'  + uid + '/rewards?type=' + type)
        .done(function( data ) {
          $(e.currentTarget).find('.modal-body').html(data.uid);
        });
    });
    
    $('#rewardsModal').on('hide.bs.modal', function(e) {
      $(e.currentTarget).find('.modal-body').text('Loading...');
    });
    
    // Add loading background.
    $('.rtpr-loading').click(function() {
      $('<div class="rtpr-overlay"><img src="/sites/all/themes/pressa/assets/images/loading.gif"></div>').insertBefore(this);
      
        console.log('adding class to body');
    });
    
    // Play YouTube or Vimeo Videos.
    

    $('.video-btn').click(function() {
        var videoSrc = $(this).data( "src" );
        var theModal = $(this).data("target");
        var videoWidth = $(this).data( "width" );
        var videoHeight = $(this).data("height");
        var videoAllow = $(this).data( "allow" );
        if (videoSrc) {
          $('#videoModal iframe').attr('src', videoSrc);
          $('#videoModal iframe').attr('width', videoWidth);
          $('#videoModal iframe').attr('height', videoHeight);
          $('#videoModal iframe').attr('allow', videoAllow);
        }
    });
    
    // when the modal is opened autoplay it  
    $('#videoModal').on('shown.bs.modal', function (e) { });
    // stop playing the youtube video when I close the modal
    $('#videoModal').on('hide.bs.modal', function (e) {
        // a poor man's stop video
        $("#videoModal").hide();
    });
    
    $('body').click(function (event) {
      if(!$(event.target).closest('#videoModal').length && !$(event.target).is('#videoModal')) {
        $("#videoModal").hide();
      }     
    });
    
    // /offer and /share modal.
    var offerPage = document.getElementById("offerModal");
    if (offerPage) {

      window.addEventListener('beforeunload', function (e) {
          e.preventDefault();
          e.returnValue = '';
          $('#offerModal').modal('show');
      });
    }
    
    var sharePage = document.getElementById("shareModal");
    if (sharePage) {
      var showShareModal = document.cookie.match(/^(.*;)?\s*showShareModal\s*=\s*[^;]+(.*)?$/);
      
      if (!showShareModal) {
        setTimeout(function() {
          $('#shareModal').modal('show');
        }, 5000);
        
        createCookie('showShareModal', true, 1);
      }
    }
/*
    $('input[type=radio][name="commerce_payment[payment_method]"]').change(function() {
      var element = $('#payment-details').detach();
      $(this).parent().append(element);  
    });

    var element = $('#payment-details').detach();
    $('edit-commerce-payment-payment-method-paypal-wppcommerce-payment-paypal-wpp').parent().append(element)
*/
});

function createCookie(name,value,days) {
  if (days) {
    var date = new Date();
    date.setTime(date.getTime()+(days*24*60*60*1000));
    var expires = "; expires="+date.toGMTString();
  }
  else var expires = "";
  document.cookie = name+"="+value+expires+"; path=/";
}
/*

jQuery.fn.cleardefault = function() {
    return this.focus(function() {
        if (this.value == this.defaultValue) {
            this.value = "";
        }
    }).blur(function() {
        if (!this.value.length) {
            this.value = this.defaultValue;
        }
    });
};
*/

function detectIE() {
    var ua = window.navigator.userAgent;
    var msie = ua.indexOf('MSIE ');
    var trident = ua.indexOf('Trident/');

    if (msie > 0) {
        // IE 10 or older => return version number
        return parseInt(ua.substring(msie + 5, ua.indexOf('.', msie)), 10);
    }

    if (trident > 0) {
        // IE 11 (or newer) => return version number
        var rv = ua.indexOf('rv:');
        return parseInt(ua.substring(rv + 3, ua.indexOf('.', rv)), 10);
    }

    // other browser
    return false;
}

function enrollRP(element) {
  if (element.checked) {
    document.cookie = "Drupal.visitor.rewards_program=1;path=/";
  }
  else {
    document.cookie = "Drupal.visitor.rewards_program=0;path=/";
  }
}

