//
// Created by Zhou,Baochuan on 18/6/1.
//

#include "common.h"
#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    cout << "Boost 版本:" << BOOST_VERSION << endl;
    return RUN_ALL_TESTS();
}


