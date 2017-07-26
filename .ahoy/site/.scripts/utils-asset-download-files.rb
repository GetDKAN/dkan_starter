require "./dkan/.ahoy/.scripts/config.rb"

`ahoy cmd-proxy exec mkdir -p backups`
site = `ahoy utils name`
aws_url = CONFIG["private"]["aws"]["scrubbed_data_url"]
asset = "#{aws_url}/#{site}.prod.files.tar.gz"

`LC_TIME=en_US.UTF-8 perl .ahoy/site/.scripts/s3curl.pl --id local #{asset} > backups/ #{site}.prod.files.tar.gz`

puts ""
puts "Unpacking the files asset."
puts ""

`tar xvzf backups/#{site}.prod.files.tar.gz`
