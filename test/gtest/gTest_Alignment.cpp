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
#include <ccc/test/container_prototypes.h>

#include <gtest/gtest.h>

#include <cstddef>

#include <ccc/test/consistent_integers.h>
#include <ccc/memory.h>

#pragma pack(push)

#pragma pack(16)

// Set alignment to at least max alignment requirement of one member type on any platform

struct Int16Plain
{
    int16_t m;
};

struct ArrayOf5Int16Plain
{
    int16_t m[5];
};

#if CCC_ALIGNAS_AVAILABLE

struct Int16Alignas1
{
    alignas(1) int16_t m;
};

struct Int16Alignas4
{
    alignas(4) int16_t m;
};

struct ArrayOf5Int16Alignas1
{
    alignas(1) int16_t m[5];
};

struct ArrayOf5Int16Alignas4
{
    alignas(4) int16_t m[5];
};

#endif

struct Int16Aligned1
{
    CCC_ALIGNED(int16_t, 1) m;
};

struct Int16Aligned4
{
    CCC_ALIGNED(int16_t, 4) m;
};

struct ArrayOf5Int16Aligned1
{
    CCC_ALIGNED(int16_t m[5], 1);
};

struct ArrayOf5Int16Aligned4
{
    CCC_ALIGNED(int16_t m[5], 4);
};

struct Int16TypedefAligned1
{
    ccc::Aligned<int16_t, 1>::type m;
};

struct Int16TypedefAligned4
{
    ccc::Aligned<int16_t, 4>::type m;
};

struct ArrayOf5Int16TypedefAligned1
{
    ccc::Aligned<int16_t[5], 1>::type m;
};

struct ArrayOf5Int16TypedefAligned4
{
    ccc::Aligned<int16_t[5], 4>::type m;
};

struct Int16Padded1
{
    ccc::PaddedValue<int16_t, 1> m;
};

struct Int16Padded4
{
    ccc::PaddedValue<int16_t, 4> m;
};

struct ArrayOf5Int16Padded1
{
    ccc::PaddedArray<int16_t, 5, 1> m;
};

struct ArrayOf5Int16Padded4
{
    ccc::PaddedArray<int16_t, 5, 4> m;
};

TEST(Alignment, Modifiers)
{
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Plain));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Alignas1));
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Alignas4));
#endif
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Aligned1));
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Aligned4));
#if defined __GNUC__
    EXPECT_EQ(1, CCC_ALIGNOF(Int16TypedefAligned1));
#elif defined _MSC_VER
    EXPECT_EQ(2, CCC_ALIGNOF(Int16TypedefAligned1));
#endif
    EXPECT_EQ(4, CCC_ALIGNOF(Int16TypedefAligned4));
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Padded1));
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Padded4));
    EXPECT_EQ(2, sizeof(Int16Plain));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(2, sizeof(Int16Alignas1));
    EXPECT_EQ(4, sizeof(Int16Alignas4));
#endif
    EXPECT_EQ(2, sizeof(Int16Aligned1));
    EXPECT_EQ(4, sizeof(Int16Aligned4));
    EXPECT_EQ(2, sizeof(Int16TypedefAligned1));
    EXPECT_EQ(4, sizeof(Int16TypedefAligned4));
    EXPECT_EQ(2, sizeof(Int16Padded1));
    EXPECT_EQ(4, sizeof(Int16Padded4));
    EXPECT_EQ(2, CCC_ALIGNOF(ArrayOf5Int16Plain));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(2, CCC_ALIGNOF(ArrayOf5Int16Alignas1));
    EXPECT_EQ(4, CCC_ALIGNOF(ArrayOf5Int16Alignas4));
#endif
    EXPECT_EQ(2, CCC_ALIGNOF(ArrayOf5Int16Aligned1));
    EXPECT_EQ(4, CCC_ALIGNOF(ArrayOf5Int16Aligned4));
#if defined __GNUC__
    EXPECT_EQ(1, CCC_ALIGNOF(ArrayOf5Int16TypedefAligned1));
#elif defined _MSC_VER
    EXPECT_EQ(2, CCC_ALIGNOF(ArrayOf5Int16TypedefAligned1));
#endif
    EXPECT_EQ(4, CCC_ALIGNOF(ArrayOf5Int16TypedefAligned4));
    EXPECT_EQ(2, CCC_ALIGNOF(ArrayOf5Int16Padded1));
    EXPECT_EQ(2, CCC_ALIGNOF(ArrayOf5Int16Padded4));
    EXPECT_EQ(10, sizeof(ArrayOf5Int16Plain));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(10, sizeof(ArrayOf5Int16Alignas1));
    EXPECT_EQ(12, sizeof(ArrayOf5Int16Alignas4));
