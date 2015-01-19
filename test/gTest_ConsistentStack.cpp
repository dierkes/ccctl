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

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#include <type_traits>
#include <vector>

typedef ccc::ConsistentStack<char, std::size_t, 10> StackOfChars;
typedef ccc::ConsistentStack<ccc::tPOD, std::size_t, 10> StackOfPODs;
typedef ccc::ConsistentStack<ccc::cNoPOD, std::size_t, 10> StackOfNonPODs;

template<> std::size_t StaticContainerTest<StackOfChars>::m_Capacity = 10;
template<> std::size_t StaticContainerTest<StackOfPODs>::m_Capacity = 10;
template<> std::size_t StaticContainerTest<StackOfNonPODs>::m_Capacity = 10;

#if (__cplusplus >= 201103L)
TEST(ConsistentStack, TypeTraits_Cpp11)
{
    EXPECT_FALSE(std::is_pod<StackOfChars>::value);
    EXPECT_TRUE(std::is_standard_layout<StackOfChars>::value);
}
#endif

typedef ::testing::Types<StackOfChars, StackOfPODs, StackOfNonPODs> StackImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentStack, RegularContainerTest, StackImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentStack, StaticContainerTest, StackImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentStack, SequenceContainerTest, StackImplementations);

typedef reftest<ccc::ConsistentStack<char, std::size_t, 10>, std::vector<char> > RefStackOfChars;
typedef reftest<ccc::ConsistentStack<ccc::tPOD, std::size_t, 10>, std::vector<ccc::tPOD> > RefStackOfPODs;
typedef reftest<ccc::ConsistentStack<ccc::cNoPOD, std::size_t, 10>, std::vector<ccc::cNoPOD> > RefStackOfNonPODs;

typedef ::testing::Types<RefStackOfChars, RefStackOfPODs, RefStackOfNonPODs> RefStackImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentStack, SequenceContainerRefTest, RefStackImplementations);
