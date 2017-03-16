;(function($){
  Drupal.behaviors.wddh_front_page = {
    attach: function (context, settings) {
     $('#page_submenu a[href*="#"]:not([href="#"]), .back-to-top').click(function() {
        if (location.pathname.replace(/^\//,'') == this.pathname.replace(/^\//,'') && location.hostname == this.hostname) {
          var target = $(this.hash);
          target = target.length ? target : $('[name=' + this.hash.slice(1) +']');
          if (target.length) {
            $('html, body').animate({
              scrollTop: target.offset().top
            }, 1000);
            return false;
          }
        }
      });
      $('.apps-carousel').slick({
        dots: true,
        infinite: false,
        arrows:false,
        slidesToShow: 1,
        slidesToScroll: 1
      });

      $('.collections-carousel').slick({
        dots: true,
        infinite: false,
        arrows:false,
        slidesToShow: 3,
        slidesToScroll: 3, 
        responsive: [
          {
            breakpoint: 992,
            settings: {
              slidesToShow: 2,
              slidesToScroll: 2
            }
          },
          {
            breakpoint: 540,
            settings: {
              slidesToShow: 1,
              slidesToScroll: 1
            }
          }
        ]
      });
    }
  };
})(jQuery);


  