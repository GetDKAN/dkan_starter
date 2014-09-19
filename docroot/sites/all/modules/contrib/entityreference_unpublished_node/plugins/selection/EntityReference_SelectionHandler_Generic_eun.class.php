<?php

/**
 * A generic Entity handler.
 *
 * The generic base implementation has a variety of overrides to workaround
 * core's largely deficient entity handling.
 */
class EntityReference_SelectionHandler_Generic_eun extends EntityReference_SelectionHandler_Generic {

  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $target_entity_type = $field['settings']['target_type'];

    // Check if the entity type does exist and has a base table.
    $entity_info = entity_get_info($target_entity_type);
    if (empty($entity_info['base table'])) {
      return EntityReference_SelectionHandler_Broken::getInstance($field, $instance);
    }

    if (class_exists($class_name = 'EntityReference_SelectionHandler_Generic_eun_' . $target_entity_type)) {
      return new $class_name($field, $instance, $entity_type, $entity);
    }
    
    if (class_exists($class_name = 'EntityReference_SelectionHandler_Generic_' . $target_entity_type)) {
      return new $class_name($field, $instance, $entity_type, $entity);
    }
    else {
      return new EntityReference_SelectionHandler_Generic($field, $instance, $entity_type, $entity);
    }
  }
}

/**
 * Override for the Node type.
 *
 * This only exists to workaround core bugs.
 */
class EntityReference_SelectionHandler_Generic_eun_node extends EntityReference_SelectionHandler_Generic_node {
  public function entityFieldQueryAlter(SelectQueryInterface $query) {
    // Adding the 'node_access' tag is sadly insufficient for nodes: core
    // requires us to also know about the concept of 'published' and
    // 'unpublished'. We need to do that as long as there are no access control
    // modules in use on the site. As long as one access control module is there,
    // it is supposed to handle this check.
    if (!user_access('bypass node access') && !count(module_implements('node_grants')) && !user_access('reference unpublished nodes')) {
      $base_table = $this->ensureBaseTable($query);
      $query->condition("$base_table.status", NODE_PUBLISHED);
    }
  }
}
