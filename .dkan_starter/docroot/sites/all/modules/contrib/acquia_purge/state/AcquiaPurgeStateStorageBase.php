<?php

/**
 * @file
 * Contains AcquiaPurgeStateStorageBase.
 */

/**
 * Base layer for state storage backends.
 */
abstract class AcquiaPurgeStateStorageBase implements AcquiaPurgeStateStorageInterface {

  /**
   * Indicates if ::commit() has been registered to be called at shutdown.
   *
   * @var bool
   */
  protected $commit = FALSE;

  /**
   * Item instances.
   *
   * @var AcquiaPurgeStateItemInterface[]
   */
  protected $items = array();

  /**
   * The payload buffer which gets synchronized with memcached.
   *
   * @var mixed[]
   */
  protected $buffer = array();

  /**
   * Propagate AcquiaPurgeStateItem objects from the given buffer data.
   *
   * @param mixed $buffer
   *   Raw buffer payload to initialize state data from.
   *
   * @return int
   *   The number of items it was able to load from the buffer.
   */
  public function __construct($buffer) {
    $loaded_items = 0;
    if (is_array($buffer) && count($buffer)) {
      foreach ($buffer as $key => $value) {
        if (!is_string($key)) {
          continue;
        }
        $this->buffer[$key] = $value;
        $this->items[$key] = new AcquiaPurgeStateItem($this, $key, $value);
        $loaded_items++;
      }
    }
    return $loaded_items;
  }

  /**
   * {@inheritdoc}
   */
  public function set(AcquiaPurgeStateItemInterface $item) {
    if (!$this->commit) {
      $this->commit = TRUE;
      drupal_register_shutdown_function(array($this, 'commit'));
    }
    $this->items[$item->getKey()] = $item;
    $this->buffer[$item->getKey()] = $item->get();
  }

  /**
   * {@inheritdoc}
   */
  public function get($key, $default = NULL) {
    if (!isset($this->items[$key])) {
      $this->items[$key] = new AcquiaPurgeStateItem($this, $key, $default);
    }
    return $this->items[$key];
  }

  /**
   * {@inheritdoc}
   */
  public function getCounter($key) {
    if (isset($this->items[$key])) {
      if (!($this->items[$key] instanceof AcquiaPurgeStateCounterInterface)) {
        $this->items[$key] = new AcquiaPurgeStateCounter(
          $this,
          $key,
          $this->items[$key]->get()
        );
      }
    }
    else {
      $this->items[$key] = new AcquiaPurgeStateCounter($this, $key, 0);
    }
    return $this->items[$key];
  }

  /**
   * {@inheritdoc}
   */
  public function wipe() {
    $this->items = array();
    $this->buffer = array();
  }

}
