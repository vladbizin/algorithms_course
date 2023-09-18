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
void bucket_sort(std::vector<T>& v, int n)
{
	if (v.size() <= 1) return;

	n = v.size() < n ? v.size() : n;
	std::vector<std::vector<T>> buckets(n);
	auto minmax= std::minmax_element(v.begin(), v.end());
	const T denom = *minmax.second- *minmax.first;
	if (denom == 0) return;
	int i;
	for (i = 0; i < v.size(); i++)
	{
		buckets[std::floor((n - 1) * (v[i] - *minmax.first) / denom)].push_back(std::move(v[i]));
	}
	for (i = 0; i < n; i++)
	{
		bucket_sort(buckets[i], n);
	}
	v = std::move(buckets[0]);
	for (i = 1; i < n; i++)
	{
		std::move(buckets[i].begin(), buckets[i].end(), std::back_inserter(v));
	}
	return;
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
	merge_sort(v, mid+1, end);
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

	std::normal_distribution<> normal_dist{ 0,5E2 };
	auto normal_gen = [&normal_dist, &mersenne_engine]() {
		return normal_dist(mersenne_engine);
	};




	

	/*int size = 1E6;
	int dots = 100;
	int i_step = size / 1000 / dots;
	const size_t iterations = 1E1;
	std::vector<double> v;
	v.reserve(size);

	std::vector<double> ten_buckets(dots), twenty_buckets(dots), hundred_buckets(dots), thousand_buckets(dots), N(dots);
	for (int i = 0; i < dots * i_step; i += i_step)
	{
		int n = i / i_step;
		std::cout << n + 1 << '\n';
		v.resize((n + 1) * i_step * 1000);
		std::cout<<v.size()<<'\n';
		generate(v.begin(), v.end(), uniform_gen);
		for (int j = 0; j < iterations; j++)
		{
			auto to_sort = v;
			start = Time::now();
			bucket_sort(to_sort, 10);
			stop = Time::now();
			ten_buckets[n] += (std::chrono::duration_cast<mks>(stop - start).count());


			start = Time::now();
			bucket_sort(to_sort, 20);
			stop = Time::now();
			twenty_buckets[n] += (std::chrono::duration_cast<mks>(stop - start).count());


			start = Time::now();
			bucket_sort(to_sort, 100);
			stop = Time::now();
			hundred_buckets[n] += (std::chrono::duration_cast<mks>(stop - start).count());

			start = Time::now();
			bucket_sort(to_sort, 1000);
			stop = Time::now();
			thousand_buckets[n] += (std::chrono::duration_cast<mks>(stop - start).count());
		}
		ten_buckets[n] /= iterations;
		twenty_buckets[n] /= iterations;
		hundred_buckets[n] /= iterations;
		thousand_buckets[n] /= iterations;
		N[n] = (n + 1) * i_step;
	}

	std::scientific;
	plt::figure_size(1200, 780);
	plt::title("bucket sort");
	plt::named_plot("10 buckets", N, ten_buckets, "-");
	plt::named_plot("20 buckets", N, twenty_buckets, "-");
	plt::named_plot("100 buckets", N, hundred_buckets, "-");
	plt::named_plot("1000 buckets", N, thousand_buckets, "-");
	plt::xlabel("N, E3");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();*/




	/*const int size = 1E7;
	const int dots = 100;
	const int i_step = size / 1000 / dots;
	const int iterations = 1E0;
	std::vector<float> v;
	v.reserve(size);

	std::vector<double> merge(dots), bucket_hundred(dots), N(dots);
	for (int i = 0; i < dots * i_step; i += i_step)
	{
		int n = i / i_step;
		std::cout << n + 1 << '\n';
		v.resize((n + 1) * i_step * 1000);
		std::cout << v.size() << '\n';
		generate(v.begin(), v.end(), uniform_gen);
		auto to_sort = v;
		for (int j = 0; j < iterations; j++)
		{
			to_sort = v;
			start = Time::now();
			merge_sort(to_sort, 0, to_sort.size() - 1);
			stop = Time::now();
			merge[n] += (std::chrono::duration_cast<mks>(stop - start).count());

			to_sort = v;
			start = Time::now();
			bucket_sort(to_sort, 100);
			stop = Time::now();
			bucket_hundred[n] += (std::chrono::duration_cast<mks>(stop - start).count());

		}
		merge[n] /= iterations;
		bucket_hundred[n] /= iterations;
		N[n] = (n + 1) * i_step;
	}

	std::scientific;
	plt::figure_size(1200, 780);
	plt::title("merge vs bucket");
	plt::named_plot("merge sort", N, merge, "-");
	plt::named_plot("bucket sort", N, bucket_hundred, "-");
	plt::xlabel("N, E3");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();*/




	/*const int size = 1E7;
	const int dots = 100;
	const int i_step = size / 1000 / dots;
	const int iterations = 1E0;
	std::vector<float> v_normal;
	std::vector<float> v_uniform;
	v_normal.reserve(size);
	v_uniform.reserve(size);

	std::vector<double> bucket_normal(dots), bucket_uniform(dots), N(dots);
	for (int i = 0; i < dots * i_step; i += i_step)
	{
		int n = i / i_step;
		std::cout << n + 1 << '\n';
		v_normal.resize((n + 1) * i_step * 1000);
		v_uniform.resize((n + 1) * i_step * 1000);
		generate(v_normal.begin(), v_normal.end(), normal_gen);
		auto to_sort_normal = v_normal;
		generate(v_uniform.begin(), v_uniform.end(), uniform_gen);
		auto to_sort_uniform = v_uniform;
		for (int j = 0; j < iterations; j++)
		{
			to_sort_normal = v_normal;
			start = Time::now();
			bucket_sort(to_sort_normal, 100);
			stop = Time::now();
			bucket_normal[n] += (std::chrono::duration_cast<mks>(stop - start).count());

			to_sort_uniform = v_uniform;
			start = Time::now();
			bucket_sort(to_sort_uniform, 100);
			stop = Time::now();
			bucket_uniform[n] += (std::chrono::duration_cast<mks>(stop - start).count());

		}
		bucket_normal[n] /= iterations;
		bucket_uniform[n] /= iterations;
		N[n] = (n + 1) * i_step;
	}

	std::scientific;
	plt::figure_size(1200, 780);
	plt::title("merge sort");
	plt::named_plot("normal dist", N, bucket_normal, "-");
	plt::named_plot("uniform dist", N, bucket_uniform, "-");
	plt::xlabel("N, E3");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();*/




	const int size = 1E4;
	const int iterations = 1E0;
	std::vector<float> v(size);
	generate(v.begin(), v.end(), uniform_gen);
	std::vector<float> bubble = v;

	std::vector<double> bucket(2 * size - 1), merge(2 * size - 1);
	std::vector<int> N(2 * size - 1);
	
	int i, j;
	auto to_sort = bubble;

	N[v.size() - 1] = 0;
	for (int k = 0; k < iterations; k++)
	{
		to_sort = bubble;
		start = Time::now();
		bucket_sort(to_sort, 100);
		stop = Time::now();
		bucket[v.size() - 1] += (std::chrono::duration_cast<mks>(stop - start).count());

		to_sort = bubble;
		start = Time::now();
		merge_sort(to_sort, 0, to_sort.size()-1);
		stop = Time::now();
		merge[v.size() - 1] += (std::chrono::duration_cast<mks>(stop - start).count());
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
			bucket_sort(to_sort, 100);
			stop = Time::now();
			bucket[v.size() + i] += (std::chrono::duration_cast<mks>(stop - start).count());

			to_sort = bubble;
			start = Time::now();
			merge_sort(to_sort, 0, to_sort.size() - 1);
			stop = Time::now();
			merge[v.size() + i] += (std::chrono::duration_cast<mks>(stop - start).count());
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
			bucket_sort(to_sort, 100);
			stop = Time::now();
			bucket[v.size() - 2 - i] += (std::chrono::duration_cast<mks>(stop - start).count());

			to_sort = bubble;
			start = Time::now();
			merge_sort(to_sort, 0, to_sort.size() - 1);
			stop = Time::now();
			merge[v.size() - 2 - i] += (std::chrono::duration_cast<mks>(stop - start).count());
		}
	}

	std::scientific;
	plt::figure_size(1200, 780);
	plt::title("matrix");
	plt::named_plot("bucket sort", N, bucket, "-");
	plt::xlabel("N, E3");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::figure_size(1200, 780);
	plt::title("matrix");
	plt::named_plot("merge sort", N, merge, "-");
	plt::xlabel("N, E3");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();
}