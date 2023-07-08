#pragma once

#include <memory>
#include <array>
#include <iostream>
#include <map>

#include "srt.h"

namespace mistery_render
{

template<class Contain>
inline void PrintContain(Contain cont)
{
    for (size_t i = 0; i < cont.size(); i++)
    {
        std::cout<< cont[i] <<" ";
    }
    std::cout<<"\n";
}

/**
 * @brief Base material, PBR extension
 * @tparam real_t type of real_number in Material
 */
template <class real_t>
struct Material
{
    std::string name;

    std::array<real_t, 3> ambient;
    std::array<real_t, 3> diffuse;
    std::array<real_t, 3> specular;
    std::array<real_t, 3> transmittance;
    std::array<real_t, 3> emission;
    real_t shininess;
    real_t ior;       // index of refraction
    real_t dissolve;  // 1 == opaque; 0 == fully transparent

    int illum;

    int dummy;  // Suppress padding warning.
    
    std::vector<std::vector<std::array<real_t, 4>>> * ambient_tex = nullptr;             // map_Ka
    std::vector<std::vector<std::array<real_t, 4>>> * diffuse_tex = nullptr;             // map_Kd
    std::vector<std::vector<std::array<real_t, 4>>> * specular_tex = nullptr;            // map_Ks
    std::vector<std::vector<std::array<real_t, 4>>> * specular_highlight_tex = nullptr;  // map_Ns
    std::vector<std::vector<std::array<real_t, 4>>> * bump_tex = nullptr;                // map_bump, map_Bump, bump
    std::vector<std::vector<std::array<real_t, 4>>> * displacement_tex = nullptr;        // disp
    std::vector<std::vector<std::array<real_t, 4>>> * alpha_tex = nullptr;               // map_d
    std::vector<std::vector<std::array<real_t, 4>>> * reflection_tex = nullptr;          // refl

    // PBR extension
    real_t roughness;            // [0, 1] default 0
    real_t metallic;             // [0, 1] default 0
    real_t sheen;                // [0, 1] default 0
    real_t clearcoat_thickness;  // [0, 1] default 0
    real_t clearcoat_roughness;  // [0, 1] default 0
    real_t anisotropy;           // aniso. [0, 1] default 0
    real_t anisotropy_rotation;  // anisor. [0, 1] default 0
    real_t pad0;

    std::vector<std::vector<std::array<real_t, 4>>> * roughness_tex = nullptr;  // map_Pr
    std::vector<std::vector<std::array<real_t, 4>>> * metallic_tex = nullptr;   // map_Pm
    std::vector<std::vector<std::array<real_t, 4>>> * sheen_tex = nullptr;      // map_Ps
    std::vector<std::vector<std::array<real_t, 4>>> * emissive_tex = nullptr;   // map_Ke
    std::vector<std::vector<std::array<real_t, 4>>> * normal_tex = nullptr;     // norm. For normal mapping.
};

template <class real_t, size_t size_n>
struct TexturePool
{
private:
    std::map<std::string, size_t> tex_map;
    std::array<std::vector<std::vector<std::array<real_t, 4>>>, size_n> tex_array;
public:
    bool InsertTexture(const std::string &name, const std::vector<std::vector<std::array<real_t, 4>>> &textureData)
    {
        // if (tex_map.find(name) != tex_map.end())
        // {
        //     return false;
        // }

        for (size_t i = 0; i < tex_array.size(); ++i)
        {
            if (tex_array[i].empty())
            {
                tex_array[i] = textureData;
                tex_map[name] = i;
                return true;
            }
        }
        return false;
    }

    bool DeleteTexture(const std::string &name)
    {
        auto it = tex_map.find(name);
        if (it == tex_map.end())
        {
            return false;
        }
        tex_array[it->second].resize(0);
        return true;
    }

    std::vector<std::vector<std::array<real_t, 4>>> *  GetTexture(const std::string &name)
    {
        auto it = tex_map.find(name);
        if (it == tex_map.end())
        {
            return nullptr;
        }
        return &(tex_array[it->second]);
    }
};


/**
 * @brief Transform struct
 */
struct Transform
{
    m_math::Vector3d trans = m_math::Vector3d({0,0,0});
    m_math::Vector3d rot = m_math::Vector3d({0,0,0});
    m_math::Vector3d scal = m_math::Vector3d({1,1,1});

    inline m_math::Matrix4d MartrixSRT() const
    {
        return srt::TranslateMatrix(trans[0], trans[1], trans[2]) * \
                srt::RotateXMatrix(rot[0]) * srt::RotateYMatrix(rot[1]) * srt::RotateZMatrix(rot[2]) * \
                srt::ScaleMatrix(scal[0], scal[1], scal[2]);
    }

    inline m_math::Vector4d Trans(m_math::Vector4d vec) const
    {
        return MartrixSRT() * vec;
    }

};


/**
 * @brief Base Vertex linked Material
 * @tparam real_t type of real_number in Material
 */
template <class real_t>
struct Vertex
{
    std::array<real_t, 4> position = {0,0,0,0};
    std::array<real_t, 3> normal = {0,0,0};
    std::array<real_t, 2> texcoord = {0,0};
    Material<real_t> * material = nullptr;

    Transform * transform = nullptr;

    Vertex()
    {

    }

    Vertex(const std::array<real_t, 4> &pos, const std::array<real_t, 3> &n, 
            const std::array<real_t, 2> &texc, Material<real_t> * mat) : 
            position(pos), normal(n), texcoord(texc), material(mat)
    {
        
    }

    inline void Print() const
    {
        PrintContain(position);
        PrintContain(normal);
        PrintContain(texcoord);
    }


};


}