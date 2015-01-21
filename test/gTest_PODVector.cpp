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

#include <ccc/pod_vector.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <vector>

typedef ccc::PODVector<int, std::size_t, 10> ContainerOfInts;
typedef ccc::PODVector<tPOD, std::size_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PODVector, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::PODVector<int, std::size_t, 10>, std::vector<int> > RefContainerOfInts;
typedef reftest<ccc::PODVector<tPOD, std::size_t, 10>, std::vector<tPOD> > RefContainerOfPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfSequenceContainer, RefContainerImplementations);
