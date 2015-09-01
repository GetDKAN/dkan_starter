/**
 * @file
 * Acquia Purge - AJAX client side queue processor.
 *
 * This behavior will call to Acquia Purge's AJAX path until its purging queue
 * is empty and everything is processed. Also works without on-screen reporting.
 */

(function ($) {
  Drupal.behaviors.AcquiaPurgeAjaxProcessor = {
    attach: function (context) {
      $(document).ready(function() {

        // Declare the trigger path the script will call back home to.
        var triggerPath = 'acquia_purge_ajax_processor';
        var triggerPath = Drupal.settings.basePath + triggerPath;

        // Declare reference variables to the main container and its elements.
        var apbox = '.acquia_purge_messages';
        var apbox_errors = '#aperror';
        var apbox_widget = '#apwidget';
        var apbox_log = '#aplog';

        // Declare a error counter and a error limit before we stop processing.
        var errorCounter = 0;
        var errorCounterLimit = 3;

        // Determine if on-screen reporting is enabled or not.
        function uiActivated() {
          if ($(apbox).length > 0) {
            return true;
          }
          return false;
        }

        // Prepare the container element with the structures that we will need.
        function uiInitialize() {

          // Set a element reference to the apbox container.
          apbox = $(apbox);

          // Wrap the currently existing HTML string into apbox_widget.
          apbox.html("<div id='apwidget'>" + apbox.html() + "</div>");
          apbox_widget = apbox.find(apbox_widget);

          // Prepend the hidden errors container and reference it.
          var html = "<div id='aperror' style='display:none;' class='messages ";
          html = html + "error'>&nbsp;</div>";
          apbox.prepend(html);
          apbox_errors = apbox.find(apbox_errors);

          // Append the unordered purge log to the container and reference it.
          html = "<div id='aplog' style='display: none;'><ul></ul></div>";
          apbox.append(html);
          apbox_log = apbox.find(apbox_log);

          // As the apbox container is hidden by default, reveal our loading UI.
          apbox.show();
        }

        // Setup the given error message in the container and visualize. Returns
        // TRUE when things can continue or FALSE when the error limit crossed.
        function uiError(message) {
          var message = typeof message !== 'undefined' ? message : false;
          var message_old = apbox_errors.html();

          // If message wasn't passed we hide and delete the current message.
          if (!message) {
            apbox_errors.slideUp('slow');
            apbox_errors.html('&nbsp;');
            return;
          }
          else {

            // Increase the error counter.
            errorCounter = errorCounter + 1;
          }

          // Don't do anything when the new and old messages are the same.
          if (message == message_old) {
            return (errorCounter < errorCounterLimit) ? true : false;
          }

          // When the box is invisible: set the message and unfold.
          if (!apbox_errors.is(':visible')) {
            apbox_errors.html(message);
            apbox_errors.slideDown('slow');
            return (errorCounter < errorCounterLimit) ? true : false;
          }

          // The only resulting case is a message replacement, handle nicely.
          var apn = "<div class='apn' style='display:none;'>"
            + message + "</div>";
          apbox_errors.html("<div class='apo'>" + apbox_errors.html()
            + "</div>");
          apbox_errors.append(apn);
          apbox_errors.find('.apo').slideUp('slow');
          apbox_errors.find('.apn').slideDown('slow', function() {
            apbox_errors.html(message);
          });
          return (errorCounter < errorCounterLimit) ? true : false;
        }

        // Enable the throbber on the widget container.
        function uiThrobberOn() {
          var throbber_path = Drupal.settings.basePath + 'misc/throbber.gif';
          var throbber = apbox.find('#apthrobr');

          // Create the throbber when it doesn't exist.
          if (!($(throbber).length > 0)) {
            var html = "<div id='apthrobr' style='display: none;'>&nbsp;</div>";
            apbox_errors.after(html);
            throbber = apbox.find('#apthrobr');
            throbber.css('background-image', 'url(' + throbber_path + ')');
            throbber.css('background-color', 'transparent');
            throbber.css('background-position', '0px -18px');
            throbber.css('background-repeat', 'no-repeat');
            throbber.css('margin-top', '-21px');
            throbber.css('position', 'relative');
            throbber.css('z-index', '1');
            throbber.css('height', '18px;');
            throbber.css('width', '18px');
            throbber.css('top', '1.8em');
            throbber.css('left', '-2.2em');
          }

          // Then just make it visible.
          throbber.fadeIn(100);
        }

        // Disable the throbber on the widget container.
        function uiThrobberOff() {
          var throbber = apbox.find('#apthrobr');
          throbber.fadeOut(100);
        }

        // Add new items to the purge log history widget.
        function uiLogHistory(purges) {
          var list_items_limit = 10;
          var list_items = apbox_log.find('ul');

          // Slowly slide the widget on screen once purges are logged.
          if (!apbox_log.is(':visible')) {
            apbox_log.show();
          }

          // Iterate each URL and append it to the list items.
          $.each(purges, function(key, url) {

            // Check the existing list and add the item if its new to us.
            var alreadyInList = false;
            list_items.find('li').each(function(index) {
              if ($(this).text() == url) {
                alreadyInList = true;
              }
            });

            // Addition logic when the item is indeed unique.
            if (!alreadyInList) {

              // List quota reached, skip effects.
              if (list_items.find('li').length == list_items_limit) {
                list_items.find('li').first().remove();
                list_items.append("<li>" + url + '</li>');
                list_items.find('li').last().css('list-style', 'none');
              }
              else {
                list_items.append("<li style='display:none;'>" + url + '</li>');
                list_items.find('li').last().css('list-style', 'none');
                list_items.find('li').last().slideDown(500);
              }
            }
          });
        }

        // Build off the log history viewer and hide its items.
        function uiLogHistoryHide() {
          apbox_log.fadeTo(1000, 0).slideUp(1000);
        }

        // Tear the user interface down and hide it for the user.
        function uiTearDown() {
          uiThrobberOff();
          uiLogHistoryHide();
          uiError();

          // Hide ourselves with a quick animation.
          setTimeout(function() {apbox.slideUp(500);}, 600);
        }

        // Make a request back home and trigger a couple of purges each run.
        function eventLoopRun() {

          // Initialize the throbber.
          if (uiActivated()) {
            uiThrobberOn();
          }

          // Start a recursive call and call ourselves upon success.
          $.ajax({
            url: triggerPath,
            cache: false,
            dataType: "json",
            context: document.body,
            success: function(data) {

              // Replace the inner text with the loaded widget.
              $(apbox_widget).html(data['widget']);

              // Disable the throbber since we're done again.
              if (uiActivated() && (!data['locked'])) {
                uiThrobberOff();
              }

              // Report successfully purged URLs to the GUI's logging widget.
              if (uiActivated() && (data['purgehistory'].length > 0)) {
                uiLogHistory(data['purgehistory']);
              }

              // Handle error conditions and remove errors when they are gone.
              var errorHandlerCallsForHalt = false;
              if (uiActivated() && data['error']) {
                if (!uiError(data['error'])) {
                  errorHandlerCallsForHalt = true;
                }
              }
              else if (uiActivated() && (!data['error'])) {
                uiError();
              }

              // Follow up a next request with a small pause.
              if (data['running']) {
                if (!errorHandlerCallsForHalt) {
                  if (data['locked'] || data['error']) {
                    setTimeout(function() {eventLoopRun();}, 5000);
                  }
                  else {
                    setTimeout(function() {eventLoopRun();}, 500);
                  }
                }
                else {
                  uiThrobberOff();
                }
              }

              // Start building off the interface since the work is done.
              else {
                if (uiActivated()) {
                  uiTearDown();
                }
                quit();
              }
            },
            error: function(request) {

              // Sometimes requests randomly fail with HTTP 200 (OK), continue
              // processing requests as it most probably just did its work.
              if (Number(request['status']) == 200) {
                if (uiActivated()) {
                  uiThrobberOff();
                }
                eventLoopRun();
              }

              // 403 responses indicate the backend logged out, quit.
              else if (Number(request['status']) == 403) {
                if (uiActivated()) {
                  uiTearDown();
                }
                quit();
              }

              // Else, report the error occurred and tear the UI partly down.
              else {
                if (uiActivated()) {
                  uiThrobberOff();
                  uiLogHistoryHide();
                  var msg = "Something went wrong while communicating with the";
                  msg = msg + " server. Last known response was '";
                  msg = msg + request['statusText'] + "' with HTTP code ";
                  msg = msg + request['status'] + ".";
                  uiError(msg);
                }
              }
            }
          });
        }

        // Remove the behavior so it won't act up when AJAX requests are made.
        function quit() {
          delete Drupal.behaviors.AcquiaPurgeAjaxProcessor;
        }

        // Initialize the UI when we have detected its base element.
        if (uiActivated()) {
          uiInitialize();
        }

        // Start the cascade of purge events until its marked finished.
        eventLoopRun();
      });
    }
  };

})(jQuery);
