require 'yaml'

unless [].respond_to? :to_h
  class Array
    def to_h
      Hash[self]
    end
  end
end

def sort_merge config, config_example
  if config.nil?
    return config_example
  end

  if config_example == {} and config == {}
    return {}
  end

  if config_example == {}
    return sort_merge(config_example, config)
  end

  if config.instance_of? Hash and config_example.instance_of? Hash
    config_example = config_example.map { |k,v|
      if v.instance_of? Hash and config[k].nil?
        [k, v.sort.to_h]
      else
        [k, sort_merge(config[k], v)]
      end
    }.to_h
  else
    if config.instance_of? Array
      return config.sort
    else
      return config
    end
  end

  return config_example.sort.to_h
end

def main
  config_path = File.dirname(__FILE__) + "/../../../config/config.yml"
  puts config_path
  config_example_path = File.dirname(__FILE__) + "/../../../config/example.config.yml"

  if File.readable? config_path
    config = YAML::load(File::read(config_path))
  else
    config = {}
  end

  if File.readable? config_example_path
    config_example = YAML::load(File::read(config_example_path))
  else
    config_example = {}
  end

  if File.writable? config_path
    File.open(config_path, "w") do |file|
      file.write(YAML::dump(sort_merge(config, config_example)))
      file.close
    end

  end
end

if ENV["PROCESS_CONFIG"]
  main
end

