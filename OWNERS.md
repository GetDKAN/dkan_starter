#OWNERS FILE

Purpose
--------
This file is to help clarify which NuCivic teams are responsible for what folder and files in this repository. Internally this helps us to be more efficient and have better quality control.

Teams
------

* **Product** - The DKAN product team, internally at NuCivic called "Mars". Generally responsible for the product itself, releases, and functional tests.
* **DevOps** - The NuCivic DevOps team, internally at NuCivic  called "Pluto". Generally responsile for development environments, testing frameworks, and automation.


Owners
------

## Structure
Pluto owns the structure so can change folders or architecture of Data Starter.

### Pluto
```bash
assets/
config/ 
hooks/common/
tests/
.ahoy.yml
.probo.yml
build.make
circle.yml
run-tests.sh
```

### Mars 
```
projects/modules/
tests/features
README.md
build-dkan.make
drupal-org-core.make
overrides.make
```

### NA
These folders are automated.
```
dkan/
docroot/
```

Implementers
------
Client Implementation can update these on behalf of Product
```
config/
projects/modules/
tests/features/
drupal-org-core.make
overrides.make
```
