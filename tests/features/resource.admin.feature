
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
  Scenario: Edit any resource
    Given I am logged in as "John"
    And I am on "Resource 02" page
    When I click "Edit"
    And I fill in "title" with "Resource 02 edited"
    And I press "Save"
    Then I should see "Resource Resource 02 edited has been updated"
    When I am on "Content" page
    Then I should see "Resource 02 edited"

  @api @wip
  Scenario: Publish any resource
    Given I am logged in as "John"
    And I am on "Resource 04" page
    When I click "Edit"
    And I select "published" for "publishing options"
    And I press "Save"
    Then I should see "Resource Resource 04 edited has been updated"

  # TODO

  @api @wip
  Scenario: Unpublish any resource
    Given I am on the homepage

  @api @wip
  Scenario: Manage datastore of any resource
    Given I am logged in as "John"
    And I am on "Resource 02" page
    When I press "Manage datastore"
    Then I should see "There is nothing to manage! You need to upload or link to a file in order to use the datastore."

  @api @wip
  Scenario: Import items on datastore of any resource
    Given I am logged in as "John"
    And I am on "Resource 02" page
    When I press "Manage datastore"
    And I press "Import"
    And I press "Import"
    And I wait
    Then I should see "Last import"
    And I should see "imported items total"

  @api @wip
  Scenario: Delete items on datastore of any resource
    Given I am logged in as "John"
    And I am on "Resource 04" page
    When I press "Manage datastore"
    And I press "Delete items"
    And I press "Delete"
    And I wait
    Then I should see "items have been deleted."
    When I press "Manage datastore"
    Then I should see "No imported items."

  @api @wip
  Scenario: Drop datastore of any resource
    Given I am logged in as "John"
    And I am on "Resource 04" page
    And I press "Manage datastore"
    When I press "Drop datastore"
    And I press "Drop"
    Then I should see "Datastore dropped!"
    And I should see "Your file for this resource is not added to the datastore"
    When I press "Manage datastore"
    Then I should see "No imported items."

  @api @wip
  Scenario: Add revision to any resource
    Given I am logged in as "John"
    And I am on "Resource 02" page
    When I click "Edit"
    And I fill in "title" with "Resource 02 edited"
    And I check "Create new revision"
    And I press "Save"
    Then I should see "Resource Resource 02 edited has been updated"
    When I press "Revisions"
    And I click "first" revision
    Then I should see "Resource 02 edited"

  @api @wip
  Scenario: Revert any resource revision
    Given I am logged in as "John"
    And I am on "Resource 02" page
    When I click "Edit"
    And I fill in "title" with "Resource 02 edited"
    And I check "Create new revision"
    And I press "Save"
    Then I should see "Resource Resource 02 edited has been updated"
    When I press "Revisions"
    And I click "Revert" in the "second" row
    # TODO: This is NOT working. Throws "You are not authorized to access this page"
    Then the resource should be reverted

  @api @wip
  Scenario: Delete any resource
    Given I am logged in as "John"
    And I am on "Resource 02" page
    When I click "Delete"
    And I press "Delete"
    Then I should see "Resource Resource 02 has been deleted"