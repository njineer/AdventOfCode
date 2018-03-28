#!/usr/bin/env ruby

require 'thread'
require 'timeout'

class String
    def is_i?
        /\A[-+]?\d+\z/ === self
    end
end

class Program
    attr_reader :id, :txq, :rxq
    attr_accessor :registers, :sends, :index
    def initialize(id, txq, rxq)
        @id = id
        @txq = txq
        @rxq = rxq
        @registers = Hash.new(0)
        @registers[:p] = id
        @sends = 0
        @index = 0
    end

    def reg_or_value(str) 
        if str.is_i? 
            str.to_i 
        else 
            @registers[str.to_sym] 
        end
    end
end

def run(instructions, prog)
    while true
        #break if prog.is_dead? and inactive.is_dead?
        instr = instructions[prog.index]
        cmd = instr[0].to_sym
        arg1, arg2 = instr.drop(1)
    
        #puts "p#{prog.id}: #{cmd} #{arg1}" + if arg2 then " #{arg2}" else "" end if debug

        # process instructions
        case cmd
            when :snd 
                prog.txq << prog.reg_or_value(arg1)
                prog.sends += 1
            when :set 
                prog.registers[arg1.to_sym] = prog.reg_or_value(arg2)
            when :add
                prog.registers[arg1.to_sym] += prog.reg_or_value(arg2)
            when :mul
                prog.registers[arg1.to_sym] *= prog.reg_or_value(arg2)
            when :mod
                prog.registers[arg1.to_sym] %= prog.reg_or_value(arg2)
            when :rcv
                begin
                    rx_val = Timeout::timeout(0.1) { prog.rxq.pop }
                    prog.registers[arg1.to_sym] = rx_val
                rescue Timeout::Error
                    break
                end
            when :jgz
            else 
                puts "unknown instruction #{cmd}"
        end
    
        # next instruction offset
        if cmd === :jgz and prog.reg_or_value(arg1) > 0
            prog.index += prog.reg_or_value(arg2)
        else
            prog.index += 1
        end

        # out of bounds -> done
        break if prog.index < 0 or prog.index >= instructions.length
    end
end

if __FILE__ == $0
    if ARGV.length < 1
        puts "missing input file"
    else 
        instructions = File.readlines(ARGV[0]).map { |line| line.split(" ") }.freeze
        q0, q1 = Queue.new, Queue.new
        p0, p1 = Program.new(0, q0, q1), Program.new(1, q1, q0)
        t0 = Thread.new { run(instructions, p0) }
        t1 = Thread.new { run(instructions, p1) }
        t0.join
        t1.join
        puts "0 sends: #{p0.sends}"
        puts "1 sends: #{p1.sends}"
    end
end

