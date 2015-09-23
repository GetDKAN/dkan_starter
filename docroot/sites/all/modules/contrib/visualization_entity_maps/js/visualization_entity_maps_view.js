(function($) {
  Drupal.behaviors.VisualizationEntityMapsView = {
    attach: function (context) {
      var isIframe = !$('.content').is(':visible');
      var state = $('.field-name-field-ve-map-settings .field-item:eq(0)').text();
      var $el = $('#map');
      var title;
      var height;
      var $body;
      var mapDefaults;
      var model;
      var dataset;
      var mapConfig;

      $el.append('<div class="alert alert-info loader">Loading <span class="spin"></span></div>');

      // Needed when Leaflet is uglified
      L.Icon.Default.imagePath = Drupal.settings.visualizationEntityMaps.leafletPath + '/dist/images';

      if (state) {
        state = new recline.Model.ObjectState(JSON.parse(state));
        $body = $(document.body);
        $window = $(window);
        $iframe_shell = $('#iframe-shell');
        $body.removeClass('admin-menu');

        mapDefaults = { showTitle: true };
        mapState = _.extend({}, mapDefaults, state.get('mapState'));

        if ($iframe_shell.length) {
          if(mapState.showTitle){
            title = $iframe_shell.find('h2 a').html();
            $body.prepend('<h2 class="veTitle">' + title + '</h2>');
            height = getVisualizationHeight(true);
            resize();
          } else {
            height = getVisualizationHeight(false);
          }
          state.set('height', height);
          state.set('width', $window.width() - 10);
          $window.on('resize', resize);
        } else {
          state.set('width', $('.field-name-field-ve-settings').width());
        }

        model = state.get('source');
        model.url = cleanURL(model.url);
        dataset = new recline.Model.Dataset(model);

        mapConfig = {
          model: dataset,
          el: $el,
          state: mapState,
        };

        // Remove limitation of 100 rows. There is no 'unlimited' setting.
        dataset.queryState.attributes.size = 10000000;
        dataset.fetch().done(function(d) {
          var state = mapConfig.state;
          var tooltipFields = state.tooltipField || [];
          var pickedFields = tooltipFields.concat([state.geomField, state.latField, state.lonField]);
          var geoPointRegex = /\(-?[\d.]+?, -?[\d.]+?\)/;
          var records;

          // If we use the geomfield
          if (state.geomField) {
            d.fields.each(function(field) {
              if (field.id === state.geomField) {
                field.type = 'geo_point';
              }
            });
          }

          records = d.records.reduce(function(memo, record){

            // Grab location. it's returns either a boolean (latlon)
            // or an array (geofield) depends on which location field are we
            // using to geolocate.
            var location = hasLocation(record, state);

            if(location) {

              // if we have a valid geomfield then we use it.
              _.isArray(location) && record.set(state.geomField, location[0]);

              // Hide fields we don't want to show.
              record = _.isEmpty(tooltipFields)
                ? replaceNull(record.toJSON())
                : replaceNull(_.pick(record.toJSON(), pickedFields));
              memo.push(record);
            }
            return memo;
          }, []);

          // Initialize a model after cleanup
          mapConfig.model = new recline.Model.Dataset({
            records: records
          });

          // Because we change the model we need to override
          // and fetch the records again.
          mapConfig.model.queryState.attributes.size = 10000000;
          mapConfig.model.fetch();

          var map = new recline.View.Map(mapConfig);
          map.render();

          // Delay resize until next javascript tick.
          setTimeout(resize, 0);

          // Remove spiner
          $el.find('.loader').remove();
        });
      }

      /**
       * Check if a record can be geolocated
       * @param  {Object}  r record
       * @param  {Object]} s state
       * @return {Boolean}
       */
      function hasLocation(r, s) {
        return r.get(s.latField) && r.get(s.lonField) || isGeoPoint(r.get(s.geomField));
      }

      /**
       * Check if a string contains a valid geopoint
       * @param  {string} geopoint
       * @return {Boolean}
       */
      function isGeoPoint(geopoint) {
        var geoPointRegex = /\(-?[\d.]+?, -?[\d.]+?\)/;
        return geopoint && geopoint.match(geoPointRegex);
      }

      /**
       * Replace null values with a dash
       * @param  {Object} record
       * @return {Object}
       */
      function replaceNull(record){
        return _.mapValues(record, function(value){
          if(value === null)
            return '-' ;
          else
            return value;
        });
      }

      function resize(){
        var $title = $body.find('h2.veTitle');
        var hasTitle = !!$title.length;
        var height = getVisualizationHeight(hasTitle);
        $('.recline-nvd3').height(height);
        $('.recline-map .map').height(height);
      }

      function getVisualizationHeight(hasTitle) {
        var height = (!hasTitle)
          ? $(window).height()
          : $(window).height() - $body.find('h2.veTitle').outerHeight(true);

        return height;
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
