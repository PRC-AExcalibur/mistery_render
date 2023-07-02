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
private:

};


class Scene : public Actor
{
private:

public:
    std::vector<Mesh> meshes;
    std::vector<Light> lights;
    std::vector<Camera> cameras;

    Scene()
    {

    }

};


class CameraRender
{
    friend Shader<double, ColorRGB_d>;
private:
    std::shared_ptr<Shader<double, ColorRGB_d>> shader = nullptr;
    std::vector<Vertex<double>> vert_buf;

public:
    Image<ColorRGB_d> * img;
    Camera * camera;

    CameraRender(Image<ColorRGB_d> * img_ptr, Camera * cma)
    {
        vert_buf.reserve(100000);
        img = img_ptr;
        camera = cma;
    }

    void SetShader(std::shared_ptr<Shader<double, ColorRGB_d>> shader_ptr)
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
            PushVertexBuffer(scene.meshes[i].GetVertexList(), &(scene.meshes[i].transform_origin));
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