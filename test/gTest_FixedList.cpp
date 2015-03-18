/*
 * gTest_FixedList.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */


#include <gtest/gtest.h>

#include <ccc/fixed_list.h>

TEST(FixedList, experimental)
{
    typedef ccc::FixedList<int, std::size_t> FixedContainer;
    FixedContainer c(10);
}
