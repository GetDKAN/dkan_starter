This details how to update the docker images for DKAN. NuCivic owns the containers but the same steps can be made to create your own images. We will also accept timely PRs with an updated container.

Step-by-step guide
-----------------

Steps to pull, change, commit and push the docker web image:
Please substitute the following in the instructions with the proper details.

.. code-block:: bash

  Container: web
  Project: datastarterprivate
  Team/Owner: nuams
  Image: drupal-apache-php
  
Image Tag/Version to commit: 1.0-5.6 (increment this with every new commit)

1. Create a Docker Hub account at https://hub.docker.com
2. Ask for permissions to access and commit to the nuams team https://hub.docker.com/u/nuams/ . (Create a ticket and contact Pluto to get help with this)
3. docker-image start default
4. git clone nucivic/docker-drupal-apache-php
5. cd docker-drupal-apache-php
6. edit Dockerfile
7. run docker build -t nucivic/drupal-apache-php:v<x>.
8. git clone nucivic/dkan_starter
9. cd dkan_starter
10. edit dkan/.ahoy/docker-compose.yml with new image location
11. ahoy docker up
12. ahoy docker exec web bash
13. Verify things look right (iterate if not)
14. return to docker-drupal-apache-php commit your work and create a PR.
15. Once PR gets reviewed and merged go to docker hub and create a new tag
16. check your new image tag here: https://hub.docker.com/r/nuams/drupal-apache-php/tags/
