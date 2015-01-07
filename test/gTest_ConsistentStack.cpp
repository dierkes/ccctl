/**
 *
 * @file
 *
 * @author Frank Dierkes
 *
 * $LastChangedBy$
 * $Date$
 * $Revision$
 *
 * @remarks
 *
 */

#include <ccc/consistent_stack.h>

#include <gtest/gtest.h>

TEST(ConsistentStack, basic)
{
    typedef int value_type;
    typedef unsigned int size_type;
    const size_type Capacity = 7;
    ccc::ConsistentStack<value_type, size_type, Capacity, false> A = ccc::ConsistentStack<value_type, size_type, Capacity, false>();
    EXPECT_EQ(Capacity, A.max_size());
}
