<?php
/**
 * @file
 * Template for the node__dataset__chororpleth.
 *
 * Variables passed from theme function:
 *   - NONE
 */

$loading_text = t('Loading');
?>
<div id="choropleth-dataset" class="recline-data-explorer">
  <div class="data-view-sidebar"></div>
	<div class="data-view-container">
    <div class="loader"><?php print $loading_text ?>...</div>
  </div>
</div>
