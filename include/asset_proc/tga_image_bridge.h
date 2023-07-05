#pragma once

#include <memory>
#include <array>

#include "tga_image.h"
#include "../image.h"

namespace mistery_render
{

template <class Color>
bool ImageFromTGA(const std::string &tga_filename, Image<Color> &image_target)
{
    TGAImage tga_image;
    if (tga_image.read_tga_file(tga_filename))
    {
        int width = tga_image.width();
        int height = tga_image.height();

        image_target = Image<Color>(width, height);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                TGAColor tga_color = tga_image.get(x, y);
                Color rgb_color = {
                    tga_color[2] / 255.0,
                    tga_color[1] / 255.0,
                    tga_color[0] / 255.0
                };
                image_target.SetColor(x, y, rgb_color);
            }
        }
        return true;
    }
    else
    {
        std::cerr << "Failed to read TGA file." << std::endl;
        return false;
    }
}


}