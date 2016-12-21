(function ($) {

    Drupal.leaflet_widget = Drupal.leaflet_widget || {};

    Drupal.behaviors.geofield_widget = {
        attach: attach
    };

    function attach(context, settings) {
        $('.leaflet-widget').once().each(function(i, item) {
            var id = $(item).attr('id'),
            options = settings.leaflet_widget_widget[id];
            if (options.toggle) {
              $('#' + id + '-input').before('<div class="map btn btn-default" style="cursor: pointer;" id="' + id + '-geojson-toggle">GEOJSON</div>');
              $('#' + id + '-input').before('<div class="map btn btn-default" style="cursor: pointer;" id="' + id + '-point-toggle">POINT</div></br><input type="text" id="manual-' + id + '-point-input" name="manual-point">');
                $('#manual-' + id + '-point-input').hide();
              $('#' + id + '-geojson-toggle').click(function () {
                $(item).toggle();
                if ($(this).hasClass('map')) {
                  $(this).text('Use map');
                  $(this).removeClass('map');
                  $('#' + id + '-input').get(0).type = 'text';

                  //Hide select geographic areas if is enable.
                  if (options.geographic_areas) {
                    $('.geographic_areas_desc').hide();
                  }
                }
                else {
                  $(this).text('GEOJSON');
                  $('#' + id + '-input').get(0).type = 'hidden';
                  $(this).addClass('map');

                  //Show select geographic areas if is enable.
                  if (options.geographic_areas) {
                    $('.geographic_areas_desc').show();
                  }
                }
              });

               $('#' + id + '-point-toggle').click(function () {
                $(item).toggle();
                $('#manual-' + id + '-point-input').toggle();

                if ($(this).hasClass('map')) {
                  $(this).text('Use map');
                  $(this).removeClass('map');
                  $('#manual-' + id + '-point-input').get(0).type = 'text';

                  //Hide select geographic areas if is enable.
                  if (options.geographic_areas) {
                    $('.geographic_areas_desc').hide();
                  }
                }
                else {
                  $(this).text('POINT');
                  $('#' + id + '-input').get(0).type = 'hidden';
                  $(this).addClass('map');
                  var point_string = $('#manual-' + id + '-point-input').val();

                  if (point_string) {
                      var point_array = point_string.split(',');
                      var geojsonFeature = {
                        "type": "Feature",
                        "properties": {},
                        "geometry": {
                            "type": "Point",
                            "coordinates": [point_array[0], point_array[1]]
                        }
                    };
                    L.geoJson(geojsonFeature).addTo(map);
                    leafletWidgetFormWrite(map._layers, id);
                  }

                  //Show select geographic areas if is enable.
                  if (options.geographic_areas) {
                    $('.geographic_areas_desc').show();
                  }
                }
              });

            }
            if (options.geographic_areas) {
              var json_data = {};
              var selectList = "<div class='geographic_areas_desc'><p></br>Select a state to add into the map:</p><select id='geographic_areas' name='area'>";
              selectList += "<option value='0'>" + Drupal.t('-none-') + "</option>";

              for (i = 0; i < options.areas.length; i++) {
                json_data = jQuery.parseJSON(options.areas[i]);
                $.each(json_data.features, function (index, item) {
                    selectList += "<option value='" + item.id + "'>" + item.properties.name + "</option>";
                });
              }
              selectList += "</select></div></br>";
              $('#' + id + '-input').before(selectList);

              $('#geographic_areas').change(function() {
                var area = $(this).val();

                for (i = 0; i < options.areas.length; i++) {
                  json_data = jQuery.parseJSON(options.areas[i]);
                  $.each(json_data.features, function (index, item) {
                    if (item.id == area) {
                      L.geoJson(item).addTo(map);
                      leafletWidgetFormWrite(map._layers, id);
                    }
                  });
                }
              });
            }
            var map = L.map(id, options.map);

            L.tileLayer(options.map.base_url).addTo(map);

            var current = $('#' + id + '-input').val();
            current = JSON.parse(current);
            var layers = Array();
            if (current.features.length) {
              var geojson = L.geoJson(current)
              for (var key in geojson._layers) {
                layers.push(geojson._layers[key]);
               }
            }

            var Items = new L.FeatureGroup(layers).addTo(map);
            // Autocenter if that's cool.
            if (options.map.auto_center) {
              if (current.features.length) {
                map.fitBounds(Items.getBounds());
              }
            }

            var drawControl = new L.Control.Draw({
                autocenter: true,
                draw: {
                  position: 'topleft',
                  polygon: options.draw.tools.polygon,
                  circle: options.draw.tools.circle,
                  marker: options.draw.tools.marker,
                  rectangle: options.draw.tools.rectangle,
                  polyline: options.draw.tools.polyline
                },
                edit: {
                  featureGroup: Items
                }

              });

              map.addControl(drawControl);

              map.on('draw:created', function (e) {
                var type = e.layerTypee,
                  layer = e.layer;
                // Remove already created layers. We only want to save one
                // per field.
                leafletWidgetLayerRemove(map._layers, Items);
                // Add new layer.
                Items.addLayer(layer);
              });

              $(item).parents('form').submit(function(event){
                if ($('#' + id + '-toggle').hasClass('map')) {
                  leafletWidgetFormWrite(map._layers, id)
                }
              });

            Drupal.leaflet_widget[id] = map;
        });
    }

    /**
     * Writes layer to input field if there is a layer to write.
     */
    function leafletWidgetFormWrite(layers, id) {
      var write  = Array();
      for (var key in layers) {
        if (layers[key]._latlngs || layers[key]._latlng) {
          write.push(layerToGeometry(layers[key]));
        }
      }
      // If no value then provide empty collection.
      if (!write.length) {
        write = JSON.stringify({"type":"FeatureCollection","features":[]});
      }
      $('#' + id + '-input').val('{"type":"FeatureCollection", "features":[' + write + ']}');
    }

    /**
     * Removes layers that are already on the map.
     */
    function leafletWidgetLayerRemove(layers, Items) {
      for (var key in layers) {
        if (layers[key]._latlngs || layers[key]._latlng) {
          Items.removeLayer(layers[key]);
        }
      }
    }

    // This will all go away once this gets into leaflet main branch:
    // https://github.com/jfirebaugh/Leaflet/commit/4bc36d4c1926d7c68c966264f3cbf179089bd998
    var layerToGeometry = function(layer) {
      var json, type, latlng, latlngs = [], i;

      if (L.Marker && (layer instanceof L.Marker)) {
        type = 'Point';
        latlng = LatLngToCoords(layer._latlng);
        return JSON.stringify({"type": type, "coordinates": latlng});

      } else if (L.Polygon && (layer instanceof L.Polygon)) {
        type = 'Polygon';
        latlngs = LatLngsToCoords(layer._latlngs, 1);
        return JSON.stringify({"type": type, "coordinates": [latlngs]});

      } else if (L.Polyline && (layer instanceof L.Polyline)) {
        type = 'LineString';
        latlngs = LatLngsToCoords(layer._latlngs);
        return JSON.stringify({"type": type, "coordinates": latlngs});

      }
    }

    var LatLngToCoords = function (LatLng, reverse) { // (LatLng, Boolean) -> Array
      var lat = parseFloat(reverse ? LatLng.lng : LatLng.lat),
        lng = parseFloat(reverse ? LatLng.lat : LatLng.lng);

      return [lng,lat];
    }

    var LatLngsToCoords = function (LatLngs, levelsDeep, reverse) { // (LatLngs, Number, Boolean) -> Array
      var coord,
        coords = [],
        i, len;

      for (i = 0, len = LatLngs.length; i < len; i++) {
          coord = levelsDeep ?
                  LatLngToCoords(LatLngs[i], levelsDeep - 1, reverse) :
                  LatLngToCoords(LatLngs[i], reverse);
          coords.push(coord);
      }

      return coords;
    }

}(jQuery));
