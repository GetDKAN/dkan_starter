Features Banish Module
----------------------

The `Features Banish Module <https://www.drupal.org/project/features_banish>`_ allows you as a developer to be sure that certain features components will NEVER get exported.

There are three options to leverage this:

 * Banish the component in your feature's mymodule.info file
 * Set the 'features_banish_items' system variable
 * Implement hook_features_banish_alter()

Banishing for DKAN Starter Projects can be exported into the custom_config module. Please refer to :doc:`custom-config` to know more about how are me managing this.
