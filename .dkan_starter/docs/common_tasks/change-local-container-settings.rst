Change local container settings
-------------------------------

Update php {{memory_limit}}:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Locate the php.ini file used for the ahoy based dkan setup, usually dkan/.ahoy/.docker/etc/php5/php.ini
2. Edit the file to set the memory_limit php variable to the needed value, for example (512M, -1 for unlimited).
3. We need to restart the docker containers. ahoy docker stop;; ahoy docker up
