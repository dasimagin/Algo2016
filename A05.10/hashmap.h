#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <functional>


template<
    class Key,
    class Value,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>
>
class HashMap {
    
    // Data types
    // ==========
    
public:
    
    typedef Key key_type;
    typedef Value value_type;
    typedef Hash hasher_type;
    typedef KeyEqual key_equal_type;
    
    typedef HashMap<key_type, value_type, hasher_type, key_equal_type> self_type;
    typedef const HashMap<key_type, value_type, hasher_type, key_equal_type> const_self_type;
    
    typedef std::pair<const key_type, value_type> pair_type;
    typedef const std::pair<const key_type, value_type> const_pair_type;
    
    
    // Data storage
    // ============
    
private:
    
    static const constexpr size_t _init_buckets = 5;
    
    // bucket storege
    std::vector<std::list<pair_type>> _buckets;
    
    // number of stored elements
    size_t _size;
    
    // increase number of buckets if the load factor is more than this value
    float _max_load_factor = 0.7;
    
    // current hasher instance/function
    hasher_type _hasher;
    
    // current comparator instance/function
    key_equal_type _key_equal;
    
    
    // Constructors
    // ============
    
public:

    HashMap(hasher_type current_hasher = hasher_type(),
            key_equal_type current_key_equal = key_equal_type())
    : _size(0)
    , _hasher(current_hasher)
    , _key_equal(current_key_equal)
    , _buckets(_init_buckets)
    {}
    
    template <class It>
    HashMap(It begin, It end,
            hasher_type current_hasher = hasher_type(),
            key_equal_type current_key_equal = key_equal_type())
    : HashMap(current_hasher, current_key_equal)
    {
        insert(begin, end);
    }
    
    HashMap(std::initializer_list<pair_type> list,
            hasher_type current_hasher = hasher_type(),
            key_equal_type current_key_equal = key_equal_type())
    : HashMap(current_hasher, current_key_equal)
    {
        insert(list.begin(), list.end());
    }
    
    HashMap(const HashMap& other)
    : _size(other._size)
    , _hasher(other._hasher)
    , _key_equal(other._key_equal)
    , _buckets(other._buckets)
    {}
    
    
    // Bucket interface
    // ================
    
public:

    // this iterator can be used to iterate through a single bucket but not across buckets
    typedef typename std::list<pair_type>::iterator local_iterator;
    typedef typename std::list<pair_type>::const_iterator const_local_iterator;
    
    /* Returns an iterator to the beginning of the specified bucket. */
    local_iterator begin(size_t idx) { return _buckets[idx].begin(); }
    const_local_iterator begin(size_t idx) const { return _buckets[idx].cbegin(); }
    const_local_iterator cbegin(size_t idx) const { return _buckets[idx].cbegin(); }
    
    /* Returns an iterator to the end of the specified bucket. */
    local_iterator end(size_t idx) { return _buckets[idx].end(); }
    const_local_iterator end(size_t idx) const { return _buckets[idx].cend(); }
    const_local_iterator cend(size_t idx) const { return _buckets[idx].cend(); }
    
    /* Returns the number of buckets. */
    size_t bucket_count() const { return _buckets.size(); }

    /* Returns the number of elements in specific bucket. */
    size_t bucket_size(size_t idx) const { return _buckets[idx].size(); }
    
    /* Returns the bucket index for specific key */
    size_t bucket(const key_type &key) const { return _hasher(key) % bucket_count(); }
    
    
    // Iterators
    // =========
    
private:

    /* Base iterator template */
    template<class HashMapType, class PairType>
    class _iterator : std::iterator<std::forward_iterator_tag, PairType> {
        
        friend class HashMap<key_type, value_type, hasher_type, key_equal_type>;
        
        static_assert(std::is_same<typename HashMapType::key_type, key_type>::value, "");
        static_assert(std::is_same<typename HashMapType::value_type, value_type>::value, "");
        static_assert(std::is_same<typename HashMapType::hasher_type, hasher_type>::value, "");
        static_assert(std::is_same<typename HashMapType::key_equal_type, key_equal_type>::value, "");
        static_assert(std::is_same<PairType, typename HashMapType::pair_type>::value ||
                      std::is_same<PairType, typename HashMapType::const_pair_type>::value, "");
        
