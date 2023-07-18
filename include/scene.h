#pragma once

#include <memory>
#include <array>
#include <iostream>

#include "base_data_struct.h"
#include "shader.h"

namespace mistery_render
{

class Actor
{

public:
    Transform transform_origin;
    Actor()
    {

    }
};

class Camera: public Actor
{
private:

};

class Mesh : public Actor
{
private:
    std::vector<Vertex<double>> & vertexs_origin;
public:
    Mesh(std::vector<Vertex<double>> &vert) : Actor(), vertexs_origin(vert)
    {

    }
    inline const std::vector<Vertex<double>> & GetVertexList() const
    {
        return vertexs_origin;
    }
};

class Light : public Actor
{
public:
    m_math::Vector<double, 3> ambient = m_math::Vector<double, 3>({1,1,1});
    m_math::Vector<double, 3> diffuse = m_math::Vector<double, 3>({1,1,1});
    m_math::Vector<double, 3> specular = m_math::Vector<double, 3>({1,1,1});

    Light(const m_math::Vector3d &ambi, const m_math::Vector3d &diff, const m_math::Vector3d &spec) : ambient(ambi), diffuse(diff), specular(spec)
    {
    }
};

class PointLight : public Light
{
public:
    PointLight(const m_math::Vector3d &ambi, const m_math::Vector3d &diff, const m_math::Vector3d &spec) : Light(ambi, diff, spec)
    {
    }
};

class DirectionalLight : public Light
{
public:
    m_math::Vector3d direction;

    DirectionalLight(const m_math::Vector3d &ambi, const m_math::Vector3d &diff, const m_math::Vector3d &spec, 
                const m_math::Vector3d &direction) : Light(ambi, diff, spec), direction(direction)
    {
    }
};

class Scene : public Actor
{
private:

public:
    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    std::vector<Camera *> cameras;

    Scene()
    {

    }

    ~Scene()
    {
        for (size_t i = 0; i < meshes.size(); i++)
        {
            delete meshes[i];
        }
        for (size_t i = 0; i < lights.size(); i++)
        {
            delete lights[i];
        }
        for (size_t i = 0; i < cameras.size(); i++)
        {
            delete cameras[i];
        }
    }

};

template <class color_t>
class CameraRender
{
    friend Shader<double, color_t>;
private:
    std::shared_ptr<Shader<double, color_t>> shader = nullptr;
    std::vector<Vertex<double>> vert_buf;

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

    void UpdateVertexBufferFromScene(Scene & scene)
    {
        for (size_t i = 0; i < scene.meshes.size(); i++)
        {
            PushVertexBuffer(scene.meshes[i]->GetVertexList(), &(scene.meshes[i]->transform_origin));
        }
    }

    void Render()
    {
        if (camera == nullptr)
        {
            return;
        }
        
        shader->UpdateCameraTransform(camera->transform_origin);
        shader->BindVertexBuffer(vert_buf);
        shader->VertexShade();
        shader->FragmentShade();
    }
};

}