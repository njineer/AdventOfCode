#include <iostream>
#include <cctype>
#include <algorithm>
#include <regex>
#include <cassert>
#include <sstream>
#include <climits>
#include <map>
#include <set>

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
bool operator<(const Point& p1, const Point& p2) {
    return tie(p1.x, p1.y) < tie(p2.x, p2.y);
}
bool operator==(const Point& p1, const Point& p2) {
    return tie(p1.x, p1.y) == tie(p2.x, p2.y);
}

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
    map<Point, int> grid;
    vector<int> distances(points.size());

    // for each x,y in relevant space
    for (int y=min_y; y <= max_y; y++) {
        for (int x=min_x; x <= max_x; x++) {
            auto xy_point = Point(x,y);
            // get the distance from each Point
            transform(points.begin(), points.end(), distances.begin(),
                      [&xy_point](const Point& p){ return manhattan(p, xy_point); });
            auto min_dist = *min_element(distances.begin(), distances.end());

            // check each Point to see if it is the minimum distance
            // store -1 for ties, Point index for unique
            for (int i=0; i < points.size(); i++) {
                if (manhattan(points[i], xy_point) == min_dist) {
                    auto coord = Point(x,y);
                    if (grid.count(coord)) {
                        grid[coord] = -1;
                        break;
                    } else {
                        grid[coord] = i;
                    }
                }
            }
        }
    }
    
    // find infinite-area Points by finding the neighbor of the
    //   coords around the edge/border of the relevant space
    set<int> infinite;
    for (int x=0; x<=max_x; x++) infinite.insert(grid[Point(x, min_y)]);
    for (int x=0; x<=max_x; x++) infinite.insert(grid[Point(x, max_y)]);
    for (int y=0; y<=max_y; y++) infinite.insert(grid[Point(min_x, y)]);
    for (int y=0; y<=max_y; y++) infinite.insert(grid[Point(max_x, y)]);
  
    map<int, int> freq;
    for (const auto& xy : grid) {
        if(!infinite.count(xy.second)) {
            freq[xy.second]++;
        }
    }
    cout << "Largest area: " 
         << max_element(freq.begin(), freq.end(), [](const auto& p1, const auto& p2){ return p1.second < p2.second; })->second
         << endl;

    return 0;
}
