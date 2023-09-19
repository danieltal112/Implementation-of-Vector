
#ifndef VL_VECTOR_H
#define VL_VECTOR_H

#include <iostream>
#include <algorithm>
#include <exception>

#define INIT_STATIC_CAPACITY 16
#define CAP_TWO 2
#define CAP_THREE 3
#define ERROR_INDEX "Error: an index should be larger than zero\
and smaller than size."

/**
 *  class of generic vector
 */
template<class T, size_t StaticCapacity = INIT_STATIC_CAPACITY>
class vl_vector
{
private:
    T _static_cap[StaticCapacity];
    T *_dynamic_cap = nullptr;
    size_t _size;
    size_t _capacity;

    size_t cap_c(size_t size, size_t num)
    {
        if ((size + num) <= StaticCapacity)
        { return StaticCapacity; }
        return ((CAP_THREE * (size + num)) / CAP_TWO);
    }

public:
///  constructors
    //default cons't
    vl_vector() : _size(0), _capacity(StaticCapacity)
    {};

    //copy cons't
    vl_vector(const vl_vector<T, StaticCapacity> &other) :
            _size(other._size), _capacity(other._capacity)
    { (*this) = other; }

    //sequence based cons't
    template<class ForwardIterator>
    vl_vector(ForwardIterator first, ForwardIterator last);

    //singale_v_init_cons't
    vl_vector(size_t count, T argument);

    //destructor
    ~vl_vector()
    {
        if (_dynamic_cap != nullptr)
        {
            delete[] _dynamic_cap;
            _dynamic_cap = nullptr;
        }
    }

///  iterator
    typedef T *iterator;
    typedef const T *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    iterator begin()
    { return data(); }

    iterator end()
    { return data() + _size; }

    const_iterator begin() const
    { return data(); }

    const_iterator end() const
    { return data() + _size; }

    const_iterator cbegin() const
    { return data(); }

    const_iterator cend() const
    { return data() + _size; }

    reverse_iterator rbegin()
    { return (reverse_iterator) end(); }

    reverse_iterator rend()
    { return (reverse_iterator) begin(); }

    const_reverse_iterator rbegin() const
    { return (const_reverse_iterator) end(); }

    const_reverse_iterator rend() const
    { return (const_reverse_iterator) begin(); }

    const_reverse_iterator crbegin() const
    { return (const_reverse_iterator) cend(); }

    const_reverse_iterator crend() const
    { return (const_reverse_iterator) cbegin(); }


///  functions
    size_t size() const
    { return _size; }

    size_t capacity() const
    { return _capacity; }

    //check if vector is empty
    bool empty() const
    { return (_size == 0); }

    T at(size_t index) const;

    T &at(size_t index);

    void push_back(T argument);

    /**
     * A function that receives an iterator that points to an element in
     * vec and the beginning and end of an array to be inserted into vec
     * @tparam ForwardIterator
     * @param position
     * @param first
     * @param last
     * @return iterator that pointer to the first new argument in vec
     */
    template<class ForwardIterator>
    iterator insert(const_iterator position, ForwardIterator first,
                    ForwardIterator last)
    {
        size_t len_1 = std::distance(first, last);
        size_t len_2 = std::distance(cbegin(), position);
        if (_size + len_1 > _capacity)
        {
            _capacity = cap_c(_size, len_1);
            T *temp = new T[_capacity];
            std::copy(begin(), end(), temp);
            if (_size > StaticCapacity)
            {
                delete[] _dynamic_cap;
            }
            _dynamic_cap = temp;
        }
        std::move_backward(begin() + len_2, end(), end() + len_1);
        std::copy(first, last, begin() + len_2);
        _size += len_1;
        return begin() + len_2;
    }

    /**
     * like up "insert" insert but one argument
     * @param position
     * @param element
     * @return iterator that pointer to the new argument in vec
     */
    iterator insert(const_iterator position, T element)
    {
        vl_vector<T, StaticCapacity> new_vec;
        new_vec.push_back(element);
        return (insert(position, new_vec.begin(), new_vec.end()));
    }

    /**
     * A function that receives an instance of members
     * from the array of vec and deletes them
     * @param first
     * @param last
     * @return Iterator for limb to the right of "last"
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        size_t len_1 = std::distance(first, last);
        size_t len_2 = std::distance(cbegin(), first);
        if (_size - len_1 <= StaticCapacity)
        {
            std::copy(cbegin(), first, _static_cap);
            std::copy(last, cend(), _static_cap + len_2);
            if (_size > StaticCapacity)
            {
                delete[] _dynamic_cap;
                _dynamic_cap = nullptr;
                _capacity = cap_c(_size-len_1,0);
            }
        } else
        {
            std::copy(cbegin(), first, _dynamic_cap);
            std::copy(last, cend(), _dynamic_cap + len_2);
        }
        _size -= len_1;
        return begin() + len_2;
    }

    /**
     * The function receives an iterator that points to an organ
     * in the array and deletes it
     * @param arg
     * @return Iterator for limb to the right of "arg"
     */
    iterator erase(const_iterator arg)
    {
        return erase(arg, arg + 1);
    }

    void pop_back();

    void clear();

    T *data() const;

    //check if argument in vector
    bool contains(T argument) const
    {
        return (std::find(begin(), end(), argument) != end());
    }

    /// operators
    vl_vector &operator=(const vl_vector &vec);

    T &operator[](size_t i)
    {
        return data()[i];
    }

