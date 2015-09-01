<?php

/**
 * @file
 * Contains AcquiaPurgeDiskStateStorage.
 */

/**
 * File backed state storage.
 */
class AcquiaPurgeDiskStateStorage extends AcquiaPurgeStateStorageBase {

  /**
   * The raw payload to compare changes against.
   *
   * @var string
   */
  protected $raw = '';

  /**
   * The URI identifier to the file (on disk) to store state data in.
   *
   * @var string
   */
  protected $uri;

  /**
   * Construct AcquiaPurgeDiskStateStorage.
   *
   * @param string $uri
   *   The URI identifier to the file (on disk) to store state data in.
   */
  public function __construct($uri) {
    $this->uri = $uri;
    if (file_exists($this->uri)) {
      if ($buffer = file_get_contents($this->uri)) {
        if (parent::__construct(unserialize($buffer))) {
          $this->raw = $buffer;
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function commit() {
    if (!$this->commit) {
      return;
    }
    else {
      $this->commit = FALSE;
    }
    $raw_new = serialize($this->buffer);
    if ($raw_new !== $this->raw) {
      $this->raw = $raw_new;
      file_put_contents($this->uri, $this->raw);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function wipe() {
    parent::wipe();
    $this->raw = '';
    if (file_exists($this->uri)) {
      drupal_unlink($this->uri);
    }
  }

}
