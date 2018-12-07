#!/usr/bin/env ruby

class DanceMove
    attr_reader :name, :a, :b
    def initialize(name, a, b=nil)
        case name
            when "s"
                @a = a.to_i
            when "x"
                @a, @b = a.to_i, b.to_i
            when "p"
                @a, @b = a, b
            else
                raise "Unknown dance move #{name}"
        end
        @name = name
    end

    def to_s()
        "DanceMove(name=#{@name}, a=#{@a}" + (b ? ", b=#{@b})" : ")")
    end
end

def parseInput(filename)
    puts "arg #{filename}"
    File.read(filename)
        .split(',')
        .map { |line| line.chomp }
        .map { |token|
            if m = token.match(/([a-z])([a-z0-9]+)(?:\/([a-z0-9]+))?/)
                name = m.captures[0]
                a = m.captures[1]
                b = if m.length > 2 then m.captures[2] else nil end
                DanceMove.new(name, a, b)
            else
                puts "no match for #{token}"
            end
        }
end

def dance(programs_state, dance_moves)
    programs = programs_state.clone
    dance_moves.each do |dm|
        case dm.name
            when "s"
                programs = programs.slice(-dm.a..-1) + programs.slice(0..(-dm.a-1))    
            when "x"
                programs[dm.a], programs[dm.b] = programs[dm.b], programs[dm.a]
            when "p"
                programs.tr!("#{dm.a}#{dm.b}", "#{dm.b}#{dm.a}")
        end
    end
    return programs
end

if __FILE__ == $0
    if ARGV.length == 0
        puts "missing input file"
    else
        dance_moves = parseInput(ARGV[0])
        dances = ARGV.length > 1 ? ARGV[1].to_i : 1
        programs = ("a".."p").to_a.join
        history = []
        cycle_end = -1
        dances.times do |i|
            if history.include?(programs)
                cycle_end = i
                break
            else
                history << programs.clone
            end
            programs = dance(programs, dance_moves)
        end
        puts("\tfound cycle at #{cycle_end}") if cycle_end >= 0
        puts "\nend: #{history[dances % cycle_end]}"
    end
end
