#include <iostream>
#include <type_traits>

class X {
public:
    X(int x = 0):x(x) {};


    int print() const{
        return x;
    }
private:
    int x;
};


template <typename T>
class MyVector {
public:


    MyVector():m_size(0), m_capacity(0), m_arr(nullptr){}

    explicit MyVector(size_t size) : m_capacity(size), m_size(size), m_arr((T*)(new char[size * sizeof(T)])) {}
    MyVector(size_t size, const T& default_value) : MyVector(size)
    {
        for (int i = 0; i < size; ++i)
        {
            new(m_arr + i) T(default_value);
        }
    }

    MyVector(const MyVector& other) : m_size(other.m_size), m_capacity(other.m_size)
    {
        m_arr = (T*)new char[m_size * sizeof(T)];
        for (int i = 0; i < m_size; ++i)
        {
            new(m_arr + i) T(other.m_arr[i]);
        }
    }

    MyVector(MyVector&& other): m_size(other.m_size), m_capacity(other.m_capacity), m_arr(other.m_arr)
    {
        other.m_size = 0;
        other.m_capacity = 0;
        other.m_arr = nullptr;
    }


    //Is there another option?
    template<class U>
    friend class MyVector;

    template<typename U, typename = std::enable_if_t<std::is_constructible<T, U>::value>>
    MyVector(const MyVector<U>& other)
    {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_arr = (T*)new char[m_size * sizeof(T)];
        for (int i = 0; i < m_size; ++i)
        {
            new(m_arr + i) T(other.m_arr[i]);
        }
    }


    MyVector& operator=(const MyVector& other)
    {
        m_size = other.m_size;
        m_capacity = other.m_size;
        m_arr = (X*)new char[m_size * sizeof(X)];

        for (int i = 0; i < m_size; ++i)
        {
            new(m_arr + i) X(other.m_arr[i]);
        }

        return *this;
    }

    MyVector& operator=(MyVector&& other)
    {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_arr = other.m_arr;

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_arr = nullptr;
        return *this;
    }

    ~MyVector()
    {
        delete[] m_arr;
    }

    T& operator[](int index)
    {
        return m_arr[index];
    }

    const T& operator[](int index) const
    {
        return m_arr[index];
    }

    T& at(int index)
    {
        if (index >= m_size || index < 0)
        {
            throw std::out_of_range("Wrong index");
        }
        return m_arr[index];
    }

    const T& at(int index) const
    {
        if (index >= m_size || index < 0)
        {
            throw std::out_of_range("Wrong index");
        }
        return m_arr[index];
    }

    T& front()
    {
        if (empty())
            throw std::length_error("Vector is empty");
        return m_arr[0];
    }

    const T& front() const
    {
        if (empty())
            throw std::length_error("Vector is empty");
        return m_arr[0];
    }

    const T& back() const
    {
        if (empty())
            throw std::length_error("Vector is empty");
        return m_arr[m_size-1];
    }

    size_t size() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_capacity;
    }

    bool empty() const {
        return size() == 0;
    }

    void pop_back()
    {
        m_arr[m_size - 1].~T();
        m_size--;
    }

    void push_back(const T& val)
    {
        if (m_arr == nullptr)
            reserve(10);
        if (m_size >= m_capacity)
            reserve(m_size * 2);
        new(m_arr + m_size) T(val);
        m_size++;
    }

    void push_back(T&& val)
    {
        if (m_arr == nullptr)
            reserve(10);
        if (m_size >= m_capacity)
            reserve(m_size * 2);
        new(m_arr + m_size) T(std::move(val));

        ++m_size;
    }

    template<typename... Args>
    void emplace_back(Args... args)
    {
        if (m_arr == nullptr)
            reserve(10);
        if (m_size >= m_capacity)
            reserve(m_size * 2);
        new(m_arr + m_size) T(args...);
        ++m_size;
    }

    void reserve(size_t n)
    {
        if (n < capacity())
            return;
        m_capacity = n;
        auto next = (T*)(new char[n * sizeof(T)]);
        for (int i = 0; i < size(); i++)
        {
            if(std::is_move_constructible<decltype(m_arr[i])>())
                new(next + i) T(std::move(m_arr[i]));
            else
            {
                new(next + i) T(m_arr[i]);
                m_arr[i].~T();
            }
        }
        delete[] (char*)m_arr;
        m_arr = next;
    }

private:
    size_t m_capacity;
    size_t m_size;

    T* m_arr;
};

#include <vector>

int main() {
    MyVector<X> v;
    v.reserve(200);

    v.push_back(X(1));
    v.push_back(X(2));

    MyVector<X> v2 = v;
    MyVector<X> v3;
    v3 = v;

    MyVector<X> v4;
    v4 = std::move(v);

    for (unsigned int i = 0; i < v3.size(); ++i) {
        std::cout << v3[i].print() << std::endl;
    }
    std::cout << "v3 size is " << v3.size() << std::endl;
    std::cout << "v3 capasity is " << v3.capacity() << std::endl;

    for (unsigned int i = 0; i < v4.size(); ++i) {
        std::cout << v4[i].print() << std::endl;
    }
    std::cout << "v4 size is " << v4.size() << std::endl;
    std::cout << "v4 capasity is " << v4.capacity() << std::endl;


    MyVector<int> vi;
    MyVector<float> vf;
    vf = vi;

    return 0;
}