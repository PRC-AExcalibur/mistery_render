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
        inline T Dot(const Vector<T, dim> &rhs) const
        {
            return (*this)* rhs;
        }

        /**
         * @brief cross product for 3D vectors only
         */
        template<typename = std::enable_if_t<dim == 3>>
        inline Vector<T, 3> Cross(const Vector<T, dim> &rhs) const
        {
            Vector<T, 3> res;
            res[0] = this->elements[1][0] * rhs[2] - this->elements[2][0] * rhs[1];
            res[1] = this->elements[2][0] * rhs[0] - this->elements[0][0] * rhs[2];
            res[2] = this->elements[0][0] * rhs[1] - this->elements[1][0] * rhs[0];
            return res;
        }


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
            return IsEqual(this->Dot(target), T(0));
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

    typedef Vector<double,3> Vector3d;

}

}