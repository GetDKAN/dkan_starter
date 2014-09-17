Feature: Testing data contributor role and permissions

  @api
  Scenario: Can see the administration menu
    Given users:
      | name         | mail                  | status     | roles     |
      | data_contributor  | data_contributor@test.com  | 1          | 226931607 |
      And I am logged in as "data_contributor"
    When I am on the homepage
    Then I should see the administration menu

  @api @javascript
  Scenario: Can create a Dataset and Resource nodes
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And I am logged in as "data_contributor"
    When I am on "/node/add/dataset"
      And I fill in "edit-title" with "Test"
      And I fill in "body[und][0][value]" with "Test description"
      And I press "Next: Add data"
    Then I should see "Dataset Test has been created"
    When I fill in "edit-title" with "Test Resource"
      And I press "Save"
    Then I should see "Resource Test Resource has been created"

  @api
  Scenario: View my unpublished datasets
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And "dataset" nodes:
        | title   | author           | status |
        | test    | data_contributor | 0      |
        | another | data_contributor | 0      |
      And I am logged in as "data_contributor"
    When I am on "/admin/dkan/unpublished-data"
    Then I should see "test"
      And I should see "another"

  @api
  Scenario: View my unpublished resources
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And "resource" nodes:
        | title   | author           | status |
        | test    | data_contributor | 0      |
        | another | data_contributor | 0      |
      And I am logged in as "data_contributor"
    When I am on "/admin/dkan/unpublished-data"
    Then I should see "test"
      And I should see "another"

  @api
  Scenario: Update my own dataset
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And "dataset" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And I am logged in as "data_contributor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I fill in "body[und][0][value]" with "Test description Update"
      And I press "Save"
    Then I should see "Dataset test has been updated"

  @api
  Scenario: Update my own resource
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And "resource" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And I am logged in as "data_contributor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I fill in "body[und][0][value]" with "Test description Update"
      And I press "Save"
    Then I should see "Resource test has been updated"

  @api
  Scenario: Delete my own dataset
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And "dataset" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And I am logged in as "data_contributor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I press "Delete"
    Then I should see "This action cannot be undone"
      And I press "Delete"
      And I should see "Dataset test has been deleted"

  @api
  Scenario: Delete my own resource
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      And "resource" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And I am logged in as "data_contributor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I press "Delete"
    Then I should see "This action cannot be undone"
      And I press "Delete"
      And I should see "Resource test has been deleted"
