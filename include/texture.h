#pragma once

#include "math.h"
#include "image.h"

namespace mistery_render
{

template <class real_t = double, size_t color_dim = 4>
using Texture1Dim = std::vector<std::array<real_t, color_dim>>;

template <class real_t = double, size_t color_dim = 4>
using Texture2Dim = std::vector<std::vector<std::array<real_t, color_dim>>>;

namespace texture
{

template <class real_t = double>
inline ColorRGBA<real_t> Lerp(const Texture1Dim<real_t, 4>& texture1d, double u)
{
    size_t max_x = texture1d.size();

    size_t int_u = static_cast<size_t>(u * (max_x - 1));
    if (int_u + 1 >= max_x) int_u = (int_u + 1) % max_x;

    double frac_u = u * (max_x - 1) - int_u;

    m_math::Vector<real_t, 4> color1(texture1d[int_u]);
    m_math::Vector<real_t, 4> color2(texture1d[int_u + 1]);
    return (1.0 - frac_u) * color1 + frac_u * color2;
}


template <class real_t = double>
inline ColorRGBA<real_t> Lerp2(const Texture2Dim<real_t, 4>& texture2d, double u, double v)
{
    size_t max_x = texture2d[0].size();
    size_t max_y = texture2d.size();

    size_t int_u = static_cast<size_t>(u * (max_x - 1));
    size_t int_v = static_cast<size_t>(v * (max_y - 1));

    if (int_u + 1 >= max_x) int_u = (int_u + 1) % max_x;
    if (int_v + 1 >= max_y) int_v = (int_v + 1) % max_y;

    double frac_u = u * (max_x - 1) - int_u;
    double frac_v = v * (max_y - 1) - int_v;

    m_math::Vector<real_t, 4> color1 (texture2d[int_v][int_u]);
    m_math::Vector<real_t, 4> color2 (texture2d[int_v][int_u + 1]);
    m_math::Vector<real_t, 4> color3 (texture2d[int_v + 1][int_u]);
    m_math::Vector<real_t, 4> color4 (texture2d[int_v + 1][int_u + 1]);

    m_math::Vector<real_t, 4> color_top = (1.0 - frac_u) * color1 + frac_u * color2;
    m_math::Vector<real_t, 4> color_bottom = (1.0 - frac_u) * color3 + frac_u * color4;
    return (1.0 - frac_v) * color_top + frac_v * color_bottom;
}


}

}