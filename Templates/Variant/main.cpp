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

template <typename ...Args>
class Variant
{
public:
    Variant() = default;


    template <typename T>
    Variant(const T& other)
    {
        *(T*)data = other;
        index = get_index_by_type<0, T, Args...>();
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

int main() {
    Variant<char, int, double > v;
    v = 10;
    std::cout << v.get<int>() << std::endl;
    v = 1.;
    std::cout << v.get<double>() << std::endl;

    return 0;

}