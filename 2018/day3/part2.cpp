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

struct InputParser {
    const static regex num_re;
    const static sregex_iterator re_itr_end;

    array<int, 5> parse(const string& input) {
        array<int, 5> claim;
        auto re_itr_begin = sregex_iterator(input.begin(), input.end(), num_re);
        transform(re_itr_begin, re_itr_end, claim.begin(), [](auto s){return stoi(s.str());});
        return claim;
    }
};
const regex InputParser::num_re("(-?\\d+)");
const sregex_iterator InputParser::re_itr_end = sregex_iterator();


int main (int argc, char** argv) {

    vector<array<int, 5>> claims;
    map<pair<int, int>, int> used_squares;
    InputParser parser;

    // extract numbers for each line into array
    for (string input; getline(cin, input);) {
        auto claim = parser.parse(input);
        auto id     = claim[0];
        auto left   = claim[1];
        auto top    = claim[2];
        auto width  = claim[3];
        auto height = claim[4];
        // update the use of each square in this rectangle
        for (int x=left; x < left+width; x++) {
            for (int y=top; y < top+height; y++) {
                used_squares[make_pair(x,y)]++;
            }
        }
        // save the rectangle for later
        claims.emplace_back(claim);
    } 
    cout << count_if(used_squares.begin(), used_squares.end(), [](auto& sq){ return sq.second > 1; }) << endl;

    // for each rectangle
    for (auto& claim: claims) {
        auto id     = claim[0];
        auto left   = claim[1];
        auto top    = claim[2];
        auto width  = claim[3];
        auto height = claim[4];
        bool overlap = false;
        // check each square for overlap
        for (int x=left; x < left+width; x++) {
            for (int y=top; y < top+height; y++) {
                if (used_squares[make_pair(x,y)] > 1) {
                    overlap = true;
                    // don't bother to keep looking
                    break;
                }
            }
            if (overlap) {
                // don't bother to keep looking
                break;
            }
        }
        if (!overlap) {
            // this is the unique rectangle
            cout << id << endl;
            break;
        }
    }

    return 0;
}
