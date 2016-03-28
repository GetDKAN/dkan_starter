<?php

/**
 * @file
 * Contains AcquiaPurgeStateItem.
 */

/**
 * Provides a single state item kept in state storage.
 */
class AcquiaPurgeStateItem implements AcquiaPurgeStateItemInterface {

  /**
   * The state storage in which the item has been stored.
   *
   * @var AcquiaPurgeStateStorageInterface
   */
  protected $storage;

  /**
   * The key with which the object is stored in state storage.
   *
   * @var string
   */
  protected $key;

  /**
   * The value of the state item.
   *
   * @var mixed
   */
  protected $value;

  /**
   * {@inheritdoc}
   */
  public function __construct(AcquiaPurgeStateStorageInterface $storage, $key, $value) {
    $this->storage = $storage;
    $this->value = $value;
    $this->key = $key;
  }

  /**
   * {@inheritdoc}
   */
  public function get() {
    return $this->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getKey() {
    return $this->key;
  }

  /**
   * {@inheritdoc}
   */
  public function set($value) {
    $this->value = $value;
    $this->storage->set($this);
  }

}
