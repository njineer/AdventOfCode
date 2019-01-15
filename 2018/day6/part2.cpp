#include <iostream>
#include <cctype>
#include <algorithm>
#include <regex>
#include <cassert>
#include <sstream>

using namespace std;


// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from right end, in-place
inline void rtrim(std::string &s) {
    s.erase(
        std::find_if(s.rbegin(), 
                     s.rend(), 
                     [](int ch) { return !std::isspace(ch); }).base(), 
        s.end()
    );
}

struct Point {
    int x, y;

    Point(int _x, int _y) : x(_x), y(_y) { }
    
    const string str() const {
        ostringstream ss;
        ss << "<Point(x=" << x << ", y=" << y << ")>";
        return ss.str();
    }

};

int main (int argc, char** argv) {
    const regex num_re("(\\d+)");
    const sregex_iterator re_itr_end = sregex_iterator();
    vector<Point> points;
    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;
        auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
        auto x = stoi((re_itr++)->str());
        auto y = stoi((re_itr++)->str());
        points.emplace_back(x,y);
        assert(re_itr == re_itr_end);
    } 

    for (const auto& p : points)
        cout << p.str() << endl;

    return 0;
}
