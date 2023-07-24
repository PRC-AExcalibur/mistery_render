#pragma once

#include <memory>
#include <array>
#include <iostream>

#include "base_data_struct.h"

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
    virtual ~Light()
    {
    }

    virtual m_math::Vector<double, 3> GetDirection(const m_math::Vector<double, 3> position) const = 0;
};

class PointLight : public Light
{
public:
    PointLight(const m_math::Vector3d &ambi, const m_math::Vector3d &diff, const m_math::Vector3d &spec) : Light(ambi, diff, spec)
    {
    }
    virtual ~PointLight()
    {
    }
    virtual m_math::Vector<double, 3> GetDirection(const m_math::Vector<double, 3> position) const override
    {
        return m_math::Vector<double, 3>(this->transform_origin.trans - position);
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
    virtual ~DirectionalLight()
    {
    }
    virtual m_math::Vector<double, 3> GetDirection(const m_math::Vector<double, 3> position) const override
    {
        return direction;
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

}