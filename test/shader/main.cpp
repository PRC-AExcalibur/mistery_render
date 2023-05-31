# include "../test.h"

using namespace mistery_render;


void TriangleTest() 
{
    double image_width = 512;
    double image_height = 256;
    Image_RGB_d triangle_test(image_width, image_height);
    ZBuffer zb_triangle = MakeZBuffer(triangle_test);

    std::array<m_math::Vector3d, 3> tri1;
    tri1[0] = m_math::Vector3d({100, 100, 50});
    tri1[1] = m_math::Vector3d({200, 100, 50});
    tri1[2] = m_math::Vector3d({150, 50, 50});
    TriangleDraw(tri1, zb_triangle, triangle_test, {1, 0, 0});

    std::array<m_math::Vector3d, 3> tri2;
    tri2[0] = m_math::Vector3d({150, 100, 100});
    tri2[1] = m_math::Vector3d({250, 100, -100});
    tri2[2] = m_math::Vector3d({200, 50, -100});
    TriangleDraw(tri2, zb_triangle, triangle_test, {0, 1, 0});

    std::array<m_math::Vector3d, 3> tri3;
    tri3[0] = m_math::Vector3d({150, 70, 200});
    tri3[2] = m_math::Vector3d({100, 120, -100});
    tri3[1] = m_math::Vector3d({200, 120, -100});
    TriangleDraw(tri3, zb_triangle, triangle_test, {0, 0, 1});

    std::ofstream file_triangle("output/test/triangle_test.ppm");
    Save2ppm(triangle_test, file_triangle);
    file_triangle.close();
}


int main() 
{
    TriangleTest();
    return 0;
}