/*
 * gTest_SequenceContainer.h
 *
 *  Created on: Jan 8, 2015
 *      Author: dierkes
 */

#ifndef CCC_GTEST_SEQUENCECONTAINER_H_
#define CCC_GTEST_SEQUENCECONTAINER_H_

#include <exception>
#include <ciso646>

#include <ccc/iterator.h>

#include <gtest/gtest.h>

#include "utils.h"
#include "consistent_integers.h"

template<typename Container, typename Reference>
struct reftest
{
    typedef Container container;
    typedef Reference reference;
    typedef ContainerFactory<Container> container_factory;
    typedef ContainerFactory<Reference> reference_factory;
};

template<typename T>
class TestOfSequenceContainer: public ::testing::Test
{
protected:
//    friend bool operator==(const typename T::container& lhs, const typename T::reference& rhs)
//    {
//        bool Equal = true;
//        typename T::container::const_iterator it_lhs = lhs.begin();
//        typename T::reference::const_iterator it_rhs = rhs.begin();
//        while (Equal and (it_lhs != lhs.end()) and (it_rhs != rhs.end()))
//        {
//            Equal = *it_lhs == *it_rhs;
//        }
//        return Equal and (it_lhs == lhs.end()) and (it_rhs == rhs.end());
//    }

//    friend bool operator==(const typename T::reference& lhs, const typename T::container& rhs)
//    {
//        return rhs == lhs;
//    }

    bool ContainersAreEqual(const typename T::container& lhs, const typename T::reference& rhs)
    {
        bool Equal = true;
        typename T::container::const_iterator it_lhs = lhs.begin();
        typename T::reference::const_iterator it_rhs = rhs.begin();
        while (Equal and (it_lhs != lhs.end()) and (it_rhs != rhs.end()))
        {
            Equal = *it_lhs == *it_rhs;
            ++it_lhs;
            ++it_rhs;
        }
        return Equal and (it_lhs == lhs.end()) and (it_rhs == rhs.end());
    }

    void insert_end_N(typename T::container& c, typename T::reference& r, uint64_t N)
    {
        for (uint64_t n = 0; n < N; ++n)
        {
            typename T::container::value_type o =
                    random_object<typename T::container::value_type>();
            c.insert(c.end(), o);
            r.insert(r.end(), o);
        }
    }

    void insert_begin_N(typename T::container& c, typename T::reference& r, uint64_t N)
    {
        for (uint64_t n = 0; n < N; ++n)
        {
            typename T::container::value_type o =
                    random_object<typename T::container::value_type>();
            c.insert(c.begin(), o);
            r.insert(r.begin(), o);
        }
    }

    void erase_end_N(typename T::container& c, typename T::reference& r, uint64_t N)
    {
        for (uint64_t n = 0; n < N; ++n)
        {
            typename T::container::iterator it_c = c.end();
            typename T::reference::iterator it_r = r.end();
            --it_c;
            --it_r;
            c.erase(it_c);
            r.erase(it_r);
        }
    }

    void erase_begin_N(typename T::container& c, typename T::reference& r, uint64_t N)
    {
        for (uint64_t n = 0; n < N; ++n)
        {
            c.erase(c.begin());
            r.erase(r.begin());
        }
    }
};

TYPED_TEST_CASE_P(TestOfSequenceContainer);

TYPED_TEST_P(TestOfSequenceContainer, InsertErase)
{
{
    typename TypeParam::container c = ContainerFactory<typename TypeParam::container>::Create();
    typename TypeParam::reference r = ContainerFactory<typename TypeParam::reference>::Create();
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_end_N(c, r, 1));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_end_N(c, r, 1));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_end_N(c, r, 3));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_begin_N(c, r, 3));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->erase_end_N(c, r, 3));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->erase_begin_N(c, r, 5));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_TRUE(c.empty());
    EXPECT_TRUE(r.empty());
}
{
    typename TypeParam::container c = ContainerFactory<typename TypeParam::container>::Create();
    typename TypeParam::reference r = ContainerFactory<typename TypeParam::reference>::Create();
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_begin_N(c, r, 1));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_EQ(r.front(), c.front());
    EXPECT_EQ(r.back(), c.back());
}
{
    typename TypeParam::container c = ContainerFactory<typename TypeParam::container>::Create();
    typename TypeParam::container d = ContainerFactory<typename TypeParam::container>::Create();
    typename TypeParam::reference r = ContainerFactory<typename TypeParam::reference>::Create();
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_begin_N(c, r, 7));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(d.assign(r.begin(), r.end()));
    EXPECT_TRUE(this->ContainersAreEqual(d, r)) << PrintContent(d) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->erase_end_N(c, r, 3));
    EXPECT_NO_THROW(d = c);
    EXPECT_TRUE(this->ContainersAreEqual(d, r)) << PrintContent(d) << std::endl << PrintContent(r);
    typename TypeParam::container::value_type v = random_object<typename TypeParam::container::value_type>();
    EXPECT_NO_THROW(d.assign(static_cast<uint64_t>(4), v));
    EXPECT_NO_THROW(r.assign(static_cast<uint64_t>(4), v));
    EXPECT_TRUE(this->ContainersAreEqual(d, r)) << PrintContent(d) << std::endl << PrintContent(r);
}
{
    typename TypeParam::container c = ContainerFactory<typename TypeParam::container>::Create();
    typename TypeParam::container d = ContainerFactory<typename TypeParam::container>::Create();
    typename TypeParam::reference r = ContainerFactory<typename TypeParam::reference>::Create();
    typename TypeParam::reference s = ContainerFactory<typename TypeParam::reference>::Create();
    typename TypeParam::container::value_type v = random_object<typename TypeParam::container::value_type>();
    EXPECT_NO_THROW(c.insert(c.begin(), static_cast<uint64_t>(4), v));
    EXPECT_NO_THROW(r.insert(r.begin(), static_cast<uint64_t>(4), v));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(this->insert_end_N(d, s, 5));
    EXPECT_NO_THROW(c.insert(c.end(), s.begin(), s.end()));
    EXPECT_NO_THROW(r.insert(r.end(), s.begin(), s.end()));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(c.erase(c.begin(), ccc::next(c.begin(), 5)));
    EXPECT_NO_THROW(r.erase(r.begin(), ccc::next(r.begin(), 5)));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
    EXPECT_NO_THROW(c.insert(c.end(), s.begin(), s.end()));
    EXPECT_NO_THROW(r.insert(r.end(), s.begin(), s.end()));
    EXPECT_TRUE(this->ContainersAreEqual(c, r)) << PrintContent(c) << std::endl << PrintContent(r);
}
}

REGISTER_TYPED_TEST_CASE_P(TestOfSequenceContainer, InsertErase);

#endif /* CCC_GTEST_SEQUENCECONTAINER_H_ */
