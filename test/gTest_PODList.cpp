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
#include <type_traits>

#include <gtest/gtest.h>
#include <ccc/iterator.h>
#include <ccc/pod_list.h>

#include <list>
#include <iostream>
#include <vector>

#include <iostream>

#include "gTest_Container.h"

template <class T, class Alloc, class SizeType, SizeType Capacity>
bool operator==(ccc::PODList<T, SizeType, Capacity>& lhs, std::list<T, Alloc>& rhs)
{
    typedef ccc::PODList<T, SizeType, Capacity> tLHS;
    typedef std::list<T, Alloc> tRHS;
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

template <class T, class SizeType, SizeType Capacity>
bool operator==(ccc::PODList<T, SizeType, Capacity>& lhs, std::vector<T>& rhs)
{
    typedef ccc::PODList<T, SizeType, Capacity> tLHS;
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

typedef int value_type;
typedef unsigned int size_type;
const size_type Capacity = 7;

typedef ccc::PODList<int, size_type, Capacity> ListOfInts;
typedef ccc::PODList<ccc::tPOD, size_type, Capacity> ListOfPODs;
typedef ccc::PODList<ccc::cNoPOD, size_type, Capacity> ListOfNonPODs;

TEST(PODList, TypeTraits)
{
}

#if (__cplusplus >= 201103L)
TEST(PODList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ccc::tPOD>::value);
    EXPECT_FALSE(std::is_pod<ccc::cNoPOD>::value);
    EXPECT_TRUE(std::is_pod<ListOfInts>::value);
    EXPECT_TRUE(std::is_pod<ListOfPODs>::value);
    EXPECT_FALSE(std::is_pod<ListOfNonPODs>::value);
}
#endif

TEST(PODList, initialization)
{
    ListOfInts A = ListOfInts();
    EXPECT_EQ(0, A.size());
    EXPECT_EQ(0, A.m_Nodes[0].m_Prev);
    EXPECT_EQ(0, A.m_Nodes[0].m_Next);
    EXPECT_EQ(0, A.m_Deallocated.size());
    EXPECT_NO_THROW(A.emplace(A.end()));
    A.clear();
    EXPECT_EQ(0, A.size());
    EXPECT_EQ(0, A.m_Nodes[0].m_Prev);
    EXPECT_EQ(0, A.m_Nodes[0].m_Next);
    EXPECT_EQ(1, A.m_Deallocated.size());

    ListOfInts B;
    EXPECT_EQ(0, B.size());
    EXPECT_EQ(0, B.m_Nodes[0].m_Prev);
    EXPECT_EQ(0, B.m_Nodes[0].m_Next);
    EXPECT_EQ(0, B.m_Deallocated.size());
    EXPECT_NO_THROW(B.emplace(B.end()));

    ListOfPODs C = ListOfPODs();
    EXPECT_EQ(0, C.size());
    EXPECT_EQ(0, C.m_Nodes[0].m_Prev);
    EXPECT_EQ(0, C.m_Nodes[0].m_Next);
    EXPECT_EQ(0, C.m_Deallocated.size());
    EXPECT_NO_THROW(C.emplace(C.end()));

//    ListOfPODs D;
//    EXPECT_EQ(0, D.size());
//    EXPECT_EQ(0, D.m_Nodes[0].m_Prev);
//    EXPECT_EQ(0, D.m_Nodes[0].m_Next);
//    EXPECT_EQ(0, D.m_Deallocated.size());
//    EXPECT_EQ(D.begin(), D.end());
//    EXPECT_NO_THROW(D.emplace_back());
//    EXPECT_NO_THROW(D.emplace(D.end()));

    ListOfNonPODs E = ListOfNonPODs();
    EXPECT_EQ(0, E.size());
    EXPECT_EQ(0, E.m_Nodes[0].m_Prev);
    EXPECT_EQ(0, E.m_Nodes[0].m_Next);
    EXPECT_EQ(0, E.m_Deallocated.size());
//    EXPECT_NO_THROW(E.emplace_back());
    EXPECT_NO_THROW(E.emplace(E.end()));
//    EXPECT_EQ(0, E.size());
//    EXPECT_EQ(1, E.m_Nodes[0].m_Prev);
//    EXPECT_EQ(1, E.m_Nodes[0].m_Next);
//    EXPECT_EQ(0, E.m_Deallocated.size());

//    ListOfNonPODs F;
//    EXPECT_EQ(0, F.size());
//    EXPECT_EQ(0, F.m_Nodes[0].m_Prev);
//    EXPECT_EQ(0, F.m_Nodes[0].m_Next);
//    EXPECT_EQ(0, F.m_Deallocated.size());
//    F.clear();
//    EXPECT_EQ(0, F.size());
//    EXPECT_EQ(0, F.m_Nodes[0].m_Prev);
//    EXPECT_EQ(0, F.m_Nodes[0].m_Next);
//    EXPECT_EQ(0, F.m_Deallocated.size());
//    EXPECT_NO_THROW(F.emplace_back());
//    EXPECT_NO_THROW(F.emplace(F.end()));

    ccc::Test_ValueInitialization<ListOfInts>();
    ccc::Test_ValueInitialization<ListOfPODs>();
    ccc::Test_ValueInitialization<ListOfNonPODs>();
    ccc::Test_DefaultInitialization<ListOfInts>();
    ccc::Test_DefaultInitialization<ListOfPODs>();
    ccc::Test_DefaultInitialization<ListOfNonPODs>();
}

typedef ::testing::Types<ListOfInts, ListOfPODs> ListImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODList, BaseContainerTest, ListImplementations);

