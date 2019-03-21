#!/usr/bin/env ruby

require 'colorize'

def RecursiveThing(number,color)
	if(number >= 0)
		puts "#{color} thread, number is #{number}.".colorize(color)
		sleep(1+rand(10))
		RecursiveThing(number-1,color)
	else
		puts "#{color} thread complete!".colorize(color)
	end
end

colors = [:green,:blue,:yellow,:red,:cyan]
threads = []

colors.each {|color|threads << Thread.new{RecursiveThing(10,color)}}
threads.each {|thread| thread.join}
puts "All threads complete!"
