<?php

/**
 * @file
 * Contains SearchApiAcquiaSearchService.
 */


/**
 * Search API service class for Acquia Search.
 */
class SearchApiAcquiaSearchService extends SearchApiSolrService {

  /**
   * The connection class used by this service.
   *
   * Must implement SearchApiSolrConnectionInterface.
   *
   * @var string
   */
  protected $connection_class = 'SearchApiAcquiaSearchConnection';

  /**
   * Create a connection to the Solr server as configured in $this->options.
   */
  protected function connect() {
    $this->setConnectionOptions();

    if (!$this->solr) {
      if (!class_exists($this->connection_class)) {
        throw new SearchApiException(t('Invalid class @class set as Solr connection class.', array('@class' => $this->connection_class)));
      }
      $options = $this->options + array('server' => $this->server->machine_name);


      $this->solr = new $this->connection_class($options);
      if (!($this->solr instanceof SearchApiSolrConnectionInterface)) {
        $this->solr = NULL;
        throw new SearchApiException(t('Invalid class @class set as Solr connection class.', array('@class' => $this->connection_class)));
      }
    }

    // allow the connection to override the derived key
    if (isset($this->options['derived_key'])) {
      $this->solr->setDerivedKey($this->options['derived_key']);
    }
  }

  /**
   * View this server's settings.
   */
  public function viewSettings() {
    $output = '';

    // Set our special overrides if applicable
    $this->setConnectionOptions();

    $options = $this->options;
    $url = $options['scheme'] . '://' . $options['host'] . ':' . $options['port'] . $options['path'];
    $output .= "<dl>\n  <dt>";
    $output .= t('Acquia Search Server');
    $output .= "</dt>\n  <dd>";
    $output .= $url;
    $output .= '</dd>';
    if ($options['http_user']) {
      $output .= "\n  <dt>";
      $output .= t('Basic HTTP authentication');
      $output .= "</dt>\n  <dd>";
      $output .= t('Username: @user', array('@user' => $options['http_user']));
      $output .= "</dd>\n  <dd>";
      $output .= t('Password: @pass', array('@pass' => str_repeat('*', strlen($options['http_pass']))));
      $output .= '</dd>';
    }
    $output .= "\n</dl>";

    return $output;
  }

  /**
   * Set some special overrides for Acquia Search
   */
  public function setConnectionOptions() {
    // Modify connection details live on every connect so we do not need to
    // resave the server details if we make modifications in settings.php.
    $identifier = acquia_agent_settings('acquia_identifier');
    $subscription = acquia_agent_settings('acquia_subscription_data');

    // Get our override if we have one. Otherwise use the default.
    $search_host = variable_get('acquia_search_host', 'search.acquia.com');
    if (!empty($subscription['heartbeat_data']['search_service_colony'])) {
      $search_host = $subscription['heartbeat_data']['search_service_colony'];
    }

    // Get our solr path
    $search_path = variable_get('acquia_search_path', '/solr/' . $identifier);

    $this->options['host'] = $search_host;
    $this->options['path'] = $search_path;

    // We can also have overrides per server setting.
    // Apply the overrides in the "search_api_acquia_overrides" variable.
    $name = $this->server->machine_name;
    $overrides = variable_get('search_api_acquia_overrides', array());
    if (isset($overrides[$name]) && is_array($overrides[$name])) {
      $this->options = array_merge($this->options, $overrides[$name]);
    }
  }

