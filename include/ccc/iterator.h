/**
 *
 * @file This file contains useful iterator related functions that come with C++11.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#include <iterator>

#ifndef CCC_ITERATOR_H_
#define CCC_ITERATOR_H_

namespace ccc
{

template <class ForwardIteratorType>
ForwardIteratorType next(ForwardIteratorType Iterator, typename std::iterator_traits<ForwardIteratorType>::difference_type N = 1)
{
    std::advance(Iterator, N);
    return Iterator;
}

template <class BidirectionalIteratorType>
BidirectionalIteratorType prev(BidirectionalIteratorType Iterator, typename std::iterator_traits<BidirectionalIteratorType>::difference_type N = 1)
{
    std::advance(Iterator, -N);
    return Iterator;
}

}

#endif /* CCC_ITERATOR_H_ */
