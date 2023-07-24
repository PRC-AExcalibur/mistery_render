#pragma once

#include "image.h"
#include "texture.h"
#include "scene.h"
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


    template <class Color, typename FShader, class real_t = double>
    inline void TriangleDrawFrame(const Vertex<real_t>& vertex0, const Vertex<real_t>& vertex1, const Vertex<real_t>& vertex2, 
                    ZBuffer &zbuffer, Image<Color> &img, const FShader &light_functor, int cut_n = 1)
    {
        auto v0 = vertex0.position;
        auto v1 = vertex1.position;
        auto v2 = vertex2.position;
        std::array<std::array<real_t, 4>, 3> points = std::array<std::array<real_t, 4>, 3> ({(v0), (v1), (v2)});

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

        double cut_step = 1.0/cut_n;
        for (real_t x_pixel = bbox_min[0]; x_pixel <= bbox_max[0]; ++ x_pixel) 
        {
            for (real_t y_pixel = bbox_min[1]; y_pixel <= bbox_max[1]; ++ y_pixel) 
            {
                real_t depth = zbuffer.GetColor(static_cast<int>(x_pixel), static_cast<int>(y_pixel));
                int sample_num = 0;
                m_math::Vector<real_t, 4> color_sample_sum = m_math::Vector<real_t, 4>();
                real_t depth_sample_max = depth;

                for (int i_x = 0; i_x < cut_n; i_x++)
                {
                    real_t x = x_pixel + i_x * cut_step;
                    
                    for (int i_y = 0; i_y < cut_n; i_y++)
                    {
                        real_t y = y_pixel + i_y * cut_step;
                        {
                            m_math::Vector<real_t, 3> bc = Barycentric(points, m_math::Vector<real_t, 3>({x, y, 0}));
                            if (bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0) 
                            {
                                real_t z = points[0][2] * bc[0] + points[1][2] * bc[1] + points[2][2] * bc[2];
                                if (depth < z) 
                                {
                                    m_math::Vector<real_t, 4> color_uv = light_functor.GetColor(vertex0, vertex1, vertex2, bc);

                                    color_sample_sum += color_uv;
                                    depth_sample_max = std::max(z, depth_sample_max);
                                    sample_num++;
                                }
                            }
                        }
                    }
                }
                
                if (sample_num>0)
                {
                    zbuffer.SetColor(static_cast<int>(x_pixel), static_cast<int>(y_pixel), depth_sample_max);
                    img.SetColor(static_cast<int>(x_pixel), static_cast<int>(y_pixel), Color(color_sample_sum / sample_num));
                }
            }
        }
    }

    template <class real_t>
    struct GetTextureColor
    {
        m_math::Vector<real_t, 4> GetColor(const Vertex<real_t> &vertex0, const Vertex<real_t> &vertex1, const Vertex<real_t> &vertex2,
                                            const m_math::Vector<real_t, 3> &bc) const
        {
            double u_tmp = vertex0.texcoord[0] * bc[0] + vertex1.texcoord[0] * bc[1] + vertex2.texcoord[0] * bc[2];
            double v_tmp = vertex0.texcoord[1] * bc[0] + vertex1.texcoord[1] * bc[1] + vertex2.texcoord[1] * bc[2];
            if(vertex0.material->diffuse_tex == nullptr)
            {
                return m_math::Vector<real_t, 4>();
            }
            return texture::Lerp2(vertex0.material->diffuse_tex, u_tmp, v_tmp);
        }
    };

    template <class real_t>
    struct GetPhongColor
    {
        std::vector<Light *> lights;
        m_math::Vector<real_t, 3> pos_v0;
        m_math::Vector<real_t, 3> pos_v1;
        m_math::Vector<real_t, 3> pos_v2;

        m_math::Vector<real_t, 4> GetColor(const Vertex<real_t> &vertex0, const Vertex<real_t> &vertex1, const Vertex<real_t> &vertex2,
                                            const m_math::Vector<real_t, 3> &bc) const
        {
            double u_tmp = vertex0.texcoord[0] * bc[0] + vertex1.texcoord[0] * bc[1] + vertex2.texcoord[0] * bc[2];
            double v_tmp = vertex0.texcoord[1] * bc[0] + vertex1.texcoord[1] * bc[1] + vertex2.texcoord[1] * bc[2];

            m_math::Vector<real_t, 3> diffuse_color = m_math::Vector<real_t, 3>(vertex0.material->diffuse);
            m_math::Vector<real_t, 3> specular_color = m_math::Vector<real_t, 3>(vertex0.material->specular);
            if (vertex0.material->diffuse_tex != nullptr)
            {
                m_math::Vector<real_t, 4> col_tmp = texture::Lerp2(vertex0.material->diffuse_tex, u_tmp, v_tmp);
                diffuse_color = m_math::Vector<real_t, 3>({col_tmp[0], col_tmp[1], col_tmp[2]});
            }
            if (vertex0.material->specular_tex != nullptr)
            {
                m_math::Vector<real_t, 4> col_tmp = texture::Lerp2(vertex0.material->specular_tex, u_tmp, v_tmp);
                specular_color = m_math::Vector<real_t, 3>({col_tmp[0], col_tmp[1], col_tmp[2]});
            }

            m_math::Vector<real_t, 3> position = bc[0] * m_math::Vector<real_t, 3>({vertex0.position[0], vertex0.position[1], vertex0.position[2]}) 
                                            + bc[1] * m_math::Vector<real_t, 3>({vertex1.position[0], vertex1.position[1], vertex1.position[2]}) 
                                            + bc[2] * m_math::Vector<real_t, 3>({vertex2.position[0], vertex2.position[1], vertex2.position[2]});

            m_math::Vector<real_t, 3> normal = bc[0] * m_math::Vector<real_t, 3>({vertex0.normal[0], vertex0.normal[1], vertex0.normal[2]}) 
                                            + bc[1] * m_math::Vector<real_t, 3>({vertex1.normal[0], vertex1.normal[1], vertex1.normal[2]}) 
                                            + bc[2] * m_math::Vector<real_t, 3>({vertex2.normal[0], vertex2.normal[1], vertex2.normal[2]});
            normal.NormalizeInplace();

            m_math::Vector<real_t, 3> res_tmp = m_math::Vector<real_t, 3>();

            for (auto light_i:lights)
            {
                m_math::Vector<real_t, 3> frag_pos = bc[0] * pos_v0 + bc[1] * pos_v1 + bc[2] * pos_v2;
                m_math::Vector<real_t, 3> light_dir = light_i->GetDirection(frag_pos).Normalize();
                m_math::Vector<real_t, 3> view_dir = frag_pos.Normalize();
                m_math::Vector<real_t, 3> half_way_dir = m_math::Vector<real_t, 3>(light_dir + view_dir).Normalize();

                double diff = std::max(light_dir * normal, 0.0);
                double spec = std::pow(std::max(normal * half_way_dir, 0.0), vertex0.material->shininess);

                m_math::Vector<real_t, 3> ambient =  diffuse_color.HadamardProduct(light_i->ambient);
                m_math::Vector<real_t, 3> diffuse =  diff * diffuse_color.HadamardProduct(light_i->diffuse);
                m_math::Vector<real_t, 3> specular =  spec * specular_color.HadamardProduct(light_i->specular);

                res_tmp += ambient + diffuse + specular;
            }

            return m_math::Vector<real_t, 4>({res_tmp[0], res_tmp[1], res_tmp[2], 1});
        }
    };




    template <class real_t, class color_t>
    class Shader
    {
    protected:
        Image<color_t> * img = nullptr;
        Transform camera_transform;
        std::vector<Vertex<real_t>> shader_vertex_buffer = {};
        std::vector<Light *> shader_light_buffer = {};

        ZBuffer zbuffer = ZBuffer(1,1);

    public:
        virtual ~Shader() {};

        virtual void SetImgPtr(Image<color_t> * img_ptr)
        {
            img = img_ptr;
            zbuffer = MakeZBuffer(* (this->img));
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

        void BindLightBuffer(const std::vector<Light *> light_buffer)
        {
            shader_light_buffer = light_buffer;
        }

        void VertexBufferSRT()
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

        void NormalBufferSRT()
        {
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i++)
            {
                if (this->shader_vertex_buffer[i].transform != nullptr)
                {
                    Transform trans_tmp = *(this->shader_vertex_buffer[i].transform);

                    m_math::Vector<real_t, 3> normal({ this->shader_vertex_buffer[i].normal[0], 
                                                    this->shader_vertex_buffer[i].normal[1], 
                                                    this->shader_vertex_buffer[i].normal[2]});
                    auto mat4x4 = trans_tmp.MartrixSRT();
                    m_math::Matrix<real_t, 3, 3> mat3x3 = m_math::Matrix<real_t, 3, 3>({mat4x4[0][0], mat4x4[1][0], mat4x4[2][0], 
                                                                                    mat4x4[0][1], mat4x4[1][1], mat4x4[2][1], 
                                                                                    mat4x4[0][2], mat4x4[1][2], mat4x4[2][2]});
                    normal = m_math::InverseMatrix(mat3x3) * normal;
                    this->shader_vertex_buffer[i].normal = {normal[0], normal[1], normal[2]};
                }
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
        
        FlatShader(color_t color_init)
        {
            color = color_init;
        }
        ~FlatShader(){}

        size_t TriangleFragmentShade(const color_t& color_frag, size_t idx)
        {
            auto v0 = this->shader_vertex_buffer[idx].position;
            auto v1 = this->shader_vertex_buffer[idx + 1].position;
            auto v2 = this->shader_vertex_buffer[idx + 2].position;
            std::array<std::array<real_t, 4>, 3> points = std::array<std::array<real_t, 4>, 3> ({(v0), (v1), (v2)});

            TriangleDraw<std::array<std::array<real_t, 4>, 3>, color_t, real_t>(points, this->zbuffer, *(this->img), color_frag);
            return idx + 3;
        }


        virtual bool VertexShade() override
        {
            this->VertexBufferSRT();
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


    template <class real_t, class color_t>
    class TextureShader : public Shader<real_t, color_t>
    {
    public:
        int ssaa_scale = 1;
        TextureShader(int ssaa_scale_init = 1) : ssaa_scale(ssaa_scale_init)
        {

        }
        ~TextureShader(){}

        size_t TextureTriangleFragmentShade(size_t idx, const GetTextureColor<real_t> &light_func)
        {
            TriangleDrawFrame<color_t, GetTextureColor<real_t>, real_t>(this->shader_vertex_buffer[idx], this->shader_vertex_buffer[idx + 1], 
                                                    this->shader_vertex_buffer[idx + 2], this->zbuffer, *(this->img), light_func, ssaa_scale);
            return idx + 3;
        }

        virtual bool VertexShade() override
        {
            this->VertexBufferSRT();
            return true;
        }

        virtual bool FragmentShade() override
        {
            GetTextureColor<real_t> light_functor;
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i += 3) 
            {
                this->TextureTriangleFragmentShade(i, light_functor);
            }
            return true;
        }

    };


    template <class real_t, class color_t>
    class BlinnPhongShader : public Shader<real_t, color_t>
    {
    protected:
        std::vector<m_math::Vector<real_t, 3>> shader_vertex_buffer_pos = {};

        void PosBufferSRT()
        {
            shader_vertex_buffer_pos.resize(this->shader_vertex_buffer.size());
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i++)
            {
                if (this->shader_vertex_buffer[i].transform != nullptr)
                {
                    Transform trans_tmp = *(this->shader_vertex_buffer[i].transform);

                    m_math::Vector<real_t, 4> pos({ this->shader_vertex_buffer[i].position[0], 
                                                    this->shader_vertex_buffer[i].position[1], 
                                                    this->shader_vertex_buffer[i].position[2], 
                                                    this->shader_vertex_buffer[i].position[3] });
                    pos = trans_tmp.MartrixSRT() * pos;
                    this->shader_vertex_buffer_pos[i] = m_math::Vector<real_t, 3>({pos[0], pos[1], pos[2]});
                }
            }
        }

    public:
        int ssaa_scale = 1;
        BlinnPhongShader(int ssaa_scale_init = 1) : ssaa_scale(ssaa_scale_init)
        {

        }
        ~BlinnPhongShader(){}

        size_t BlinnPhongFragmentShade(size_t idx, const GetPhongColor<real_t> &light_func)
        {
            TriangleDrawFrame<color_t, GetPhongColor<real_t>, real_t>(this->shader_vertex_buffer[idx], this->shader_vertex_buffer[idx + 1], 
                                                    this->shader_vertex_buffer[idx + 2], this->zbuffer, *(this->img), light_func, ssaa_scale);
            return idx + 3;
        }

        virtual bool VertexShade() override
        {
            this->PosBufferSRT();
            this->NormalBufferSRT();
            this->VertexBufferSRT();
            return true;
        }

        virtual bool FragmentShade() override
        {
            GetPhongColor<real_t> light_functor;
            light_functor.lights = this->shader_light_buffer;
            for (size_t i = 0; i < this->shader_vertex_buffer.size(); i += 3) 
            {
                light_functor.pos_v0 = shader_vertex_buffer_pos[i];
                light_functor.pos_v1 = shader_vertex_buffer_pos[i+1];
                light_functor.pos_v2 = shader_vertex_buffer_pos[i+2];

                this->BlinnPhongFragmentShade(i, light_functor);
            }
            return true;
        }

    };



template <class color_t>
class CameraRender
{
    friend Shader<double, color_t>;
private:
    std::shared_ptr<Shader<double, color_t>> shader = nullptr;
    std::vector<Vertex<double>> vert_buf;
    std::vector<Light *> light_buf;

public:
    Image<color_t> * img;
    Camera * camera;

    CameraRender(Image<color_t> * img_ptr, Camera * cma)
    {
        vert_buf.reserve(100000);
        img = img_ptr;
        camera = cma;
    }

    void SetShader(std::shared_ptr<Shader<double, color_t>> shader_ptr)
    {
        shader = shader_ptr;
        shader->SetImgPtr(img);
    }

    void ClearVertexBuffer()
    {
        vert_buf.clear();
    }

    void PushVertexBuffer(const std::vector<Vertex<double>> &vert_list, Transform * trans)
    {
        for (size_t i = 0; i < vert_list.size(); i++)
        {
            Vertex<double> vert_tmp = vert_list[i];
            vert_tmp.transform = trans;
            vert_buf.emplace_back(vert_tmp);
        }
    }
    void PushVertexBuffer(const Vertex<double> &vert)
    {
        vert_buf.emplace_back(vert);
    }

    void UpdateFromScene(Scene & scene)
    {
        for (size_t i = 0; i < scene.meshes.size(); i++)
        {
            PushVertexBuffer(scene.meshes[i]->GetVertexList(), &(scene.meshes[i]->transform_origin));
        }
        light_buf = scene.lights;
    }

    void Render()
    {
        if (camera == nullptr)
        {
            return;
        }
        
        shader->UpdateCameraTransform(camera->transform_origin);
        shader->BindVertexBuffer(vert_buf);
        shader->BindLightBuffer(light_buf);
        shader->VertexShade();
        shader->FragmentShade();
    }
};

}