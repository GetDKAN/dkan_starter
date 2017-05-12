require 'yaml'

path = File.expand_path("/home/ubuntu/dkan_dash/dkan/test/behat.yml")
data = YAML::load_file(path)
data["default"]["suites"]["dkan"]["contexts"].push("Drupal\\DKANExtension\\Context\\DkanDashContext")
File.open(path, "w") {|f| f.write data.to_yaml }
