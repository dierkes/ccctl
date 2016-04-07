/**
 *
 * @file This file contains the FixedVector container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_FIXED_VECTOR_H_
#define CCC_FIXED_VECTOR_H_

#include<ccc/consistent_vector.h>

namespace ccc
{

template <typename T, typename SizeType = unsigned int, unsigned int Alignment = 8, bool UseRawMemOps = false>
class FixedVector : public ConsistentVector<T, SizeType, 0, Alignment, UseRawMemOps, false, true>
{
public:
    explicit FixedVector(SizeType Capacity)
    {
        this->m_Storage.allocate(Capacity);
    }

    FixedVector(FixedVector const& Other)
    {
        this->m_Storage.allocate(Other.m_Storage.m_Capacity);
        this->assign(Other.begin(), Other.end());
    }

    void operator=(FixedVector const& Other)
    {
        if (this->m_Storage.m_Capacity != Other.m_Storage.m_Capacity)
        {
            FixedVector Tmp(Other.m_Storage.m_Capacity);
            this->swap(Tmp);
        }
        this->assign(Other.begin(), Other.end());
    }

    ~FixedVector()
    {
        this->clear();
    }

    void swap(FixedVector& Other)
    {
        std::swap(this->m_Storage.data(), Other.m_Storage.data());
        std::swap(this->m_Storage.capacity(), Other.m_Storage.capacity());
        std::swap(this->m_End, Other.m_End);
    }
};

}

#endif /* CCC_FIXED_VECTOR_H_ */
