#pragma once

#include <memory>
#include <array>

#include "tiny_obj_loader.h"
#include "tga_image.h"
#include "../base_data_struct.h"

namespace mistery_render
{

template <class real_t>
std::vector<std::vector<std::array<real_t, 3>>> ParseTextureTGA(std::string tex_name)
{
    TGAImage tga;
    if (!tga.read_tga_file(tex_name))
    {
        std::cerr << "Error reading TGA file: " << tex_name << std::endl;
        return {{}}; // 返回空数据表示加载失败
    }

    std::vector<std::vector<std::array<real_t, 3>>> texture(tga.height(), std::vector<std::array<real_t, 3>>(tga.width()));
    for (int y = 0; y < tga.height(); ++y)
    {
        for (int x = 0; x < tga.width(); ++x)
        {
            TGAColor pixel = tga.get(x, y);
            texture[y][x] = {static_cast<real_t>(pixel[2]) / 255.0f, static_cast<real_t>(pixel[1]) / 255.0f, static_cast<real_t>(pixel[0]) / 255.0f}; // 注意: TGA中BGR顺序
        }
    }
    return texture;
}

template <class real_t>
std::vector<std::vector<std::array<real_t, 3>>> LoadTexture(std::string tex_name)
{
    if (tex_name.size()==0)
    {
        return {{}};
    }
    
    size_t dot_position = tex_name.find_last_of('.');
    if (!(dot_position != 0 && dot_position != tex_name.length() - 1))
    {
        return {{}};
    }
    std::string file_type = tex_name.substr(dot_position + 1);

    if (file_type.compare("tga")==0)
    {
        return ParseTextureTGA<real_t>(tex_name);
    }

    return {{}};

}

template <class real_t>
inline Material<real_t> MatObjToMaterial(const tinyobj::material_t &srcMat)
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
    dstMat.ambient_tex = LoadTexture<real_t>(srcMat.ambient_texname);
    dstMat.diffuse_tex = LoadTexture<real_t>(srcMat.diffuse_texname);
    dstMat.specular_tex = LoadTexture<real_t>(srcMat.specular_texname);
    dstMat.specular_highlight_tex = LoadTexture<real_t>(srcMat.specular_highlight_texname);
    dstMat.bump_tex = LoadTexture<real_t>(srcMat.bump_texname);
    dstMat.displacement_tex = LoadTexture<real_t>(srcMat.displacement_texname);
    dstMat.alpha_tex = LoadTexture<real_t>(srcMat.alpha_texname);
    dstMat.reflection_tex = LoadTexture<real_t>(srcMat.reflection_texname);

    // Copy PBR properties
    dstMat.roughness = srcMat.roughness;
    dstMat.metallic = srcMat.metallic;
    dstMat.sheen = srcMat.sheen;
    dstMat.clearcoat_thickness = srcMat.clearcoat_thickness;
    dstMat.clearcoat_roughness = srcMat.clearcoat_roughness;
    dstMat.anisotropy = srcMat.anisotropy;
    dstMat.anisotropy_rotation = srcMat.anisotropy_rotation;
    dstMat.pad0 = srcMat.pad0;

    dstMat.roughness_tex = LoadTexture<real_t>(srcMat.roughness_texname);
    dstMat.metallic_tex = LoadTexture<real_t>(srcMat.metallic_texname);
    dstMat.sheen_tex = LoadTexture<real_t>(srcMat.sheen_texname);
    dstMat.emissive_tex = LoadTexture<real_t>(srcMat.emissive_texname);
    dstMat.normal_tex = LoadTexture<real_t>(srcMat.normal_texname);

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
        Vertex<real_t> res({0, 0, 0, -1.0}, {0, 0, 0}, {0, 0}, mat_->at(mat_id));

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

template <class real_t>
inline std::string load_obj(const std::string &obj_path,
                            std::shared_ptr<tinyobj::ObjReader> obj_reader,
                            std::shared_ptr<std::vector<Material<real_t>>> material_list,
                            std::shared_ptr<std::vector<ModelObj<real_t>>> model_list)
{
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;

    if (obj_reader->ParseFromFile(obj_path, config))
    {
        if (obj_reader->Valid())
        {
            const std::vector<tinyobj::material_t> mats = obj_reader->GetMaterials();
            for (size_t i = 0; i < mats.size(); i++)
            {
                material_list->push_back(MatObjToMaterial<double>(mats[i]));
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