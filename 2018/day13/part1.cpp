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

        //=================================
        //      Internal classes
        //=================================
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

        //=================================
        //      Static members
        //=================================
        const static string track_chars;
        const static string cart_chars;
        const static map<Tracks::Direction, map<char, Tracks::Direction>> next_dir;
        const static map<Tracks::Direction, array<Tracks::Direction, 3>> next_dir_intersection;
        static string dir_str(const Direction& dir);

        //=================================
        //      Containers, variables
        //=================================
        map<int_pair, char> tracks;
        vector<Cart> carts;
        size_t row_count;

        //=================================
        //      Methods
        //=================================
        
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
        }

        // move a cart forward one track
        void tick_cart(Cart& cart) {
            // update coordinates
            int_pair next_coord;
            switch (cart.dir) {
                case Direction::Up:    next_coord = make_pair(--cart.row, cart.col); break;
                case Direction::Down:  next_coord = make_pair(++cart.row, cart.col); break;
                case Direction::Left:  next_coord = make_pair(cart.row, --cart.col); break;
                case Direction::Right: next_coord = make_pair(cart.row, ++cart.col); break;
                default: break;
            }
           
            // get the next track this cart will touch, and update its direction
            auto next_track = tracks.at(next_coord);
            Direction new_dir;
            if (next_track == '+') {
                new_dir = next_dir_intersection.at(cart.dir).at(cart.intersections++ % 3);
            } else {
                new_dir = next_dir.at(cart.dir).at(next_track);
            }
            cart.dir = new_dir;
        }
       
        // check for collisions
        //      i.e. >1 cart at the same coords
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
        Tracks() : row_count(0) 
        { } 

        // parse a line from input and add a row to the tracks
        void add_row(const string& line) {
            for (size_t i=0; i < line.size(); ++i) {
                auto chr = line[i];

                // no track
                if (isspace(chr)) {
                    continue;
                }
                // normal track
                else if (track_chars.find(chr) != string::npos) {
                    tracks[make_pair(row_count, i)] = chr;
                }
                // cart
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
                // unsupported character
                else {
                    cerr << "Unrecognized char '" << chr << "' in input" << endl;
                }
            }
            // adjust dimensions
            ++row_count;
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

        // stringify each cart info for displaying
        string info() {
            ostringstream ss;
            for (auto& cart : carts) {
                ss << "\t" << cart.str() << endl;
            }
            return ss.str();
        }

};

//=================================
//      Static members
//=================================

// chars representing tracks
const string Tracks::track_chars = "|-/\\+";
// chars representing carts
const string Tracks::cart_chars = "^v<>";

// lookup new direction based on current direction and next track touched
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

// lookup new direction based on current direction and current intersection behavior
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

// stringify Direction enum for displaying
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

