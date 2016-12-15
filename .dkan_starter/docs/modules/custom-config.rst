Custom Config
-------------

The idea for the "Custom Config" module is to have all of the overrides for a DKAN project in a single place. These would not include new features that a project uses but just the configuration changes. This makes it easier to see and evaluate the configurations for a DKAN project because they are in a uniform place.

The "Custom Configuration" module is designed to contain:

* A list of custom modules to enable by default
* Overrides of "Data Config"
* Custom functions that override DKAN
* Features exports that override DKAN
* All the features components that need to be banished through features_banish
* custom updates (hook_update_N)

List of Custom Modules to Enable
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``/config/modules/custom_config/custom_config.features.features_master.inc`` file contains a list of all of the modules and themes that should be enabled or disabled by default in a project that are not part of DKAN Starter.

The list of modules that DKAN Starter enables can be found in ``/assets/modules/data_config/data_config.module``.

For more information see :doc:`../common_tasks/enable-or-disable-a-module`.
