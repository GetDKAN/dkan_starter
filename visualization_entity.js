/**
 * @file
 * Provides behaviour for visualization views.
 */

(function ($) {
  Drupal.behaviors.VisualizationEntity = {
    attach: function (context) {
      $(".visualization-embed .embed-code-wrapper").hide();
      $(".visualization-embed a.embed-link").live('click', function(){
        $(this).parents('.visualization-embed').find('.embed-code-wrapper').toggle();
        return false;
      });
    },
  };
})(jQuery);
