#include <iostream>
#include <cctype>
#include <algorithm>
#include <regex>
#include <cassert>
#include <sstream>
#include <climits>
#include <numeric>

using namespace std;

#define DISTANCE 10000

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
int manhattan(const Point& p1, const Point& p2) {
    return abs(p1.x-p2.x) + abs(p1.y-p2.y);
}

int main (int argc, char** argv) {
    const regex num_re("(\\d+)");
    const sregex_iterator re_itr_end = sregex_iterator();

    vector<Point> points;
    int min_x, max_x, min_y, max_y;
    min_x = min_y = INT_MAX;
    max_x = max_y = INT_MIN;

    // read input into list of Points, track min/max x/y
    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;
        auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
        auto x = stoi((re_itr++)->str());
        auto y = stoi((re_itr++)->str());
        points.emplace_back(x,y);
        assert(re_itr == re_itr_end);

        min_x = min(min_x, x);
        min_y = min(min_y, y);
        max_x = max(max_x, x);
        max_y = max(max_y, y);
    } 
    
    cout << "min/max: (" << min_x << ", " << min_y << ")...(" << max_x << ", " << max_y << ")" << endl;

    // shamelessly inspired by a python solution from 
    //   https://www.reddit.com/r/adventofcode/comments/a3kr4r/2018_day_6_solutions/
    vector<int> distances(points.size());
    int area = 0;

    // for each x,y in relevant space
    for (int y=min_y; y <= max_y; y++) {
        for (int x=min_x; x <= max_x; x++) {
            auto xy_point = Point(x,y);
            // get the distance from each Point
            transform(points.begin(), points.end(), distances.begin(),
                      [&xy_point](const Point& p){ return manhattan(p, xy_point); });

            if (accumulate(distances.begin(), distances.end(), 0) < DISTANCE)
                area++;
        }
    }
    
    cout << "Central area: " << area << endl;
    return 0;
}

