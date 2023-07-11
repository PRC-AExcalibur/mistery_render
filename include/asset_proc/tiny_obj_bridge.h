#pragma once

#include <memory>
#include <array>

#include "tiny_obj_loader.h"
#include "tga_image_bridge.h"
#include "../base_data_struct.h"
#include "../test.h"

namespace mistery_render
{

template <class real_t>
std::vector<std::vector<std::array<real_t, 4>>> ParseTextureTGA(std::string tex_name)
{
    TGAImage tga;
    if (!tga.read_tga_file(tex_name))
    {
        std::cerr << "Error reading TGA file: " << tex_name << std::endl;
        return {{}};
    }
    tga.flip_vertically();

    std::vector<std::vector<std::array<real_t, 4>>> texture(tga.height(), std::vector<std::array<real_t, 4>>(tga.width()));
    for (int y = 0; y < tga.height(); ++y)
    {
        for (int x = 0; x < tga.width(); ++x)
        {
            TGAColor pixel = tga.get(x, y);
            texture[y][x] = {static_cast<real_t>(pixel[2]) / 255.0f, static_cast<real_t>(pixel[1]) / 255.0f, 
                            static_cast<real_t>(pixel[0]) / 255.0f, static_cast<real_t>(pixel[3]) / 255.0f};
        }
    }
    return texture;
}

template <class real_t, size_t tex_n>
std::vector<std::vector<std::array<real_t, 4>>> * LoadTexture(std::string tex_name, std::shared_ptr<TexturePool<real_t, tex_n>> texture_pool)
{
    if (tex_name.size()==0)
    {
        return nullptr;
    }
    
    size_t dot_position = tex_name.find_last_of('.');
    if (!(dot_position != 0 && dot_position != tex_name.length() - 1))
    {
        return nullptr;
    }
    std::string file_type = tex_name.substr(dot_position + 1);

    if (file_type.compare("tga")==0)
    {
        if (texture_pool->GetTexture(tex_name) == nullptr)
        {
            texture_pool->InsertTexture(tex_name, ParseTextureTGA<real_t>(tex_name));
        }
        return texture_pool->GetTexture(tex_name);
    }

    return nullptr;

}

template <class real_t, size_t tex_n>
inline Material<real_t> MatObjToMaterial(const tinyobj::material_t &srcMat, std::shared_ptr<TexturePool<real_t, tex_n>> texture_pool, const std::string& path = "")
{
    Material<real_t> dstMat;

    // Copy basic properties
    dstMat.name = srcMat.name;

    dstMat.ambient = {srcMat.ambient[0], srcMat.ambient[1], srcMat.ambient[2]};
    dstMat.diffuse = {srcMat.diffuse[0], srcMat.diffuse[1], srcMat.diffuse[2]};
    dstMat.specular = {srcMat.specular[0], srcMat.specular[1], srcMat.specular[2]};
    dstMat.transmittance = {srcMat.transmittance[0], srcMat.transmittance[1], srcMat.transmittance[2]};
    dstMat.emission = {srcMat.emission[0], srcMat.emission[1], srcMat.emission[2]};
    dstMat.shininess = srcMat.shininess;
    dstMat.ior = srcMat.ior;
    dstMat.dissolve = srcMat.dissolve;
    dstMat.illum = srcMat.illum;
    dstMat.dummy = srcMat.dummy;

    // Copy texture names
    dstMat.ambient_tex = LoadTexture<real_t>(path + srcMat.ambient_texname, texture_pool);
    dstMat.diffuse_tex = LoadTexture<real_t>(path + srcMat.diffuse_texname, texture_pool);
    dstMat.specular_tex = LoadTexture<real_t>(path + srcMat.specular_texname, texture_pool);
    dstMat.specular_highlight_tex = LoadTexture<real_t>(path + srcMat.specular_highlight_texname, texture_pool);
    dstMat.bump_tex = LoadTexture<real_t>(path + srcMat.bump_texname, texture_pool);
    dstMat.displacement_tex = LoadTexture<real_t>(path + srcMat.displacement_texname, texture_pool);
    dstMat.alpha_tex = LoadTexture<real_t>(path + srcMat.alpha_texname, texture_pool);
    dstMat.reflection_tex = LoadTexture<real_t>(path + srcMat.reflection_texname, texture_pool);

    // Copy PBR properties
    dstMat.roughness = srcMat.roughness;
    dstMat.metallic = srcMat.metallic;
    dstMat.sheen = srcMat.sheen;
    dstMat.clearcoat_thickness = srcMat.clearcoat_thickness;
    dstMat.clearcoat_roughness = srcMat.clearcoat_roughness;
    dstMat.anisotropy = srcMat.anisotropy;
    dstMat.anisotropy_rotation = srcMat.anisotropy_rotation;
    dstMat.pad0 = srcMat.pad0;

    dstMat.roughness_tex = LoadTexture<real_t>(path + srcMat.roughness_texname, texture_pool);
    dstMat.metallic_tex = LoadTexture<real_t>(path + srcMat.metallic_texname, texture_pool);
    dstMat.sheen_tex = LoadTexture<real_t>(path + srcMat.sheen_texname, texture_pool);
    dstMat.emissive_tex = LoadTexture<real_t>(path + srcMat.emissive_texname, texture_pool);
    dstMat.normal_tex = LoadTexture<real_t>(path + srcMat.normal_texname, texture_pool);

    return dstMat;
}


// only support triangle face
template <class real_t>
class ModelObj
{
private:
    std::shared_ptr<tinyobj::ObjReader> obj_ = nullptr;
    std::shared_ptr<std::vector<Material<real_t>>> mat_ = nullptr;

