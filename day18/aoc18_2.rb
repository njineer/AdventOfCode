#!/usr/bin/env ruby
#
class String
    def is_i?
        /\A[-+]?\d+\z/ === self
    end
end

class Program
    attr_reader :id
    attr_accessor :registers, :sends, :queue, :index, :status, :history

    def initialize(id)
        @id = id
        @registers = Hash.new(0)
        @registers[:p] = id
        @sends = 0
        @queue = Array.new
        @index = 0
        @status = :run
        @history = 0
    end

    def reg_or_value(str) 
        if str.is_i? 
            str.to_i 
        else 
            @registers[str.to_sym] 
        end
    end

    def is_dead?
        [:block, :done].include? @status and @queue.empty?
    end
end

def run(instructions, p0, p1, debug=false)
    watchdog = 0
    active, inactive = p0, p1
    while true
        break if active.is_dead? and inactive.is_dead?
        instr = instructions[active.index]
        cmd = instr[0].to_sym
        arg1, arg2 = instr.drop(1)
    
        puts "p#{active.id}: #{cmd} #{arg1}" + if arg2 then " #{arg2}" else "" end if debug

        # process instructions
        case cmd
            when :snd 
                inactive.queue << active.reg_or_value(arg1)
                active.sends += 1
                puts "\tsnd #{active.reg_or_value(arg1)}" if debug
            when :set 
                print "\tregisters[#{arg1.to_sym}] (#{active.registers[arg1.to_sym]}) = #{active.reg_or_value(arg2)} => " if debug
                active.registers[arg1.to_sym] = active.reg_or_value(arg2)
                puts "#{active.registers[arg1.to_sym]}" if debug
            when :add
                print "\tregisters[#{arg1.to_sym}] (#{active.registers[arg1.to_sym]}) += #{active.reg_or_value(arg2)} => " if debug
                active.registers[arg1.to_sym] += active.reg_or_value(arg2)
                puts "#{active.registers[arg1.to_sym]}" if debug
            when :mul
                print "\tregisters[#{arg1.to_sym}] (#{active.registers[arg1.to_sym]}) *= #{active.reg_or_value(arg2)} => " if debug
                active.registers[arg1.to_sym] *= active.reg_or_value(arg2)
                puts "#{active.registers[arg1.to_sym]}" if debug
            when :mod
                print "\tregisters[#{arg1.to_sym}] (#{active.registers[arg1.to_sym]}) %= #{active.reg_or_value(arg2)} => " if debug
                active.registers[arg1.to_sym] %= active.reg_or_value(arg2)
                puts "#{active.registers[arg1.to_sym]}" if debug
            when :rcv
                if active.queue.empty?
                    puts "\tqueue block" if debug
                    active.status = :block
                    active, inactive = inactive, active
                    next
                else
                    active.registers[arg1.to_sym] = active.queue.shift
                    puts "\trcv #{active.registers[arg1.to_sym]}" if debug
                    active.status = :run
                end
            when :jgz
            else 
                puts "unknown instruction #{cmd}"
        end
    
        # next instruction offset
        if cmd === :jgz and active.reg_or_value(arg1) > 0
            print "\tjump #{active.reg_or_value(arg2)} to " if debug
            active.index += active.reg_or_value(arg2)
            puts "#{active.index} because #{active.reg_or_value(arg1)} != 0" if debug
        else
            active.index += 1
        end

        # out of bounds -> done
        if active.index < 0 or active.index >= instructions.length
            puts "\tp#{active.id} done" if debug
            active.status = :done
            if inactive.is_inactive?
                puts "single deadlock after out of bounds" if debug
                break
            end
            active, inactive = inactive, active
        end

        active.history += 1
        watchdog += 1; 
        if watchdog > 200000
            puts "WATCHDOG" if debug
            break
        end
    end
end

if __FILE__ == $0
    if ARGV.length < 1
        puts "missing input file"
    else 
        instructions = File.readlines(ARGV[0]).map { |line| line.split(" ") }.freeze
        p0, p1 = Program.new(0), Program.new(1)
        run(instructions, p0, p1)
        puts "0 history: #{p0.history}"
        puts "1 history: #{p1.history}"
        puts "0 sends: #{p0.sends}"
        puts "1 sends: #{p1.sends}"
    end
end
