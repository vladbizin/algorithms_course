#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cmath>

#include "C:\Users\vladb\source\repos\7smstr\matplotlib-cpp-master\matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include<chrono>
using Time = std::chrono::high_resolution_clock;
using mks = std::chrono::microseconds;

template<typename T>
std::vector<std::vector<T>>bubbleMatrix(std::vector<T> v)
{
	std::vector<std::vector<T>> matrix(2 * v.size() - 1);
	int i, j;
	matrix[v.size() - 1] = v;
	for (i = 0; i < v.size() - 1; i++)
	{
		for (j = 0; j < v.size() - i - 1; j++)
		{
			if (v[j] > v[j + 1])
			{
				std::swap(v[j], v[j + 1]);
			}
		}
		matrix[v.size() + i] = v;
	}
	v = matrix[v.size() - 1];
	for (i = 0; i < v.size() - 1; i++)
	{
		for (j = 0; j < v.size() - i - 1; j++)
		{
			if (v[j] < v[j + 1])
			{
				std::swap(v[j], v[j + 1]);
			}
		}
		matrix[v.size() - 2 - i] = v;
	}
	return matrix;
}

template<typename T>
void print_vector(std::vector<T>& v) {
	for (auto i : v)
	{
		std::cout << i << "\n";

	}
	std::cout << std::endl;
}

template<typename T>
void print_matrix(std::vector<std::vector<T>>& m)
{
	for (auto i : m)
	{
		printVector(i);
	}
}




template<typename T>
void bitonic_merge(std::vector<T>& v, int low, int cnt, int dir)
{
	if (cnt > 1)
	{
		int k = cnt / 2;
		for (int i = low; i < low + k; i++)
		{
			if (dir == (v[i] > v[i + k])) std::swap(v[i], v[i + k]);
		}
		bitonic_merge(v, low, k, dir);
		bitonic_merge(v, low + k, k, dir);
	}
	else
	{
		return;
	}
}




template<typename T>
void bitonic_sort(std::vector<T>& v, int low, int cnt, int dir)
{
	if (cnt > 1)
	{
		bitonic_sort(v, low, cnt/2, 1);

		bitonic_sort(v, low + cnt/2, cnt/2, 0);

		bitonic_merge(v, low, cnt, dir);
	}
	else
	{
		return;
	}
}




template<typename T>
void merge(std::vector<T>& v, const int left, const int mid, const int right)
{
	const int L_size = mid - left + 1;
	std::vector<T> L;
	L.reserve(L_size);
	for (int i = 0; i < L_size; i++)
	{
		L.push_back(std::move(v[left + i]));
	}


	const int R_size = right - mid;
	std::vector<T> R;
	R.reserve(R_size);
	for (int i = 0; i < R_size; i++)
	{
		R.push_back(std::move(v[mid + 1 + i]));
	}


	int left_i = 0, right_i = 0, v_i = left;
	while (left_i < L_size && right_i < R_size)
	{
		if (L[left_i] <= R[right_i]) v[v_i++] = std::move(L[left_i++]);
		else v[v_i++] = std::move(R[right_i++]);
	}

	while (left_i < L_size) v[v_i++] = std::move(L[left_i++]);
	while (right_i < R_size) v[v_i++] = std::move(R[right_i++]);
	return;
}

template<typename T>
void merge_sort(std::vector<T>& v, const int begin, const int end)
{
	if (begin >= end) return;

	const int mid = begin + (end - begin) / 2;
	merge_sort(v, begin, mid);
	merge_sort(v, mid + 1, end);
	merge(v, begin, mid, end);
	return;
}




