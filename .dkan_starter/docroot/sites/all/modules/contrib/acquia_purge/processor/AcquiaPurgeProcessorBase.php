<?php

/**
 * @file
 * Contains AcquiaPurgeProcessorBase.
 */

/**
 * Base class for processors that process items from the queue.
 */
abstract class AcquiaPurgeProcessorBase implements AcquiaPurgeProcessorInterface {

  /**
   * The Acquia Purge service object.
   *
   * @var AcquiaPurgeService
   */
  protected $service;

  /**
   * Construct a new AcquiaPurgeProcessorBase instance.
   *
   * @param AcquiaPurgeService $service
   *   The Acquia Purge service object.
   */
  public function __construct(AcquiaPurgeService $service) {
    $this->service = $service;
  }

  /**
   * Attempt to process a chunk from the queue.
   *
   * @param bool $log
   *   (optional) Whether diagnostic failure should be logged or not.
   */
  protected function processQueueChunk($log = TRUE) {

    // Test if the diagnostic tests prohibit purging the queue.
    if (!_acquia_purge_are_we_allowed_to_purge()) {
      if ($log) {
        $err = _acquia_purge_get_diagnosis(ACQUIA_PURGE_SEVLEVEL_ERROR);
        _acquia_purge_get_diagnosis_logged($err);
      }
      return;
    }

    // Acquire a lock and process a chunk from the queue.
    if ($this->service->lockAcquire()) {
      $this->service->process();
      $this->service->lockRelease();
    }
  }

}
