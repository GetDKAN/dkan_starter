Feature: Resource

  Background:
    Given users:
      | name    | mail             | roles                |
      | John    | john@test.com    | portal administrator |
      | Admin   | admin@test.com   | portal administrator |
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
      | Celeste | Group 02 | member               | Active            |
    And resources:
      | title       | dataset    | author   | published | datastore created |
      | Resource 01 | Dataset 01 | Katie    | Yes       | No                |
      | Resource 02 | Dataset 01 | Katie    | Yes       | No                |
      | Resource 03 | Dataset 02 | Celeste  | No        | Yes               |
      | Resource 04 | Dataset 01 | Katie    | No        | Yes               |
      | Resource 05 | Dataset 02 | Celeste  | Yes       | Yes               |

  @api @wip
  Scenario: View published resource
    Given I am on the homepage
    And I follow "Datasets"
    And I click "Dataset 01"
    When I click "Resource 01"
    # License information should be shown
    Then I should be able to see the "Resource 01" detail page

  @api @wip
  Scenario: View published resource data as a Graph
    Given I am on "Resource 01" page
    When I click "Graph"
    Then I should view the "resource" content as "graph"

  @api @wip
  Scenario: View published resource data as a Grid
    Given I am on "Resource 01" page
    When I click "Grid"
    Then I should view the "resource" content as "grid"

  @api @wip
  Scenario: View published resource data as Map
    Given I am on "Resource 01" page
    When I click "Map"
    Then I should view the "resource" content as "map"

  @api @wip
  Scenario: View the Data API information for a published resource
    Given I am on "Resource 01" page
    When I press "Data API"
    Then I should see "The Resource ID for this resource is"
    And I should see "Example Query"

  @api @wip
  Scenario: View previous revisions of published resource
    Given I am on "Resource 01" page
    When I press "Revisions"
    Then I should see the list of revisions

  @api @wip
  Scenario: Compare revisions of published resource
    Given I am on "Resource 01" page
    And I press "Revisions"
    When I select "revision 1"
    And I select "revision 2"
    And I press "Compare"
    Then I should see the revisions diff

  # TODO: Needs definition.

  @api @wip
  Scenario: View resource data on map automatically if lat and long info is present
    Given I am on the homepage