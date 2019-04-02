#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <array>
#include <map>
#include <regex>
#include <iterator>

using namespace std;


class Cpu {
    public: 
        typedef void(Cpu::*Operation)(const int, const int, const int);

        Cpu(int _ip): ip_reg(next(registers.begin(), _ip)), ip(0), registers({0}) { }
        // Cpu(int _ip): ip_reg(next(registers.begin(), _ip)), ip(_ip) { }

        int get_register(int i) const {
            return registers[i];
        }

        string get_registers() const {
            ostringstream ss;
            for_each(registers.begin(), registers.end(), [&ss](auto reg) { ss << reg << ", "; });
            return ss.str();
        }

        void execute(const vector<tuple<string, int, int, int>>& instructions) {
            int wd = 10000;
            while(ip >= 0 && ip < instructions.size() && wd-- > 0) {
                auto& [op, a, b, c] = instructions[ip];
                cout << "[ip=" << ip 
                     << "]\t| { " << get_registers() << "}\t" 
                     << op << "(" << a << ", " << b << ", " << c << ") ";
                *ip_reg = ip;
                auto fn = ops.at(op);
                (this->*fn)(a, b, c);
                ip = *ip_reg + 1;
                cout << "\t| [ip=" << ip << "]\t| { " << get_registers() << "} "  << endl;
            }
        }

    private:
        static const map<string, Operation> ops;

        array<int, 6> registers;
        int* ip_reg;
        int ip;

        void addr(const int a, const int b, const int c);
        void addi(const int a, const int b, const int c);
        void mulr(const int a, const int b, const int c);
        void muli(const int a, const int b, const int c);
        void banr(const int a, const int b, const int c);
        void bani(const int a, const int b, const int c);
        void borr(const int a, const int b, const int c);
        void bori(const int a, const int b, const int c);
        void setr(const int a, const int b, const int c);
        void seti(const int a, const int b, const int c);
        void gtir(const int a, const int b, const int c);
        void gtri(const int a, const int b, const int c);
        void gtrr(const int a, const int b, const int c);
        void eqir(const int a, const int b, const int c);
        void eqri(const int a, const int b, const int c);
        void eqrr(const int a, const int b, const int c);

};

// addr (add register) stores into register C the result of adding register A and value B.
inline void Cpu::addr(const int a, const int b, const int c) {
    registers[c] = registers[a] + registers[b];
}
// addi (add immediate) stores in register C the result of adding register A and register B.
inline void Cpu::addi(const int a, const int b, const int c) {
    registers[c] = registers[a] + b;
}
// mulr (multiply register) stores in register C the result of multiplying register A and register B.
inline void Cpu::mulr(const int a, const int b, const int c) {
    registers[c] = registers[a] * registers[b];
}
// muli (multiply immediate) stores in register C the result of multiplying register A and value B.
inline void Cpu::muli(const int a, const int b, const int c) {
    registers[c] = registers[a] * b;
}
// banr (bitwise AND register) stores in register C the result of the bitwise AND of register A and register B.
inline void Cpu::banr(const int a, const int b, const int c) {
    registers[c] = registers[a] & registers[b];
}
// bani (bitwise AND immediate) stores in register C the result of the bitwise AND of register A and value B.
inline void Cpu::bani(const int a, const int b, const int c) {
    registers[c] = registers[a] & b;
}
// borr (bitwise OR register) stores in register C the result of the bitwise OR of register A and register B.
inline void Cpu::borr(const int a, const int b, const int c) {
    registers[c] = registers[a] | registers[b];
}
// bori (bitwise OR immediate) stores in register C the result of the bitwise OR of register A and value B.
inline void Cpu::bori(const int a, const int b, const int c) {
    registers[c] = registers[a] | b;
}
// setr (set register) copies the contents of register A into register C. (Input B is ignored.)
inline void Cpu::setr(const int a, const int b, const int c) {
    registers[c] = registers[a];
}
// seti (set immediate) stores value A in register C. (Input B is ignored.)
inline void Cpu::seti(const int a, const int b, const int c) {
    registers[c] = a;
}
// gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
inline void Cpu::gtir(const int a, const int b, const int c) {
    registers[c] = (a > registers[b]) ? 1 : 0;
}
// gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
inline void Cpu::gtri(const int a, const int b, const int c) {
    registers[c] = (registers[a] > b) ? 1 : 0;
}
// gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
inline void Cpu::gtrr(const int a, const int b, const int c) {
    registers[c] = (registers[a] > registers[b]) ? 1 : 0;
}
// eqir (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
inline void Cpu::eqir(const int a, const int b, const int c) {
    registers[c] = (a == registers[b]) ? 1 : 0;
}
// eqri (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
inline void Cpu::eqri(const int a, const int b, const int c) {
    registers[c] = (registers[a] == b) ? 1 : 0;
}
// eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
inline void Cpu::eqrr(const int a, const int b, const int c) {
    registers[c] = (registers[a] == registers[b]) ? 1 : 0;
}

// collect each operator for easy looping
const map<string, Cpu::Operation> Cpu::ops { 
    { "addr", &Cpu::addr },
    { "addi", &Cpu::addi },
    { "mulr", &Cpu::mulr },
    { "muli", &Cpu::muli },
    { "banr", &Cpu::banr },
    { "bani", &Cpu::bani },
    { "borr", &Cpu::borr },
    { "bori", &Cpu::bori },
    { "setr", &Cpu::setr },
    { "seti", &Cpu::seti },
    { "gtir", &Cpu::gtir },
    { "gtri", &Cpu::gtri },
    { "gtrr", &Cpu::gtrr },
    { "eqir", &Cpu::eqir },
    { "eqri", &Cpu::eqri },
    { "eqrr", &Cpu::eqrr }
};


int main (int argc, char** argv) {

    const regex ip_re("#ip\\s+(\\d+)");
    const regex instr_re("([a-z]+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)");

    string input;
    getline(cin, input);
    smatch match;
    
    int ip;
    if (regex_match(input, match, ip_re)) {
        ip = stoi(match[1]);
    }
    else {
        throw "missing ip set.";
    }

    Cpu cpu(ip);
    vector<tuple<string, int, int, int>> instructions;
    while (getline(cin, input)) {
        if (regex_match(input, match, instr_re)) {
            instructions.emplace_back(match[1], stoi(match[2]), stoi(match[3]), stoi(match[4]));
        }
        // skip empty lines
        else if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) {
            continue;
        }
        else {
            cerr << "Skipping bad input: " << input << endl;
        }
    }

    cpu.execute(instructions);
    cout << cpu.get_register(0) << endl;

    return 0;
}

