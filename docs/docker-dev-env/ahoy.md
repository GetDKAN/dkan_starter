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

# TODO: enable ahoy v2
Everything below this line is currently not yet available.
---

## Upgrading to ahoy v2
### Rational
Now that dkan_starter is open sourced we need a straight forward way of allowing
ahoy command overrides for  the default setup commands.  This need is satisfied by
switching to using the ahoy binary for  version 2 which has an overriding
feature built in. Thus we are updating the version of ahoy we are using from version
1 to version 2.

### Expected errors
Unfortunately this upgrade is backwards incompatible with ahoy
config files that are for ahoy version 1.  If you try to use ahoy with an
imcompatible set of configs you will see an error like the following:

```
2016/10/31 13:41:11 AHOY! [fatal] ==> Ahoy only supports API version 'v2', but 'v1' given in /Users/dkinzer/docker-share/sites/dkan_starter/.ahoy.yml
panic: AHOY! [fatal] ==> Ahoy only supports API version 'v2', but 'v1' given in /Users/dkinzer/docker-share/sites/dkan_starter/.ahoy.yml
```

Projects that are currently using ahoy version 1 will need to be updated to use
ahoy version 2 by upgrading the project to the latest dkan or dkan_starter.

However, if for some reason this is not possible the developer will
need to switch  ahoy binaries from version 1 to version 2 depending on what config
file version the project is using (see the following sections for details)

### Updating the binary
*  move your current ahoy binary to ahoy1 in the same folder it is (you might
   need it to switch between ahoy1 and ahoy2 projects in the interim).
* get ahoy2: run
```bash
if [ -z ${version+x} ]; then
  version=2.0.0-alpha
fi
os=`uname -s | tr '[:upper:]' '[:lower:]'`
wget  https://nucivic-binaries.s3-us-west-1.amazonaws.com/ahoy/$version/ahoy-$os-amd64 -O ./ahoy-$version && \
        chmod +x ./ahoy-$version
```
* move this new copy of ahoy to where the old ahoy binary was.
* symlink ahoy to this binary `ln -s /usr/local/bin/ahoy-2.0.0-alpha /usr/local/bin/ahoy`

### Switching between versions
To switch between the two versions of ahoy you need only update the symlink to
point to the version of the ahoy binary that you require.

Make sure that you use ahoy1 binary with ahoy1 config files and ahoy2 binaries
with ahoy2 config files or you will see the error above.
