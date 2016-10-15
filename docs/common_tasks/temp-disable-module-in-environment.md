# Temporarily disable a module for an environment

Some modules are temporarily disabled or enabled for different environments.

For example, maillog and devel are enabled for local and dev environments and acquia search modules are disabled for non-Acquia environments.

To temporarily disable or enable a module for an environment, include it in the **$conf['features_master_temp_disabled_modules']** or **$conf['features_master_temp_enabled_modules']** array in **settings.custom.php**
