<?php
// NuCivic Data Starter
$aliases['data-starter'] = array(
  'parent' => '@nuams_default',
  'site-name' => 'NuCivic Data Starter',
  'git' => array(
    'url' => 'git@github.com:NuCivic/data_starter.git',
    'branch' => ' adding_viz_entity',
  ),
);
$aliases['data-starter.qa'] = array(
  'parent' => '@nudata',
  'root' => '/var/www/data_starter_qa',
  'base_rootroot' => '/var/www/data_starter_qa/docroot',
  'uri' => 'starter.qa',
  'remote-host' => 'renault.nuamsdev.com',
);