        typedef typename std::conditional<
            std::is_const<HashMapType>::value,
            typename HashMapType::const_local_iterator,
            typename HashMapType::local_iterator
        >::type local_iterator;
        
        HashMapType *_current_map;
        size_t _bucket_index;
        local_iterator _bucket_pos, _end_bucket_pos;
        
    public:

        bool is_end() const {
            return !_current_map || _bucket_index == _current_map->bucket_count();
        }
        
    public:

        _iterator() : _iterator(nullptr) { }
        
        _iterator(const _iterator &mit)
        : _iterator(mit._current_map, mit._bucket_index,
                    mit._bucket_pos, mit._end_bucket_pos)
        {}
        
        _iterator &operator=(_iterator rhs) {
            swap(rhs);
            return *this;
        }
        
        void swap(_iterator &rhs) {
            std::swap(_current_map, rhs._current_map);
            std::swap(_bucket_index, rhs._bucket_index);
            std::swap(_bucket_pos, rhs._bucket_pos);
            std::swap(_end_bucket_pos, rhs._end_bucket_pos);
        }
        
    private:

        /* Build an end iterator */
        explicit _iterator(HashMapType *current_map)
        : _iterator(current_map, current_map ? current_map->bucket_count() : 0)
        {}
        
        /* Build an iterator that is pointing to the first element of bucket. */
        _iterator(HashMapType *current_map, size_t bucket_index)
        : _iterator(current_map, bucket_index,
                    current_map && bucket_index < current_map->bucket_count() ?
                    current_map->begin(bucket_index) : local_iterator(),
                    current_map && bucket_index < current_map->bucket_count() ?
                    current_map->end(bucket_index) : local_iterator())
        {}
        
        /* Build an iterator that pointing to specified element of bucket. */
        _iterator(HashMapType *current_map, size_t bucket_index,
                  local_iterator bucket_pos, local_iterator end_bucket_pos)
        : _current_map(current_map)
        , _bucket_index(bucket_index)
        , _bucket_pos(bucket_pos)
        , _end_bucket_pos(end_bucket_pos)
        {
            if (!is_end() && _bucket_pos == _end_bucket_pos) {
                move();
            }
        }
        
    public:

        /* Comparsions.
         * End iterators considered equal for all hashmaps. */
        friend bool operator==(const _iterator &lhs, const _iterator &rhs) {
            if (lhs.is_end() || rhs.is_end()) { return lhs.is_end() && rhs.is_end(); }
            if (lhs._current_map != rhs._current_map) { return false; }
            return lhs._bucket_index == rhs._bucket_index && lhs._bucket_pos == rhs._bucket_pos;
        }
        friend bool operator!=(const _iterator &lhs, const _iterator &rhs) {
            return !(lhs == rhs);
        }
        
    private:

        /* Traversal: move to the next element.
         * Skip empty buckets. */
        void move() {
            if (is_end()) { return; }
            if (_bucket_pos != _end_bucket_pos) { ++_bucket_pos; }
            while (_bucket_pos == _end_bucket_pos) {
                ++_bucket_index;
                if (!is_end()) {
                    _bucket_pos = _current_map->begin(_bucket_index);
                    _end_bucket_pos = _current_map->end(_bucket_index);
                } else {
                    return;
                }
            }
        }
        
    public:

        /* Increments */
        _iterator &operator++() { move(); return *this; }
        _iterator operator++(int) { _iterator tmp(*this); operator++(); return tmp; }
        
    public:

        /* Dereferencing */
        PairType &operator*() { return *_bucket_pos; }
        PairType *operator->() { return &(*_bucket_pos); }
        
    public:

        /* Conversions */
        operator _iterator<const HashMapType, const PairType>() { return _iterator(*this); }
    };
    
public:

    typedef _iterator<self_type, pair_type> iterator;
    typedef _iterator<const_self_type, const_pair_type> const_iterator;
    
    /* Returns an iterator to the beginning. */
    iterator begin() { return iterator(this, 0); };
    const_iterator begin() const { return const_iterator(this, 0); };
    const_iterator cbegin() const { return const_iterator(this, 0); };
    
