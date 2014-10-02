<?php
/**
 * @file
 * Template for the chororpleth map wrapper.
 *
 * Variables passed in from theme function:
 *  - $node - The fully loaded dataset node object.
 *  - $dataset_url - The url for the dataset.
 *  - $choroplethable - bool indicating if the dataset has choroplethable
 *    resources.
 *  - $choropleth_none = Message for if there is no choropleth.
 *  - $back_button_text = Text for the back button.
 *  - $choropleth = the base html for the choropleth map.
 */

?>
<?php if (!empty($dataset_url)) : ?>
  <div><a href="<?php print $dataset_url ?>" class="btn btn-primary"role="button" ><?php print $back_button_text ?></a></div><br />
<?php endif; ?>
<div class="choropleth-shell">
  <?php if ($choroplethable  && !empty($choropleth)) : ?>
    <?php print $choropleth; ?>
  <?php else : ?>
    <?php print $choropleth_none; ?>
  <?php endif; ?>
</div>
