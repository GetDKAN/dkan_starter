require 'erb'
require 'yaml'

template = File.open("assets/templates/circle.yml.erb", "r").read

class CircleCIConfig
  attr_accessor :temp_dirs
  attr_accessor :memory_limit
  attr_accessor :parallelism

  def initialize config
    default_test_dirs = ["tests/features", "dkan/test/features", "config/tests/features"]

    if !config["circle"] || !config["circle"]["test_dirs"]
      @test_dirs = default_test_dirs
    else
      @test_dirs = config["circle"]["test_dirs"]
    end

    if !config["circle"] || !config["circle"]["memory_limit"]
      @memory_limit = "512M"
    else
      @memory_limit = config["circle"]["memory_limit"]
    end

    if !config["circle"] || !config["circle"]["parallelism"]
      @parallelism = "5"
    else
      @parallelism = config["circle"]["parallelism"]
    end

    default_skip_tags = [ "customizable", "fixme", "testBug"]
    if !config["circle"] || !config["circle"]["skip_tags"]
      @skip_tags = process_skip_tags(default_skip_tags)
    else
      @skip_tags = process_skip_tags(config["circle"]["skip_tags"])
    end
  end

  def render(template)
    ERB.new(template).result(binding)
  end

  private

  def process_skip_tags(tags)
    tags.map {|w| "~@#{w}" }
  end
end

config = YAML.load_file("config/config.yml")
circle_ci_config = CircleCIConfig.new(config)
File.write('.circleci/config.yml', circle_ci_config.render(template))
