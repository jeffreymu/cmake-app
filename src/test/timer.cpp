//
// Created by Zhou,Baochuan on 18/6/4.
//

#include "common.h"
#include <gtest/gtest.h>
#include <boost/progress.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost;
using namespace boost::gregorian;

// 日期展示
TEST(date, show)
{
    date d1(2018, 06, 02), d2(2018, 06, 04);
    // 计算相隔天数
    ASSERT_EQ(2, (d2 - d1).days());

    /*  date now(day_clock::local_day());
    cout << "今天:" << to_iso_extended_string(now) << endl;

    date yesterday = now - days(1);
    cout << "昨天:" << to_iso_extended_string(yesterday) << endl;

    date tomorrow = now + days(1);
    cout << "明天:" << to_iso_extended_string(tomorrow) << endl; */
}
