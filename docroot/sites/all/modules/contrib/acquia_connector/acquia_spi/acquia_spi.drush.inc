<?php
/**
 * @file
 * Drush integration for the acquia_spi module.
 */

/**
 * Implements hook_drush_command().
 */
function acquia_spi_drush_command() {
  $items = array();
  $items['spi-get'] = array(
    'description' => dt('Output raw Acquia SPI data.'),
    'options' => array(
      'outfile' => dt('Optional. A file to write data to in the current directory. If omitted Drush will output to stdout.'),
      'format' => dt('Optional. Format may be json, print_r, or var_dump. Defaults to print_r.'),
    ),
    'examples' => array(
      'spi-get --format=json --outfile=spi.json' => 'Write JSON encoded SPI data to spi.json in current directory.',
    ),
  );
  $items['spi-test-validate'] = array(
    'callback'    => 'drush_acquia_spi_custom_test_validate',
    'description' => dt('Perform a validation check on any modules with Acquia SPI custom tests.'),
    'aliases'     => array('spi-tv'),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function acquia_spi_drush_help($section) {
  switch ($section) {
    case 'drush:spi-get':
      return dt("Output raw Acquia SPI data.
 NOTE: If the Acquia SPI module is not enabled or installed, execute with:
   drush -i !spidir
 ", array('!spidir' => dirname(__FILE__)));
  }
}

/**
 * SPI get callback.
 */
function drush_acquia_spi_spi_get() {
  $raw_spi = _acquia_spi_drush_get();
  switch (drush_get_option('format','print_r')) {
    case 'json':
      $spi = drush_json_encode($raw_spi);
      break;
    case 'var_dump':
    case 'var_export':
      $spi = var_export($raw_spi, 1);
      break;
    case 'print_r':
    default:
      $spi = print_r($raw_spi, 1);
      break;
  }
  if ($file = drush_get_option('outfile')) {
    // reset our working directory so that relative paths work as expected
    $wd = getcwd();
    chdir($_SERVER['PWD']);
    if (file_put_contents($file, $spi)) {
      drush_log(dt('SPI Data written to !outfile.', array('!outfile' => realpath($file))), 'success');
    }
    chdir($wd);
  }
  else {
    echo $spi;
  }
}

/**
 * If the Acquia SPI module is not enabled, include acquia_spi.module.
 *
 * HINT: invoke drush with the --include option pointing module folder,
 * otherwise drush will not execute spi-get.
 */
function _acquia_spi_drush_get() {
  global $conf;
  $conf['acquia_spi_ssl_override'] = TRUE;
  if (!function_exists('acquia_spi_get')) {
    drush_include(dirname(__FILE__), 'acquia_spi', drush_drupal_major_version(), 'module');
  }
  if (!function_exists('acquia_agent_load_versions')) {
    drush_include(dirname(dirname(__FILE__)) . '/acquia_agent', 'acquia_agent', drush_drupal_major_version(), 'module');
  }
  return acquia_spi_get();
}

/**
 * A command callback and drush wrapper for custom test validation.
 *
 */
function drush_acquia_spi_custom_test_validate() {

  $pass = array();
  $failure = array();

  $modules = module_implements('acquia_spi_test');
  drush_print_r(dt("Acquia SPI custom tests were detected in: @modules \n", array('@modules' => implode(', ', $modules))));

  foreach (module_implements('acquia_spi_test') as $module) {
    $function = $module . '_acquia_spi_test';
    if (function_exists($function)) {
      $result = acquia_spi_test_validate($function());

      if (!$result['result']) {
        $failure[] = $module;
        drush_print_r(dt("[FAILED]  Validation failed for '@module' and has been logged.", array('@module' => $module)));
        $custom_data[$module] = $result;

        foreach ($result['failure'] as $test_name => $test_failures) {
          foreach ($test_failures as $test_param => $test_value) {
            $variables = array(
              '!module_name' => $module,
              '@message'     => $test_value['message'],
              '!param_name'  => $test_param,
              '!test_name'   => $test_name,
              '!value'       => $test_value['value'],
            );
            drush_print_r(dt("[DETAILS] @message for parameter '!param_name'; current value '!value'. (Test !test_name in module !module_name)", $variables));
            watchdog('acquia spi test', "<em>Custom test validation failed</em>: @message for parameter '!param_name'; current value '!value'. (<em>Test '!test_name' in module '!module_name'</em>)", $variables);
          }
        }
      }
      else {
        $pass[] = $module;
        drush_print_r(dt("[PASSED]  Validation passed for '@module.'", array('@module' => $module)));
      }
    }
    drush_print_r("");
  }

  $message = 'Validation checks completed.';
  if (count($pass) > 0) {
    $variables['@passes'] = implode(', ', $pass);
    $variables['@pass_count'] = count($pass);
    $message .= "\n@pass_count module(s) passed validation: @passes.";
  }
  if (count($failure) > 0) {
    $variables['@failures'] = implode(', ', $failure);
    $variables['@fail_count'] = count($failure);
    $message .= "\n@fail_count module(s) failed validation: @failures.";
  }

  drush_print_r(dt($message, $variables));
}
