<?php

define ('TEST_CONST', 100);

// Test file for various scenarios where functions might be misinterpreted.
/**
 * Implements functionThatShouldNotShowUp();
 */
function test_function() {
  // Test a normal function should work properly.
  normalFunction('test data');

  // Test exceptions: start simple - call a function that doesn't exist with a space in between the function name
  // and "(".
  functionThatDoesNotExist ('param1', 'param2');

  functionThatDoesNotExist2    ('param1', 'param2');

  // Call a function within a function.
  abc(def('abc'));

  // Use a constant, that can be misconstrued as a function potentially.
  $test_result = 3 - TEST_CONST;

  // Test referring to an object.
  $testJsonResponse = (object) array ('data' => 'abc');
  $testJsonResponse->data = 'abc';

  $result = drupal_json_decode($json->data);

  // Create a reference to returned value
  $a = &reference_returner_function();

  // Static method call
  StaticTestClass::staticFunctionCall();

  // Method call
  $someInstance->someMetod();

  // Creating a class
  $a = new stdClass();
}

// class StaticTestClass {
//   public static function staticFunctionCall() {
//     return NULL;
//   }
// }

?>
