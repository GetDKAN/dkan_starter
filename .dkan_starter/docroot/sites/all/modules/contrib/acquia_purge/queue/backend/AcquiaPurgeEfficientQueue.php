<?php

/**
 * @file
 * Contains EfficientQueue.
 */

/**
 * Efficient query bundling database queue.
 *
 * Enriches SystemQueue with methods defined in AcquiaPurgeQueueInterface which
 * attempt to reduce database communication as much as possible. By bundling
 * items into single queries, total queries and roundtrips reduce drastically!
 */
class AcquiaPurgeEfficientQueue extends SystemQueue implements AcquiaPurgeQueueInterface {

  /**
   * The state storage which holds the counter state items.
   *
   * @var AcquiaPurgeStateStorageInterface
   */
  protected $state;

  /**
   * Construct a AcquiaPurgeEfficientQueue instance.
   *
   * @param AcquiaPurgeStateStorageInterface $state
   *   The state storage required for the queue counters.
   */
  public function __construct(AcquiaPurgeStateStorageInterface $state) {
    $this->state = $state;
    parent::__construct('acquia_purge');
  }

  /**
   * {@inheritdoc}
   */
  public function bad() {
    return $this->state->getCounter('qbad');
  }

  /**
   * {@inheritdoc}
   */
  public function good() {
    return $this->state->getCounter('qgood');
  }

  /**
   * {@inheritdoc}
   */
  public function total() {
    return $this->state->getCounter('qtotal');
  }

  /**
   * {@inheritdoc}
   *
   * SystemQueue::claimItem() doesn't included expired items in its query
   * which means that it essentially breaks its own interface promise. Therefore
   * we overload the implementation with one that does do this accurately. This
   * should however flow back to core, which I'm doing as part of my D8 work.
   */
  public function claimItem($lease_time = 30) {

    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    while (TRUE) {
      $conditions = array(':name' => $this->name, ':now' => time());
      $item = db_query_range('SELECT * FROM {queue} q
        WHERE name = :name AND ((expire = 0) OR (:now > expire))
        ORDER BY created, item_id
        ASC', 0, 1, $conditions)->fetchObject();
      if ($item) {
        $item->item_id = (int) $item->item_id;
        $item->expire = (int) $item->expire;
        $item->created = (int) $item->created;

        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $update = db_update('queue')
          ->fields(array(
            'expire' => time() + $lease_time,
          ))
          ->condition('item_id', $item->item_id);

        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          $item->data = unserialize($item->data);
          return $item;
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function claimItemMultiple($claims = 10, $lease_time = 30) {
    $returned_items = $item_ids = array();

    // Retrieve all items in one query.
    $conditions = array(':name' => $this->name, ':now' => time());
    $items = db_query_range('SELECT * FROM {queue} q
      WHERE name = :name AND ((expire = 0) OR (:now > expire))
      ORDER BY created, item_id
      ASC', 0, $claims, $conditions);

    // Iterate all returned items and unpack them.
    while ($item = $items->fetchObject()) {
      $item_ids[] = $item->item_id;
      $item->item_id = (int) $item->item_id;
      $item->expire = (int) $item->expire;
      $item->created = (int) $item->created;
      $item->data = unserialize($item->data);
      $returned_items[] = $item;
    }

    // Update the items (marking them claimed) in one query.
    if (count($returned_items)) {
      db_update('queue')
        ->fields(array(
          'expire' => time() + $lease_time,
        ))
        ->condition('item_id', $item_ids, 'IN')
        ->execute();
    }

    // Return the generated items, whether its empty or not.
    return $returned_items;
  }

  /**
   * {@inheritdoc}
   */
  public function createItem($data) {
    if (parent::createItem($data)) {
      $this->total()->increase();
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function createItemMultiple(array $items) {
    $records = array();

    // Build a array with all exactly records as they should turn into rows.
    $time = time();
    foreach ($items as $data) {
      $records[] = array(
        'name' => $this->name,
        'data' => serialize($data),
        'created' => $time,
      );
    }

    // Insert all of them using just one multi-row query.
    $query = db_insert('queue')->fields(array('name', 'data', 'created'));
    foreach ($records as $record) {
      $query->values($record);
    }

    // Execute the query and finish the call.
    if ($query->execute()) {
      $this->total()->increase(count($records));
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function deleteItemMultiple(array $items) {
    if (empty($items)) {
      return;
    }
    $item_ids = array();
    foreach ($items as $item) {
      $item_ids[] = $item->item_id;
    }
    db_delete('queue')
      ->condition('item_id', $item_ids, 'IN')
      ->execute();
    $this->good()->increase(count($item_ids));
  }

  /**
   * {@inheritdoc}
   */
  public function deleteQueue() {
    parent::deleteQueue();
    $this->total()->set(0);
    $this->bad()->set(0);
    $this->good()->set(0);
  }

  /**
   * {@inheritdoc}
   */
  public function numberOfItems() {
    return (int) parent::numberOfItems();
  }

  /**
   * {@inheritdoc}
   */
  public function releaseItemMultiple(array $items) {
    if (empty($items)) {
      return array();
    }
    $item_ids = array();
    foreach ($items as $item) {
      $item_ids[] = $item->item_id;
    }
    $update = db_update('queue')
      ->fields(array('expire' => 0))
      ->condition('item_id', $item_ids, 'IN')
      ->execute();
    if ($update) {
      $this->bad()->increase(count($item_ids));
      return array();
    }
    else {
      return $items;
    }
  }

}
