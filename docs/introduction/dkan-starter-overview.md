DKAN Starter Overview
=============

DKAN Starter provides two main functions:

1. Tools for an Agile development workflow
2. Tools for updating DKAN, Drupal, and Contributed Drupal modules and 3rd party libraries

Agile Workflow
-------------

DKAN Starter is part of an agile workflow for creating DKAN sites. That workflow walks through the following stages:

1. Feature Request
   * A feature request is created from a stakeholder
2. User Stories
   * The feature is translated into user stories
3. Feature Implementation
   * Behat tests created with the user stories
   * Feature branch created to work on the feature
   * Developers use their local development environment to work on the feature
   * A pull request is created when the feature is ready for review
4. Quality Assurance
   * Passing tests are verified on the pull request
   * Project Owner, Project Manager, or stakeholder verifies the feature is complete

DKAN Starter provides integrations with the following tools to facilitate this workflow:

<<<<<<< HEAD
* **[Github](http://github.com)** for VCS and project management
* **[CircleCI](http://circleci.com)** for tests
* **[Ahoy](https://github.com/DevinciHQ/ahoy)** for simplifying our CLI commands
* **[Probo](http://probo.ci)** for quality assurance
* **[Docker](http://docker.io)** for local development
* **[Acquia](http://acquia.com)** for hosting and deployment
=======
* **[Github](http://github.com>)** for VCS and project management
* **[CircleCI](http://circleci.com>)** for tests
* **[Ahoy](https://github.com/DevinciHQ/ahoy>)** for simplifying our CLI commands
* **[Probo](http://probo.ci>)** for quality assurance
* **[Docker](http://docker.io>)** for local development
* **[Acquia](http://acquia.com>)** for hosting and deployment
>>>>>>> Updates docs

All of these tools can be switched out by updating the ahoy commands that implement each part of the workflow.

DKAN Updates
--------

DKAN Starter serves as an upstream for client projects. DKAN Starter is updated with a new release as soon as a new version of DKAN is released. DKAN releases updates frequently as patch releases for security updates and bug fixes.

Without proper management updating a Drupal or DKAN site quickly can be very difficult. DKAN Starter provides a single command to create an updated branch when a DKAN version is released and tests and quality assurance sites to quickly verify updates are successful.

Config/
-------
All of the development for DKAN Starter implementers is done in the ``config/`` folder. Everything outside of the ``config/`` folder will be overwritten after every update. GovDelivery provides updates to DKAN Starter immediately after DKAN patch or minor releases or after an update to DKAN Starter. Implementers can simply run the command ``ahoy build update DKAN-STARTER-VERSION`` to get the latest version of DKAN and DKAN Starter.
