<?php

/**
 * @file
 * Template for visualization embed button.
 *
 * Variables:
 * - $embed_url: the url of the rendered content to be embedded.
 */
?>
<div class="visualization-embed">
  <a class="embed-link" href="#embed-wrapper"><?php print t('Embed'); ?></a>
  <div id="embed-wrapper" class="embed-code-wrapper">
    <textarea class="embed-code" style="height: 25px;"><iframe width="960" height="660" src="<?php print $embed_url ?>" frameborder="0"></iframe></textarea>
  </div>
</div>
