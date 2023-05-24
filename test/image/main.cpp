# include "../test.h"

using namespace mistery_render;

void ppmTest()
{
    double image_width = 256;
    double image_height = 256;
    Image_RGB_d image_test(image_width, image_height);
    for (size_t i = 0; i < image_width; i++) 
    {
        for (size_t j = 0; j < image_height; j++)
        {
            double r = double(i) / (image_width-1);
            double g = double(j) / (image_height-1);
            double b = 0.0;
            
            image_test.image[j][i] = ColorRGB<double>(r,g,b);
        }
    }
    std::ofstream file("output/test/image_test.ppm");
    Save2ppm(image_test, file);
    file.close();
}

int main() 
{
    ppmTest();
    return 0;
}