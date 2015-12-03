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
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <list>

typedef ccc::ConsistentList<int, uint64_t, 10> ContainerOfInts;
typedef ccc::ConsistentList<tPOD, uint64_t, 10> ContainerOfPODs;
typedef ccc::ConsistentList<cNoPOD, uint64_t, 10> ContainerOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(ConsistentList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_standard_layout<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs, ContainerOfNonPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, TestOfRegularContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, TestOfStaticContainer, ContainerImplementations);

typedef RefPair<ccc::ConsistentList<int, uint64_t, 10>, std::list<int> > RefContainerOfInts;
typedef RefPair<ccc::ConsistentList<tPOD, uint64_t, 10>, std::list<tPOD> > RefContainerOfPODs;
typedef RefPair<ccc::ConsistentList<cNoPOD, uint64_t, 10>, std::list<cNoPOD> > RefContainersOfNonPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs, RefContainersOfNonPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentList, TestOfSequenceContainer, RefContainerImplementations);
