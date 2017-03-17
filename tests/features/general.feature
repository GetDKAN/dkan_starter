
Feature: General


  ##################################################################
  # ALL ( ANONYMOUS + AUTHENTICATED )
  ##################################################################

  @api @no-main-menu
  Scenario: See about page
    Given I am on the homepage
    When I click "Datasets"
    Then I should see "Content Types"

  # This is a dummy test so the feature is no empty in the case of all other scenarios being skipped.
  Scenario: Dummy test
    Given I am on the homepage
