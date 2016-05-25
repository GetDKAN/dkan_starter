<?php
/**
 * Transpose config.yml to php array.
 */

include 'vendor/autoload.php';
use Symfony\Component\Yaml\Parser;
use Symfony\Component\Yaml\Dumper;

try {
  $yaml = new Parser();
  $config = $yaml->parse(file_get_contents('../config/config.yml'));
  $file = fopen('../config/config.php', 'w');

  $config_string = var_export($config, TRUE);

  $output = <<<EOT
<?php
/**
 * @file
 * Contents generated automatically by `ahoy site config` command.
 * Do not edit.
 */
\$conf = $config_string;
EOT;
  fwrite($file, $output);

} catch (Exception $e) {
  echo "An error happened trying to transpose the config.yml file:\n{$e->getMessage()}\n";
} finally {
  if ($file) {
    fclose($file);
  }
}

