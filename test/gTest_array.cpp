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

#include <gtest/gtest.h>
#include <ccc/array.h>

//LINK_LOGGING_TO_COUT;

TEST(Array, basic)
{
    typedef int value_type;
    typedef unsigned int size_type;
    const size_type Capacity = 10;
    ccc::Array<value_type, size_type, Capacity> A;
    EXPECT_EQ(Capacity, A.max_size());
    EXPECT_EQ(Capacity, A.size());
    const value_type InitialValue = 5;
    A.fill(InitialValue);
    bool IsInitializedCorrectly = true;
    for (ccc::Array<value_type, size_type, Capacity>::iterator it = A.begin(); it != A.end(); ++it)
    {
        IsInitializedCorrectly &= (InitialValue == *it);
    }
    EXPECT_TRUE(IsInitializedCorrectly);
}
