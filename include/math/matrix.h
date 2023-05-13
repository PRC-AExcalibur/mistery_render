#pragma once

#include "internal.h"

namespace mistery_render
{

namespace m_math
{

    /**
     * @brief class matrix in {mistery_render::m_math}
     * @param T: type of elements
     * @param row: row of this matrix
     * @param col: col of this matrix
     */
    template <class T, size_t row, size_t col>
    class matrix
    {

    public:
        std::array<std::array<T, col>, row> elements;

        /**
         * @brief constructor default (zero matrix);
         * class T must provide constructor T(0)
         */
        matrix()
        {
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    elements[i][j] = T(0);
                }
            }
        };

        /**
         * @brief constructor by elems
         * @param elems: elements of this matrix
         */
        matrix(const std::array<std::array<T, col>, row> &elem) : elements(elem)
        {
        };

        /**
         * @brief constructor copy
         * @param mat: matrix for copy
         */
        matrix(const matrix &mat) : elements(mat.elements)
        {
        }

        /**
         * @brief destructor copy
         */
        ~matrix()
        {
        };

        /**
         * @brief matrix overload operator=
         */
        inline matrix<T, row, col> &operator=(const matrix<T, row, col> &rhs)
        {
            this->elements = rhs.elements;
            return *this;
        }

        /**
         * @brief matrix overload operator[]
         */
        inline std::array<T, col> &operator[](size_t index)
        {
            return this->elements[index];
        }

        /**
         * @brief matrix overload operator[] const
         */
        inline std::array<T, col> operator[](size_t index) const
        {
            return this->elements[index];
        }

        /**
         * @brief matrix overload += operator
         */
        inline friend matrix<T, row, col> &operator+=(matrix &lhs, const matrix &rhs)
        {
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    lhs.elements[i][j] += rhs.elements[i][j];
                }
            }
            return lhs;
        }

        /**
         * @brief matrix overload -= operator
         */
        inline friend matrix<T, row, col> &operator-=(matrix &lhs, const matrix<T, row, col> &rhs)
        {
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    lhs.elements[i][j] -= rhs.elements[i][j];
                }
            }
            return lhs;
        }

        /**
         * @brief matrix overload *= operator for scalar
         */
        inline friend matrix<T, row, col> &operator*=(matrix &lhs, const T scalar)
        {
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    lhs.elements[i][j] *= scalar;
                }
            }
            return lhs;
        }

        /**
         * @brief matrix overload /= operator for scalar
         */
        inline friend matrix<T, row, col> &operator/=(matrix &lhs, const T scalar)
        {
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    lhs.elements[i][j] /= scalar;
                }
            }
            return lhs;
        }

        /**
         * @brief 2 matrix overload operator+
         */
        inline friend matrix<T, row, col> operator+(const matrix &lhs, const matrix &rhs)
        {
            matrix<T, row, col> result;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    result.elements[i][j] = lhs.elements[i][j] + rhs.elements[i][j];
                }
            }
            return result;
        }

        /**
         * @brief 2 matrix overload operator-
         */
        inline friend matrix<T, row, col> operator-(const matrix &lhs, const matrix &rhs)
        {
            matrix<T, row, col> result;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    result.elements[i][j] = lhs.elements[i][j] - rhs.elements[i][j];
                }
            }
            return result;
        }

        /**
         * @brief scalar and matrix overload operator*
         */
        inline friend matrix<T, row, col> operator*(const T scalar, const matrix &mat)
        {
            matrix<T, row, col> result;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    result.elements[i][j] = scalar * mat.elements[i][j];
                }
            }
            return result;
        }

        /**
         * @brief matrix and scalar overload operator*
         */
        inline friend matrix<T, row, col> operator*(const matrix &mat, const T scalar)
        {
            matrix<T, row, col> result;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    result.elements[i][j] = scalar * mat.elements[i][j];
                }
            }
            return result;
        }

        /**
         * @brief matrix and scalar overload operator/
         */
        inline friend matrix<T, row, col> operator/(const matrix &mat, const T scalar)
        {
            matrix<T, row, col> result;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    result.elements[i][j] = mat.elements[i][j] / scalar;
                }
            }
            return result;
        }

        /**
         * @brief 2 matrix overload operator*
         */
        template <size_t row2>
        inline friend matrix<T, row, row2> operator*(const matrix<T, row, col> &lhs, const matrix<T, col, row2> &rhs)
        {
            matrix<T, row, row2> result;
            std::pair<size_t, size_t> ij;

            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < row2; ++j)
                {
                    for (size_t k = 0; k < col; ++k)
                    {
                        result.elements[i][j] += lhs.elements[i][k] * rhs.elements[k][j];
                    }
                }
            }
            return result;
        }

        /**
         * @brief transpose of matrix
         */
        inline matrix<T, col, row> transpose() const
        {
            matrix<T, col, row> transposed;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    transposed.elements[j][i] = this->elements[i][j];
                }
            }
            return transposed;
        }

        /**
         * @brief get elements of this matrix
         * @return std::array<std::array<T, col> , row>
         * @retval elements of this matrix
         */
        inline std::array<std::array<T, col>, row> get_elements() const
        {
            return elements;
        }
    };

    typedef matrix<double, 3, 3> matrix3d;
    typedef matrix<double, 3, 4> matrix_3x4d;
    typedef matrix<double, 4, 3> matrix_4x3d;

}

}
