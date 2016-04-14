<?php
include 'vendor/autoload.php';
use Symfony\Component\Yaml\Parser;
use Symfony\Component\Yaml\Dumper;

try {
  $yaml = new Parser();
  $overrides_make = $yaml->parse(file_get_contents('../overrides.make'));
  $drupal_org_make = $yaml->parse(file_get_contents('../dkan/drupal-org.make'));

  if (is_array($overrides_make) && is_array($drupal_org_make))  {
    $drupal_org_make_override = array_replace_recursive($drupal_org_make, $overrides_make);
    $dumper = new Dumper();
    $drupal_org_make_yaml = $dumper->dump($drupal_org_make_override, 4);
    file_put_contents('../dkan/drupal-org.make', $drupal_org_make_yaml);
  }

} catch (Exception $e) {

  echo "An error happened trying to override drupal-org.make:\n{$e->getMessage()}\n";
}

