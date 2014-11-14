(function($) {
  Drupal.behaviors.visualization_entity_geojson_bundle = {
    attach: function(context) {
      var settings = Drupal.settings.visualization_entity_geojson_bundle;

      var div = $('#visualization');
      var map = new L.map(div.get(0)).setView([37.8, -96], 4);

      var geojson = new L.GeoJSON(settings.geojson);
      var bounds = geojson.getBounds();

      for (var layer in geojson._layers) {
        if (geojson._layers.hasOwnProperty(layer)){
          var feature = geojson._layers[layer].feature;
          for ( var j = 1; j < feature.geometry.coordinates[0].length; j++ ) {
            var latlng = feature.geometry.coordinates[0][j];
            latlng = new L.LatLng(latlng[1],latlng[0]);
            bounds.extend(latlng);
          }
        }
      }

      map.fitBounds(bounds);

      var mapUrl = "//otile{s}-s.mqcdn.com/tiles/1.0.0/osm/{z}/{x}/{y}.png";
      var osmAttribution = 'Map data &copy; 2011 OpenStreetMap contributors, Tiles Courtesy of <a href="http://www.mapquest.com/" target="_blank">MapQuest</a> <img src="//developer.mapquest.com/content/osm/mq_logo.png">';
      var bg = new L.TileLayer(mapUrl, {maxZoom: 18, attribution: osmAttribution, subdomains: '1234'});

      map.addLayer(bg);
      geojson.addTo(map);
    },
  };
})(jQuery);
