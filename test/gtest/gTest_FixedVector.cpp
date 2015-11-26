/*
 * gTest_FixedVector.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */


#include <gtest/gtest.h>

#include <ccc/fixed_vector.h>

TEST(FixedVector, experimental)
{
    typedef ccc::FixedVector<int, std::size_t> FixedContainer;
    FixedContainer c(10);
}
