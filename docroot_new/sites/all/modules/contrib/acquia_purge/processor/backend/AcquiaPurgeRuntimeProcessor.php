<?php

/**
 * @file
 * Contains AcquiaPurgeRuntimeProcessor.
 */

/**
 * Processes the queue at the end of EVERY request.
 */
class AcquiaPurgeRuntimeProcessor extends AcquiaPurgeProcessorBase implements AcquiaPurgeProcessorInterface {

  /**
   * Path to the script client.
   *
   * @var string
   */
  protected $processingOccurred = FALSE;

  /**
   * {@inheritdoc}
   */
  public static function isEnabled() {
    return (bool) _acquia_purge_variable('acquia_purge_lateruntime');
  }

  /**
   * {@inheritdoc}
   */
  public function __construct($service) {
    parent::__construct($service);
    drupal_register_shutdown_function(array($this, 'onShutdown'));
  }

  /**
   * {@inheritdoc}
   */
  public function getSubscribedEvents() {
    return array('onExit');
  }

  /**
   * Attempt to process a chunk from the queue.
   *
   * When processing already occurred earlier during this request, it can occur
   * that this call will not process anything anymore. To prevent resource
   * shortage, _acquia_purge_get_capacity() maintains global status of how much
   * items can still be processed, and can return 0 at some point.
   *
   * @param bool $log
   *   (optional) Whether diagnostic failure should be logged or not.
   */
  protected function processQueueChunk($log = TRUE) {
    if (!$this->processingOccurred) {
      parent::processQueueChunk($log);
      $this->processingOccurred = TRUE;
    }
  }

  /**
   * Implements event onExit.
   *
   * @see acquia_purge_exit()
   */
  public function onExit() {
    $this->processQueueChunk();
  }

  /**
   * Custom shutdown function from which we check if work needs to be done.
   *
   * @see acquia_purge_exit()
   */
  public function onShutdown() {
    $this->processQueueChunk();
  }

  /**
   * Destruct a AcquiaPurgeRuntimeProcessor instance.
   */
  public function __destruct() {
    $this->processQueueChunk();
  }

}
