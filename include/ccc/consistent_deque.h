/**
 *
 * @file This file contains the ConsistentDeque container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_CONSISTENT_DEQUE_H_
#define CCC_CONSISTENT_DEQUE_H_

#include <ccc/pod_deque.h>

namespace ccc
{

template<class T, class SizeType, SizeType Capacity, std::size_t Alignment = 8, bool UseRawMemOps =
        ccc::is_trivially_copyable_guaranteed<T>::value>
struct ConsistentDeque: public PODDeque<T, SizeType, Capacity, Alignment, UseRawMemOps>
{
    ConsistentDeque()
    {
        this->m_Begin = 0;
        this->m_End = 0;
    }

    ~ConsistentDeque()
    {
        // destroy all (valid) elements?
    }
};

}

#endif /* CCC_CONSISTENT_DEQUE_H_ */
