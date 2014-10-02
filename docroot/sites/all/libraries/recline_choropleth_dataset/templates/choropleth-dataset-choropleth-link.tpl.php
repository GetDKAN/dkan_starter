<?php
/**
 * @file
 * Template for the link to the map view of the choropleth.
 *
 * Passed from theme function.
 *  - $choropleth_url - The url to the choropleth view of the dataset.
 *  - $icon_url - The url for the map icon.
 *  - $description = Description of what these links will take you to.
 *  - $label = Label text for the heading on the link box.
 *  - $button_text = Text message for the button.
 *  - $anchor_title = Title attribute text of the anchor link.
 */
?>
<?php if (!empty($choropleth_url)) : ?>
<div class="resource-choropleth clearfix">
  <label><?php print $label?></label>
  <?php if (!empty($icon_url)) : ?>
    <div class='icon'>
      <a href="<?php print $choropleth_url ?>" title="<?php print $anchor_title ?>">
        <img src="/<?php print $icon_url?>" alt="map icon"/>
      </a>
    </div>
  <?php endif;?>
  <div class="">
    <p class=""><?php print $description ?></p>
    <a href="<?php print $choropleth_url ?>" class="btn btn-primary"><?php print $button_text ?></a>
  </div>
</div>
<?php endif; ?>
