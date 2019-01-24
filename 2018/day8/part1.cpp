#include <iostream>
#include <string>
#include <stack>

using namespace std;

inline void dec(int& x) {
    --x;
    if (x < 0) x = 0;
}

int main (int argc, char** argv) {

    int pending_children = 0;
    int md_sum = 0;
    stack<int> pending_metadata;
    int children, metadata, md;
    int input;

    // while there are 
    while(cin >> input) {
        cout << "[" << input << "] ";
        children = input;
        if (pending_children > 0) {
            cout << "new child" << endl;
            pending_children--;
            cin >> metadata;
            if (children == 0) {
                cout << "\t0 children, consuming " << metadata << " metadata: {";
                for (int i=0; i < metadata; i++){
                    cin >> md;
                    cout << md << ", ";
                    md_sum += md;
                }
                cout << "}" << endl;
            } else {
                cout << "\t" << children << " children, " << metadata << " metadata" << endl;
                pending_children += children;
                pending_metadata.push(metadata);
            }
        } else if (!pending_metadata.empty()) {
            md = input;
            metadata = pending_metadata.top();
            pending_metadata.pop();
            cout << "consuming " << metadata << " metadata: {";
            md_sum += md;
            for (int i=1; i < metadata; i++){
                cin >> md;
                cout << md << ", ";
                md_sum += md;
            }
            cout << "}" << endl;
        } else {
            cout << "new root" << endl;
            children = input;
            pending_children += children;
            cin >> metadata;
            pending_metadata.push(metadata);
            cout << "\t" << children << " children, " << metadata << " metadata" << endl;
        }
    }

    cout << endl << "metadata total: " << md_sum << endl;
    return 0;
}
