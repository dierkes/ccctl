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
#include <ccc/array.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#include <type_traits>
#include <deque>

typedef ccc::PODDeque<int, std::size_t, 10> ContainerOfInts;
typedef ccc::PODDeque<tPOD, std::size_t, 10> ContainerOfPODs;
typedef ccc::PODDeque<cNoPOD, std::size_t, 10> ContainerOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(PODDeque, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs, ContainerOfNonPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODDeque, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODDeque, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODDeque, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::PODDeque<int, std::size_t, 10>, std::deque<int> > RefContainerOfInts;
typedef reftest<ccc::PODDeque<tPOD, std::size_t, 10>, std::deque<tPOD> > RefContainerOfPODs;
typedef reftest<ccc::PODDeque<cNoPOD, std::size_t, 10>, std::deque<cNoPOD> > RefContainersOfNonPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs, RefContainersOfNonPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODDeque, TestOfSequenceContainer, RefContainerImplementations);
