(function($) {
  Drupal.behaviors.choropleth = {
    attach: function(context) {
      var choroDataColumn = [Drupal.settings.choropleth.choropleth_data_column] || [];
      var choropleth = Drupal.settings.choropleth.choropleth || '';
      var colorScale = Drupal.settings.choropleth.choropleth_color_scale || ['#FFEDA0', '#FEB24C', '#E31A1C', '#800026'];
      var breakpoints = Drupal.settings.choropleth.choropleth_breakpoints || [];
      var map_type = false;
      var map_label = '';
      var polygons;
      var location_default = [];
      location_default.lat = 0;
      location_default.lon = 0;
      location_default.zoom = 1;

      // Stop if choropleth isn't selected as a view to display.
      if (!choropleth.length) {
        return;
      }

      if (breakpoints.length) {
        var breakpoints = breakpoints.split(', ');
        var item = {};
        var items = [];
        for (var i = 0; i < breakpoints.length; i++) {
          items[i] = breakpoints[i];
        }
        breakpoints = items;
      }

      /**
       * Sets the maptype for a map
       * @param {string} type
       *  The type of the map attempting to be set.
       * @param {Boolean} force
       *  A flag to allow a map type to override an existing type.
       * @returns {string|Drupal.behaviors.choropleth.attach.map_type}
       *   The type of map established.
       */
      function set_map_type(type, force) {
        var force = force || false;
        var type = type || false;
        // Determine if type has already been set or if force changed.
        if (!map_type || force) {
          map_type = type;
        }
        return map_type;
      }

      /**
       * Sets the map label for a map
       * @param {string} label
       *  The label of the map attempting to be set.
       * @param {Boolean} force
       *  A flag to allow a map label to override an existing label.
       * @returns {string|Drupal.behaviors.choropleth.attach.map_label}
       *   The type of map established.
       */
      function set_map_label(label, force) {
        var force = force || false;
        var label = label || false;
        // Determine if label has already been set or if force changed.
        if (!map_label || force) {
          map_label = label;
        }
        return map_label;
      }
      // This event is trigged when the recline module creates
      // a new dataExplorer object.
      $(document).bind('createDataExplorer', function(event) {
         // Check to see if choropleth is enabled.
        if (typeof(Drupal.settings.choropleth.enable) != "undefined" && Drupal.settings.choropleth.enable) {
          var explorer = window.dataExplorer;
          var model = explorer.model;
          var fields = model.toTemplateJSON().fields;

          $.each(fields, function(i) {
            var field_id = fields[i].id.toLowerCase();
            // Process special case fields.  First one set, wins.
            switch (field_id) {
              case 'state':
              case 'states':
                // Process map as states.
                set_map_type('states');
                set_map_label('State');
                polygons = Drupal.settings.choropleth.statesData;
                // Set to contigous USA.
                location_default.lat = 37.8;
                location_default.lon = -96;
                location_default.zoom = 4;
                break;
              case 'country':
                // Process map as World.
                // Functionality not built yet.  Needs country polygons.
                set_map_type('world');
                set_map_label('Country');
                break;
              case 'county':
                // Process map as county.
                // Functionality not built yet.  Needs county polygons and default location fitbounds method.
                set_map_type('county');
                set_map_label('County');
                break;
              case 'city':
                // Process map as a state - functionality not built yet.
                // Functionality not built yet.  Needs city polygons and default location fitbounds method.
                set_map_type('state');
                set_map_label('City');
                break;
              case 'region':
                // Process map as region (loosely typed)- functionality not built yet.
                // Functionality not built yet.  Needs custom polygons method and default location fitbounds method.
                set_map_type('region');
                set_map_label('Region');
                break;
              default:
                // Nothing special so keep going.
            }
          });


          if (map_type) {
            // If recline dataset (dkan resource) presents a field that matches a case above,
            // we recreate the whole multiview and its current views
            // attaching also the Choropleth map View.

            var view = new recline.View.ChoroplethMap({
              polygons: polygons,
              model: model,
              map_type: map_type,
              label_to_map: map_label,
              location_default: location_default,
              selectable_fields: choroDataColumn,
              breakpoints: breakpoints,
              base_color: colorScale,
            });

            current_views = explorer.options.views;
            new_views = [];

            // Choropleth goes first ...
            new_views.push({
              id: 'choroplethmap',
              label: map_label + ' by ' + map_label,
              view: view
            });
            // ... then current views
            $.each(current_views, function (i, v) {
              var cls = null;
              switch(v.id) {
                case 'grid':
                  cls = recline.View.SlickGrid;
                  break;
                case 'graph':
                  cls = recline.View.Graph;
                  break;
                case 'map':
                  cls = recline.View.Map;
                  break;
              }
              if (cls) {
                new_views.push({
                  id: v.id,
                  label: v.label,
                  view: new cls({
                    model: model
                  })
                });
              }
            });

            // Grab state.
            var state = explorer.options.state;
            // Remove dom elements.
            explorer.remove();
            window.explorerDiv.html('');
            // Recreate the whole thing.
            var $el = $('<div />');
            $el.appendTo(window.explorerDiv);
            explorer = new recline.View.MultiView({
              model: model,
              el: $el,
              state: state,
              views: new_views
            });
          }
        }
      });
    }
  };
})(jQuery);
