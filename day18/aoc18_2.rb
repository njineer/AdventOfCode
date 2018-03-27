#!/usr/bin/env ruby
#
class String
    def is_i?
        /\A[-+]?\d+\z/ === self
    end
end

class Program
    attr_reader :id
    attr_accessor :registers, :sends, :queue, :index, :status

    def initialize(id)
        @id = id
        @registers = Hash.new(0)
        @registers[:p] = id
        @sends = 0
        @queue = Array.new
        @index = 0
        @status = :run
    end

    def reg_or_value(str) 
        if str.is_i? 
            str.to_i 
        else 
            @registers[str.to_sym] 
        end
    end

    def is_inactive?
        [:block, :done].include? @status
    end
end

def run(instructions, p0, p1)
    watchdog = 0
    active, inactive = p0, p1
    while true
        instr = instructions[active.index]
        cmd = instr[0].to_sym
        arg1, arg2 = instr.drop(1)
    
        #puts "#{cmd} #{arg1} #{arg2}" if active === p0

        # process instructions
        case cmd
            when :snd 
                inactive.queue << active.reg_or_value(arg1)
                active.sends += 1
            when :set 
                active.registers[arg1.to_sym] = active.reg_or_value(arg2)
            when :add
                active.registers[arg1.to_sym] += active.reg_or_value(arg2)
            when :mul
                active.registers[arg1.to_sym] *= active.reg_or_value(arg2)
            when :mod
                active.registers[arg1.to_sym] %= active.reg_or_value(arg2)
            when :rcv
                if active.queue.empty?
                    break if inactive.is_inactive?
                    active.status = :block
                    active, inactive = inactive, active
                    next
                else
                    active.registers[arg1.to_sym] = active.queue.shift
                    active.status = :run
                end
            when :jgz
            else 
                puts "unknown instruction #{cmd}"
        end
    
        # next instruction offset
        if cmd === :jgz and active.reg_or_value(arg1) != 0
            active.index += active.reg_or_value(arg2)
        else
            active.index += 1
        end
        if active.index < 0 or active.index >= instructions.length
            active.status = :done
            break if inactive.is_inactive?
            active, inactive = inactive, active
        end

        break if active.is_inactive? and inactive.is_inactive?
        watchdog += 1; break if watchdog > 200000
    end
end

if __FILE__ == $0
    if ARGV.length < 1
        puts "missing input file"
    else 
        instructions = File.readlines(ARGV[0]).map { |line| line.split(" ") }.freeze
        p0, p1 = Program.new(0), Program.new(1)
        run(instructions, p0, p1)
        puts p0.status
        puts p1.status
        puts p1.sends

    end
end
