(function($) {
  Drupal.behaviors.VisualizationEntityMapsView = {
    attach: function (context) {
      var isIframe = !$('.content').is(':visible');
      var state = $('.field-name-field-ve-map-settings .field-item:eq(0)').text();
      var el = $('#map');

      // Needed when Leaflet is uglified
      L.Icon.Default.imagePath = Drupal.settings.visualizationEntityMaps.leafletPath + '/dist/images';

      if (state) {
        state = new recline.Model.ObjectState(JSON.parse(state));
        var model = state.get('source');
        model.url = cleanURL(model.url);
        var dataset = new recline.Model.Dataset(model);

        mapState = state.get('mapState');

        var mapConfig = {
          model: dataset,
          el: el,
          state: mapState,
        };

        dataset.fetch()
          .done(function(d) {

            if (mapConfig.state.geomField) {
              d.fields.each(function(field) {
                if (field.id === mapConfig.state.geomField) {
                  field.type = 'geo_point';
                }
              });

              d.records.each(function(r) {
                match = r.get(mapConfig.state.geomField).match(/\(-?[\d.]+?, -?[\d.]+?\)/);
                if (match) {
                  r.set(mapConfig.state.geomField, match[0]);
                } else {
                  r.set(mapConfig.state.geomField, '');
                }
              });
            }

            var map = new recline.View.Map(mapConfig);
            map.render();

          });
      }

      function cleanURL(url) {
        var haveProtocol = new RegExp('^(?:[a-z]+:)?//', 'i');
        if(haveProtocol.test(url)){
          url = url.replace(haveProtocol, '//');
        }
        return url;
      }
    }
  };
})(jQuery);
