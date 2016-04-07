
Feature: General


  ##################################################################
  # ALL ( ANONYMOUS + AUTHENTICATED )
  ##################################################################

  @api
  Scenario: See about page
    Given I am on the homepage
    When I click "About"
    Then I should see "DKAN is the Drupal-based version of CKAN"
