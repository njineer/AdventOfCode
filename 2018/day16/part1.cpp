#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <array>
#include <functional>
#include <map>
#include <memory>

using namespace std;

using array4i = array<int, 4>;
using array4i_ptr = unique_ptr<array4i>;

struct Sample {
    array4i_ptr before, after, instr;

    Sample(array4i_ptr& _before, array4i_ptr& _instr, array4i_ptr& _after) 
    { 
        before = move(_before);
        instr = move(_instr);
        after = move(_after);
    }

    string str() {
        ostringstream ss;
        int a,b,c,d;
        tie(a,b,c,d) = *before;
        ss << "before(" << a << ", " << b << ", " << c << ", " << d << "); ";
        tie(a,b,c,d) = *instr;
        ss << "instr(" << a << ", " << b << ", " << c << ", " << d << "); ";
        tie(a,b,c,d) = *after;
        ss << "after(" << a << ", " << b << ", " << c << ", " << d << "); ";
        return ss.str();
    }

};

class Instructions {
    public:
        enum class InstructionName {
            addr, addi,
            mulr, muli,
            banr, bani,
            borr, bori,
            setr, seti,
            gtir, gtri, gtrr,
            eqir, eqri, eqrr
        };

    private:
        static array4i_ptr addr(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr addi(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr mulr(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr muli(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr banr(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr bani(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr borr(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr bori(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr setr(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr seti(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr gtir(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr gtri(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr gtrr(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr eqir(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr eqri(const array4i_ptr& before, const array4i_ptr& instr);
        static array4i_ptr eqrr(const array4i_ptr& before, const array4i_ptr& instr);

        static const 
            map<InstructionName, function<array4i_ptr(const array4i_ptr&, const array4i_ptr&)>> ops;

    public:
        static int analyze(const vector<Sample>& samples);
};

// addr (add register) stores into register C the result of adding register A and register B.
array4i_ptr Instructions::addr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] + regs[b];
    return make_unique<array4i>(regs);
}
// addi (add immediate) stores into register C the result of adding register A and instrue B.
array4i_ptr Instructions::addi(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] + b;
    return make_unique<array4i>(regs);
}
// mulr (multiply register) stores into register C the result of multiplying register A and register B.
array4i_ptr Instructions::mulr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] * regs[b];
    return make_unique<array4i>(regs);
}
// muli (multiply immediate) stores into register C the result of multiplying register A and instrue B.
array4i_ptr Instructions::muli(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] * b;
    return make_unique<array4i>(regs);
}
// banr (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
array4i_ptr Instructions::banr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] & regs[b];
    return make_unique<array4i>(regs);
}
// bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and instrue B.
array4i_ptr Instructions::bani(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] & b;
    return make_unique<array4i>(regs);
}
// borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
array4i_ptr Instructions::borr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] | regs[b];
    return make_unique<array4i>(regs);
}
// bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and instrue B.
array4i_ptr Instructions::bori(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a] | b;
    return make_unique<array4i>(regs);
}
// setr (set register) copies the contents of register A into register C. (Input B is ignored.)
array4i_ptr Instructions::setr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = regs[a];
    return make_unique<array4i>(regs);
}
// seti (set immediate) stores instrue A into register C. (Input B is ignored.)
array4i_ptr Instructions::seti(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = a;
    return make_unique<array4i>(regs);
}
// gtir (greater-than immediate/register) sets register C to 1 if instrue A is greater than register B. Otherwise, register C is set to 0.
array4i_ptr Instructions::gtir(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = (a > regs[b]) ? 1 : 0;
    return make_unique<array4i>(regs);
}
// gtri (greater-than register/immediate) sets register C to 1 if register A is greater than instrue B. Otherwise, register C is set to 0.
array4i_ptr Instructions::gtri(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = (regs[a] > b) ? 1 : 0;
    return make_unique<array4i>(regs);
}
// gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
array4i_ptr Instructions::gtrr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = (regs[a] > regs[b]) ? 1 : 0;
    return make_unique<array4i>(regs);
}
// eqir (equal immediate/register) sets register C to 1 if instrue A is equal to register B. Otherwise, register C is set to 0.
array4i_ptr Instructions::eqir(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = (a == regs[b]) ? 1 : 0;
    return make_unique<array4i>(regs);
}
// eqri (equal register/immediate) sets register C to 1 if register A is equal to instrue B. Otherwise, register C is set to 0.
array4i_ptr Instructions::eqri(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = (regs[a] == b) ? 1 : 0;
    return make_unique<array4i>(regs);
}
// eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
array4i_ptr Instructions::eqrr(const array4i_ptr& before, const array4i_ptr& instr) {
    auto regs = *before;
    auto [_, a, b, c] = *instr;
    regs[c] = (regs[a] == regs[b]) ? 1 : 0;
    return make_unique<array4i>(regs);
}

const map<Instructions::InstructionName, function<array4i_ptr(const array4i_ptr&, const array4i_ptr&)>> 
Instructions::ops {
    { InstructionName::addr, addr },
    { InstructionName::addi, addi },
    { InstructionName::mulr, mulr },
    { InstructionName::muli, muli },
    { InstructionName::banr, banr },
    { InstructionName::bani, bani },
    { InstructionName::borr, borr },
    { InstructionName::bori, bori },
    { InstructionName::setr, setr },
    { InstructionName::seti, seti },
    { InstructionName::gtir, gtir },
    { InstructionName::gtri, gtri },
    { InstructionName::gtrr, gtrr },
    { InstructionName::eqir, eqir },
    { InstructionName::eqri, eqri },
    { InstructionName::eqrr, eqrr }
};

int Instructions::analyze(const vector<Sample>& samples) {
    // for each sample
    return count_if(samples.begin(), samples.end(), [](auto& s) {
        // count the number of equivalent operations
        auto equivalents = count_if(ops.begin(), ops.end(), [&s](auto& op) {
            auto result = op.second(s.before, s.instr);
            return equal(s.after->begin(), s.after->end(), result->begin());
        });
        return equivalents >= 3;
    });
}

array4i_ptr parse_line(const string& s, const regex& re) {
    smatch match;
    auto re_itr = sregex_iterator(s.begin(), s.end(), re);
    auto end_itr = sregex_iterator();
    if (re_itr == end_itr) {
        throw "regex mismatch: " + s;
    }

    array4i a;
    for (int i=0; i < 4; i++) {
        a[i] = stoi((re_itr++)->str());
    }

    return make_unique<array4i>(a);
}

int main (int argc, char** argv) {

    const regex num_re("(\\d+)");

    vector<Sample> samples;

    int empty_lines = 0;
    int i = 0;
    array4i_ptr tb, ta, ti;

    for (string input; getline(cin, input);) {

        // samples are separate by one empty line
        //      >1 means end of part1
        if (empty_lines > 1)  {
            break;
        }
        // note and skip empty lines
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) {
            ++empty_lines;
            continue;
        }
        else {
            empty_lines = 0;
        }

        // extract a array of 4 ints from before/instruction/after lines
        auto line_instrs = parse_line(input, num_re);
        switch(i % 3) {
            case 0: 
                tb = move(line_instrs);
                break;
            case 1: 
                ti = move(line_instrs);
                break;
            case 2: 
                ta = move(line_instrs);
                samples.emplace_back(tb, ti, ta);
                break;
            default: break;
        }

        ++i;
    }

    cout << Instructions::analyze(samples) << endl;

    return 0;
}