int main()
{
	std::random_device rnd_device;
	std::mt19937 mersenne_engine{ rnd_device() };

	auto start = Time::now();
	auto stop = Time::now();

	std::uniform_real_distribution<double> uniform_dist{ 0, 1E3 };
	auto uniform_gen = [&uniform_dist, &mersenne_engine]() {
		return uniform_dist(mersenne_engine);
	};

	std::poisson_distribution<int> poisson_dist(100);
	auto poisson_gen = [&poisson_dist, &mersenne_engine]() {
		return poisson_dist(mersenne_engine);
	};

	std::binomial_distribution<> binomial_dist(100, 0.5);
	auto binomial_gen = [&binomial_dist, &mersenne_engine]() {
		return binomial_dist(mersenne_engine);
	};

	double exp_dist_l = 1;
	std::exponential_distribution<> exponential_dist(exp_dist_l);
	auto exponential_gen = [&exponential_dist, &mersenne_engine]() {
		return exponential_dist(mersenne_engine);
	};

	std::normal_distribution<> normal_dist{ 0,1E1 };
	auto normal_gen = [&normal_dist, &mersenne_engine]() {
		return normal_dist(mersenne_engine);
	};

	

	/*const int size = std::pow(2,13);
	const int iterations = 1E0;
	std::vector<float> v(size);
	generate(v.begin(), v.end(), uniform_gen);
	std::vector<float> bubble = v;

	std::vector<double> bitonic(2 * size - 1), merge(2 * size - 1);
	std::vector<int> N(2 * size - 1);

	int i, j;
	auto to_sort = bubble;

	N[v.size() - 1] = 0;
	for (int k = 0; k < iterations; k++)
	{
		to_sort = bubble;
		start = Time::now();
		bitonic_sort(to_sort, 0, to_sort.size(), 1);
		stop = Time::now();
		bitonic[v.size() - 1] += (std::chrono::duration_cast<mks>(stop - start).count());
	}
	std::cout << 0 << '\n';

	for (i = 0; i < bubble.size() - 1; i++)
	{
		for (j = 0; j < bubble.size() - i - 1; j++)
		{
			if (bubble[j] > bubble[j + 1])
			{
				std::swap(bubble[j], bubble[j + 1]);
			}
		}
		N[v.size() + i] = i + 1;
		std::cout << i + 1 << '\n';
		for (int k = 0; k < iterations; k++)
		{
			to_sort = bubble;
			start = Time::now();
			bitonic_sort(to_sort, 0, to_sort.size(), 1);
			stop = Time::now();
			bitonic[v.size() + i] += (std::chrono::duration_cast<mks>(stop - start).count());
		}
	}


	bubble = v;
	for (i = 0; i < bubble.size() - 1; i++)
	{
		for (j = 0; j < bubble.size() - i - 1; j++)
		{
			if (bubble[j] < bubble[j + 1])
			{
				std::swap(bubble[j], bubble[j + 1]);
			}
		}
		N[v.size() - 2 - i] = -i - 1;
		std::cout << -i - 1 << '\n';
		for (int k = 0; k < iterations; k++)
		{
			to_sort = bubble;
			start = Time::now();
			bitonic_sort(to_sort, 0, to_sort.size(), 1);
			stop = Time::now();
			bitonic[v.size() - 2 - i] += (std::chrono::duration_cast<mks>(stop - start).count());
		}
	}

	std::scientific;
	plt::figure_size(1200, 780);
	plt::title("matrix");
	plt::named_plot("bitonic sort", N, bitonic, "-");
	plt::xlabel("N, E3");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();*/



	int pow = 23;
	const int size = std::pow(2,pow);
	const int iterations = 1E1;
	std::vector<float> v;
	v.reserve(size);

	std::vector<double> merge(pow + 1), bitonic(pow + 1), N(pow + 1);
	for (int i = 0; i <= pow; i += 1)
	{
		v.resize(std::pow(2,i));
		std::cout << v.size() << '\n';
		generate(v.begin(), v.end(), uniform_gen);
		auto to_sort = v;
		for (int j = 0; j < iterations; j++)
		{
			to_sort = v;
			start = Time::now();
			merge_sort(to_sort, 0, to_sort.size() - 1);
			stop = Time::now();
			merge[i] += (std::chrono::duration_cast<mks>(stop - start).count());

			to_sort = v;
			start = Time::now();
			bitonic_sort(to_sort, 0, to_sort.size(), 1);
			stop = Time::now();
			bitonic[i] += (std::chrono::duration_cast<mks>(stop - start).count());

		}
		merge[i] /= iterations;
		bitonic[i] /= iterations;
		N[i] = std::pow(2,i);
	}

	std::scientific;
	plt::figure_size(1200, 780);
	plt::title("merge vs bucket");
	plt::named_plot("merge sort", N, merge, "o-");
	plt::named_plot("bitonic sort", N, bitonic, "o-");
	plt::xlabel("N");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();



}
