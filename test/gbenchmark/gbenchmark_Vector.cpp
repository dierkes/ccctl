/*
 * gbenchmark_Vector.cpp
 *
 *  Created on: Mar 27, 2016
 *      Author: dierkes
 */

#include <benchmark/benchmark.h>

#include <vector>
#include <ccc/static_vector.h>
#include <ccc/fixed_vector.h>

template<typename T, typename TSize, TSize Capacity, typename TVector>
struct VectorFactory
{
    static TVector Construct()
    {
        return TVector();
    }

    static TVector Construct(TSize n)
    {
        return TVector(n);
    }
};

template<typename T, typename TSize, TSize Capacity>
struct VectorFactory<T, TSize, Capacity, std::vector<T> >
{
    static std::vector<T> Construct()
    {
        std::vector<T> v;
        v.reserve(Capacity);
        return v;
    }

    static std::vector<T> Construct(TSize n)
    {
        return std::vector<T>(n);
    }
};

template<typename T, typename TSize, TSize Capacity>
struct VectorFactory<T, TSize, Capacity, ccc::StaticVector<T, TSize, Capacity> >
{
    static ccc::StaticVector<T, TSize, Capacity> Construct()
    {
        return ccc::StaticVector<T, TSize, Capacity>();
    }
};

template<typename T, typename TSize, TSize Capacity>
struct VectorFactory<T, TSize, Capacity, ccc::FixedVector<T, TSize> >
{
    static ccc::FixedVector<T, TSize> Construct()
    {
        return ccc::FixedVector<T, TSize>(Capacity);
    }
};

template <typename TVector>
static void BM_InitializeVector(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        TVector v = VectorFactory<int, unsigned int, 10, TVector>::Construct();
        v.push_back(42);
        v.push_back(43);
    }
}

typedef ccc::StaticVector<int, unsigned int, 10> StaticVector;
BENCHMARK_TEMPLATE(BM_InitializeVector, std::vector<int>);
BENCHMARK_TEMPLATE(BM_InitializeVector, ccc::StaticVector<int, unsigned int, 10>);
BENCHMARK_TEMPLATE(BM_InitializeVector, ccc::FixedVector<int, unsigned int>);

static void BM_StaticVector(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        ccc::StaticVector<int, unsigned int, 10> v;
        v.push_back(42);
        v.push_back(43);
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(BM_StaticVector);

static void BM_FixedVector(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        ccc::FixedVector<int, unsigned int> v(10);
        v.push_back(42);
        v.push_back(43);
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(BM_FixedVector);

static void BM_Array(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        int a[10];
        unsigned int e = 0;
        a[e] = 42;
        ++e;
        benchmark::DoNotOptimize(a[e]);
        benchmark::DoNotOptimize(e);
        a[e] = 43;
        ++e;
        benchmark::DoNotOptimize(a[e]);
        benchmark::DoNotOptimize(e);
    }
}

BENCHMARK(BM_Array);

static void BM_StdVector(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        std::vector<int> v;
        v.reserve(10);
        v.push_back(42);
        v.push_back(43);
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(BM_StdVector);

BENCHMARK_MAIN()
