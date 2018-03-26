#!/usr/bin/env ruby

class String
    # convert to int if it's an int, else do nothing
    # TODO: find a more ruby-ish way to notify if successful
    def to_i_if_i
        if /\A[-+]?\d+\z/ === self
            [self.to_i, true]
        else 
            [self.to_s, false]
        end
    end
end


if __FILE__ == $0
    if ARGV.length < 1
        puts "missing input file"
    else 
        infile = ARGV[0]
        registers = Hash.new
        last_freq = nil
        instructions = File.readlines(infile).map { |line| line.split(" ") }

        index = 0
        while true
            instr, reg, arg = instructions[index]
            # if there is an arg, get the int or the register content
            value, is_int = arg.to_i_if_i if arg
            value = if !is_int then registers[value] else value end
            # make sure this register is initialized
            registers[reg] = 0 if !registers.has_key? reg
            # process instructions
            case instr
                when "snd" 
                    last_freq = registers[reg]
                when "set" 
                    registers[reg] = value
                when "add"
                    registers[reg] += value
                when "mul"
                    registers[reg] *= value
                when "mod"
                    registers[reg] %= value
                when "rcv"
                    break if registers[reg] != 0
                when "jgz"
                else 
                    puts "unknown instruction #{instr}"
            end

            # next instruction offset
            if instr === "jgz" and registers[reg] != 0
                index += value
            else
                index += 1
            end
            if index < 0 or index >= instructions.length
                break
            end
        end
        puts "last freq = #{last_freq}"
    end
end
