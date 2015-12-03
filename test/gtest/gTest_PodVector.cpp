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

#include <cstddef>
#include <vector>

#include <ccc/pod_vector.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"
#include <ccc/test/consistent_types.h>

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

typedef ccc::PodVector<int, uint16_t, 10> ContainerOfInts;
typedef ccc::PodVector<tPOD, uint16_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PodVector, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PodVector, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodVector, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodVector, TestOfStaticContainer, ContainerImplementations);

typedef ::testing::Types<
        RefPair<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 2>, std::vector<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 4>, std::vector<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 8>, std::vector<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 16>, std::vector<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<2, 2>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<2, 2> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<4, 4>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<4, 4> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<8, 8>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<8, 8> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<16, 16>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<16, 16> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<2, 1>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<2, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<4, 1>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<4, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<8, 1>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<8, 1> > >,
        RefPair<ccc::PodVector<ccc_test::Pod<16, 1>, uint8_t, 10, 1>, std::vector<ccc_test::Pod<16, 1> > >
> RefPairTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PodVector, TestOfSequenceContainer, RefPairTypes);

typedef ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 2, 1> PodVector_1_1_1_2_1;

typedef ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 2, 2> PodVector_1_1_1_2_2;
typedef ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 2, 4> PodVector_1_1_1_2_4;
typedef ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 2, 8> PodVector_1_1_1_2_8;
typedef ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 2, 16> PodVector_1_1_1_2_16;

typedef ccc::PodVector<ccc_test::Pod<2, 2>, uint8_t, 2, 2> PodVector_2_2_1_2_2;
typedef ccc::PodVector<ccc_test::Pod<4, 4>, uint8_t, 2, 4> PodVector_4_4_1_2_4;
typedef ccc::PodVector<ccc_test::Pod<8, 8>, uint8_t, 2, 8> PodVector_8_8_1_2_8;
typedef ccc::PodVector<ccc_test::Pod<16, 16>, uint8_t, 2, 16> PodVector_16_16_1_2_16;

typedef ccc::PodVector<ccc_test::Pod<2, 1>, uint8_t, 2, 1> PodVector_2_1_1_2_1;
typedef ccc::PodVector<ccc_test::Pod<4, 1>, uint8_t, 2, 1> PodVector_4_1_1_2_1;
typedef ccc::PodVector<ccc_test::Pod<8, 1>, uint8_t, 2, 1> PodVector_8_1_1_2_1;
typedef ccc::PodVector<ccc_test::Pod<16, 1>, uint8_t, 2, 1> PodVector_16_1_1_2_1;

TEST(PodVector, ConsistentSize)
{
    EXPECT_EQ(3, sizeof(PodVector_1_1_1_2_1));

    EXPECT_EQ(4, sizeof(PodVector_1_1_1_2_2));
    EXPECT_EQ(8, sizeof(PodVector_1_1_1_2_4));
    EXPECT_EQ(16, sizeof(PodVector_1_1_1_2_8));
    EXPECT_EQ(32, sizeof(PodVector_1_1_1_2_16));

    EXPECT_EQ(6, sizeof(PodVector_2_2_1_2_2));
    EXPECT_EQ(12, sizeof(PodVector_4_4_1_2_4));
    EXPECT_EQ(24, sizeof(PodVector_8_8_1_2_8));
    EXPECT_EQ(48, sizeof(PodVector_16_16_1_2_16));

    EXPECT_EQ(5, sizeof(PodVector_2_1_1_2_1));
    EXPECT_EQ(9, sizeof(PodVector_4_1_1_2_1));
    EXPECT_EQ(17, sizeof(PodVector_8_1_1_2_1));
    EXPECT_EQ(33, sizeof(PodVector_16_1_1_2_1));
}

