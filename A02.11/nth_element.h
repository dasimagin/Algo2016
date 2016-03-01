/* Quick-select algorithm implementation */

#ifndef __QSELECT_H__
#define __QSELECT_H__

#include <iostream>
#include <cmath>
#include <random>

namespace qselect {
    // Ahead definitions

#define _default_cmp std::less<typename std::iterator_traits<RandomIt>::value_type>

    // Internal functions
    
    template <typename RandomIt, class Comparator = _default_cmp>
    inline void insort(RandomIt begin, RandomIt end, Comparator cmp = _default_cmp());
    
    template <typename RandomIt, class Comparator = _default_cmp>
    inline RandomIt median_of_medians_select(RandomIt begin, RandomIt end,
                                             Comparator cmp = _default_cmp());
    
    template <typename RandomIt>
    inline RandomIt random_select(RandomIt begin, RandomIt end);
    
    template <typename RandomIt, class Comparator = _default_cmp>
    inline std::pair<RandomIt, RandomIt> partition(RandomIt begin, RandomIt end,
                                                   RandomIt pivot, Comparator cmp = _default_cmp());
    
    template <typename RandomIt, class Comparator = _default_cmp>
    RandomIt qselect(RandomIt begin, RandomIt end, size_t k, size_t fallback_depth,
                     Comparator cmp = _default_cmp());
    
    // Interface
    
    template <typename RandomIt, class Comparator = _default_cmp>
    inline RandomIt nth_element(RandomIt first, RandomIt nth, RandomIt last,
                                Comparator cmp = _default_cmp());

#undef _default_cmp
}

/**
 * In-place insertion sort is used for brute-forcing small arrays. */
template <typename RandomIt, class Comparator>
void qselect::insort(RandomIt begin, RandomIt end, Comparator cmp) {
    for (RandomIt i = begin; i != end; ++i) {
        for (RandomIt j = i; j != begin && cmp(*j, *(j - 1)); --j) {
            std::iter_swap(j, j - 1);
        }
    }
}

/**
 * Select a pivot element using the median of medians algorithm. */
template <typename RandomIt, class Comparator>
RandomIt qselect::median_of_medians_select(RandomIt begin, RandomIt end, Comparator cmp) {
    const size_t blocks = (end - begin) / 5;
    
    // First, medians are extracted from subarrays and moved into the beginning
    // of the array.
    RandomIt medians = begin;
    for (size_t block = 0; block < blocks; ++block) {
        qselect::insort(begin + 5 * block, begin + 5 * block + 5, cmp);
        std::iter_swap(medians++, begin + 5 * block + 2);
    }
    
    // Second, median of medians is found and returned.
    return qselect::qselect(begin, medians, blocks / 2, 0, cmp);
}

/**
 * Select a pivot element using a random function. */
template <typename RandomIt>
RandomIt qselect::random_select(RandomIt begin, RandomIt end) {
    static std::random_device rand_dev;
    static std::mt19937 generator(rand_dev());
    
    if (end == begin) {
        return end;
    } else {
        std::uniform_int_distribution<size_t> distr(0, end - begin - 1);
        return begin + distr(generator);
    }
}

/**
 * A simple partition function rearranges `[begin, end)` and returns
 * pair of iterators `(l, r)` so that all elements in `[begin, l)` are
 * less than `pivot`, all elements in `[l, r)` are equel to `pivot`,
 * all elements in `[r, end)` are greater than `pivot`. */
template <typename RandomIt, class Comparator>
std::pair<RandomIt, RandomIt> qselect::partition(RandomIt begin, RandomIt end,
                                                 RandomIt pivot, Comparator cmp) {
    std::pair<RandomIt, RandomIt> bounds;
    
    RandomIt current = begin;
    
    // First, move all elements that are less than the pivot element
    // to the beginning of the first segment.
    for(RandomIt i = begin; i != end; i++) {
        if (cmp(*i, *pivot)) {
            if (pivot == i) {
                pivot = current;
            } else if (pivot == current) {
                pivot = i;
            }
            std::iter_swap(current, i);
            ++current;
        }
    }
    
    bounds.first = current;
    
    // Second, move all elements that are equel to the pivot element
    // to the beginning of the second segment.
    for(RandomIt i = current; i != end; i++) {
        if (!cmp(*i, *pivot) && !cmp(*pivot, *i)) {
            if (pivot == i) {
                pivot = current;
            } else if (pivot == current) {
                pivot = i;
            }
            std::iter_swap(current, i);
            ++current;
        }
    }
    
    bounds.second = current;
    
    return bounds;
}

/**
 * Calculates the K-th order statistics.
 * This is an internal function which should not be accessed outside the namespace.
 * Use `qselect::nth_element` instead. */
template <typename RandomIt, class Comparator>
RandomIt qselect::qselect(RandomIt begin, RandomIt end, size_t k, size_t fallback_depth,
                          Comparator cmp) {
    if (end - begin < 5) {
        // Small arrays are brute-forced
        qselect::insort(begin, end, cmp);
        return begin + k;
    }
    
    RandomIt pivot;
    
    if (fallback_depth > 0) {
        // If the maximal depth hadn't been reached, use the random selector.
        // It does not guarantee O(n) maximal time yet it is usually faster
        // than the median of medians algorithm.
        pivot = qselect::random_select(begin, end);
    } else {
        // Fall back to the median of medians if we are too deep in the recursion tree.
        pivot = qselect::median_of_medians_select(begin, end, cmp);
    }
    
    std::pair<RandomIt, RandomIt> bounds = qselect::partition(begin, end, pivot, cmp);
    
    if (fallback_depth > 0) {
        fallback_depth -= 1;
    }
    
    size_t section_one = bounds.first - begin;
    size_t section_two = bounds.second - bounds.first;
    
    if (k < section_one) {
        return qselect::qselect(begin, bounds.first, k, fallback_depth, cmp);
    } else if (k < section_one + section_two) {
        return begin + k;
    } else {
        return qselect::qselect(bounds.second, end, k - section_one - section_two,
                                fallback_depth, cmp);
    }
}

/**
 * Calculates the K-th order statistics. */
template <typename RandomIt, class Comparator>
RandomIt qselect::nth_element(RandomIt first, RandomIt nth, RandomIt last, Comparator cmp) {
    return qselect::qselect(first, last, nth - first, log(last - first), cmp);
}

#endif // __QSELECT_H__
