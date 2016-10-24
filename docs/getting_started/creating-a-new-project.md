# Creating a New Project

## Github

To create a new project simply copy the DKAN Starter github repository and create a new project in Github.

## ProboCI

### Sign up

[Sign up](http://probo.ci) for a ProboCI account. Enable the project you just created in Github by clicking "Active Repos" in ProboCI and selecting your poject.

### Add Assets

Add the [S3Curl](https://github.com/rtdp/s3curl/blob/master/s3curl.pl) script as an asset. To do so click "Build Assets" in a project and upload the file. See:

<img width="1263" alt="Screenshot of ProboCI with S3curl updloaded in Build Assets tab" src="https://cloud.githubusercontent.com/assets/512243/19433741/f22316f0-942f-11e6-99b0-270cfc47fc3d.png">

## CircleCI

### Sign up

[Sign up](http://circleci.com) for a CircleCI account. Enable the project you just created in Github by clicking "Add Projects" and following the instructions.

### Project Settings

Configure the following settings in the CirlceCI project settings section.

#### Build Environment

Select ``Ubuntu 14.04 (Trusty)`` as the Ubuntu version.

#### Adjust Parallelism
We recommend at least 3x.

#### Environmental Variables
Add an AWS Key and ID that will allow you to retrieve S3 files from you AWS account. If you have a different solution for storing and retrieving backups.

## AWS

Sign up for an Amazon Web Services account. Create a user that has access to access private S3 buckets.

Update your repository to include the AWS S3 url in the ``config/config.yml`` file.

## Jenkins

We use Jenkins for our automated tasks including backups. We employ Jenkins jobs for the following:

* Backups to S3
* Acquia purge job
* Cron runs
* Data.json caching
* Datastore queue

We will share the actual jobs for this soon.

At the least you need a mechanism for providing pruned and sanitized backups for your site to S3 or another mechanism to do so.

You can simply setup the ``ahoy utils asset-download-db`` job to download directly from your production instance for local development, ProboCI, and CirlceCI however we recommend using a pruned and sanitized version of your database for these services.

## Custom Site Configuration

There are a number of configuration steps that are captured in the ``config/`` folder.

For details see [Custom Site Configuration](custom-site-configruation).

## Docker

See the [Docker](docker-dev-env/installation) for details on setting up and using Docker.

Once you have setup the rest of these services, developers simply need to run:

```bash
$ docker-machine start default; eval "$(docker-machine env default)"
```

This sets starts docker.

```bash
$ ahoy docker up
```

Running this inside your project docroot starts the docker containers for your project.

```bash
$ ahoy site up
```

Running this inside your project docroot:

* Adds ``docroot/sites/default/setting.docker.php`` which adds settings to connect your DKAN site to your docker containers
* Downloads a local copy of your database in the ``backups/`` folder
* Downloads a local copy of your files if setup in ``config/config.yml``
* Runs an "Environment switch" and sets your environment to "local"
* Runs other deployment tasks as defined in ``hooks/commands`` and ``config/settings.custom.php``
