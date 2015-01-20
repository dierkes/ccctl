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

#include <ccc/consistent_list.h>

#include "gTest_Container.h"

#include <type_traits>

typedef ccc::ConsistentList<char, std::size_t, 10> ListOfChars;
typedef ccc::ConsistentList<tPOD, std::size_t, 10> ListOfPODs;
typedef ccc::ConsistentList<cNoPOD, std::size_t, 10> ListOfNonPODs;

template<> std::size_t TestOfStaticContainer<ListOfChars>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ListOfPODs>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ListOfNonPODs>::m_Capacity = 10;

#if (__cplusplus >= 201103L)
TEST(ConsistentList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<ListOfChars>::value);
    EXPECT_TRUE(std::is_standard_layout<ListOfChars>::value);
}
#endif

typedef ::testing::Types<ListOfChars, ListOfPODs, ListOfNonPODs> ListImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, TestOfRegularContainer, ListImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, TestOfStaticContainer, ListImplementations);

//typedef ::testing::Types<reftest<> >

//INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, StaticContainerTest, ListImplementations);

typedef ccc::ConsistentList<cThrowWhenConstructed, std::size_t, 10> ListOfThrowers;

TEST(ConsistentList, ElementInitialization)
{
    // It would be nice, if the elements were not constructed. However, that's not the case right now.
//    ListOfNonPODs c = ListOfNonPODs();
//    EXPECT_NE(ListOfNonPODs::value_type(), c.m_Values[0]);

//    EXPECT_NO_THROW(ListOfThrowers());
}