    /* Returns an iterator to the end. */
    iterator end() { return iterator(this); };
    const_iterator end() const { return const_iterator(this); };
    const_iterator cend() const { return const_iterator(this); };
    
    
    // Capacity
    // ========
    
public:
    /* Returns the number of stored elements. */
    size_t size() const { return _size; }
    
    /* Returns the maximum possible number of elements.
     * Exceeding this number will cause rehash. */
    size_t max_size() const {
        return static_cast<size_t>(_max_load_factor * static_cast<float>(bucket_count()));
    }
    
    /* Returns true if no elements stored in hashmap. */
    bool empty() const { return size() == 0; }
    
    
    // Modifiers
    // =========
    
public:
    /* Removes all elements from the container. */
    void clear() {
        _buckets.clear();
        _buckets.resize(_init_buckets);
        _size = 0;
    }
    
    /* Inserts element(s) into the container,
     * if the container doesn't already contain an element with an equivalent key.
     * If rehashed, invalidates all iterators (end iterators included). */
    std::pair<iterator, bool> insert(const pair_type &value) {
        iterator it = find(value.first);
        if (it == end()) {
            size_t idx = bucket(value.first);
            _buckets[idx].push_front(value);
            ++_size;
            if (load_factor() > max_load_factor()) {
                rehash(2 * bucket_count());
                return {find(value.first), true};
            } else {
                return {iterator(this, idx), true};
            }
        } else {
            return {it, false};
        }
    }
    template <class It> void insert(It begin, It end) {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }
    void insert(std::initializer_list<pair_type> ilist) {
        insert(ilist.begin(), ilist.end());
    }
    
    /* Removes specified elements from the container.
     * References and iterators to the erased elements are invalidated.
     * Other iterators and references are not invalidated. */
    size_t erase(const key_type &key) {
        iterator it = find(key);
        if (it == end()) {
            return 0;
        } else {
            erase(it);
            return 1;
        }
    }
    iterator erase(iterator it) {
        if (it == end()) {
            return end();
        } else {
            iterator end_it = it;
            return erase(it, ++end_it);
        }
    }
    iterator erase(iterator first, iterator last) {
        for (; first != last;) {
            local_iterator erased_element = first._bucket_pos;
            size_t bucket_index = first._bucket_index;
            ++first;
            _buckets[bucket_index].erase(erased_element);
            --_size;
        }
        return last;
    }
    
    /* Exchanges the contents of the container with those of other.
     * Does not invoke any move, copy, or swap operations on individual elements.
     * Invalidates all iterators. */
    void swap(HashMap &rhs) {
        std::swap(_buckets, rhs._buckets);
        std::swap(_size, rhs._size);
        std::swap(_max_load_factor, rhs._max_load_factor);
        std::swap(_hasher, rhs._hasher);
        std::swap(_key_equal, rhs._key_equal);
    }
    
    /* Assign */
    HashMap& operator=(HashMap rhs) {
        swap(rhs);
        return *this;
    }
    HashMap& operator=(std::initializer_list<pair_type> ilist) {
        return *this = HashMap(ilist);
    }
    
    
    // Lookup
    // ======
    
public:
    /* Returns a reference to the mapped value of the element with key equivalent to key.
     * If no such element exists, an exception of type std::out_of_range is thrown. */
    value_type &at(const key_type &key) {
        iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range("");
        } else {
            return it->second;
        }
    }
    const value_type &at(const key_type &key) const {
        const_iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range("");
        } else {
            return it->second;
        }
    }
    
    /* Returns a reference to the mapped value of the element with key equivalent to key.
     * Inserts a new value if the element with specified key not found. */
    value_type &operator[](const key_type &key) {
        iterator it = find(key);
        if (it == end()) {
            return insert({key, value_type()}).first->second;
        } else {
            return it->second;
        }
    }
    
    /* Returns the number of elements with key key. */
    size_t count(const key_type &key) const {
        return static_cast<size_t>(find(key) != cend());
    }
    
    /* Finds an element with key equivalent to key. */
    iterator find(const key_type &key) {
        size_t idx = bucket(key);
        for (auto v = begin(idx); v != end(idx); ++v) {
            if (_key_equal(v->first, key)) {
                return iterator(this, idx, v, end(idx));
            }
        }
        return iterator(this);
    }
    const_iterator find(const key_type &key) const {
        size_t idx = bucket(key);
        for (auto v = cbegin(idx); v != cend(idx); ++v) {
            if (_key_equal(v->first, key)) {
                return const_iterator(this, idx, v, cend(idx));
            }
        }
        return const_iterator(this);
    }
    
    /* Returns a range containing all elements with key key in the container.
     * The range is defined by two iterators, the first pointing to the first
     * element of the wanted range and the second pointing past the last element of the range. */
    std::pair<iterator, iterator> equal_range(const key_type &key) {
        iterator it = find(key);
        if (it == end()) {
            return {end(), end()};
        } else {
            iterator end_it = it;
            return {it, ++end_it};
        }
    }
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
        const_iterator it = find(key);
        if (it == cend()) {
            return {cend(), cend()};
        } else {
            return {it, std::next(it, 1)};
        }
    }
    
    
    // Hash policy
    // ===========
    
