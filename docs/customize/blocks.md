# Override core blocks

Implement `hook_block_view_alter`

```php
/**
 * Implements hook_block_view_alter()
 */
function dkan_sitewide_usda_overrides_block_view_alter(&$data, $block) {
  if ($block->delta == 'dkan_sitewide_data_extent') {
    $data['content'] = dkan_sitewide_usda_overrides_data_extent_block();
  }
}
```