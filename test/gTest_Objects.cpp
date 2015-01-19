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

#include "gTest_Objects.h"

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
}


template <>
cNoPOD random_object<cNoPOD>()
{
    int Value = std::rand();
    return cNoPOD(Value, static_cast<double>(Value));
}

