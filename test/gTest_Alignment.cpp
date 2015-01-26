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

#include <ccc/alignment.h>

#include <gtest/gtest.h>

#include <cstddef>


struct tPacked
{
    char a;
    char b;
    int c;
};

#if (__cplusplus >= 201103L)

struct tAligned
{
    alignas(8) char a;
    alignas(8) char b;
    alignas(8) int c;
    alignas(8) char d[9];
};

#else

struct tAligned
{
    ccc::PaddedValue<char, 8> a;
    ccc::PaddedValue<char, 8> b;
    ccc::PaddedValue<int, 8> c;
    ccc::PaddedArray<char, 9, 8> d;
};

#endif

TEST(Alignment, a)
{
    EXPECT_EQ(40, sizeof(tAligned));
    EXPECT_EQ(0, offsetof(tAligned, a));
    EXPECT_EQ(8, offsetof(tAligned, b));
    EXPECT_EQ(16, offsetof(tAligned, c));
    EXPECT_EQ(24, offsetof(tAligned, d));
}

TEST(Alignment, Padded)
{
    EXPECT_EQ(8, sizeof(ccc::Padded<long, 8>));
    EXPECT_EQ(8, sizeof(ccc::Padded<char[4], 8>));
    EXPECT_EQ(1, sizeof(ccc::Padded<char, 1>));
    EXPECT_EQ(8, sizeof(ccc::Padded<char, 8>));
    EXPECT_EQ(4, sizeof(ccc::Padded<char, 4>));
    EXPECT_EQ(8, sizeof(ccc::Padded<long, 8>));
}

TEST(Alignment, PaddedArray)
{
    EXPECT_EQ(8, sizeof(ccc::PaddedArray<char, 7, 8>));
    EXPECT_EQ(8, sizeof(ccc::PaddedArray<char, 8, 8>));
    EXPECT_EQ(16, sizeof(ccc::PaddedArray<char, 9, 8>));
    ccc::PaddedArray<int, 9, 8> a;
    EXPECT_NO_THROW(a[0] = 1);
    EXPECT_EQ(1, a[0]);
    EXPECT_EQ(1, *(&a[0]));
}
