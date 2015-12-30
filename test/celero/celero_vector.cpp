#include <celero/Celero.h>
#include <celero/Benchmark.h>

#include <thread>
#include <chrono>

#ifndef WIN32
#include <cstdlib>
#include <cmath>
#endif

#include <vector>
#include <array>
#include <ccc/static_vector.h>
#include <ccc/fixed_vector.h>

#include <random>

const int N = 10000;
int g_prev = 0;

class ArrayFixture: public celero::TestFixture
{
public:
    ArrayFixture()
            : Gen(RandomDevice()), RandomBool(0.5), RandomInt(-100, 100)
    {
    }

    inline bool FillArray(int* a, int N)
    {
        int prev = 0;
        for (int i = 0; i < N; ++i)
        {
            a[i] = i * prev + prev * prev;
            prev = a[i];
        }
        return true;
    }

    inline bool FillArrayWithMultiplication(int* a, int N)
    {
        int prev = 0;
        for (int i = 0; i < N; ++i)
        {
            a[i] = i * prev + prev * prev;
            prev = a[i];
        }
        return true;
    }

    template <typename T>
    inline bool FillVectorWithMultiplication(T& v, int N)
    {
        int prev = 0;
        for (int i = 0; i < N; ++i)
        {
            v.push_back(i * prev + prev * prev);
            prev = v.back();
        }
        return true;
    }

    inline bool FillArrayWithRandom(int* a, int N)
    {
        for (int i = 0; i < N; ++i)
        {
            a[i] = RandomInt(RandomDevice);
        }
        return true;
    }

    template <typename T>
    inline bool FillVectorWithRandom(T& v, int N)
    {
        for (int i = 0; i < N; ++i)
        {
            v.push_back(RandomInt(RandomDevice));
        }
        return true;
    }

    // Improved with Bernoulli distribution of booleans.
    std::random_device RandomDevice;
    std::mt19937 Gen;

    // give "true" 1/2 of the time
    std::bernoulli_distribution RandomBool;
    std::uniform_int_distribution<int> RandomInt;
};

BENCHMARK_F(FillWithMultiplication, std_vector, ArrayFixture, 300, 100)
{
    std::vector<int> v;
    v.reserve(N);
    celero::DoNotOptimizeAway(FillVectorWithMultiplication(v, N));
}

BENCHMARK_F(FillWithMultiplication, ccc_PodVector, ArrayFixture, 300, 100)
{
    ccc::PodVector<int, std::size_t, N> v = ccc::PodVector<int, std::size_t, N>();
    celero::DoNotOptimizeAway(FillVectorWithMultiplication(v, N));
}

BENCHMARK_F(FillWithMultiplication, ccc_StaticVector, ArrayFixture, 300, 100)
{
    ccc::StaticVector<int, std::size_t, N, true> v;
    celero::DoNotOptimizeAway(FillVectorWithMultiplication(v, N));
}

BENCHMARK_F(FillWithMultiplication, ccc_FixedVector, ArrayFixture, 300, 100)
{
    ccc::FixedVector<int> v(N);
    celero::DoNotOptimizeAway(FillVectorWithMultiplication(v, N));
}

BASELINE_F(FillWithMultiplication, stack_array, ArrayFixture, 300, 100)
{
    int a[N];
    celero::DoNotOptimizeAway(FillArrayWithMultiplication(&a[0], N));
}

BENCHMARK_F(FillWithMultiplication, stack_array_fill, ArrayFixture, 300, 100)
{
    int a[N];
    celero::DoNotOptimizeAway(FillArray(&a[0], N));
}

BENCHMARK_F(FillWithMultiplication, heap_array, ArrayFixture, 300, 100)
{
    int* a = new int[N];
    celero::DoNotOptimizeAway(FillArrayWithMultiplication(&a[0], N));
    delete[] a;
}

BENCHMARK_F(FillWithMultiplication, std_array, ArrayFixture, 300, 100)
{
    std::array<int, N> a;
    celero::DoNotOptimizeAway(FillArrayWithMultiplication(&a[0], N));
}
