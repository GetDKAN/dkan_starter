<?php

namespace Drupal\DKANExtension\Context;

use Behat\Behat\Hook\Scope\BeforeFeatureScope;

/**
 * Defines application features from the specific context.
 */
class ClamAvContext extends RawDKANContext {

  public static $modules_before_feature = array();

  /**
   * @BeforeFeature @clamav
   */
  public static function BeforeFeatureClamav(BeforeFeatureScope $scope) {
    self::$modules_before_feature = module_list(TRUE);

    @module_enable(array(
      'clamav',
    ));
    drupal_flush_all_caches();
  }

  /**
   * @AfterFeature @clamav
   */
  public static function AfterFeatureClamav(BeforeFeatureScope $scope) {
    $modules_after_feature = module_list(TRUE);

    $modules_to_disable = array_diff_assoc(
      $modules_after_feature,
      self::$modules_before_feature
    );
    module_disable(array_values($modules_to_disable));
    drupal_uninstall_modules(array_values($modules_to_disable));
    drupal_flush_all_caches();
  }

}