    T operator[](size_t i) const
    {
        return data()[i];
    }

    bool operator==(const vl_vector &vec) const;

    bool operator!=(const vl_vector &vec) const
    {
        return (!((*this) == vec));
    }
};
///  End of class

/**
 * constructor which receives an array of iterators and inserts them into vec
 * @tparam ForwardIterator
 * @param first
 * @param last
 */
template<class T, size_t StaticCapacity>
template<class ForwardIterator>
vl_vector<T, StaticCapacity>::vl_vector(ForwardIterator first,
                                        ForwardIterator last)
{
    _size = std::distance(first, last);
    if (_size > StaticCapacity)
    {
        _capacity = cap_c(0, _size);
        _dynamic_cap = new T[_capacity];
        std::copy(first, last, _dynamic_cap);

    } else
    {
        _capacity = cap_c(0,_size);
        if (_size != 0)
        {
            std::copy(first, last, _static_cap);
        }
    }
}

/**
 *A constructor that receives an organ and the number of times we
 * want to insert it into vec
 * @param count
 * @param argument
 */
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity>::vl_vector(size_t count, T argument)
{
    if (count == 0)
    {
        _size = 0;
        _capacity = cap_c(_size,count);
        return;
    }
    _size = count;
    if (count <= StaticCapacity)
    {
        _capacity = cap_c(0,count);
        for (size_t i = 0; i < count; i++)
        {
            _static_cap[i] = argument;
        }
    } else
    {
        _capacity = cap_c(0, count);
        _dynamic_cap = new T[_capacity];
        for (size_t i = 0; i < _size; i++)
        {
            _dynamic_cap[i] = argument;
        }
    }
}

/**
 * @tparam T
 * @tparam StaticCapacity
 * @param index
 * @return returns the value of the member in index i
 */
template<class T, size_t StaticCapacity>
T vl_vector<T, StaticCapacity>::at(size_t index) const
{
    if (index >= _size)
    {
        throw std::out_of_range(ERROR_INDEX);
    }
    return (*this)[index];
}

/**
 * @tparam T
 * @tparam StaticCapacity
 * @param index
 * @return returns the value of the member in index i
 */
template<class T, size_t StaticCapacity>
T &vl_vector<T, StaticCapacity>::at(size_t index)
{
    if (index >= _size)
    {
        throw std::out_of_range(ERROR_INDEX);
    }
    return (*this)[index];
}

/**
 *  push argument to vec
 * @tparam T
 * @tparam StaticCapacity
 * @param argument
 */
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::push_back(T argument)
{
    if (_size + 1 <= StaticCapacity)
    {
        _static_cap[_size] = argument;
        _size++;
    } else if (_size == StaticCapacity)
    {
        _capacity = cap_c(_size, 1);
        T *temp = new T[_capacity];
        std::copy(begin(), end(), temp);
        temp[_size] = argument;
        _size++;
        _dynamic_cap = temp;
    } else if (_size + 1 <= _capacity)
    {
        _dynamic_cap[_size] = argument;
        _size++;

    } else
    {
        _capacity = cap_c(_size, 1);
        T *temp = new T[_capacity];
        std::copy(begin(), end(), temp);
        temp[_size] = argument;
        delete[] _dynamic_cap;
        _dynamic_cap = temp;
        _size++;
    }
}

/**
 * Removes the last organ found in vec
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::pop_back()
{
    if (_size == 0)
    { return; }
    else if (_size - 1 == StaticCapacity)
    {
        _capacity = cap_c(_size - 1, 0);
        std::copy(begin(), end() - 1, _static_cap);
        delete[] _dynamic_cap;
        _dynamic_cap = nullptr;
    }
    _size--;
}

/**
 * Returns a pointer to the array that Vector is currently using
 * @tparam T
 * @tparam StaticCapacity
 * @return
 */
template<class T, size_t StaticCapacity>
T *vl_vector<T, StaticCapacity>::data() const
{
    if (_dynamic_cap != nullptr)
    {
        return _dynamic_cap;
    } else
    {
        return (T *) _static_cap;
    }
}

/**
 * Resets the vector
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
void vl_vector<T, StaticCapacity>::clear()
{
    vl_vector<T, StaticCapacity> new_vec;
    *this = new_vec;
}

/**
 * operator Placement (=)
 */
template<class T, size_t StaticCapacity>
vl_vector<T, StaticCapacity> &
vl_vector<T, StaticCapacity>::operator=(const vl_vector &vec)
{
    if (this == &vec)
    {
        return *this;
    }
    if (_size > StaticCapacity)
    {
        delete[] _dynamic_cap;
        _dynamic_cap = nullptr;
    }
    _size = vec._size;
    _capacity = vec._capacity;
    if (vec._size > StaticCapacity)
    {
        _dynamic_cap = new T[_capacity];
        std::copy(vec.begin(), vec.end(), _dynamic_cap);
    } else
    {
        std::copy(vec.begin(), vec.end(), _static_cap);
    }
    return *this;
}

/**
 * operator Comparison (==)
 * return bool true or false in accordance
 */
template<class T, size_t StaticCapacity>
bool vl_vector<T, StaticCapacity>::operator==(const vl_vector &vec) const
{
    if (_size != vec._size)
    { return false; }

    for (size_t i = 0; i < _size; i++)
    {
        if (data()[i] != vec.data()[i])
        { return false; }
    }
    return true;
}

#endif //VL_VECTOR_H
