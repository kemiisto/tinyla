#ifndef TINYLA_MAT_H
#define TINYLA_MAT_H

#include <tinyla/det.hpp>
#include <tinyla/vec.hpp>
#include <initializer_list>
#include <iostream>

namespace tinyla
{
    enum class mat_init {
        uninitialized,
        zero,
        identity,
        diagonal
    };

    template<std::size_t N, typename T>
    requires(N >= 2)
    class mat;

    template<std::size_t N, typename T>
    requires(N >= 2)
    class mat {
    public:
        constexpr explicit mat(mat_init init, vec<N,T> v = vec<N,T>{vec_init::zero})
        {
            switch (init) {
                case mat_init::uninitialized:
                    break;
                case mat_init::zero:
                    set_to_zero();
                    break;
                case mat_init::identity:
                    set_to_identity();
                    break;
                case mat_init::diagonal:
                    set_to_diagonal(v);
                    break;
            }
        }

        constexpr mat(std::initializer_list<T> values)
        {
            assert(values.size() == N*N);
            auto it = values.begin();
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    m[j][i] = *it++;
                }
            }
        }

        constexpr T& operator[](std::size_t row, std::size_t column)
        {
            return m[column][row];
        }

        constexpr T operator[](std::size_t row, std::size_t column) const
        {
            return m[column][row];
        }

        constexpr mat operator*=(const mat& other)
        {
            const auto o = other; // prevent aliasing when &o == this
            *this = *this * o;
            return *this;
        }

        constexpr void set_to_zero()
        {
            for (std::size_t i = 0; i < N; ++i) {
                std::fill(&m[i][0], &m[i][0] + N, T{0});
            }
        }

        constexpr void set_to_identity()
        {
            auto i = tinyla::vec<N,T>{vec_init::uninitialized};
            i.fill(T{1});
            set_to_diagonal(i);
        }

        constexpr void set_to_diagonal(vec<N,T> const& v)
        {
            set_to_zero();
            for (size_t i = 0; i < N; ++i) {
                m[i][i] = v[i];
            }
        }

        constexpr T determinant() const requires (N == 4)
        {
            return det4(m);
        }

        constexpr mat inverted() const requires (N == 4)
        {
            auto inv = mat<N,T>{mat_init::uninitialized};

            auto det = det4(m);
            if (close_to_zero(det)) {
                return tinyla::mat<N,T>{mat_init::identity};
            }

            det = T{1} / det;

            inv.m[0][0] =  det3(m, 1, 2, 3, 1, 2, 3) * det;
            inv.m[0][1] = -det3(m, 0, 2, 3, 1, 2, 3) * det;
            inv.m[0][2] =  det3(m, 0, 1, 3, 1, 2, 3) * det;
            inv.m[0][3] = -det3(m, 0, 1, 2, 1, 2, 3) * det;

            inv.m[1][0] = -det3(m, 1, 2, 3, 0, 2, 3) * det;
            inv.m[1][1] =  det3(m, 0, 2, 3, 0, 2, 3) * det;
            inv.m[1][2] = -det3(m, 0, 1, 3, 0, 2, 3) * det;
            inv.m[1][3] =  det3(m, 0, 1, 2, 0, 2, 3) * det;

            inv.m[2][0] =  det3(m, 1, 2, 3, 0, 1, 3) * det;
            inv.m[2][1] = -det3(m, 0, 2, 3, 0, 1, 3) * det;
            inv.m[2][2] =  det3(m, 0, 1, 3, 0, 1, 3) * det;
            inv.m[2][3] = -det3(m, 0, 1, 2, 0, 1, 3) * det;

            inv.m[3][0] = -det3(m, 1, 2, 3, 0, 1, 2) * det;
            inv.m[3][1] =  det3(m, 0, 2, 3, 0, 1, 2) * det;
            inv.m[3][2] = -det3(m, 0, 1, 3, 0, 1, 2) * det;
            inv.m[3][3] =  det3(m, 0, 1, 2, 0, 1, 2) * det;

            return inv;
        }

        constexpr T* data() noexcept { return *m; }
        constexpr const T* data() const noexcept { return *m; }

        constexpr bool close_to(const mat& other)
        {
            // A simple iterative algorithm but for column-major order.
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    if (!close(m[i][j], other.m[i][j])) {
                        return false;
                    }
                }
            }
            return true;
        }

        friend mat operator*(const mat& a, const mat& b)
        {
            auto c = tinyla::mat<N,T>{mat_init::uninitialized};
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    c.m[i][j] = T{0};
                    for (size_t k = 0; k < N; ++k) {
                        c.m[i][j] += a.m[k][j] * b.m[i][k];
                    }
                }
            }
            return c;
        }

        friend vec<N, T> operator*(const mat& a, const vec<N, T>& b)
        {
            auto mv = tinyla::vec<N,T>{vec_init::uninitialized};
            for (size_t i = 0; i < N; ++i) {
                mv.v[i] = T{0};
                for (size_t j = 0; j < N; ++j) {
                    mv.v[i] += a.m[j][i] * b.v[j];
                }
            }
            return mv;
        }

    private:
        T m[N][N];
    };

    using mat2i = mat<2,int>;
    using mat3i = mat<3,int>;
    using mat4i = mat<4,int>;

    using mat2f = mat<2,float>;
    using mat3f = mat<3,float>;
    using mat4f = mat<4,float>;
}

#endif // TINYLA_MAT_H
