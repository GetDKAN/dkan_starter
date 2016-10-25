# Ahoy

<img src="https://camo.githubusercontent.com/3c5f11de213d31ad57f73be305d05f37d3feade3/687474703a2f2f6936352e74696e797069632e636f6d2f76717277676c2e706e67" alt="ahoy logo"/>

[Ahoy](https://github.com/DevinciHQ/ahoy) is a small open source tool for creating cli apps like drush, drupal-console, or custom bash scripts. Written in Go and compiled down to a single binary, it has no dependencies which make it fast and easy to install.

See [Ahoy's documentation](https://github.com/DevinciHQ/ahoy) for more details.

## Why Ahoy?
We use ahoy to help us do the following:
* Standardize workflow steps so that everyone is doing things exactly the same way
* Abstract more complex workflow steps and scripts behind higher level commands, which:
* Makes it easier for developers, as they can see a list of commands available along with descriptions of each
* Makes it easier for the devops team to improve the processes without having to retrain developers or change documentation
* Reuse abstracted commands in scripts like CircleCI, or even other ahoy commands.
* Make it easier for anyone to add or tweak the commands without knowledge of bash, php, etc
* Allow projects (or users) to have their own custom commands in addition to the defaults.

## Available commands
Ahoy is self documenting. Type ``ahoy`` to see available commands. Using these commands is detailed throughout this documentation.

## DKAN and Ahoy

DKAN's ahoy commands are stored in the ``.ahoy/`` folder in DKAN.

## DKAN Starter and Ahoy

DKAN Starter ahoy commands are stored in the ``.ahoy/sites/`` folder in DKAN.

## Upgrading to ahoy v2
*  move your current ahoy binary to ahoy1 in the same folder it is (you might
   need it to switch between ahoy1 and ahoy2 projects in the interim).
* get ahoy2: run
```bash
if [ -z ${version+x} ]; then
  version=2.0.1-alpha
fi
os=`uname -s | tr '[:upper:]' '[:lower:]'`
wget  https://nucivic-binaries.s3-us-west-1.amazonaws.com/ahoy/$version/ahoy-$os-amd64 -O ./ahoy && \
        chmod +x ./ahoy
```
* (OR)
```bash
curl https://gist.githubusercontent.com/dkinzer/862b6191964389b1bb8cd5deda936880/raw/02107a496d89fbe1dc9bf6c2fc3b7c6946f847de/get-ahoy.sh | bash
```
* move this new copy of ahoy to where the old ahoy binary was.
