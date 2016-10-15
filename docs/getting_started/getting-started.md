# Create a Project Plan

Before you [[Create a new project]] read the following and either sign up for the tools you can use and / or create a plan to replace the default tools you can't.

There are many reasons why teams can't use the tools below and we have worked with many of them that have had successful workflows and deployments.

## Determine Which Tools You Can User

DKAN Starter is optimized to work with the following:

* Github
* Docker
* AWS S3
* ProboCI
* CircleCI
* Acquia

You will be best supported if you can use these tools. If you can't use one or more of these tools make sure to plan out how to replace them at the beginning of your project.

## Create Accounts with Tools You Can Use

Create accounts with the tools that you can use in the list above. Docker is the only tool you don't have to create an account for.

## Create Alternative Workflows with for the Tools You Can't Use

If you can't use these tools you can create overrides by overriding ahoy.

### Overriding Ahoy

Currently to override ahoy you need to copy the commands in ``nucivic-ahoy/site.ahoy.yml`` to ``config/custom.ahoy.yml`` and change the commands that access tools you can't use. We are working on updating this workflow to make it easier to override individual commands so aren't further documenting this soon to updated process.

For example if you can't use S3 or you want to store copies of your pruned and sanitized databases elsewhere you can overwrite ``ahoy site asset-download-db`` and put in your own logic for getting the database you want to work with locally.

### Github
If you can't use Github you can still use the rest of the tools above. You will need to fire events to Probo and CircleCI for your QA and testing and likely need to change some other items in this toolchain.

### Probo.CI

If you can't use ProboCI you can create other mechanisms for creating Quality Assurance sites. We've been successful doing this in the past by wiring a webook from Github to Jenkins and using our build tools on a bespoke server.

*Do not start development without a plan to replace these tools if you can't use them. You will create a very expensive level of technical debt very quickly if you aren't writing tests or using DKAN Starter as intended.*

## Contact GovDelivery

We would love to hear from you if you are using DKAN Starter. Contact us through the DKAN channels: https://github.com/NuCivic/dkan#getting-help-with-dkan or email us at ``aaron.couch (at) govdelivery.com``. Let us know your use case, questions or concerns.
