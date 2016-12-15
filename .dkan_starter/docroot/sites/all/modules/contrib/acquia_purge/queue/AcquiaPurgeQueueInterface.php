<?php

/**
 * @file
 * Contains AcquiaPurgeQueueInterface.
 */

/**
 * DrupalReliableQueueInterface derivative supporting efficient storage models.
 *
 * @see DrupalReliableQueueInterface()
 * @see http://github.com/nielsvm/purge/blob/8.x-3.x/src/Queue/PluginInterface.php
 */
interface AcquiaPurgeQueueInterface extends DrupalReliableQueueInterface {

  /**
   * Construct a queue object.
   *
   * @param AcquiaPurgeStateStorageInterface $state
   *   The state storage required for the queue counters.
   */
  public function __construct(AcquiaPurgeStateStorageInterface $state);

  /**
   * Retrieve the failed purges counter.
   *
   * @return AcquiaPurgeStateCounterInterface
   *   The counter.
   */
  public function bad();

  /**
   * Retrieve the successful purges counter.
   *
   * @return AcquiaPurgeStateCounterInterface
   *   The counter.
   */
  public function good();

  /**
   * Retrieve the total queue items counter.
   *
   * @return AcquiaPurgeStateCounterInterface
   *   The counter.
   */
  public function total();

  /**
   * Add multiple items to the queue and store them efficiently.
   *
   * @param array $items
   *   Non-associative array containing arrays with arbitrary data to be
   *   associated with the new tasks in the queue.
   *
   * @return true|false
   *   TRUE if all items got created successfully, or FALSE if just one of them
   *   failed being created.
   */
  public function createItemMultiple(array $items);

  /**
   * Claims multiple items from the queue for processing.
   *
   * @param int $claims
   *   Determines how many claims at once should be claimed from the queue. When
   *   the queue is unable to return as many items as requested it will return
   *   as much items as it can.
   * @param int $lease_time
   *   How long the processing is expected to take in seconds, defaults to an
   *   hour. After this lease expires, the item will be reset and another
   *   consumer can claim the item. For idempotent tasks (which can be run
   *   multiple times without side effects), shorter lease times would result
   *   in lower latency in case a consumer fails. For tasks that should not be
   *   run more than once (non-idempotent), a larger lease time will make it
   *   more rare for a given task to run multiple times in cases of failure,
   *   at the cost of higher latency.
   *
   * @return array
   *   On success we return a non-associative array with item objects. When the
   *   queue has no items that can be claimed, this doesn't return FALSE as
   *   claimItem() does, but an empty array instead.
   *
   *   If claims return, the objects have at least these properties:
   *   - data: the same as what what passed into createItem().
   *   - item_id: the unique ID returned from createItem().
   *   - created: timestamp when the item was put into the queue.
   */
  public function claimItemMultiple($claims = 10, $lease_time = 3600);

  /**
   * Delete multiple items from the queue at once.
   *
   * @param array $items
   *   Non-associative array with item objects as returned by
   *   claimItemMultiple() or DrupalQueueInterface::claimItem().
   */
  public function deleteItemMultiple(array $items);

  /**
   * Release multiple items that the worker could not process.
   *
   * Another worker can come in and process these before the timeout expires.
   *
   * @param array $items
   *   Non-associative array with item objects as returned by
   *   claimItemMultiple() or DrupalQueueInterface::claimItem().
   *
   * @return array
   *   Empty array upon full success, else the remaining items that failed.
   */
  public function releaseItemMultiple(array $items);

}
