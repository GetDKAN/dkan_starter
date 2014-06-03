<?php
/**
 * @file
 * nudata_story.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function nudata_story_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_stories:stories
  $menu_links['main-menu_stories:stories'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'stories',
    'router_path' => 'stories',
    'link_title' => 'Stories',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_stories:stories',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Stories');


  return $menu_links;
}
