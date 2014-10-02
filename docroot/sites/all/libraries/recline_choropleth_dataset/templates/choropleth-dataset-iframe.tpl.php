<?php
/**
 * @file
 * Template for the chororpleth map wrapper.
 *
 * Variables passed in from theme function:
 *  - $node - The fully loaded dataset node object.
 *  - $choroplethable - bool indicating if the dataset has choroplethable
 *    resources.
 *  - $choropleth_none - Text to display in case there is no choroplethable map.
 *  - $choropleth - The html that makes up the map.
 */

?>
<?php if ($choropleth) : ?>
<div class="choropleth-shell iframe">
  <?php if ($choroplethable  && !empty($choropleth)) : ?>
    <?php print $choropleth; ?>
  <?php else : ?>
    <?php print $choropleth_none; ?>
  <?php endif; ?>
</div>
<?php endif; ?>
