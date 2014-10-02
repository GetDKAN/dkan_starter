<?php
/**
 * @file
 * Template for the chororpleth map view.
 *
 * Variables passed in from theme function:
 *   - $map_view_html = The base html for the map
 */

?>
<?php if ($map_view_html) : ?>
<div class="choropleth-map-view">
  <?php print $map_view_html; ?>
</div>
<?php endif; ?>
