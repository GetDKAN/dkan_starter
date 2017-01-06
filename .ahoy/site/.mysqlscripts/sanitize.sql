-- 
-- Scrub important information from a Drupal database.
-- 

-- Remove all email addresses.
UPDATE users SET mail=CONCAT('user', uid, '@example.com'), init=CONCAT('user', uid, '@example.com') WHERE uid != 0;

-- Example: Disable a module by setting its system.status value to 0.
-- UPDATE system SET status = 0 WHERE name = 'securepages';

-- Example: Update or delete variables via the variable table.
-- DELETE FROM variable WHERE name='secret_key';
-- Note that to update variables the value must be a properly serialized php array.
-- UPDATE variable SET value='s:24:"http://test.gateway.com/";' WHERE name='payment_gateway';

-- IMPORTANT: If you change the variable table, clear the variables cache.
-- DELETE FROM cache WHERE cid = 'variables';

-- Scrub url aliases for non-admins since these also reveal names
-- Add the IGNORE keyword, since a user may have multiple aliases, and without
-- this keyword the attempt to store duplicate dst values causes the query to fail.
-- UPDATE IGNORE url_alias SET dst = CONCAT('users/', REPLACE(src,'/', '')) WHERE src IN (SELECT CONCAT('user/', u.uid) FROM users u WHERE u.uid NOT IN (SELECT uid FROM users_roles WHERE rid=3) AND u.uid > 0);

-- don't leave e-mail addresses, etc in comments table.
-- UPDATE comments SET name='Anonymous', mail='', homepage='http://example.com' WHERE uid=0;

-- Scrub webform submissions.
-- UPDATE webform_submitted_data set data='*scrubbed*';

-- remove sensitive customer data from custom module
-- TRUNCATE custom_customer_lead_data;

-- USER PASSWORDS
-- Drupal 7 requires sites to generate a hashed password specific to their site. A script in the 
-- docroot/scripts directory is provided for doing this. From your docroot run the following:
--      
--    scripts/password-hash.sh password
--
-- this will generate a hash for the password "password". In the following statements replace
-- $REPLACE THIS$ with your generated hash.

UPDATE users SET pass = '$S$DAIFIwaPZLsfNqEZkYY1Wklf6TIQal1uObBpIPk1UDUsc6qgPi6b' WHERE uid IN (SELECT uid FROM users_roles WHERE rid=3) AND uid > 0;
UPDATE users SET name = 'admin' WHERE uid = 1;
UPDATE users SET name = 'admin' WHERE name = 'administrator';
UPDATE users SET pass = '$S$DAGmJJr.MOF1M6wTF/YEU6yBchL5kkAvaMGgvXQtVVJyD4KXmc5G'  WHERE uid = 1;
UPDATE users set name = md5(name) where name <> 'admin' AND uid <> 0;

TRUNCATE flood;
TRUNCATE history;
TRUNCATE sessions;

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `watchdog` (
  `wid` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary Key: Unique watchdog event ID.',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT 'The users.uid of the user who triggered the event.',
  `type` varchar(64) NOT NULL DEFAULT '' COMMENT 'Type of log message, for example "user" or "page not found."',
  `message` longtext NOT NULL COMMENT 'Text of log message to be passed into the t() function.',
  `variables` longblob NOT NULL COMMENT 'Serialized array of variables that match the message string and that is passed into the t() function.',
  `severity` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'The severity level of the event; ranges from 0 (Emergency) to 7 (Debug)',
  `link` varchar(255) DEFAULT '' COMMENT 'Link to view the result of the event.',
  `location` text NOT NULL COMMENT 'URL of the origin of the event.',
  `referer` text COMMENT 'URL of referring page.',
  `hostname` varchar(128) NOT NULL DEFAULT '' COMMENT 'Hostname of the user who triggered the event.',
  `timestamp` int(11) NOT NULL DEFAULT '0' COMMENT 'Unix timestamp of when event occurred.',
  PRIMARY KEY (`wid`),
  KEY `type` (`type`),
  KEY `uid` (`uid`),
  KEY `severity` (`severity`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Table that contains logs of all system events.';
/*!40101 SET character_set_client = @saved_cs_client */;
TRUNCATE watchdog;

SELECT concat('TRUNCATE TABLE ', TABLE_NAME, ';') FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE 'access%';

SELECT concat('TRUNCATE TABLE ', TABLE_NAME, ';') FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE 'devel_%';

SELECT concat('TRUNCATE TABLE ', TABLE_NAME, ';') FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE 'cache%';

SELECT concat('TRUNCATE TABLE ', TABLE_NAME, ';') FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE 'migrate_%';

SELECT concat('TRUNCATE TABLE ', TABLE_NAME, ';') FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE 'search_%';
