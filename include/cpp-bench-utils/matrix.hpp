#pragma once

namespace cbu
{
    enum class matrix_layout
    {
        row_major = 0,
        col_major = 1
    };

    template <typename T>
#ifdef __CUDACC__
    __host__ __device__
#endif
    T& mat(T* data, size_t ld, size_t idx_major, size_t idx_minor) noexcept
    {
        return data[idx_major * ld + idx_minor];
    }

    template <typename T>
#ifdef __CUDACC__
    __host__ __device__
#endif
    T* mat_ptr(T* data, size_t ld, size_t idx_major, size_t idx_minor) noexcept
    {
        return &data[idx_major * ld + idx_minor];
    }


    template <typename T>
    void print_matrix(T* data, size_t m, size_t n)
    {
        std::cout << "Matrix " << m << " x " << n << ":\n";
        for (size_t i = 0; i < m; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                std::cout << std::setw(5) << mat<T>(data, m, i, j) << " ";
            }
        }
    }

    template <typename T, matrix_layout b_layout>
    void matrix_multiply_ref(
        std::vector<T>& a,
        std::vector<T>& b,
        std::vector<T>& c,
        size_t m, size_t n, size_t k)
        {
            using namespace cbu;
            size_t lda = k, ldb = b_layout == matrix_layout::row_major ? n : k, ldc = n;
            for (size_t i = 0; i < m; i++)
            {
                for (size_t j = 0; j < n; j++)
                {
                    T sum = 0;
                    for (size_t p = 0; p < k; p++)
                    {
                        if constexpr (b_layout == matrix_layout::row_major)
                        {
                            sum += mat(a.data(), lda, i, p) * mat(b.data(), ldb, p, j);
                        }
                        else
                        {
                            sum += mat(a.data(), lda, i, p) * mat(b.data(), ldb, j, p);
                        }
                    }
                    mat(c.data(), ldc, i, j) = sum;
                }
            }
        }
}
