/**
 *
 * @file
 * Copyright &copy; Audi AG. All rights reserved.
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

#include <celero/Celero.h>
#include <celero/Benchmark.h>

#include <ccc/static_vector.h>
#include <vector>

#include <random>

CELERO_MAIN

BASELINE(ccctl, std_vector, 30, 100)
{
    std::vector<int> v;
    v.reserve(1000);
    for (int i = 0; i < 1000; ++i)
    {
        v.push_back(i);
    }
}

BENCHMARK(ccctl, ccc_StaticVector, 30, 100)
{
    ccc::StaticVector<int, int, 1000> v;
    for (int i = 0; i < 1000; ++i)
    {
        v.push_back(i);
    }
}
