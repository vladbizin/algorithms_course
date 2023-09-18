#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#include "C:\Users\vladb\source\repos\7smstr\matplotlib-cpp-master\matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include<chrono>
using Time = std::chrono::high_resolution_clock;
using mks = std::chrono::microseconds;
auto start = Time::now();
auto stop = Time::now();



void print_marriages(std::vector<int>& w_partners)
{
    int n = w_partners.size();

    std::cout << "\n-----------------\npartner couples\n________________\n|woman\t|man\t|" << std::endl;
    for (int w = 0; w < n; w++)
    {
        std::cout << "|" << w << "\t" << '|' << w_partners[w] << "\t|\n";
    }
    std::cout << std::endl;
}

void generate_info(std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref, std::vector<std::vector<int>>& rmw, std::vector<std::vector<int>>& rwm)
{
    int n = m_pref.size();

    std::vector<int> rand_indices(n);
    for (int i = 0; i < n; i++)
    {
        rand_indices[i] = i;
    }


    for (int i = 0; i < n; i++)
    {
        std::random_shuffle(rand_indices.begin(), rand_indices.end());
        m_pref[i] = rand_indices;
        std::random_shuffle(rand_indices.begin(), rand_indices.end());
        w_pref[i] = rand_indices;
        for (int r = 0; r < n; r++)
        {
            rmw[i][m_pref[i][r]] = r;
            rwm[i][w_pref[i][r]] = r;
        }
    }
}

void print_info(std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref, std::vector<std::vector<int>>& rmw, std::vector<std::vector<int>>& rwm)
{
    int n = m_pref.size();

    std::cout << "men preferences:\nm\\r\t";

    for (int r = 0; r < n; r++) std::cout << r << '\t';
    std::cout << "\n";

    for (int m = 0; m < n; m++)
    {
        std::cout << m << '\t';
        for (int r = 0; r < n; r++)
        {
            std::cout << m_pref[m][r] << '\t';
        }
        std::cout << std::endl;
    }

    std::cout << "\nmen ranks:\nm\\w\t";

    for (int w = 0; w < n; w++) std::cout << w << '\t';
    std::cout << "\n";

    for (int m = 0; m < n; m++)
    {
        std::cout << m << '\t';
        for (int w = 0; w < n; w++)
        {
            std::cout << rmw[m][w] << '\t';
        }
        std::cout << std::endl;
    }

    std::cout << "\nwomen preferences:\nw\\r\t";

    for (int r = 0; r < n; r++) std::cout << r << '\t';
    std::cout << "\n";

    for (int w = 0; w < n; w++)
    {
        std::cout << w << '\t';
        for (int r = 0; r < n; r++)
        {
            std::cout << w_pref[w][r] << '\t';
        }
        std::cout << std::endl;
    }

    std::cout << "\nwomen ranks:\nw\\m\t";

    for (int m = 0; m < n; m++) std::cout << m << '\t';
    std::cout << "\n";

    for (int w = 0; w < n; w++)
    {
        std::cout << w << '\t';
        for (int m = 0; m < n; m++)
        {
            std::cout << rwm[w][m] << '\t';
        }
        std::cout << std::endl;
    }

}




bool w_prefers_m_over_m1(std::vector<std::vector<int>>& w_pref, int w, int m, int m1)
{
    int n = w_pref.size();
    for (int i = 0; i < n; i++)
    {
        if (w_pref[w][i] == m1) return false;
        else if (w_pref[w][i] == m) return true;
    }
}

std::vector<int> stable_marriages(std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref)
{
    int n = m_pref.size();
    std::vector<int> w_partners(n, -1);

    std::vector<bool> m_single(n, true);

    int single_count = n;
    int m, w, m1;

    while (single_count > 0)
    {
        for (m = 0; m < n; m++) if (m_single[m] == true) break;
   
        for (int i = 0; i < n && m_single[m] == true; i++)
        {
            w = m_pref[m][i];

            if (w_partners[w] == -1)
            {
                w_partners[w] = m;
                m_single[m] = false;
                single_count--;
            }
            else
            {
                m1 = w_partners[w];

                if (w_prefers_m_over_m1(w_pref, w, m, m1))
                {
                    w_partners[w] = m;
                    m_single[m] = false;
                    m_single[m1] = true;
                }
            }
        }
    }
    return w_partners;
}





int single_count(std::vector<int>& m_single)
{
    return std::accumulate(m_single.begin(), m_single.end(), 0);
}

int next_man(int m, std::vector<int>& m_single)
{
    int n = m_single.size();
    for (int ind, i = 1; i < n; i++)
    {
        ind = m + i < n ? m + i : m + i - n;
        if (m_single[ind]) return ind;
    }
}

void try_marry_next_man(int m, std::vector<int>& m_single, std::vector<int>& w_partners, std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref)
{
    int n = m_pref.size();
    for (int w, m1, i = 0; i < n && m_single[m]; i++)
    {
        w = m_pref[m][i];

        if (w_partners[w] == -1)
        {
            w_partners[w] = m;
            m_single[m] = 0;
        }
        else
        {
            m1 = w_partners[w];

            if (w_prefers_m_over_m1(w_pref, w, m, m1))
            {
                w_partners[w] = m;
                m_single[m] = false;
                m_single[m1] = true;
            }
        }
    }
    if (single_count(m_single)) try_marry_next_man(next_man(m, m_single), m_single, w_partners, m_pref, w_pref);
    else
    {
        //print_marriages(w_partners);
        return;
    }

}

