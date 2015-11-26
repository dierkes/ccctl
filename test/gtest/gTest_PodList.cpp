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

#include <ccc/pod_list.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <list>

typedef ccc::PodList<int, uint64_t, 10> ContainerOfInts;
typedef ccc::PodList<tPOD, uint64_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PodList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::PodList<int, uint16_t, 10>, std::list<int> > RefContainerOfInts;
typedef reftest<ccc::PodList<tPOD, uint16_t, 10>, std::list<tPOD> > RefContainerOfPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfSequenceContainer, RefContainerImplementations);
