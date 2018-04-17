<?php
`cp -Rn custom/docroot/* docroot/`;
`cp custom/settings.custom.php docroot/sites/default/`;
`drush --root=docroot -y make --no-core --contrib-destination=docroot/sites/all custom/custom.make --no-recursion --no-cache --verbose`;