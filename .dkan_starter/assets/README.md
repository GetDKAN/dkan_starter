Assets
======

The directory structure of assets mirrors docroot.

When managing a project with
Drush Make or automating your Drush Make builds with Build Manager, everything
should have a canonical home OUTSIDE the docroot. That canonical home can be a
separate repo outside you site repository (which is the case for all versioned
projects referenced in make files), or it can be inside your site repo somewhere
other that the docroot. To keep things simple and intuitive, here's the
guideline we follow in this site repository:
 
**Anything in your code base (1) whose canonical home is INSIDE your site repo
and (2) which is not a project belongs in the assets directory.**