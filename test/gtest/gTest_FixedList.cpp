/*
 * gTest_FixedList.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */


#include <gtest/gtest.h>

#include <ccc/fixed_list.h>
#include <ccc/test/consistent_integers.h>

TEST(FixedList, experimental)
{
    typedef ccc::FixedList<int, uint64_t> FixedContainer;
    FixedContainer c(10);
}
