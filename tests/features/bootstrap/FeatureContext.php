<?php

use Drupal\DrupalExtension\Context\RawDrupalContext;
use Drupal\DrupalExtension\Context\DrupalContext;
use Behat\Behat\Context\SnippetAcceptingContext;
use Behat\Behat\Hook\Scope\BeforeStepScope;
use Behat\Behat\Hook\Scope\AfterStepScope;
use Behat\Behat\Hook\Scope\BeforeScenarioScope;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Gherkin\Node\TableNode;
use Behat\Behat\Tester\Exception\PendingException;

/**
 * Defines application features from the specific context.
 */
class FeatureContext extends RawDrupalContext implements SnippetAcceptingContext {

  // Store pages to be referenced in an array.
  protected $pages = array();
  protected $groups = array();

  /**
   * Initializes context.
   *
   * Every scenario gets its own context instance.
   * You can also pass arbitrary arguments to the
   * context constructor through behat.yml.
   */
  public function __construct() {
    // Set the default timezone to NY
    date_default_timezone_set('America/New_York');
  }

  /******************************
   * HOOKS
   ******************************/


  /** @AfterStep */
  public function debugStepsAfter(AfterStepScope $scope)
  {
    // Tests tagged with @debugEach will perform each step and wait for [ENTER] to proceed.
    if ($this->scenario->hasTag('debugEach')) {
      $env = $scope->getEnvironment();
      $drupalContext = $env->getContext('Drupal\DrupalExtension\Context\DrupalContext');
      $minkContext = $env->getContext('Drupal\DrupalExtension\Context\MinkContext');
      // Print the current URL.
      try {
        $minkContext->printCurrentUrl();
      }
      catch(Behat\Mink\Exception\DriverException $e) {
        print "No Url";
      }
      $drupalContext->iPutABreakpoint();
    }
  }

  /**
   * @BeforeStep
   */
  public function debugStepsBefore(BeforeStepScope $scope)
  {
    // Tests tagged with @debugBeforeEach will wait for [ENTER] before running each step.
    if ($this->scenario->hasTag('debugBeforeEach')) {
      $env = $scope->getEnvironment();
      $drupalContext = $env->getContext('Drupal\DrupalExtension\Context\DrupalContext');
      $drupalContext->iPutABreakpoint();
    }
  }

  /**
   * @BeforeScenario
   */
   public function registerScenario(BeforeScenarioScope $scope) {
     // Scenario not usually available to steps, so we do ourselves.
     // See issue
     $this->scenario = $scope->getScenario();
     //print  $this->scenario->getTitle();
   }

  /****************************
   * HELPER FUNCTIONS
   ****************************/

  public function addPage($page) {
    $this->pages[$page['title']] = $page;
  }

  /**
   * Get Group by name
   *
   * @param $name
   * @return Group or FALSE
   */
  private function getGroupByName($name) {
    foreach($this->groups as $group) {
      if ($group->title == $name) {
        return $group;
      }
    }
    return FALSE;
  }

  /**
   * Get Group Role ID by name
   *
   * @param $name
   * @return Group Role ID or FALSE
   */
  private function getGroupRoleByName($name) {

    $group_roles = og_get_user_roles_name();

    return array_search($name, $group_roles);
  }

  /**
   * Get Membership Status Code by name
   *
   * @param $name
   * @return Membership status code or FALSE
   */
  private function getMembershipStatusByName($name) {
    switch($name) {
      case 'Active':
        return OG_STATE_ACTIVE;
        break;
      case 'Pending':
        return OG_STATE_PENDING;
        break;
      case 'Blocked':
        return OG_STATE_BLOCKED;
        break;
      default:
        break;
    }

    return FALSE;
  }

  /**
   * Explode a comma separated string in a standard way.
   *
   */
  function explode_list($string) {
    $array = explode(',', $string);
    $array = array_map('trim', $array);
    return is_array($array) ? $array : array();
  }
  /*****************************
   * CUSTOM STEPS
   *****************************/

  /**
   * @Given pages:
   */
  public function addPages(TableNode $pagesTable) {
    foreach ($pagesTable as $pageHash) {
      // @todo Add some validation.
      $this->addPage($pageHash);
    }
  }

  /**
   * @Given I am on (the) :page page
   */
  public function iAmOnPage($page_title)
  {
    if (isset($this->pages[$page_title])) {
      $session = $this->getSession();
      $url = $this->pages[$page_title]['url'];
      $session->visit($this->locatePath($url));
      $code = $session->getStatusCode();

      if ($code < 200 || $code >= 300) {
        throw new Exception("Page $page_title ($url) visited, but it returned a non-2XX response code of $code.");
      }
    }
    else {
      throw new Exception("Page $page_title not found in the pages array, was it added?");
    }

  }

