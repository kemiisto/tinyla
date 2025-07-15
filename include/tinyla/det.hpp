#ifndef TINYLA_DET_HPP
#define TINYLA_DET_HPP

// Calculate the determinant of a 2x2 sub-matrix.
template<typename T>
static T det2(const T m[4][4], int col0, int col1, int row0, int row1)
{
    return m[col0][row0] * m[col1][row1] - m[col0][row1] * m[col1][row0];
}

// Calculate the determinant of a 3x3 sub-matrix.
template<typename T>
static T det3(const T m[4][4],
              int col0, int col1, int col2,
              int row0, int row1, int row2)
{
    return m[col0][row0] * det2(m, col1, col2, row1, row2)
        - m[col1][row0] * det2(m, col0, col2, row1, row2)
        + m[col2][row0] * det2(m, col0, col1, row1, row2);
}

// Calculate the determinant of a 4x4 matrix.
template<typename T>
static T det4(const T m[4][4])
{
    T det;
    det  = m[0][0] * det3(m, 1, 2, 3, 1, 2, 3);
    det -= m[1][0] * det3(m, 0, 2, 3, 1, 2, 3);
    det += m[2][0] * det3(m, 0, 1, 3, 1, 2, 3);
    det -= m[3][0] * det3(m, 0, 1, 2, 1, 2, 3);
    return det;
}

#endif // TINYLA_DET_HPP
