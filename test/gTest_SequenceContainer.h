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

#include <gtest/gtest.h>
#include "gTest_Objects.h"

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

    void insert_end_N(typename T::container& c, typename T::reference& r, std::size_t N)
    {
        for (std::size_t n = 0; n < N; ++n)
        {
            typename T::container::value_type o = random_object<typename T::container::value_type>();
            c.insert(c.end(), o);
            r.insert(r.end(), o);
        }
    }

    void insert_begin_N(typename T::container& c, typename T::reference& r, std::size_t N)
    {
        for (std::size_t n = 0; n < N; ++n)
        {
            typename T::container::value_type o = random_object<typename T::container::value_type>();
            c.insert(c.begin(), o);
            r.insert(r.begin(), o);
        }
    }

    void erase_end_N(typename T::container& c, typename T::reference& r, std::size_t N)
    {
        for (std::size_t n = 0; n < N; ++n)
        {
            typename T::container::iterator it_c = c.end();
            typename T::reference::iterator it_r = r.end();
            --it_c;
            --it_r;
            c.erase(it_c);
            r.erase(it_r);
        }
    }

    void erase_begin_N(typename T::container& c, typename T::reference& r, std::size_t N)
    {
        for (std::size_t n = 0; n < N; ++n)
        {
            c.erase(c.begin());
            r.erase(r.begin());
        }
    }
};

TYPED_TEST_CASE_P(TestOfSequenceContainer);

TYPED_TEST_P(TestOfSequenceContainer, InsertErase){
typename TypeParam::container c = ContainerFactory<typename TypeParam::container>::Create();
typename TypeParam::reference r = ContainerFactory<typename TypeParam::reference>::Create();
//EXPECT_EQ(c, r);
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

REGISTER_TYPED_TEST_CASE_P(TestOfSequenceContainer, InsertErase);


#endif /* CCC_GTEST_SEQUENCECONTAINER_H_ */