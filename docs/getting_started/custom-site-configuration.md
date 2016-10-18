# Custom Site Configuration

These are steps to setup the configuration for your site.

## Adding Site Name

The site name needs to be set in order for the docker local environment to talk to the site correctly as well as for backups and testing to be able to identify the backup location.

The site name is captured in ``config/aliases.local.php``.

You can manually add the site name there or use:

```bash
$ ahoy utils name
```

## Update ``config/config.yml``

The following customizations (other than Acquia environment) are currently available in ``config/config.yml``. We expect to add more configuration settings here including installed modules, variables, and other settings.

```yml
default:
  hostname: localhost                   [We recommend keeping this.]
  https_everywhere: FALSE               [Whether all traffic is redirected to https. Recommended.]
  https_securepages: FALSE              [Whether to use securepages module for mixed https. Not recommended.]
  clamav:     
    enable: FALSE                       [Whether to enable clamav module. Requires clamav support from host.]
  stage_file_proxy_origin: changeme     [Whether to use state file proxy module for files. Recommended as set to staging or production environment.]
  fast_file:      
    enable: TRUE                        [Whether to use DKAN's fast file import for the Datastore.]
    limit: 10MB
    queue: 50MB
private:
  aws:
    scrubbed_data_url: CHANGE ME        [Private S3 bucket for backups.]
  probo:
    password: CHANGE ME                 [Password for user 1 on ProboCI sites. This is changed from production for security reasons.]
```
## Deciding Which Modules are Enabled

Only modules that are part of the list at ``assets/modules/data_config/data_config.module:data_config_enabled_modules()`` or added to ``config/modules/custom/custom_config/custom_config.features.features_master.inc:custom_config_features_master_defaults()`` will be enabled any time you switch environments.

If you want to add modules add them to the list here in the ``custom_config_features_master_defaults()`` function.

There are also modules that are enabled or disabled on certain environments. These are defined in ``assets/sites/default/settings.php`` in the

```php
switch(ENVIRONMENT) {
}
```

statement. They are listed per environment. You can see an example for the ``local`` case:

```php
$conf['features_master_temp_enabled_modules'] = array(
  'dblog',
  'devel',
  'maillog',
  'views_ui',
  'clamav',
);
```

We've added the ``features_master_temp_enabled_modules`` feature so that some modules can be turned on locally or on test environments. The ``maillog`` module keeps emails from being sent to clients or users.

## Adding Custom and Contributed Modules, Libraries and Themes

"Custom" modules are those that are not publicly available and are associated only with your site.

"Contributed" modules are those that are housed out of the site repository.

See:

* [Adding a custom module](../common_tasks/add-custom-module)
* [Adding a contributed module](../common_tasks/add-contrib-module)
* [Adding a contributed library](../common_tasks/add-contrib-library)
* [Adding Custom Behat tests](../common_tasks/add-custom-behat-test)