    int shape_index_ = 0;

    inline Vertex<real_t> GetVertex(int indices_index) const
    {

        const tinyobj::attrib_t &attrib = obj_->GetAttrib();
        const tinyobj::shape_t &shape = obj_->GetShapes()[shape_index_];

        int mat_id = shape.mesh.material_ids[indices_index % 3];
        // const tinyobj::material_t &mat = obj_->GetMaterials()[mat_id];
        // std::cout<<"id "<<mat_id<<"\n";
        Vertex<real_t> res({0, 0, 0, -1.0}, {0, 0, 0}, {0, 0}, &(mat_->at(mat_id)));

        // if (indices_index>=shape.mesh.indices.size() || indices_index<0)
        // {
        //     return res;
        // }
        tinyobj::index_t idx_set = shape.mesh.indices[indices_index];

        if (idx_set.vertex_index >= 0)
        {
            real_t x = attrib.vertices[idx_set.vertex_index * 3 + 0];
            real_t y = attrib.vertices[idx_set.vertex_index * 3 + 1];
            real_t z = attrib.vertices[idx_set.vertex_index * 3 + 2];
            res.position = {x, y, z, 1.0};
        }
        if (idx_set.normal_index >= 0)
        {
            real_t x = attrib.normals[idx_set.normal_index * 3 + 0];
            real_t y = attrib.normals[idx_set.normal_index * 3 + 1];
            real_t z = attrib.normals[idx_set.normal_index * 3 + 2];
            res.normal = {x, y, z};
        }
        if (idx_set.texcoord_index >= 0)
        {
            real_t x = attrib.texcoords[idx_set.texcoord_index * 2 + 0];
            real_t y = attrib.texcoords[idx_set.texcoord_index * 2 + 1];
            res.texcoord = {x, y};
        }
        return res;
    }

public:
    ModelObj(){}
    ModelObj(std::shared_ptr<tinyobj::ObjReader> obj_reader,
             std::shared_ptr<std::vector<Material<real_t>>> material,
             int shape_idx) : obj_(obj_reader), mat_(material), shape_index_(shape_idx)
    {
    }

    inline bool PushVertexBuffer(std::vector<Vertex<real_t>> &vertex_buffer) const
    {
        if (shape_index_ >= (int)(obj_->GetShapes().size()))
        {
            return false;
        }
        int max_idx = obj_->GetShapes()[shape_index_].mesh.indices.size();
        for (int i = 0; i < max_idx; i++)
        {
            vertex_buffer.push_back(GetVertex(i));
        }
        return true;
    }
};

template <class real_t, size_t tex_n>
inline std::string load_obj(const std::string &obj_path,
                            std::shared_ptr<tinyobj::ObjReader> obj_reader,
                            std::shared_ptr<std::vector<Material<real_t>>> material_list,
                            std::shared_ptr<std::vector<ModelObj<real_t>>> model_list, 
                            std::shared_ptr<TexturePool<real_t, tex_n>> texture_pool)
{
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;

    if (obj_reader->ParseFromFile(obj_path, config))
    {
        if (obj_reader->Valid())
        {
            const std::vector<tinyobj::material_t> mats = obj_reader->GetMaterials();
            size_t last_slash_idx = obj_path.find_last_of('/');
            std::string mats_path = (last_slash_idx == std::string::npos) ? "" : obj_path.substr(0, last_slash_idx+1);
            for (size_t i = 0; i < mats.size(); i++)
            {
                material_list->push_back(MatObjToMaterial<double>(mats[i], texture_pool, mats_path));
            }
            for (size_t i = 0; i < obj_reader->GetShapes().size(); i++)
            {
                model_list->push_back(ModelObj(obj_reader, material_list, i));
            }
            return "";
        }
        else
        {
            return "Warn: " + obj_reader->Warning();
            return "Err: " + obj_reader->Error();
        }
    }
    else
    {
        return "Failed to load: " + obj_path;
    }
}

}