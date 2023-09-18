#include <iostream>
#include <stdint.h>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <unordered_set>

#include "C:\Users\vladb\source\repos\7smstr\matplotlib-cpp-master\matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include<chrono>
using Time = std::chrono::high_resolution_clock;
using mks = std::chrono::microseconds;
auto start = Time::now();
auto stop = Time::now();

bool is_prime(int n)
{
    // Corner cases
    if (n <= 1)  return false;
    if (n <= 3)  return true;

    // This is checked so that we can skip 
    // middle five numbers in below loop
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;

    return true;
}

int next_prime(int N)
{

    // Base case
    if (N <= 1)
        return 2;

    int prime = N;
    bool found = false;

    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;

        if (is_prime(prime))
            found = true;
    }

    return prime;
}






class XXHash32
{
private:

    static const uint32_t Prime1 = 2654435761U;
    static const uint32_t Prime2 = 2246822519U;
    static const uint32_t Prime3 = 3266489917U;
    static const uint32_t Prime4 = 668265263U;
    static const uint32_t Prime5 = 374761393U;

    static const uint32_t MaxBufferSize = 15 + 1;

    uint32_t      state[4]; // state[2] == seed if totalLength < MaxBufferSize
    unsigned char buffer[MaxBufferSize];
    unsigned int  bufferSize;
    uint64_t      totalLength;

    static inline uint32_t rotateLeft(uint32_t x, unsigned char bits)
    {
        return (x << bits) | (x >> (32 - bits));
    }

    static inline void process(const void* data, uint32_t& state0, uint32_t& state1, uint32_t& state2, uint32_t& state3)
    {
        const uint32_t* block = (const uint32_t*)data;
        state0 = rotateLeft(state0 + block[0] * Prime2, 13) * Prime1;
        state1 = rotateLeft(state1 + block[1] * Prime2, 13) * Prime1;
        state2 = rotateLeft(state2 + block[2] * Prime2, 13) * Prime1;
        state3 = rotateLeft(state3 + block[3] * Prime2, 13) * Prime1;
    }

public:

    explicit XXHash32(uint32_t seed)
    {
        state[0] = seed + Prime1 + Prime2;
        state[1] = seed + Prime2;
        state[2] = seed;
        state[3] = seed - Prime1;
        bufferSize = 0;
        totalLength = 0;
    }

    bool add(const void* input, uint64_t length)
    {
        // no data ?
        if (!input || length == 0)  return false;
        
        totalLength += length;
        // byte-wise access
        const unsigned char* data = (const unsigned char*)input;

        // unprocessed old data plus new data still fit in temporary buffer ?
        if (bufferSize + length < MaxBufferSize)
        {
            // just add new data
            while (length-- > 0)
                buffer[bufferSize++] = *data++;
            return true;
        }
        
        // point beyond last byte
        const unsigned char* stop = data + length;
        const unsigned char* stopBlock = stop - MaxBufferSize;
        
        // some data left from previous update ?
        if (bufferSize > 0)
        {
            // make sure temporary buffer is full (16 bytes)
            while (bufferSize < MaxBufferSize)
                buffer[bufferSize++] = *data++;
            
            // process these 16 bytes (4x4)
            process(buffer, state[0], state[1], state[2], state[3]);
        }
        
        // copying state to local variables helps optimizer A LOT
        uint32_t s0 = state[0], s1 = state[1], s2 = state[2], s3 = state[3];
        // 16 bytes at once     while (data <= stopBlock)
        {
            // local variables s0..s3 instead of state[0]..state[3] are much faster
            process(data, s0, s1, s2, s3);
            data += 16;
        }
        // copy back
        state[0] = s0; state[1] = s1; state[2] = s2; state[3] = s3;
        
        // copy remainder to temporary buffer
        bufferSize = stop - data;
        for (unsigned int i = 0; i < bufferSize; i++) buffer[i] = data[i];

        // done
        return true;
    }