TEST(PODList, basic)
{
    typedef std::vector<value_type> CmpList;
    typedef ccc::PODList<value_type, size_type, Capacity> MyList;
    MyList A = MyList();
    std::list<value_type> C = std::list<value_type>();
    value_type array[Capacity] = { 5 };
    std::vector<value_type> V = std::vector<value_type>(0);
    V << 1;
    V.clear();
    EXPECT_EQ(Capacity, A.max_size());
    EXPECT_EQ(static_cast<size_type>(0), A.size());
    EXPECT_NO_THROW(A.push_back(5)); // (5)
    C.push_back(5);
    EXPECT_TRUE(A == C);
    EXPECT_TRUE(A == (V << 5));
    V.clear();
    EXPECT_EQ(5, A.front());
    EXPECT_EQ(5, A.back());
    EXPECT_NO_THROW(A.pop_front());
    EXPECT_EQ(static_cast<size_type>(0), A.size());
//    EXPECT_EQ(value_type(), A[0]); // value should be default initialized
    EXPECT_NO_THROW(A.push_front(3));
    EXPECT_TRUE(A == (V << 3));
    V.clear();
    EXPECT_NO_THROW(A.push_back(123));
    EXPECT_TRUE(A == (V << 3 << 123));
    V.clear();
    EXPECT_NO_THROW(A.push_front(234));
    EXPECT_TRUE(A == (V << 234 << 3 << 123));
    V.clear();
    EXPECT_EQ(234, A.front());
    EXPECT_EQ(123, A.back());
    A.insert(A.begin(), 2);
    EXPECT_TRUE(A == (V << 2 << 234 << 3 << 123));
    V.clear();
    A.insert(A.end(), 45);
    EXPECT_TRUE(A == (V << 2 << 234 << 3 << 123 << 45));
    V.clear();
    A.insert(ccc::next(A.begin()), 13);
    EXPECT_TRUE(A == (V << 2 << 13 << 234 << 3 << 123 << 45));
    V.clear();
    A.insert(ccc::prev(A.end(), 2), 15);
    EXPECT_TRUE(A == (V << 2 << 13 << 234 << 3 << 15 << 123 << 45));
    V.clear();
    EXPECT_THROW(A.push_back(678), std::bad_alloc);
    EXPECT_THROW(A.push_front(678), std::bad_alloc);
    EXPECT_TRUE(A == (V << 2 << 13 << 234 << 3 << 15 << 123 << 45));
    V.clear();
    A.erase(A.begin());
    EXPECT_TRUE(A == (V << 13 << 234 << 3 << 15 << 123 << 45));
    V.clear();
    A.erase(next(A.begin()));
    EXPECT_TRUE(A == (V << 13 << 3 << 15 << 123 << 45));
    V.clear();
    A.erase(ccc::prev(A.end(), 2));
    EXPECT_TRUE(A == (V << 13 << 3 << 15 << 45));
    V.clear();
    EXPECT_NO_THROW(A.emplace_front());
    EXPECT_TRUE(A == (V << value_type() << 13 << 3 << 15 << 45));
    V.clear();
    EXPECT_NO_THROW(A.emplace_back());
    EXPECT_TRUE(A == (V << value_type() << 13 << 3 << 15 << 45 << value_type()));
    V.clear();
    MyList::iterator itEmplaced = A.emplace(ccc::next(A.begin(), 2));
    EXPECT_TRUE(A == (V << value_type() << 13 << value_type() << 3 << 15 << 45 << value_type()));
    V.clear();
    *itEmplaced = 5;
    EXPECT_TRUE(A == (V << value_type() << 13 << 5 << 3 << 15 << 45 << value_type()));
    V.clear();
    EXPECT_THROW(A.emplace_front(), std::bad_alloc);
    EXPECT_THROW(A.emplace_back(), std::bad_alloc);
    EXPECT_THROW(A.emplace(A.begin()), std::bad_alloc);
//    EXPECT_TRUE(false);
}
