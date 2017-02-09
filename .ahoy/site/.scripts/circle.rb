require 'erb'
require 'yaml'

template = File.open("assets/templates/circle.yml.erb", "r").read

class CircleCIConfig
  attr_accessor :temp_dirs
  attr_accessor :memory_limit

  def initialize config
    default_test_dirs = ["tests/features", "dkan/test/features", "config/tests/features"]

    if !config["circle"] || !config["circle"]["test_dirs"]
      @test_dirs = default_test_dirs
    else
      @test_dirs = config["circle"]["test_dirs"]
    end

    if !config["circle"] || !config["circle"]["memory_limit"]
      @memory_limit = "256M"
    else
      @memory_limit = config["circle"]["memory_limit"]
    end
  end

  def render(template)
    ERB.new(template).result(binding)
  end
end

config = YAML.load_file("config/config.yml")
circle_ci_config = CircleCIConfig.new(config)
File.write('circle.yml', circle_ci_config.render(template))