TEST(PodVector, ConsistentAlignment)
{
    EXPECT_EQ(1, CCC_ALIGNOF(PodVector_1_1_1_2_1));

    EXPECT_EQ(2, CCC_ALIGNOF(PodVector_1_1_1_2_2));
    EXPECT_EQ(4, CCC_ALIGNOF(PodVector_1_1_1_2_4));
    EXPECT_EQ(8, CCC_ALIGNOF(PodVector_1_1_1_2_8));
    EXPECT_EQ(16, CCC_ALIGNOF(PodVector_1_1_1_2_16));

    EXPECT_EQ(2, CCC_ALIGNOF(PodVector_2_2_1_2_2));
    EXPECT_EQ(4, CCC_ALIGNOF(PodVector_4_4_1_2_4));
    EXPECT_EQ(8, CCC_ALIGNOF(PodVector_8_8_1_2_8));
    EXPECT_EQ(16, CCC_ALIGNOF(PodVector_16_16_1_2_16));

    EXPECT_EQ(1, CCC_ALIGNOF(PodVector_2_1_1_2_1));
    EXPECT_EQ(1, CCC_ALIGNOF(PodVector_4_1_1_2_1));
    EXPECT_EQ(1, CCC_ALIGNOF(PodVector_8_1_1_2_1));
    EXPECT_EQ(1, CCC_ALIGNOF(PodVector_16_1_1_2_1));
}

TEST(PodVector, ConsistentLayout)
{
    EXPECT_EQ(1, CCC_OFFSETOF(PodVector_1_1_1_2_1, m_Storage[0]));
    EXPECT_EQ(2, CCC_OFFSETOF(PodVector_1_1_1_2_1, m_Storage[1]));

    EXPECT_EQ(2, CCC_OFFSETOF(PodVector_1_1_1_2_2, m_Storage[0]));
    EXPECT_EQ(3, CCC_OFFSETOF(PodVector_1_1_1_2_2, m_Storage[1]));
    EXPECT_EQ(4, CCC_OFFSETOF(PodVector_1_1_1_2_4, m_Storage[0]));
    EXPECT_EQ(5, CCC_OFFSETOF(PodVector_1_1_1_2_4, m_Storage[1]));
    EXPECT_EQ(8, CCC_OFFSETOF(PodVector_1_1_1_2_8, m_Storage[0]));
    EXPECT_EQ(9, CCC_OFFSETOF(PodVector_1_1_1_2_8, m_Storage[1]));
    EXPECT_EQ(16, CCC_OFFSETOF(PodVector_1_1_1_2_16, m_Storage[0]));
    EXPECT_EQ(17, CCC_OFFSETOF(PodVector_1_1_1_2_16, m_Storage[1]));

    EXPECT_EQ(2, CCC_OFFSETOF(PodVector_2_2_1_2_2, m_Storage[0]));
    EXPECT_EQ(4, CCC_OFFSETOF(PodVector_2_2_1_2_2, m_Storage[1]));
    EXPECT_EQ(4, CCC_OFFSETOF(PodVector_4_4_1_2_4, m_Storage[0]));
    EXPECT_EQ(8, CCC_OFFSETOF(PodVector_4_4_1_2_4, m_Storage[1]));
    EXPECT_EQ(8, CCC_OFFSETOF(PodVector_8_8_1_2_8, m_Storage[0]));
    EXPECT_EQ(16, CCC_OFFSETOF(PodVector_8_8_1_2_8, m_Storage[1]));
    EXPECT_EQ(16, CCC_OFFSETOF(PodVector_16_16_1_2_16, m_Storage[0]));
    EXPECT_EQ(32, CCC_OFFSETOF(PodVector_16_16_1_2_16, m_Storage[1]));

    EXPECT_EQ(1, CCC_OFFSETOF(PodVector_2_1_1_2_1, m_Storage[0]));
    EXPECT_EQ(3, CCC_OFFSETOF(PodVector_2_1_1_2_1, m_Storage[1]));
    EXPECT_EQ(1, CCC_OFFSETOF(PodVector_4_1_1_2_1, m_Storage[0]));
    EXPECT_EQ(5, CCC_OFFSETOF(PodVector_4_1_1_2_1, m_Storage[1]));
    EXPECT_EQ(1, CCC_OFFSETOF(PodVector_8_1_1_2_1, m_Storage[0]));
    EXPECT_EQ(9, CCC_OFFSETOF(PodVector_8_1_1_2_1, m_Storage[1]));
    EXPECT_EQ(1, CCC_OFFSETOF(PodVector_16_1_1_2_1, m_Storage[0]));
    EXPECT_EQ(17, CCC_OFFSETOF(PodVector_16_1_1_2_1, m_Storage[1]));
}

