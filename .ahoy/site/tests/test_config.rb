require "minitest/autorun"
require "./.scripts/config.rb"


class TestConfig < MiniTest::Unit::TestCase
  def test_sort_merge_empy_cofig_empty_example
    config_example = {}
    config = {}
    expected = {}
    actual = sort_merge(config, config_example)
    assert_equal expected.to_a, actual.to_a
  end

  def test_sort_merge_empty_example
    config_example = {}
    config = { "x" => {"b" => 2, "a" => 3},
               "z" => 1, "b" => 2}
    result = sort_merge config, config_example
    expected = {"a" => 3, "b" => 2}
    actual = result["x"]
    assert_equal expected.to_a, actual.to_a
  end

  def test_sort_merge_array_value
    config_example = { "a" => [], "z" => 1, "b" => 2}
    config = { "a" => [], "z" => 1, "b" => 2}

    expected = {"a" => [], "b" => 2, "z" => 1}
    actual = sort_merge config, config_example
    assert_equal expected, actual
  end

  def test_sort_merge_inner_array_value
    config_example = { "a" => { "x" => "a"}, "z" => 1, "b" => 2}
    config = { "a" => { "x" => "a"}, "z" => 1, "b" => 2}

    expected = {"a" => { "x" => "a"}, "b" => 2, "z" => 1}
    actual = sort_merge config, config_example
    assert_equal expected, actual
  end

  def test_sort_merge_sorts
    config_example = {"z" => 1, "b" => 2, "a" => 3}
    config = {}

    expected = {"a" => 3, "b" => 2, "z" => 1}
    actual = sort_merge config, config_example
    assert_equal expected.to_a, actual.to_a
  end

  def test_sort_merge_merges_toplevel
    config_example = {"z" => 10, "b" => 10, "a" => 3}
    config = {"z" => 1, "b" => 2}

    expected = {"z" => 1, "b" => 2, "a" => 3}
    actual = sort_merge config, config_example
    assert_equal expected, actual
  end

  def test_sort_merge_sorts_second_level
    config_example = { "x" => {"z" => 1, "b" => 2, "a" => 3},
               "z" => 1, "b" => 2}
    config = { "x" => {"z" => 1, "b" => 2, "a" => 3},
               "z" => 1, "b" => 2}
    result = sort_merge config, config_example
    expected = {"a" => 3, "b" => 2, "z" => 1}
    actual = result["x"]
    assert_equal expected.to_a, actual.to_a
  end

  def test_sort_merge_merges_second_level
    config_example = { "x" => {"z" => 1, "b" => 2, "a" => 3},
               "z" => 1, "b" => 2}
    config = { "x" => {"b" => 2, "a" => 3},
               "z" => 1, "b" => 2}
    result = sort_merge config, config_example
    expected = {"a" => 3, "b" => 2, "z" => 1}
    actual = result["x"]
    assert_equal expected.to_a, actual.to_a
  end
end


