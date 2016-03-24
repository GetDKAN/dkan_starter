(function($) {

  $(document).on('ready', function(){

    try {
      var settings = JSON.parse($('#edit-field-ve-settings-und-0-value').val());
    } catch (e) {
      var settings = {};
    }

    var $resourceField = $('#edit-field-uuid-resource-und-0-target-uuid');
    var $resizeField = $('#edit-field-ve-table-resize-und');
    var $pagerField = $('#edit-field-ve-table-pager-und');
    var $numRecordsField = $('#edit-field-ve-table-numrecords-und-0-value');
    var $settingsField = $('#edit-field-ve-settings-und-0-value');
    var $numRecordsEl = $('#edit-field-ve-table-numrecords');

    if (!$numRecordsField.val()) {
      $numRecordsField.val('100');
      settings.numRecords = '100';
      $settingsField.val(JSON.stringify(settings));

    }

    if (!$pagerField.prop('checked')) {
      $numRecordsEl.addClass('field-hidden');
    }

    $resourceField.on('autocompleteSelect', function(event, node) {
      var re = /\[(.*?)\]/;
      var uuid = re.exec($resourceField.val())[1];
      settings.source = '/node/' + uuid + '/download';
      $settingsField.val(JSON.stringify(settings));
    });

    $resizeField.on('change', function() {
      settings.resize = $(this).prop('checked');
      $settingsField.val(JSON.stringify(settings));
    });

    $pagerField.on('change', function() {
      var checked = $(this).prop('checked');
      settings.pager = checked;
      $settingsField.val(JSON.stringify(settings));
      if (checked) {
        $numRecordsEl.removeClass('field-hidden');
      } else {
        $numRecordsEl.addClass('field-hidden');
      }
    });

    $numRecordsField.on('change', function() {
      settings.numRecords = $(this).val();
      $settingsField.val(JSON.stringify(settings));
    });

  });
})(jQuery);
