require 'erb'
require 'yaml'

template = File.open("assets/templates/circle.yml.erb", "r").read

class CircleCIConfig
  attr_accessor :temp_dirs
  attr_accessor :memory_limit

  def initialize config
    default_test_dirs = ["tests/features", "dkan/test/features", "config/tests/features"]
    @test_dirs = config["default"]["test_dirs"] ? config["default"]["test_dirs"] : default_test_dirs 
    @memory_limit = config["default"]["memory_limit"] ? config["default"]["memory_limit"] : "256M"
  end

  def render(template)
    ERB.new(template).result(binding)
  end
end

config = YAML.load_file("config/config.yml")
circle_ci_config = CircleCIConfig.new(config)
puts circle_ci_config.render(template);
