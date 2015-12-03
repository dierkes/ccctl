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

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <deque>

typedef ccc::PodDeque<int, uint64_t, 10> ContainerOfInts;
typedef ccc::PodDeque<tPOD, uint64_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PodDeque, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PodDeque, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodDeque, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodDeque, TestOfStaticContainer, ContainerImplementations);

typedef RefPair<ccc::PodDeque<int, uint64_t, 10>, std::deque<int> > RefContainerOfInts;
typedef RefPair<ccc::PodDeque<tPOD, uint64_t, 10>, std::deque<tPOD> > RefContainerOfPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PodDeque, TestOfSequenceContainer, RefContainerImplementations);

TEST(PodDeque, ModuloOperation)
{
    typedef ccc::PodDeque<int, uint64_t, 10> C_size_t;
    EXPECT_EQ(9, C_size_t::modulo(-1, 10));
    EXPECT_EQ(1, C_size_t::modulo(1, 10));
    EXPECT_EQ(9, C_size_t::modulo(-11, 10));
    EXPECT_EQ(1, C_size_t::modulo(11, 10));
    typedef ccc::PodDeque<int, unsigned char, 10> C_uint8_t;
    EXPECT_EQ(9, C_uint8_t::modulo(-1, 10));
    EXPECT_EQ(1, C_uint8_t::modulo(1, 10));
    EXPECT_EQ(9, C_uint8_t::modulo(-11, 10));
    EXPECT_EQ(1, C_uint8_t::modulo(11, 10));
}
