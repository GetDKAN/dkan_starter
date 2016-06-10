<?php

/**
 * @file
 * Hooks provided by the advanced page expiration module.
 */

/**
 * Alter the max_age before being set.
 *
 * This hook is called right before the cache header is set. It can therefore be
 * used to set the cache header regardless of any other state in the system,
 * including whether caching is enabled or a user is logged in.
 *
 * For that reason, it is important that the function ape_check_cacheable() is
 * used if the standard caching logic should still be respected.
 *
 * @param int $max_age
 *   Set this to the new desired value.
 * @param int $original_max_age
 *   The original set value so it can be used for comparison.
 */
function hook_ape_cache_expiration_alter(&$max_age, $original_max_age) {
  // Set cache lifetime to 10 minutes if on front page and cacheable.
  if (!ape_check_cacheable()) {
    return;
  }
  if (drupal_is_front_page() && $original_max_age < 400) {
    $max_age = 400;
  }
}
