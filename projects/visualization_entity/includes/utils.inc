<?php
/**
 * @file
 * Common utils for Visualization Entity module.
 */

/**
 *  Filter an array by key instead of value.
 */
function visualization_entity_filter_array_by_key($options, $function, $params = array()) {
  foreach ($options as $key => $value) {
    $preserve = call_user_func_array($function, array_merge((array) $key, $params));
    if (!$preserve) {
      unset($options[$key]);
    }
  }
  return $options;
}

/**
 *  Check if a given entity has a valid file in $field_name.
 */
function visualization_entity_validate_file($eid, $entity_type, $field_name, $validator) {
  if (is_int($eid)) {
    $entity = entity_load_single($entity_type, $eid);
    if (!property_exists($entity, $field_name)) {
      return FALSE;
    }
    $entity_file_field = $entity->{$field_name}[LANGUAGE_NONE][0];
    if (empty($entity_file_field)) {
      return FALSE;
    }
    return $validator($entity_file_field);
  }
  else {
    return TRUE;
  }
}

/**
 *  Check if a given file is a valid json file.
 */
function visualization_entity_is_geojson($file_field) {
  $geojson_path = drupal_realpath($file_field['uri']);
  $geojson_file = file_get_contents($geojson_path);
  return json_decode($geojson_file);
}

/**
 *  Check if a given file is a valid csv file.
 */
function visualization_entity_has_csv_extension($file_field) {
  return visualization_entity_has_extension($file_field['uri'], 'csv');
}

/**
 *  Get csv columns from a csv file.
 */
function visualization_entity_csv_columns($file_field) {
  $columns = array_map('strtolower', visualization_entity_csv_row($file_field, 0));
  $columns = array_map('trim', $columns);
  return $columns;
}

/**
 *  Check if csv has any numeric field.
 */
function visualization_entity_has_numeric_field($file_field) {
  $f = fopen(drupal_realpath($file_field['uri']), 'r');
  $values = visualization_entity_csv_row($file_field, 1);
  foreach ($values as $value) {
    if (is_numeric($value)) {
      fclose($f);
      return TRUE;
    }
  }
  fclose($f);
  return FALSE;
}

/**
 *  Check if a path has an extension.
 */
function visualization_entity_has_extension($uri, $extension) {
  return visualization_entity_get_extension($uri) == $extension;
}

/**
 *  Get the extension from uri.
 */
function visualization_entity_get_extension($uri) {
  $resource_path = drupal_realpath($uri);
  $file_mime_type = file_get_mimetype($resource_path);
  $path_parts = pathinfo($resource_path);
  return $path_parts['extension'];
}

/**
 *  Get a row from csv.
 */
function visualization_entity_csv_row($file_field, $row_number) {
  $f = fopen(drupal_realpath($file_field['uri']), 'r');
  $delimiter = ($file_field['delimiter'])? $file_field['delimiter'] : DEFAULT_CSV_DELIMITER;
  for ($i = 0; $i < $row_number + 1; $i++) {
    $line = fgetcsv($f, 0, $delimiter);
  }
  fclose($f);
  return $line;
}

