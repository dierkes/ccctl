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

#include <ccc/consistent_deque.h>

#include "gTest_Container.h"

#include <type_traits>

typedef ccc::ConsistentDeque<char, std::size_t, 10> DequeOfChars;
typedef ccc::ConsistentDeque<ccc::tPOD, std::size_t, 10> DequeOfPODs;
typedef ccc::ConsistentDeque<ccc::cNoPOD, std::size_t, 10> DequeOfNonPODs;

template<> std::size_t StaticContainerTest<DequeOfChars>::m_Capacity = 10;
template<> std::size_t StaticContainerTest<DequeOfPODs>::m_Capacity = 10;
template<> std::size_t StaticContainerTest<DequeOfNonPODs>::m_Capacity = 10;

#if (__cplusplus >= 201103L)
TEST(ConsistentDeque, TypeTraits_Cpp11)
{
    EXPECT_FALSE(std::is_pod<DequeOfChars>::value);
    EXPECT_TRUE(std::is_standard_layout<DequeOfChars>::value);
}
#endif

typedef ::testing::Types<DequeOfChars, DequeOfPODs, DequeOfNonPODs> DequeImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentDeque, RegularContainerTest, DequeImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentDeque, StaticContainerTest, DequeImplementations);
