

#include<numeric>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<chrono>
#include<vector>
#include<cmath>
#include<codecvt>
#include<iomanip>
#include<bitset>
#include<valarray>
#include "C:\Users\vladb\source\repos\7smstr\matplotlib-cpp-master\matplotlibcpp.h"
namespace plt = matplotlibcpp;

using Time = std::chrono::high_resolution_clock;
using mks = std::chrono::microseconds;

std::vector<int> brute_force(const std::wstring& str, const std::wstring& txt)
{
    std::vector<int> results(3, 0); //0- индекс первого вхождения, 1- число найденных подстрок, 2- число коллизий
    results[0] = -1;
    const size_t M = str.length();
    const size_t N = txt.length();
    int i, j;

    for (i = 0; i < N - M; i++)
    {
        for (j = 0; j < M; j++)
        {
            if (j == 1)
            {
                results[2] += 1;
            }
            if (str[j] != txt[i+j])
            {
                break;
            }
        }
        if (j == M)
        {
            if (results[0] == -1) results[0] = i;
            results[1] += 1;
        }
    }
    return results;
}


std::vector<int> RK_basic_search(const std::wstring& str, const std::wstring& txt)
{
    std::vector<int> results(3, 0); //0- индекс первого вхождения, 1- число найденных подстрок, 2- число коллизий
    results[0] = -1;
    const size_t M = str.length();
    const size_t N = txt.length();
    unsigned __int64 str_hash = 0;      //string sum value 
    unsigned __int64 txt_hash = 0;      //txt sum value
    const unsigned int modulo = 999999937;
    int i, j;



    for (i = 0; i < M; i++)
    {
        str_hash = (str_hash + str[i]) % modulo;
        txt_hash = (txt_hash + txt[i]) % modulo;
    }


    for (i = 0; i <= N - M; i++)
    {
        if (str_hash == txt_hash)
        {
            results[2] += 1;
            for (j = 0; j < M; j++)
            {
                if (str[j] != txt[i+j]) break;
            }

            if (j == M)
            {
                if (results[0] == -1) results[0] = i;
                results[1] += 1;
            }
        }

        if (i < N - M)
        {
            txt_hash = ((txt_hash - txt[i]) % modulo + txt[i+M]) % modulo;
        }
    }
    return results;
}


std::vector<int> RK_adv_search(const std::wstring& str, const std::wstring& txt)
{
    std::vector<int> results(3, 0); //0- индекс первого вхождения, 1- число найденных подстрок, 2- число коллизий
    results[0] = -1;
    const size_t M = str.length();
    const size_t N = txt.length();
    unsigned __int64 modulo = 999999999989;
    const unsigned int alph_size = 1000;
    unsigned __int64 alph_size_pow = 1;
    int i=0, j;
    unsigned __int64 str_hash = str[i] % modulo;
    unsigned __int64 txt_hash = txt[i] % modulo;

    for (i = 1; i < M; i++)
    {
        str_hash = (str_hash * alph_size + str[i]) % modulo;
        txt_hash = (txt_hash * alph_size + txt[i]) % modulo;
        alph_size_pow = (alph_size_pow * alph_size) % modulo;
    }


    for (i = 0; i <= N - M; i++)
    {
        if (str_hash == txt_hash)
        {
            results[2] += 1;
            for (j = 0; j < M; j++)
            {
                if (str[j] != txt[i+j]) break;
            }

            if (j == M)
            {
                if (results[0] == -1) results[0] = i;
                results[1] += 1;
            }
        }

        if (i < N - M)
        {
            txt_hash = (((txt_hash - txt[i] * alph_size_pow) % modulo + modulo) * alph_size + txt[i+M]) % modulo;
        }
    }
    return results;
}


std::wstring readFile(const char* filename)
{
    std::wifstream wif(filename);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf16<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}




