<?php

/**
 * @file
 * Contains AcquiaPurgeProcessorsService.
 */

/**
 * Service that loads and bundles queue processor backends.
 */
class AcquiaPurgeProcessorsService {

  /**
   * All processors, ::isEnabled() is called to see if they should load.
   *
   * @var string[]
   */
  protected $backends = array(
    'AcquiaPurgeAjaxProcessor',
    'AcquiaPurgeCronProcessor',
    'AcquiaPurgeRuntimeProcessor',
  );

  /**
   * The loaded backends.
   *
   * @var AcquiaPurgeProcessorInterface
   */
  protected $processors = array();

  /**
   * Event-processor mapping.
   *
   * @var AcquiaPurgeProcessorInterface[]
   */
  protected $events = array();

  /**
   * The Acquia Purge service object.
   *
   * @var AcquiaPurgeService
   */
  protected $service;

  /**
   * Construct a new AcquiaPurgeProcessorsService instance.
   *
   * @param AcquiaPurgeService $service
   *   The Acquia Purge service object.
   */
  public function __construct(AcquiaPurgeService $service) {
    $this->service = $service;

    // Initialize the processors that advertize themselves as enabled.
    foreach ($this->backends as $class) {
      if ($class::isEnabled()) {
        $this->processors[$class] = new $class($this->service);
      }
    }

    // Query the enabled processors for their subscribed events.
    foreach ($this->processors as $processor) {
      foreach ($processor->getSubscribedEvents() as $event) {
        if (!isset($this->events[$event])) {
          $this->events[$event] = array();
        }
        $this->events[$event][] = $processor;
      }
    }
  }

  /**
   * Emit a particular event.
   *
   * @param string $event
   *   Name of the event, often derived from hook implementations found directly
   *   in acquia_purge.module, but could also be 'onItemsQueued'.
   * @param mixed $a1
   *   First optional parameter to pass by reference.
   * @param mixed $a2
   *   Second optional parameter to pass by reference.
   */
  public function emit($event, &$a1 = NULL, &$a2 = NULL) {

    // Don't emit when there are no subscribers.
    if (!isset($this->events[$event])) {
      return;
    }

    // Emit to subscribed processors and pass on any given parameters.
    if (isset($this->events[$event])) {
      foreach ($this->events[$event] as $processor) {
        if (is_null($a1)) {
          $processor->$event();
        }
        elseif (is_null($a2)) {
          $processor->$event($a1);
        }
        else {
          $processor->$event($a1, $a2);
        }
      }
    }
  }

  /**
   * Retrieve a loaded processor object.
   *
   * @param string $class
   *   The name of the processor class you need.
   *
   * @return AcquiaPurgeProcessorInterface|false
   *   Returns the processor, or FALSE when it doesn't exist.
   */
  public function get($class) {
    if (!isset($this->processors[$class])) {
      return FALSE;
    }
    return $this->processors[$class];
  }

}
