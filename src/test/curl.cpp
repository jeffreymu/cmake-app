//
// Created by Zhou,Baochuan on 18/6/4.
//

#include "common.h"
#include "http.h"
#include <gtest/gtest.h>

using namespace work;

// curl版本要求
TEST(curl, all)
{
    EXPECT_EQ(3, CURLVERSION_NOW);
    Http http;
    string url = "http://47.95.220.249/";
    ASSERT_FALSE(http.get(url).empty());
}

