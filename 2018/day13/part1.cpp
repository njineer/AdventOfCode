#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include <optional>

using namespace std;

using int_pair = pair<int, int>;

class Tracks {
    private:
        enum class Direction {
            Up,
            Down,
            Left,
            Right
        };

        struct Cart {
            int row, col;
            Direction dir;
            int intersections = 0;

            Cart(int _row, int _col, Direction _dir) 
                : row(_row), col(_col), dir(_dir), intersections(0)
            { }

            Cart() : row(0), col(0), dir(Direction::Up), intersections(0)
            { }

            inline int_pair coords() const {
                return make_pair(row, col);
            }

            string str() {
                ostringstream ss;
                ss << "<Cart (row=" << row 
                   << ", col=" << col 
                   << ", dir=" << dir_str(dir) 
                   << ", intersections=" << intersections << ")>";
                return ss.str();
            }
        };

        const static string track_chars;
        const static string cart_chars;
        const static map<Tracks::Direction, map<char, Tracks::Direction>> next_dir;
        const static map<Tracks::Direction, array<Tracks::Direction, 3>> next_dir_intersection;
        static string dir_str(const Direction& dir);

        map<int_pair, char> tracks;
        vector<Cart> carts;
        size_t row_count, col_count, cart_count;

        void add_cart(size_t row, size_t col, char direction) {
            Direction dir;
            switch (direction) {
                case '^': dir = Direction::Up; break;
                case 'v': dir = Direction::Down; break;
                case '<': dir = Direction::Left; break;
                case '>': dir = Direction::Right; break;
                default: break;
            }
            carts.emplace_back(row, col, dir);
            ++cart_count;
        }

        void tick_cart(Cart& cart) {
            //cout << "tick cart: " << cart.str() << endl;
            int_pair next_coord;
            switch (cart.dir) {
                case Direction::Up:    next_coord = make_pair(--cart.row, cart.col); break;
                case Direction::Down:  next_coord = make_pair(++cart.row, cart.col); break;
                case Direction::Left:  next_coord = make_pair(cart.row, --cart.col); break;
                case Direction::Right: next_coord = make_pair(cart.row, ++cart.col); break;
                default: break;
            }
            
            auto next_track = tracks.at(next_coord);
            Direction new_dir;
            if (next_track == '+') {
                new_dir = next_dir_intersection.at(cart.dir).at(cart.intersections++ % 3);
            } else {
                new_dir = next_dir.at(cart.dir).at(next_track);
            }
            cart.dir = new_dir;
        }
        
        optional<int_pair> collision() {
            set<int_pair> cart_coords;
            for (auto& cart : carts) {
                if (!cart_coords.insert(cart.coords()).second) {
                    return cart.coords();
                }
            }
            return nullopt;
        }


    public:
        Tracks() : row_count(0), col_count(0)
        { } 

        void add_row(const string& line) {
            for (size_t i=0; i < line.size(); ++i) {
                auto chr = line[i];
                if (isspace(chr)) {
                    continue;
                }
                else if (track_chars.find(chr) != string::npos) {
                    tracks[make_pair(row_count, i)] = chr;
                }
                else if (cart_chars.find(chr) != string::npos) {
                    switch (chr) {
                        case '>': 
                        case '<': 
                            tracks[make_pair(row_count, i)] = '-'; break;
                        case '^': 
                        case 'v': 
                            tracks[make_pair(row_count, i)] = '|'; break;
                        default: break;
                    }
                    add_cart(row_count, i, chr);
                }
                else {
                    cerr << "Unrecognized char '" << chr << "' in input" << endl;
                }
            }
            ++row_count;
            col_count = max(col_count, line.size());
        }

        optional<int_pair> tick() {
            sort(carts.begin(), carts.end(), [](const Cart& c1, const Cart& c2)
                 { return c1.coords() < c2.coords(); });
            for (auto& cart : carts) {
                tick_cart(cart);
                auto result = collision();
                if (result.has_value()) {
                    return result.value();
                }
            }
            return nullopt;
        }

        string info() {
            ostringstream ss;
            ss << "Tracks: [" << row_count << " x " << col_count << "]" << endl;
            for (auto& cart : carts) {
                ss << "\t" << cart.str() << endl;
            }
            return ss.str();
        }

};
const string Tracks::track_chars = "|-/\\+";
const string Tracks::cart_chars = "^v<>";
const map<Tracks::Direction, map<char, Tracks::Direction>> Tracks::next_dir = {
    { Direction::Up, {
        { '|' , Direction::Up },
        { '/' , Direction::Right },
        { '\\' , Direction::Left },
    }},
    { Direction::Down, {
        { '|' , Direction::Down },
        { '/' , Direction::Left },
        { '\\' , Direction::Right },
    }},
    { Direction::Left, {
        { '-' , Direction::Left },
        { '/' , Direction::Down },
        { '\\' , Direction::Up },
    }},
    { Direction::Right, {
        { '-' , Direction::Right },
        { '/' , Direction::Up },
        { '\\' , Direction::Down },
    }}
};

const map<Tracks::Direction, array<Tracks::Direction, 3>> Tracks::next_dir_intersection = {
    { Tracks::Direction::Up, {
        Tracks::Direction::Left,
        Tracks::Direction::Up,
        Tracks::Direction::Right
    }},
    { Tracks::Direction::Down, {
        Tracks::Direction::Right,
        Tracks::Direction::Down,
        Tracks::Direction::Left,
    }},
    { Tracks::Direction::Left, {
        Tracks::Direction::Down,
        Tracks::Direction::Left,
        Tracks::Direction::Up,
    }},
    { Tracks::Direction::Right, {
        Tracks::Direction::Up,
        Tracks::Direction::Right,
        Tracks::Direction::Down,
    }}
};

string Tracks::dir_str(const Direction& dir) {
    string str;
    switch (dir) {
        case Direction::Up: str = "Up"; break;
        case Direction::Down: str = "Down"; break;
        case Direction::Left: str = "Left"; break;
        case Direction::Right: str = "Right"; break;
        default: str = "UNKNOWN"; break;
    }
    return str;
}


int main (int argc, char** argv) {

    Tracks tracks;

    // read all non-empty lines
    for (string input; getline(cin, input);) {
        if (all_of(input.begin(), input.end(), [](auto ch){ return isspace(ch); })) 
            break;
        tracks.add_row(input);
    } 

    optional<int_pair> collision;
    while(!collision.has_value()) {
        collision = tracks.tick();
    }
    auto coords = collision.value(); 
    cout << "collision @ (" << coords.second << "," << coords.first <<  ")" << endl;

    return 0;
}

