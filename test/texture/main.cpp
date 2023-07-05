#include "../test.h"

using namespace mistery_render;


void tgaImageTest(std::string tga_filename)
{
    Image_RGB_d img_test(1,1);
    TestExpect(ImageFromTGA(tga_filename, img_test), true, "Read TGA Image Test");
    std::ofstream file("output/test/tga_test.ppm");
    Save2ppm(img_test, file);
    file.close();
}

int main()
{
    tgaImageTest("../model/cubic/keqing.tga");
    return 0;
}