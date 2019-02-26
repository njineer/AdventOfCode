#include <iostream>
#include <cctype>
#include <regex>
#include <algorithm>
#include <cassert>

using namespace std;


int main (int argc, char** argv) {
    const regex num_re("(\\d+)");
    const sregex_iterator re_itr_end = sregex_iterator();

    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;
        auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
        auto x = stoi((re_itr++)->str());
        auto y = stoi((re_itr++)->str());
        assert(re_itr == re_itr_end);
    } 
    
    return 0;
}
