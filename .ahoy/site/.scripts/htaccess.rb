require 'erb'
require 'yaml'

htaccess_template = File.open("assets/templates/.htaccess.erb", "r").read

class Htaccess
  attr_accessor :https_everywhere
  attr_accessor :redirect_hosts
  attr_accessor :prod_host

  def initialize config
    @redirect_hosts =  config["redirectDomains"] ? config["redirectDomains"] : []
    @https_everywhere = config["default"]["https_everywhere"] ? config["default"]["https_everywhere"] : false
    @prod_host = config["default"]["hostname"]
  end

  def render(template)
    ERB.new(template).result(binding)
  end
end

config = YAML.load_file("config/config.yml")
htaccess = Htaccess.new(config)
puts htaccess.render(htaccess_template);
