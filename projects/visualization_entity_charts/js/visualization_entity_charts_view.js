/**
 * @file
 * Provides options for chart visualization.
 */

(function ($) {
  Drupal.behaviors.VisualizationEntityChartsView = {
    attach: function (context) {
      var isIframe = !$('.content').is(':visible');
      var state = $('.field-name-field-ve-settings .field-item:eq(0)').text();
      var $el;
      var title;
      var $body;

      if(state){
        state = new recline.Model.ObjectState(JSON.parse(state));
        $body = $(document.body);
        $body.removeClass('admin-menu');

        if($('#iframe-shell').length){
          $el = $('#iframe-shell');
          if(state.get('showTitle')){
            title = $el.find('h2 a').html();
            $body.prepend('<h2>' + title + '</h2>');
          }
          state.set('height', $(window).height());
          state.set('width', $(window).width() - 10);
        } else {
          $el = $('#graph');
          state.set('width', $('.field-name-field-ve-settings').width());
        }

        var model = state.get('source');
        model.url = cleanURL(model.url);
        var dataset = new recline.Model.Dataset(model);
        var graph = null;

        dataset.fetch().done(function(dataset){
          graph = new recline.View.nvd3[state.get('graphType')]({
            model: dataset,
            state: state,
            el: $el
          });
          graph.render();

        });
      }
      function cleanURL(url){
        var haveProtocol = new RegExp('^(?:[a-z]+:)?//', 'i');
        if(haveProtocol.test(url)){
          url = url.replace(haveProtocol, '//');
        }
        return url;
      }
    }
  };
})(jQuery);
