
Feature: General


  ##################################################################
  # ALL ( ANONYMOUS + AUTHENTICATED )
  ##################################################################

  @api
  Scenario: See about page
    Given I am on the homepage
    When I follow "about"
    Then I should see the "about" page
