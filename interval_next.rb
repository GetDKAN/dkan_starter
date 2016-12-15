require 'thread'

start_time = Time.now.to_i
duration = 15 * 60
interval = 18
one_minute_left = duration - 60

[
  Thread.new {
    while true
      current_time = Time.now.to_i
      time_lapsed = current_time - start_time
      if  time_lapsed < interval
        `say start`
        sleep interval
      elsif time_lapsed > duration
        `say stop`
        exit
      else
        `say next`
        sleep interval
      end
    end
  },

  Thread.new {
    while true
      current_time = Time.now.to_i
      time_lapsed = current_time - start_time
      if time_lapsed > one_minute_left
        `say one minute`
        break
      end
    end
  }
].each do |th| th.join() end

