/*
 * gTest_FixedDeque.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */

#include <gtest/gtest.h>

#include <ccc/fixed_deque.h>
#include <ccc/test/consistent_integers.h>

TEST(FixedDeque, experimental)
{
    typedef ccc::FixedDeque<int, uint64_t> FixedContainer;
    FixedContainer c(10);
}


