# Create a Project Plan

Before you [Create a new project](creating-a-new-project.md) read the following and either sign up for the tools you can use and / or create a plan to replace the default tools you can't.

There are many reasons why teams can't use the tools below and we have worked with many of them that have had successful workflows and deployments.

## Determine Which Tools You Can Use

DKAN Starter is optimized to work with the following:

* [Github](http://github.com)
* [Docker](http://docker.io)
* [AWS S3](https://aws.amazon.com/s3/)
* [ProboCI](http://probo.ci)
* [CircleCI](http://circleci.com)
* [Acquia](http://acquia.com)
* [Jenkins](https://jenkins.io)

You will be best supported if you can use these tools. If you can't use one or more of these tools make sure to plan out how to replace them at the beginning of your project.

## Create Accounts with Tools You Can Use

Create accounts with the tools that you can use in the list above. Docker is the only tool you don't have to create an account for.

See [Creating a New Project](creating-a-new-project.md) for details about each of the services above.

## Create A Project Plan for Tools You Can't Use

If you can't use any of the tools listed above it is imperative that you create a plan for a workaround.

For example if you can't use Github but are authorized to use BitBucket you need to plan to replace the Github and ProboCI and CircleCI integrations. If you cannot use Acquia you need to have a host that has multiple environments and supports Drush aliases. If you can't use a production server with drush aliases you need a way to allow the backup system to still work correctly.

### Github
If you can't use Github you can still use the rest of the tools above. You will need to fire events to Probo and CircleCI for your QA and testing and likely need to change some other items in this toolchain.

### Probo.CI

If you can't use ProboCI you can create other mechanisms for creating Quality Assurance sites. We've been successful doing this in the past by wiring a webook from Github to Jenkins and using our build tools on a bespoke server.

### CirlceCI

If you can't use CircleCI you need a test-runner for verifying that current DKAN tests don't break as you build and to test new functionality. If you have to work within your own data center you can install CirlceCI. CircleCI can also be installed quite easily within an Amazon VPC if you sign up for [Enterprise](https://circleci.com/enterprise/).

### Docker

DKAN has its own fleet of docker containers our developers work with on a daily basis that are optimized for DKAN.

These docker containers were developed with help from [HelioCore](https://heliocore.com/).

If you can't install Docker and Docker Machine on your local machines you need a way to allow developers to work on individual instances of the website. This can be done with other tools like Vagrant or by setting up a cloud VPS. The solution needs to accommodate testing developer branches. This needs to be in place before development starts on the project.

### Jenkins
If you are unable to use Jenkins it is recommended to use a similar task runner. We don't recommend using cron jobs as they don't report errors unless configured specifically to do so.

### AWS S3
If you are unable to use Amazon's S3 for backups you need to setup a place to keep backups that is secure and developers and CI tools can access.

### Acquia
If you are unable to use Acquia there are many hosts that can be substituted or a bespoke hosting platform can be setup, however, it needs to have the following characteristics:

* Git integration
* Drush alias support
* Dev, Stage, and Production environments
* Triggers that fire when code changes in an environment
* Ability to deploy code, database, and file changes between environments
* Ability to withstand expected traffic results for DKAN and client customizations
* Ability to load test the production or replica of the production environment

***Do not start development without a plan to replace these tools if you can't use them. You will create a very expensive level of technical debt very quickly if you aren't writing tests or using DKAN Starter as intended.***

## Project Plan Checklist

You can use [this checklist](https://docs.google.com/spreadsheets/d/167rJVtwfg5S5kBdsbr34i27nDR3-u8Ligh4lypobips/edit#gid=0) as a tool for preparing your Project Plan. Make a copy for your own project.

## Contact Granicus

We would love to hear from you if you are using DKAN Starter. Contact us through the [DKAN channels](https://getdkan.com/community). Let us know your use case, questions or concerns.
