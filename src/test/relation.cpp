//
// Created by Zhou,Baochuan on 18/6/6.
//

#include "common.h"
#include "relation.h"
#include <gtest/gtest.h>

using namespace work;

// 人物关系规则提取
TEST(relation, regular_extract)
{
    // 对应数据文件是否存在
    ASSERT_TRUE(boost::filesystem::is_regular_file("./data/relation/D_map_family"));
    ASSERT_TRUE(boost::filesystem::exists("./data/relation/F_map_family"));
    ASSERT_TRUE(boost::filesystem::exists("./data/relation/tmp_relationship_modules"));

    // 数据接口是否异常
    ptree root;
    read_ini("conf/work.ini", root);
    string url = root.get<string>("relationKg.url");
    string response = conv::between(Http::get(url), "UTF-8", "GBK");
    ASSERT_TRUE(!response.empty());
}
