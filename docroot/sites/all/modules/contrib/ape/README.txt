SUMMARY
-------
Many sites run behind Varnish and have a desire for some pages to never expire
and other pages to expire as quickly as every minute. The idea behind this
module is that it provides three main features for controlling the cache-control
header:
    1. A secondary cache length that can be used for a list of exception paths.
    2. A list of paths that should be excluded from caching by setting the cache
    control header to no-cache.
    3. Allow cache-control headers for 301 and 302 redirects using drupal_goto
    to have individual cache lengths set.
The module also includes rules integration and manipulates the core performance
page a bit so users can find the module's configuration page. Overall it's a
fairly simple module that just manipulates the cache-control header based on
path, and possibly more complex options with Rules.


REQUIREMENTS
------------
None.


RECOMMENDED MODULES
-------------------
 * Rules (https://www.drupal.org/project/rules):
   When enabled, a new action is available to set the max-age header.


INSTALLATION
------------
* Install as usual, see
https://www.drupal.org/documentation/install/modules-themes/modules-7 for
further information.


CONFIGURATION
-------------
* Configure user permissions in Administration » People » Permissions:
  - Administer advanced page expiration
    Allows access to the advanced page expiration configuration page where
    different criteria can be set for max-age expiration values.
* Customize the menu settings in Administration » Configuration » Performance »
Advanced page expiration.
