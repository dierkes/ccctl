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

typedef ::testing::Types<
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 2>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 4>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 8>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 16>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<2, 2>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<2, 2> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<4, 4>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<4, 4> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<8, 8>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<8, 8> > >,
#if !(defined _MSC_VER && defined CCC_X86)
        RefPair<ccc::PodDeque<ccc_test::Pod<16, 16>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<16, 16> > >,
#endif
        RefPair<ccc::PodDeque<ccc_test::Pod<2, 1>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<2, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<4, 1>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<4, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<8, 1>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<8, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<16, 1>, uint8_t, 10, 1>, std::deque<ccc_test::Pod<16, 1> > >,
        // Using raw mem ops:
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 2, true>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 4, true>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 8, true>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<1, 1>, uint8_t, 10, 16, true>, std::deque<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<2, 2>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<2, 2> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<4, 4>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<4, 4> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<8, 8>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<8, 8> > >,
#if !(defined _MSC_VER && defined CCC_X86)
        RefPair<ccc::PodDeque<ccc_test::Pod<16, 16>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<16, 16> > >,
#endif
        RefPair<ccc::PodDeque<ccc_test::Pod<2, 1>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<2, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<4, 1>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<4, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<8, 1>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<8, 1> > >,
        RefPair<ccc::PodDeque<ccc_test::Pod<16, 1>, uint8_t, 10, 1, true>, std::deque<ccc_test::Pod<16, 1> > >
> RefPairTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PodDeque, TestOfSequenceContainer, RefPairTypes);

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
