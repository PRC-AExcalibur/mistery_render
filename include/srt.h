#pragma once

#include "math.h"

namespace mistery_render
{

namespace srt
{

/**
 * @brief Creates a 4x4 scale matrix
 * @param sx The scale factor along the x-axis
 * @param sy The scale factor along the y-axis
 * @param sz The scale factor along the z-axis
 * @return A 4x4 matrix representing the scaling transformation
 */
inline m_math::Matrix4d ScaleMatrix(double sx, double sy, double sz) 
{
    return m_math::Matrix4d({sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1});
}

/**
 * @brief Creates a 4x4 translation matrix
 * @param dx The translation distance along the x-axis
 * @param dy The translation distance along the y-axis
 * @param dz The translation distance along the z-axis
 * @return A 4x4 matrix representing the translation transformation
 */
inline m_math::Matrix4d TranslateMatrix(double dx, double dy, double dz) 
{
    return m_math::Matrix4d({1, 0, 0, dx, 0, 1, 0, dy, 0, 0, 1, dz, 0, 0, 0, 1});
}

/**
 * @brief Creates a 4x4 rotation matrix around the X-axis
 * @param theta The rotation angle in radians
 * @return A 4x4 matrix representing the rotation around the X-axis
 */
inline m_math::Matrix4d RotateXMatrix(double theta) 
{
    return m_math::Matrix4d({1, 0, 0, 0, 0, cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1});
}

/**
 * @brief Creates a 4x4 rotation matrix around the Y-axis
 * @param theta The rotation angle in radians
 * @return A 4x4 matrix representing the rotation around the Y-axis
 */
inline m_math::Matrix4d RotateYMatrix(double theta) 
{
    return m_math::Matrix4d({cos(theta), 0, sin(theta), 0, 0, 1, 0, 0, -sin(theta), 0, cos(theta), 0, 0, 0, 0, 1});
}

/**
 * @brief Creates a 4x4 rotation matrix around the Z-axis
 * @param theta The rotation angle in radians
 * @return A 4x4 matrix representing the rotation around the Z-axis
 */
inline m_math::Matrix4d RotateZMatrix(double theta) 
{
    return m_math::Matrix4d({cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
}

// /**
//  * @brief Transforms a 4D point using a given 4x4 matrix
//  * @param matrix The 4x4 transformation matrix
//  * @param point The 4D point to be transformed, 3D point represented as a 4D vector with w=1
//  * @return The transformed 4D vector
//  */
// inline m_math::Vector4d TransformPoint(const m_math::Matrix4d &matrix, const m_math::Vector4d &point)
// {
//     return matrix * point;
// }


}

}