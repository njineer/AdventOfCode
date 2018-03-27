#!/usr/bin/env ruby
#
require 'thread'

class String
    def is_i?
        /\A[-+]?\d+\z/ === self
    end
end


def run(instructions, registers, tx, rx, debug=false)
    Thread.current[:instr] = 0
    Thread.current[:send] = 0
    index = 0
    sends = 0
    get_reg_or_value = -> str { if str.is_i? then str.to_i else registers[str] end }

    while true
        Thread.current[:instr] += 1
        instr = instructions[index]
        cmd = instr[0].to_sym
        arg1, arg2 = instr.drop(1)
        
        puts "#{cmd} #{arg1} #{arg2}" if debug
    
        # process instructions
        case cmd
            when :snd 
                tx << get_reg_or_value.call(arg1)
                sends += 1
                Thread.current[:send] += 1
            when :set 
                registers[arg1] = get_reg_or_value.call(arg2)
            when :add
                registers[arg1] += get_reg_or_value.call(arg2)
            when :mul
                registers[arg1] *= get_reg_or_value.call(arg2)
            when :mod
                registers[arg1] %= get_reg_or_value.call(arg2)
            when :rcv
                registers[arg1] = rx.pop
            when :jgz
            else 
                puts "unknown instruction #{cmd}"
        end
    
        # next instruction offset
        if cmd === :jgz and get_reg_or_value.call(arg1) != 0
            index += get_reg_or_value.call(arg2)
        else
            index += 1
        end
        if index < 0 or index >= instructions.length
            break
        end
    end
    sends
end

if __FILE__ == $0
    if ARGV.length < 1
        puts "missing input file"
    else 
        instructions = File.readlines(ARGV[0]).map { |line| line.split(" ") }.freeze

        registers_0, registers_1 = Hash.new(0), Hash.new(0)
        registers_0[:p] = 0
        registers_1[:p] = 0

        q0, q1 = Queue.new, Queue.new

        t0 = Thread.new do run(instructions, registers_0, q0, q1, false) end
        t1 = Thread.new do run(instructions, registers_1, q1, q0, true) end

        t0.join(2)
        t1.join(2)

        puts "0: instr= #{t0[:instr]}"
        puts "1: instr= #{t1[:instr]}"

    end
end
