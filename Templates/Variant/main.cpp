#include <iostream>
#include <variant>
#include <utility>

template <typename ... Is>
struct max_sz;

template <>
struct max_sz<>
{
    static constexpr size_t val = 0;
};

template <typename First, typename... Rest>
struct max_sz<First, Rest...>
{
    static constexpr size_t val = std::max(sizeof(First), max_sz<Rest...>::val);
};



template <int N, typename T, typename U>
int get_index_by_type()
{
    return N * std::is_same<T, U>::value + (1 - std::is_same<T, U>::value) * -1;
}


template <int N, typename T, typename U, typename Next, typename... Args>
int get_index_by_type()
{
    return N * std::is_same<T, U>::value + (1 - std::is_same<T, U>::value) *
                                           get_index_by_type<N + 1, T, Next, Args...>();
}



template <int N, typename T>
void delete_type_by_index(void* data, int index)
{
    ((T*)data)->~T();
    //return N * std::is_same<T, U>::value + (1 - std::is_same<T, U>::value) * -1;

}


template <int N, typename T, typename Next, typename... Args>
void delete_type_by_index(void* data , int index)
{
    if(N == index)
    {
        ((T*)data)->~T();
        return;
    }
    else delete_type_by_index<N+1, Next, Args...>(data , index);
    //return N * std::is_same<T, U>::value + (1 - std::is_same<T, U>::value) *
      //                                     get_index_by_type<N + 1, T, Next, Args...>();
}


template <typename ...Args>
class Variant
{
public:
    Variant() = default;


    template <typename T>
    Variant(const T& other)
    {
        if(index != -1)delete_type_by_index<0, Args...>(&data, index);
        new (&data) T(other);
        index = get_index_by_type<0, T, Args...>();
    }

    template <typename T>
    Variant(T&& other)
    {
        if(index != -1)delete_type_by_index<0, Args...>(&data, index);
        new (&data) T(std::forward<T>(other));
        index = get_index_by_type<0, T, Args...>();
    }

    template <typename T>
    Variant& operator=(const T& other)
    {
        if(index != -1)delete_type_by_index<0, Args...>(&data, index);
        new (&data) T(other);
        index = get_index_by_type<0, T, Args...>();
        return *this;
    }


    template <typename T>
    Variant& operator=(T&& other)
    {
        if(index != -1)delete_type_by_index<0, Args...>(&data, index);
        new (&data) T(std::forward<T>(other));
        index = get_index_by_type<0, T, Args...>();
        return *this;
    }


    ~Variant()
    {
        if(index != -1)delete_type_by_index<0, Args...>(&data, index);
    }

    template <int N>
    std::tuple_element_t<N, std::tuple<Args...>>& get()
    {

        if(index == N)return *(std::tuple_element_t<N, std::tuple<Args...>>*)data;
        throw std::exception();
    }

    template <typename T>
    T& get()
    {
        if(index == get_index_by_type<0, T, Args...>())return *(T*)data;
        throw std::exception();
    }



private:
    int index = -1;
    char data[max_sz<Args...>::val];
};

struct X{
    ~X() {
        std::cout<<"Destruct\n";
    }
    X(X&&) = default;
    X&operator=(X&&) = default;
    int i;
};


int main() {
    {
        Variant<char, int, double, X> v;
        v = 10;
        std::cout << v.get<int>() << std::endl;
        v = 1.;
        std::cout << v.get<double>() << std::endl;
        v = X{3};
        std::cout << v.get<X>().i << std::endl;
    }
    return 0;

}