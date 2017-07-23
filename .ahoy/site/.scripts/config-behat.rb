require './dkan/.ahoy/.scripts/config.rb'
require 'yaml'

class BehatConfig
  def initialize suite = 'dkan', config_path = 'dkan/test/behat.yml'
    @behat_config_path = config_path
    @behat_config = YAML.load_file(@behat_config_path)
    @default_contexts = CONFIG['behat']['contexts']
    @contexts_key = {
      'datasets' => 'Drupal\DKANExtension\Context\DatasetContext',
      'services' => 'Drupal\DKANExtension\Context\ServicesContext'
    }
    @contexts = @behat_config['default']['suites'][suite]['contexts']
  end

  def set context, argument, value
    c_index = @contexts.find_index {|c| c.is_a?(Hash) && c.has_key?(context)}
    return if c_index.nil?
    a_index = @contexts[c_index][context].find_index{|a| a.is_a?(Hash) && a.has_key?(argument)}
    @contexts[c_index][context][a_index][argument].merge!(value)
  end

  def get context, argument
    c_index = @contexts.find_index {|c| c.is_a?(Hash) && c.has_key?(context)}
    return if c_index.nil?
    a_index = @contexts[c_index][context].find_index{|a| a.is_a?(Hash) && a.has_key?(argument)}
    @contexts[c_index][context][a_index][argument]
  end

  def process
    @contexts_key.each {|key, context|
      next if @default_contexts[key].nil?
      @default_contexts[key].each{|argument, value|
        set(context, argument, value)
      }
    }
  end

  def dump
    File.open(@behat_config_path, 'w') do |file|
      file.write(@behat_config.to_yaml.gsub(/- - (.*)/, '- [\1]'))
    end
  end
end

if ENV['PROCESS_CONFIG']
  {
    #'dkan' => 'dkan/test/behat.yml',
    #'dkan_stater' => 'tests/behat.dkan_starter.yml',
    'custom' => 'config/tests/behat.custom.yml'
  }.each {|suite, config_path|
     config = BehatConfig.new suite, config_path
     config.process
     config.dump
   }
end