#endif
    EXPECT_EQ(10, sizeof(ArrayOf5Int16Aligned1));
    EXPECT_EQ(12, sizeof(ArrayOf5Int16Aligned4));
    EXPECT_EQ(10, sizeof(ArrayOf5Int16TypedefAligned1));
    EXPECT_EQ(12, sizeof(ArrayOf5Int16TypedefAligned4));
    EXPECT_EQ(10, sizeof(ArrayOf5Int16Padded1));
    EXPECT_EQ(12, sizeof(ArrayOf5Int16Padded4));
}

/** C++ Standard: 5.3.3/2
 *
 *  When applied to a class, the result [of sizeof] is the number of bytes in an object of that
 *  class, including any padding required for placing objects of that type in an array.
 *
 * [dcl.array]:
 * An object of array type contains a contiguously allocated non-empty set of N subobjects of type T.
 */



#if CCC_ALIGNAS_AVAILABLE

#pragma pack(1)

struct PackedAlignas1
{
    alignas(1) int64_t m;
};

struct PackedAlignas8
{
    alignas(8) int8_t m;
};

struct PackedAlignas16
{
    alignas(16) int8_t m;
};

#pragma pack(8)

struct NonPackedAlignas1
{
    alignas(1) int64_t m;
};

struct NonPackedAlignas4
{
    alignas(4) int8_t m;
};

struct NonPackedAlignas8
{
    alignas(8) int8_t m;
};

struct NonPackedAlignas16
{
    alignas(16) int8_t m;
};

TEST(PragmaPackAndAlignas, Pack8)
{
    EXPECT_EQ(1, CCC_ALIGNOF(PackedAlignas1) );
#if defined __GNUC__
    EXPECT_EQ(1, CCC_ALIGNOF(PackedAlignas8));
#elif defined _MSC_VER
    EXPECT_EQ(8, CCC_ALIGNOF(PackedAlignas8));
#endif
#if defined __GNUC__
    EXPECT_EQ(1, CCC_ALIGNOF(PackedAlignas16));
#elif defined _MSC_VER
    EXPECT_EQ(16, CCC_ALIGNOF(PackedAlignas16));
#endif
    EXPECT_EQ(8, CCC_ALIGNOF(NonPackedAlignas1));
    EXPECT_EQ(4, CCC_ALIGNOF(NonPackedAlignas4));
    EXPECT_EQ(8, CCC_ALIGNOF(NonPackedAlignas8));
#if defined __GNUC__
    EXPECT_EQ(8, CCC_ALIGNOF(NonPackedAlignas16));
#elif defined _MSC_VER
    EXPECT_EQ(16, CCC_ALIGNOF(NonPackedAlignas16));
#endif
}

#endif // CCC_ALIGNAS_AVAILABLE

#pragma pack(1)
struct Int16Packed1Aligned1a
{
    ccc::Aligned<int16_t, 1>::type m;
};

#pragma pack(1)
struct Int16Packed1Aligned1b
{
    CCC_ALIGNED(int16_t, 1) m;
};

#pragma pack(1)
struct Int16Packed1Aligned4a
{
    ccc::Aligned<int16_t, 4>::type m;
};

#pragma pack(1)
struct Int16Packed1Aligned4b
{
    CCC_ALIGNED(int16_t, 4) m;
};

#pragma pack(8)
struct Int16Packed8Aligned1a
{
    ccc::Aligned<int16_t, 1>::type m;
};

#pragma pack(8)
struct Int16Packed8Aligned1b
{
    CCC_ALIGNED(int16_t, 1) m;
};

#pragma pack(8)
struct Int16Packed8Aligned4a
{
    ccc::Aligned<int16_t, 4>::type m;
};

#pragma pack(8)
struct Int16Packed8Aligned4b
{
    CCC_ALIGNED(int16_t, 4) m;
};

TEST(PragmaPackAndAligned, Pack8)
{
    EXPECT_EQ(2, sizeof(Int16Packed1Aligned1a));
    EXPECT_EQ(2, sizeof(Int16Packed1Aligned1b));
#if defined __GNUC__
    EXPECT_EQ(2, sizeof(Int16Packed1Aligned4a));
    EXPECT_EQ(2, sizeof(Int16Packed1Aligned4b));
#elif defined _MSC_VER
    EXPECT_EQ(4, sizeof(Int16Packed1Aligned4a));
    EXPECT_EQ(4, sizeof(Int16Packed1Aligned4b));
#endif
    EXPECT_EQ(2, sizeof(Int16Packed8Aligned1a));
    EXPECT_EQ(2, sizeof(Int16Packed8Aligned1b));
    EXPECT_EQ(4, sizeof(Int16Packed8Aligned4a));
    EXPECT_EQ(4, sizeof(Int16Packed8Aligned4b));
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed1Aligned1a));
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed1Aligned1b));
#if defined __GNUC__
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed1Aligned4a));
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed1Aligned4b));
    EXPECT_EQ(1, CCC_ALIGNOF(Int16Packed8Aligned1a));
