#!/usr/bin/env ruby

class String
    def is_i?
        /\A[-+]?\d+\z/ === self
    end
end


def run(instructions, registers)
    last_freq = nil
    index = 0
    get_reg_or_value = -> str { if str.is_i? then str.to_i else registers[str] end }

    while true
        instr = instructions[index]
        cmd = instr[0].to_sym
        arg1, arg2 = instr.drop(1)
        
        puts "#{cmd}, #{arg1}, #{arg2}"
    
        # process instructions
        case cmd
            when :snd 
                last_freq = get_reg_or_value.call(arg1)
            when :set 
                registers[arg1] = get_reg_or_value.call(arg2)
            when :add
                registers[arg1] += get_reg_or_value.call(arg2)
            when :mul
                registers[arg1] *= get_reg_or_value.call(arg2)
            when :mod
                registers[arg1] %= get_reg_or_value.call(arg2)
            when :rcv
                break if registers[arg1] != 0
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
    last_freq
end

if __FILE__ == $0
    if ARGV.length < 1
        puts "missing input file"
    else 
        infile = ARGV[0]
        registers = Hash.new(0)
        instructions = File.readlines(infile).map { |line| line.split(" ") }.freeze
        last_freq = run(instructions, registers)

        puts "last freq = #{last_freq}"
    end
end
