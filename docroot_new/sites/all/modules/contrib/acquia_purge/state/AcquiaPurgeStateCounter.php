<?php

/**
 * @file
 * Contains AcquiaPurgeStateCounter.
 */

/**
 * Provides a single counter kept in state storage.
 */
class AcquiaPurgeStateCounter extends AcquiaPurgeStateItem implements AcquiaPurgeStateCounterInterface {

  /**
   * {@inheritdoc}
   */
  public function __construct(AcquiaPurgeStateStorageInterface $storage, $key, $value) {
    parent::__construct($storage, $key, $value);
    if (!is_int($this->value)) {
      $this->value = (int) $this->value;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function decrease($amount = 1) {
    if (!is_int($amount)) {
      $amount = (int) $amount;
    }
    $this->set($this->value - $amount);
  }

  /**
   * {@inheritdoc}
   */
  public function increase($amount = 1) {
    if (!is_int($amount)) {
      $amount = (int) $amount;
    }
    $this->set($this->value + $amount);
  }

  /**
   * {@inheritdoc}
   */
  public function set($value) {
    if (!is_int($value)) {
      $value = (int) $value;
    }
    parent::set($value);
  }

}
