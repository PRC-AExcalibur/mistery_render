#pragma once

#include <math.h>
#include <array>


namespace mistery_render
{

namespace m_math
{
    /**
     * @brief const PI
     */
    const double kPi = M_PI;

    /**
     * @brief const 0.5*PI
     */
    const double kHalfPi = M_PI_2;


    /**  
     * @brief small value close to zero for double
     */  
    const double kDoubleAsZero = 1e-9;

    /**  
     * @brief compares 2 double numbers for equality with a given tolerance
     * @param a: first number to compare
     * @param b: second number to compare
     * @param eps: tolerance for the comparison
     * @return a==b with a given tolerance
     */  
    inline bool IsEqual(double a, double b, double eps)
    {
        return fabs(a-b)<eps;
    }

    /**  
     * @brief compares 2 class for equality; if T is double, call IsEqual(a,b,eps) which eps = kDoubleAsZero
     * @param a: first class to compare
     * @param b: second class to compare
     * @return a==b for equality
     */  
    template<typename T>
    inline bool IsEqual(T a, T b)
    {  
        if constexpr (std::is_floating_point_v<T>)
        {
            return IsEqual(a, b, kDoubleAsZero);
        }
        else 
        {
            return a == b;
        }  
    } 


}

}

