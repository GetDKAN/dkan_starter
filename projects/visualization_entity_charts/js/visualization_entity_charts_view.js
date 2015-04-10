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

      if(state){
        state = new recline.Model.ObjectState(JSON.parse(state));

        $(document.body).removeClass('admin-menu');
        if(isIframe){
          $el = $('#iframe-shell');
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