  /**
   * @When I search for :term
   */
  public function iSearchFor($term) {
    $session = $this->getSession();
    $search_form_id = '#dkan-sitewide-dataset-search-form--2';
    $search_form = $session->getPage()->findAll('css', $search_form_id);
    if (count($search_form) == 1) {
      $search_form = array_pop($search_form);
      $search_form->fillField("search", $term);
      $search_form->pressButton("edit-submit--2");
      $results = $session->getPage()->find("css", ".view-dkan-datasets");
      if (!isset($results)) {
        throw new Exception("Search results region not found on the page.");
      }
    }
    else if(count($search_form) > 1) {
      throw new Exception("More than one search form found on the page.");
    }
    else if(count($search_form) < 1) {
      throw new Exception("No search form with the id of found on the page.");
    }
  }

  /**
   * @Then I should see a dataset called :text
   *
   * @throws \Exception
   *   If region or text within it cannot be found.
   */
  public function iShouldSeeADatasetCalled($text)
  {
    $session = $this->getSession();
    $page = $session->getPage();
    $search_region = $page->find('css', '.view-dkan-datasets');
    $search_results = $search_region->findAll('css', '.views-row');

    $found = false;
    foreach( $search_results as $search_result ) {

      $title = $search_result->find('css', 'h2');

      if ($title->getText() === $text) {
        $found = true;
      }
    }

    if (!$found) {
      throw new \Exception(sprintf("The text '%s' was not found", $text));
    }
  }

  /**
   * @Given groups:
   */
  public function addGroups(TableNode $groupsTable)
  {
    // Map readable field names to drupal field names.
    $field_map = array(
      'author' => 'author',
      'title' => 'title',
      'published' => 'published'
    );

    foreach ($groupsTable as $groupHash) {
      $node = new stdClass();
      $node->type = 'group';
      foreach($groupHash as $field => $value) {
        if(isset($field_map[$field])) {
          $drupal_field = $field_map[$field];
          $node->$drupal_field = $value;
        }
        else {
          throw new Exception(sprintf("Group field %s doesn't exist, or hasn't been mapped. See FeatureContext::addGroups for mappings.", $field));
        }
      }
      $created_node = $this->getDriver()->createNode($node);

      // Add the created node to the groups array.
      $this->groups[$created_node->nid] = $created_node;

      // Add the url to the page array for easy navigation.
      $this->addPage(array(
        'title' => $created_node->title,
        'url' => '/node/' . $created_node->nid
      ));
    }
  }

  /**
   * Creates multiple group memberships.
   *
   * Provide group membership data in the following format:
   *
   * | user  | group     | role on group        | membership status |
   * | Foo   | The Group | administrator member | Active            |
   *
   * @Given group memberships:
   */
  public function addGroupMemberships(TableNode $groupMembershipsTable)
  {
    foreach ($groupMembershipsTable->getHash() as $groupMembershipHash) {

      if (isset($groupMembershipHash['group']) && isset($groupMembershipHash['user'])) {

        $group = $this->getGroupByName($groupMembershipHash['group']);
        $user = user_load_by_name($groupMembershipHash['user']);

        // Add user to group with the proper group permissions and status
        if ($group && $user) {

          // Add the user to the group
          og_group("node", $group->nid, array(
            "entity type" => "user",
            "entity" => $user,
            "membership type" => OG_MEMBERSHIP_TYPE_DEFAULT,
            "state" => $this->getMembershipStatusByName($groupMembershipHash['membership status'])
          ));

          // Grant user roles
          $group_role = $this->getGroupRoleByName($groupMembershipHash['role on group']);
          og_role_grant("node", $group->nid, $user->uid, $group_role);

        } else {
          if (!$group) {
            throw new Exception(sprintf("No group was found with name %s.", $groupMembershipHash['group']));
          }
          if (!$user) {
            throw new Exception(sprintf("No user was found with name %s.", $groupMembershipHash['user']));
          }
        }
      } else {
        throw new Exception(sprintf("The group and user information is required."));
      }
    }
  }

  /**
   * @Given datasets:
   */
  public function addDatasets(TableNode $datasetsTable)
  {
    // Map readable field names to drupal field names.
    $field_map = array(
      'author' => 'author',
      'title' => 'title',
      'description' => 'body',
      'publisher' => 'og_group_ref',
      'published' => 'published',
      'resource format' => 'resource format',
      'tags' => 'field_tags',
    );

    foreach ($datasetsTable as $groupHash) {
      $node = new stdClass();
      $node->type = 'group';
      foreach($groupHash as $field => $value) {
        if(isset($field_map[$field])) {
          switch ($field) {

            case 'tags':
            case 'publisher':
              $value = $this->explode_list($value);

          }
          $drupal_field = $field_map[$field];
          $node->$drupal_field = $value;
        }
        else {
          throw new Exception(sprintf("Dataset's field %s doesn't exist, or hasn't been mapped. See FeatureContext::addDatasets for mappings.", $field));
        }
      }
      $created_node = $this->getDriver()->createNode($node);

      // Add the created node to the datasets array.
      $this->datasets[$created_node->nid] = $created_node;

      // Add the url to the page array for easy navigation.
      $this->addPage(array(
        'title' => $created_node->title,
        'url' => '/node/' . $created_node->nid
      ));
    }
  }

