/*
 * gTest_PragmaPack.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: dierkes
 */

#include <gtest/gtest.h>

#include <ccc/compat.h>
#include <ccc/test/consistent_integers.h>

#pragma pack(push)

#pragma pack(1)
struct Int16Packed1
{
    int16_t m;
};

struct Int32Packed1
{
    int32_t m;
};

struct Int64Packed1
{
    int64_t m;
};

#pragma pack(2)
struct Int16Packed2
{
    int16_t m;
};

struct Int32Packed2
{
    int32_t m;
};

struct Int64Packed2
{
    int64_t m;
};

#pragma pack(4)
struct Int16Packed4
{
    int16_t m;
};

struct Int32Packed4
{
    int32_t m;
};

struct Int64Packed4
{
    int64_t m;
};

#pragma pack(8)

struct Int32Packed8
{
    int32_t m;
};

struct Int64Packed8
{
    int64_t m;
};

#pragma pack(16)

struct Int64Packed16
{
    int64_t m;
};

TEST(PragmaPack, vsAlignment)
{
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed1));
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Packed2));
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Packed4));
    EXPECT_EQ(1, CCC_ALIGNOF(Int32Packed1));
    EXPECT_EQ(2, CCC_ALIGNOF(Int32Packed2));
    EXPECT_EQ(4, CCC_ALIGNOF(Int32Packed4));
    EXPECT_EQ(4, CCC_ALIGNOF(Int32Packed8));
    EXPECT_EQ(1, CCC_ALIGNOF(Int64Packed1));
    EXPECT_EQ(2, CCC_ALIGNOF(Int64Packed2));
    EXPECT_EQ(4, CCC_ALIGNOF(Int64Packed4));
    EXPECT_EQ(8, CCC_ALIGNOF(Int64Packed8));
    EXPECT_EQ(8, CCC_ALIGNOF(Int64Packed16));
}

#pragma pack(8)
struct PlainStructPacked8
{
    int8_t a;
    int64_t b;
};

#pragma pack(8)
template <typename T1, typename T2>
struct TemplateStructPacked8
{
    T1 a;
    T2 b;
};

#pragma pack(1)
struct PlainStructPacked1
{
    int8_t a;
    int64_t b;
};

#pragma pack(1)
template <typename T1, typename T2>
struct TemplateStructPacked1
{
    T1 a;
    T2 b;
};

#pragma pack(1)
TEST(PragmaPack, vsTemplates_Packed1)
{
    EXPECT_EQ(16, sizeof(PlainStructPacked8));
    EXPECT_EQ(16, sizeof(TemplateStructPacked8<int8_t, int64_t>));
    EXPECT_EQ(9, sizeof(PlainStructPacked1));
    EXPECT_EQ(9, sizeof(TemplateStructPacked1<int8_t, int64_t>));
}

#pragma pack(8)
TEST(PragmaPack, vsTemplates_Packed8)
{
    EXPECT_EQ(16, sizeof(PlainStructPacked8));
    EXPECT_EQ(16, sizeof(TemplateStructPacked8<int8_t, int64_t>));
    EXPECT_EQ(9, sizeof(PlainStructPacked1));
    EXPECT_EQ(9, sizeof(TemplateStructPacked1<int8_t, int64_t>));
}

#if CCC_ALIGNAS_AVAILABLE

#pragma pack(1)
struct Int16Packed1Alignas4
{
    alignas(4) int16_t m;
};

#pragma pack(4)
struct Int16Packed4Alignas1
{
    alignas(1) int16_t m;
};

TEST(PragmaPack, vsAlignas)
{
#if defined __GNUC__
    EXPECT_EQ(2, sizeof(Int16Packed1Alignas4));
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed1Alignas4));
#elif defined _MSC_VER
    EXPECT_EQ(4, sizeof(Int16Packed1Alignas4));
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Packed1Alignas4));
#endif
    EXPECT_EQ(2, sizeof(Int16Packed4Alignas1));
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Packed4Alignas1));
}

#endif

#pragma pack(pop)
