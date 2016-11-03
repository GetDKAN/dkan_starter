Environment Module
------------------

The `Environment Module <http://drupal.org/project/environment>`_ provides a drush command to allow a chain of setup procedures to run when an environment switch happens. This could be whatever you fancy and can do with php within a bootstraped drupal instance.
It provides a hook implementation to allow modules to attach custom setup procedures when the command is called from the cli. The hook in question is hook_environment_switch and it usually looks something like this:

.. code-block:: php

   <?php
    /**
     * Implements hook_environment_switch().
     */
    function YOUR_MODULE_environment_switch($target_env, $current_env) {
      // Declare each optional development-related module
      $devel_modules = array(
        'bulk_export',
        'context_ui',
        'devel',
        'devel_generate',
        'devel_node_access',
        'imagecache_ui',
        'update',
        'spaces_ui',
        'views_ui',
      );

      switch ($target_env) {
        case 'production':
          module_disable($devel_modules);
          drupal_set_message('Disabled development modules');
          return;
        case 'development':
          module_enable($devel_modules);
          drupal_set_message('Enabled development modules');
          return;
      }
    }

With the scenario above when you call the drush command from the cli to switch to the so called development environment :

.. code-block:: bash

    drush env-switch development

it will enable all the module defined in the $devel_modules variable. On the other hand, when you switch to the production environment:

.. code-block:: bash

    drush env-switch production

The opposite will happen (all the devel modules will be disable). This is of course a very simple implementation of what you can achieve with this module.

For DKAN Starter we are making heavy use of the environment module to allow stuff to happen when we deploy code and copy databases through environments. If you want to fully grasp how we are doing this you need to read the following pieces of documentation:

 * :doc:`custom-config`
 * :doc:`devinci`
 * Hands free deployments
