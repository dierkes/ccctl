/*
 * pod_pair.h
 *
 *  Created on: Oct 16, 2015
 *      Author: dierkes
 */

#ifndef CCC_POD_PAIR_H_
#define CCC_POD_PAIR_H_

#include <cstddef>

#include <ccc/alignment.h>

namespace ccc
{

template <typename T1, typename T2, std::size_t Alignment = 8>
struct PodPair
{
    typedef T1 first_type;
    typedef T2 second_type;

    PaddedValue<T1, Alignment> first;
    PaddedValue<T2, Alignment> second;
};

template <typename T1, typename T2, std::size_t Alignment = 8>
PodPair<T1, T2, Alignment> make_pair(const T1& First, const T2& Second)
{
    PodPair<T1, T2, Alignment> P;
    P.first = First;
    P.second = Second;
    return P;
}

}

#endif /* CCC_POD_PAIR_H_ */
