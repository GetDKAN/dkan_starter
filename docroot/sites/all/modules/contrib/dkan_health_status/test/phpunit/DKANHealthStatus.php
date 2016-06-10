<?php
/**
 * @file
 * Unit tests for dkan_health_status functions.
 */

module_load_include('module', 'dkan_health_status');

/**
 * Base dkan_health_status unit test class.
 */
class DkanHealthStatusBaseTest extends PHPUnit_Framework_TestCase {

  /**
   * Verify that access check works correctly.
   *
   * @covers dkan_health_status_api_access_check().
   */
  public function testDkanHealthStatusAccess() {
    $this->assertEquals(dkan_health_status_api_access_check('keypassed', 'keyexpected', FALSE, FALSE), FALSE);
    $this->assertEquals(dkan_health_status_api_access_check('keyexpected', 'keyexpected', FALSE, FALSE), TRUE);
    $this->assertEquals(dkan_health_status_api_access_check('keyexpected', 'keyexpected', TRUE, TRUE), TRUE);
    $this->assertEquals(dkan_health_status_api_access_check('keyexpected', 'keyexpected', TRUE, FALSE), FALSE);
  }

  /**
   * Verify that index count works correctly.
   *
   * @covers dkan_health_status_dkan_health_status_monitor_check_solr_index_count().
   */
  public function testGetEmailReceiverUser() {
    $index = search_api_index_load('datasets');
    search_api_index_items($index);
    $count = dkan_health_status_dkan_health_status_monitor_check_solr_index_count();
    $this->assertEquals($count['node_count'],$count['search_count']);
  }
}
