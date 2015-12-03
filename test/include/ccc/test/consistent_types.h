/**
 *
 * @file
 *
 */

#ifndef CCC_TEST_CONSISTENT_TYPES_H_
#define CCC_TEST_CONSISTENT_TYPES_H_

#include <ccc/alignment.h>
#include <ccc/test/consistent_integers.h>

namespace ccc_test
{

struct PodSize1Alignment1
{
    int8_t m;
};

struct PodSize2Alignment1
{
    int8_t m[2];
};

struct PodSize2Alignment2
{
    int16_t m;
};

template <uint64_t Alignment>
struct PodMember
{
};

template <>
struct PodMember<1>
{
    int8_t m;
};

template <>
struct PodMember<2>
{
    int16_t m;
};

template <>
struct PodMember<4>
{
    int32_t m;
};

template <>
struct PodMember<8>
{
    int64_t m;
};

template <>
struct PodMember<16>
{
    CCC_ALIGNED(int64_t, 16) m;
};

template <uint64_t Size, uint64_t Alignment, uint64_t Residual = Size % Alignment>
struct PodMemberArray
{
};

template <uint64_t Size, uint64_t Alignment>
struct PodMemberArray<Size, Alignment, 0>
{
    PodMember<Alignment> m[Size / Alignment];

    bool operator==(const PodMemberArray<Size, Alignment, 0>& rhs) const
    {
        bool b = true;
        for (uint64_t i = 0; i < Size / Alignment; ++i)
        {
            b &= this->m[i].m == rhs.m[i].m;
        }
        return b;
    }

    bool operator!=(const PodMemberArray<Size, Alignment, 0>& rhs) const
    {
        return !(*this == rhs);
    }

    friend inline std::ostream& operator<<(std::ostream& os, PodMemberArray<Size, Alignment, 0> o)
    {
        os << "[";
        for (uint64_t i = 0; i < Size / Alignment; ++i)
        {
            os << o.m[i].m << ",";
        }
        os << "]";
        return os;
    }
};

template <uint64_t Size, uint64_t Alignment>
struct Pod
{
    PodMemberArray<Size, Alignment> m;

    bool operator==(const Pod<Size, Alignment>& rhs) const
    {
        return this->m == rhs.m;
    }

    bool operator!=(const Pod<Size, Alignment>& rhs) const
    {
        return !(*this == rhs);
    }

    friend inline std::ostream& operator<<(std::ostream& os, Pod<Size, Alignment> o)
    {
        os << o.m;
        return os;
    }
};

}

#endif /* CCC_TEST_CONSISTENT_TYPES_H_ */
