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

#include <ccc/pod_deque.h>
#include <ciso646>

#include <gtest/gtest.h>
#include <deque>
#include <iostream>
#include <vector>

// a lot of problems because of const
//template <class T, class Alloc, class SizeType, SizeType Capacity, bool UseRawMemOps>
//bool operator==(const ccc::ArrayDeque<T, SizeType, Capacity, UseRawMemOps>& lhs, const std::deque<T, Alloc>& rhs)
//{
//    typedef const ccc::ArrayDeque<T, SizeType, Capacity, UseRawMemOps> tLHS;
//    typedef std::deque<T, Alloc> tRHS;
//    bool bSuccess = false;
//    if (lhs.size() == rhs.size())
//    {
//        bSuccess = true;
//        tLHS::const_iterator itlhs = lhs.begin();
//        tRHS::const_iterator itrhs = rhs.begin();
//        while (itlhs != lhs.end())
//        {
//            bSuccess &= *itlhs == *itrhs;
//            ++itlhs;
//            ++itrhs;
//        }
//    }
//    return bSuccess;
//}

template <class T, class Alloc, class SizeType, SizeType Capacity, bool UseRawMemOps>
bool operator==(ccc::PODDeque<T, SizeType, Capacity, UseRawMemOps>& lhs, std::deque<T, Alloc>& rhs)
{
    typedef ccc::PODDeque<T, SizeType, Capacity, UseRawMemOps> tLHS;
    typedef std::deque<T, Alloc> tRHS;
    bool bSuccess = false;
    if (lhs.size() == rhs.size())
    {
        bSuccess = true;
        typename tLHS::iterator itlhs = lhs.begin();
        typename tRHS::iterator itrhs = rhs.begin();
        while (itlhs != lhs.end())
        {
            bSuccess &= *itlhs == *itrhs;
            ++itlhs;
            ++itrhs;
        }
    }
    return bSuccess;
}

template <class T>
std::vector<T>& operator<<(std::vector<T>& v, const T& o)
{
  v.push_back(o);
  return v;
}

template <class T, class SizeType, SizeType Capacity, bool UseRawMemOps>
bool operator==(ccc::PODDeque<T, SizeType, Capacity, UseRawMemOps>& lhs, std::vector<T>& rhs)
{
    typedef ccc::PODDeque<T, SizeType, Capacity, UseRawMemOps> tLHS;
    typedef const std::vector<T> tRHS;
    bool bSuccess = false;
    if (lhs.size() == rhs.size())
    {
        bSuccess = true;
        typename tLHS::iterator itlhs = lhs.begin();
        typename tRHS::iterator itrhs = rhs.begin();
        while (itlhs != lhs.end())
        {
            bSuccess &= *itlhs == *itrhs;
            ++itlhs;
            ++itrhs;
        }
    }
    return bSuccess;
}

TEST(PODDeque, basic)
{
    typedef int value_type;
    typedef unsigned int size_type;
    const size_type Capacity = 7;
    typedef std::vector<value_type> CmpList;
    typedef ccc::PODDeque<value_type, size_type, Capacity, false> MyDeque;
    MyDeque A = MyDeque();
    std::deque<value_type> C = std::deque<value_type>();
//    value_type array[Capacity] = {5};
    std::vector<value_type> V = std::vector<value_type>(0);
    V << 1; V.clear();
    EXPECT_EQ(Capacity, A.max_size());
    EXPECT_EQ(static_cast<size_type>(0), A.size());
    EXPECT_NO_THROW(A.push_back(5)); // (5)
    C.push_back(5);
    EXPECT_TRUE(A == C);
    EXPECT_TRUE(A == (V << 5)); V.clear();
    EXPECT_EQ(5, A.front());
    EXPECT_EQ(5, A.back());
    EXPECT_NO_THROW(A.pop_front());
    EXPECT_EQ(static_cast<size_type>(0), A.size());
    EXPECT_EQ(value_type(), A[0]); // value should be default initialized
    EXPECT_NO_THROW(A.push_front(3));
    EXPECT_TRUE(A == (V << 3)); V.clear();
    EXPECT_NO_THROW(A.push_back(123));
    EXPECT_TRUE(A == (V << 3 << 123)); V.clear();
    EXPECT_NO_THROW(A.push_front(234));
    EXPECT_TRUE(A == (V << 234 << 3 << 123)); V.clear();
    EXPECT_EQ(234, A.front());
    EXPECT_EQ(123, A.back());
    A.insert(A.begin(), 2);
    EXPECT_TRUE(A == (V << 2 << 234 << 3 << 123)); V.clear();
    A.insert(A.end(), 45);
    EXPECT_TRUE(A == (V << 2 << 234 << 3 << 123 << 45)); V.clear();
    A.insert(A.begin() + 1, 13);
    EXPECT_TRUE(A == (V << 2 << 13 << 234 << 3 << 123 << 45)); V.clear();
    A.insert(A.end() - 2, 15);
    EXPECT_TRUE(A == (V << 2 << 13 << 234 << 3 << 15 << 123 << 45)); V.clear();
    EXPECT_THROW(A.push_back(678), std::bad_alloc);
    EXPECT_THROW(A.push_front(678), std::bad_alloc);
    EXPECT_TRUE(A == (V << 2 << 13 << 234 << 3 << 15 << 123 << 45)); V.clear();
    A.erase(A.begin());
    EXPECT_TRUE(A == (V << 13 << 234 << 3 << 15 << 123 << 45)); V.clear();
    A.erase(A.begin() + 1);
    EXPECT_TRUE(A == (V << 13 << 3 << 15 << 123 << 45)); V.clear();
    A.erase(A.end() - 2);
    EXPECT_TRUE(A == (V << 13 << 3 << 15 << 45)); V.clear();
    EXPECT_NO_THROW(A.emplace_front());
    EXPECT_TRUE(A == (V << value_type() << 13 << 3 << 15 << 45)); V.clear();
    EXPECT_NO_THROW(A.emplace_back());
    EXPECT_TRUE(A == (V << value_type() << 13 << 3 << 15 << 45 << value_type())); V.clear();
    MyDeque::iterator itEmplaced = A.emplace(A.begin() + 2);
    EXPECT_TRUE(A == (V << value_type() << 13 << value_type() << 3 << 15 << 45 << value_type())); V.clear();
    *itEmplaced = 5;
    EXPECT_TRUE(A == (V << value_type() << 13 << 5 << 3 << 15 << 45 << value_type())); V.clear();
    EXPECT_THROW(A.emplace_front(), std::bad_alloc);
    EXPECT_THROW(A.emplace_back(), std::bad_alloc);
    EXPECT_THROW(A.emplace(A.begin()), std::bad_alloc);
//    EXPECT_TRUE(false);
}
