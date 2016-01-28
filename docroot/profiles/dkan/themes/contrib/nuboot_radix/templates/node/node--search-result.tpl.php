<?php
$groups = og_get_entity_groups('node', $node);
$group_list = null;
$wrapper = entity_metadata_wrapper('node', $node);

if(!empty($groups['node'])) {
    $groups = array_map(function($gid){
        $g = node_load($gid);
        return $g->title;
    }, array_values($groups['node']));
    $group_list = implode(',', $groups);
}
?>
<article class="node-search-result row" xmlns="http://www.w3.org/1999/html">
   <div class="col-md-2 col-lg-1 col-xs-2 icon-container">
        <?php
        $icon = array('type'=> $type, 'class' => array('search-result-icon'));
        print nuboot_radix_icon($icon);
        ?>
   </div>
   <div class="col-md-10 col-lg-11 col-xs-10 search-result-description">
       <h2 class="node-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
       <div class="group-membership"><?php print $group_list ?></div>
       <?php if(!empty($wrapper->body->value())): ?>
           <div class="node-description"><?php print text_summary($wrapper->body->value->value(), 'plain_text', 250) ?></div>
       <?php endif; ?>
       <?php print render($content['resources']); ?>
   </div>
</article>