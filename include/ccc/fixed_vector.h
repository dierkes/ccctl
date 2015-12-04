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

    ~FixedVector()
    {
        this->clear();
        this->m_Storage.deallocate();
    }
};

}

#endif /* CCC_FIXED_VECTOR_H_ */
