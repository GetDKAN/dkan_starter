<?php

/**
 * @file
 * Contains AcquiaPurgeMemcachedStateStorage.
 */

/**
 * Memcached backed state storage.
 */
class AcquiaPurgeMemcachedStateStorage extends AcquiaPurgeStateStorageBase {

  /**
   * Memcached key used to store our state data in.
   *
   * @var string
   */
  protected $key;

  /**
   * Memcached bin used to store our state data in memcached.
   *
   * @var string
   */
  protected $bin;

  /**
   * Construct AcquiaPurgeMemcachedStateStorage.
   *
   * @param string $key
   *   Memcached key used to store our state data in.
   * @param string $bin
   *   Memcached bin used to store our state data in memcached.
   */
  public function __construct($key, $bin) {
    $this->key = $key;
    $this->bin = $bin;
    parent::__construct(dmemcache_get($this->key, $this->bin));
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
    dmemcache_set($this->key, $this->buffer, 0, $this->bin);
  }

  /**
   * {@inheritdoc}
   */
  public function wipe() {
    parent::wipe();
    dmemcache_delete($this->key, $this->bin);
  }

}
