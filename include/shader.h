#pragma once

#include "image.h"
#include <random>

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
     * @tparam PointsContainer The container of 3 points, size of container must >=3, and size of vector must >=2
     * @tparam real_t The type of real number in vector
     * @param points The points of triangle
     * @param target_point The point for which the barycentric coordinates are to be calculated
     * @return barycentric coordinates
     * @attention If the target_point is outside the triangle, returns {-1,1,1}
     */
    template <class PointsContainer, class real_t>
    inline m_math::Vector<real_t, 3> Barycentric(const PointsContainer& points, m_math::Vector<real_t, 3> target_point) 
    {
        // m_math::Vector3d ab = points[1] - points[0];
        // m_math::Vector3d ac = points[2] - points[0];
        // m_math::Vector3d pa = points[0] - target_point;

        m_math::Vector<real_t, 3> ux ({points[1][0] - points[0][0], points[2][0] - points[0][0], points[0][0] - target_point[0]});
        m_math::Vector<real_t, 3> uy ({points[1][1] - points[0][1], points[2][1] - points[0][1], points[0][1] - target_point[1]});

        m_math::Vector<real_t, 3> u = m_math::Cross(ux, uy);

        if (std::abs(u[2])>m_math::kDoubleAsZero) 
        {
            return m_math::Vector<real_t, 3>({1.0-(u[0]+u[1])/u[2], u[0]/u[2], u[1]/u[2]}); 
        }
        return m_math::Vector<real_t, 3>({-1,1,1});
    } 


    /**
     * @brief Draw a triangle on zbuffer_image with a specific color
     * @tparam PointsContainer The container of 3 points, size of container must >=3, and size of vector must >=3
     * @tparam real_t The type of real number in vector
     * @tparam Color The type of color in image
     * @param points The points (x,y,z) of the triangle
     * @param img A reference to the image on which the line will be drawn
     * @param color The color that will be used to draw the line
     */
    template <class PointsContainer, class Color, class real_t = double>
    inline void TriangleDraw(const PointsContainer& points, ZBuffer &zbuffer, Image<Color> &img, Color color)
    {
        m_math::Vector<real_t, 2> img_size({(real_t)img.GetWidth() - 1, (real_t)img.GetHeight() - 1});
        m_math::Vector<real_t, 2> bbox_min({ std::numeric_limits<real_t>::max(),  std::numeric_limits<real_t>::max()});
        m_math::Vector<real_t, 2> bbox_max({-std::numeric_limits<real_t>::max(), -std::numeric_limits<real_t>::max()});

        for (size_t i=0; i<3; i++) 
        {
            for (size_t j=0; j<2; j++) 
            {
                bbox_min[j] = std::max(0.0, std::min(bbox_min[j], points[i][j]));
                bbox_max[j] = std::min(img_size[j], std::max(bbox_max[j], points[i][j]));
            }
        }

        for (real_t x = bbox_min[0]; x <= bbox_max[0]; ++x) 
        {
            for (real_t y = bbox_min[1]; y <= bbox_max[1]; ++y) 
            {
                m_math::Vector<real_t, 3> bc = Barycentric(points, m_math::Vector<real_t, 3>({x, y, 0}));
                if (bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0) 
                {
                    real_t z = points[0][2] * bc[0] + points[1][2] * bc[1] + points[2][2] * bc[2];
                    if (zbuffer.GetColor(static_cast<int>(x), static_cast<int>(y)) < (z + m_math::kDoubleAsZero)) 
                    {
                        zbuffer.SetColor(static_cast<int>(x), static_cast<int>(y), z);
                        img.SetColor(static_cast<int>(x), static_cast<int>(y), color);
                    }
                }
            }
        }
    }


    template <class real_t, class color_t>
    class Shader
    {
    protected:
        Image<color_t> * img = nullptr;
        Transform camera_transform;
        std::vector<Vertex<real_t>> shader_vertex_buffer = {};

    public:
        virtual ~Shader() {};

        virtual void SetImgPtr(Image<color_t> * img_ptr)
        {
            img = img_ptr;
        }

        void UpdateCameraTransform(const Transform & trans)
        {
            camera_transform = trans;
        }

        void BindVertexBuffer(const std::vector<Vertex<real_t>> &vertex_buffer)
        {
            shader_vertex_buffer.resize(vertex_buffer.size());
            for (size_t i = 0; i < vertex_buffer.size(); i++)
            {
                shader_vertex_buffer[i] = vertex_buffer[i];
            }
        }

        virtual bool VertexShade() = 0;

        virtual bool FragmentShade() = 0;

    };

    template <class real_t, class color_t>
    class PrintShader : public Shader<real_t, color_t>
    {
    public:
        PrintShader(){}
        ~PrintShader(){}
        virtual bool VertexShade() override
        {
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i++)
            {
                std::cout<<"---- vertex: "<<i + 1<<"----\n";
                this->shader_vertex_buffer[i].Print();
            }
            return true;
        }
        virtual bool FragmentShade() override
        {
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i = i + 3)
            {
                std::cout<<"---- face: "<<i / 3 + 1<<"----\n";
                this->shader_vertex_buffer[i].Print();
                this->shader_vertex_buffer[i+1].Print();
                this->shader_vertex_buffer[i+2].Print();
            }
            return true;
        }
    };

    template <class real_t, class color_t>
    class FlatShader : public Shader<real_t, color_t>
    {

    public:
        color_t color;
        ZBuffer zbuffer = ZBuffer(1,1);
        
        FlatShader(color_t color_init)
        {
            color = color_init;
        }
        ~FlatShader(){}

        virtual void SetImgPtr(Image<color_t> * img_ptr) override
        {
            this->img = img_ptr;
            zbuffer = MakeZBuffer(* (this->img));
        }

        void VertexBfferSRT()
        {
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i++)
            {
                if (this->shader_vertex_buffer[i].transform != nullptr)
                {
                    Transform trans_tmp = *(this->shader_vertex_buffer[i].transform);
                    trans_tmp.trans -= this->camera_transform.trans;
                    trans_tmp.rot -= this->camera_transform.rot;
                    trans_tmp.scal = trans_tmp.scal.HadamardProduct(this->camera_transform.scal);

                    m_math::Vector<real_t, 4> pos({ this->shader_vertex_buffer[i].position[0], 
                                                    this->shader_vertex_buffer[i].position[1], 
                                                    this->shader_vertex_buffer[i].position[2], 
                                                    this->shader_vertex_buffer[i].position[3] });
                    pos = trans_tmp.MartrixSRT() * pos;
                    this->shader_vertex_buffer[i].position = {pos[0], pos[1], pos[2], pos[3]};
                }
            }
        }

        size_t TriangleFragmentShade(const color_t& color_frag, size_t idx)
        {
            auto v0 = this->shader_vertex_buffer[idx].position;
            auto v1 = this->shader_vertex_buffer[idx + 1].position;
            auto v2 = this->shader_vertex_buffer[idx + 2].position;
            std::array<std::array<real_t, 4>, 3> points = std::array<std::array<real_t, 4>, 3> ({(v0), (v1), (v2)});

            TriangleDraw<std::array<std::array<real_t, 4>, 3>, color_t, real_t>(points, zbuffer, *(this->img), color_frag);
            return idx + 3;
        }


        virtual bool VertexShade() override
        {
            this->VertexBfferSRT();
            return true;
        }

        virtual bool FragmentShade() override
        {
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i += 3) 
            {
                this->TriangleFragmentShade(color, i);
            }
            return true;
        }
    };

    template <class real_t, class color_t>
    class RandomFlatShader : public FlatShader<real_t, color_t>
    {
    public:
        std::mt19937 rand_gen;
        std::uniform_real_distribution<double> color_range;
        RandomFlatShader(color_t color_init) : FlatShader<real_t, color_t>(color_init), rand_gen(std::random_device{}()), color_range(0.0, 1.0)
        {

        }
        ~RandomFlatShader(){}

        virtual bool FragmentShade() override
        {
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i += 3) 
            {
                color_t color_rand = color_t();
                for (size_t i = 0; i < 3; i++)
                {
                    color_rand[i] = color_range(rand_gen);
                }
                this->TriangleFragmentShade(color_rand, i);
            }
            return true;
        }

    };



}