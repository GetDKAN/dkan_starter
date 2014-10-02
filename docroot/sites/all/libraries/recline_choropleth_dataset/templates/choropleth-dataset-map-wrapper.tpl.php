<?php
/**
 * @file
 * Template for the chororpleth map wrapper.
 *
 * Variables passed in from theme function:
 *  - $node - The fully loaded dataset node object.
 *  - $dataset_url = The url to the dataset for this map.
 *  - $embedable_url = The url to embed this map.
 *  - $choropleth = The choropleth map html.
 *  - $embed_toggle_text = The text that activates the ebmed toggle.
 *  - $embed_help_text = The text that tells the visitor what to do with the
 *    embed code.
 *  - $link_source_title = The text for the title attribute on the source link.
 *  - $privacy_policy_url = The url of the Whitehouse's privacy policy on
 *    WH.gov.
 *  - $privacy_policy_url_title_text = The title text of the privacy policy
 *    link.
 *  - $privacy_policy_url_text = The link text to the privacy policy page.
 */
?>
<?php if ($choropleth) : ?>
<div class="choropleth-map-wrapper">
  <?php print $choropleth; ?>
</div>
<div class="choropleth-map-links">
  <!-- TO-DO: Make the display of the source URL something that can be toggled. -->
  <!-- <div>source: <a href="<?php print $dataset_url; ?>" target="_blank" title="<?php print $link_source_title; ?> <?php print $dataset_url; ?>"><?php print $dataset_url; ?></a></div> -->
  <div>
    <a id="embed-toggle"><?php print $embed_toggle_text; ?></a>
    <a id="privacy-policy" href="<?php print $privacy_policy_url; ?>" target="blank" title="<?php print $privacy_policy_url_title_text; ?>"><?php print $privacy_policy_url_text; ?></a>
    <div id="embed-toggle-reveal" class="element-hidden">
      <div class="help-text"><?php print $embed_help_text; ?></div>
      <textarea id="embed-me" ><iframe src="<?php print $embedable_url; ?>" height="655" width="100%" frameborder="0" scrolling="auto" seamless></iframe></textarea>
    </div>
  </div>
</div>
<?php endif; ?>
