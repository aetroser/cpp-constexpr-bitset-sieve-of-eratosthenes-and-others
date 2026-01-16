#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <map>
#include <string>
#include <string_view>
#include <sstream>
#include <numeric>
#include <math.h>
#include <set>
#include <fstream>
#include <chrono>
#include <ratio>
#include <type_traits>
#include <random>

using ui64 = uint64_t;
using i64 = int64_t;
using bitset_ui64 = std::vector<ui64>;
using primes_ui64 = std::vector<ui64>;

namespace utility
{
	using namespace std::string_view_literals;

	static constexpr ui64 sum_of_divisors(ui64 n)
	{
		if (n <= 1)
			return 0;
		ui64 s = 1;
		for (ui64 i = 2; i * i <= n; ++i)
			if (n % i == 0)
			{
				s += i;
				if (i * i != n)
					s += n / i;
			}
		return s;
	}

	template <typename T>
	static void display_1d_vector(const std::vector<T>& v)
	{
		for (const T& e : v)
			std::cout << e << '\n';
	}

	template <typename Rep = long long, typename Period = std::milli>
	class Benchmark
	{
		using duration = std::chrono::duration<Rep, Period>;
	public:
		~Benchmark()
		{
			const auto end = std::chrono::steady_clock::now();
			std::cout << std::chrono::duration_cast<duration>(end - _start).count() << time_span() << '\n';
		}
	private:
		constexpr std::string_view time_span() const
		{
			if constexpr (std::is_same_v<std::milli, Period>)
				return "ms"sv;
			if constexpr (std::is_same_v<std::micro, Period>)
				return "us"sv;
			if constexpr (std::is_same_v<std::ratio<1>, Period>)
				return "s"sv;
			if constexpr (std::is_same_v<std::nano, Period>)
				return "ns"sv;
			return "??"sv;
		}

		std::chrono::steady_clock::time_point _start = std::chrono::steady_clock::now();
	};

	static constexpr bitset_ui64 sieve_of_eratosthenes_bitset(ui64 n)
	{
		if (n < 2) // no primes smaller than 2
			return {};

		if (n % 64 != 0)					// If number doesn't fit exactly in 64-bit bitfield, then n is expanded.
			n = ((n + 63) / 64) * 64 - 1;	// Does the rounding (ceil), and subtracts 1, as to not cause another ui64 to be explaced onto the vector (bitset)

		std::vector<ui64> bitset(n / 64 + 1, UINT64_MAX); // Creates the bitset, according to the  size of n
		bitset[0] ^= 0b11;

		for (ui64 i = 2; i * i <= n; ++i)					// 1. Starting point for removing multiples of i
			if (bitset[i / 64] & (1ull << (i % 64)))		// 2. If number in bitset is marked as prime, proceed
				for (ui64 j = i * i; j <= n; j += i)		// 3. Remove multiples of i, starting from i * i
					bitset[j / 64] &= ~(1ull << (j % 64));	// 4. Uncheck bit of multiple of i

		return bitset; // Returns the completed bitset of primes
	}

	static constexpr primes_ui64 extract_primes(const bitset_ui64& v)
	{
		std::vector<ui64> primes;
		size_t size = v.size();

		for (size_t i = 0; i < size; ++i)
		{
			for (size_t j = 0; j < 64; ++j)
			{
				bool prime = v[i] & 1ull << j;
				if (prime)
					primes.emplace_back(j + 64 * i);
			}
		}

		return primes;
	}

	static void print_sieve_bitset(const bitset_ui64& v)
	{
		size_t size = v.size();
		for (size_t i = 0; i < size; ++i)
		{
			for (size_t j = 0; j < 64; ++j)
			{
				bool prime = v[i] & 1ull << j;
				std::cout << prime;
			}
			std::cout << '\n';
		}
	}
}
