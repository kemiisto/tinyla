#ifndef TINYLA_MAT_INL
#define TINYLA_MAT_INL

template<std::size_t N, typename T>
requires(N >= 2)
constexpr tinyla::mat<N,T>::mat(mat_init init, vec<N,T> v)
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

template<std::size_t N, typename T>
requires(N >= 2)
constexpr tinyla::mat<N,T>::mat(std::initializer_list<T> values)
{
    assert(values.size() == N*N);
    auto it = values.begin();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            m[j][i] = *it++;
        }
    }
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T& tinyla::mat<N,T>::operator[](std::size_t row, std::size_t column)
{
    return m[column][row];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinyla::mat<N,T>::operator[](std::size_t row, std::size_t column) const
{
    return m[column][row];
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr void tinyla::mat<N,T>::set_to_zero()
{
    for (std::size_t i = 0; i < N; ++i) {
        std::fill(&m[i][0], &m[i][0] + N, T{0});
    }
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr void tinyla::mat<N,T>::set_to_identity()
{
    auto i = tinyla::vec<N,T>{vec_init::uninitialized};
    i.fill(T{1});
    set_to_diagonal(i);
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr void tinyla::mat<N,T>::set_to_diagonal(vec<N,T> const& v)
{
    set_to_zero();
    for (size_t i = 0; i < N; ++i) {
        m[i][i] = v[i];
    }
}

// Calculate the determinant of a 2x2 sub-matrix.
template<typename T>
static inline T det2(const T m[4][4], int col0, int col1, int row0, int row1)
{
    return m[col0][row0] * m[col1][row1] - m[col0][row1] * m[col1][row0];
}

// Calculate the determinant of a 3x3 sub-matrix.
template<typename T>
static inline T det3(const T m[4][4],
     int col0, int col1, int col2,
     int row0, int row1, int row2)
{
    return m[col0][row0] * det2(m, col1, col2, row1, row2)
        - m[col1][row0] * det2(m, col0, col2, row1, row2)
        + m[col2][row0] * det2(m, col0, col1, row1, row2);
}

// Calculate the determinant of a 4x4 matrix.
template<typename T>
static inline T det4(const T m[4][4])
{
    T det;
    det  = m[0][0] * det3(m, 1, 2, 3, 1, 2, 3);
    det -= m[1][0] * det3(m, 0, 2, 3, 1, 2, 3);
    det += m[2][0] * det3(m, 0, 1, 3, 1, 2, 3);
    det -= m[3][0] * det3(m, 0, 1, 2, 1, 2, 3);
    return det;
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T tinyla::mat<N,T>::determinant() const requires (N == 4)
{
    return det4(m);
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr tinyla::mat<N, T> tinyla::mat<N,T>::inverted() const requires (N == 4)
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

template<std::size_t N, typename T>
requires(N >= 2)
constexpr T* tinyla::mat<N,T>::data() noexcept
{
    return *m;
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr const T* tinyla::mat<N,T>::data() const noexcept
{
    return *m;
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr tinyla::mat<N,T> tinyla::mat<N,T>::operator*=(const mat<N,T>& other)
{
    const auto o = other; // prevent aliasing when &o == this
    *this = *this * o;
    return *this;
}

template<std::size_t N, typename T>
requires(N >= 2)
constexpr bool tinyla::mat<N,T>::close_to(mat<N,T> const& other)
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

template<std::size_t N, typename T>
requires(N >= 2)
tinyla::mat<N,T> tinyla::operator*(const mat<N,T>& a, const mat<N,T>& b)
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

template<std::size_t N, typename T>
requires(N >= 2)
tinyla::vec<N,T> tinyla::operator*(const mat<N,T>& m, const vec<N,T>& v)
{
    auto mv = tinyla::vec<N,T>{vec_init::uninitialized};
    for (size_t i = 0; i < N; ++i) {
        mv.v[i] = T{0};
        for (size_t j = 0; j < N; ++j) {
            mv.v[i] += m.m[j][i] * v.v[j];
        }
    }
    return mv;
}

#endif // TINYLA_MAT_INL
