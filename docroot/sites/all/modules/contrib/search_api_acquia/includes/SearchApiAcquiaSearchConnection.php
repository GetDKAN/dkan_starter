<?php

/**
 * Starting point for the Solr API. Represents a Solr server resource and has
 * methods for pinging, adding, deleting, committing, optimizing and searching.
 *
 */
class SearchApiAcquiaSearchConnection extends SearchApiSolrConnection {

  /**
   * The derived key used to HMAC hash the search request.
   *
   * @var string
   */
  protected $derivedKey;

  /**
   * Creates an authenticator based on a data string and HMAC-SHA1.
   *
   * @see acquia_search_authenticator()
   */
  function authenticator($string, $nonce, $derived_key = NULL) {
    if (empty($derived_key)) {
      $derived_key = $this->getDerivedKey();
    }
    if (empty($derived_key)) {
      // Expired or invalid subscription - don't continue.
      return '';
    }
    else {
      $time = time();
      $hash = hash_hmac('sha1', $time . $nonce . $string, $derived_key);
      return 'acquia_solr_time=' . $time . '; acquia_solr_nonce=' . $nonce . '; acquia_solr_hmac=' . $hash . ';';
    }
  }

  /**
   * Sets the derived key used to HMAC hash the search request.
   *
   * @param string $derived_key
   *   The derived key.
   */
  public function setDerivedKey($derived_key) {
    $this->derivedKey = $derived_key;
  }

  /**
   * Derive a key for the solr hmac using the information shared with
   * acquia.com.
   *
   * @see _acquia_search_derived_key()
   */
  public function getDerivedKey() {
    if (!isset($this->derivedKey)) {
      $key = acquia_agent_settings('acquia_key');
      $subscription = acquia_agent_settings('acquia_subscription_data');
      $identifier = acquia_agent_settings('acquia_identifier');
      // We use a salt from acquia.com in key derivation since this is a shared
      // value that we could change on the AN side if needed to force any
      // or all clients to use a new derived key.  We also use a string
      // ('solr') specific to the service, since we want each service using a
      // derived key to have a separate one.
      if (empty($subscription['active']) || empty($key) || empty($identifier)) {
        // Expired or invalid subscription - don't continue.
        $this->derivedKey = '';
      }
      else {
        $salt = isset($subscription['derived_key_salt']) ? $subscription['derived_key_salt'] : '';
        $derivation_string = $identifier . 'solr' . $salt;
        $this->derivedKey =  hash_hmac('sha1', str_pad($derivation_string, 80, $derivation_string), $key);
      }
    }
    return $this->derivedKey;
  }

  /**
   * Send an optimize command.
   *
   * We want to control the schedule of optimize commands ourselves,
   * so do a method override to make ->optimize() a no-op.
   *
   * @see SearchApiSolrConnection::optimize()
   */
  public function optimize($waitFlush = true, $waitSearcher = true, $timeout = 3600) {
    return TRUE;
  }

  /**
   * Modify the url and add headers appropriate to authenticate to Acquia Search.
   *
   * @return
   *  The nonce used in the request.
   */
  public function prepareRequest(&$url, &$options, $use_data = TRUE) {
    // Add a unique request ID to the URL.
    $id = uniqid();
    if (!stristr($url,'?')) {
      $url .= "?";
    }
    else {
      $url .= "&";
    }
    $url .= 'request_id=' . $id;
    // If we're hosted on Acquia, and have an Acquia request ID,
    // append it to the request so that we map Solr queries to Acquia search requests.
    if (isset($_ENV['HTTP_X_REQUEST_ID'])) {
      $xid = empty($_ENV['HTTP_X_REQUEST_ID']) ? '-' : $_ENV['HTTP_X_REQUEST_ID'];
      $url .= '&x-request-id=' . rawurlencode($xid);
    }
    if ($use_data && isset($options['data'])) {
      list($cookie, $nonce) = $this->authCookie($url, $options['data']);
    }
    else {
      list($cookie, $nonce) = $this->authCookie($url);
    }
    if (empty($cookie)) {
      throw new Exception('Invalid authentication string - subscription keys expired or missing.');
    }
    $options['headers']['Cookie'] = $cookie;
    $options['headers'] += array('User-Agent' => 'search_api_acquia/'. variable_get('search_api_acquia_version', '7.x'));
    $options['context'] = acquia_agent_stream_context_create($url, 'acquia_search');
    if (!$options['context']) {
      throw new Exception(t("Could not create stream context"));
    }
    return $nonce;
  }

