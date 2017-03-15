<?php

/**
 * @file
 * Provides class that expires panels.
 */

class ExpirePanels implements ExpireInterface {

  /**
   * Executes expiration actions for panels.
   *
   * @param object $display
   *   Panel display.
   *
   * @param int $action
   *   Action that has been executed.
   *
   * @param bool $skip_action_check
   *   Shows whether should we check executed action or just expire node.
   */
  public function expire($display, $action, $skip_action_check = FALSE) {
    $panel_pane = $this->getPanelPage($display);

    $enabled_actions = variable_get('expire_panels_actions', array());
    $enabled_actions = array_filter($enabled_actions);

    // Stop further expiration if executed action is not selected by admin.
    if (!in_array($action, $enabled_actions) && !$skip_action_check) {
      return;
    }

    $expire_urls = array();

    // Expire front page.
    $expire_front_page = variable_get('expire_panels_front_page', EXPIRE_PANELS_FRONT_PAGE);
    if ($expire_front_page) {
      $expire_urls = ExpireAPI::getFrontPageUrls();
    }

    // Expire panel page.
    $expire_panel_page = variable_get('expire_panels_panel_page', EXPIRE_PANELS_PANEL_PAGE);
    if (!empty($panel_pane->path) && $expire_panel_page) {
      $expire_urls['panel-' . $panel_pane->path] = $this->placeholdersToWildcard($panel_pane->path);
    }

    // Expire panelizer page.
    // @todo currently supports only update.
    $expire_panelizer = variable_get('expire_panels_panelizer_page', EXPIRE_PANELS_PANELIZER_PAGE);
    if (module_exists('panelizer') && $expire_panelizer && !empty($display->context['panelizer'])) {
      $panelizer = $display->context['panelizer'];
      if (!empty($panelizer->type[2])) {
        $function = 'expire_' . $panelizer->type[2] . '_update';
        if (function_exists($function)) {
          $function($panelizer->data);
        }
      }
    }

    // Expire custom pages.
    $expire_custom = variable_get('expire_panels_custom', EXPIRE_PANELS_CUSTOM);
    if ($expire_custom) {
      $pages = variable_get('expire_panels_custom_pages');
      $urls = ExpireAPI::expireCustomPages($pages, array('panel' => $display));
      $expire_urls = array_merge($expire_urls, $urls);
    }

    // Flush page cache for expired urls.
    ExpireAPI::executeExpiration($expire_urls, 'panels', $display);
  }

  /**
   * Returns panel page for display.
   *
   * @param object $display
   *   Panel display.
   *
   * @return object|null
   *   Panel page.
   */
  public function getPanelPage($display) {
    ctools_include('export');
    $panel_handler = $this->getPanelHandler($display);
    if (empty($panel_handler)) {
      return NULL;
    }

    $pages = ctools_export_load_object('page_manager_pages', 'conditions', array('task' => $panel_handler->task, 'name' => $panel_handler->subtask));
    if (empty($pages)) {
      return NULL;
    }

    return reset($pages);
  }

  /**
   * Returns panel handler for display.
   *
   * @param object $display
   *   Panel display.
   *
   * @return object|null
   *   Panel handler.
   */
  public function getPanelHandler($display) {
    ctools_include('export');
    $handlers = ctools_export_load_object('page_manager_handlers');
    $panel_handler = NULL;

    // Find panel handler by did property.
    foreach ($handlers as $handler) {
      if (empty($panel_handler) && !empty($handler->conf['did']) && $handler->conf['did'] == $display->did) {
        $panel_handler = $handler;
      }
    }

    return $panel_handler;
  }

  /**
   * Replace placeholders with wildcard.
   */
  public function placeholdersToWildcard($url) {
    $processed_url = array();
    $parts = explode('/', $url);

    // @todo need to handle wildcards within URL.
    foreach ($parts as $part) {
      // Look for placeholder that starts with "%" or "!".
      if (preg_match('/^%|^!(|' . DRUPAL_PHP_FUNCTION_PATTERN . ')$/', $part)) {
        return implode('/', $processed_url) . '|wildcard';
      }
      $processed_url[] = $part;
    }

    return $url;
  }
}
