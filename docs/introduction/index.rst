DKAN Starter Overview
=============

DKAN Starter is part of a workflow that starts with a feature request from a client, moves to a local development environment where a developer or team of developers implements the feature, through the testing and quality assurance steps, and results in that feature being deployed to a production environment.

DKAN Starter itself is a git repository though is optimized for the following tools:

* *Github* for VCS and project management
* *CircleCI* for tests
* *Ahoy* for simplifying our CLI commands
* *Probo* for quality assurance
* *Docker* for local development
* *Acquia* for hosting and deployment

All of these tools can be switched out by updating the ahoy commands that implement each part of the workflow.

## ``Config/``

All of the development for DKAN Starter implementers is done in the ``config/`` folder. Everything outside of the ``config/`` folder will be overwritten after every update. GovDelivery provides updates to DKAN Starter immediately after DKAN patch or minor releases or after an update to DKAN Starter. Implementers can simply run the command ``ahoy build update DKAN-STARTER-VERSION`` to get the latest version of DKAN and DKAN Starter.


.. toctree::
   :maxdepth: 1

   dkan-starter-annotated
   getting-started
   creating-a-new-project
   releases
