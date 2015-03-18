/*
 * gTest_Storage.h
 *
 *  Created on: Mar 15, 2015
 *      Author: dierkes
 */

#include <gtest/gtest.h>

#include <ccc/storage.h>

#include "gTest_Objects.h"

TEST(Storage, StaticInitialized)
{
    typedef cUniqueID<true> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<std::size_t>();
    ccc::StaticInitializedStorage<U, std::size_t, 10> sis;

    sis.construct_and_assign(0, U());
    sis.destroy(&sis[0]);
    EXPECT_EQ(10, U::CurrentIDs.size());
}

TEST(Storage, StaticUninitialized)
{
    typedef cUniqueID<true> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<std::size_t>();
    ccc::StaticUninitializedStorage<U, std::size_t, 10> sus;

    sus.construct_and_assign(0, U());
    sus.destroy(&sus[0]);
    EXPECT_EQ(0, U::CurrentIDs.size());
}

TEST(Storage, FixedInitialized)
{
    typedef cUniqueID<true> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<std::size_t>();
    ccc::FixedInitializedStorage<U, std::size_t> fis;
    fis.allocate(10);

    fis.construct_and_assign(0, U());
    fis.destroy(&fis[0]);
    EXPECT_EQ(10, U::CurrentIDs.size());
    fis.deallocate();
}

TEST(Storage, FixedUninitialized)
{
    typedef cUniqueID<true> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<std::size_t>();
    ccc::FixedUninitializedStorage<U, std::size_t> fus;
    fus.allocate(10);

    fus.construct_and_assign(0, U());
    fus.destroy(&fus[0]);
    fus[0] = U();
    fus[1] = U();
    EXPECT_EQ(0, U::CurrentIDs.size());
    fus.deallocate();
}
