# Visualization Entity Charts
This modules provides the ability to create embedable NVD3 charts.

### Installation 
- Make sure you have [visualization_entity](https://github.com/NuCivic/visualization_entity) module already installed and enabled.

## Install from github zip
```
# cd into your site's docroot and:
mkdir -p sites/all/modules/nucivic/
cd sites/all/modules/nucivic
wget https://github.com/NuCivic/visualization_entity_charts/archive/master.zip
unzip master.zip
mv visualization_entity_charts-master visualization_entity_charts
cd ../../../../
drush make --no-core sites/all/modules/nucivic/visualization_entity_charts/visualization_entity_charts.make
drush -y en visualization_entity_charts
drush cc all
```

## Install from git working copy

```
# cd into your site's docroot and:
mkdir -p sites/all/modules/nucivic/
cd sites/all/modules/nucivic/
git clone git@github.com:NuCivic/visualization_entity_charts.git
cd ../../../../
drush make --no-core sites/all/modules/nucivic/visualization_entity_charts/visualization_entity_charts.make
drush -y en visualization_entity_charts
drush cc all
```

### Usage
TO-DO
