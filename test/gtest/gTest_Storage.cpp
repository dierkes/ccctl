/*
 * gTest_Storage.h
 *
 *  Created on: Mar 15, 2015
 *      Author: dierkes
 */

#include <gtest/gtest.h>

#include <ccc/storage.h>

#include "utils.h"
#include <ccc/test/consistent_integers.h>

TEST(Storage, StaticInitialized)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    EXPECT_EQ(0, U::CurrentIDs.size());
    ccc::StaticInitializedStorage<U, uint64_t, 10> sis;
    EXPECT_EQ(10, U::CurrentIDs.size());
    sis.construct_and_assign(&sis[0], U());
    EXPECT_EQ(10, U::CurrentIDs.size());
    sis.destroy(&sis[0]);
    EXPECT_EQ(10, U::CurrentIDs.size());
}

TEST(Storage, StaticUninitialized)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    EXPECT_EQ(0, U::CurrentIDs.size());
    ccc::StaticUninitializedStorage<U, uint64_t, 10> sus;
    EXPECT_EQ(0, U::CurrentIDs.size());
    sus.construct_and_assign(&sus[0], U());
    EXPECT_EQ(1, U::CurrentIDs.size());
    sus.destroy(&sus[0]);
    EXPECT_EQ(0, U::CurrentIDs.size());
}

TEST(Storage, FixedInitialized)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    EXPECT_EQ(0, U::CurrentIDs.size());
    ccc::FixedInitializedStorage<U, uint64_t> fis;
    EXPECT_EQ(0, U::CurrentIDs.size());
    fis.allocate(10);
    EXPECT_EQ(10, U::CurrentIDs.size());
    fis.construct_and_assign(&fis[0], U());
    EXPECT_EQ(10, U::CurrentIDs.size());
    fis.destroy(&fis[0]);
    EXPECT_EQ(10, U::CurrentIDs.size());
    fis.deallocate();
    EXPECT_EQ(0, U::CurrentIDs.size());
}

TEST(Storage, FixedUninitialized)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    EXPECT_EQ(0, U::CurrentIDs.size());
    ccc::FixedUninitializedStorage<U, uint64_t> fus;
    EXPECT_EQ(0, U::CurrentIDs.size());
    fus.allocate(10);
    EXPECT_EQ(0, U::CurrentIDs.size());
    fus.construct_and_assign(&fus[0], U());
    EXPECT_EQ(1, U::CurrentIDs.size());
    fus.destroy(&fus[0]);
    EXPECT_EQ(0, U::CurrentIDs.size());
    fus.deallocate();
    EXPECT_EQ(0, U::CurrentIDs.size());
}
