<?php
// Plugin definition
$plugin = array(
  'title' => t('DDH Custom Front'),
  'category' => t('Custom'),
  'icon' => 'ddh_full_width.png',
  'theme' => 'ddh_full_width',
  'css' => 'ddh_full_width.css',
  'regions' => array(
    'top-first' => t('Top First Column'),
    'top-second' => t('Top Second Column'),
    'top-full' => t('Top Full'),
    'middle' => t('Middle Row'),
    'bottom-first' => t('Bottom First Column'),
    'bottom-second' => t('Bottom Second Column'),
    'bottom-third' => t('Bottom Third Column'),
    'footer' => t('Footer'),
  ),
);

