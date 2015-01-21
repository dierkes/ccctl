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

typedef ccc::PODList<int, std::size_t, 10> ContainerOfInts;
typedef ccc::PODList<tPOD, std::size_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PODList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::PODList<int, std::size_t, 10>, std::list<int> > RefContainerOfInts;
typedef reftest<ccc::PODList<tPOD, std::size_t, 10>, std::list<tPOD> > RefContainerOfPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfSequenceContainer, RefContainerImplementations);
