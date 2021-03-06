# time:1m55.91s
@api @disablecaptcha @datastore
Feature: Datastore
  In order to know the datastore is working
  As a website user
  I need to be able to add and remove items from the datastore

  Background:
    Given users:
      | name    | mail                | roles           |
      | Gabriel | gabriel@example.com | content creator |
      | Katie   | katie@example.com   | site manager    |
      | Daniel  | daniel@example.com  | content creator |
      | Jaz     | editor@example.com  | editor          |
    Given groups:
      | title    | author  | published |
      | Group 01 | Katie   | Yes       |
      | Group 02 | Katie   | Yes       |
    And group memberships:
      | user    | group    | role on group        | membership status |
      | Gabriel | Group 01 | member               | Active            |
      | Jaz     | Group 01 | administrator member | Active            |
      | Daniel  | Group 02 | member               | Active            |
    Given datasets:
      | title      | publisher | author  | published | description |
      | Dataset 01 | Group 01  | Gabriel | Yes       | Test        |
      | Dataset 02 | Group 02  | Daniel  | Yes       | Test        |
    And "Format" terms:
      | name    |
      | csv     |
    And resources:
      | title       | publisher | format | dataset    | author  | published | description               | link file |
      | Resource 01 | Group 01  | csv    | Dataset 01 | Gabriel | Yes       | The resource description. | https://s3.amazonaws.com/dkan-default-content-files/files/datastore-simple.csv |
      | Resource 02 | Group 02  | csv    | Dataset 02 | Daniel  | Yes       | The resource description. | https://s3.amazonaws.com/dkan-default-content-files/files/datastore-simple2.csv |

  @api
  Scenario: Anonymous users should not be able to manage datastores
    Given I am an anonymous user
    Then I "should not" be able to manage the "Resource 01" datastore

  @api @javascript
  Scenario: Content Creators should be able to manage only datastores
  associated with the resources they own
    Given I am logged in as "Gabriel"
    Then I "should" be able to manage the "Resource 01" datastore
    Given I am logged in as "Daniel"
    Then I "should not" be able to manage the "Resource 01" datastore

  @api
  Scenario: Editors should be able to manage only datastores associated with
  resources created by members of their groups
    Given I am logged in as "Jaz"
    Then I "should" be able to manage the "Resource 01" datastore
    And I "should not" be able to manage the "Resource 02" datastore

  @api
  Scenario: Site Managers should be able to manage any datastore
    Given I am logged in as "Katie"
    Then I "should" be able to manage the "Resource 01" datastore
    And I "should" be able to manage the "Resource 02" datastore 
