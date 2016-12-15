
Feature: General


  ##################################################################
  # ALL ( ANONYMOUS + AUTHENTICATED )
  ##################################################################

  @api
  Scenario: See about page
    Given I am on the homepage
    When I click "Datasets"
    Then I should see "Content Types"
