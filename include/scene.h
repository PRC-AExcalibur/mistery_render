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

class Camera;


class Mesh : public Actor
{
    friend Camera;
private:
    std::vector<Vertex<double>> & vertexs_origin;
public:
    Mesh(std::vector<Vertex<double>> &vert) : Actor(), vertexs_origin(vert)
    {

    }
};


class Light : public Actor
{
private:

};


class Scene : public Actor
{
    friend Camera;
private:

public:
    std::vector<Mesh> meshes;
    std::vector<Light> lights;

    Scene()
    {

    }
    // void LinkCamera();
    // void AddCamera(Camera camera);

};


class Camera : public Actor
{
private:
    std::shared_ptr<Shader<double>> shader = nullptr;
    std::vector<Vertex<double>> vert_buf;

    std::vector<std::vector<double>> img;

public:
    Camera()
    {
        vert_buf.reserve(100000);

    }

    void SetShader(std::shared_ptr<Shader<double>> shader_ptr)
    {
        shader = shader_ptr;
    }

    void ClearVertexBuffer()
    {
        vert_buf.clear();
    }

    void PushVertexBuffer(const std::vector<Vertex<double>> &vert_list)
    {
        for (size_t i = 0; i < vert_list.size(); i++)
        {
            vert_buf.emplace_back(vert_list[i]);
        }
    }
    void PushVertexBuffer(const Vertex<double> &vert)
    {
        vert_buf.emplace_back(vert);
    }

    void UpdateVertexBufferFromScene(const Scene & scene)
    {
        for (size_t i = 0; i < scene.meshes.size(); i++)
        {
            PushVertexBuffer(scene.meshes[i].vertexs_origin);
        }
    }

    void Render()
    {
        shader->VertexShade(vert_buf);
        shader->FragmentShade(vert_buf);
    }
};

}