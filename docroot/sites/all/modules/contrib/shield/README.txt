Description
-----------
This module creates a simple shield for your site with Apache authentication. When configured, users will be prompted for a simple username/password combination.

Requirements
------------
Drupal 7.x

Installation
------------
1. Copy the entire shield directory to your contributed modules directory.

2. Log in as an administrator. Enable the module at admin/modules.

Configuration
-------------
1. Configure which roles have permission to administer the Shield module at admin/people/permissions.

2. Visit admin/config/system/shield to enable authentication.

3a. Complete the user and password fields in the 'credentials' section of the Shield configuration page. When the user field is left empty, authentication will be disabled.

3b. Alternatively, the user (e.g. 'FOO') and password (e.g. 'BAR') values can be configured via settings.php's $conf array:
$conf['shield_user'] = 'FOO';
$conf['shield_pass'] = 'BAR';

3c. Alternatively, the user (e.g. 'FOO') and password (e.g. 'BAR') values can be configured via drush:
drush vset shield_user FOO
drush vset shield_user BAR
# To disable:
drush vdel shield_user

Support
-------
Please use the issue queue for filing bugs with this module at
https://www.drupal.org/project/issues/shield
