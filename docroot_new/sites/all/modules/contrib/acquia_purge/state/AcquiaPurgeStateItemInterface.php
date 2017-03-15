<?php

/**
 * @file
 * Contains AcquiaPurgeStateItemInterface.
 */

/**
 * Describes a single state item kept in state storage.
 */
interface AcquiaPurgeStateItemInterface {

  /**
   * Construct a state item object.
   *
   * @param AcquiaPurgeStateStorageInterface $storage
   *   The state storage in which the item has been stored.
   * @param int $key
   *   The key with which the object is stored in state storage.
   * @param mixed $value
   *   The value of the state item.
   */
  public function __construct(AcquiaPurgeStateStorageInterface $storage, $key, $value);

  /**
   * Get the item value.
   *
   * @return mixed
   *   The value of the item.
   */
  public function get();

  /**
   * Get the item key.
   *
   * @return string
   *   The key of the item.
   */
  public function getKey();

  /**
   * Store the state item in state item storage.
   *
   * @param mixed $value
   *   The new value.
   */
  public function set($value);

}
