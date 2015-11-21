#include <celero/Celero.h>
#include <celero/Benchmark.h>

#include <thread>
#include <chrono>

#ifndef WIN32
#include <cstdlib>
#include <cmath>
#endif

#include <vector>
#include <ccc/static_vector.h>
#include <ccc/fixed_vector.h>

#include <random>

const int N = 10000;

class ArrayFixture : public celero::TestFixture
{
public:
	ArrayFixture() :
		Gen(RandomDevice()),
		RandomBool(0.5),
		RandomInt(-100, 100)
	{
	}

	inline bool FillArray(int* a, int N)
	{
		int prev = 0;
		for (int i = 0; i < N; ++i)
		{
			a[i] = i * prev + prev * prev;
			prev = a[i];
			//a[i] = RandomInt(RandomDevice);
		}
		return true;
	}

	template <typename T>
	inline bool FillVector(T& v, int N)
	{
		for (int i = 0; i < N; ++i)
		{
			int prev = 0;
			v.push_back(i * prev + prev * prev);
			prev = v.back();
			//v.push_back(RandomInt(RandomDevice));
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

BASELINE_F(CCCTL, std_vector, ArrayFixture, 30, 100)
{
	std::vector<int> v;
	v.reserve(N);
	//for (int i = 0; i < N; ++i)
	//{
	//	v.push_back(i);
	//}
	celero::DoNotOptimizeAway(FillVector(v, N));
}

BENCHMARK_F(CCCTL, ccc_PodVector, ArrayFixture, 30, 100)
{
	ccc::PODVector<int, std::size_t, N> v = ccc::PODVector<int, std::size_t, N>();
	//for (int i = 0; i < N; ++i)
	//{
	//	v.push_back(i);
	//}
	celero::DoNotOptimizeAway(FillVector(v, N));
}

BENCHMARK_F(CCCTL, ccc_StaticVector, ArrayFixture, 30, 100)
{
	ccc::StaticVector<int, std::size_t, N> v;
	//for (int i = 0; i < N; ++i)
	//{
	//	v.push_back(i);
	//}
	celero::DoNotOptimizeAway(FillVector(v, N));
}

BENCHMARK_F(CCCTL, ccc_FixedVector, ArrayFixture, 30, 100)
{
	ccc::FixedVector<int> v(N);
	//for (int i = 0; i < N; ++i)
	//{
	//	v.push_back(i);
	//}
	celero::DoNotOptimizeAway(FillVector(v, N));
}

BENCHMARK_F(CCCTL, stack_array, ArrayFixture, 30, 100)
{
	int a[N];
	//for (int i = 0; i < N; ++i)
	//{
	//	a[i] = i;
	//}
	celero::DoNotOptimizeAway(FillArray(&a[0], N));
}

BENCHMARK_F(CCCTL, heap_array, ArrayFixture, 30, 100)
{
	int* a = new int[N];
	//for (int i = 0; i < N; ++i)
	//{
	//	a[i] = i;
	//}
	celero::DoNotOptimizeAway(FillArray(&a[0], N));
	delete[] a;
}
