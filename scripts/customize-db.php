<?php
require_once "util.php";

`zcat custom/db.sql.gz | drush --root=docroot sqlc`;