<?php

/**
 * @file
 * Migration File for DKAN Harvest.
 */


/**
 * Implements hook_migrate_api().
 */
function dkan_harvest_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'dkan_harvest' => array(
        'title' => t('dkan_harvest'),
      ),
    ),
    'migrations' => array(
      'dkan_harvest_data_json' => array(
        'class_name' => 'DataJSONHarvest',
        'group_name' => 'dkan_harvest',
        'title' => t('data.json harvest'),
      ),
    ),
  );
  return $api;
}

class DataJSONList extends MigrateList {
  protected $files;
  /**
   * Builds the list of files paths inside DKAN_HARVEST_CACHE_DIR.
   */
  public function __construct() {
    parent::__construct();
    // Build list of items based off sources definitions.
    if (!isset($this->files)) {
      $files = array();
      $sources = dkan_harvest_sources_definition();
      foreach ($sources as $key => $source) {
        if ($source['type'] == 'data.json') {
          $dir = DKAN_HARVEST_CACHE_DIR . '/' . $key;
          $dir = drupal_realpath($dir);
          foreach (glob($dir . '/*') as $filename) {
            if (!in_array(pathinfo($filename)['filename'], array('.', '..'))) {
              $files[] = $filename;
            }
          }
        }
      }
      $this->files = $files;
    }
  }
  /**
   * Implements MigrateList::getIdList().
   */
  public function getIdList() {
    $ids = array();
    $cache = cache_get('dkan_harvest_ids');
    if ($cache && isset($cache->data) && count($cache->data) == count($this->files)) {
      return array_keys($cache->data);
    }
    else {
      foreach ($this->files as $file) {
        if (file_exists($file)) {
          $data = json_decode(file_get_contents($file));
          $ids[$data->identifier] = $file;
        }
      }
      cache_set('dkan_harvest_ids', $ids, 'cache');
      return array_keys($ids);
    }
  }

  /**
   * Implements MigrateList::__toString().
   */
  public function __toString() {
    return '';
  }

  /**
   * Implements MigrateList::computeCount().
   */
  public function computeCount() {
    $count = count($this->getIdList());
    foreach ($this->files as $file) {
      if (file_exists($file)) {
        $data = json_decode(file_get_contents($file));
        $count = $count + count($data->distribution);
      }
    }
    return $count;
  }
}

class DataJSONItem extends MigrateItem {
  /**
   * Implements MigrateItem::getItem().
   */
  public function getItem($id) {

    $migration = Migration::currentMigration();

    if ($cache = cache_get('dkan_harvest_ids')) {
      $files = $cache->data;
      $file = $files[$id];

      $json = file_get_contents($file);
      // Test if file exists.
      if ($json === FALSE) {
        $message = t('Loading of !objecturl failed:', array('!objecturl' => $id));
        $migration->getMap()->saveMessage(array($id), $message, MigrationBase::MESSAGE_ERROR);
        return new stdClass();
      }
      // Test if file is valid json.
      $json = drupal_json_decode($json);
      if (is_null($json)) {
        $message = t('!objecturl does not contain a valid json string:', array('!objecturl' => $id));
        $migration->getMap()->saveMessage(array($id), $message, MigrationBase::MESSAGE_ERROR);
        return new stdClass();
      }

      // Validate that source is set properly for item.
      $key = explode('/', $file);
      end($key);
      $key = prev($key);
      $sources = dkan_harvest_sources_definition();
      if (!isset($sources[$key])) {
        $message = t('!objecturl does not belong to a valid harvest source:', array('!objecturl' => $id));
        $migration->getMap()->saveMessage(array($id), $message, MigrationBase::MESSAGE_ERROR);
        return new stdClass();
      }
      $source = $sources[$key];
      // Set key to ease item identification while processing the row.
      $json['source'] = $key;
      // Set defaults.
      if (isset($source['defaults'])) {
        foreach ($source['defaults'] as $default => $value) {
          $json[$default] = $value;
        }
      }
      // Set overrides.
      foreach ($source['overrides'] as $override => $value) {
        $json[$override] = $value;
      }
      return (object) $json;
    }
    else {
      $message = t('Loading of files for !objecturl failed:', array('!objecturl' => $id));
      $migration->getMap()->saveMessage(array($id), $message, MigrationBase::MESSAGE_ERROR);
      return new stdClass();
    }
  }
}

class DataJSONHarvest extends MigrateDataJsonDatasetBase {
  /**
   * Registers endpoints.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $fields = $this->getDataJsonDatasetFields();
    $this->source = new MigrateSourceList(
      new DataJSONList(),
      new DataJSONItem(), $fields
    );
    $this->map = new MigrateSQLMap(
        $this->machineName,
        array(
          'uuid' => array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
            'description' => 'id',
          ),
        ),
        MigrateDestinationNode::getKeySchema()
    );
    $this->destination = new MigrateDestinationNode(
      'dataset',
      array('text_format' => 'html')
    );
    $this->addDataJsonMapping();
  }

  /**
   * Implements prepareRow.
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }

 /**
  *TODO: get and set highWater mark per source.
  *
  * Note: I think the only way to do this with a single migration is to get the
  * idlist and run the migration separately like migrate.drush.inc.
  */
  public function getHighwater() {
    $highwater = db_select('migrate_status', 'ms')
              ->fields('ms', array('highwater'))
              ->condition('machine_name', $this->machineName)
              ->execute()
              ->fetchField();
    // Returns empty highwater.
    return '';
  }

  /**
   * Prepare Identifier.
   */
  public function prepareIdentifier($identifier) {
    if (filter_var($identifier, FILTER_VALIDATE_URL)) {
      $host = parse_url($identifier, PHP_URL_HOST);
      $identifier = parse_url($identifier, PHP_URL_PATH);
      $identifier = explode('/', $identifier);
      $identifier = end($identifier);
      $identifier = $host . '--' . $identifier;
    }
    return $identifier;
  }

  /**
   * Implements prepare.
   */
  public function prepare($node, $row) {
    parent::prepare($node, $row);
  }
}
