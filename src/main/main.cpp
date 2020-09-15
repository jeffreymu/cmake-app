#include "common.h"
#include <boost/algorithm/string.hpp>


using namespace boost;
int main(int argc, char** argv) {
    string src = " OK";
    algorithm::trim(src);
    cout << src << endl;
    return 0;
}


