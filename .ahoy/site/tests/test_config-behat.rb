require "minitest/autorun"
require "./.ahoy/site/.scripts/config-behat.rb"

class TestConfig < MiniTest::Unit::TestCase
  def setup
    CONFIG["behat"]["contexts"]["datasets"] = {"fields" => {}}
    @context = "Drupal\\DKANExtension\\Context\\DatasetContext"
    @config = BehatConfig.new
  end

  def test_default_argument_value
    expected = "title"
    actual = @config.get(@context, "fields")["title"]
    assert_equal expected, actual
  end

  def test_override_argument_value
    @config.set(@context, "fields", {"title" => "foo"})
    expected = "foo"
    actual = @config.get(@context, "fields")["title"]
    assert_equal expected, actual
  end

  def test_add_new_argument_value
    @config.set(@context, "fields", {"bar" => "none"})
    expected = "none"
    actual = @config.get(@context, "fields")["bar"]
    assert_equal expected, actual
  end

  def test_process_smoke
    @config.process
    expected = "title"
    actual = @config.get(@context, "fields")["title"]
    assert_equal expected, actual
  end

  def test_process_overrides
    CONFIG["behat"]["contexts"]["datasets"] = {"fields" => {"title" => "foo"}}
    @config.process
    expected = "foo"
    actual = @config.get(@context, "fields")["title"]
    assert_equal expected, actual
  end

  def test_process_add
    CONFIG["behat"]["contexts"]["datasets"] = {"fields" => {"foo" => "bar"}}
    @config.process
    expected = "bar"
    actual = @config.get(@context, "fields")["foo"]
    assert_equal expected, actual
  end
end
