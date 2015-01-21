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

#include <ccc/consistent_vector.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <vector>

typedef ccc::ConsistentVector<int, std::size_t, 10> ContainerOfInts;
typedef ccc::ConsistentVector<tPOD, std::size_t, 10> ContainerOfPODs;
typedef ccc::ConsistentVector<cNoPOD, std::size_t, 10> ContainerOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(ConsistentVector, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_standard_layout<ContainerOfPODs>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs, ContainerOfNonPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfRegularContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::ConsistentVector<int, std::size_t, 10>, std::vector<int> > RefContainerOfInts;
typedef reftest<ccc::ConsistentVector<tPOD, std::size_t, 10>, std::vector<tPOD> > RefContainerOfPODs;
typedef reftest<ccc::ConsistentVector<cNoPOD, std::size_t, 10>, std::vector<cNoPOD> > RefContainerOfNonPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs, RefContainerOfNonPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfSequenceContainer, RefContainerImplementations);