template <typename ContainerType, uint64_t Capacity, uint64_t Alignment>
struct LayoutTraits
{
    typedef typename ContainerType container_type;
    typedef typename ContainerType::value_type value_type;
    typedef typename ContainerType::size_type size_type;

    static uint64_t ExpectedAlignment()
    {
        return std::max(Alignment, static_cast<uint64_t>(std::max(CCC_ALIGNOF(value_type), CCC_ALIGNOF(size_type))));
    }

    static uint64_t ExpectedOffsetStorage(uint64_t i)
    {
        return std::max(Alignment, static_cast<uint64_t>(std::max(CCC_ALIGNOF(value_type), sizeof(size_type)))) + i * sizeof(value_type);
    }

    static uint64_t ExpectedSize()
    {
        uint64_t SizeOfDataArray = Capacity * sizeof(value_type);
        uint64_t ExpectedPadding = SizeOfDataArray % Alignment ? Alignment - (SizeOfDataArray % Alignment) : 0;
        return ExpectedOffsetStorage(0) + SizeOfDataArray + ExpectedPadding;
    }
};

template<typename T>
class ConsistentLayoutTest: public ::testing::Test
{
protected:

};

TYPED_TEST_CASE_P(ConsistentLayoutTest);

TYPED_TEST_P(ConsistentLayoutTest, ConsistentSize)
{
    EXPECT_EQ(TypeParam::ExpectedSize(), sizeof(TypeParam::container_type));
}

TYPED_TEST_P(ConsistentLayoutTest, ConsistentAlignment)
{
    EXPECT_EQ(TypeParam::ExpectedAlignment(), CCC_ALIGNOF(TypeParam::container_type));
}

TYPED_TEST_P(ConsistentLayoutTest, ConsistentLayout)
{
    typedef typename TypeParam::container_type container_type;
    EXPECT_EQ(TypeParam::ExpectedOffsetStorage(0), CCC_OFFSETOF(container_type, m_Storage[0]));
    EXPECT_EQ(TypeParam::ExpectedOffsetStorage(1), CCC_OFFSETOF(container_type, m_Storage[1]));
}

REGISTER_TYPED_TEST_CASE_P(ConsistentLayoutTest, ConsistentSize, ConsistentAlignment, ConsistentLayout);

typedef ::testing::Types<
        LayoutTraits<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 1>, 10, 1>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 2>, 10, 2>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 4>, 10, 4>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 8>, 10, 8>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<1, 1>, uint8_t, 10, 16>, 10, 16>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<2, 2>, uint8_t, 10, 2>, 10, 2>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<4, 4>, uint8_t, 10, 4>, 10, 4>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<8, 8>, uint8_t, 10, 8>, 10, 8>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<16, 16>, uint8_t, 10, 16>, 10, 16>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<2, 1>, uint8_t, 10, 1>, 10, 1>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<4, 1>, uint8_t, 10, 1>, 10, 1>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<8, 1>, uint8_t, 10, 1>, 10, 1>,
        LayoutTraits<ccc::PodVector<ccc_test::Pod<16, 1>, uint8_t, 10, 1>, 10, 1>
> LayoutTraitsTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PodVector, ConsistentLayoutTest, LayoutTraitsTypes);
