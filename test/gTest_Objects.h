/*
 * gTest_Objects.h
 *
 *  Created on: Jan 19, 2015
 *      Author: dierkes
 */

#ifndef CCC_GTEST_OBJECTS_H_
#define CCC_GTEST_OBJECTS_H_

#include <cstdlib>
#include <exception>
#include <sstream>

template <typename T>
T random_object()
{
    return T();
}

template <typename T>
struct ContainerFactory
{
    static T Create()
    {
        return T();
    }
};


struct tPOD
{
    int x;
    double y;

    bool operator==(const tPOD& rhs) const
    {
        return (this->x == rhs.x) && (this->y == rhs.y);
    }

    bool operator!=(const tPOD& rhs) const
    {
        return !(*this == rhs);
    }

    friend inline std::ostream& operator<<(std::ostream& os, tPOD o)
    {
        os << "(" << o.x << "," << o.y << ")";
        return os;
    }
};


class cNoPOD
{
public:
    cNoPOD()
            : m_x(1), m_y(2.0)
    {
    }

    cNoPOD(int x, double y)
            : m_x(x), m_y(y)
    {
    }

    bool operator==(const cNoPOD& rhs) const
    {
        return (this->m_x == rhs.m_x) && (this->m_y == rhs.m_y);
    }

    bool operator!=(const cNoPOD& rhs) const
    {
        return !(*this == rhs);
    }

    friend inline std::ostream& operator<<(std::ostream& os, cNoPOD o)
    {
        os << "(" << o.m_x << "," << o.m_y << ")";
        return os;
    }
private:
    int m_x;
    double m_y;
};


class cThrowWhenConstructed
{
public:
    cThrowWhenConstructed()
    {
        throw std::exception();
    }
};

template <typename ContainerType>
std::string PrintContent(ContainerType c)
{
    std::stringstream ss;
    ss << "[";
    for (typename ContainerType::iterator it = c.begin(); it != c.end(); ++it)
    {
        ss << *it << ",";
    }
    ss << "]";
    return ss.str();
}

template <>
int random_object<int>();

template <>
tPOD random_object<tPOD>();

template <>
cNoPOD random_object<cNoPOD>();

#endif /* CCC_GTEST_OBJECTS_H_ */