int main()
{
    /*std::wstring wtxt = readFile("ch.txt");
    int len = 9;
    std::wstring wstr= wtxt.substr(wtxt.length() / 2, len);
    std::wstring_convert<std::codecvt_utf16<wchar_t>> conv1;
    std::string txt = conv1.to_bytes(wtxt);
    std::string str = conv1.to_bytes(wstr);
    std::vector<int> basic_res = RK_basic_search(wstr, wtxt);
    std::vector<int> adv_res = RK_adv_search(wstr, wtxt);
    std::vector<int> brute_res = brute_force(wstr, wtxt);
    std::cout << txt.length() / 2 << '\n';
    std::cout << basic_res[0] << '\t' << basic_res[1] << '\t' << basic_res[2] << '\n';
    std::cout << adv_res[0] << '\t' << adv_res[1] << '\t' << adv_res[2] << '\n';
    std::cout << brute_res[0] << '\t' << brute_res[1] << '\t' << brute_res[2] << '\n';*/










    std::wstring wtxt = readFile("ch.txt");
    int str_len = 9;
    std::wstring wstr = wtxt.substr(wtxt.length() / 2, str_len);
    std::wstring_convert<std::codecvt_utf16<wchar_t>> conv1;

    const size_t iterations = 1E3;
    const int M = wstr.length();
    const int N = wtxt.length();
    const int size = 50;
    const int n = N / size;
    std::vector<double> basic_dur(size), adv_dur(size), brute_dur(size), N_x(size);
    std::vector<int> basic_res;
    std::vector<int> adv_res;
    std::vector<int> brute_res;

    for (size_t i = 0; i < size ; i++)
    {
        std::cout << (i + 1) * 100 / size << '\n';
        std::wstring to_search_through = wtxt.substr(0, (i + 1) * n );
        N_x[i] = (i + 1)*n;

        auto start = Time::now();
        for (size_t j = 0; j < iterations; j++)
        {
            basic_res = RK_basic_search(wstr, to_search_through);
        };
        auto stop = Time::now();
        basic_dur[i]=(std::chrono::duration_cast<mks>(stop - start).count()) / iterations;

        start = Time::now();
        for (size_t j = 0; j < iterations; j++)
        {
            adv_res = RK_adv_search(wstr, to_search_through);
        };
        stop = Time::now();
        adv_dur[i] = (std::chrono::duration_cast<mks>(stop - start).count()) / iterations;

        start = Time::now();
        for (size_t j = 0; j < iterations; j++)
        {
            brute_res = brute_force(wstr, to_search_through);
        };
        stop = Time::now();
        brute_dur[i] = (std::chrono::duration_cast<mks>(stop - start).count()) / iterations;
    }
 
    plt::figure_size(1200, 780);
    plt::title("M=const=" + std::to_string(M) + ", " + std::to_string(iterations) + " iterations");
    plt::named_plot("basic RK search", N_x, basic_dur, "o-");
    plt::named_plot("advanced RK search", N_x, adv_dur, "o-");
    plt::named_plot("brute force search", N_x, brute_dur, "o-");
    plt::xlabel("N");
    plt::ylabel("Average Time, mks");
    plt::legend();
    plt::show();



    /*std::wstring wtxt = readFile("ch.txt");
    std::wstring wstr;
    std::wstring_convert<std::codecvt_utf16<wchar_t>> conv1;

    const size_t iterations = 1E3;
    int M;
    const int N = wtxt.length();
    const int size=9;
    std::vector<double> basic_dur(size), adv_dur(size), brute_dur(size), M_x(size);
    std::vector<int> basic_res;
    std::vector<int> adv_res;
    std::vector<int> brute_res;

    for (size_t i = 0; i < size; i++)
    {
        wstr = wtxt.substr(wtxt.length()/2, i + 1);
        M_x[i] = (i + 1);

        auto start = Time::now();
        for (size_t j = 0; j < iterations; j++)
        {
            basic_res = RK_basic_search(wstr, wtxt);
        };
        auto stop = Time::now();
        basic_dur[i] = (std::chrono::duration_cast<mks>(stop - start).count()) / iterations;

        start = Time::now();
        for (size_t j = 0; j < iterations; j++)
        {
            adv_res = RK_adv_search(wstr, wtxt);
        };
        stop = Time::now();
        adv_dur[i] = (std::chrono::duration_cast<mks>(stop - start).count()) / iterations;

        start = Time::now();
        for (size_t j = 0; j < iterations; j++)
        {
            brute_res = brute_force(wstr, wtxt);
        };
        stop = Time::now();
        brute_dur[i] = (std::chrono::duration_cast<mks>(stop - start).count()) / iterations;
    }
 
    plt::figure_size(1200, 780);
    plt::title("N=const=" + std::to_string(N) + ", " + std::to_string(iterations) + " iterations");
    plt::named_plot("basic search", M_x, basic_dur, "o-");
    plt::named_plot("advanced search", M_x, adv_dur, "o-");
    plt::named_plot("brute force search", M_x, brute_dur, "o-");
    plt::xlabel("M");
    plt::ylabel("Average Time, mks");
    plt::legend();
    plt::show();*/



}