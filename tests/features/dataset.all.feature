
Feature: Dataset Features
  In order to realize a named business value
  As an explicit system actor
  I want to gain some beneficial outcome which furthers the goal

  Additional text...


  Background:
    Given users:
      | name    | mail             | roles                |
      | John    | john@test.com    | administrator        |
      | Admin   | admin@test.com   | administrator        |
      | Gabriel | gabriel@test.com | content editor       |
      | Jaz     | jaz@test.com     | data contributor     |
      | Katie   | katie@test.com   | data contributor     |
      | Martin  | martin@test.com  | data contributor     |
      | Celeste | celeste@test.com | data contributor     |
    Given groups:
      | title    | author | published |
      | Group 01 | Admin  | Yes       |
      | Group 02 | Admin  | Yes       |
      | Group 03 | Admin  | No        |
    And group memberships:
      | user    | group    | role on group        | membership status |
      | Gabriel | Group 01 | administrator member | Active            |
      | Katie   | Group 01 | member               | Active            |
      | Jaz     | Group 01 | member               | Pending           |
      | Admin   | Group 02 | administrator member | Active            |
      | Celeste | Group 02 | member               | Active            |
    And datasets:
      | title      | format | author  | published        | Date         | tags   |
      | Dataset 01 | CSV    | Gabriel | Yes              | Feb 01, 2015 | Health |
      | Dataset 02 | XLS    | Gabriel | Yes              | Mar 13, 2015 | Gov    |
      | Dataset 03 | CSV    | Katie   | Yes              | Feb 17, 2013 | Health |
      | Dataset 04 | CSV    | Celeste | No, Draft        | Dic 21, 2015 | Gov    |
      | Dataset 05 | CSV    | Katie   | No, Needs review | Dic 21, 2015 | Gov    |
    And resources:
      | title       | dataset    | published |
      | Resource 01 | Dataset 01 | Yes       |
      | Resource 02 | Dataset 01 | Yes       |
      | Resource 03 | Dataset 02 | Yes       |

  @api @wip
  Scenario: View list of most recent published datasets (on homepage)
    Given I am on the homepage
    Then I should see "3" items in the "datasets" region
    And I should see the list with "desc" order by "date changed"

  @api @wip
  Scenario: View list of published datasets
    Given I am on the homepage
    When I click "Datasets"
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region

  @api @wip
  Scenario: Search datasets by "date changed" with "asc" order
    Given I am on "Datasets" page
    When I fill in "search" with "2015"
    And I select "date changed" from "criteria"
    And I select "asc" from "order"
    And I press "Apply"
    Then I should see "2 datasets"
    And I should see "2" items in the "datasets" region
    And I should see the list with "asc" order by "date changed"

  @api @wip
  Scenario: Search datasets by "date changed" with "desc" order
    Given I am on "Datasets" page
    When I fill in "search" with "2015"
    And I select "date changed" from "criteria"
    And I select "desc" from "order"
    And I press "Apply"
    Then I should see "2 datasets"
    And I should see "2" items in the "datasets" region
    And I should see the list with "desc" order by "date changed"

  @api @wip
  Scenario: Search datasets by "title" with "asc" order
    Given I am on "Datasets" page
    When I fill in "search" with "Dataset"
    And I select "title" from "criteria"
    And I select "asc" from "order"
    And I press "Apply"
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region
    And I should see the list with "asc" order by "title"

  @api @wip
  Scenario: Search datasets by "title" with "desc" order
    Given I am on "Datasets" page
    When I fill in "search" with "Dataset"
    And I select "title" from "criteria"
    And I select "desc" from "order"
    And I press "Apply"
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region
    And I should see the list with "desc" order by "title"

  @api @wip
  Scenario: Reset dataset search filters
    Given I am on "Datasets" page
    When I fill in "search" with "01"
    And I press "Apply"
    Then I should see "1 datasets"
    And I should see "1" items in the "datasets" region
    When I press "Reset"
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region

  @api @wip
  Scenario: View available tag filters for datasets
    Given I am on "Datasets" page
    Then I should see "Health (2)" in the "Filter by tags" region
    Then I should see "Gov (1)" in the "Filter by tags" region

  @api @wip
  Scenario: View available resource format filters for datasets
    Given I am on "Datasets" page
    Then I should see "CSV (2)" in the "Filter by resources format" region
    Then I should see "XLS (1)" in the "Filter by resources format" region

  @api @wip
  Scenario: View available author filters for datasets
    Given I am on "Datasets" page
    Then I should see "Gabriel (2)" in the "Filter by author" region
    Then I should see "Katie (1)" in the "Filter by author" region

  @api @wip
  Scenario: Filter dataset search results by tags
    Given I am on "Datasets" page
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region
    When I click "Health" in the "Filter by tags" region
    Then I should see "2 datasets"
    And I should see "2" items in the "datasets" region

  @api @wip
  Scenario: Filter dataset search results by resource format
    Given I am on "Datasets" page
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region
    When I click "CSV" in the "Filter by resources format" region
    Then I should see "2 datasets"
    And I should see "2" items in the "datasets" region

  @api @wip
  Scenario: Filter dataset search results by author
    Given I am on "Datasets" page
    Then I should see "3 datasets"
    And I should see "3" items in the "datasets" region
    When I click "Gabriel" in the "Filter by author" region
    Then I should see "2 datasets"
    And I should see "2" items in the "datasets" region

  @api @wip
  Scenario: View published dataset
    Given I am on "Datasets" page
    When I click "Dataset 01"
    # I should see the license information
    Then I should see "Dataset 01" detail page

  @api @wip
  Scenario: Share published dataset on Google +
    Given I am on "Dataset 01" page
    When I click "Google+" in the "social" region
    Then I should be redirected to "Google+" sharing page for "Dataset 01"

  @api @wip
  Scenario: Share published dataset on Twitter
    Given I am on "Dataset 01" page
    When I click "Twitter" in the "social" region
    Then I should be redirected to "Twitter" sharing page for "Dataset 01"

  @api @wip
  Scenario: Share published dataset on Facebook
    Given I am on "Dataset 01" page
    When I click "Facebook" in the "social" region
    Then I should be redirected to "Facebook" sharing page for "Dataset 01"

  @api @wip
  Scenario: View published dataset information as JSON
    Given I am on "Dataset 01" page
    When I click "JSON" in the "other access" region
    Then I should see the content in "JSON" format

  @api @wip
  Scenario: View published dataset information as RDF
    Given I am on "Dataset 01" page
    When I click "RDF" in the "other access" region
    Then I should see the content in "RDF" format

  @api @wip
  Scenario: Download file from published dataset
    Given I am on "Dataset 01" page
    When I press "Download" in the "Resource 01" row
    Then A file should be downloaded

  # TODO: Needs definition
  @api @wip
  Scenario: View a list of suggested datasets when viewing a dataset
    Given I am on the homepage