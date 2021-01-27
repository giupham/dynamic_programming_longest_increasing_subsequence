///////////////////////////////////////////////////////////////////////////////
// subsequence.hpp
//
// An exhaustive optimization algorithm for solving
// the longest increasing subsequence problem.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <sstream>
#include <vector>

#include "timer.hpp"

using sequence = std::vector<int>;

// Convert a sequence into a human-readable string useful for pretty-printing
// or debugging.
std::string sequence_to_string(const sequence& seq)
{
    std::stringstream ss;
    ss << "[";
    bool first = true;
    for (auto& x : seq)
    {
        if (!first)
        {
            ss << ", ";
        }
        ss << x;
        first = false;
    }
    ss << "]";
    return ss.str();
}

// Generate a pseudorandom sequence of the given size, using the given
// seed, where all elements are in the range [0, max_element]. max_element
// must be non-negative.
sequence random_sequence(size_t size, unsigned seed, int max_element)
{

    assert(max_element >= 0);

    sequence result;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, max_element);

    for (size_t i = 0; i < size; ++i)
    {
        result.push_back(dist(gen));
    }

    return result;
}

/*This function takes sequence input
* and returns true if they are of an increasing order false otherwise
*/
bool is_increasing(const sequence& A)
{

    for (size_t i = 1; i < A.size(); ++i)
    {
        // if false, return false
        if (!(A.at(i - 1) < A.at(i)))
        {
            return false;
        }
    }

    return true;
}

/*This function takes a sequence as an input
* Find the subsequence where element are in an increasing order
* The longest subsequnce with the above conditions will be returned
*/
sequence longest_increasing_powerset(const sequence& A)
{
    const size_t n = A.size();
    sequence best;
    std::vector<size_t> stack(n + 1, 0);
    size_t k = 0;
    while (true)
    {

        if (stack[k] < n)
        {
            stack[k + 1] = stack[k] + 1;
            k++;
        }
        else
        {
            stack[k - 1]++;
            k--;
        }

        if (k == 0)
        {
            break;
        }

        sequence candidate;
        for (size_t i = 1; i <= k; ++i)
        {
            candidate.push_back(A[stack[i] - 1]);
        }

        // Checks whether sequence is in an increasing order,and whether candidate's size is larger than the current best size
        if (is_increasing(candidate) && best.size() < candidate.size())
        {
            best = candidate;
        }
    }

    return best;
}
