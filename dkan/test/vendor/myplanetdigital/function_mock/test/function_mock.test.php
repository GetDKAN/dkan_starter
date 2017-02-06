<?php

require_once '../function_mock.php';

class FunctionMockTest extends PHPUnit_Framework_TestCase
{

  public function testCreateMockFunctionDefinitions() {
    // Generate a stub method dynamically and ensure you can call it.
    $result = FunctionMock::createMockFunctionDefinitions(array('test_method1', 'test_method2'));

    $this->assertEquals('function test_method1() { return FunctionMock::getStubbedValue(__FUNCTION__, count(func_get_args()) > 0 ? func_get_args() : NULL); } ' . 
      'function test_method2() { return FunctionMock::getStubbedValue(__FUNCTION__, count(func_get_args()) > 0 ? func_get_args() : NULL); } ', $result);

    FunctionMock::stub('test_method1', 3);

    $this->assertEquals(3, test_method1());
  }

  public function testStub() {
    // Set up the test input.
    $testStubValue = 3;
    $functionName = 'testStub';

    FunctionMock::createMockFunctionDefinition($functionName);
    FunctionMock::stub($functionName, $testStubValue);

    // Check that it sets it correctly.
    $this->assertEquals($testStubValue, testStub());
  }

  public function testGetStubbedValueForStub() {
    $testStubValue = 3;
    $functionName = 'test';

    FunctionMock::createMockFunctionDefinition($functionName);
    FunctionMock::stub($functionName, $testStubValue);
    FunctionMock::stub($functionName, $testStubValue);

    $actualResult = FunctionMock::getStubbedValue($functionName);

    $this->assertEquals($testStubValue, $actualResult);
  }

  public function testStubWithParameters() {
    // Set up the test input.
    $testStubValue = 3;
    $testStubValueWithParam = 5;
    $functionName = 'testStubParams';
    $paramList = array('param1', 'param2');

    FunctionMock::createMockFunctionDefinition($functionName);
    FunctionMock::stub($functionName, $testStubValue);
    FunctionMock::stub($functionName, $testStubValueWithParam, $paramList);

    // Check that it sets it correctly.
    $this->assertEquals($testStubValue, testStubParams());    
    $this->assertEquals($testStubValueWithParam, testStubParams('param1', 'param2'));    
  }

  /**
   * @expectedException        StubMissingException
   * @expectedExceptionMessage nonExistentStub has not been stubbed yet.
   */
  public function testGetStubbedValueForNonExistentMock() {
    $actualResult = FunctionMock::getStubbedValue('nonExistentStub');
  }

  public function testGetUniqueFunctions() {
    $result = FunctionMock::findFunctionsNeedingMocks(array('./test_php.php'));

    $this->assertEquals(7, count($result));
  }

  public function testGenerateMockFunctions() {
    // Generate a stub method dynamically and ensure you can call it.
    FunctionMock::generateMockFunctions(array('./test_php.php'));

    FunctionMock::stub('abc', 3);

    $this->assertEquals(3, abc('http://abc.com/get'));
  }

  public function testResetStubs() {
    // Stub a few functions, then reset them and see that nothing shows up.
    $testStubValue = 3;
    $functionName = 'testResetStub';

    FunctionMock::createMockFunctionDefinition($functionName);
    FunctionMock::stub($functionName, $testStubValue);

    // Test first that the stub exists and works.
    $this->assertEquals($testStubValue, testResetStub());

    // Then, reset the stubs and make sure it doesn't return anything.
    FunctionMock::resetStubs();

    // Test first that the stub exists and works.
    try {
      testResetStub();
      $this->fail('Should have thrown a StubMissingException');
    } catch (StubMissingException $e) {
      // Expected behavior.
      return;
    }
  }

  public function testStubMissingExceptionHelpful() {
    // Test functions that haven't been stubbed yet and see that it returns
    // a helpful message for what to return.
    $testStubValue = 3;
    $functionName = 'firstStub';

    // Create the mock definition, but don't stub it just yet.
    FunctionMock::createMockFunctionDefinition($functionName);

    // Test first that the stub exists and works.
    try {
      testResetStub();
      $this->fail('Should have thrown a StubMissingException');
    } catch (StubMissingException $e) {
      // Expected behavior.
      return;
    }
  }
  
  public function testMockWithNoParams() {
    // Set up the test input.
    
    $functionName = 'testMock';

    FunctionMock::createMockFunctionDefinition($functionName);
    FunctionMock::mock($functionName);

    // Make calls to stubbed function.
    testMock();
    testMock();
    testMock();
    
    $this->assertEquals(3, FunctionMock::verifyMockTimesCalled($functionName));
  }
  
  public function testMockWithParams() {
    // Set up the test input.
    
    $functionName = 'testMockWithParams';

    FunctionMock::createMockFunctionDefinition($functionName);
    FunctionMock::mock($functionName);

    // Create testing params
    $arg1 = 125;
    $arg2 = "Test String";
    $arg3 = array('a', 'b', 5);
    $arg4 = new stdClass();
    $arg4->a = 'a';
    $arg4->b = 22;
    $arg5 = NULL;
    $arg6 = true;
    
    
    // Check that it sets it correctly.
    testMockWithParams($arg1, $arg2);
    $this->assertEquals(1, FunctionMock::verifyMockTimesCalled($functionName, $arg1, $arg2));
    
    testMockWithParams($arg3, $arg4, $arg1);
    $this->assertEquals(1, FunctionMock::verifyMockTimesCalled($functionName, $arg3, $arg4, $arg1));
    
    testMockWithParams($arg5, $arg6, $arg4);
    $this->assertEquals(1, FunctionMock::verifyMockTimesCalled($functionName, $arg3, $arg4, $arg1));
  }
  
  /**
   * @expectedException        FunctionWasNotMockedException
   * @expectedExceptionMessage nonExistentMock was not mocked.
   */
  public function testVerifyMockCalledForNonExistingMock() {
    $actualResult = FunctionMock::verifyMockTimesCalled('nonExistentMock');
  }

  // TODO: Write some exception handling cases.
  // /**
  //  * @expectedException        BadFunctionCallException
  //  * @expectedExceptionMessage testNonMockedFunction does not exist or is not a mock.
  //  */
  // public function testCannotStubNotMockedFunction() {
  //   // Set up the test input.
  //   $testStubValue = 3;
  //   $functionName = 'testNonMockedFunction';

  //   FunctionMock::stub($functionName, $testStubValue);

  //   // Check that it sets it correctly.
  //   $this->assertEquals($testStubValue, testStub());        
  // }  
}

class DBQueryStubObject
{
  private $valueToReturn;

  public function __construct($valueToReturn) {
    $this->valueToReturn = $valueToReturn;
  }

  public function fetchObject() {
    return $this->valueToReturn;
  }
}  

?>