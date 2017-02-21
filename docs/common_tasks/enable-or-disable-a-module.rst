Enable or Disable a module
--------------------------

Modules are automatically enabled or disabled every time there is an environment switch. Environment switches happen any time a database changes between Acquia instances or to Circle or locally.

List of Default Enabled Modules
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A list of modules exists in the `data_config_enabled_modules() <https://github.com/NuCivic/dkan_starter/blob/master/assets/modules/data_config/data_config.module#L6>`_ function. This is a list of modules that are always enabled. This list is periodically updated.

List of Custom Enabled Modules 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A list of custom enabled modules exists in the `custom_config_features_master_defaults() <https://github.com/NuCivic/dkan_starter/blob/master/config/modules/custom/custom_config/custom_config.features.features_master.inc>`_ function in `/config/modules/custom/custom_config/custom_config.features.features_master.inc. <https://github.com/NuCivic/dkan_starter/blob/master/config/modules/custom/custom_config/custom_config.features.features_master.inc>`_

This function takes the output of `data_config_enabled_modules() <https://github.com/NuCivic/dkan_starter/blob/master/assets/modules/data_config/data_config.module#L6>`_ and allows the final output to be overwritten.

**ANY MODULE NOT INCLUDED IN THE $features_master ARRAY WILL BE DISABLED ON PRODUCTION.**

Enabling or Disabling a Module on Production
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

While modules can be manually enabled, they will be turned off anytime new code is deployed. 

To add a module to the enabled modules list, add the module to the $features_master list in the `custom_config_features_master_defaults() <https://github.com/NuCivic/dkan_starter/blob/master/config/modules/custom/custom_config/custom_config.features.features_master.inc>`_ function:

.. code-block:: php

 <?php
 function custom_config_features_master_defaults() {

   module_load_include('module', 'data_config');

   $features_master = data_config_enabled_modules();

   // Disable module.
   unset($features_master['example_module_to_disable']);

   // Enable module.
   $features_master['example_module_to_enable']);

   return $features_master;
 }

Temporarily Enabling or Disabling a Module for an Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See :doc:`temp-disable-module-in-environment`

