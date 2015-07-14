Feature: Testing content editor role and permissions

  @api
  Scenario: Access a list of unpublished content
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | editor           | editor@test.com           | 1      | 254633039 |
      And "dataset" nodes:
        | title        | author           | status |
        | test dataset | data_contributor | 0      |
      And "resource" nodes:
        | title         | author           | status |
        | test resource | data_contributor | 0      |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members                 | nodes                      |
        | test group | data_contributor,editor | test dataset,test resource |
    When I am logged in as "editor"
      And I am on "/admin/dkan/unpublished-data"
    Then I should see "test dataset"
      And I should see "test resource"

  @api
  Scenario: Review a dataset posted by a data contributor
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | editor           | editor@test.com           | 1      | 254633039 |
      And "dataset" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members                 | nodes |
        | test group | data_contributor,editor | test  |
    When I am logged in as "editor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I fill in "body[und][0][value]" with "Updated body"
      And I press "Save"
    Then I should see "Dataset test has been updated"

  @api
  Scenario: Review a resource posted by a data contributor
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | editor           | editor@test.com           | 1      | 254633039 |
      And "resource" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members                 | nodes |
        | test group | data_contributor,editor | test  |
    When I am logged in as "editor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I fill in "body[und][0][value]" with "Updated body"
      And I press "Save"
    Then I should see "Resource test has been updated"
  
  @api @javascript
  Scenario: Publish a dataset posted by a data contributor
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | editor           | editor@test.com           | 1      | 254633039 |
      And "dataset" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members                 | nodes |
        | test group | data_contributor,editor | test  |
    When I am logged in as "editor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I check the box "edit-status"
      And I press "Save"
    Then I should see "Dataset test has been updated"
  
  @api @javascript
  Scenario: Publish a resource posted by a data contributor
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | editor           | editor@test.com           | 1      | 254633039 |
      And "resource" nodes:
        | title | author           | status |
        | test  | data_contributor | 0      |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members                 | nodes |
        | test group | data_contributor,editor | test  |
    When I am logged in as "editor"
      And I am on "/admin/dkan/unpublished-data"
    When I click "edit"
      And I check the box "edit-status"
      And I press "Save"
    Then I should see "Resource test has been updated"

  # @api @javascript
  # Scenario: Manage Datastore
  #   Given users:
  #     | name             | mail                      | status | roles     |
  #     | editor           | editor@test.com           | 1      | 254633039 |
  #   When I am logged in as "editor"
  #     And I am on "/dataset/afghanistan-election-districts/resource/6b659860-b86c-46b9-9b86-62b47f446458"
  #     And I click "Manage Datastore" in the "toolbar" region
  #     And I press "Import"
  #     And I wait for "7" seconds
  #     And I click "View" in the "toolbar" region
  #   Then I should see "resource has been added to the datastore"
