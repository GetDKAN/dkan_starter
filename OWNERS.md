#OWNERS FILE

Purpose
--------
This file is to help clarify which NuCivic teams are responsible for what folder and files in this repository. Internally this helps us to be more efficient and have better quality control.

Teams
------

* **Product** - The DKAN product team, internally at NuCivic called "Mars". Generally responsible for the product itself, releases, and functional tests.
* **Engineerng** - The NuCivic Engineering team, internally at NuCivic  called "Pluto". Generally responsile for development environments, testing frameworks, and automation.


Owners
------

## Structure
Pluto owns the structure so can change folders or architecture of Data Starter.

### Engineering
Pluto owns most of the folders and files that are part of the automation.

- `DEFAULT`

### Product 

- `projects/modules/` (Any non-DKAN OOB modules + custom_config module)
- `tests/features` (Any non-DKAN OOB features)
- `build-dkan.make` (Modules added to project)
- `drupal-org-core.make` (Drupal core version)
- `overrides.make` (Overrides of DKAN or DKAN contrib modules)

### NA
These folders are automated.
```
dkan/
docroot/
```

Implementers
------
Client Implementation can update these on behalf of Product

- `config/`
- `projects/modules/`
- `tests/features/`
- `drupal-org-core.make`
- `overrides.make`
