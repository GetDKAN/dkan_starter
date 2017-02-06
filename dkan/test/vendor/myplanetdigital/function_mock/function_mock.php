<?php

define('TOKEN_CODE', 0);
define('TOKEN_VALUE', 1);

define('DEFAULT_STUB_VALUE', 'default_stub_value');
define('NO_TOKEN_FOUND_CODE', -999);
define('NEW_LINE', "\r\n");

/**
 * Class that supports stubbing functions.
 */
class FunctionMock
{
  /**
   * An array of keyed by names of stubbed functions that stores return values 
   * for stubbed functions.
   * 
   * @var array
   */
  private static $stubFunctionList = array();
  
  /**
   * An array of mocked states keyed by function names.
   * 
   * @var array 
   */
  private static $mockStates = array();

  /**
   * Returns a stubbed value based on the function name.
   *
   * @param $functionName
   *   The name of the function to retrieve the stubbed value from.
   * @return
   *   The stubbed value for the function, if it exists. Otherwise it
   *   will throw an exception asking a stubbed value to be defined.
   * @throws StubMissingException
   */
  public static function getStubbedValue($functionName, $paramList = NULL) {
    if (array_key_exists($functionName, self::$stubFunctionList)) {
      if (self::paramListSpecificStubExists($functionName, $paramList)) {
        return self::$stubFunctionList[$functionName][serialize($paramList)];
      } else {
        return self::$stubFunctionList[$functionName][DEFAULT_STUB_VALUE];
      }
    } else if (array_key_exists($functionName, self::$mockStates)) {
      self::$mockStates[$functionName][serialize($paramList)][] = TRUE;
    } else {
      throw new StubMissingException($functionName);
    } 
  }

  /**
   * Checks if a stub based on a set of parameters exists.
   *
   * @param $functionName
   *   The name of the function to retrieve the stubbed value from.
   * @param $paramList
   *   Array of parameters passed into the function to match on.
   * @return
   *   TRUE if it's found, FALSE otherwise.
   */
  private static function paramListSpecificStubExists($functionName, $paramList) {
    return $paramList !== NULL && array_key_exists(serialize($paramList), self::$stubFunctionList[$functionName]);    
  }

  /** 
   * Sets up a stub value for a given mock function. 
   *
   * @param $functionName
   *   The name of the function to retrieve the stubbed value from.
   * @param $returnValue
   *   The value that you want returned when the function is called.
   * @param $paramList
   *   Optional array of parameters you want an exact match on, so you can 
   *   do a conditional stub.
   * @throws FunctionDoesNotExistException
   */
  public static function stub($functionName, $returnValue, $paramList = NULL) {
    if (!function_exists($functionName)) {
      throw new FunctionDoesNotExistException($functionName);
    }

    if ($paramList !== NULL) {
      // Make a key out of the $paramList array, by simply serializing it into a string.
      self::$stubFunctionList[$functionName][serialize($paramList)] = $returnValue;
    } else {
      self::$stubFunctionList[$functionName][DEFAULT_STUB_VALUE] = $returnValue;
    }
  }
  
  /**
   * Sets up a mock i.e. behaviour verification for a given function.
   * 
   * @param String $functionName
   *   The name of the function to retrieve the stubbed value from.
   * @param String $context
   *   A description of context in which behavior is observed.
   * @throws FunctionDoesNotExistException
   */
  public static function mock($functionName) {
    if (!function_exists($functionName)) {
      throw new FunctionDoesNotExistException($functionName);
    }
    
    self::$mockStates[$functionName] = array();
  }
  
  /**
   * Returns number of times mocked function was called.
   * 
   * @param $functionName 
   * @param ... Any parameters that mock is expected to be called with.
   * 
   * @return Number of times a function aws called with given list of parameters.
   * 
   * @throws FunctionDoesNotExistException
   */
  public static function verifyMockTimesCalled($functionName) {
    if (!isset(self::$mockStates[$functionName])) {
      throw new FunctionWasNotMockedException($functionName);
    }
    
    $paramList = func_get_args();
    // Remove function name from array, leaving only arguments.
    array_shift($paramList);
    
    $paramList = !empty($paramList) ? $paramList : NULL;
    
    $serialized_param_list = serialize($paramList);    
    
    if (isset(self::$mockStates[$functionName][$serialized_param_list])) {
      return count(self::$mockStates[$functionName][$serialized_param_list]);
    }

    return 0;
  }

