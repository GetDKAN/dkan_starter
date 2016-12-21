# Custom Search Facets

## The problems

### Search api index feature export is messy

The feature exports for the dkan core search api indexes are in fact `entity_imports` and the value being imported is a **json string**. You can't get around this with `features_overrides`, it does not offer a way to override individual items of the feature.


```php
/**
 * Implements hook_default_search_api_index().
 */
function dkan_sitewide_search_db_default_search_api_index() {
  $items = array();
  $items['datasets'] = entity_import('search_api_index', '{
    "name" : "datasets",
    "machine_name" : "datasets",
    ...
      "fields" : {
        "author" : { "type" : "integer", "entity_type" : "user" },
        "changed" : { "type" : "date" },
        "created" : { "type" : "date" },
        "field_author" : { "type" : "string" },
        "field_license" : { "type" : "string" },
        "field_resources:body:value" : { "type" : "list\\u003Ctext\\u003E" },
        "field_resources:field_format" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_tags" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_topic" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "og_group_ref" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "search_api_access_node" : { "type" : "list\\u003Cstring\\u003E" },
        "search_api_language" : { "type" : "string" },
        "search_api_viewed" : { "type" : "text" },
        "status" : { "type" : "boolean" },
        "title" : { "type" : "string" },
        "type" : { "type" : "string" }
      },
```

### Facets

Facet configuration for fields don't play nice with features as well

### Search page panel

The whole search page is exported as a feature

## Mise en place

Prepare a module to hold the search customizations. For the porpouse of this docs, let's name it `dkan_search_customizations`.

In `dkan_search_customizations.module` prepare a helper function to retrieve all the new taxonomies to be added to the index:

```php
function dkan_search_customizations_facet_list() {
  return array(
    'field_brand',
    'field_country',
    'field_customer_type',
  );
}
```

This array is going to be used multiple times so it's good practice to avoid repetition.

Same goes for the searchers/indexes you want to add these facets to. Prepare another helper function to retrieve those:

```php
function dkan_search_customizations_searchers_list() {
  return array(
    'search_api@datasets',
    'search_api@groups_di'
  );
}
```

## The solutions

### Add fields to the index

Implement `hook_default_search_api_index_alter`

```php
/**
 * Implements hook_default_search_api_index_alter().
 */
function dkan_search_customizations_default_search_api_index_alter(&$defaults) {
  $searchers = dkan_search_customizations_searchers_list();
  foreach ($searchers as $searcher) {
    $searcher = str_replace('search_api@', '', $searcher);
    $facets = dkan_search_customizations_facet_list();
    foreach ($facets as $facet) {
      if (isset($defaults[$searcher])) {
        $defaults[$searcher]->options['fields'][$facet] = array(
          'type' => 'list<integer>',
          'entity_type' => 'taxonomy_term',
        );
      }
    }
  }
}
```

This will mark the field to be indexed for the given indexes/searchers.

#### QA

* Revert the core search modules
````
ahoy drush -y fr --force dkan_sitewide_search_db dkan_dataset_groups
````
* Go to `/admin/config/search/search_api`
* All the overriden indexes should appear to be using the *Default* Configuration (Not overriden)
* Inspect `admin/config/search/search_api/index/<searcher|index>/fields` for all indexes in `dkan_search_customizations_searchers_list`
* All fields in `dkan_search_customizations_facet_list` should be checked to be indexed

### Enable facets for the indexes

There's no hook implementation to accomplish this. The facet configuration needs to be saved in the db. In order to do this, the most straigthforward solution is to write a function to take care of enabling the facets and have that function run on deployment through the env-switch hook. It'll look something like this:

```php
function dkan_search_customizations_enable_facets() {
  $searchers = dkan_search_customizations_searchers_list();
  foreach ($searchers as $searcher) {
    $realm = 'block';
    $facets_to_enable = dkan_search_customizations_facet_list();
    $adapter = facetapi_adapter_load($searcher);
    $facet_info = facetapi_get_facet_info($searcher);
    foreach (array_keys($facet_info) as $item) {
      $facet = facetapi_facet_load($item, $searcher) ;
      if (in_array($item, $facets_to_enable)) {
        $facet_settings = $adapter->getFacet($facet)->getSettings($realm);
        $facet_settings->enabled = 1;
        ctools_export_crud_save('facetapi', $facet_settings);
      }
    }
  }
}
```

