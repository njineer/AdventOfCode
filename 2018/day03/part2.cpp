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


struct Rectangle {
    int id, left, top, width, height;

    Rectangle(array<int, 5>& params) {
        id     = params[0];
        left   = params[1];
        top    = params[2];
        width  = params[3];
        height = params[4];
    }
};



int main (int argc, char** argv) {

    vector<Rectangle> claims;
    map<pair<int, int>, int> used_squares;
    InputParser parser;

    // extract numbers for each line into array
    for (string input; getline(cin, input);) {
        auto claim = parser.parse(input);
        auto rectangle = Rectangle(claim);
        // update the use of each square in this rectangle
        for (int x=rectangle.left; x < rectangle.left+rectangle.width; x++) {
            for (int y=rectangle.top; y < rectangle.top+rectangle.height; y++) {
                used_squares[make_pair(x,y)]++;
            }
        }
        // save the rectangle for later
        claims.emplace_back(rectangle);
    } 
    cout << count_if(used_squares.begin(), used_squares.end(), [](auto& sq){ return sq.second > 1; }) << endl;

    // for each rectangle
    for (auto& rectangle: claims) {
        bool overlap = false;
        // check each square for overlap
        for (int x=rectangle.left; x < rectangle.left+rectangle.width; x++) {
            for (int y=rectangle.top; y < rectangle.top+rectangle.height; y++) {
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
            cout << rectangle.id << endl;
            break;
        }
    }

    return 0;
}
