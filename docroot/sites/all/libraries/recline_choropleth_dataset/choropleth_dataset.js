var state = [];
(function($) {
  Drupal.behaviors.choroplethDataset = {
    attach: function(context) {
      // This toggle is for enabling the hash history. Disable it for testing.
      var use_hash = true;
      var targetDiv = Drupal.settings.choroplethDataset.targetDiv;
      var resources = Drupal.settings.choroplethDataset.resources;
      var pointLabel = Drupal.settings.choroplethDataset.pointLabel;
      var location_default = [];
      // Set to contiguous USA.
      location_default.lat = 51.25;
      location_default.lon = -110;
      location_default.zoom = 3;
      var oHash = {values:{resource:'', year:'', category:'', checkedPoints:''}};
      var view;
      var dataset;

      // Prepare array of breakpoints.
      for (var i = 0; i < resources.length; i++) {
        if (resources[i].breakpoints.length) {
          var breakpoints = resources[i].breakpoints.split(',');
          var items = [];
          for (var k = 0; k < breakpoints.length; k++) {
            items[k] = breakpoints[k];
          }
          resources[i].breakpoints = items;
        }
      }

      /**
       * Checks if every resource is loaded
       * @return {boolean}
       *   a boolean indicating if all resources are loaded.
       */
      function resourcesLoaded() {
        var loaded = true;
        $.each(resources, function(k, e) {
          if (!resources[k].loaded) {
            loaded = false;
            return false;
          }
        });
        return loaded;
      }

      // Make an ajax call for each resource.
      $.each(resources, function(k, e) {
        $.ajax({
          url: resources[k].file,
          timeout: 3000,
          success: function(data) {
            // On load, create instance of recline model.
            data = data.replace(/(\r\n|\n|\r)/gm,"\n");
            dataset = new recline.Model.Dataset({
              records: recline.Backend.CSV.parseCSV(data, {delimiter: resources[k].delimiter}),
            });

            dataset.fetch();
            resources[k].loaded = true;
            resources[k].dataset = dataset;

            // If all resources are loaded, create instance of view.
            if (resourcesLoaded()) {
              // Pointers to containers.
              var container = $(targetDiv + ' .data-view-container');
              var sidebar = $(targetDiv + ' .data-view-sidebar');
              $(document).trigger('choropleth-prepare', [resources, dataset, state]);
              // Make view instance.
              var options = {
                polygons: Drupal.settings.choroplethDataset.statesData,
                resources: resources,
                pointLabel: pointLabel,
                label_to_map: 'state',
                location_default: location_default,
              };
              options = _.extend(state, options);
              view = new recline.View.MultiDatasetChoroplethMap(options);
              // Grasp the element an attach them to the containers.
              container.append(view.el);
              sidebar.append(view.elSidebar);
              // Make the render.
              view.render();
              $(document).trigger('choropleth-ready', view, state);
            }
          },
          error: function(x, t, m) {
            // delete resources[k];
          }
        });
      });


      /**
       * Set the hash to match the current values of the selectors.
       *
       * @returns {Boolean}
       */
      oHash.setHash = function() {
        if (use_hash) {
          // Read the values of the currently selected selectors and use them.
          oHash.values.resource = $("#resource-form option:selected").val();
          oHash.values.category = view.state.get('category');
          oHash.values.year = view.state.get('year');
          oHash.values.columnToDisplay = view.state.get('columnToDisplay');

          // Cycle through point based checkboxes to get ids of any checked.
          //var checked = $('#points-form input[type="checkbox"]:checked').map(function(index,dom){return dom.id});
          var checkedString = '';
          $('#points-form input[type="checkbox"]:checked').each(function(){
            checkedString += $(this).attr('id').replace("recline-point-input-","");
            checkedString += '-';
          });
          oHash.values.checkedPoints = checkedString.slice(0, - 1);
          oHash.changedByFilter = true;
          document.location.hash = oHash.values.resource + '|' + oHash.values.year + '|' + oHash.values.category + '|' + oHash.values.checkedPoints;
          // Update the hash on the embed code.
          oHash.setEmbedHash();

          return true;
        }
      };


      /**
       * Reads the current hash and builds the options.state from it.
       *
       * @returns {Boolean}
       */
      oHash.readHash = function() {
        if (use_hash) {
          // Read the hash and get rid of #.
          var sHash = document.location.hash.replace('#','');
          sHash = sHash || '';
          // Parse the hash at |.
          var aHashVars = sHash.split("|");
          oHash.values.resource = (typeof aHashVars[0] !== 'undefined' ) ? aHashVars[0] : '';
          oHash.values.year = (typeof aHashVars[1] !== 'undefined' ) ? aHashVars[1] : '';
          oHash.values.category = (typeof aHashVars[2] !== 'undefined' ) ? aHashVars[2] : '';
          oHash.values.checkedPoints = (typeof aHashVars[3] !== 'undefined' ) ? aHashVars[3] : '';

          //var state = view.state.toJSON();

          if (!!oHash.values.resource) {
            // Set the resource control.
            var resource_key = 0;
            // Loop through possible datasets to get the dataset id to make active.
            for (index = 0; index < resources.length; ++index) {
              if (resources[index].title == oHash.values.resource) {
                resource_key = index;
              }
            }
            state['activeDataset'] = resource_key;
          }

          if (!!oHash.values.year) {
           //Set the year control.
            state['year'] = parseInt(oHash.values.year, 10);
          }

          if (!!oHash.values.category) {
            // Set the category control.
            state['category'] = oHash.values.category;
          }

          if (!!oHash.values.checkedPoints) {
           // Split on - to get individual resource numbers in an array.
           state['activePoints'] = oHash.values.checkedPoints.split("-");
          }
          else {
            state['activePoints'] = [];
          }

        }
        return true;
      };

      /**
       * Reads the hash and sets the controls to the values of the hash.
       *
       * @returns {Boolean}
       */
      oHash.matchHash = function() {
        if (use_hash) {
          oHash.readHash();

          var resource_key = state['activeDataset'];
          var columnToDisplay = resources[resource_key]['fieldToDisplay'].toLowerCase();
          var selectable_columns = view.menu._refreshSelectableColumns(resource_key);
          state['selectableColumns'] = columnToDisplay ? [columnToDisplay] : selectable_columns;
          state['columnToDisplay'] = columnToDisplay ? columnToDisplay : selectable_columns[0];



          view.menu.state.set(state);
          view.menu.render();
          view.menu.updateColorScale(view.breakpoints, view.base_color);
          view.pointRedraw();
          oHash.setEmbedHash();
        }
        return true;
      };

      /**
       * Takes the current hash and appends it to the embed iframe url.
       *
       * @returns {Boolean}
       */
      oHash.setEmbedHash = function() {
        if (use_hash) {
          var iframe_original = $('#embed-me').text();
          // Make it look like a dom element rather than just straight text.
          var iframe = '<div>'+iframe_original+'</div>';
          var src_original = $('iframe', iframe).attr('src');
          // Replace src with the new location.
          var location_new = Drupal.settings.choropleth_dataset.embedable_url + document.location.hash;
          var iframe_new = iframe_original.replace(src_original, location_new);
          iframe_original = $('#embed-me').text(iframe_new);
        }
        return true;
      };



      //////////// BINDS /////////////////////////////////

      // Bind to watch for actual changes to the URL hash.
      $(window).bind('hashchange', function(e) {
        // Determine if it was from a control interaction or history change.
        if (oHash.changedByFilter) {
          // Hash was changed by filter so do nothing but reset the flag.
          oHash.changedByFilter = false;
        }
        else {
          // Hash was changed by back button or history change.
          oHash.matchHash();
        }
      });

      // Bind the embed toggle.
      $('#embed-toggle').on("click",function(){
        $('#embed-toggle-reveal').toggleClass('element-hidden');
      });


      // Bind to the choropleth-changed event.
      $(document ).on( "choropleth-changed", function( event, selector, value ) {
        if ((typeof selector !== 'undefined' ) && (typeof value !== 'undefined')) {
          oHash[selector] = value;
        }
        oHash.setHash();
      });


      // Bind to the choropleth-prepare event.
      $(document ).on( "choropleth-prepare", function(event, resources, dataset, state) {
        // Read the hash and build the state options so they will be present at choropleth initialize.
        oHash.readHash();
      });

      // Bind to the choropleth-ready event.
      $(document ).on( "choropleth-ready", function(event, view, state) {
        // Hide the loader.
        $('#choropleth-dataset .loader').remove();
        oHash.setHash();
      });
    }
  };
})(jQuery);
