#pragma once

#include "image.h"

namespace mistery_render
{

    /**
     * @brief Draw a line on an image with a specific color
     * @param start_point The starting point (x,y) of the line
     * @param end_point The ending point (x,y) of the line
     * @param img A reference to the image on which the line will be drawn
     * @param color The color that will be used to draw the line
     */
    template <class Color>
    inline void LineDraw(m_math::Vector2i start_point, m_math::Vector2i end_point, Image<Color> &img, const Color &color)
    {
        bool transpose = std::abs(start_point[0] - end_point[0]) < std::abs(start_point[1] - end_point[1]);
        if (transpose)
        {
            std::swap(start_point[0], start_point[1]);
            std::swap(end_point[0], end_point[1]);
        }
        if (start_point[0] > end_point[0])
        {
            std::swap(start_point[0], end_point[0]);
            std::swap(start_point[1], end_point[1]);
        }
        int dx = end_point[0] - start_point[0];
        int dy = end_point[1] - start_point[1];
        int d_err = std::abs(dy) * 2;
        int err = 0;
        int y = start_point[1];
        for (int x = start_point[0]; x <= end_point[0]; x++)
        {
            if (transpose)
            {
                img.SetColor(y, x, color);
            }
            else
            {
                img.SetColor(x, y, color);
            }
            err += d_err;
            if (err > dx)
            {
                y += (end_point[1] > start_point[1] ? 1 : -1);
                err -= dx * 2;
            }
        }
    }

    /**
     * @brief Draw a circle on an image with a specific color
     * @param center The center point (x,y) of the circle
     * @param radius The radius of the circle
     * @param img A reference to the image on which the line will be drawn
     * @param color The color that will be used to draw the line
     */
    template <class Color>
    inline void CircleDraw(m_math::Vector2i center, int radius, Image<Color> &img, const Color &color)
    {
        int x = 0; 
        int y = radius;
        int d = (radius - 1) * 2;

        while (x < y) 
        {
            img.TrySetColor(center[0] + x, center[1] + y, color);
            img.TrySetColor(center[0] - x, center[1] + y, color);
            img.TrySetColor(center[0] - y, center[1] + x, color);
            img.TrySetColor(center[0] + y, center[1] + x, color);
            img.TrySetColor(center[0] + x, center[1] - y, color);
            img.TrySetColor(center[0] - x, center[1] - y, color);
            img.TrySetColor(center[0] - y, center[1] - x, color);
            img.TrySetColor(center[0] + y, center[1] - x, color);
            if (d < 0)
            {
                d += 2*x + 3;
            }
            else 
            {
                d += 2*(x - y) + 5;
                y--;
            }
            x++;
        }
    }


}