<?php

/**
 * @file
 * Contains the JavaScript snippet insertion code.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Returns JavaScript snippets.
 *
 * @return array
 *   Associative array of snippets keyed by type: script, noscript and
 *   data_layer.
 */
function google_tag_snippets() {
  $snippets = array(
    'script' => _google_tag_script_snippet(),
    'noscript' => _google_tag_noscript_snippet(),
    'data_layer' => _google_tag_data_layer_snippet(),
  );
  // Allow other modules to alter the snippets.
  drupal_alter('google_tag_snippets', $snippets);
  return $snippets;
}

/**
 * Returns JavaScript script snippet.
 *
 * @return array
 *   The script snippet.
 */
function _google_tag_script_snippet() {
  // Gather data.
  $container_id = variable_get('google_tag_container_id', '');
  $container_id = trim(drupal_json_encode($container_id), '"');
  $data_layer = variable_get('google_tag_data_layer', 'dataLayer');
  $data_layer = trim(drupal_json_encode($data_layer), '"');
  $compact = variable_get('google_tag_compact_snippet', 1);

  // Build script snippet.
  $script = <<<EOS
(function(w,d,s,l,i){

  w[l]=w[l]||[];
  w[l].push({'gtm.start':new Date().getTime(),event:'gtm.js'});
  var f=d.getElementsByTagName(s)[0];
  var j=d.createElement(s);
  var dl=l!='dataLayer'?'&l='+l:'';
  j.type='text/javascript';
  j.src='//www.googletagmanager.com/gtm.js?id='+i+dl;
  j.async=true;
  f.parentNode.insertBefore(j,f);

})(window,document,'script','$data_layer','$container_id');
EOS;
  if ($compact) {
    $script = str_replace(array("\n", '  '), '', $script);
  }
/*
  $script = <<<EOS
<!-- Google Tag Manager -->
$script
<!-- End Google Tag Manager -->
EOS;
*/
  return $script;
}

/**
 * Returns JavaScript noscript snippet.
 *
 * @return array
 *   The noscript snippet.
 */
function _google_tag_noscript_snippet() {
  // Gather data.
  $container_id = variable_get('google_tag_container_id', '');
  $container_id = trim(drupal_json_encode($container_id), '"');
  $compact = variable_get('google_tag_compact_snippet', 1);

  // Build noscript snippet.
  $noscript = <<<EOS
<noscript><iframe src="//www.googletagmanager.com/ns.html?id=$container_id"
 height="0" width="0" style="display:none;visibility:hidden"></iframe></noscript>
EOS;
  if ($compact) {
    $noscript = str_replace("\n", '', $noscript);
  }
/*
  $noscript = <<<EOS
<!-- Google Tag Manager -->
$noscript
<!-- End Google Tag Manager -->
EOS;
*/
  return $noscript;
}

/**
 * Returns JavaScript data layer snippet or adds items to data layer.
 *
 * @return string|null
 *   The data layer snippet or NULL.
 */
function _google_tag_data_layer_snippet() {
  // Gather data.
  $data_layer = variable_get('google_tag_data_layer', 'dataLayer');
  $data_layer = trim(drupal_json_encode($data_layer), '"');
  $whitelist = variable_get('google_tag_whitelist_classes', GOOGLE_TAG_WHITELIST_CLASSES);
  $blacklist = variable_get('google_tag_blacklist_classes', GOOGLE_TAG_BLACKLIST_CLASSES);

  $classes = array();
  $names = array('whitelist', 'blacklist');
  foreach ($names as $name) {
    google_tag_text_clean($$name, 'array');
    if (empty($$name)) {
      continue;
    }
    $classes["gtm.$name"] = $$name;
  }

  if ($classes) {
    // Build data layer items.
    if (module_exists('datalayer') && $data_layer == 'dataLayer') {
      // Add classes to data layer.
      // @todo How to or can we set the name of the data layer?
      // This is an open issue on datalayer project; after it is implemented in
      // datalayer then remove second condition on if block.
      datalayer_add($classes);
    }
    else {
      // Build data layer snippet.
      $script = "var $data_layer = [" . drupal_json_encode($classes) . '];';
      return $script;
    }
  }
}
