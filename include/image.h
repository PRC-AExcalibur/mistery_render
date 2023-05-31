#pragma once

#include "math.h"

#include <vector>
#include <iostream>
#include <fstream>


namespace mistery_render
{
    /**  
     * @brief Represents a color in RGB format.
     */  
    template <class T>
    class ColorRGB : public m_math::Vector<T,3>
    {
    public:
        /**  
         * @brief Default constructor.  
         */ 
        ColorRGB() : m_math::Vector<T,3>()
        {
        }
        /**  
         * @brief Constructor with RGB values.  
         * @param r Red component  
         * @param g Green component  
         * @param b Blue component  
         */ 
        ColorRGB(T r, T g, T b) : m_math::Vector<T,3>({r,g,b})
        {
        }
        /**  
         * @brief Destructor.  
         */  
        ~ColorRGB()
        {
        }
    };

    /**  
     * @brief Represents a color in RGBA format.  
     */  
    template <class T>
    class ColorRGBA : public m_math::Vector<T,4>
    {
    public:
        /**  
         * @brief Default constructor.  
         */ 
        ColorRGBA() : m_math::Vector<T,4>()
        {
        }
        /**  
         * @brief Constructor with RGBA values.  
         * @param r Red component  
         * @param g Green component  
         * @param b Blue component  
         * @param a Alpha component (transparency)  
         */ 
        ColorRGBA(T r, T g, T b, T a) : m_math::Vector<T,4>({r,g,b,a})
        {
        }

        /**  
         * @brief Destructor.  
         */  
        ~ColorRGBA()
        {
        }
    };

    /**  
     * @brief Represents an image with a specified color type.  
     */  
    template <class Color>
    class Image
    {
    private: 
        std::vector<std::vector<Color>> image;
    public: 
        /**  
         * @brief Constructor with image dimensions.  
         * @param width Width of the image  
         * @param height Height of the image  
         */  
        Image(size_t width, size_t height, Color color = Color())
        {
            image.resize(height);
            for (size_t i = 0; i < height; i++)
            {
                image[i].resize(width);
                for (size_t j = 0; j < width; j++)
                {
                    image[i][j] = color;
                }
            }
        }


        /**  
         * @brief Get color of element in this image
         * @attention Not check index < (width or height)
         * @param width_idx Index of width
         * @param height_idx Index of height
         * @return color The color which is at the image(width_idx, height_idx)
         */  
        Color GetColor(size_t width_idx, size_t height_idx)
        {
            return image[height_idx][width_idx];
        }

        /**  
         * @brief Set color of element in this image
         * @attention Not check index < (width or height)
         * @param width_idx Index of width
         * @param height_idx Index of height
         * @param color The color which will be at the image(width_idx, height_idx)
         */  
        void SetColor(size_t width_idx, size_t height_idx, const Color& color)
        {
            image[height_idx][width_idx] = color;
        }

        /**  
         * @brief Set color of element in this image
         * @param width_idx Index of width
         * @param height_idx Index of height
         * @param color The color which will be at the image(width_idx, height_idx)
         * @return The color setting is successful or not
         */  
        bool TrySetColor(size_t width_idx, size_t height_idx, const Color& color)
        {
            if(height_idx<GetHeight() && width_idx<GetWidth())
            {
                image[height_idx][width_idx] = color;
                return true;
            }
            return false;
        }

        /**  
         * @brief Get the height of the image.  
         * @return Height of the image  
         */  
        inline size_t GetHeight() const
        {
            return image.size();
        }

        /**  
         * @brief Get the width of the image.  
         * @return Width of the image  
         */ 
        inline size_t GetWidth() const
        {
            if (image.size()==0)
            {
                return 0;
            }
            return image[0].size();
        }

        template <class T>
        friend std::ostream& Save2ppm(Image<ColorRGB<T>> img, std::ostream& target);

    };

    /**  
     * @brief Saves a single RGB color to a PPM file.  
     * @param rgb RGB color to save  
     * @param target Output stream to write to  
     * @return Reference to the output stream  
     */  
    template <class T>
    inline std::ostream& Save2ppm(ColorRGB<T> rgb, std::ostream& target)
    {
        int ir = int(255.999 * rgb[0]);
        int ig = int(255.999 * rgb[1]);
        int ib = int(255.999 * rgb[2]);
        target<<ir<<' '<<ig<<' '<<ib<<'\n';
        return target;
    }
    
    /**  
     * @brief Saves an image with RGB colors to a PPM file.  
     * @param img Image to save  
     * @param target Output stream to write to  
     * @return Reference to the output stream  
     */  
    template <class T>
    inline std::ostream& Save2ppm(Image<ColorRGB<T>> img, std::ostream& target)
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


    using ColorRGB_d = ColorRGB<double>;
    using ColorRGBA_d = ColorRGBA<double>;

    using Image_RGB_d = Image<ColorRGB<double>>;
    using Image_RGBA_d = Image<ColorRGBA<double>>;
}