  /**
   * Resets all the stubbed functions.
   */
  public static function resetStubs() {
    // Just empty the array.
    self::$stubFunctionList = array();
  }
  
  /**
   * Resets all the mocks.
   */
  public static function resetMocks() {
    // Just empty the array.
    self::$mockStates = array();
  }

  /**
   * Finds the nearest previous token that doesn't contain a space as the value.
   *
   * @param &$tokens
   *   Array of PHP language tokens to search.
   * @param $i
   *   Index to start searching from. The function will search for the next item from this point.
   * @return
   *   The value of the next non space token, or an array with a TOKEN_CODE of NO_TOKEN_FOUND_CODE is
   *   returned.
   */
  private static function findPrevNonSpaceToken(&$tokens, $i) {
    $counter = $i - 1;

    if ($counter <= 0) {
      return array(TOKEN_CODE => NO_TOKEN_FOUND_CODE);
    }

    while ($tokens[$counter][TOKEN_CODE] === T_WHITESPACE &&
            $counter >= 0) {
      $counter--;  
    }

    return $tokens[$counter];
  }

  /**
   * Finds the next token that doesn't contain a space as the value.
   *
   * @param &$tokens
   *   Array of PHP language tokens to search.
   * @param $i
   *   Index to start searching from. The function will search for the next item from this point.
   * @return
   *   The value of the next non space token, or an array with a TOKEN_CODE of NO_TOKEN_FOUND_CODE is
   *   returned.
   */
  private static function findNextNonSpaceToken(&$tokens, $i) {
    $counter = $i + 1;    

    if ($counter >= count($tokens)) {
      return array(TOKEN_CODE => NO_TOKEN_FOUND_CODE);
    }

    while ($tokens[$counter][TOKEN_CODE] === T_WHITESPACE &&
            $counter < count($tokens)) {
      $counter++;  
    }

    return $tokens[$counter];
  }

  /**
   * Returns the names of all the functions that would require 
   * mocks to be defined (based on a list of source files to search holistically).
   *
   * @param $srcFiles
   *   List of source files to consider. Note: In performing the search these files will be loaded and executed.
   * @return
   *   Array of function names that will need to be mocked.
   */  
  public static function findFunctionsNeedingMocks($srcFiles) {
    // Loop through all the source files to find the ones that need to be searched.
    $completeFile = self::loadFiles($srcFiles);

    // Do a token based search next based on the loaded files, which is closer but picks up some 
    // strings that don't entirely match.
    $tokens = token_get_all($completeFile);
        
    $result = array();

    for ($i = 0; $i < count($tokens); $i++) {
      $token = $tokens[$i];

      if ($token[TOKEN_CODE] !== T_STRING) {
        continue;
      }

      $nextToken = self::findNextNonSpaceToken($tokens, $i);

      if ($nextToken[TOKEN_CODE] !== '(') {
          // Not a function call.
          continue;
      }

      $prevToken = self::findPrevNonSpaceToken($tokens, $i);

      if ($prevToken[TOKEN_CODE] === NO_TOKEN_FOUND_CODE) {
          // It's a function definition, not a function call.
          continue;
      }

      if ($prevToken[TOKEN_CODE] === T_FUNCTION) {
          // It's a function definition, not a function call.
          continue;
      }

      if ($prevToken[TOKEN_CODE] === T_OBJECT_OPERATOR) {
          // It's a method invocation, not a function call.
          continue;
      }

      if ($prevToken[TOKEN_CODE] === T_DOUBLE_COLON) {
          // It's a static method invocation, not a function call.
          continue;
      }

      if ($prevToken[TOKEN_CODE] === T_NEW) {          
          // It's a class instantiation.
          continue;
      }
      
      // If it gets all the way here, it's a function call.
      $result[] = $token[TOKEN_VALUE];
    }

    // Look for functions that do not exist. These are the ones that need to be mocked.
    $result = array_filter($result, function($function) { return !function_exists(trim($function)); });

    return $result;
  }

