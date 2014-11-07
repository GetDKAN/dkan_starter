(function($) {
  Drupal.behaviors.visualization_choropleth = {
    attach: function(context) {
      var settings = Drupal.settings.visualization_choropleth;
      var resources = settings.resources;
      var geojson = settings.geojson;
      var data_column = settings.data_column ? [settings.data_column] : [];
      var map_column = settings.map_column;
      var geojson_key = settings.geojson_key;
      var geojson_label = settings.geojson_label;
      var color_scale = settings.colors || ['#FFEDA0', '#FEB24C', '#E31A1C', '#800026'];
      var breakpoints = settings.breakpoints ? settings.breakpoints : [];

      var view = {};
      if (resources.length === 1) {
        // Process resource and create recline model instance.s
        resource = resources[0].dataset;
        resource = resource.replace(/(\r\n|\n|\r)/gm,"\n");
        resource = new recline.Model.Dataset({
          records: recline.Backend.CSV.parseCSV(resource, resources[0].delimiter),
        });

        view = new recline.View.ChoroplethMap({
          polygons: geojson,
          model: resource,
          map_column: map_column,
          selectable_fields: data_column,
          breakpoints: breakpoints,
          geojson_key: geojson_key,
          geojson_label: geojson_label,
          base_color: color_scale,
          avg: resources[0].avg,
          unitOfMeasure: resources[0].unitOfMeasure,
        });
      }
      else {
        for (var k = 0; k < resources.length; k++){
          var dataset = {};
          resources[k].baseColor = color_scale;
          if (breakpoints.length > 0) {
            resources[k].breakpoints = breakpoints;
          }
          resources[k].selectable_fields = data_column;
          resources[k].fieldToDisplay = data_column.length > 0 ? data_column[0] : '';
          resources[k].dataset = new recline.Model.Dataset({
            records: recline.Backend.CSV.parseCSV(
              resources[k].dataset.replace(/(\r\n|\n|\r)/gm,"\n"),
              resources[k].delimiter
            ),
          });
        }
        view = new recline.View.MultiDatasetChoroplethMap({
          polygons: geojson,
          resources: resources,
          selectable_fields: data_column,
          label_to_map: map_column,
          base_color: color_scale,
        });
      }
      // Attach html and render the Recline view.
      var container = $('#visualization .data-view-container');
      var sidebar = $('#visualization .data-view-sidebar');
      container.append(view.el);
      sidebar.append(view.elSidebar);
      view.render();
    },
  };
})(jQuery);
