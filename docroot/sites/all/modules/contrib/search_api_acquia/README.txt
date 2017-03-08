Search API Acquia module
=========================

This module provides integration between your Drupal site and Acquia's
hosted search service. It requires the Acquia Connector module, a valid Acquia
subscription, and Search API Solr module.


Installation
--------------------------------------------------------------------------------

Consult the online documentation at https://docs.acquia.com/network/install for
installation instructions.

Notes on Acquia Search data protection and index auto-switching
--------------------------------------------------------------------------------

Search API Acquia module attempts to auto-detect your environment and automatically
connect to the best-fit Acquia Search index available. This is done to attempt
to protect your data in your production Solr instance; otherwise a development
site could easily overwrite or delete the data in your production index.

This functionality was previously available as a third-party module
https://www.drupal.org/project/acquia_search_multi_subs which will now become
deprecated.

Depending on the indexes already provisioned on your Acquia Subscription, the
module will follow these rules to connect to the proper index:

* If your site is running within Acquia Cloud, Search API Acquia will connect to
  the index name that matches the current environment (dev/stage/prod) and
  current multi-site instance.
* If the module can't find an appropriate index above, it will then enforce
  READ-ONLY mode on the production Solr index. This allows you to still test
  searching from any site while protecting your production, user-facing index.

The current state is noted on the Drupal UI's general status report at
/admin/reports/status, as well as when attempting to edit each connection.

You can override this behavior using code snippets or a Drupal variable. This,
however, poses risks to your data that you should be aware of.  Please consult
our documentation at https://docs.acquia.com/acquia-search/multiple-cores to
find out more.

