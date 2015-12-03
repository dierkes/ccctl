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
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <deque>

typedef ccc::ConsistentDeque<int, uint64_t, 10> ContainerOfInts;
typedef ccc::ConsistentDeque<tPOD, uint64_t, 10> ContainerOfPODs;
typedef ccc::ConsistentDeque<cNoPOD, uint64_t, 10> ContainerOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(ConsistentDeque, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_standard_layout<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs, ContainerOfNonPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentDeque, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentDeque, TestOfRegularContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentDeque, TestOfStaticContainer, ContainerImplementations);

typedef RefPair<ccc::ConsistentDeque<int, uint64_t, 10>, std::deque<int> > RefContainerOfInts;
typedef RefPair<ccc::ConsistentDeque<tPOD, uint64_t, 10>, std::deque<tPOD> > RefContainerOfPODs;
typedef RefPair<ccc::ConsistentDeque<cNoPOD, uint64_t, 10>, std::deque<cNoPOD> > RefContainersOfNonPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs, RefContainersOfNonPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentDeque, TestOfSequenceContainer, RefContainerImplementations);
