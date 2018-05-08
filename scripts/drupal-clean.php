<?php
/**
 * @file
 *
 * Clean up the docroot.
 */

$gitignores = [];
exec("find docroot -type f -name '.gitignore'",$gitignores);

foreach ($gitignores as $gitignore) {
  `rm {$gitignore}`;
}