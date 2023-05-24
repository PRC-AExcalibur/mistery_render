#pragma once

#include "math.h"

#include <vector>
#include <iostream>
#include <fstream>


namespace mistery_render
{

    template <class T>
    class ColorRGB : public m_math::Vector<T,3>
    {
    public:
        ColorRGB() : m_math::Vector<T,3>()
        {
        }
        ColorRGB(T r, T g, T b) : m_math::Vector<T,3>({r,g,b})
        {
        }
        ~ColorRGB()
        {
        }
    };

    template <class T>
    class ColorRGBA : public m_math::Vector<T,4>
    {
    public:
        ColorRGBA() : m_math::Vector<T,4>()
        {
        }
        ColorRGBA(T r, T g, T b, T a) : m_math::Vector<T,4>({r,g,b,a})
        {
        }
        ~ColorRGBA()
        {
        }
    };

    template <class Color>
    class ImageRGB
    {
    public: 
        std::vector<std::vector<Color>> image;

        ImageRGB(size_t width, size_t height)
        {
            image.resize(height);
            for (size_t i = 0; i < height; i++)
            {
                image[i].resize(width);
                for (size_t j = 0; j < width; j++)
                {
                    image[i][j] = Color();
                }
            }
        }

        inline size_t GetHeight() const
        {
            return image.size();
        }

        inline size_t GetWidth() const
        {
            if (image.size()==0)
            {
                return 0;
            }
            return image[0].size();
        }

    };

    template <class T>
    inline std::ostream& Save2ppm(ColorRGB<T> rgb, std::ostream& target)
    {
        int ir = int(255.999 * rgb[0]);
        int ig = int(255.999 * rgb[1]);
        int ib = int(255.999 * rgb[2]);
        target<<ir<<' '<<ig<<' '<<ib<<'\n';
        return target;
    }

    template <class T>
    inline std::ostream& Save2ppm(ImageRGB<ColorRGB<T>> img, std::ostream& target)
    {
        target << "P3\n" << img.GetWidth() << ' ' << img.GetHeight() << "\n255\n";
        for (auto row:img.image)
        {
            for (auto color:row)
            {
                Save2ppm(color, target);
            }
        }
        return target;
    }

}