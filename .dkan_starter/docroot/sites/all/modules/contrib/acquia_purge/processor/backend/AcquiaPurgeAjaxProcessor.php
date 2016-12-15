<?php

/**
 * @file
 * Contains AcquiaPurgeAjaxProcessor.
 */

/**
 * Process the queue using a AJAX client-side UI.
 */
class AcquiaPurgeAjaxProcessor extends AcquiaPurgeProcessorBase implements AcquiaPurgeProcessorInterface {

  /**
   * Path blacklist from where the processor UI should stay away.
   *
   * @var string[]
   */
  protected $blacklist = array(
    'admin/config/development/performance/manualpurge/autocomplete',
    'acquia_purge_ajax_processor',
    'admin/config/system/expire',

    // Common Drupal paths where the UI is undesirable.
    'admin/reports/status',
    'admin/reports/dblog',
    'admin/dashboard/customize',
    'admin/dashboard',
    'system/ajax',
    'file/ajax',
    'file/progress',
    'toolbar/toggle',
    'batch',

    // Media module.
    'media/browser',
    'media/browser/testbed',
    'media/browser/list',
    'media/browser/library',
  );

  /**
   * Whether assets have been attached or not yet.
   *
   * @var bool
   */
  protected $initialized = FALSE;

  /**
   * Path to the script client.
   *
   * @var string
   */
  protected $scriptClient = '/processor/backend/AcquiaPurgeAjaxProcessor.js';

  /**
   * {@inheritdoc}
   */
  public static function isEnabled() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function __construct($service) {
    parent::__construct($service);
    $this->scriptClient = $this->service->modulePath . $this->scriptClient;
  }

  /**
   * {@inheritdoc}
   */
  public function getSubscribedEvents() {
    return array('onInit', 'onMenu', 'onItemsQueued');
  }

  /**
   * Implements event onInit.
   *
   * @see acquia_purge_init()
   */
  public function onInit() {

    // Do not trigger the on-screen purger if this path is blacklisted.
    if (!$this->isPathBlacklisted()) {

      // Check if this user needs the client-side processor loaded.
      if (self::isUserOwningTheQueue($this->service)) {

        // Load the static assets that will kick in the processor.
        $this->initializeClientSideProcessor();
      }
    }
  }

  /**
   * Implements event onItemsQueued.
   *
   * @see AcquiaPurgeService::addPath()
   * @see AcquiaPurgeService::addPaths()
   */
  public function onItemsQueued() {
    $this->registerUserAsQueueOwner();
  }

  /**
   * Implements event onMenu.
   *
   * @see acquia_purge_menu()
   */
  public function onMenu(&$items) {
    $items['acquia_purge_ajax_processor'] = array(
      'title' => 'Acquia Purge AJAX processor',
      'page callback' => 'AcquiaPurgeAjaxProcessor::pathCallback',
      'access callback' => 'user_is_logged_in',
      'file' => 'processor/backend/AcquiaPurgeAjaxProcessor.php',
      'type' => MENU_CALLBACK,
    );
  }

