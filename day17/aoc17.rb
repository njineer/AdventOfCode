#!/usr/bin/env ruby

if __FILE__ == $0
    if ARGV.length < 1
        puts "missing step size."
    else
        step = ARGV[0].to_i
        circle_buffer = [0]
        current = 0

        (1..2017).each do |i|
            current = ((current + step) % i) + 1
            circle_buffer.insert(current, i)
        end

        index2017 = circle_buffer.find_index(2017)
        #puts "2017 @ #{circle_buffer.find_index(2017)}"
        puts circle_buffer[(index2017 + 1) % circle_buffer.length]

        current = 0
        after_zero = 0
        (1..50000000).each do |i|
            current = ((current + step) % i) + 1
            after_zero = i if current == 1 
        end

        puts after_zero

    end
end