  /**
   * Load and include files into the PHP context.
   *
   * @param $srcFiles
   *   List of source files to load.
   * @return
   *   Combined contents of all the files and their PHP code loaded and executed. Note if any errors occur on
   *   any specific file being loaded, it will be ignored.
   */  
  private static function loadFiles($srcFiles) {
    $result = '';
    foreach ($srcFiles as $srcFile) {
      include_once $srcFile;
      $result .= file_get_contents($srcFile);
    }

    return $result;
  }

  /**
   * Generate an actual function definition that can be stubbed, based on a function name.
   *
   * @param $functionName
   *   Function name you want a mock for.
   * @return
   *   PHP function definition code that was executed.
   */  
  public static function createMockFunctionDefinition($functionName) {    
    $newFunctionDefinition = 'function ' . $functionName 
      . '() { return FunctionMock::getStubbedValue(__FUNCTION__, count(func_get_args()) > 0 ? func_get_args() : NULL); } ';
    
    if (!function_exists($functionName)) {
      eval($newFunctionDefinition);
    }

    return $newFunctionDefinition;
  }  

  /**
   * Generate actual function definitions that can be stubbed, based on an array of function names.
   *
   * @param $functionList
   *   List of functions to create mocks for.
   * @return
   *   Combined contents of all PHP code for mock functions created based on the function list.
   */  
  public static function createMockFunctionDefinitions($functionList) {
    $functionString = '';
    
    foreach ($functionList as $item) {      
      $result = self::createMockFunctionDefinition($item);

      $functionString .= $result;
    }

    return $functionString;    
  }

  /**
   * Finds and generates mock functions based on a set of source files provided.
   *
   * Any functions that are not already PHP functions or declared within the scope of 
   * the passed source files list will be auto-generated with mocks.
   *
   * @param $srcFiles
   *   List of source files to consider for creating mock functions.
   * @return
   *   Combined contents of all PHP code for mock functions created based on the function list.
   */ 
  public static function generateMockFunctions($srcFiles) {
    $functionList = self::findFunctionsNeedingMocks($srcFiles);

    return self::createMockFunctionDefinitions($functionList);
  }
}

/**
 * Custom exception for a stub missing.
 */
class StubMissingException extends Exception
{
  public function __construct($functionName, $code = 0, Exception $previous = null) {
      $this->message = NEW_LINE . $functionName . ' has not been stubbed yet.' . NEW_LINE 
        . 'Please call FunctionMock::stub(\'' . $functionName . '\', <default_stub_value>) to set a return value for any set of parameters.' . NEW_LINE 
        . 'If you return a stub value based on a specific set of parameters, use FunctionMock::stub(\'' . $functionName . '\', <stub_value>, <array of parameters>).' . NEW_LINE;
 
      parent::__construct($this->message, $code, $previous);
  }

  public function __toString() {
    return $this->message;
  }
}

/**
 * Custom exception for a function not existing.
 */
class FunctionDoesNotExistException extends Exception
{
  public function __construct($functionName, $code = 0, Exception $previous = null) {
      $this->message = $functionName . ' does not exist, either in real or mocked format.' . "\r\n" .
        'Please call FunctionMock::createMockFunctionDefinition(\'' . $functionName . '\') to create a mock version of it.';
 
      parent::__construct($this->message, $code, $previous);
  }

  public function __toString() {
    return $this->message;
  }
}

/**
 * Custom exception for not mocked functions.
 */
class FunctionWasNotMockedException extends Exception
{
  public function __construct($functionName, $code = 0, Exception $previous = null) {
      $this->message = $functionName . ' was not mocked.' . "\r\n" .
        'Please call FunctionMock::mock(\'' . $functionName . '\') to create a mock for it.';
 
      parent::__construct($this->message, $code, $previous);
  }
  
  public function __toString() {
    return $this->message;
  }
}

?>