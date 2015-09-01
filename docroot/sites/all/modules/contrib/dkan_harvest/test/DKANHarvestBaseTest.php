<?php
class DKANHarvestBaseTest extends PHPUnit_Framework_TestCase
{

    protected function setUp()
    {
      if (file_exists("public://dkan-harvest-cache/demo.getdkan.com/90a2b708-7fea-4b92-8aee-43c4cfdd5f48")) {
        file_unmanaged_delete("public://dkan-harvest-cache/demo.getdkan.com/90a2b708-7fea-4b92-8aee-43c4cfdd5f48");
      }
      if (file_exists("public://dkan-harvest-cache/demo.getdkan.com/c2150dce-db96-4007-ba3f-fb4f3774902d")) {
        file_unmanaged_delete("public://dkan-harvest-cache/demo.getdkan.com/c2150dce-db96-4007-ba3f-fb4f3774902d");
      }
      if (file_exists("public://dkan-harvest-cache/demo.getdkan.com")) {
        drupal_rmdir("public://dkan-harvest-cache/demo.getdkan.com");
      }
      if (file_exists("public://dkan-harvest-cache")) {
        drupal_rmdir("public://dkan-harvest-cache");
      }
    }

    /**
     * TODO: move to base class.
     */
    public function getNodeByTitle($title) {
      $node = new stdClass();
      $query = new EntityFieldQuery();

      $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('title', $title)
        ->range(0,1)
        ->execute();

      if (!empty($entities['node'])) {
        $node = node_load(array_shift(array_keys($entities['node'])));
      }
      return $node;
    }

    /**
     * @covers dkan_harvest_sources_definition().
     */
    public function testDKANHarvestSourcesDefinition()
    {
      $sources = dkan_harvest_sources_definition();
      $this->assertEquals($sources['demo.getdkan.com']['filters']['keyword'][0], 'election');
    }

    /**
     * @covers dkan_harvest_sources_prepare_cache_dir().
     */
    public function testDKANHarvestSourcesPrepareCacheDir()
    {
      $dirName = 'test_dir';
      $this->assertEquals(file_exists(DKAN_HARVEST_CACHE_DIR . '/' . $dirName), NULL);
      $dir = dkan_harvest_sources_prepare_cache_dir($dirName);
      $this->assertEquals(file_exists(DKAN_HARVEST_CACHE_DIR . '/' . $dirName), TRUE);
      drupal_rmdir($dir);
      $this->assertEquals(file_exists(DKAN_HARVEST_CACHE_DIR . '/' . $dirName), NULL);
    }

    /**
     * @covers dkan_harvest_prepare_item_id().
     */
    public function testDKANHarvestPrepareItemId()
    {
      $url = 'http://example.com/what';
      $dir = dkan_harvest_prepare_item_id($url);
      $this->assertEquals($dir, 'what');

      $url = 'http://example.com/what/now';
      $dir = dkan_harvest_prepare_item_id($url);
      $this->assertEquals($dir, 'now');

      $url = 'http://example.com';
      $dir = dkan_harvest_prepare_item_id($url);
      $this->assertEquals($dir, '');
    }

    /**
     * @covers dkan_harvest_cache_data_process().
     */
    public function testDKANHarvestDataProcess()
    {
      $this->assertEquals(file_exists("public://dkan-harvest-cache/demo.getdkan.com/90a2b708-7fea-4b92-8aee-43c4cfdd5f48"), NULL);
      $sources = $this->DKANTestSource();
      dkan_harvest_cache_data_process($sources, microtime());
      $this->assertFileExists("public://dkan-harvest-cache/demo.getdkan.com/90a2b708-7fea-4b92-8aee-43c4cfdd5f48");
      $this->assertFileExists("public://dkan-harvest-cache/demo.getdkan.com/c2150dce-db96-4007-ba3f-fb4f3774902d");
    }

    /**
     * @covers dkan_harvest_filter_datasets().
     */
    public function testDKANHarvestDatasetFilter()
    {
      $sources = $this->DKANTestSource();
      $source = $sources['demo.getdkan.com'];
      $file = file_get_contents($source['remote']);
      $json = drupal_json_decode($file);
      $datasets = $json['dataset'];

      $this->assertEquals(count($datasets), 4);
      $this->assertEquals($datasets[0]['title'], "Wisconsin Polling Places TEST");
      $this->assertEquals($datasets[1]['title'], "US National Foreclosure Statistics January 2012 TEST");
      $this->assertEquals($datasets[2]['title'], "Gold Prices in London 1950-2008 (Monthly) TEST");
      $this->assertEquals($datasets[3]['title'], "Afghanistan Election Districts TEST");
      $datasets = dkan_harvest_filter_datasets($datasets, $source['filters'], $source['excludes']);
      $this->assertEquals(count($datasets), 2);
      $this->assertEquals($datasets[0]['title'], "Wisconsin Polling Places TEST");
      $this->assertEquals($datasets[1]['title'], "Afghanistan Election Districts TEST");
    }

    public function DKANTestSource()
    {
      return dkan_harvest_test_harvest_sources();
    }

    public function testDKANHarvestMigrate()
    {
      $node = $this->getNodeByTitle("Wisconsin Polling Places TEST");
      $node->title = isset($node->title) ? $node->title : NULL;
      $this->assertEquals($node->title, NULL);
      dkan_harvest_run();
      $node = $this->getNodeByTitle("Wisconsin Polling Places TEST");
      $this->assertEquals($node->title, "Wisconsin Polling Places TEST");
    }
}
