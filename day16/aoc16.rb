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
            if token.length == 2
                name, a = token.chars
                DanceMove.new(name, a)
            elsif token.length == 4
                name, a, _, b = token.chars
                DanceMove.new(name, a, b)
            else
                raise "Unexpected input format #{token}"
            end
        }
end

if __FILE__ == $0
    if ARGV.length == 0
        puts "missing input file"
    else
        dance_moves = parseInput(ARGV[0])
        puts dance_moves
        programs = ("a".."p").to_a.join
        puts programs
        dance_moves.each do |dm|
            print "#{dm} => "
            case dm.name
                when "s"
                    programs = programs.slice(-dm.a..-1) + programs.slice(0..-dm.a)    
                when "x", "p"
                    programs[dm.a], programs[dm.b] = programs[dm.b], programs[dm.a]
            end
            puts programs
        end
        puts programs
    end
end
