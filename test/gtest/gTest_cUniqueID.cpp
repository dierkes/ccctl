/*
 * gTest_cUniqueID.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: dierkes
 */

#include <gtest/gtest.h>

#include "utils.h"
#include <ccc/test/consistent_integers.h>


TEST(UniqueID, DefaultInitialization)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    U obj_0;
    EXPECT_EQ(0, obj_0.m_ID);
    EXPECT_EQ(1, U::NextID);
    EXPECT_EQ(1, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
}

TEST(UniqueID, ValueInitialization)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    U obj_0 = U();
    EXPECT_EQ(0, obj_0.m_ID);
    EXPECT_EQ(1, U::NextID);
    EXPECT_EQ(1, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
}

TEST(UniqueID, Assignment)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    U obj_0;
    U obj_1 = obj_0;
    EXPECT_EQ(1, obj_1.m_ID);
    EXPECT_EQ(2, U::NextID);
    EXPECT_EQ(2, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
}

TEST(UniqueID, Scope)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    {
        U obj_0;
        EXPECT_EQ(0, obj_0.m_ID);
        EXPECT_EQ(1, U::NextID);
        EXPECT_EQ(1, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
    }
    EXPECT_EQ(0, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
}

TEST(UniqueID, Destructor)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    U obj_0;
    EXPECT_EQ(0, obj_0.m_ID);
    EXPECT_EQ(1, U::NextID);
    EXPECT_EQ(1, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
    obj_0.~cUniqueID();
    EXPECT_EQ(0, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
}

TEST(UniqueID, Array)
{
    typedef cUniqueID<false> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<uint64_t>();
    {
        U a[2];
        EXPECT_EQ(0, a[0].m_ID);
        EXPECT_EQ(1, a[1].m_ID);
        EXPECT_EQ(2, U::NextID);
        EXPECT_EQ(2, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
        {
            a[0] = U();
        }
        EXPECT_EQ(0, a[0].m_ID);
        EXPECT_EQ(1, a[1].m_ID);
        EXPECT_EQ(3, U::NextID);
        EXPECT_EQ(2, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
    }
    EXPECT_EQ(0, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
}