void stable_marriages_recursive_n2(std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref)
{
    int n = m_pref.size();
    std::vector<int> w_partners(n, -1);
    std::vector<int> m_single(n, 1);
    
    try_marry_next_man(0, m_single, w_partners, m_pref, w_pref);
}




void marry_m_w(int m, int w, std::vector<bool>& w_single, std::vector<int>& m_partners, std::vector<int>& w_partners)
{
    m_partners[m] = w;
    w_partners[w] = m;
    w_single[w] = false;
}

bool stable(int m, int w, std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref, std::vector<std::vector<int>>& rmw, std::vector<std::vector<int>>& rwm, std::vector<bool>& w_single, std::vector<int>& m_partners, std::vector<int>& w_partners)
{
    int r = rmw[m][w];
    bool s = true;
    int i = 0;
    int alt_w;
    int alt_m;
    while (s && i < r)                                                              //check if (there is alternative w that m prefers to w) and (alternative 
    {                                                                               //woman prefers m to her current partner)
        alt_w = m_pref[m][i++];
        if (!w_single[alt_w]) s = (rwm[alt_w][m] > rwm[alt_w][w_partners[alt_w]]);
    }

    i = 0;
    r = rwm[w][m];
    while (s && i < r)                                                              //check if (there is alternative m that w prefers to m) and (alternative
    {                                                                               //man prefers w to his current partner)
        alt_m = w_pref[w][i++];
        if (alt_m<m) s = (rmw[alt_m][w] > rmw[alt_m][m_partners[alt_m]]);
    }
    return s;
}

void try_marry_m(int m, std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref, std::vector<std::vector<int>>& rmw, std::vector<std::vector<int>>& rwm, std::vector<bool>& w_single, std::vector<int>& m_partners, std::vector<int>& w_partners)
{
    int n = m_pref.size();
    for (int w : m_pref[m])
    {
        if ((w_single[w]) && (stable(m, w, m_pref, w_pref, rmw, rwm, w_single, m_partners, w_partners)))
        {
            marry_m_w(m, w, w_single, m_partners, w_partners);
            if (m < n - 1) try_marry_m(m + 1, m_pref, w_pref, rmw, rwm, w_single, m_partners, w_partners);
            else
            {
                //print_marriages(w_partners);
                return;
            }
            w_single[w] = true;
        }
    }
}

void stable_marriages_recursive(std::vector<std::vector<int>>& m_pref, std::vector<std::vector<int>>& w_pref, std::vector<std::vector<int>>& rmw, std::vector<std::vector<int>>& rwm)
{
    int n = m_pref.size();
    std::vector<bool> w_single(n, true);
    std::vector<int> m_partners(n,-1);
    std::vector<int> w_partners(n,-1);
    try_marry_m(0, m_pref, w_pref, rmw, rwm, w_single, m_partners, w_partners);
}







int main()
{
    //int n = 10;
    // 
    //std::vector<std::vector<int>> m_pref(n, std::vector<int>(n, 0));  //arrays of w by their ranks for rach m:  m_pref[m][0] is a woman that m prefers most and so on
    //std::vector<std::vector<int>> w_pref(n, std::vector<int>(n, 0));  //arrays of m by their ranks for rach w:  w_pref[w][0] is a woman that m prefers most and so on
    //std::vector<std::vector<int>> rmw(n, std::vector<int>(n, 0));       //arrays of ranks of w for each m: rmw[m][w] is a rank of w for m
    //std::vector<std::vector<int>> rwm(n, std::vector<int>(n, 0));       //arrays of ranks of m for each w: rwm[w][m] is a rank of m for w
    // 
    //generate_info(m_pref, w_pref, rmw, rwm);
    // 
    //print_info(m_pref, w_pref, rmw, rwm);
    // 
    //std::vector<int> w_partners = stable_marriages(m_pref, w_pref);
    //print_marriages(w_partners);
    // 
    //stable_marriages_recursive_n2(m_pref, w_pref);

    //stable_marriages_recursive(m_pref, w_pref, rmw, rwm);














    int max = 10000;
    int size = 100;
    int step = max / size;
    std::vector<int> pairs(size);
    std::vector<double> recursive_time_n2(size);
    //std::vector<double> recursive_time(size);


    for (int n = step; n <= max; n+=step)
    {
        std::cout << (n / step - 1) << '\n';
        pairs[n / step - 1] = n;


        std::vector<int> rand_indices(n, 0);
        std::vector < std::vector<int>> m_pref(n, std::vector<int>(n, 0));
        std::vector < std::vector<int>> w_pref(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> rmw(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> rwm(n, std::vector<int>(n, 0));


        generate_info(m_pref, w_pref, rmw, rwm);
        
        start = Time::now();
        stable_marriages_recursive_n2(m_pref, w_pref);
        stop = Time::now();
        recursive_time_n2[n / step - 1] = (std::chrono::duration_cast<mks>(stop - start).count())/1e6;

        /*start = Time::now();
        stable_marriages_recursive(m_pref, w_pref, rmw, rwm);
        stop = Time::now();
        recursive_time[n / step - 1] = (std::chrono::duration_cast<mks>(stop - start).count()) / 1e6;*/

    }
    plt::figure_size(1200, 780);
    plt::title("stable marriages");
    plt::named_plot("loop time, s", pairs, recursive_time_n2, "o-");
    //plt::semilogy(pairs, recursive_time, "o-");
    plt::xlabel("pairs amount, N");
    plt::ylabel("time");
    plt::legend();

    plt::show();







    return 0;
}   