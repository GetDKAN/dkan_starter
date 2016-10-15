Releases
==========

Releases for DKAN Starter are posted on the Github release page:  https://github.com/NuCivic/dkan_starter/releases)

Please read DKAN Starter changelog for keeping up to date as well as the release notes.

We've created a standard for naming DKAN Starter releases:

```
DKAN version -> Data Starter version -> Client Site versioning -> Live Version the client is running
```

So if we've got:
```
1.12.2.4
```

That means:
* *1.12.2* -> dkan's patch release dkan_starter is running
* *.4* -> we've released 4 incremental updates over dkan_starter since we've rebuild data_starter using 1.12.2

Every time we rebuild data_starter using a new version we'll reset the counter to 0. So:
* 1.13.0

Means we just rebuilt data_starter with the ``7.x-1.13`` tag.