Running this function will mark the facets to be enabled. We also need to make sure this runs on deployments. To do that, implement `hook_environment_switch` for your module

```php
function dkan_search_customizations_environment_switch($target_env, $current_env) {
	dkan_search_customizations_enable_facets()
}
```

#### QA

* Run `ahoy drush -y env-switch --force local`
* Visit `admin/config/search/search_api/index/<searcher|index>/facets` for all the overriden indexes
* Facets should be enabled

### Add facets block to search pages

This can be accomplished with a `hook_panels_pre_render` implementation:

```php
function dkan_search_customizations_panels_pre_render(&$display, $renderer) {
  $searcher = FALSE;
  // Detect with searcher it is based on the panel display.
  if (is_numeric(strpos($display->cache_key, 'page-datasets'))) {
    $searcher = 'search_api@datasets';
  }
  if (is_numeric(strpos($display->cache_key, 'panel_context:node_view::node_view_panel_context_3'))) {
    $searcher = 'search_api@groups_di';
  }
  // If searcher is ment to be overriden retrieve blocks.
  if ($searcher) {
    $map = facetapi_get_delta_map();
    $realms = array();
    $panes = array();
    $faceted_fields = dkan_search_customizations_facet_list();
    // Cross reference fields with facet map
    foreach($map as $machine_name => $facet) {
      foreach($faceted_fields as $field) {
        // Special case for resource taxonomies
        $f = str_replace('field_resources:', '', $field);
        // If Facet matchs with field and searcher, save realm.
        if (is_numeric(strpos($facet, $f)) && is_numeric(strpos($facet, $searcher))) {
          $realms[$field] = $machine_name;
        }
      }
    }

	// Prepare panes for realms.
    foreach($realms as $field => $machine_name) {
      // Create a new pane based on the facet realm.
      $pane = panels_new_pane('block', $machine_name, TRUE);
      $pane->uuid = ctools_uuid_generate();
      $pane->pid = 'new-' . $pane->uuid;
      $pane->panel = 'sidebar';
      $pane->subtype = 'facetapi-' . $machine_name;

      unset($pane->did);
      $pane->shown = TRUE;
      
      // Customize titles.
      switch($field){
        case 'field_brand':
          $title = 'Brand';
          break;
        case 'field_country':
          $title = 'Country';
          break;
        case 'field_customer_type':
          $title = 'Customer Type';
          break;
      }
      $pane->configuration = array(
        'override_title' => TRUE,
        'override_title_heading' => 'h2',
        'override_title_text' => $title
      );
      $pane->css = array(
        'css_id' => '',
        'css_class' => 'pane-facetapi pane-block',
      );
      $pane->style = array(
        'settings' => array(
          'pane_title' => '%title',
          'pane_collapsed' => TRUE,
          'pane_empty_check' => FALSE,
        ),
        'style' => 'collapsible',
      );
      $panes[$field] = $pane;
    }

    foreach ($faceted_fields as $field) {
      if (isset($panes[$field])) {
        $pane = $panes[$field];
        // Adds facet block reference to the sidebar.
        $display->panels['sidebar'][] = $pane->pid;
        // Add pane to the pane content.
        $display->content[$pane->pid] = $pane;
      }
    }
  }  
}
```

#### QA

* Rebuild registry and clear cache
* Blocks should render in the search page

## Future enhacements

* Field and searchers options could be enhance to use a structure like this:

```php
function dkan_search_customizations_facet_list() {
  return array(
    'field_brand' => array(
    	'searchers' => array('search_api@datasets', 'search_api@groups_di'),
    	'title' => 'Brand',
    ),
    'field_country' => array(
    	'searchers' => array('search_api@datasets'),
    	'title' => 'Country',
    ),
    'field_customer_type' => array(
    	'searchers' => array('search_api@groups_di'),
    	'title' => 'Customer Type',
    ),
  );
}
```

* These options, with some work, could be wrapped up in a generic module that retrieves the fields and searchers from variables. Variables could be set from `settings.custom.php`.