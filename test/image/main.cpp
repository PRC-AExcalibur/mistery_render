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

            image_test.SetColor(i, j, ColorRGB<double>(r,g,b));
        }
    }
    std::ofstream file("output/test/image_test.ppm");
    Save2ppm(image_test, file);
    file.close();
    TestExpect(" ", " ", "Save PPM Image Test");
}

void DrawLineTest()
{
    double image_width = 512;
    double image_height = 256;
    Image_RGB_d line_test(image_width, image_height);

    m_math::Vector2i p1({10,10});
    m_math::Vector2i p2({10,200});
    m_math::Vector2i p3({200,200});
    LineDraw(p1,p2,line_test,ColorRGB<double>(1,0,0));
    LineDraw(p2,p3,line_test,ColorRGB<double>(0,1,0));
    LineDraw(p1,p3,line_test,ColorRGB<double>(0,0,1));

    std::ofstream file_line("output/test/line_test.ppm");
    Save2ppm(line_test, file_line);
    file_line.close();
    TestExpect(" ", " ", "Draw Line Test");
}

void DrawCircleTest()
{
    double image_width = 512;
    double image_height = 256;
    Image_RGB_d circle_test(image_width, image_height);

    m_math::Vector2i p1({10,10});
    m_math::Vector2i p2({10,100});
    m_math::Vector2i p3({100,100});
    CircleDraw(p1,30,circle_test,ColorRGB<double>(1,0,0));
    CircleDraw(p2,50,circle_test,ColorRGB<double>(0,1,0));
    CircleDraw(p3,60,circle_test,ColorRGB<double>(0,0,1));

    std::ofstream file_line("output/test/circle_test.ppm");
    Save2ppm(circle_test, file_line);
    file_line.close();
    TestExpect(" ", " ", "Draw Circle Test");
}

int main() 
{
    ppmTest();
    DrawLineTest();
    DrawCircleTest();
    return 0;
}