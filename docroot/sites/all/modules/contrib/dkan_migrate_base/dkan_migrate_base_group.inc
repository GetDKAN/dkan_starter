<?php

/**
 * @file
 * Migration Class for Groups
 */

/**
 * Migrate CKAN group
 */
class MigrateCkanGroupBase extends MigrateCkanBase {
  /**
   * Here we go.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $fields = $this->getCkanGroupFields();
    $list_url = $this->endpoint . 'group_list';
    $item_url = $this->endpoint . 'group_show?id=:id';
    $this->page = isset($arguments['page']) ? $arguments['page'] : '';
    $this->offset = isset($arguments['offset']) ? $arguments['offset'] : '';

    $this->source = new MigrateSourceList(new CKANListJSON(
      $list_url,
      array('page' => $this->page,
            'offset' => $this->offset,
      )
    ),
    new CKANItemJSON($item_url, $fields), $fields);

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

    $this->destination = new MigrateDestinationNode('group');
    $this->addFieldMapping('id', 'uuid');
    $this->addFieldMapping('uuid', 'id');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'description');
    $this->addFieldMapping('created', 'created');
  }

  /**
   * Creates list of fields for CKAN Dataset.
   */
  public function getCkanGroupFields() {
    return array(
      "title" => "Title",
      "created" => "Created",
      "description" => "Description",
    );
  }
}
