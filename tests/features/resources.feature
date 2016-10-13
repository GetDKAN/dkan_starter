Feature: Resources

  Background:
    Given users:
      | name    | mail                | roles                |
      | Gabriel | gabriel@example.com | content creator      |
    Given groups:
      | title    | author  | published |
      | Group 01 | admin  | Yes       |
    And group memberships:
      | user    | group    | role on group        | membership status |
      | Gabriel | Group 01 | administrator member | Active            |
    And "Tags" terms:
      | name    |
      | Health  |
    And datasets:
      | title      | publisher | author  | published        | tags     | description |
      | Dataset 01 | Group 01  | Gabriel | Yes              | Health   | Test        |
    And "Format" terms:
      | csv     |
    And resources:
      | title       | publisher | format | dataset    | author   | published | description |
      | Resource 01 | Group 01  | csv    | Dataset 01 | Gabriel    | Yes       | Yes          |
      | Resource 02 | Group 01  | html   | Dataset 01 | Gabriel    | Yes       | Yes          |

  @fixme @api @javascript
  Scenario: ClamAV scan for resource clean file upload
    Given I am logged in as "Gabriel"
    And I am on "Resource 01" page
    When I click "Edit"
    And I click "Upload"
    And I attach the file "postvaccinedeaths.csv" to "field_upload[und][0][resup]" using file resup
    And I wait for the file upload to finish
    And I press "Save"
    Then I should see "Resource Resource 01 has been updated"
    When I am on "Resource 01" page
    Then I should see "postvaccinedeaths.csv"

  @fixme @api @javascript
  Scenario: ClamAV scan for resource test signature upload
    Given I am logged in as "Gabriel"
    And I am on "Resource 01" page
    And  I click "Edit"
    And I click "Upload"
    When I attach the file "eicarcom.html" to "field_upload[und][0][resup]" using file resup
    And I wait for the file upload to finish
    Then I should see "A virus has been detected in the file. The file will not be accepted."
    And I press "Save"
    Then I should not see "eicarcom.html"
