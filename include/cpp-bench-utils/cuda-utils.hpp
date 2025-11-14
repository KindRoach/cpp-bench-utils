#pragma once

#include <vector>
#include <thrust/device_vector.h>

#include "vector.hpp"

namespace cbu {

    constexpr size_t WARP_SIZE = 32;

    void cuda_check(cudaError_t err) {
        if (err != cudaSuccess) {
            auto message = cudaGetErrorString(err);
            std::cerr << message << std::endl;
            throw std::runtime_error(message);
        }
    }

    template<typename T>
    void cuda_acc_check(std::vector<T> &gt, thrust::device_vector<T> d_vec) {
        size_t size = gt.size();
        std::vector<T> actual(size);
        thrust::copy(d_vec.begin(), d_vec.end(), actual.begin());
        acc_check(gt, actual);
    }

    template <typename T>
    void cuda_acc_check(thrust::device_vector<T>& d_expected, thrust::device_vector<T>& d_actual)
    {
        size_t size = d_expected.size();
        std::vector<T> h_expected(size);
        std::vector<T> h_actual(size);
        thrust::copy(d_expected.begin(), d_expected.end(), h_expected.begin());
        thrust::copy(d_actual.begin(), d_actual.end(), h_actual.begin());
        acc_check(h_expected, h_actual);
    }
}