  /**
   * Validate the hmac for the response body.
   *
   * @return
   *  The response object.
   */
  public function authenticateResponse($response, $nonce, $url) {
    $hmac = $this->extractHmac($response->headers);
    if (!$this->validResponse($hmac, $nonce, $response->data)) {
      throw new Exception('Authentication of search content failed url: '. $url);
    }
    return $response;
  }

  /**
   * Look in the headers and get the hmac_digest out.
   *
   * @see acquia_search_extract_hmac()
   */
  protected function extractHmac($headers) {
    $reg = array();
    if (is_array($headers)) {
      foreach ($headers as $name => $value) {
        if (strtolower($name) == 'pragma' && preg_match("/hmac_digest=([^;]+);/i", $value, $reg)) {
          return trim($reg[1]);
        }
      }
    }
    return '';
  }

  /**
   * Validate the authenticity of returned data using a nonce and HMAC-SHA1.
   *
   * @return boolean
   *  TRUE or FALSE depending on whether the response is valid.
   *
   * @see acquia_search_valid_response()
   */
  protected function validResponse($hmac, $nonce, $string, $derived_key = NULL) {
    if (empty($derived_key)) {
      $derived_key = $this->getDerivedKey();
    }
    return $hmac == hash_hmac('sha1', $nonce . $string, $derived_key);
  }

  /**
   * Make a request to a servlet (a path) that's not a standard path.
   *
   * @override
   */
  public function makeServletRequest($servlet, array $params = array(), array $options = array()) {
    // Add default params.
    $params += array(
      'wt' => 'json',
    );

    $url = $this->constructUrl($servlet, $params);
    // We assume we only authenticate the URL for other servlets.
    $nonce = $this->prepareRequest($url, $options, FALSE);
    $response = $this->makeHttpRequest($url, $options);
    $response = $this->checkResponse($response);
    return $this->authenticateResponse($response, $nonce, $url);
  }

  /**
   * Central method for making a GET operation against this Solr Server
   *
   * @override
   */
  protected function sendRawGet($url, array $options = array()) {
    $nonce = $this->prepareRequest($url, $options);
    $response = $this->makeHttpRequest($url, $options);
    $response = $this->checkResponse($response);
    return $this->authenticateResponse($response, $nonce, $url);
  }

  /**
   * Central method for making a POST operation against this Solr Server
   *
   * @override
   */
  protected function sendRawPost($url, array $options = array()) {
    $options['method'] = 'POST';
    // Normally we use POST to send XML documents.
    if (!isset($options['headers']['Content-Type'])) {
      $options['headers']['Content-Type'] = 'text/xml; charset=UTF-8';
    }
    $nonce = $this->prepareRequest($url, $options);
    $response = $this->makeHttpRequest($url, $options);
    $response = $this->checkResponse($response);
    return $this->authenticateResponse($response, $nonce, $url);
  }

  /**
   * Modify a solr base url and construct a hmac authenticator cookie.
   *
   * @param $url
   *  The solr url beng requested - passed by reference and may be altered.
   * @param $string
   *  A string - the data to be authenticated, or empty to just use the path
   *  and query from the url to build the authenticator.
   * @param $derived_key
   *  Optional string to supply the derived key.
   *
   * @return
   *  An array containing the string to be added as the content of the
   *  Cookie header to the request and the nonce.
   *
   * @see acquia_search_auth_cookie
   */
  function authCookie(&$url, $string = '', $derived_key = NULL) {
    $uri = parse_url($url);

    // Add a scheme - should always be https if available.
    if (in_array('ssl', stream_get_transports(), TRUE) && !defined('ACQUIA_DEVELOPMENT_NOSSL')) {
      $scheme = 'https://';
      $port = '';
    }
    else {
      $scheme = 'http://';
      $port = (isset($uri['port']) && $uri['port'] != 80) ? ':'. $uri['port'] : '';
    }
    $path = isset($uri['path']) ? $uri['path'] : '/';
    $query = isset($uri['query']) ? '?'. $uri['query'] : '';
    $url = $scheme . $uri['host'] . $port . $path . $query;

    // 32 character nonce.
    $nonce = base64_encode(drupal_random_bytes(24));

    if ($string) {
      $auth_header = $this->authenticator($string, $nonce, $derived_key);
    }
    else {
      $auth_header = $this->authenticator($path . $query, $nonce, $derived_key);
    }
    return array($auth_header, $nonce);
  }

}
