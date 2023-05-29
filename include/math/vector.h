#pragma once

#include "internal.h"
#include "matrix.h"

namespace mistery_render
{

namespace m_math
{
    /**
     * @brief class vector in {mistery_render::m_math}
     * @param T: type of elements
     * @param dim: dimension of this vector
     */
    template <class T, size_t dim>
    class Vector : public Matrix<T, dim, 1>
    {
    public:
    
        /**
         * @brief constructor default (zero vector);
         * class T must provide constructor T(0)
         */
        Vector():Matrix<T, dim, 1>()
        {
        };

        /**
         * @brief constructor by elems
         * @param elems: elements of this vector
         */
        Vector(const std::array<std::array<T, 1>, dim> &elem) : Matrix<T, dim, 1>(elem)
        {
        };

        /**
         * @brief constructor by vector
         * @param elems: elements of this vector
         */
        Vector(const Matrix<T, dim, 1> &mat) : Matrix<T, dim, 1>(mat)
        {
        };

        /**
         * @brief constructor copy
         * @param mat: vector for copy
         */
        Vector(const Vector &vec) : Matrix<T, dim, 1>(vec.elements)
        {
        }

        /**
         * @brief destructor copy
         */
        ~Vector()
        {
        };

        /**
         * @brief vector overload operator=
         */
        inline Vector<T, dim> &operator=(const Vector<T, dim> &rhs)
        {
            this->elements = rhs.elements;
            return *this;
        }

        /**
         * @brief vector overload operator[]
         */
        inline T &operator[](size_t index)
        {
            return this->elements[index][0];
        }

        /**
         * @brief vector overload operator[] const
         */
        inline T operator[](size_t index) const
        {
            return this->elements[index][0];
        }

        /**
         * @brief dot product for 2 vector overload operator*
         */
        inline friend T operator*(const Vector<T, dim> &lhs, const Vector<T, dim> &rhs)
        {
            T res = lhs[0] * rhs[0];
            for (size_t i = 1; i < dim; i++)
            {
                res += lhs[i] * rhs[i];
            }
            return res;
        }

        /**
         * @brief dot product for 2 vector
         */
        template <class T_f, size_t dim_f>
        friend T_f Dot(const Vector<T_f, dim_f> &lhs, const Vector<T_f, dim_f> &rhs);

        /**
         * @brief cross product for 3D vectors only
         */
        template <class T_f>
        friend Vector<T_f, 3> Cross(const Vector<T_f, 3> &lhs, const Vector<T_f, 3> &rhs);


        /**
         * @brief norm of vector
         */
        inline T Norm() const
        {
            return sqrt((*this) * (*this));
        }

        /**  
         * @brief normalizes the vector by returning a new normalized vector
         * @attention norm of the vector must be non-zero
         */ 
        inline Vector<T, dim> Normalize() const
        {
            Vector<T, dim> res = *this;
            T n_tmp = res.Norm();
            for (size_t i = 0; i < dim; i++)
            {
                res[i] /= n_tmp;
            }
            return res;
        }

        /**  
         * @brief normalizes the vector in-place
         * @attention norm of the vector must be non-zero
         */ 
        inline Vector<T, dim>& NormalizeInplace()
        {
            T n_tmp = this->Norm();
            for (size_t i = 0; i < dim; i++)
            {
                this->elements[i][0] /= n_tmp;
            }
            return *this;
        }

        /**  
         * @brief check if the vector is equal to the zero vector
         */   
        inline bool IsZero() const 
        {
            return IsEqual(*this, Vector());
        }

        /**
         * @brief check if the vector is orthogonal (perpendicular) to the target vector
         * @attention norm of 2 vectors must be non-zero
         * @param target target vector to check orthogonality with
         */ 
        inline bool IsOrthogonal(Vector<T, dim> target) const 
        {
            return IsEqual(Dot(*this, target), T(0));
        }

        /**    
         * @brief check if the vector is parallel to the target vector
         * @attention norm of 2 vectors must be non-zero
         * @param target The vector to check parallelism with
         */  
        inline bool IsParallel(Vector<T, dim> target) const 
        {
            return this->Normalize() == target.Normalize();
        }

    };

    /**
     * @brief dot product for 2 vector
     */
    template <class T_f, size_t dim_f>
    inline T_f Dot(const Vector<T_f, dim_f> &lhs, const Vector<T_f, dim_f> &rhs)
    {
        return lhs * rhs;
    }

    /**
     * @brief cross product for 3D vectors only
     */
    template <class T_f>
    inline Vector<T_f, 3> Cross(const Vector<T_f, 3> &lhs, const Vector<T_f, 3> &rhs)
    {
        Vector<T_f, 3> res;
        res[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
        res[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
        res[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
        return res;
    }



    using Vector2i = Vector<int,2>;
    using Vector3i = Vector<int,3>;
    using Vector4i = Vector<int,4>;
    
    using Vector2d = Vector<double,2>;
    using Vector3d = Vector<double,3>;
    using Vector4d = Vector<double,4>;

}

}