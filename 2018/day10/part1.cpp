#include <iostream>
#include <string>
#include <regex>
#include <cassert>
#include <vector>
#include <sstream>


using namespace std;

struct Point {
    int px, py, vx, vy;

    Point(int _px, int _py, int _vx, int _vy):
        px(_px), py(_py), vx(_vx), vy(_vy) { }

    string str() {
        ostringstream ss;
        ss << "Point<"
           << "pos=(" << px << ", " << py << "), "
           << "vel=(" << vx << ", " << vy << ")>";
        return ss.str();
    }
};

int main (int argc, char** argv) {

    // regex match single capital letters
    const regex num_re("([+-]?\\d+)");
    const sregex_iterator re_itr_end = sregex_iterator();

    vector<Point> points;

    // read all non-empty lines
    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;

        // expecting two pairs of ints per line, position and velocity
        auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
        assert(re_itr != re_itr_end);
        auto px = stoi((re_itr++)->str());
        auto py = stoi((re_itr++)->str());
        auto vx = stoi((re_itr++)->str());
        auto vy = stoi((re_itr++)->str());
        assert(re_itr == re_itr_end); // make sure input is relatively OK

        points.emplace_back(px, py, vx, vy);

        cout << points.back().str() << endl;
    } 
    return 0;
}
