#include <iostream>
#include <string>
#include <regex>
#include <cassert>
#include <vector>
#include <sstream>
#include <climits>
#include <algorithm>

#define MSG_HEIGHT_L 5
#define MSG_HEIGHT_H 10
#define TIME_LIMIT 25000

using namespace std;

struct Point {
    int px, py, vx, vy;

    Point():
        px(0), py(0), vx(0), vy(0) { }

    Point(int _px, int _py, int _vx, int _vy):
        px(_px), py(_py), vx(_vx), vy(_vy) { }

    string str() const{
        ostringstream ss;
        ss << "Point<"
           << "pos=(" << px << ", " << py << "), "
           << "vel=(" << vx << ", " << vy << ")>";
        return ss.str();
    }

    void update() {
        px += vx;
        py += vy;
    }
};

// comparators for std::alg::
bool px_cmp (const Point& p1, const Point& p2) { return p1.px < p2.px; };
bool py_cmp (const Point& p1, const Point& p2) { return p1.py < p2.py; };

// a highly inefficient means of drawing the points
//      ...but it runs very infrequently (assuming low MSG_HEIGHT variance)
void draw(const vector<Point>& points) {
    auto min_x = min_element(points.begin(), points.end(), px_cmp)->px;
    auto max_x = max_element(points.begin(), points.end(), px_cmp)->px;
    auto min_y = min_element(points.begin(), points.end(), py_cmp)->py;
    auto max_y = max_element(points.begin(), points.end(), py_cmp)->py;
    
    vector<Point> row;
    row.reserve(abs(max_x-min_x));
    for (auto y=min_y; y <= max_y; y++) {
        copy_if(points.begin(), points.end(), back_inserter(row), [y](const auto& p){ return p.py == y; });
        sort(row.begin(), row.end(), [](const auto& p1, const auto& p2) { return p1.px < p2.px; });
        auto prev_itr = row.begin();
        for (auto x=min_x; x <= max_x; x++) {
           auto itr = find_if(prev_itr, row.end(), [x](const auto& p){ return p.px == x; });
           if (itr != row.end()) {
               prev_itr = itr;
               cout << "#";
           } else {
               cout << ".";
           }
        }
        cout << endl;
        row.clear();
    }
}

int main (int argc, char** argv) {

    // regex match numbers
    const regex num_re("([+-]?\\d+)");
    const auto re_itr_end = sregex_iterator();
    vector<Point> points;

    // read all non-empty lines
    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;

        // expecting two pairs of ints per line, position and velocity
        auto re_itr = sregex_iterator(input.begin(), input.end(), num_re);
        assert(re_itr != re_itr_end); // make sure input is relatively OK
        auto px = stoi((re_itr++)->str());
        auto py = stoi((re_itr++)->str());
        auto vx = stoi((re_itr++)->str());
        auto vy = stoi((re_itr++)->str());
        assert(re_itr == re_itr_end); // make sure input is relatively OK

        points.emplace_back(px, py, vx, vy);
    } 

    // establish bounds at t=0
    auto y_min = min_element(points.begin(), points.end(), py_cmp)->py;
    auto y_max = max_element(points.begin(), points.end(), py_cmp)->py;
    auto vertical_spread = abs(y_max - y_min);
    auto t = 0;

    do {
        // if the points are all within a given range, print them and the time
        if (vertical_spread >= MSG_HEIGHT_L && vertical_spread <= MSG_HEIGHT_H) {
            draw(points);
            cout << "@ t = " << t << endl << endl;
        }

        // point movement
        for(auto& p: points) {
            p.update();
        }
        
        // reevaluate bounds
        y_min = min_element(points.begin(), points.end(), py_cmp)->py;
        y_max = max_element(points.begin(), points.end(), py_cmp)->py;
        vertical_spread = abs(y_max - y_min);

    } while (t++ < TIME_LIMIT);

    return 0;
}
