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

#include <stddef.h>

#pragma pack(push, 1)

#include <ccc/pod_vector.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"
#include "consistent_integers.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <vector>

typedef ccc::PODVector<int, uint16_t, 10> ContainerOfInts;
typedef ccc::PODVector<tPOD, uint16_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PODVector, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::PODVector<int, uint16_t, 10>, std::vector<int> > RefContainerOfInts;
typedef reftest<ccc::PODVector<tPOD, uint16_t, 10>, std::vector<tPOD> > RefContainerOfPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PODVector, TestOfSequenceContainer, RefContainerImplementations);

typedef ccc::PODVector<int8_t, uint8_t, 1, 1> PodVector_8_8_1_1;
typedef ccc::PODVector<int8_t, uint8_t, 1, 2> PodVector_8_8_1_2;
typedef ccc::PODVector<int8_t, uint8_t, 1, 4> PodVector_8_8_1_4;
typedef ccc::PODVector<int8_t, uint8_t, 1, 8> PodVector_8_8_1_8;

typedef ccc::PODVector<int8_t, uint8_t, 2, 1> PodVector_8_8_2_1;
typedef ccc::PODVector<int8_t, uint8_t, 2, 2> PodVector_8_8_2_2;
typedef ccc::PODVector<int8_t, uint8_t, 2, 4> PodVector_8_8_2_4;
typedef ccc::PODVector<int8_t, uint8_t, 2, 8> PodVector_8_8_2_8;

typedef ccc::PODVector<int32_t, uint8_t, 2, 1> PodVector_32_8_2_1;
typedef ccc::PODVector<int32_t, uint8_t, 2, 2> PodVector_32_8_2_2;
typedef ccc::PODVector<int32_t, uint8_t, 2, 4> PodVector_32_8_2_4;
typedef ccc::PODVector<int32_t, uint8_t, 2, 8> PodVector_32_8_2_8;

typedef ccc::PODVector<int64_t, uint8_t, 2, 1> PodVector_64_8_2_1;
typedef ccc::PODVector<int64_t, uint8_t, 2, 2> PodVector_64_8_2_2;
typedef ccc::PODVector<int64_t, uint8_t, 2, 4> PodVector_64_8_2_4;
typedef ccc::PODVector<int64_t, uint8_t, 2, 8> PodVector_64_8_2_8;

typedef ccc::PODVector<int32_t[3], uint8_t, 2, 1> PodVector_96_8_2_1;
typedef ccc::PODVector<int32_t[3], uint8_t, 2, 2> PodVector_96_8_2_2;
typedef ccc::PODVector<int32_t[3], uint8_t, 2, 4> PodVector_96_8_2_4;
typedef ccc::PODVector<int32_t[3], uint8_t, 2, 8> PodVector_96_8_2_8;

typedef ccc::PODVector<int32_t[3], uint8_t, 9, 1> PodVector_96_8_9_1;
typedef ccc::PODVector<int32_t[3], uint8_t, 9, 2> PodVector_96_8_9_2;
typedef ccc::PODVector<int32_t[3], uint8_t, 9, 4> PodVector_96_8_9_4;
typedef ccc::PODVector<int32_t[3], uint8_t, 9, 8> PodVector_96_8_9_8;

TEST(PODVector, ConsistentSize)
{
    EXPECT_EQ(16, sizeof(ccc::PODVector<char, uint64_t, 1>));
    EXPECT_EQ(16, sizeof(ccc::PODVector<char, uint64_t, 2>));
    EXPECT_EQ(16, sizeof(ccc::PODVector<char, uint64_t, 4>));
    EXPECT_EQ(16, sizeof(ccc::PODVector<char, uint64_t, 8>));

    EXPECT_EQ(2, sizeof(PodVector_8_8_1_1));
    EXPECT_EQ(4, sizeof(PodVector_8_8_1_2));
    EXPECT_EQ(8, sizeof(PodVector_8_8_1_4));
    EXPECT_EQ(16, sizeof(PodVector_8_8_1_8));

    EXPECT_EQ(3, sizeof(PodVector_8_8_2_1));
    EXPECT_EQ(4, sizeof(PodVector_8_8_2_2));
    EXPECT_EQ(8, sizeof(PodVector_8_8_2_4));
    EXPECT_EQ(16, sizeof(PodVector_8_8_2_8));

    EXPECT_EQ(9, sizeof(PodVector_32_8_2_1));
//    EXPECT_EQ(4, sizeof(PodVector_32_8_2_2));
//    EXPECT_EQ(8, sizeof(PodVector_32_8_2_4));
//    EXPECT_EQ(16, sizeof(PodVector_32_8_2_8));

//    EXPECT_EQ(3, sizeof(PodVector_64_8_2_1));
//    EXPECT_EQ(4, sizeof(PodVector_64_8_2_2));
//    EXPECT_EQ(8, sizeof(PodVector_64_8_2_4));
//    EXPECT_EQ(16, sizeof(PodVector_64_8_2_8));

//    EXPECT_EQ(3, sizeof(PodVector_96_8_2_1));
//    EXPECT_EQ(4, sizeof(PodVector_96_8_2_2));
//    EXPECT_EQ(8, sizeof(PodVector_96_8_2_4));
//    EXPECT_EQ(16, sizeof(PodVector_96_8_2_8));

//    EXPECT_EQ(3, sizeof(PodVector_96_8_9_1));
//    EXPECT_EQ(4, sizeof(PodVector_96_8_9_2));
//    EXPECT_EQ(8, sizeof(PodVector_96_8_9_4));
//    EXPECT_EQ(16, sizeof(PodVector_96_8_9_8));
}

TEST(PodVector, ConsistentLayout)
{
    EXPECT_EQ(0, ccc_offsetof(PodVector_8_8_1_1, m_End));
    EXPECT_EQ(1, ccc_offsetof(PodVector_8_8_1_1, m_Storage));
    EXPECT_EQ(0, ccc_offsetof(PodVector_32_8_2_1, m_End));
    EXPECT_EQ(1, ccc_offsetof(PodVector_32_8_2_1, m_Storage[0]));
    EXPECT_EQ(5, ccc_offsetof(PodVector_32_8_2_1, m_Storage[1]));
    EXPECT_EQ(8, ccc_offsetof(PodVector_32_8_2_8, m_Storage[0]));
    EXPECT_EQ(12, ccc_offsetof(PodVector_32_8_2_8, m_Storage[1]));
}

#pragma pack(pop)
