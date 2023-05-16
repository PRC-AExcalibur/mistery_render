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
    class Matrix
    {

    public:
        std::array<std::array<T, col>, row> elements;

        /**
         * @brief constructor default (zero matrix);
         * class T must provide constructor T(0)
         */
        Matrix()
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
        Matrix(const std::array<std::array<T, col>, row> &elem) : elements(elem)
        {
        };

        /**
         * @brief constructor copy
         * @param mat: matrix for copy
         */
        Matrix(const Matrix &mat) : elements(mat.elements)
        {
        }

        /**
         * @brief destructor copy
         */
        ~Matrix()
        {
        };

        /**
         * @brief matrix overload operator=
         */
        inline Matrix<T, row, col> &operator=(const Matrix<T, row, col> &rhs)
        {
            this->elements = rhs.elements;
            return *this;
        }

        /**
         * @brief matrix overload operator==; if T is double, equal will use tolerance of kDoubleAsZero
         */
        inline bool operator==(const Matrix<T, row, col> &rhs)
        {
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    if (!IsEqual(this->elements[i][j], rhs.elements[i][j]))
                    {
                        return false;
                    }
                }
            }
            return true;
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
        inline friend Matrix<T, row, col> &operator+=(Matrix &lhs, const Matrix &rhs)
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
        inline friend Matrix<T, row, col> &operator-=(Matrix &lhs, const Matrix<T, row, col> &rhs)
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
        inline friend Matrix<T, row, col> &operator*=(Matrix &lhs, const T scalar)
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
        inline friend Matrix<T, row, col> &operator/=(Matrix &lhs, const T scalar)
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
        inline friend Matrix<T, row, col> operator+(const Matrix &lhs, const Matrix &rhs)
        {
            Matrix<T, row, col> result;
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
        inline friend Matrix<T, row, col> operator-(const Matrix &lhs, const Matrix &rhs)
        {
            Matrix<T, row, col> result;
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
        inline friend Matrix<T, row, col> operator*(const T scalar, const Matrix &mat)
        {
            Matrix<T, row, col> result;
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
        inline friend Matrix<T, row, col> operator*(const Matrix &mat, const T scalar)
        {
            Matrix<T, row, col> result;
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
        inline friend Matrix<T, row, col> operator/(const Matrix &mat, const T scalar)
        {
            Matrix<T, row, col> result;
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
        inline friend Matrix<T, row, row2> operator*(const Matrix<T, row, col> &lhs, const Matrix<T, col, row2> &rhs)
        {
            Matrix<T, row, row2> result;
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
         * @brief matrix output overload operator<<
         */
        inline friend std::ostream& operator<<(std::ostream& os, const Matrix<T, row, col>& m)
        {
            for (size_t i = 0; i < row; ++i)
            {  
                for (size_t j = 0; j < col; ++j)
                {
                    os << m.elements[i][j] << " ";
                }
                os << "\n";
            }
            return os;
        }

        /**
         * @brief transpose of matrix
         */
        inline Matrix<T, col, row> Transpose() const
        {
            Matrix<T, col, row> transposed;
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
         * @brief hadamard product of matrix
         */
        inline Matrix<T, col, row> HadamardProduct(const Matrix &rhs) const
        {
            Matrix<T, col, row> res;
            for (size_t i = 0; i < row; ++i)
            {
                for (size_t j = 0; j < col; ++j)
                {
                    res.elements[j][i] = this->elements[i][j] * rhs.elements[i][j];
                }
            }
            return res;
        }

        /**
         * @brief get elements of this matrix
         * @return std::array<std::array<T, col> , row>
         * @retval elements of this matrix
         */
        inline std::array<std::array<T, col>, row> GetElements() const
        {
            return elements;
        }

        /**
         * @brief get identity matrix
         */
        inline static Matrix<T, row, row> IdentityMatrix()
        {
            Matrix<T, row, row> iden = Matrix<T, row, row>();
            for (size_t i = 0; i < row; i++)
            {
                iden[i][i] = T(1);
            }
            return iden;
        }
    };

    typedef Matrix<double, 2, 2> Matrix2d;
    typedef Matrix<double, 3, 3> Matrix3d;
    typedef Matrix<double, 4, 4> Matrix4d;
    typedef Matrix<double, 3, 4> Matrix_3x4d;
    typedef Matrix<double, 4, 3> Matrix_4x3d;

}

}
