<?php
/**
 * @file
 * Template for DDH Dataset node view.
 * Custom Dataset layout.
 */
// Rating.

$node = menu_get_object();
$votes = fivestar_get_votes('node', $node->nid, 'vote');
$rating = $votes['count']['value'] ? $votes['count']['value']:0;

// Map View.
$map_link = '';
$term = array_values(taxonomy_get_term_by_name('geospatial'))[0];
if($node->field_wbddh_data_type[LANGUAGE_NONE][0]['tid'] == $term->tid) {
  $map_link = '<li class="divider"> | </li><li><span class="small-icon sprite ddh-icon-mapview"></span> <a href="http://w1es1111.worldbank.org:10095/management/mcmap/map.htm?code=&level=&indicatorcode=0553&title=Global&org=ibrd&layerSearch=' . $node->nid . '">Map View</a></li>';
}
?>

<div class="panel-display ddh-bryant clearfix <?php if (!empty($classes)) { print $classes; } ?><?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="container-fluid">
    <div class="row">
      <div class="col-md-9 ddh-dataset-content panel-panel">
        <div class="panel-panel-inner">
          <div class="back-button">
            <i class="fa fa-chevron-left circle" aria-hidden="true"></i><input type="button" value="Back" onclick="window.history.back()" />
          </div>
          <?php print $content['contentmain']; ?>

          <ul class="nav nav-tabs">
            <li class="active"><a data-toggle="tab" href="#tab1">Overview</a></li>
            <li><a data-toggle="tab" href="#tab3">Additional Information</a></li>
            <li><a data-toggle="tab" href="#tab2">Resources</a></li>
          </ul>

          <div class="tab-content">
            <div id="tab1" class="tab-pane fade in active">
              <?php print $content['overview']; ?>
            </div>
            <div id="tab3" class="tab-pane fade">
              <?php print $content['additional']; ?>
            </div>
            <div id="tab2" class="tab-pane fade">
              <?php print $content['resources']; ?>
            </div>
          </div>

          <div class="dataset-links">
            <div class="rating">
              <?php print theme('rating', array('node' => $node, 'rating' => $rating)) ?>
            </div>
            <ul class="stats">
              <li><span class="small-icon sprite ddh-icon-download"></span> <span class="blue">0</span></li>
              <li class="divider"> | </li>
              <li><span class="small-icon sprite ddh-icon-api"></span> <a href="/api/3/action/package_show?id=<?php print $node->uuid ?>">API</a></li>
              <?php print $map_link ?>
            </ul>
          </div>

          <?php if ($node): ?>
            <?php $node_view = node_view($node); $node_view['comments'] = comment_node_page_additions($node); print drupal_render($node_view); ?>
          <?php endif; ?>

        </div>
      </div>
      <div class="col-md-3 ddh-dataset-sidebar panel-panel">
        <div class="panel-panel-inner">
          <?php print $content['sidebar']; ?>
        </div>
      </div>
    </div>
  </div>

</div>
