## Custom odsm endpoint output 

The following snippet is an example of a **hook\_open\_data\_schema\_map\_results\_alter** implementation that adds **vuuid** to the **data.json** api endpoint. This uses the **custom_config** module under **config/modules** but it can be implemented under any custom module.

```php
<?php
/**
 * @file
 * Code for the Custom Config feature.
 */
	
include_once 'custom_config.features.inc';
	
/**
 * Implements hook_open_data_schema_map_results_alter().
 */
function custom_config_open_data_schema_map_results_alter(&$result, $machine_name, $api_schema) {
  if ($api_schema == 'pod_v1_1' && $machine_name == 'data_json_1_1') {
    // $node_uuid = $result['identifier'];
    foreach ($result as $key => $value) {
      $node_uuid = $value['identifier'];
      $node = entity_uuid_load('node', array($node_uuid));
      $node = $node[array_keys($node)[0]];
      $result[$key]['vuuid'] = $node->vuuid;
    }
  }
}
```