#elif defined _MSC_VER
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Packed1Aligned4a));
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Packed1Aligned4b));
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Packed8Aligned1a));
#endif
    EXPECT_EQ(2, CCC_ALIGNOF(Int16Packed8Aligned1b));
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Packed8Aligned4a));
    EXPECT_EQ(4, CCC_ALIGNOF(Int16Packed8Aligned4b));
}

struct TestPragmaPackPosition1
{
    int8_t m1;
    int64_t m2;
};

TEST(PragmaPack, Position1)
{
#if defined __GNUC__ && defined CCC_X86
    EXPECT_EQ(4, CCC_OFFSETOF(TestPragmaPackPosition1, m2));
#else
    EXPECT_EQ(8, CCC_OFFSETOF(TestPragmaPackPosition1, m2));
#endif
}

#if CCC_ALIGNAS_AVAILABLE

struct Int64Alignas1
{
    alignas(1) int64_t m;
};

//struct Int64Alignas1b
//{
//    typedef alignas(1) int64_t t; // does not work with VC 14.0
//    t m;
//};

TEST(AlignmentModifiers, Alignas)
{
    EXPECT_EQ(8, sizeof(Int64Alignas1));
    EXPECT_EQ(8, alignof(Int64Alignas1));
//    EXPECT_EQ(8, sizeof(Int64Alignas1b));
//    EXPECT_EQ(8, alignof(Int64Alignas1b));
}

#endif // CCC_ALIGNAS_AVAILABLE

#pragma pack(8)
struct Int64TypedefAligned1
{
    ccc::Aligned<int64_t, 1>::type m;
};

//#pragma pack(1)
struct Int64Aligned1
{
    CCC_ALIGNED(int64_t, 1) m;
};

TEST(AlignmentModifiers, Aligned)
{
    EXPECT_EQ(8, sizeof(Int64TypedefAligned1));
#if defined __GNUC__
    EXPECT_EQ(1, CCC_ALIGNOF(Int64TypedefAligned1));
#elif defined _MSC_VER
    EXPECT_EQ(8, CCC_ALIGNOF(Int64TypedefAligned1));
#endif
    EXPECT_EQ(8, sizeof(Int64Aligned1));
#if defined __GNUC__ && defined CCC_X86
    EXPECT_EQ(4, CCC_ALIGNOF(Int64Aligned1));
#else
    EXPECT_EQ(8, CCC_ALIGNOF(Int64Aligned1));
#endif
}

struct Size16Align8
{
    int64_t m16[2];
};

struct Size1Align1
{
    int8_t m1;
};

struct Size2Align1
{
    int8_t m2[2];
};

struct Size2Align2
{
    int16_t m2;
};

TEST(TestTypes, Size)
{
    EXPECT_EQ(1, sizeof(Size1Align1));
    EXPECT_EQ(2, sizeof(Size2Align1));
    EXPECT_EQ(2, sizeof(Size2Align2));
    EXPECT_EQ(16, sizeof(Size16Align8));
}

TEST(TestTypes, Alignment)
{
    EXPECT_EQ(1, CCC_ALIGNOF(Size1Align1));
    EXPECT_EQ(1, CCC_ALIGNOF(Size2Align1));
    EXPECT_EQ(2, CCC_ALIGNOF(Size2Align2));
#if defined __GNUC__ && defined CCC_X86
    EXPECT_EQ(4, CCC_ALIGNOF(Size16Align8));
#else
    EXPECT_EQ(8, CCC_ALIGNOF(Size16Align8));
#endif
}

#pragma pack(8)
struct Bytes16_a8
{
    int64_t m8;
    int8_t m1;
};

