(function($) {
  Drupal.behaviors.VisualizationEntityTablesView = {
    attach: function(context) {
      var title;
      var $body = $(document.body);

      // Add wrapper to limit viewport and show scrollbars.
      $('#ve-table').wrap( "<div class='ve-table-wrapper'></div>" );

      var $container = $('#ve-table');

      $container.append('<p id="ve-loading">Loading…</p>');

      if ($('#iframe-shell').length) {
        $body.removeClass('admin-menu');
        if (Drupal.settings.visualizationEntityTables.showTitle) {
          title = $('#iframe-shell').find('h2 a').html();
          $body.prepend('<h2 class="veTitle">' + title + '</h2>');
        }
      }

      function tableVerticalResize() {
        var $title = $body.find('h2.veTitle');
        console.log($title);
        var height = $title.length > 0
          ? $(window).height() - $body.find('h2.veTitle').outerHeight(true)
          : $(window).height();
        $('#iframe-shell .ve-table-wrapper').height(height);
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
      // End of Column resizing code

      $('#iframe-shell h2 a').attr('href', '#');

      var source = {
        url: Drupal.settings.visualizationEntityTables.resource,
        backend: 'csv',
      }
      dataset = new recline.Model.Dataset(source);

      // Remove limitation of 100 rows. There is no 'unlimited' setting.
      dataset.queryState.attributes.size = 10000000;

      dataset.fetch().done(function() {

        var grid = new recline.View.SlickGrid({
          model: dataset,
          el: $container,
          state: {
            gridOptions: {
              autoHeight: true,
              forceFitColumns: !Drupal.settings.visualizationEntityTables.resize,
            }
          }
        });

        grid.visible = true;
        grid.render();

        // Resize columns to fit content
        if (Drupal.settings.visualizationEntityTables.resize) {
          resizeAllColumns(grid.grid);
        }

        // Adjust table size.
        tableVerticalResize();

        $('#ve-loading').remove();

      });
    }
  };
})(jQuery);
