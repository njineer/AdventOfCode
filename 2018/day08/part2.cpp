#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

int parse(istream& is, int level=0) {
    int child_count, md_count, md, md_sum=0;
    vector<int> children;
    vector<int> metadata;

    // read counts for this node
    is >> child_count >> md_count;

    // recursively accumulate metadata sum for each child
    for (int i=0; i < child_count; i++) {
        children.push_back(parse(is, level+1));
    } 

    // parse metadata values
    for (int i=0; i < md_count; i++) {
        is >> md;
        metadata.push_back(md);
    }

    // if this node has no children, sum the metadata values
    if (children.empty()) {
        md_sum += accumulate(metadata.begin(), metadata.end(), 0);
    } 
    else {
        // use the metadata values as indices into this node's childnren
        for (auto idx : metadata) {
            if (idx <= children.size()) {
                md_sum += children[idx-1];
            }
        }
    }
   
    // return total/recursive metadata sum for this node
    return md_sum;
}

int main (int argc, char** argv) {
    //cout << endl << "metadata total: " << parse(cin) << endl;
    cout << parse(cin) << endl;
    return 0;
}
