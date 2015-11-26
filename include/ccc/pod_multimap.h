/*
 * pod_multimap.h
 *
 *  Created on: Aug 27, 2015
 *      Author: dierkes
 */

#ifndef INCLUDE_CCC_POD_MULTIMAP_H_
#define INCLUDE_CCC_POD_MULTIMAP_H_

#include <ccc/pod_pair.h>
#include <ccc/red_black_tree.h>

namespace ccc
{

#pragma pack(push, 1)

template <typename KeyType, typename T, typename SizeType, SizeType Capacity, std::size_t Alignment = 8>
struct PodMultiMap
{
    PodRedBlackTree<PodPair<KeyType, T>, SizeType, Capacity, Alignment> m_Tree;

    // insert

    // erase
};

#pragma pack(pop)

}

#endif /* INCLUDE_CCC_POD_MULTIMAP_H_ */
