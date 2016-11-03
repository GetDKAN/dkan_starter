Devinci Module
--------------

The `DEVINCI module <http://drupal.org/project/devinci>`_ makes your code context/environment aware so you can tweak configuration for your site to your liking without custom scripting.
All you need to do to setup this in your project is to include the following snippet in settings.php:

.. parsed-literal::
     require DRUPAL_ROOT . "/sites/all/modules/contrib/devinci/devinci.environments.inc";
     devinci_set_env();

This would, out of the box, set an ENVIRONMENT constant and do a number of really clever things every time Drupal is bootstrapped such as detecting if:

 * ENVIRONMENT is already set as a drupal constant
 * ENVIRONMENT is defined as a linux environment variable
 * the current instance is an acquia instance and adding the required require "/var/www/site-php/$sitegroup/$sitegroup-settings.inc"; setting file with the proper credentials for your subscription (Pantheon is on the roadmap)
 * there's a custom environment.settings.php file at DRUPAL_ROOT . '/' . conf_path() to be included.

By default, DEVINCI extends the base environment module definitions (development and production) and adds the local and test environments to the mix.
You could also provide your own array of environment mappings to devinci_set_env to contemplate special cases. If you want to properly map acquia environments to the environment + devinci definitions (local, development, test and production) you could do:

.. parsed-literal::
      $env_map = array(
        'local' => 'local',
        'dev' => 'development',
        'test' => 'test',
        'live' => 'production',
        'prod' => 'production',
        'ra' => 'production',
      );
      devinci_set_env($env_map);

this would:

 * map acquia's dev to the environment's development
 * map acquia's test to devinci's test
 * map acquia's live to environment's production
 * map acquia's prod to environment's production
 * map acquia's ra to environment's production

If the ENVIRONMENT constant is set accordingly by any of the above use-cases then you can:

 * Set configuration items based on the environment
 * Run code on environment switching

Set configuration items based on the environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By adding a switch statement to settings.php that analyzes the ENVIRONMENT constant you could set configuration items per environment.

.. code-block:: php

   <?php
   switch(ENVIRONMENT) {
     /**
      * Local Environment
      */
     case 'local':
       // Show ALL errors when working locally.
       $conf['error_level'] = ERROR_REPORTING_DISPLAY_ALL;
       ini_set("display_errors", 1);
   	 break;
     /**
      * Development Environment
      */
     case 'development':
       // Create new alias and delete old.
       $conf['pathauto_update_action'] = 2;
       break;

     /**
      * Test Environment
      */
     case 'test':
       // Enable caching like in production.
       $conf['page_cache_maximum_age'] = 900;
       $conf['cache'] = 1;
       $conf['preprocess_js'] = 1;
       $conf['preprocess_css'] = 1;
       $conf['pathauto_update_action'] = 1;
       break;

     /**
      * Production Environment
      */
     case 'production':
       // Enable the ability to send emails - via core mail in this case,
       // but it coulbe be update to use SMTP or mail API.
       $conf['mail_system'] = array (
         'default-system' => 'DefaultMailSystem',
       );
       // Enable caching for production.
       // 15 minutes max page cache time.
       $conf['page_cache_maximum_age'] = 900;
       $conf['cache'] = 1;
       $conf['preprocess_js'] = 1;
       $conf['preprocess_css'] = 1;
       $conf['pathauto_update_action'] = 1;
       // Set google tag container id.
       $conf['google_tag_container_id'] = '';
       break;

Run code on environment switching
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you add a devinci_custom_environment_switch implementation of environment's hook_custom_environment_switch to your settings.php then you can specify what needs to run when environment switching happens. A very basic implementation would be:
function devinci_custom_environment_switch($target_env, $current_env) {

.. code-block:: php

   <?php
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
       features_revert_module('custom_permissions');
       break;
   }
