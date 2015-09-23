(function($) {
  Drupal.behaviors.VisualizationEntityTablesView = {
    attach: function(context) {
      var MAX_ROWS_NUMBER = 100000000;
      var settings = Drupal.settings.visualizationEntityTables;
      var datastore = settings.datastore;
      var fileType = settings.fileType;
      var $body = $(document.body);
      var $container = $('#ve-table');
      var $iframe_shell = $('#iframe-shell');
      var $vizWrapper = $('.visualization-ve-table');
      var pager;
      var title;
      var source;

      // Add wrapper to limit viewport and show scrollbars.
      $container.wrap( "<div class='ve-table-wrapper'></div>" );
      $vizWrapper.append('<div class="alert alert-info loader">Loading <span class="spin"></span></div>');

      if ($iframe_shell.length) {
        $body.removeClass('admin-menu');
        if (settings.showTitle) {
          title = $iframe_shell.find('h2 a').html();
          $body.prepend('<h2 class="veTitle">' + title + '</h2>');
        }
      }

      $iframe_shell.find('h2 a').attr('href', '#');

      // If we have this resource into the datastore then
      // we use it.
      if(datastore){
        var drupal_base_path = Drupal.settings.basePath;
        var DKAN_API = drupal_base_path + 'api/action/datastore/search.json';
        var resource_uuid = settings.resource_uuid;
        source = {
          endpoint: window.location.origin + drupal_base_path + '/api',
          url: window.location.origin + DKAN_API + '?resource_id=' + resource_uuid,
          id: resource_uuid,
          backend: 'ckan'
        };

      // if not then if it's a csv file we try to parse it.
      } else if(fileType == 'text/csv' || fileType == 'csv') {
        source = {
          url: settings.resource,
          backend: 'csv'
        };

      // otherwise we do nothing.
      } else {
        console.warn('Preview files other than csv is not currently supported');
        return false;
      }

      dataset = new recline.Model.Dataset(source);
      dataset.queryState.attributes.size = (settings.pager)
        ? parseInt(settings.numRecords, 10)
        : MAX_ROWS_NUMBER;

      dataset.fetch().done(function() {
        var grid = new recline.View.SlickGrid({
          model: dataset,
          el: $container,
          state: {
            gridOptions: {
              autoHeight: true,
              forceFitColumns: !settings.resize,
            }
          }
        });

        grid.visible = true;
        grid.render();

        // Add pager, if enabled and recordCount > page size
        if (settings.pager && (dataset.recordCount > parseInt(settings.numRecords, 10))) {
          var frameActive = ($iframe_shell.length != 0);
          var recordCountEl = '<div class="ve-recordcount">' + dataset.recordCount + ' Records</div>';

          pager = new recline.View.Pager({
            model: dataset,
          });

          var $pagerContainer = $('<div class="pager-container"></div>');
          $pagerContainer.append(recordCountEl);
          $pagerContainer.append(pager.el);

          grid.listenTo(dataset, 'query:start', function() {
            $vizWrapper.find('.loader').show();
          });

          grid.listenTo(dataset, 'query:done', function() {
            console.log('done');
            $vizWrapper.find('.loader').fadeOut();
          });

          if (frameActive) {
            $('.ve-table-wrapper').parent().prepend($pagerContainer);
          } else {
            $('.visualization-embed').after($pagerContainer);
          }
        }

        // Resize columns to fit content
        if (settings.resize) {
          resizeAllColumns(grid.grid);
          grid.listenTo(dataset, 'query:done', function() {
            resizeAllColumns(grid.grid);
          });
        }

        // Adjust table size.
        tableVerticalResize();
        $vizWrapper.find('.loader').hide();
      });


      //==========================================
      // Functions
      // -----------------------------------------
      function tableVerticalResize() {
        var $title = $body.find('h2.veTitle');
        var height = $title.length > 0
          ? $(window).height() - $body.find('h2.veTitle').outerHeight(true)
          : $(window).height();
        if (settings.pager) {
          height -= $('.pager-container').outerHeight(true);
        }
        $iframe_shell.find('.ve-table-wrapper').height(height);
      }

      // Column resizing based on content width
      // Based on https://github.com/naresh-n/slickgrid-column-data-autosize
      function resizeAllColumns(grid) {
        var elHeaders = $container.find(".slick-header-column");
        var allColumns = grid.getColumns();
        elHeaders.each(function(index, el) {
          var columnDef = $(el).data('column');
          var headerWidth = getElementWidth(el) + 9; // Needed extra right padding
          var colIndex = grid.getColumnIndex(columnDef.id);
          var column = allColumns[colIndex];
          var autoSizeWidth = Math.max(headerWidth, getMaxColumnTextWidth(grid, columnDef, colIndex));
          allColumns[colIndex].width = autoSizeWidth;
        });
        grid.setColumns(allColumns);

        // Adjust width to ensure visibility of horizontal scrollbar in iframe
        var tableWidth = $('.grid-canvas').outerWidth();
        $('#ve-table').width(tableWidth);
        grid.setColumns(allColumns);
      }

      function getMaxColumnTextWidth(grid, columnDef, colIndex) {
        var texts = [];
        var rowEl = createRow(columnDef);
        var data = grid.getData();
        for (var i = 0; i < data.getLength(); i++) {
          texts.push(data.getItem(i)[columnDef.field]);
        }
        var template = getMaxTextTemplate(texts, columnDef, colIndex, data, rowEl);
        var width = getTemplateWidth(rowEl, template);
        deleteRow(rowEl);
        return width;
      }

      function getTemplateWidth(rowEl, template) {
        var cell = $(rowEl.find(".slick-cell"));
        cell.append(template);
        $(cell).find("*").css("position", "relative");
        return cell.outerWidth() + 1;
      }

      function getMaxTextTemplate(texts, columnDef, colIndex, data, rowEl) {
        var max = 0,
        maxTemplate = null;
        var formatFun = columnDef.formatter;
        $(texts).each(function(index, text) {
          var template;
          if (formatFun) {
            template = $("<span>" + formatFun(index, colIndex, text, columnDef, data) + "</span>");
            text = template.text() || text;
          }
          var length = text ? getElementWidthUsingCanvas(rowEl, text) : 0;
          if (length > max) {
            max = length;
            maxTemplate = template || text;
          }
        });
        return maxTemplate;
      }

      function createRow(columnDef) {
        var rowEl = $('<div class="slick-row"><div class="slick-cell"></div></div>');
        rowEl.find(".slick-cell").css({
          "visibility": "hidden",
          "text-overflow": "initial",
          "white-space": "nowrap"
        });
        var gridCanvas = $container.find(".grid-canvas");
        $(gridCanvas).append(rowEl);
        return rowEl;
      }

      function deleteRow(rowEl) {
        $(rowEl).remove();
      }

      function getElementWidth(element) {
        var width, clone = element.cloneNode(true);
        clone.style.cssText = 'position: absolute; visibility: hidden;right: auto;text-overflow: initial;white-space: nowrap;';
        element.parentNode.insertBefore(clone, element);
        width = clone.offsetWidth;
        clone.parentNode.removeChild(clone);
        return width;
      }

      function getElementWidthUsingCanvas(element, text) {
        var context = document.createElement("canvas").getContext("2d");
        context.font = element.css("font-size") + " " + element.css("font-family");
        var metrics = context.measureText(text);
        return metrics.width;
      }
    }
  };
})(jQuery);
