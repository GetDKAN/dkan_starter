require "./dkan/.ahoy/.scripts/config.rb"

name=`ahoy utils name`

if name =~ /datastarter/
  puts "Site name datastarter. Going with DKAN base install"
  `ahoy site reinstall`
else
  puts "Site name set. Pulling database from s3 bucket"
  `ahoy utils s3-setup`

  # Configure https settings.
  stage_file_proxy_origin = CONFIG["default"]["stage_file_proxy_origin"]
  if  !stage_file_proxy_origin.empty?  && stage_file_proxy_origin != "changeme"
    `ahoy utils asset-download-db`
    `ahoy utils asset-unpack-db sanitized`
  else
    `ahoy utils asset-download`
    `ahoy utils files-link`
  end

  `ahoy utils files-fix-permissions`

  # Old database should've been cleaned in the ahoy site reinstall step.
  `ahoy drush sql-cli < backups/sanitized.sql`
end
