<?php

/**
 * @file
 * Contains SearchApiAcquiaSearchMultiService.
 */

/**
 * Provides automatic environment switching for Acquia Search servers.
 */
class SearchApiAcquiaSearchMultiService extends SearchApiAcquiaSearchService {

  /**
   * {@inheritdoc}
   */
  public function connect() {
    if (!$this->solr) {
      // Set our special overrides, if applicable.
      $this->setConnectionOptions();
      parent::connect();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function setConnectionOptions() {
    $has_id = (isset($this->options['acquia_override_subscription']['acquia_override_subscription_id'])) ? true : false;
    $has_key = (isset($this->options['acquia_override_subscription']['acquia_override_subscription_key'])) ? true : false;
    $has_corename = (isset($this->options['acquia_override_subscription']['acquia_override_subscription_corename'])) ? true : false;

    if ($has_id && $has_key && $has_corename) {
      $identifier = $this->options['acquia_override_subscription']['acquia_override_subscription_id'];
      $key = $this->options['acquia_override_subscription']['acquia_override_subscription_key'];
      $corename = $this->options['acquia_override_subscription']['acquia_override_subscription_corename'];

      $this->options['path'] = '/solr/' . $corename;
      // Set the derived key for this environment.
      $subscription = $this->getAcquiaSubscription($identifier, $key);
      $derived_key_salt = $subscription['derived_key_salt'];
      $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $corename, $key);
      $this->options['derived_key'] = $derived_key;

      // Get and set our search core hostname.
      $search_host = acquia_search_multi_subs_get_hostname($corename);
      $this->options['host'] = $search_host;
    }
    else {
      parent::setConnectionOptions();
    }
  }

  /**
   * Overrides SearchApiAcquiaSearchService::configurationForm().
   *
   * Adds configuration for switching the Solr server, either automatically
   * based on the environment or manually.
   *
   * @see acquia_search_multi_subs_get_settings_form()
   */
  public function configurationForm(array $form, array &$form_state) {
    $form = parent::configurationForm($form, $form_state);

    // Only allow overriding of the connection information with our form.
    $form['modify_acquia_connection']['#access'] = FALSE;
    $form['modify_acquia_connection']['#default_value'] = FALSE;
    $form['host']['#access'] = FALSE;
    $form['path']['#access'] = FALSE;

    // Get our common settings form.
    $configuration = isset($this->options['acquia_override_subscription']) ? $this->options['acquia_override_subscription'] : array();
    acquia_search_multi_subs_get_settings_form($form, $form_state, $configuration);
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function viewSettings() {
    // If Search API 1.10+ is used, this method is deprecated in favor of
    // getExtraInformation().
    if (method_exists('SearchApiAbstractService', 'getExtraInformation')) {
      return NULL;
    }
    $output = parent::viewSettings();

    // Set our special overrides, if applicable.
    $this->setConnectionOptions();

    $options = $this->options;
    $auto_detection = (isset($options['acquia_override_subscription']['acquia_override_auto_switch']) && $options['acquia_override_subscription']['acquia_override_auto_switch']);
    $auto_detection_state = ($auto_detection) ? t('enabled') : t('disabled');
    $output .= "<dl>\n  <dt>";
    $output .= t('Acquia Search Auto Detection');
    $output .= "</dt>\n  <dd>";
    $output .= t('Auto detection of your environment is <strong>@state</strong>', array('@state' => $auto_detection_state));
    $output .= '</dd>';
    $output .= "\n</dl>";

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function getExtraInformation() {
    $auto_detection = (!isset($this->options['acquia_override_subscription']['acquia_override_auto_switch']) || $this->options['acquia_override_subscription']['acquia_override_auto_switch']);
    $auto_detection_state = ($auto_detection) ? t('enabled') : t('disabled');
    $info[] = array(
      'label' => t('Acquia Search Auto Detection'),
      'info' => t('Auto detection of your environment is <strong>@state</strong>.', array('@state' => $auto_detection_state)),
    );

    $this->setConnectionOptions();
    $info = array_merge($info, parent::getExtraInformation());
    return $info;
  }

  /**
   * Overrides SearchApiSolrService::configurationFormSubmit().
   *
   * If auto detection is not on, changes our search core name to the one that
   * was inputted.
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
    parent::configurationFormSubmit($form, $values, $form_state);

    // If we do not have auto switch enabled, statically configure the right
    // core to options.
    $has_id = (isset($form_state['values']['acquia_override_subscription_id'])) ? true : false;
    $has_key = (isset($form_state['values']['acquia_override_subscription_key'])) ? true : false;
    $has_corename = (isset($form_state['values']['acquia_override_subscription_corename'])) ? true : false;
    $has_auto_switch = !empty($form_state['values']['acquia_override_auto_switch']) ? true : false;

    if (!$has_auto_switch && $has_id && $has_key && $has_corename) {
      $identifier = $form_state['values']['acquia_override_subscription_id'];
      $key = $form_state['values']['acquia_override_subscription_key'];
      $corename = $form_state['values']['acquia_override_subscription_corename'];

      // Set our solr path
      $this->options['path'] = '/solr/' . $corename;

      // Set the derived key for this environment
      $subscription = $this->getAcquiaSubscription($identifier, $key);
      $derived_key_salt = $subscription['derived_key_salt'];
      $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $corename, $key);
      $this->options['derived_key'] = $derived_key;

      $search_host = acquia_search_multi_subs_get_hostname($corename);
      $this->options['host'] = $search_host;
    }
  }

  /**
   * Get subscription info from the acquia_connector module, and cache it for
   * 5 minutes.
   *
   * @param $acquia_identifier
   * @param $acquia_key
   * @return array
   *   Subscription data
   */
  protected function getAcquiaSubscription($acquia_identifier, $acquia_key) {
    $subscription_cache = &drupal_static(__FUNCTION__, array());
    // Get subscription and use cache.
    $cid = 'asms-subscription-' . $acquia_identifier . ':' . $acquia_key;
    if (isset($subscription_cache[$cid])) {
      $subscription = $subscription_cache[$cid];
    }
    else {
      $cached = cache_get($cid);
      if ($cached && $cached->data && REQUEST_TIME < $cached->expire) {
        $subscription = $cached->data;
        $subscription_cache[$cid] = $subscription;
      }
    }
    if (empty($subscription)) {
      $subscription = acquia_agent_get_subscription(array(), $acquia_identifier, $acquia_key);
      $subscription_cache[$cid] = $subscription;
      // Cache this for 5 minutes.
      cache_set($cid, $subscription, 'cache', REQUEST_TIME + 300);
    }
    return $subscription;
  }
}