  /**
   * Overrides SearchApiSolrService::configurationForm().
   *
   * Populates the Solr configs with Acquia Search Information.
   */
  public function configurationForm(array $form, array &$form_state) {
    $form = parent::configurationForm($form, $form_state);

    // Set our special overrides if applicable
    $this->setConnectionOptions();

    $options = $this->options += array(
      'edismax' => 0,
      'modify_acquia_connection' => FALSE,
      'scheme' => 'http',
    );

    // HTTP authentication is not needed since Acquia Search uses an HMAC
    // authentication mechanism.
    $form['http']['#access'] = FALSE;

    $form['edismax'] = array(
      '#type' => 'checkbox',
      '#title' => t('Always allow advanced syntax for Acquia Search'),
      '#default_value' => $options['edismax'],
      '#description' => t('If enabled, all Acquia Search keyword searches may use advanced <a href="@url">Lucene syntax</a> such as wildcard searches, fuzzy searches, proximity searches, boolean operators and more via the Extended Dismax parser. If not enabled, this syntax wll only be used when needed to enable wildcard searches.', array('@url' => 'http://lucene.apache.org/java/2_9_3/queryparsersyntax.html')),
      '#weight' => -30,
    );

    $form['modify_acquia_connection'] = array(
      '#type' => 'checkbox',
      '#title' => 'Modify Acquia Search Connection Parameters',
      '#default_value' => $options['modify_acquia_connection'],
      '#description' => t('Only check this box if you are absolutely certain about what you are doing. Any misconfigurations will most likely break your site\'s connection to Acquia Search.'),
      '#weight' => -20,
    );

    // Disable any port configuration option as Acquia will always be in
    //control of those ports
    $form['port']['#access'] = FALSE;
    // Disable the http method that is selected as Acquia will always be https
    // unless ACQUIA_DEVELOPMENT_NOSSL was set.
    $form['scheme']['#access'] = FALSE;

    $form['clean_ids_form']['#weight'] = 10;

    // Re-sets defaults with Acquia information.
    $form['host']['#default_value'] = $options['host'];
    $form['path']['#default_value'] = $options['path'];

    // Only display fields if we are modifying the connection parameters to the
    // Acquia Search service.
    $states = array(
      'visible' => array(
        ':input[name="options[form][modify_acquia_connection]"]' => array('checked' => TRUE),
      ),
    );
    $form['host']['#states'] = $states;
    $form['path']['#states'] = $states;

    if ($this->options['scheme'] == 'https') {
      $this->options['port'] = '443';
    }
    else {
      $this->options['port'] = '80';
    }

    // We cannot connect directly to the Solr instance, so don't make it a link.
    if (isset($form['server_description'])) {
      $url = $this->options['scheme'] . '://' . $this->options['host'] . ':' . $this->options['port'] . $this->options['path'];
      $form['server_description'] = array(
        '#type' => 'item',
        '#title' => t('Acquia Search URI'),
        '#description' => check_plain($url),
        '#weight' => -40,
      );
    }

    return $form;
  }

  /**
   * Overrides SearchApiSolrService::configurationFormValidate().
   *
   * Forces defaults if the override option is unchecked.
   *
   * @see http://drupal.org/node/1852692
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    $modified = !empty($form_state['values']['options']['form']['modify_acquia_connection']);
    if (!$modified) {

      // Set our special overrides if applicable
      $this->setConnectionOptions();

      form_set_value($form['host'], $this->options['host'], $form_state);
      form_set_value($form['port'], $this->options['port'], $form_state);
      form_set_value($form['path'], $this->options['path'], $form_state);

    }
    parent::configurationFormValidate($form, $values, $form_state);
  }

  /**
   * Overrides SearchApiSolrService::preQuery().
   *
   * Sets the eDisMax parameters if certain conditions are met, adds the default
   * parameters that are usually set in Search API's solrconfig.xml file.
   */
  protected function preQuery(array &$call_args, SearchApiQueryInterface $query) {
    $params = &$call_args['params'];

    // Bails if this is a 'mlt' query or something else custom.
    if (!empty($params['qt']) || !empty($params['defType'])) {
      return;
    }

    // The Search API module adds default "fl" parameters in solrconfig.xml
    // that are not present in Acquia Search's solrconfig.xml file. Add them
    // and others here as a backwards compatible solution.
    // @see http://drupal.org/node/1619770
    $params += array(
      'echoParams' => 'none',
      'fl' => 'item_id,score',
      'q.op' => 'AND',
      'q.alt' => '*:*',
      'spellcheck' => 'false',
      'spellcheck.onlyMorePopular' => 'true',
      'spellcheck.extendedResults' => 'false',
      'spellcheck.count' => '1',
      'hl' => 'false',
      'hl.fl' => 'spell',
      'hl.simple.pre' => '[HIGHLIGHT]',
      'hl.simple.post' => '[/HIGHLIGHT]',
      'hl.snippets' => '3',
      'hl.fragsize' => '70',
      'hl.mergeContiguous' => 'true',
    );

    // Set the qt to eDisMax if we have keywords and either the configuration
    // is set to always use eDisMax or the keys contain a wildcard (* or ?).
    $keys = $query->getOriginalKeys();
    if ($keys && is_scalar($keys) && (($wildcard = preg_match('/\S+[*?]/', $keys)) || $this->options['edismax'])) {
      $params['defType'] = 'edismax';
      if ($wildcard) {
        // Converts keys to lower case, reset keys in query and replaces param.
        $new_keys = preg_replace_callback('/(\S+[*?]\S*)/', array($this, 'toLower'), $keys);
        $query->keys($new_keys);
        $call_args['query'] = $new_keys;
      }
    }
  }

  /**
   * Convert to lower-case any keywords containing a wildcard.
   *
   * @see _acquia_search_lower()
   */
  public function toLower($matches) {
    return drupal_strtolower($matches[1]);
  }
}
