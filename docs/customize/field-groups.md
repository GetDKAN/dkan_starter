# Field groups

## Overriding core field groups

Usual requests involve changing the **fields (children)**  in a group as well as the **position (weight)** of the group in the node form. To accomplish this implement `hook_field_group_info_alter`:

```php
/**
 * Implements hook_field_group_info_alter().
 */
function usda_content_types_field_group_info_alter(&$groups) {
  // Alter group_primary
  $primary = $groups['group_primary|node|dataset|form'];
  $primary->data['weight'] = 10;
  $primary->data['format_type'] = 'accordion-item';
  $primary->data['label'] = 'Primary Information';
  $primary->data['parent_name'] = 'group_primary_wrapper';
  $primary->data['children'] = array(
    'body',
    'field_data_dictionary',
    'field_doi',
    'field_specific_product_type',
    'field_odfe_landing_page',
    'field_theme',
    'field_author_is_organization',
    'field_nal_author',
    'field_source_id',
    'title',
  );
  $groups['group_primary|node|dataset|form'] = $primary;
  // Alter weight of group_odfie_pod.
  $odfe = $groups['group_odfie_pod|node|dataset|form'];
  $odfe->data['weight'] = 4;
  $groups['group_odfie_pod|node|dataset|form'] = $odfe;
  // Remove group_additional
  unset($groups['group_additional|node|dataset|form']);
  unset($groups['group_odfie_pod|node|dataset|form']);
}
```

## Adding custom field_groups

Use features to export custom field_groups. If you need to include core fields in your field groups do remove it first for the core field group using the resource above.
