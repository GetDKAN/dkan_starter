Deployment
----------

Overview
^^^^^^^^^
We make a number of changes for deploying sites. We define deployment as a change in code in an environment. We use a number of environments including the standard "dev/stage/prod" but also local, test, and QA envrionments.

When code is deployed to an environment several changes need to happen based on the needs of the environment:

* Modules turn on or off
* Variable settings change
* Features reverts fired

For example the **maillog** module is turned on on all non-prod environments so users don't get emails from dev, stage, local, or CI environments.

Prerequisites
^^^^^^^^^^^^^

Please review the following pieces of Documentation if you haven't done so yet:

* :doc:`../modules/features-master`
* :doc:`../modules/custom-config`
* :doc:`../modules/data-config`
* :doc:`../modules/devinci`
* :doc:`../modules/environment`

Concept behind "hands free deployments"
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Combining Custom Config (Features Master) and DEVINCI (Environment) functionality we can design a deployment strategy where, if you define the conditions properly per environment and you consider those predefined conditions every time you push code, there's no actual need of manual configuration on acquia environments.

Those predefined conditions are:

* Custom Config holds a list of modules that need to be enabled.
* There are complementary modules that could be enabled (for specific or all environments) setting up values for the **features_master_temp_enabled_modules** variable in settings.php file.
* There are complementary modules that could be disabled (for specific or all environments) setting up values for the **features_master_temp_disabled_modules** variable in settings.php file.
* Any enabled module that's not cover by the above three statements will be disabled.
* There are configuration items (variables) set up for specific (or all) environments.

Hooks
^^^^^

For Granicus Data Projects we are implementing deployment hooks for:

* **post-code-deploy**: This runs when code is deployed from one environment from another (i.e elevating code from dev to test, test to prod)
* **post-code-update**: This runs when code is push to the acquia's master branch. This only runs if the dev environment is running the master branch
* **post-db-copy**: This runs when a database is deployed from on environment to another (i.e moving prod db to test and dev)

The implementation is the same for all of them. We run the following set of commands:

.. code-block:: bash

   # Create variables out of script parameters
   site=$1
   env=$2
   # Construct drush alias from site and env
   drush_alias=$site'.'$env
   # Build the target environment from DEVINCI environment detection
   target_env=`drush @$drush_alias php-eval "echo ENVIRONMENT;"`

   # Begin the deploy. Rebuild Registry
   drush @$drush_alias rr
   # Clear drush cache.
   drush cc drush
   # Force environment switching.
   drush @$drush_alias env-switch $target_env --force
   # Update the database.
   drush @$drush_alias -y updb

This very short set of commands is enough for us to be able to guaranteed that code, configuration and modules to be enable/disable for the site could be deployed successfully without manual tinkering and just relying on git.

Deployment
^^^^^^^^^^

The command that actually does it all is:

.. code-block:: bash

   drush @$drush_alias env-switch $target_env --force

Let's examine what happens when the environment switching occur following dkan_starter settings.php file.

1. Drupal is bootstrapped

   1. DEVINCI environment mapping happens first

   .. code-block:: php

      $env_map = array(
        'local' => 'local',
        'dev' => 'development',
        'test' => 'test',
        'live' => 'production',
        'prod' => 'production',
        'ra' => 'production',
      );
      devinci_set_env($env_map);
   
   2. A set of global (not environment specific) configuration is set below the environment mapping. Things like error reporting, the default mail_system, default caching options, zip compression, fast_404, and many settings more.

   3. Environment specific happens after b) enclosed in a switch statement that analyses the ``ENVIRONMENT`` constant:

   .. code-block:: php

      <?php
      switch(ENVIRONMENT) {
        case 'local':
          $conf['features_master_temp_enabled_modules'] = array(
            'devel',
            'dblog',
            'maillog',
            ...
          );
          $conf['features_master_temp_disabled_modules'] = array(
            'acquia_purge',
            'syslog',
            'expire',
            ...
          );
          ...
          break;
        case 'dev':
          ...
          break;
        case 'test':
          $conf['error_level'] = ERROR_REPORTING_HIDE;
          ...
          break;
        case 'prod':
          $conf['mail_system'] = array (
            'default-system' => 'DefaultMailSystem',
          );
          $conf['page_cache_maximum_age'] = 900;
          $conf['cache'] = 1;
          $conf['preprocess_js'] = 1;
          $conf['preprocess_css'] = 1;
          ...
          break;
      }

   There are tons of specifics per environment here and we encourage to go deep in the code to find out about them. Having said that, the configuration does follow a pattern:

   * **Local** does not need any acquia modules so they are set to be turn off by default
   * **Local** and **Dev** are treated as development environments, so we turn on development modules on those.
   * **Test** mimics the Prod environment in everything BUT email backend configuration. We simply don't want Test to send emails.
   * **Test** and **Prod** are treated as production environments, which means performance is key. We set up caching and do things like adding memcache (if available).
   * **Dev**, **Test**, and **Prod** are set to turn on every acquia module we need to make use of search and performance tuning.

2. Env switching happens

   The definition for what happens on environment switching lives in devinci_custom_environment_switch implementation of hook_custom_environment_switch. For dkan_starter we add it at the bottom of settings.php and it looks like something like this:

   .. code-block:: php

      <?php
      function devinci_custom_environment_switch($target_env, $current_env) {

        switch($target_env) {

          case 'local':
            drupal_flush_all_caches();
            features_master_features_revert('custom_config');
            break;

          case 'development':
          case 'test':
          case 'production':
            drupal_flush_all_caches();
            features_master_features_revert('custom_config');
            features_revert_module('dkan_dataset_groups');
            features_revert_module('dkan_dataset_content_types');
            features_revert_module('custom_permissions');
            break;
        }
      }

   This could vary a little from site to site but the important thing is we run two steps for every environment:
   
   * We flush caches with ``drupal_flush_all_caches()``
   * We features_master_features_revert the custom_config module which holds the list of modules to be enabled.
   * We revert modules that we need to be sure they are reverted (i.e modules containing content types).

   1. Cache flushing

      Pretty self explanatory, it flushes drupal caches.

   2. Revert custom_config

      This does all of the following:
      
      * Enables all the modules declared in custom_config.features_master.inc EXCEPT those specifically set in ``$conf['features_master_temp_disabled_modules']`` for the ``ENVIRONMENT`` the system is switching to.
      * Enables all the modules specifically set in ``$conf['features_master_temp_enabled_modules']`` for the ``ENVIRONMENT`` the system is switching to.
      * Disables everything that's not set explicitally to be enabled/disabled for the ``ENVIRONMENT`` the system is switching to.

   3. Revertion of modules

      We revert everything feature related that we are interested in keeping true to the code.
      The end goal here will be to revert EVERYTHING but at the time of this writing it is not possible. Some rewirring needs to happen on dkan to guarantee that we can do this.
