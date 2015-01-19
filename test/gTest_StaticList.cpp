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

#include <ccc/static_list.h>

#include "gTest_Container.h"

typedef ccc::StaticList<int, unsigned int, 10> ListOfInts;

#if (__cplusplus >= 201103L)
TEST(StaticList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<ListOfInts>::value);
}
#endif

TEST(StaticList, Initialization)
{
    ccc::Test_DefaultInitialization<ListOfInts>();
    ccc::Test_ValueInitialization<ListOfInts>();
}
