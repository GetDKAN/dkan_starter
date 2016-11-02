# Installing from a Backup

<div class="note">

This is a workflow for installing a site if you only have a git repository and a database dump file. This is often the case if you are requesting a production site or if you receive a backup from a vendor.

This workflow does not include continuous integration (testing, QA sites) because the user does not have access to Github or the database backups.

</div>

## Prerequisites

The following are required for this tutorial.

1. MySQL database
   * This should be a sanitized version of the production database.
2. Project Repository
   * This should be the full repository and be identical to DKAN Starter except for the ``config/`` folder.

## Step by Step Installation

### Install Docker

Follow the instructions at [Local Docker Development Environment](../docker-dev-env/index.rst) to setup your local development environment.

### Start Local Containers

Run ``ahoy docker up`` from the root of your project folder.

### Add database backup

Run ``ahoy drush sqlc < DATABASE-BACKUP-NAME.sql``. Change "DATABASE-BACKUP-NAME" for the name of your database file.

### Access Site

Run ``ahoy drush uli`` and you should get a URL for your site.
