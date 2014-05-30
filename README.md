# NuData Starter

## Purpose

This "Starterkit" is designed to facilitate building off of DKAN while keeping up-to-date with DKAN changes. The Starkerkit allows developers and site administrators the ability to get upstream changes to DKAN using a "git subtree merge" strategy.

This is useful if you want to:

- Get regular changes from the DKAN development branch.
- Have a strategy for getting future DKAN tags.
- Follow a best-practice for developing sites with DKAN.

Do not use this Starterkit if you:

- Want to use another strategy for updating DKAN or Drupal core (ie manually).
- Do not like "git subtree" or can't use it.

## Requirements

- git subtree
- drush buildmanager https://github.com/WhiteHouse/buildmanager
- drush subtree https://github.com/whitehouse/drushsubtree

## Instructions

- Clone this repository.
- Add desired modules to "build.make" file.
- Run ``drush buildmanager-build`` to get an updated version of DKAN that is coupled with your contributed modules.

#### Staying Up-to-date with Tags Only

If you desire to stay up to date with release tags instead of DKAN development releases:

- change the "branch" to the desired tag in buildmanager.config.yml
- run `` drush buildmanager-build ``

## Props
Thanks to the folks at Acquia and Whitehouse who developed this technique. For more details see: http://www.acquia.com/blog/maintaining-your-installed-drupal-distro