  /**
   * @Then I should see :arg1 items in the :arg2 region
   */
  public function iShouldSeeItemsInTheRegion($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the :arg1 detail page
   */
  public function iShouldSeeTheDetailPage($arg1)
  {
    throw new PendingException();
  }

  /**
   * @When I fill in the :arg1 form for :arg2
   */
  public function iFillInTheFormFor($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the list of permissions for the group
   */
  public function iShouldSeeTheListOfPermissionsForTheGroup()
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the list of roles for the group
   */
  public function iShouldSeeTheListOfRolesForTheGroup()
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the list of permissions for :arg1 role
   */
  public function iShouldSeeTheListOfPermissionsForRole($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see :arg1 field
   */
  public function iShouldSeeField($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Given resources:
   */
  public function resources(TableNode $table)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the list with :arg1 order by :arg2
   */
  public function iShouldSeeTheListWithOrderBy($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see :arg1 detail page
   */
  public function iShouldSeeDetailPage($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then I should be redirected to :arg1 sharing page for :arg2
   */
  public function iShouldBeRedirectedToSharingPageFor($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the content in :arg1 format
   */
  public function iShouldSeeTheContentInFormat($arg1)
  {
    throw new PendingException();
  }

  /**
   * @When I press :arg1 in the :arg2 row
   */
  public function iPressInTheRow($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then A file should be downloaded
   */
  public function aFileShouldBeDownloaded()
  {
    throw new PendingException();
  }

  /**
   * @Then I should be able to see the :arg1 detail page
   */
  public function iShouldBeAbleToSeeTheDetailPage($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see :arg1 as :arg2 in the :arg3 row
   */
  public function iShouldSeeAsInTheRow($arg1, $arg2, $arg3)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see a list of unpublished datasets owned by :arg1
   */
  public function iShouldSeeAListOfUnpublishedDatasetsOwnedBy($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then :arg1 user should receive an email
   */
  public function userShouldReceiveAnEmail($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then all :arg1 should receive an email
   */
  public function allShouldReceiveAnEmail($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then I should view the :arg1 content as :arg2
   */
  public function iShouldViewTheContentAs($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the list of revisions
   */
  public function iShouldSeeTheListOfRevisions()
  {
    throw new PendingException();
  }

  /**
   * @When I select :arg1
   */
  public function iSelect($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the revisions diff
   */
  public function iShouldSeeTheRevisionsDiff()
  {
    throw new PendingException();
  }

  /**
   * @Then I should see :arg1 resources
   */
  public function iShouldSeeResources($arg1)
  {
    throw new PendingException();
  }

  /**
   * @When I wait
   */
  public function iWait()
  {
    throw new PendingException();
  }

  /**
   * @When I click :arg1 revision
   */
  public function iClickRevision($arg1)
  {
    throw new PendingException();
  }

  /**s
   * @Then the resource should be reverted
   */
  public function theResourceShouldBeReverted()
  {
    throw new PendingException();
  }

  /**
   * @When I uncheck :arg1 in :arg2
   */
  public function iUncheckIn($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see the :arg1 page
   */
  public function iShouldSeeThePage($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then a notification should be sent to :arg1
   */
  public function aNotificationShouldBeSentTo($arg1)
  {
    throw new PendingException();
  }

  /**
   * @Then I should see :arg1 page
   */
  public function iShouldSeePage($arg1)
  {
    throw new PendingException();
  }


  /**
   * @Given blog_posts:
   */
  public function blogPosts(TableNode $table)
  {
    throw new PendingException();
  }

  /**
   * @Given vocabularies:
   */
  public function vocabularies(TableNode $table)
  {
    throw new PendingException();
  }

  /**
   * @Given terms:
   */
  public function terms(TableNode $table)
  {
    throw new PendingException();
  }

  /**
   * @Then :arg1 should not receive an email
   */
  public function shouldNotReceiveAnEmail($arg1)
  {
    throw new PendingException();
  }

  /**
   * @When I select :arg1 for :arg2
   */
  public function iSelectFor($arg1, $arg2)
  {
    throw new PendingException();
  }

  /**
   * @Then the resource should be reverted
   */
  public function theResourceShouldBeReverted2()
  {
    throw new PendingException();
  }


}
