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

#include <ciso646>

#include <gtest/gtest.h>
#include <ccc/array_deque.h>.h>

TEST(ArrayDeque, basic)
{
    typedef int value_type;
    typedef unsigned int size_type;
    const size_type Capacity = 7;
    ccc::ArrayDeque<value_type, size_type, Capacity, false> A = ccc::ArrayDeque<value_type, size_type, Capacity, false>();
    EXPECT_EQ(Capacity, A.max_size());
    EXPECT_EQ(0, A.size());
    EXPECT_TRUE(A.push_back(5)); // (5)
    EXPECT_EQ(5, A.front());
    EXPECT_EQ(5, A.back());
    EXPECT_TRUE(A.pop_front()); // ()
    EXPECT_EQ(0, A.size());
    EXPECT_EQ(value_type(), A[0]);
    EXPECT_TRUE(A.push_front(3)); // (3)
    EXPECT_EQ(3, A[0]);
    EXPECT_TRUE(A.push_back(123)); // (3, 123)
    EXPECT_TRUE((3 == A[0]) and (123 == A[1]));
    EXPECT_TRUE(A.push_front(234)); // (234, 3, 123)
    EXPECT_TRUE((234 == A[0]) and (3 == A[1]) and (123 == A[2]));
    EXPECT_EQ(234, A.front());
    EXPECT_EQ(123, A.back());
    A.insert(A.begin(), 2); // (2, 234, 3, 123)
    A.insert(A.end(), 45); // (2, 234, 3, 123, 45)
    A.insert(A.begin() + 1, 13); // (2, 13, 234, 3, 123, 45)
    A.insert(A.end() - 2, 15); // (2, 13, 234, 3, 15, 123, 45)
    EXPECT_TRUE((2 == A[0]) and (13 == A[1]) and (234 == A[2]) and (3 == A[3]) and (15 == A[4]) and (123 == A[5]) and (45 == A[6]));
    EXPECT_FALSE(A.push_back(678));
    EXPECT_FALSE(A.push_front(678));
    EXPECT_TRUE((2 == A[0]) and (13 == A[1]) and (234 == A[2]) and (3 == A[3]) and (15 == A[4]) and (123 == A[5]) and (45 == A[6]));
    A.erase(A.begin()); // (13, 234, 3, 15, 123, 45)
    EXPECT_TRUE((13 == A[0]) and (234 == A[1]) and (3 == A[2]) and (15 == A[3]) and (123 == A[4]) and (45 == A[5]));
    A.erase(A.begin() + 1); // (13, 3, 15, 123, 45)
    EXPECT_TRUE((13 == A[0]) and (3 == A[1]) and (15 == A[2]) and (123 == A[3]) and (45 == A[4]));
    A.erase(A.end() - 2); // (13, 3, 15, 45)
    EXPECT_TRUE((13 == A[0]) and (3 == A[1]) and (15 == A[2]) and (45 == A[3]));
//    EXPECT_TRUE(false);
}
