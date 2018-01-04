# Config.yml Documentation

## Docker

To modify docker configuration such as MySQL settings, use the following yaml for example:
```
docker:
  mysql_cnf:
    mysqld: #[mysqld] section in my.cnf
      - "max_allowed_packet=256M"
    client: #[client] section in my.cnf]
      - "local_infile=1"
```
