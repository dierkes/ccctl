/*
 * gTest_FixedVector.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */


#include <gtest/gtest.h>

#include <ccc/fixed_vector.h>
#include <ccc/test/consistent_integers.h>

TEST(FixedVector, experimental)
{
    typedef ccc::FixedVector<int, uint64_t> FixedContainer;
    FixedContainer c(10);
}