    uint32_t hash() const
    {
        uint32_t result = (uint32_t)totalLength;
        // fold 128 bit state into one single 32 bit value
        if (totalLength >= MaxBufferSize)
            result += rotateLeft(state[0], 1) + rotateLeft(state[1], 7) + rotateLeft(state[2], 12) + rotateLeft(state[3], 18);
        else
            // internal state wasn't set in add(), therefore original seed is still stored in state2
            result += state[2] + Prime5;

        // process remaining bytes in temporary buffer
        const unsigned char* data = buffer;
        // point beyond last byte
        const unsigned char* stop = data + bufferSize;
        
        // at least 4 bytes left ? => eat 4 bytes per step
        for (; data + 4 <= stop; data += 4) result = rotateLeft(result + *(uint32_t*)data * Prime3, 17) * Prime4;
        
        // take care of remaining 0..3 bytes, eat 1 byte per step
        while (data != stop)
            result = rotateLeft(result + (*data++) * Prime5, 11) * Prime1;
        
        // mix bits
        result ^= result >> 15;
        result *= Prime2;
        result ^= result >> 13;
        result *= Prime3;
        result ^= result >> 16;
        return result;
    }

    static uint32_t hash(const void* input, uint64_t length, uint32_t seed)
    {
        XXHash32 hasher(seed);
        hasher.add(input, length);
        return hasher.hash();
    }

};


unsigned int PJWHash(char* str)
{
    unsigned int hash = 0;
    unsigned int test = 0;

    for (; *str; str++) {
        hash = (hash << 4) + (unsigned char)(*str);

        if ((test = hash & 0xf0000000) != 0) {
            hash = ((hash ^ (test >> 24)) & (0xfffffff));
        }
    }
    return hash;
}


int word_count(std::string file_name)
{
    std::unordered_set<std::string> set;
    std::ifstream input_file;
    input_file.open(file_name);
    std::string element;

    while (input_file >> element)
    {
        set.insert(element);
    }

    return set.size();
}

int len(std::string file_name)
{
    int len = 0;
    std::ifstream input_file;
    input_file.open(file_name);
    std::string element;

    while (input_file >> element)
    {
        len++;
    }

    return len;
}

class xxset
{
private:
    std::vector<std::list<std::string>> set;
public:

    xxset(int size)
    {
        set.resize(next_prime(int(0.25*double(size))));
    }

    int insert(std::string word)
    {
        int collisions = 0;
        uint32_t result = XXHash32::hash(word.data(), word.length(), 0);
        int pos = result % set.size();

        auto it = set[pos].begin();
        while (it != set[pos].end())
        {
            if (*it == word)
            {
                return collisions;
            }
            it++;
            collisions++;
        }
        set[pos].push_back(word);
        return collisions;
    }

    double filled()
    {
        int non_empty = 0;
        {
            for (int i = 0; i < set.size(); i++)
            {
                if (set[i].size()) non_empty++;
            }
        }
        return non_empty / double(set.size());
    }

};

class pjwset
{
private:
    std::vector<std::list<std::string>> set;
public:

    pjwset(int size)
    {
        set.resize(next_prime(int(0.25 * double(size))));
    }

    int insert(std::string word)
    {
        char* cstr = new char[word.length() + 1];
        strcpy(cstr, word.c_str());
        int collisions = 0;
        uint32_t result = PJWHash(cstr);
        int pos = result % set.size();

        auto it = set[pos].begin();
        while (it!=set[pos].end())
        {
            if (*it == word)
            {
                delete[] cstr;
                return collisions;
            }
            it++;
            collisions++;
        }
        set[pos].push_back(word);
        delete[] cstr;
        return collisions;
    }

    double filled()
    {
        int non_empty = 0;
        {
            for (int i = 0; i < set.size(); i++)
            {
                if (set[i].size()) non_empty++;
            }
        }
        return non_empty / double(set.size());
    }

};


