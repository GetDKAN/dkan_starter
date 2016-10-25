<?php
/**
 * @file
 * Template for DDH Moscone.
 * Custom Dataset layout.
 */
$node = node_load($variables['display']->args[0]);
node_build_content($node, 'full');
$rating = $node->field_rating[LANGUAGE_NONE][0]['count'];
?>

<div class="panel-display ddh-moscone clearfix <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <div class="container-fluid">
    <div class="row">
      <div class="col-md-12 radix-layouts-header panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>

    <div class="row row-middle">
      <div class="col-md-9 radix-layouts-content panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['contentmain']; ?>

          <div class="dataset-links">
            <div class="rating">
              <?php print theme('rating', array('node' => $node, 'rating' => $rating)) ?>
            </div>
            <div class="stats">
              <ul class="stats-options">
              </ul>
              <ul class="stats-social">
              </ul>
            </div>
          </div>

        </div>
      </div>
      <div class="col-md-3 radix-layouts-sidebar panel-panel">
        <div class="panel-panel-inner">
          <div class="dummy-block">
            <p class="pull-right"><a href=""><i class="sprite icon-back"></i> Back</a></p>
          </div>
          <?php print $content['top-sidebar']; ?>
        </div>
      </div>
    </div>

    <div class="row row-bottom">
      <div class="col-md-8 radix-layouts-contentbottom panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['contentbottom']; ?>
        </div>
      </div>
      <div class="col-md-4 radix-layouts-bottom-sidebar panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['bottom-sidebar']; ?>
        </div>
      </div>
    </div>
  </div>

</div><!-- /.moscone -->
