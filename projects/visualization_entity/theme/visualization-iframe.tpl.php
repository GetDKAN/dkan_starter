<?php
/**
 * @file
 * Template for the choropleth visualization wrapper.
 */

?>

<style type="text/css">
    #iframe-shell .content {
    	display: none;
    }
</style>

<div id="iframe-shell">
    <?php print render($page['content']); ?>
</div>
