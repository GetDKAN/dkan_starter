Dir.glob("./.ahoy/site/tests/test_*.rb").each {|t| puts `ruby #{t}`}
Dir.glob("./dkan/.ahoy/tests/*.rb").each {|t| puts `ruby #{t}`}
