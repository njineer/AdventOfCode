#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <regex>
#include <array>
#include <map>
#include <utility>

using namespace std;

int main (int argc, char** argv) {

    regex num_re("(-?\\d+)");
    auto re_itr_end = sregex_iterator();
    array<int, 5> claim;
    map<pair<int, int>, int> used_squares;

    // extract numbers for each line into array
    for (string input; getline(cin, input);) {
        auto re_itr_begin = sregex_iterator(input.begin(), input.end(), num_re);
        transform(re_itr_begin, re_itr_end, claim.begin(), [](auto s){return stoi(s.str());});
        auto left   = claim[1];
        auto top    = claim[2];
        auto width  = claim[3];
        auto height = claim[4];
        for (int x=left; x < left+width; x++) {
            for (int y=top; y < top+height; y++) {
                used_squares[make_pair(x,y)] += 1;
            }
        }
    } 
    cout << count_if(used_squares.begin(), used_squares.end(), [](auto& sq){ return sq.second > 1; }) << endl;

    return 0;
}
