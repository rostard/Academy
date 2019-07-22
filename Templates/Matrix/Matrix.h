//
// Created by rostard on 19.07.19.
//

#ifndef TEMPLATEDMATRIX_MATRIX_H
#define TEMPLATEDMATRIX_MATRIX_H


#include <iostream>
#include <algorithm>
#include <array>
#include <utility>
#include <numeric>

template<typename T, size_t M, size_t N>
class Matrix;


template<typename Processor, typename T, size_t M, size_t N>
auto UnaryOperation(const Matrix<T, M, N>& mat, const Processor& processor)
{
    using result_type = decltype(processor(std::declval<T>()));
    Matrix<result_type, M, N> ret;
    std::transform(mat.getData().cbegin(), mat.getData().cend(), ret.getData().begin(), processor);
    return ret;
}

template<typename Processor, typename T, typename U, size_t M, size_t N>
auto BinaryOperation(const Matrix<T, M, N>& mat1, const Matrix<U, M, N>& mat2, const Processor& processor)
{
    using result_type = decltype(processor(std::declval<T>(), std::declval<U>()));
    Matrix<result_type, M, N> ret;
    std::transform(mat1.getData().cbegin(), mat1.getData().cend(), mat2.getData().cbegin(), ret.getData().begin(), processor);
    return ret;
}

template<typename tProcessor,typename rProcessor, typename T, typename U, typename V, size_t M>
auto Scalar(const Matrix<T, 1, M>& row, const Matrix<U, M, 1>& col, const V& init, const tProcessor& processor1, const rProcessor& processor2)
{

    using result_type = decltype(processor1(std::declval<T>(), std::declval<U>()));
    Matrix<result_type, M, 1> ret;
    std::transform(col.getData().cbegin(), col.getData().cend(), row.getData().cbegin(), ret.getData().begin(), processor1);
    return std::accumulate(ret.getData().cbegin(), ret.getData().cend(), init, processor2);
}



template<typename T, size_t M, size_t N>
class Matrix
{
public:
    Matrix() = default;

    explicit Matrix(const T& default_val)
    {
        for (auto& it : m_data) {
            it = default_val;
        }
    }


    //maybe should explicit
    template<typename U, size_t K, size_t L, typename = std::enable_if_t <K * L == N * M>>
    Matrix(const Matrix<U, K, L>& mat)
    {
        for(int i = 0; i < N*M; ++i)
        {
            m_data[i] = mat.getData()[i];
        }
    }

    template<typename U>
    auto operator+(const Matrix<U, M, N>& rhs)
    {

        using result_type = decltype(std::declval<T>() + std::declval<U>());
        auto ret = BinaryOperation(*this, rhs, [](const T& it1, const T& it2) {return it1 + it2; });
        return ret;
    }

    template<typename U>
    auto operator-(const Matrix<U, M, N>& rhs)
    {
        using result_type = decltype(std::declval<T>() - std::declval<U>());
        auto ret = BinaryOperation(*this, rhs, [](const T& it1, const T& it2) {return it1 - it2; });
        return ret;
    }

    template<typename U, size_t K>
    auto operator*(const Matrix<U, N, K>& rhs)
    {
        using result_type = decltype(std::declval<T>() * std::declval<U>());
        Matrix<result_type , M, K> ret;

        for(int i = 0; i < M; ++i)
        {
            for(int j = 0; j < N; ++j)
            {
                ret.getData()[i* K + j] = Scalar(this->getRow(i), rhs.getColumn(j), 0, std::multiplies<T>(), std::plus<U>());
            }
        }
//        std::transform(m_data.begin(), m_data.end(), rhs.getData().begin(), ret.m_data.begin(), [](const T& it1, const T& it2) {return it1 - it2; });

        return ret;
    }


    void print()const
    {
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                std::cout << m_data[i * N + j] << ' ';
            }
            std::cout << '\n';
        }
    }

    auto getColumn(size_t index) const
    {
        Matrix<T, M, 1> ret;
        for(int i = 0; i < M; ++i)
        {
            ret.getData()[i] = m_data[index + i * N];
        }
        return ret;
    }

    auto getRow(size_t index) const
    {
        Matrix<T, 1, N> ret;
        for(int i = 0; i < N; ++i)
        {
            ret.getData()[i] = m_data[index * N + i];
        }
        return ret;
    }

    const std::array<T, M*N>& getData() const
    {
        return m_data;
    }

    std::array<T, M*N>& getData()
    {
        return m_data;
    }

private:
    std::array<T, M*N> m_data;
};


template<typename T, typename U, size_t N, size_t M>
auto operator+(const T& num, const Matrix<U, M, N>& mat)
{

    auto ret = UnaryOperation(mat, [num](const U& it) {return num + it ; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator+(const Matrix<T, M, N>& mat, const U& num)
{
    auto ret = UnaryOperation(num, [mat](const T& it) {return it + mat; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator*(const T& num, const Matrix<U, M, N>& mat)
{
    auto ret = UnaryOperation(mat, [num](const U& it) {return num * it; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator*(const Matrix<T, M, N>& mat, const U& num)
{
    auto ret = UnaryOperation(mat, [num](const T& it) {return it * num; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator-(const T& num, const Matrix<U, M, N>& mat)
{
    auto ret = UnaryOperation(mat, [num](const U& it) {return num - it; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator-(const Matrix<T, M, N>& mat, const U& num)
{
    auto ret = UnaryOperation(mat, [num](const T& it) {return it - num; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator/(const T& num, const Matrix<U, M, N>& mat)
{
    auto ret = UnaryOperation(mat, [num](const U& it) {return num / it; });
    return ret;
}

template<typename T, typename U, size_t N, size_t M>
auto operator/(const Matrix<T, M, N>& mat, const U& num)
{
    auto ret = UnaryOperation(mat, [num](const T& it) {return it / num; });
    return ret;
}


#endif //TEMPLATEDMATRIX_MATRIX_H
