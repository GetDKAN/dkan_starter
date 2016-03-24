<?php

/**
 * @file
 * Contains AcquiaPurgeStateStorageInterface.
 */

/**
 * Describes a state storage object that maintains AcquiaPurgeStateItem objects.
 */
interface AcquiaPurgeStateStorageInterface {

  /**
   * Commit the state data to its persistent storage location.
   */
  public function commit();

  /**
   * Retrieve the object named $key.
   *
   * @param int $key
   *   The key with which the object is stored in state storage.
   * @param mixed|null $default
   *   (optional) The default value to use if the entry doesn't yet exist.
   *
   * @return AcquiaPurgeStateItemInterface
   *   The item.
   */
  public function get($key, $default = NULL);

  /**
   * Retrieve a counter object named $key.
   *
   * @param int $key
   *   The key with which the object is stored in state storage.
   *
   * @return AcquiaPurgeStateCounterInterface
   *   The counter.
   */
  public function getCounter($key);

  /**
   * Store the state item in state item storage.
   *
   * @param AcquiaPurgeStateItemInterface $item
   *   The AcquiaPurgeStateItemInterface object to store.
   */
  public function set(AcquiaPurgeStateItemInterface $item);

  /**
   * Wipe all state data.
   */
  public function wipe();

}
