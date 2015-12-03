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

#include <ccc/pod_list.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <list>

typedef ccc::PodList<int, uint64_t, 10> ContainerOfInts;
typedef ccc::PodList<tPOD, uint64_t, 10> ContainerOfPODs;

#if (__cplusplus >= 201103L)
TEST(PodList, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_pod<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_pod<ContainerOfPODs>::value);
}
#endif

template<> uint64_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> uint64_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfPODContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfStaticContainer, ContainerImplementations);

typedef ::testing::Types<
        RefPair<ccc::PodList<ccc_test::Pod<1, 1>, uint8_t, 10, 1>, std::list<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<1, 1>, uint8_t, 10, 2>, std::list<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<1, 1>, uint8_t, 10, 4>, std::list<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<1, 1>, uint8_t, 10, 8>, std::list<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<1, 1>, uint8_t, 10, 16>, std::list<ccc_test::Pod<1, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<2, 2>, uint8_t, 10, 1>, std::list<ccc_test::Pod<2, 2> > >,
        RefPair<ccc::PodList<ccc_test::Pod<4, 4>, uint8_t, 10, 1>, std::list<ccc_test::Pod<4, 4> > >,
        RefPair<ccc::PodList<ccc_test::Pod<8, 8>, uint8_t, 10, 1>, std::list<ccc_test::Pod<8, 8> > >,
#if !(defined _MSC_VER && defined CCC_X86)
        RefPair<ccc::PodList<ccc_test::Pod<16, 16>, uint8_t, 10, 1>, std::list<ccc_test::Pod<16, 16> > >,
#endif
        RefPair<ccc::PodList<ccc_test::Pod<2, 1>, uint8_t, 10, 1>, std::list<ccc_test::Pod<2, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<4, 1>, uint8_t, 10, 1>, std::list<ccc_test::Pod<4, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<8, 1>, uint8_t, 10, 1>, std::list<ccc_test::Pod<8, 1> > >,
        RefPair<ccc::PodList<ccc_test::Pod<16, 1>, uint8_t, 10, 1>, std::list<ccc_test::Pod<16, 1> > >
> RefPairTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PodList, TestOfSequenceContainer, RefPairTypes);
