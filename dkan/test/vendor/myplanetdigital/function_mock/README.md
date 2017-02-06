# Function Mocking Framework
[![Build Status](https://travis-ci.org/myplanetdigital/function_mock.png?branch=master)](https://travis-ci.org/myplanetdigital/function_mock)

Generic PHP framework to help generate function stubs that haven't been defined in a given file. Use in conjunction with [PHPUnit](https://github.com/sebastianbergmann/phpunit/) for unit testing. See [this article](http://martinfowler.com/articles/mocksArentStubs.html) for more information about mocks and stubs.

## Background

This framework was spurred from a desire to write PHPUnit tests for Drupal CMS modules. Since Drupal (7 and earlier) is not object-oriented, it makes use of functions predominantly, and often calls other functions that are assumed to have been imported by Drupal. Some of these access the database, making it hard to isolate, unfortunately.

Since everything in Drupal is enclosed in a function, and PHPUnit cannot mock functions directly (it can only mock classes),  this framework was created to allow you to generate actual 'mocks' for the functions so that they can be stubbed. Doing it this way allows you to test a .module file by itself, for example, without having to include those other dependent files.

## Creating mocks

To create a mock, use `FunctionMock::createMockFunctionDefinition($functionName)` with the name of the function to be mocked:

    e.g. FunctionMock::createMockFunctionDefinition('external_method');

What this does under the hood is actually create and evaluate a new function called `external_method()`. The implementation of it allows its return value to be stubbed to whatever you'd like, via the `FunctionMock::stub($functionName, $stubValue)` method.

## Stubbing mocks

To stub a mock's return value, use the `FunctionMock::stub(...)` method. 

There are two versions, one that sets what the method should return if called in general: `FunctionMock::stub($functionName, $stubValue)`

    e.g. FunctionMock::stub('external_method', 'abc');

Then, if the following is executed:
    
    $result = external_method();

`$result` returns `'abc'`.
    
The other version takes an array for the third argument, specifying the return value for an exact argument match: `FunctionMock::stub($functionName, $stubValue, $paramList)`

    e.g. FunctionMock::stub('external_method', 'def', array('param1', 'param2'));

Now, if the following were to be called:
    
    $result = external_method('param1', 'param2');

The value for `$result` would be `'def'`.

If you want to reset all the stubbed values, call `FunctionMock::resetStubs()`, which clears out all the stubbed value for each of the mocks. 

## Putting it all together - a PHPUnit example 

Let's use an example out of Drupal's Block module:

    /**
     * Implements hook_block_info().
     */
    function block_block_info() {
      $blocks = array();

      $result = db_query('SELECT bid, info FROM {block_custom} ORDER BY info');
      foreach ($result as $block) {
        $blocks[$block->bid]['info'] = $block->info;
        // Not worth caching.
        $blocks[$block->bid]['cache'] = DRUPAL_NO_CACHE;
      }
      return $blocks;
    }

Notice that `block_block_info()` cannot be easily tested without also testing `db_query()` as well, which accesses the database.

The key to unit testing is to assume that all of its dependent classes and functions are already working, so you'll want to assume that `db_query()` works just fine, mock it since it's an external function, and stub its return value accordingly.

Given you have [PHPUnit installed](http://phpunit.de/manual/3.7/en/installation.html), you can write a test case like so:

    <?php

    require_once '../modules/block/block.module';
    require_once '../sites/all/libraries/function_mock/function_mock.php';

    class BlockTest extends PHPUnit_Framework_TestCase
    {
        public function testBlockBlockInfo()
        {
          // Setup initial test variables.
          define('DRUPAL_NO_CACHE', -5);

          $blockInfo = array();
          $blockInfo[] = (object) array('bid' => 12345, 'info' => 'Block Info 1');
          $blockInfo[] = (object) array('bid' => 23456, 'info' => 'Block Info 2');

          FunctionMock::createMockFunctionDefinition('db_query');
          FunctionMock::stub('db_query', $blockInfo);

          // Exercise the block_block_info() method.
          $result = block_block_info();

          // Verify it worked.
          $this->assertEquals('Block Info 1', $result[12345]['info']);
          $this->assertEquals(DRUPAL_NO_CACHE, $result[12345]['cache']);
          $this->assertEquals('Block Info 2', $result[23456]['info']);
          $this->assertEquals(DRUPAL_NO_CACHE, $result[23456]['cache']);
        }
    }
    ?>

## Auto-generating mocks

Although you can generate a mock function for each one you need, you can also have function_mock autogenerate all the functions it can based on the files you're testing. For that, use `FunctionMock::generateMockFunctions($srcFileList)` and provide a list of all the source files you want to have tested. This method will search within the scope of `$srcFileList` and determine which functions don't have an implementation for them, creating mocks for each one.

Here's an example of how you could use it:

    <?php

    require_once '../modules/block/block.module';
    require_once '../sites/all/libraries/function_mock/function_mock.php';

    class BlockTest extends PHPUnit_Framework_TestCase
    {
        public function __construct()
        {
          // Generate all functions that need mocks from the block module, based on what
          // hasn't been defined yet.
          FunctionMock::generateMockFunctions(array('../modules/block/block.module'));
        }

        public function testBlockBlockInfo()
        {
          // Setup initial test variables.
          define('DRUPAL_NO_CACHE', -5);

          $blockInfo = array();
          $blockInfo[] = (object) array('bid' => 12345, 'info' => 'Block Info 1');
          $blockInfo[] = (object) array('bid' => 23456, 'info' => 'Block Info 2');

          FunctionMock::stub('db_query', $blockInfo);

          // Exercise the block_block_info() method.
          $result = block_block_info();

          // Verify it worked.
          $this->assertEquals('Block Info 1', $result[12345]['info']);
          $this->assertEquals(DRUPAL_NO_CACHE, $result[12345]['cache']);
          $this->assertEquals('Block Info 2', $result[23456]['info']);
          $this->assertEquals(DRUPAL_NO_CACHE, $result[23456]['cache']);
        }

        public function __destruct()
        {
          // Clean up the stubbed values.
          FunctionMock::resetStubs();
        }    
    }
    ?>

## Open items

Please feel free to log any issues or suggestions for this framework. So far, here are some known ones:

* Stronger exception handling when creating mocks/stubs for methods that may already exist. The mocks created are currently not tracked so it's difficult to determine which ones are system ones and which ones are mocked functions.
* Some general clean up work in `function_mock` to separate some of its functionality.
* Some more documentation for where to put the framework code if using Drupal, or straight PHP.
* Documentation for error scenarios, plus some additional tests.

## License

This projected is licensed under the terms of the MIT license.