  /**
   * Determine if the current request path is blacklisted.
   *
   * On-screen notifications and interactive on-screen purge processing is not
   * always appreciated or can break the layout at places like popups or AJAX
   * callbacks. Therefore this helper attempts to prevent user annoyances
   * the best it can, but if error messages or the on-screen purge processor
   * are shown on a certain page, you know what to patch ;).
   *
   * @return bool
   *   FALSE indicates pass, TRUE indicates that the path is blacklisted.
   */
  public function isPathBlacklisted() {

    // If we're called too early, don't make any noise.
    if (!isset($_GET['q'])) {
      return FALSE;
    }

    // If this page is in our blacklist, return TRUE.
    elseif (in_array($_GET['q'], $this->blacklist)) {
      return TRUE;
    }

    // Avoid paths that contain any of these snippets.
    $snippets = array('autocomplete', 'ajax');
    foreach ($snippets as $snippet) {
      if (stristr($_GET['q'], $snippet)) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Determine if the processor should run.
   *
   * @param AcquiaPurgeService $service
   *   The Acquia Purge service object.
   *
   * @return bool
   *   Either TRUE or FALSE.
   */
  public static function isUserOwningTheQueue(AcquiaPurgeService $service) {

    // Anonymous users can never process the queue.
    if (!user_is_logged_in()) {
      return FALSE;
    }

    // Retrieve the list of user names owning an ongoing purge process.
    $uiusers = $service->state()->get('uiusers', array())->get();

    // If the uiusers list is empty, that means no active purges are ongoing.
    if (!count($uiusers)) {
      return FALSE;
    }

    // Is the current user one of the uiusers of the actively ongoing purge?
    global $user;
    if (!in_array($user->name, $uiusers)) {
      return FALSE;
    }

    // Are we running on a Acquia Cloud environment?
    if (!_acquia_purge_are_we_on_acquiacloud()) {
      return FALSE;
    }

    // All tests passed, this user can process the queue.
    return TRUE;
  }

  /**
   * Determine if the interactive UI should be visible or not.
   *
   * When the interactive UI is not presented to the end-user, processing of the
   * queue via AJAX still happens. It happens silently in the background for as
   * long as the administrative user that triggered it, has tabs to Drupal open.
   *
   * @return bool
   *   Either TRUE or FALSE.
   */
  protected function isUiVisible() {

    // Always hide the processor in case this is requested.
    if (_acquia_purge_variable('acquia_purge_silentmode') === TRUE) {
      return FALSE;
    }

    // Only users with the 'purge on-screen' permission will actually see the
    // UI, since it could be confusing for some users.
    return user_access('purge on-screen');
  }

  /**
   * Process a chunk of items form the queue and respond in JSON.
   *
   * @return string
   *   Statistics array encoded as JSON, including a 'widget' HTML snippet.
   */
  static public function pathCallback() {
    $service = _acquia_purge_service();
    $stats = $service->stats();
    $stats['error'] = FALSE;
    $stats['widget'] = '&nbsp;';

    // Deny access when the current user didn't initiate queue processing.
    if (!self::isUserOwningTheQueue($service)) {
      $stats['running'] = FALSE;
      return drupal_json_output($stats);
    }

    // Test for blocking diagnostic issues and report any if found.
    if (!_acquia_purge_are_we_allowed_to_purge()) {
      $err = current(_acquia_purge_get_diagnosis(ACQUIA_PURGE_SEVLEVEL_ERROR));
      _acquia_purge_get_diagnosis_logged($err);
      $stats['error'] = $err['description'];
      return drupal_json_output($stats);
    }

    // Attempt to process a chunk from the queue.
    if ($service->lockAcquire()) {
      $service->process();
      foreach ($service->stats() as $key => $value) {
        $stats[$key] = $value;
      }

      // When processing stalled, the history breadcrumb often stays empty and
      // this is a clear indication that errors occurred.
      if (empty($stats['purgehistory'])) {
        $stats['error'] = t("The system seems to be having difficulties
          refreshing recent content changes. Your work won't be lost, but please
          do ask your technical administrator to check the logs.");
      }

      $service->lockRelease();
    }
    else {
      $stats['locked'] = TRUE;
    }

    // Render the status widget and render as JSON response.
    if (!$stats['error']) {
      $stats['widget'] = theme('acquia_purge_status_bar_widget', $stats);
    }
    return drupal_json_output($stats);
  }

  /**
   * Register the current user as processing owner.
   */
  protected function registerUserAsQueueOwner() {

    // Prevent registration on the CLI or as anonymous user.
    if ((php_sapi_name() === 'cli') || (!user_is_logged_in())) {
      return;
    }

    // Fetch the list of queue owners as stored in state data.
    $uiusers = $this->service->state()->get('uiusers', array())->get();

    // Register the current user when its not yet registered.
    global $user;
    if (!in_array($user->name, $uiusers)) {
      $uiusers[] = $user->name;
      $this->service->state()->get('uiusers', array())->set($uiusers);
    }
  }

  /**
   * Load the static assets to load and show the client side processor.
   */
  protected function initializeClientSideProcessor() {

    if (!$this->initialized) {

      // Load the AJAX processor behavior. As soon as the bahvior is loaded it
      // starts hitting /acquia_purge_ajax_processor, which in turn will process
      // a chunk from the queue. With queue locking in place, multiple requests
      // will not do any harm.
      drupal_add_js($this->scriptClient);

      // Although the behavior always loads and works the queue, it doesn't mean
      // its always presented to the user. Print the DSM message when needed.
      if ($this->isUiVisible()) {
        $message = t("There have been changes to content, and these need to be
          refreshed throughout the system. There may be a delay before the changes
          appear to all website visitors.");
        drupal_set_message($message, 'acquia_purge_messages', FALSE);

        // Add inline CSS to hide the DSM box, this covers cases where the
        // script didn not yet or could not load (see d.o. 2014461).
        drupal_add_css('.acquia_purge_messages {display:none;}',
          array('type' => 'inline'));
      }

      $this->initialized = TRUE;
    }
  }

}
