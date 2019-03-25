#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <cassert>

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

        static string name_str(const InstructionName& iname);

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
            unordered_map<InstructionName, function<array4i_ptr(const array4i_ptr&, const array4i_ptr&)>> ops;

    public:
        static 
            unordered_set<Instructions::InstructionName> compare(const unique_ptr<Sample>& sample);
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

const unordered_map<Instructions::InstructionName, function<array4i_ptr(const array4i_ptr&, const array4i_ptr&)>> 
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

unordered_set<Instructions::InstructionName> Instructions::compare(const unique_ptr<Sample>& sample) {
    unordered_set<InstructionName> eq;
    for (auto& op : ops) {
        auto result = op.second(sample->before, sample->instr);
        if (equal(sample->after->begin(), sample->after->end(), result->begin())) {
            eq.insert(op.first);
        }
    }
    return eq;
}

string Instructions::name_str(const InstructionName& iname) {
    string name;
    switch (iname) {
        case InstructionName::addr: name = "addr"; break;
        case InstructionName::addi: name = "addi"; break;
        case InstructionName::mulr: name = "mulr"; break;
        case InstructionName::muli: name = "muli"; break;
        case InstructionName::banr: name = "banr"; break;
        case InstructionName::bani: name = "bani"; break;
        case InstructionName::borr: name = "borr"; break;
        case InstructionName::bori: name = "bori"; break;
        case InstructionName::setr: name = "setr"; break;
        case InstructionName::seti: name = "seti"; break;
        case InstructionName::gtir: name = "gtir"; break;
        case InstructionName::gtri: name = "gtri"; break;
        case InstructionName::gtrr: name = "gtrr"; break;
        case InstructionName::eqir: name = "eqir"; break;
        case InstructionName::eqri: name = "eqri"; break;
        case InstructionName::eqrr: name = "eqrr"; break;
        default: name="unknown"; break;
    }
    return name;
};


array4i_ptr parse_line(const string& s, const regex& re) {
    smatch match;
    auto re_eq_itr = sregex_iterator(s.begin(), s.end(), re);
    auto end_eq_itr = sregex_iterator();
    if (re_eq_itr == end_eq_itr) {
        throw "regex mismatch: " + s;
    }

    array4i a;
    for (int i=0; i < 4; i++) {
        a[i] = stoi((re_eq_itr++)->str());
    }

    return make_unique<array4i>(a);
}

int main (int argc, char** argv) {

    const regex num_re("(\\d+)");

    vector<unique_ptr<Sample>> samples;

    int empty_lines = 0;
    int line = 0;
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
        switch(line % 3) {
            case 0: 
                tb = move(line_instrs);
                break;
            case 1: 
                ti = move(line_instrs);
                break;
            case 2: 
                ta = move(line_instrs);
                samples.push_back(make_unique<Sample>(tb, ti, ta));
                break;
            default: break;
        }

        ++line;
    }

    // read the test program
    vector<array4i_ptr> program;
    for (string input; getline(cin, input);) {
        // note and skip empty lines
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) {
            continue;
        }
        program.emplace_back(move(parse_line(input, num_re)));
    }

    unordered_map<int, Instructions::InstructionName> opcodes;

    // frequency of instructions
    array<unordered_map<Instructions::InstructionName, int>, 16> match_counts;

    for (auto& sample : samples) {
        auto opcode = sample.get()->instr->at(0);

        auto potential_matches = Instructions::compare(sample);
        for (auto& pm : potential_matches) { 
            match_counts[opcode][pm]++;
        }
    }

    for (int i=0; i < match_counts.size(); i++) {
        opcodes[i] = max_element(match_counts[i].begin(), match_counts[i].end(),
            [](auto& p1, auto& p2) { return p1.second < p2.second; })->first;
    }

    for (auto& op: opcodes) {
        cout << op.first << " " << Instructions::name_str(op.second) << endl;
    }

    return 0;
}


