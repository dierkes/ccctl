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

#include "utils.h"

template <>
int random_object<int>()
{
    return std::rand();
}


template <>
tPOD random_object<tPOD>()
{
    int Value = std::rand();
    tPOD Object;
    Object.x = Value;
    Object.y = static_cast<double>(Value);
    return Object;
}


template <>
cNoPOD random_object<cNoPOD>()
{
    int Value = std::rand();
    return cNoPOD(Value, static_cast<double>(Value));
}

template<> std::size_t cUniqueID<true>::NextID = 0;
template<> std::set<std::size_t> cUniqueID<true>::CurrentIDs = std::set<std::size_t>();
template<> std::size_t cUniqueID<false>::NextID = 0;
template<> std::set<std::size_t> cUniqueID<false>::CurrentIDs = std::set<std::size_t>();
