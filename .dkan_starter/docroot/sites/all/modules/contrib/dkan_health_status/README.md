# DKAN Health Status Check

This as adopted from http://drupal.org/project/health but slipped down. The UI and cron were removed and the base module is just 125 lines. 

### Add health checks

To add health checks see ``dkan_health_status.checks.inc``. Implement ``hook_dkan_health_status_monitors``. The key in your array will provide the health check id as well as a an endpoint ``/health/dkan/check_key``.  

### Setup

You mush add ``$conf['dkan_health_status_health_api_access_key'] = "API_KEY"`` to ``settings.php``. The API_KEY is used to request the check: ``/health/dkan/check_key?key=API_KEY``
