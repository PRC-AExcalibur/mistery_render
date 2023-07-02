#pragma once

#include <memory>
#include <array>
#include <iostream>

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
    
    std::vector<std::vector<std::array<real_t, 3>>> ambient_tex;             // map_Ka
    std::vector<std::vector<std::array<real_t, 3>>> diffuse_tex;             // map_Kd
    std::vector<std::vector<std::array<real_t, 3>>> specular_tex;            // map_Ks
    std::vector<std::vector<std::array<real_t, 3>>> specular_highlight_tex;  // map_Ns
    std::vector<std::vector<std::array<real_t, 3>>> bump_tex;                // map_bump, map_Bump, bump
    std::vector<std::vector<std::array<real_t, 3>>> displacement_tex;        // disp
    std::vector<std::vector<std::array<real_t, 3>>> alpha_tex;               // map_d
    std::vector<std::vector<std::array<real_t, 3>>> reflection_tex;          // refl

    // PBR extension
    real_t roughness;            // [0, 1] default 0
    real_t metallic;             // [0, 1] default 0
    real_t sheen;                // [0, 1] default 0
    real_t clearcoat_thickness;  // [0, 1] default 0
    real_t clearcoat_roughness;  // [0, 1] default 0
    real_t anisotropy;           // aniso. [0, 1] default 0
    real_t anisotropy_rotation;  // anisor. [0, 1] default 0
    real_t pad0;

    std::vector<std::vector<std::array<real_t, 3>>> roughness_tex;  // map_Pr
    std::vector<std::vector<std::array<real_t, 3>>> metallic_tex;   // map_Pm
    std::vector<std::vector<std::array<real_t, 3>>> sheen_tex;      // map_Ps
    std::vector<std::vector<std::array<real_t, 3>>> emissive_tex;   // map_Ke
    std::vector<std::vector<std::array<real_t, 3>>> normal_tex;     // norm. For normal mapping.
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