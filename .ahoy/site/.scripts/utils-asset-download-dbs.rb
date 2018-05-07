require "./dkan/.ahoy/.scripts/config.rb"

`ahoy cmd-proxy exec mkdir -p backups`
site = `ahoy utils name`
db = ARGV[0]
aws_url = CONFIG["private"]["aws"]["scrubbed_data_url"]
asset = "#{aws_url}/#{site}.prod.#{db}.sql.gz"

`LC_TIME=en_US.UTF-8 perl .ahoy/site/.scripts/s3curl.pl --id local #{asset} > backups/#{db}.sql.gz`

puts ""
puts "Unpacking the $db database."
puts ""

`ahoy cmd-proxy gunzip backups/#{db}.sql.gz -f`
`ahoy cmd-proxy cp -f backups/#{db}.sql backups/last_install.sql`
