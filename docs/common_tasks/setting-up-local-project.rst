Setting up a project locally
----------------------------

1. Install docker as described in `this guide <../docker-dev-env/installation.html>`_
2. Run 

.. code-block:: bash

  docker-machine start default; eval "$(docker-machine env default)"

if you haven't already

3. cd into ``~/docker-share/CLIENT-GIT-REPO``
4. Setup S3 backup access
  1. Get S3 credentials
  2. Create aws credentials file
      1. ``vim ~/.s3curl``
      2. ``chmod 600 ~/.s3curl`` # make file so only owner can read and write
      3. Add: 
      
      .. code-block:: bash
      
         %awsSecretAccessKeys = (
            local => {
              id => 'AWS_ID',
              key => 'AWS_KEY',
            }
          );

5. Bring up docker containers:

.. code-block:: bash

   ahoy docker up
   
6. Setup your site

.. code-block:: bash

   ahoy site up
   
7. Get the site URL

.. code-block:: bash

   ahoy docker url
   

Troubleshooting
===============

s3curl
~~~~~~

The asset download command which is called from `ahoy ci setup` uses an AWS perl script.  This script usually will work without issues, however sometimes you may need to download a missing Perl module.  For example:

If you run into the following error:

.. code-block:: bash

   ahoy utils asset-download-db
   Can't locate Digest/HMAC_SHA1.pm in @INC (you may need to install the Digest::HMAC_SHA1 module) (@INC contains: /usr/local/Cellar/perl/5.24.0_1/lib/perl5/site_perl/5.24.0/darwin-thread-multi-2level /usr/local/Cellar/perl/5.24.0_1/lib/perl5/site_perl/5.24.0 /usr/local/Cellar/perl/5.24.0_1/lib/perl5/5.24.0/darwin-thread-multi-2level /usr/local/Cellar/perl/5.24.0_1/lib/perl5/5.24.0 /usr/local/lib/perl5/site_perl/5.24.0 .) at nucivic-ahoy/.scripts/s3curl.pl line 20.
   BEGIN failed--compilation aborted at nucivic-ahoy/.scripts/s3curl.pl line 20.

then, do this:

.. code-block:: bash

   perl -MCPAN -e "install Digest::HMAC_SHA1;"

of course the specific module will depend on your error.

Hostname/alias errors
~~~~~~~~~~~~~~~~~~~~~

The `ahoy ci setup` command will fail if you do not have the Acquia aliases set up correctly on your local environment. Make sure you are logged into Acquia (drush ac-api-login) then update your Acquia aliases (drush acquia-update).
