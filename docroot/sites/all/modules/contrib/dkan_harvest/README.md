[![Build Status](https://travis-ci.org/NuCivic/dkan_harvest.svg?branch=7.x-1.x)](https://travis-ci.org/NuCivic/dkan_harvest)

**DEPRECATED:** This module has been moved into [DKAN core](https://github.com/NuCivic/dkan) for release 7.x-1.13. To maintain backward compatibility with DKAN 7.x-1.12 and subsequent patch releases this project will remain on Github but new features will be applied directly to the DKAN core folder `modules/dkan/dkan_harvest`.

If you want to try this module on DKAN < 1.13, do NOT use the `7.x-1.x` branch, which is significantly lacking in features. Latest development has happened on branch `harvest_dkan_integration`. Again, the current `7.x-1.x` branch of [DKAN core](https://github.com/NuCivic/dkan) includes the most stable version of DKAN harvest. Using the bleeding-edge `7.x-1.x` or Waiting for the stable release of DKAN 1.13 is the recommended path for trying DKAN Harvest.

## What is DKAN Harvest?

DKAN Harvest is a module that can be used to regulary harvest open data from open `APIs` **(Only project open data data.json endpoints for now)**. 

### What do you mean by `harvest open data`?

Grab open data from the web and create [DKAN's](http://nucivic.com/dkan) datasets and resources from it.

### How does it works?

It saves the subscribed data locally to files in `drupal's public:// folder`. Then it runs a migration that creates the `dataset` and `resource` DKAN nodes.

### Ok, subscribed data?

You need to let the module know where to find this `open data`. You can do that implementing the `hook_harvest_sources` hook:

```
function hook_harvest_sources() {
  return array(
    'source_id' => array(
      'remote' => 'http://data_json_remote',
      'type' => 'data.json',
      // Filter items preseting the following values (Optional).
      'filters' => array('keyword' => array('health')),
      // Exclude items presenting the following values (Optional).
      'excludes' => array('keyword' => array('tabacco')),
      // Provide defaults (Optional).
      'defaults' => array('keyword' => array('harvested dataset'),
      // Provide overrides (Optional).
      'overrides' => array('author' => 'Author'),
    ),
  );
}
```

We have an `dkan_harvest_example` module in place to provide a clear example on how to accomplish the above.

## Usage

### Harvest data + Migration run

```
# Harvest data and run migration.
$ drush dkan-harvest-run
# Alias
$ drush dkan-hr
```

### Just harvest data
```
# Saves data to public://dkan-harvest-cache
$ drush dkan-cache-harvested-data
# Alias
$ drush dkan-chd
```

### Just run migration
```
# Run migration
$ drush dkan-migrate-cached-data
# Alias
$ drush dkan-mcd
```

## Todo's

+ Move as much as possible from `DataJSONHarvest` class to **dkan_migrate_base** 's own `MigrateDataJsonDatasetBase`
+ Extend functionality to standards other than `data.json`
+ Create drupal admin page to subscribe sources of data.
+ ...
