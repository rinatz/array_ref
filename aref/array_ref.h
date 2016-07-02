/*
The MIT License (MIT)

Copyright (c) 2016 Kenichiro IDA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef AREF_ARRAY_REF_H_
#define AREF_ARRAY_REF_H_

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace aref {

template <typename T>
class ArrayRef {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    ArrayRef() {}

    ArrayRef(T* data, size_type i, size_type j)
            : data_(data + i), size_(j - i) {
        assert(size_ >= 0);
    }

    template <int N>
    ArrayRef(std::array<T, N>& v, size_type i, size_type j)
            : data_(v.data() + i), size_(j - i) {
        assert(size_ >= 0);
        assert(size_ <= v.size());
    }

    ArrayRef(std::vector<T>& v, size_type i, size_type j)
            : data_(v.data() + i), size_(j - i) {
        assert(size_ >= 0);
        assert(size_ <= v.size());
    }

    ~ArrayRef() {
        data_ = nullptr;
        size_ = 0;
    }

    reference at(size_type n) {
        if (n >= size_) {
            throw std::out_of_range("array_ref subscript out of range");
        }

        return data_[n];
    }

    const_reference at(size_type n) const {
        if (n >= size_) {
            throw std::out_of_range("array_ref subscript out of range");
        }

        return data_[n];
    }

    ArrayRef operator()(size_type i, size_type j) const {
        return ArrayRef(data_, i, j);
    }

    reference operator[](size_type n) {
        assert(n < size_);
        return data_[n];
    }

    const_reference operator[](size_type n) const {
        assert(n < size_);
        return data_[n];
    }

    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    pointer data() { return data_; }
    const_pointer data() const { return data_; }

    iterator begin() { return data_; }
    const_iterator begin() const { return data_; }

    iterator end() { return data_ + size_; }
    const_iterator end() const { return data_ + size_; }

    const_iterator cbegin() const { return data_; }
    const_iterator cend() const { return data_ + size_; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return reverse_iterator(end()); }

    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return reverse_iterator(end()); }
    const_reverse_iterator crend() const { return reverse_iterator(begin()); }

    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }

    void swap(ArrayRef& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

private:
    pointer data_ = nullptr;
    size_type size_ = 0;
};

struct slice {
    slice(size_t i, size_t j) : i(i), j(j) {}

    size_t i;
    size_t j;
};

template <typename T>
inline ArrayRef<T> operator|(T* data, const slice& s) {
    return ArrayRef<T>(data, s.i, s.j);
}

template <typename T, int N>
inline ArrayRef<T> operator|(std::array<T, N>& v, const slice& s) {
    return ArrayRef<T>(v, s.i, s.j);
}

template <typename T>
inline ArrayRef<T> operator|(std::vector<T>& v, const slice& s) {
    return ArrayRef<T>(v, s.i, s.j);
}

template <typename T>
inline bool operator==(const ArrayRef<T>& x, const ArrayRef<T>& y) {
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template <typename T>
inline bool operator!=(const ArrayRef<T>& x, const ArrayRef<T>& y) {
    return !(x == y);
}

template <typename T>
inline bool operator<(const ArrayRef<T>& x, const ArrayRef<T>& y) {
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T>
inline bool operator>(const ArrayRef<T>& x, const ArrayRef<T>& y) {
    return y < x;
}

template <typename T>
inline bool operator<=(const ArrayRef<T>& x, const ArrayRef<T>& y) {
    return !(x > y);
}

template <typename T>
inline bool operator>=(const ArrayRef<T>& x, const ArrayRef<T>& y) {
    return !(x < y);
}

template <typename T>
inline void swap(ArrayRef<T>& x, ArrayRef<T>& y) {
    x.swap(y);
}

}  // namespace aref

#endif  // ARRAY_REF_H_