TEST(Alignment, Bytes16_a8)
{
    struct S
    {
        Bytes16_a8 a[2];
    };

#if defined __GNUC__ && defined CCC_X86
    EXPECT_EQ(12, sizeof(Bytes16_a8));
    EXPECT_EQ(4, CCC_ALIGNOF(Bytes16_a8));
    EXPECT_EQ(24, sizeof(S));
    EXPECT_EQ(12, CCC_OFFSETOF(S, a[1]));
#else
    EXPECT_EQ(16, sizeof(Bytes16_a8));
    EXPECT_EQ(8, CCC_ALIGNOF(Bytes16_a8));
    EXPECT_EQ(32, sizeof(S));
    EXPECT_EQ(16, CCC_OFFSETOF(S, a[1]));
#endif
}

#if CCC_ALIGNAS_AVAILABLE

#pragma pack(8)
struct Bytes16_a8b
{
    alignas(8) int8_t m8;
    alignas(8) int8_t m1;
};

TEST(Alignment, Bytes16_a8b)
{
    EXPECT_EQ(16, sizeof(Bytes16_a8b));
    EXPECT_EQ(8, CCC_ALIGNOF(Bytes16_a8b));
}

#endif // CCC_ALIGNAS_AVAILABLE

#pragma pack(1)
struct Bytes9_a1
{
    int64_t m8;
    int8_t m1;
};

TEST(Alignment, Bytes9_a1)
{
    EXPECT_EQ(9, sizeof(Bytes9_a1));
    EXPECT_EQ(1, CCC_ALIGNOF(Bytes9_a1));
}

#pragma pack(1)
struct AlignedByPadding32_p1_a8
{
    ccc::PaddedValue<Bytes9_a1, 8> m1;
    ccc::PaddedValue<Bytes9_a1, 8> m2;
};

TEST(Alignment, AlignedByPadding32_p1_a8)
{
    EXPECT_EQ(32, sizeof(AlignedByPadding32_p1_a8));
}

#pragma pack(1)
struct AlignedByPadding18_p1_a1
{
    ccc::PaddedValue<Bytes9_a1, 1> m1;
    ccc::PaddedValue<Bytes9_a1, 1> m2;
};

TEST(Alignment, AlignedByPadding18_p1_a1)
{
    EXPECT_EQ(18, sizeof(AlignedByPadding18_p1_a1));
}

#pragma pack(8)
struct MisalignedByPadding18_p8_a1
{
    ccc::PaddedValue<Bytes9_a1, 1> m1;
    ccc::PaddedValue<Bytes9_a1, 1> m2;
};

TEST(Alignment, MisalignedByPadding18_p8_a1)
{
    EXPECT_EQ(18, sizeof(MisalignedByPadding18_p8_a1));
}

TEST(Alignment, ContainerPrototypes)
{
    EXPECT_EQ(24, sizeof(ccc::ContainerPlain<int32_t, uint16_t, 5>));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(24, sizeof(ccc::ContainerAlignas<int32_t, uint16_t, 5, 1>));
#endif // CCC_ALIGNAS_AVAILABLE
#if defined __GNUC__
    EXPECT_EQ(22, sizeof(ccc::ContainerAligned<int32_t, uint16_t, 5, 1>)); // violation of alignment rule: 1 < alignof(int32_t)
#elif defined _MSC_VER
    EXPECT_EQ(24, sizeof(ccc::ContainerAligned<int32_t, uint16_t, 5, 1>)); // violation of alignment rule: 1 < alignof(int32_t)
#endif // __GNUC__ vs. _MSC_VER
    EXPECT_EQ(24, sizeof(ccc::ContainerPadded<int32_t, uint16_t, 5, 1>));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(24, sizeof(ccc::ContainerAlignas<int32_t, uint16_t, 5, 2>));
#endif // CCC_ALIGNAS_AVAILABLE
#if defined __GNUC__
    EXPECT_EQ(22, sizeof(ccc::ContainerAligned<int32_t, uint16_t, 5, 2>)); // violation of alignment rule: 1 < alignof(int32_t)
#elif defined _MSC_VER
    EXPECT_EQ(24, sizeof(ccc::ContainerAligned<int32_t, uint16_t, 5, 2>)); // violation of alignment rule: 1 < alignof(int32_t)
#endif // __GNUC__ vs. _MSC_VER
    EXPECT_EQ(24, sizeof(ccc::ContainerPadded<int32_t, uint16_t, 5, 2>));
#if CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(24, sizeof(ccc::ContainerAlignas<int32_t, uint16_t, 5, 4>));
#endif // CCC_ALIGNAS_AVAILABLE
    EXPECT_EQ(24, sizeof(ccc::ContainerAligned<int32_t, uint16_t, 5, 4>)); // no violation of alignment rule anymore
    EXPECT_EQ(24, sizeof(ccc::ContainerPadded<int32_t, uint16_t, 5, 4>));
}

#pragma pack(pop)
