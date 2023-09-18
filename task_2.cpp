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
				std:: swap(v[j], v[j + 1]);
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
void printVector(std::vector<T>& v) {
	for (auto i : v)
	{
		std::cout << i << " ";

	}
	std::cout << std::endl;
}

template<typename T>
void printMatrix(std::vector<std::vector<T>>& m)
{
	for (auto i : m)
	{
		printVector(i);
	}
}




template<typename T>
int partition(std::vector<T>& v, int low, int high)
{
	T pivot = v[(low + high) / 2];
	int i = low;
	int j = high;
	while (true)
	{
		while (v[i] < pivot)
			i += 1;
		while (v[j] > pivot)
			j -= 1;
		if (i >= j)
			return j;
		std::swap(v[i++], v[j--]);
	}
}

template<typename T>
void quicksort(std::vector<T>& v, int low, int high)
{
	if (low >= high)
		return;
	else
	{
		int p = partition(v, low, high);
		quicksort(v, low, p);
		quicksort(v, p + 1, high);
	}

}

template<typename T>
void stable_quicksort(std::vector<T>& v)
{
	if (v.size() <= 1)
	{
		return;
	}
	else
	{
		int size = v.size();
		int mid = size / 2;
		auto pivot = v[mid];
		std::vector<T> smaller;
		smaller.reserve(size);
		std::vector<T> greater;
		greater.reserve(size);
		for (int i = 0; i < size; i++)
		{
			if (i != mid)
			{
				if (v[i] < pivot)
					smaller.push_back(std::move(v[i]));
				else if (v[i] > pivot)
					greater.push_back(std::move(v[i]));
				else
				{
					if (i < mid)
						smaller.push_back(std::move(v[i]));
					else
						greater.push_back(std::move(v[i]));
				}
			}
		}
		stable_quicksort(smaller);
		v = smaller;
		v.push_back(std::move(pivot));
		stable_quicksort(greater);
		std::move(greater.begin(), greater.end(), std::back_inserter(v));
		return;
	}
}


int main()
{
	std::random_device rnd_device;
	std::mt19937 mersenne_engine{ rnd_device() };

	auto start = Time::now();
	auto stop = Time::now();

	/*std::uniform_int_distribution<int> uniform_dist{ -100, 100 };
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
	};*/

	std::normal_distribution<> normal_dist{ 0,100};
	auto normal_gen = [&normal_dist, &mersenne_engine]() {
		return normal_dist(mersenne_engine);
	};



	
	int step = 1000;
	int size = 100;
	const size_t iterations = 1E1;
	std::vector<int> stable_vec;
	stable_vec.reserve(step * size);
	std::vector<int> unstable_vec;
	unstable_vec.reserve(step * size);

	std::vector<double> stable_dur(size), unstable_dur(size), N(size);

	for (int i = 0; i < size; i++)
	{
		std::cout << i << '\n';
		stable_vec.resize(step * (i + 1));
		unstable_vec.resize(step * (i + 1));
		
		for (int j = 0; j < iterations; j++)
		{
			generate(stable_vec.begin(), stable_vec.end(), normal_gen);
			unstable_vec = stable_vec;

			start = Time::now();
			stable_quicksort(stable_vec);
			stop = Time::now();
			stable_dur[i] += (std::chrono::duration_cast<mks>(stop - start).count());

			start = Time::now();
			quicksort(unstable_vec, 0, step * (i + 1) - 1);
			stop = Time::now();
			unstable_dur[i] += (std::chrono::duration_cast<mks>(stop - start).count());
		}
		stable_dur[i] /= iterations;
		unstable_dur[i] /= iterations;
		N[i] = i + 1;
	}


	plt::figure_size(1200, 780);
	plt::title("stable quicksort");
	plt::named_plot("stable", N, stable_dur, "o-");
	plt::xlabel("N, E2");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::figure_size(1200, 780);
	plt::title("unstable quicksort");
	plt::named_plot("unstable", N, unstable_dur, "o-");
	plt::xlabel("N, E2");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();




	/*std::vector<double> v(1E4);
	generate(v.begin(), v.end(), normal_gen);
	std::vector<std::vector<double>> M = bubbleMatrix(std::move(v));
	int size = M.size();
	int iter = 1E1;
	std::vector<double> stable_dur(size), unstable_dur(size), S(size);
	for (int i = 0; i < size; i++)
	{
		std::cout << double(i+1)*100/M.size() << '\n';
		S[i] =i;
		for (int j = 0; j < iter; j++);
		{
			v = M[i];
			start = Time::now();
			stable_quicksort(v);
			stop = Time::now();
			stable_dur[i] += (std::chrono::duration_cast<mks>(stop - start).count());

			v = M[i];
			start = Time::now();
			quicksort(v, 0, v.size() - 1);
			stop = Time::now();
			unstable_dur[i] += (std::chrono::duration_cast<mks>(stop - start).count());
		}
		stable_dur[i] /= iter;
		unstable_dur[i] /= iter;
	}
	plt::figure_size(1200, 780);
	plt::title("unstable quicksort");
	plt::named_plot("unstable", S, unstable_dur, "-");
	plt::xlabel("Matrix column");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::figure_size(1200, 780);
	plt::title("stable quicksort");
	plt::named_plot("stable", S, stable_dur, "-");
	plt::xlabel("Matrix column");
	plt::ylabel("Average Time, mks");
	plt::legend();

	plt::show();*/
}