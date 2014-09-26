Feature: Testing group organizer role and permissions

  @api @javascript
  Scenario: Add Author to group
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | group_organizer  | go@test.com               | 1      | 161863194 |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |

    When I am logged in as "group_organizer"
      And I am on "/group/test-group"
    When I click "Group" in the "toolbar" region
      And I click "Add people"
      And I wait for "3" seconds
    When I fill in "name" with "data_contributor"
      And I wait for "3" seconds
      And I press "edit-submit"
      And I wait for "3" seconds
    Then I should see "data_contributor has been added to the group"

  @api @javascript
  Scenario: Remove Author from group
    Given users:
      | name             | mail                      | status | roles     |
      | data_contributor | data_contributor@test.com | 1      | 226931607 |
      | group_organizer  | go@test.com               | 1      | 161863194 |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members                          |
        | test group | data_contributor |
    When I am logged in as "group_organizer"
      And I am on "/group/test-group"
    When I click "Group" in the "toolbar" region
      And I click "People"
      And I check the box "edit-views-bulk-operations-0"
      And I click "remove"
      And I press "Remove"
    Then I should see "The membership was removed."
      And I should not see "data_contributor"
