require "./dkan/.ahoy/.scripts/config.rb"

puts "Running drush rr --no-cache-clear"
drush_alias = ENV['drush_alias']
target_env = ENV['targe_env']

`drush @#{drush_alias} rr --no-cache-clear`

puts "Truncating cache table"
`drush @#{drush_alias} sqlq "TRUNCATE cache;"`

puts "Running database update"
`drush @#{drush_alias} updatedb -y`

puts "Clearing caches"
`drush @#{drush_alias} cc all`

puts "Checking drupal boostrap."
drupal=`drush @#{drush_alias} status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//'`

if  drupal =~ /Successful/
  puts "Installation detected, running deploy script"
  `drush @#{drush_alias} en custom_config -y`
  `drush @#{drush_alias} cc all`
  `drush @#{drush_alias} -y fr --force custom_config`
  `drush @#{drush_alias} env-switch #{target_env} --force`
  `drush @#{drush_alias} -y updb`

  db_based_search=`drush @#{drush_alias} pmi dkan_acquia_search_solr | grep disabled`

  if db_based_search.nil? and db_based_search.empty?
    puts "SOLR Search, avoiding indexing data"
  else
    puts "DB Search, indexing data"
    `drush @#{drush_alias} search-api-index datasets`
    `drush @#{drush_alias} search-api-index groups_di`
    `drush @#{drush_alias} search-api-index stories_index`
  end
else
  puts "Installation not detected"
end

# Extra non-acquia steps.
if target_env  =~ /local/
  `ahoy dkan create-qa-users`

  password = CONFIG["private"]["probo"]["password"]

  if  ENV["CI"] === "true"
    password = "admin"
  end

  `drush --root=docroot user-password 1 --password="#{password}`
  `drush --root=docroot user-password admin --password="#{password}`
end
