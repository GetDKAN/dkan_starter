/**
 * @file
 */
console.log('bla');
(function ($) {
  Drupal.behaviors.reactDashboardEmbed = {
    attach: function (context) {
      $('.react_dashboard-embed #embed-height').on('keyup', renderIframeCode);
      $('.react_dashboard-embed #embed-width').on('keyup', renderIframeCode);

      $('.react_dashboard-embed .embed-code-wrapper').hide();
      $('.react_dashboard-embed').on('click', 'a.embed-link', function(){
        $(this).parents('.react_dashboard-embed').find('.embed-code-wrapper').toggle();
        return false;
      });
      function renderIframeCode(e){
        var prop = (e.currentTarget.id === 'embed-height') ? 'height' : 'width';
        var value = ($(this).val()) ? $(this).val() : (prop === 'height') ? '600' : '960';
        var iframe = $('.react_dashboard-embed #embed-code').text();
        var newCode = $(iframe).prop(prop, value).get(0).outerHTML;
        $('.react_dashboard-embed #embed-code').text(newCode);
      }
    },
  };
})(jQuery);
