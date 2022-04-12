
$('.slider-for').slick({
    slidesToShow: 3,
    slidesToScroll: 1,
    arrows: false,
    infinite: true,
    asNavFor: '.testimonials-slides',
    responsive: [
        {
            breakpoint: 1024,
            settings: {
                slidesToShow: 2,
                slidesToScroll: 1,
                infinite: true,
                dots: true
            }
        },
        {
            breakpoint: 600,
            settings: {
                slidesToShow: 1,
                slidesToScroll: 1
            }
        },
        {
            breakpoint: 480,
            settings: {
                slidesToShow: 1,
                slidesToScroll: 1
            }
        }
        // You can unslick at a given breakpoint now by adding:
        // settings: "unslick"
        // instead of a settings object
    ]
});
$('.testimonials-slides').slick({
    dots: true,
    infinite: true,
    speed: 300,
    slidesToShow: 1,
    adaptiveHeight: false,
    asNavFor: '.slider-for'
});


$(".testimonial_image_slider .slick-slide").each(function(){
    $(this).find('img').click(function(){
        $('#videoPop').modal('show')
    })
    
})





var iframe = document.getElementById('video');

// $f == Froogaloop
var player = $f(iframe);

// bind events
var playButton = document.getElementById("play-button");
playButton.addEventListener("click", function() {
  player.api("play");
});

var pauseButton = document.getElementById("pause-button");
pauseButton.addEventListener("click", function() {
  player.api("pause");
});
