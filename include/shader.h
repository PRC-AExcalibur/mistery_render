#pragma once

#include "image.h"

namespace mistery_render
{

    using ZBuffer = Image<double>;

    /**
     * @brief Creates a Z-buffer of the given image
     * @param Color The type of color data used in the image
     * @param img The reference image from which to obtain the width and height for the Z-buffer
     * @param depth The initialized depth of zbuffer,default depth is std::numeric_limits<double>::max()
     * @return Z-buffer
     */
    template <class Color>
    inline ZBuffer MakeZBuffer(const Image<Color>& img, double depth = -std::numeric_limits<double>::max())
    {
        return ZBuffer(img.GetWidth(), img.GetHeight(), depth);
    }

    /**
     * @brief Calculates the barycentric coordinates of a point within a triangle
     * @param points The points of triangle
     * @param target_point The point for which the barycentric coordinates are to be calculated
     * @return barycentric coordinates
     * @attention If the target_point is outside the triangle, returns {-1,1,1}
     */
    inline m_math::Vector3d Barycentric(const std::array<m_math::Vector3d, 3>& points, m_math::Vector3d target_point) 
    {
        // m_math::Vector3d ab = points[1] - points[0];
        // m_math::Vector3d ac = points[2] - points[0];
        // m_math::Vector3d pa = points[0] - target_point;

        m_math::Vector3d ux ({points[1][0] - points[0][0], points[2][0] - points[0][0], points[0][0] - target_point[0]});
        m_math::Vector3d uy ({points[1][1] - points[0][1], points[2][1] - points[0][1], points[0][1] - target_point[1]});

        m_math::Vector3d u = m_math::Cross(ux, uy);

        if (std::abs(u[2])>m_math::kDoubleAsZero) 
        {
            return m_math::Vector3d({1.0-(u[0]+u[1])/u[2], u[0]/u[2], u[1]/u[2]}); 
        }
        return m_math::Vector3d({-1,1,1});
    } 


    /**
     * @brief Draw a triangle on zbuffer_image with a specific color
     * @param points The points (x,y,z) of the triangle
     * @param img A reference to the image on which the line will be drawn
     * @param color The color that will be used to draw the line
     */
    template <class Color>
    inline void TriangleDraw(const std::array<m_math::Vector3d, 3>& points, ZBuffer &zbuffer, Image<Color> &img, Color color)
    {
        m_math::Vector2d img_size({(double)img.GetWidth() - 1, (double)img.GetHeight() - 1});
        m_math::Vector2d bbox_min({ std::numeric_limits<float>::max(),  std::numeric_limits<double>::max()});
        m_math::Vector2d bbox_max({-std::numeric_limits<float>::max(), -std::numeric_limits<double>::max()});

        for (size_t i=0; i<3; i++) 
        {
            for (size_t j=0; j<2; j++) 
            {
                bbox_min[j] = std::max(0.0, std::min(bbox_min[j], points[i][j]));
                bbox_max[j] = std::min(img_size[j], std::max(bbox_max[j], points[i][j]));
            }
        }

        m_math::Vector3d point_tmp({bbox_min[0], bbox_min[1], 0});
        for (point_tmp[0] = bbox_min[0]; point_tmp[0] <= bbox_max[0]; point_tmp[0]++)
        {
            for (point_tmp[1] = bbox_min[1]; point_tmp[1] <= bbox_max[1]; point_tmp[1]++)
            {
                m_math::Vector3d bc_tmp = Barycentric(points, point_tmp);
                if (bc_tmp[0] < 0.0 || bc_tmp[1] < 0.0 || bc_tmp[2] < 0.0)
                {
                    continue;
                }
                point_tmp[2] = 0;
                for (int i=0; i<3; i++) 
                {
                    point_tmp[2] += points[i][2]*bc_tmp[i];
                }
                if (zbuffer.GetColor(point_tmp[0], point_tmp[1])<point_tmp[2]+m_math::kDoubleAsZero) 
                {
                    zbuffer.SetColor(point_tmp[0], point_tmp[1], point_tmp[2]);
                    img.SetColor(point_tmp[0], point_tmp[1], color);
                }
            }
        }
    }

    template <class real_t>
    class Shader
    {
    public:
        virtual ~Shader() {};

        virtual bool VertexShade(std::vector<Vertex<real_t>> &vertex_buffer) = 0;

        virtual bool FragmentShade(std::vector<Vertex<real_t>> &vertex_buffer) = 0;

    };

    template <class real_t>
    class PrintShader : public Shader<real_t>
    {

    public:

        PrintShader(){}
        ~PrintShader(){}
        virtual bool VertexShade(std::vector<Vertex<real_t>> &vertex_buffer)
        {
            for (size_t i = 0; i < vertex_buffer.size(); i++)
            {
                std::cout<<"----"<<i<<"----\n";
                vertex_buffer[i].Print();
            }
            return true;
        }
        virtual bool FragmentShade(std::vector<Vertex<real_t>> &vertex_buffer) override
        {

            return true;
        }
    };

    template <class real_t>
    class FlatShader : public Shader<real_t>
    {

    public:
        // ColorRGB color = {0,0,0};

        FlatShader(){}
        ~FlatShader(){}
        virtual bool VertexShade(std::vector<Vertex<real_t>> &vertex_buffer)
        {
            return true;
        }
        virtual bool FragmentShade(std::vector<Vertex<real_t>> &vertex_buffer) override
        {

            return true;
        }
    };


}