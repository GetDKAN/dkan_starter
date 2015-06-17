
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

  # TODO: Change to use Workbench instead of /content

  @api @wip
  Scenario: Create resource
    Given I am logged in as "Katie"
    And I am on "My Workbench" page
    And I click "Add Content"
    And I click "Resource"
    When I fill in the "resource" form for "Resource 06"
    And I press "Save"
    Then I should see "Resource Resource 06 has been created"
    When I am on "Content" page
    Then I should see "Resource 06"

  # TODO: Needs definition.

  @api @wip
  Scenario: Create resources with GeoJSON data
    Given I am on the homepage

  # TODO: Needs definition.

  @api @wip
  Scenario: Bureau & Program Code are auto populated on creation
    Given I am on the homepage

  # TODO: Change to use Workbench instead of /content

  @api @wip
  Scenario: Edit own resource
    Given I am logged in as "Katie"
    And I am on "Resource 02" page
    When I click "Edit"
    And I fill in "title" with "Resource 02 edited"
    And I press "Save"
    Then I should see "Resource Resource 02 edited has been updated"
    When I am on "Content" page
    Then I should see "Resource 02 edited"

  @api @wip
  Scenario: A data contributor should not be able to publish resources
    Given I am logged in as "Katie"
    And I am on "Resource 02" page
    When I click "Edit"
    Then I should not see "Publishing options"

  # TODO: Needs definition. How can a data contributor unpublish content?

  @api @wip
  Scenario: Unpublish own resource
    Given I am on the homepage

  @api @wip
  Scenario: Manage datastore of own resource
    Given I am logged in as "Katie"
    And I am on "Resource 02" page
    When I press "Manage datastore"
    Then I should see "There is nothing to manage! You need to upload or link to a file in order to use the datastore."

  @api @wip
  Scenario: Import items on datastore of own resource
    Given I am logged in as "Katie"
    And I am on "Resource 02" page
    When I press "Manage datastore"
    And I press "Import"
    And I press "Import"
    And I wait
    Then I should see "Last import"
    And I should see "imported items total"

  @api @wip
  Scenario: Delete items on datastore of own resource
    Given I am logged in as "Celeste"
    And I am on "Resource 03" page
    When I press "Manage datastore"
    And I press "Delete items"
    And I press "Delete"
    And I wait
    Then I should see "items have been deleted."
    When I press "Manage datastore"
    Then I should see "No imported items."

  @api @wip
  Scenario: Drop datastore of own resource
    Given I am logged in as "Celeste"
    And I am on "Resource 03" page
    And I press "Manage datastore"
    When I press "Drop datastore"
    And I press "Drop"
    Then I should see "Datastore dropped!"
    And I should see "Your file for this resource is not added to the datastore"
    When I press "Manage datastore"
    Then I should see "No imported items."

  @api @wip
  Scenario: Add revision to own resource
    Given I am logged in as "Katie"
    And I am on "Resource 02" page
    When I click "Edit"
    And I fill in "title" with "Resource 02 edited"
    And I check "Create new revision"
    And I press "Save"
    Then I should see "Resource Resource 02 edited has been updated"
    When I press "Revisions"
    And I click "first" revision
    Then I should see "Resource 02 edited"
