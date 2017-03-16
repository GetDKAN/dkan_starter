<?php
/**
 * @file
 * sop_front_page.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function sop_front_page_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: page_manager_pages
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-076529b4-d32c-43bc-8a72-cad6fafdc00b"] = (object) array(
      'pid' => 'new-076529b4-d32c-43bc-8a72-cad6fafdc00b',
      'panel' => 'middle',
      'type' => 'block',
      'subtype' => 'sop_front_page-page_submenu',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(),
      'extras' => array(),
      'position' => 0,
      'locks' => array(),
      'uuid' => '076529b4-d32c-43bc-8a72-cad6fafdc00b',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-11969a4b-795a-40df-8d66-91f6daec6c20|panel"] = 'top-first';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-11969a4b-795a-40df-8d66-91f6daec6c20|position"] = 0;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-5dd2eb01-4ffb-44fe-8c72-07d167cf363c"]["DELETED"] = TRUE;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-72bd5bc0-8534-40f7-b38f-2a572a71f0d6"] = (object) array(
      'pid' => 'new-72bd5bc0-8534-40f7-b38f-2a572a71f0d6',
      'panel' => 'middle',
      'type' => 'block',
      'subtype' => 'sop_front_page-page_applications',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(),
      'extras' => array(),
      'position' => 5,
      'locks' => array(),
      'uuid' => '72bd5bc0-8534-40f7-b38f-2a572a71f0d6',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-7431791a-82d2-4b01-a7eb-80375c1302ec"] = (object) array(
      'pid' => 'new-7431791a-82d2-4b01-a7eb-80375c1302ec',
      'panel' => 'top-second',
      'type' => 'block',
      'subtype' => 'sop_front_page-deposit_button',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(),
      'extras' => array(),
      'position' => 0,
      'locks' => array(),
      'uuid' => '7431791a-82d2-4b01-a7eb-80375c1302ec',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-80c8cb64-cc23-4313-8b70-d0aeb2cda035"] = (object) array(
      'pid' => 'new-80c8cb64-cc23-4313-8b70-d0aeb2cda035',
      'panel' => 'middle',
      'type' => 'block',
      'subtype' => 'sop_front_page-page_region_countries',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(),
      'extras' => array(),
      'position' => 2,
      'locks' => array(),
      'uuid' => '80c8cb64-cc23-4313-8b70-d0aeb2cda035',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-8aa431e2-a9e8-4d73-b043-aa57ecbf0947"]["DELETED"] = TRUE;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-949696cf-520f-40a4-af80-725bca8b2f13"]["DELETED"] = TRUE;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-c5b2d05e-dcee-48c4-9d23-a1fcf3f81795"] = (object) array(
      'pid' => 'new-c5b2d05e-dcee-48c4-9d23-a1fcf3f81795',
      'panel' => 'middle',
      'type' => 'views',
      'subtype' => 'dkan_data_stories',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'override_pager_settings' => 0,
        'use_pager' => 0,
        'nodes_per_page' => 4,
        'pager_id' => 0,
        'offset' => 0,
        'more_link' => 0,
        'feed_icons' => 0,
        'panel_args' => 0,
        'link_to_view' => 0,
        'args' => '',
        'url' => '',
        'display' => 'default',
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(
        'css_id' => 'featured-data',
        'css_class' => '',
      ),
      'extras' => array(),
      'position' => 4,
      'locks' => array(),
      'uuid' => 'c5b2d05e-dcee-48c4-9d23-a1fcf3f81795',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-d3655ff2-8c3c-4a0b-b492-8aa98ea1b7f3|panel"] = 'top-full';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-e8ddec0d-d283-4485-8093-3c862fac7787"] = (object) array(
      'pid' => 'new-e8ddec0d-d283-4485-8093-3c862fac7787',
      'panel' => 'middle',
      'type' => 'block',
      'subtype' => 'sop_front_page-page_data_types',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(),
      'extras' => array(),
      'position' => 1,
      'locks' => array(),
      'uuid' => 'e8ddec0d-d283-4485-8093-3c862fac7787',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-e9a59ddf-15b0-4a29-8ef6-ef460ba539e0"]["DELETED"] = TRUE;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|content|new-eecb967c-a8f2-436a-9769-40617c8366c3"] = (object) array(
      'pid' => 'new-eecb967c-a8f2-436a-9769-40617c8366c3',
      'panel' => 'middle',
      'type' => 'views_panes',
      'subtype' => 'dkan_topics_featured-panel_pane_1',
      'shown' => TRUE,
      'access' => array(),
      'configuration' => array(
        'items_per_page' => 12,
        'override_title' => 0,
        'override_title_text' => '',
        'override_title_heading' => 'h2',
      ),
      'cache' => array(),
      'style' => array(
        'settings' => NULL,
      ),
      'css' => array(
        'css_id' => 'featured-topics',
        'css_class' => '',
      ),
      'extras' => array(),
      'position' => 3,
      'locks' => array(),
      'uuid' => 'eecb967c-a8f2-436a-9769-40617c8366c3',
    );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|layout"] = 'ddh_full_width';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panel_settings|style_settings|top-full"] = NULL;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|middle|0"] = 'new-076529b4-d32c-43bc-8a72-cad6fafdc00b';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|middle|1"] = 'new-e8ddec0d-d283-4485-8093-3c862fac7787';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|middle|2"] = 'new-80c8cb64-cc23-4313-8b70-d0aeb2cda035';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|middle|3"] = 'new-eecb967c-a8f2-436a-9769-40617c8366c3';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|middle|4"] = 'new-c5b2d05e-dcee-48c4-9d23-a1fcf3f81795';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|middle|5"] = 'new-72bd5bc0-8534-40f7-b38f-2a572a71f0d6';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|top-first|0"] = 'new-11969a4b-795a-40df-8d66-91f6daec6c20';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|top-full"] = array(
    0 => 'new-d3655ff2-8c3c-4a0b-b492-8aa98ea1b7f3',
  );
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|top-second|0"] = 'new-7431791a-82d2-4b01-a7eb-80375c1302ec';
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|top-second|1"]["DELETED"] = TRUE;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|panels|top-second|2"]["DELETED"] = TRUE;
  $overrides["page_manager_pages.front_page.default_handlers|page_front_page__panel_context_5036fa8b-91e0-4e46-8ab7-ca69eca273e1|conf|display|title_pane"] = 0;

 return $overrides;
}
