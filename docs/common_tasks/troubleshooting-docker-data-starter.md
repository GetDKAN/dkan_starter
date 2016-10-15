### I get "ENVIRONMENT set to , but not mapped in settings.php"

* Your "settings.docker.php" file is not set. Run **oy site setup**
* Can also be caused by the mysl container missing:
```
ahoy diagnose all
ahoy docker up
```

### I get "Could not open connection: Curl error thrown for http POST to http://browser:4444/wd/hub/session" when running tests

* Your browser container is not running. Run **ahoy diagnose all** to troubelshoot

### ERROR 1064 (42000) at line 1: You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'env: drush9: No such file or directory' at line 1
[error] The command could not be executed successfully (returned: env: drush9: No such file or directory
, code: 127)

* Make sure you are using drush version 8.0.2 (not 9).

### 'ahoy dkan test...' stalls on a step or very slow	

* Do a complete cleanup:
```
ahoy docker cleanup
ahoy docker stop
docker-machine stop default
docker-machine start default
ahoy docker up
```
