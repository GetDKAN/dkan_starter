Acquia Connector modules
================================================================================

An Acquia subscription [1] enhances the Drupal experience by providing the
support and network services to operate a trouble-free Drupal website.
Acquia subscriptions let you gain access to remote network services,
documentation and the Acquia forums. Premium subscriptions provide web-based
support ticket management, as well as telephone support.

These modules allow you to connect any Drupal 7.x site to Acquia Insight.
Acquia also has a distribution of Drupal called Acquia Drupal which is
composed of purely open source GPL licensed components. If you are looking
for a quick start with Drupal, Acquia Drupal [2] might be of great use for you.
(Note that a few Acquia Subscription services, such as the update notification and
code modification detection services, currently only work with Acquia Drupal.)

[1] http://acquia.com/products-services/acquia-network
[2] http://acquia.com/products-services/acquia-drupal

Modules in this project
--------------------------------------------------------------------------------

Acquia agent: Enables secure communication between your Drupal sites and
Acquia Insight to monitor uptime, check for updates, and collect site
information.

Acquia Site profile: Automates the collection of site information -
operating system, database, webserver, and PHP versions, installed modules,
and site modifications - to speed support communication and issue resolution.

Acquia Search: Provides integration between your Drupal site and Acquia's
hosted search service. Requires Apache Solr Search Integration module.

Installation
--------------------------------------------------------------------------------

Consult the online documentation at https://docs.acquia.com/network/install for
installation instructions.

Notes on Acquia Search data protection and index auto-switching
--------------------------------------------------------------------------------

Acquia Search module attempts to auto-detect your environment and automatically
connect to the best-fit Acquia Search index available. This is done to attempt
to protect your data in your production Solr instance; otherwise a development
site could easily overwrite or delete the data in your production index.

This functionality was previously available as a third-party module
https://www.drupal.org/project/acquia_search_multi_subs which will now become
deprecated.

Depending on the indexes already provisioned on your Acquia Subscription, the
module will follow these rules to connect to the proper index:

* If your site is running within Acquia Cloud, Acquia Search will connect to
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

Maintainers
--------------------------------------------------------------------------------

These modules are maintained by developers at Acquia. For more information on
the company and our offerings, see http://acquia.com/

Issues
--------------------------------------------------------------------------------

Contact Acquia Support if you have support questions regarding your site.

If you have issues with the submodules included in the Acquia Connector package,
you are also welcome to submit issues at http://drupal.org/project/acquia_connector
(all submitted issues are public).
