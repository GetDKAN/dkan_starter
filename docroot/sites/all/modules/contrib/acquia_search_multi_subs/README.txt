This module allows you to switch between Acquia Search cores manually or
automatically.

To start you should go to the Solr settings and edit them (see below for
details). By default the module will be enabled and will automatically detect if
you are entitled to multiple cores.

To verify if you have multiple cores available, you can deselect the
"Automatically switch when an Acquia Environment is detected" checkbox and it
will show you a list of the search cores that are available for the subscription
that you connected with using the Acquia Connector.

Getting to the Solr configuration:
- For the Apache Solr Search Integration module: Got to
  admin/config/search/apachesolr/settings and click the Acquia Search
  environment. Click "Edit" to modify the properties of this environment.
- For the Search API Solr Search module: Go to admin/config/search/search_api,
  locate your Acquia Search server and click "Edit".

If you want to test the switching locally add the following to your
settings.php file:

  $_ENV['AH_SITE_NAME'] = 'subscriptionnamedev';
  $_ENV['AH_SITE_ENVIRONMENT'] = 'dev';

You can find these variables when you execute the following on your Acquia
Server:

  debug($_ENV['AH_SITE_ENVIRONMENT']);
  debug($_ENV['AH_SITE_NAME']);

Alternatively you could fill in data from another subscription so that it takes
over your Acquia Search environment without disturbing the connection to the
Acquia Connector / Acquia Insight.