int main()
{
    std::string file_name = "War_and_Peace.txt";
    std::unordered_set<std::string> set;
    std::ifstream input_file;
    std::string line;
    std::string element;



    
    int words = word_count(file_name);
    std::cout << words;
    int size = 1000;
    int step = words / size;
    
    std::vector<double> pjw_collisions(size);
    std::vector<double> xx_collisions(size);

    std::vector<double> pjw_cpi_t(size);
    std::vector<double> xx_cpi_t(size);

    std::vector<double> pjw_cpi_d(size);
    std::vector<double> xx_cpi_d(size);

    std::vector<double> xx_filled(size);
    std::vector<double> pjw_filled(size);

    std::vector<int> Dict_size(size);
    std::vector<int> Txt_size(size);

    for (int i = 0; i < size; i++)
    {
        std::cout << (i + 1) << '\n';
        Dict_size[i] = ((i + 1) * step);

        pjwset pjw_set((i + 1) * step);
        xxset xx_set((i + 1) * step);
        
        xx_collisions[i] = 0;
        pjw_collisions[i] = 0;
        int txt_size = 1;

        input_file.open(file_name);

        while (input_file >> element)
        {
            if (set.size() >= ((i + 1) * step)) break;
            set.insert(element);
            txt_size++;

            pjw_collisions[i] += pjw_set.insert(element);
            xx_collisions[i] += xx_set.insert(element);
        }
        pjw_cpi_d[i] = pjw_collisions[i]/set.size();
        xx_cpi_d[i] = xx_collisions[i]/set.size();

        pjw_cpi_t[i] = pjw_collisions[i] / txt_size;
        xx_cpi_t[i] = xx_collisions[i] / txt_size;

        xx_filled[i] = xx_set.filled();
        pjw_filled[i] = pjw_set.filled();

        Txt_size[i] = txt_size;

        set.clear();
        input_file.close();
    }

    plt::figure_size(1200, 780);
    plt::title("xxHash vs PJWHash");
    plt::named_plot("PJW", Dict_size, pjw_collisions, "o-");
    plt::named_plot("xx", Dict_size, xx_collisions, "o-");
    plt::xlabel("Dictionary size");
    plt::ylabel("Collisions");
    plt::legend();

    plt::figure_size(1200, 780);
    plt::title("xxHash vs PJWHash");
    plt::named_plot("PJW", Dict_size, pjw_cpi_d, "o-");
    plt::named_plot("xx", Dict_size, xx_cpi_d, "o-");
    plt::xlabel("Dictionary size");
    plt::ylabel("Collisions per insertion (average)");
    plt::legend();

    plt::figure_size(1200, 780);
    plt::title("xxHash vs PJWHash");
    plt::named_plot("PJW", Dict_size, pjw_filled, "o-");
    plt::named_plot("xx", Dict_size, xx_filled, "o-");
    plt::xlabel("Dictionary size");
    plt::ylabel("Filling order");
    plt::legend();

    plt::figure_size(1200, 780);
    plt::title("xxHash vs PJWHash");
    plt::named_plot("PJW", Txt_size, pjw_collisions, "o-");
    plt::named_plot("xx", Txt_size, xx_collisions, "o-");
    plt::xlabel("Text size");
    plt::ylabel("Collisions");
    plt::legend();

    plt::figure_size(1200, 780);
    plt::title("xxHash vs PJWHash");
    plt::named_plot("PJW", Txt_size, pjw_cpi_t, "o-");
    plt::named_plot("xx", Txt_size, xx_cpi_t, "o-");
    plt::xlabel("Text size");
    plt::ylabel("Collisions per insertion (average)");
    plt::legend();

    plt::figure_size(1200, 780);
    plt::title("xxHash vs PJWHash");
    plt::named_plot("PJW", Txt_size, pjw_filled, "o-");
    plt::named_plot("xx", Txt_size, xx_filled, "o-");
    plt::xlabel("Text size");
    plt::ylabel("Filling order");
    plt::legend();

    plt::show();





    return 0;
}
