<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
?>
<div class="<?php print $classes; ?>">
  <div class="wrapper">
    <div class="row heading">
      <div class="col-xs-12 col-sm-8">
        <h2><?php print t('Latest Data Stories');?></h2>
      </div>
      <div class="col-sm-4 hidden-xs">
        <?php if ($more): ?>
          <?php print $more; ?>
        <?php endif; ?>
      </div>
    </div>
    <div class="row latest">
      <?php if ($rows): ?>
        <div class="view-content">
          <?php print $rows; ?>
        </div>
      <?php elseif ($empty): ?>
      <div class="view-empty col-xs-12">
        <?php print $empty; ?>
      </div>
      <?php endif; ?>
    </div>
    <div class="row">
      <div class="visible-xs col-xs-12">
        <?php if ($more): ?>
          <?php print $more; ?>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div><?php /* class view */ ?>