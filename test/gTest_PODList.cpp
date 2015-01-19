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

#include <type_traits>
#include <vector>

typedef ccc::PODList<char, std::size_t, 10> ContainerOfChars;
typedef ccc::PODList<tPOD, std::size_t, 10> ContainerOfPODs;
typedef ccc::PODList<cNoPOD, std::size_t, 10> ContainerOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(PODList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfChars>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<ContainerOfChars>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfChars, ContainerOfPODs, ContainerOfNonPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::PODList<char, std::size_t, 10>, std::vector<char> > RefContainerOfChars;
typedef reftest<ccc::PODList<tPOD, std::size_t, 10>, std::vector<tPOD> > RefContainerOfPODs;
typedef reftest<ccc::PODList<cNoPOD, std::size_t, 10>, std::vector<cNoPOD> > RefContainerOfNonPODs;

typedef ::testing::Types<RefContainerOfChars, RefContainerOfPODs, RefContainerOfNonPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODList, TestOfSequenceContainer, RefContainerImplementations);