public:
    /* Returns the average number of elements per bucket, that is,
     * size() divided by bucket_count() */
    float load_factor() const {
        return static_cast<float>(size()) / static_cast<float>(bucket_count());
    }
    
    /* Manages the maximum load factor.
     * The container automatically increases the number of buckets
     * if the load factor exceeds this threshold. */
    float max_load_factor() const {
        return _max_load_factor;
    }
    
    void max_load_factor(float ml) {
        _max_load_factor = ml;
        if (load_factor() > max_load_factor()) {
            rehash(2 * bucket_count());
        }
    }
    
    /* Sets the number of buckets to count and rehashes the container,
     * i.e. puts the elements into appropriate buckets considering that total number
     * of buckets has changed. If the new number of buckets makes load factor more
     * than maximum load factor `(count < size() / max_load_factor())`,
     * then the new number of buckets is at least `size() / max_load_factor()`.
     * Invalidates all iterators. */
    void rehash(size_t count) {
        if (count < static_cast<float>(size()) / max_load_factor()) {
            count = static_cast<size_t>(ceilf(static_cast<float>(size()) / max_load_factor()));
        }
        if (count < _init_buckets) {
            count = _init_buckets;
        }
        if (count == bucket_count()) {
            return;
        }
        
        std::vector<std::list<pair_type>> _old_buckets(count);
        std::swap(_buckets, _old_buckets);
        _size = 0;
        
        for (auto &bucket : _old_buckets) {
            for (auto &item : bucket) {
                insert(item);
            }
        }
    }
    
    
    // Observers
    // =========
    
public:
    /* Returns the function that hashes the keys. */
    hasher_type hash_function() const { return _hasher; }
    
    /* Returns the function that compares keys for equality. */
    key_equal_type key_eq() const { return _key_equal; }
    
    
    // Non-members
    // ===========
    
    /* Compares the contents of two unordered containers.
     * The contents of two unordered containers lhs and rhs are equal
     * if the following conditions hold:
     * -- `lhs.size() == rhs.size()`;
     * -- all key-value pairs from `lhs` have corresponding key-value pairs from `rhs`. */
    friend bool operator==(const HashMap& lhs, const HashMap& rhs) {
        if (lhs.size() != rhs.size()) { return false; }
        for (auto &i: lhs) {
            const_iterator rhs_it = rhs.find(i.first);
            if (rhs_it == rhs.cend() || *rhs_it != i) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const HashMap& lhs, const HashMap& rhs) {
        return !(lhs == rhs);
    }
};

namespace std {
    /* Exchanges the contents of the container with those of other.
     * Does not invoke any move, copy, or swap operations on individual elements.
     * Invalidates all iterators. */
    template<class Key, class Value, class Hash, class KeyEqual>
    void swap(HashMap<Key, Value, Hash, KeyEqual> &lhs, HashMap<Key, Value, Hash, KeyEqual> &rhs) {
        lhs.swap(rhs);
    }
}

#endif // __HASHMAP_H__
