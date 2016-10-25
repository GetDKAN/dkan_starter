# Setting up Your Local Docker

The steps to setup the DKAN development environment are as following:
1. Run the universal Docker installer (recommended) and/or run the commands manually to make sure that the Docker Machine is properly setup.
2. Make sure the Docker Machine is started
3. Clone DKAN, install it in Docker Machine and start developing.
Clone a Data_starter project, install it in Docker Machine and start developing.
4. Universal Docker Installer

## 1. Universal Docker Installer

The universal Docker installer is available in the following repository https://github.com/NuCivic/universal-docker-installer
Check the Readme on the Github repository to setup the docker development environment.

### 1.1 Setup the installer dependencies

https://github.com/NuCivic/universal-docker-installer#installer-dependencies

### 1.2 Environment setup
https://github.com/NuCivic/universal-docker-installer#usage

### 1.3 Troubleshooting
https://github.com/NuCivic/universal-docker-installer#troubleshooting

## 2. Docker machine
Before starting to work on DKAN sites the developer needs to make sure that the development docker machine is up and running as following:
1. docker-machine status default
2. docker-machine start default
When The developer is done developing DKAN, the docker machine could be checked and stopped as following:
1. docker-machine status default
2. docker-machine stop default
To manage the default Virtualbox machine (the docker machine), a developer can use the following commands:
1. Check the machine status: docker-machine status default
2. List the existing docker machines: docker-machine ls
3. Start the default machine: docker-machine start default
4. Stop the default machine: docker-machine stop default
5. Check the default machine IP address: docker-machine ip default
6. SSH into the default machine: docker-machine ssh default
For a detailed docker-machine command line reference check the following link: https://docs.docker.com/machine/reference/

## 3. Getting started with DKAN development
To get started with DKAN development you need to follow these steps:
1. cd ~/docker
2. git clone git@github.com:NuCivic/dkan.git
3. If you don't have you ssh key added to github do the following instead: git clone https://github.com/NuCivic/dkan.git
4. cd dkan
5. bash dkan-init.sh dkan
6. ahoy docker up
7. ahoy dkan drupal-rebuild mysql://drupal:123@db/drupal
8. ahoy dkan remake
9. ahoy dkan reinstall
10. ahoy docker url
11. ahoy docker vnc (to get the vnc url and use it with any vncviewer. The password is secret).
12. ahoy dkan test

Visit the DKAN site url to be sure the site is up and reachable.
In the dkan folder you can see the following directories:

1. dkan: The git repository of dkan profile
2. docroot: The docroot with a fresh Drupal installation and a symlink from docroot/profiles/dkan to dkan (git repository)
3. backups: Contains SQL backup/dump file (last_install.sql) for the last DKAN site reinstall.

Make changes to dkan, add, commit and push.
When done developing for this project execute the following command: ahoy docker stop
When done developing with Docker Machine for any DKAN related project execute the following command: docker-machine stop default

## 4. Getting started with DKAN Starter development
See: [Setting up a project locally](../common_tasks/setting-up-local-project)
