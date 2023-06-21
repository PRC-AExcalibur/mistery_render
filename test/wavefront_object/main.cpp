
#include "../test.h"

using namespace mistery_render;


void test_obj()
{
    std::shared_ptr <tinyobj::ObjReader> obj_reader(new tinyobj::ObjReader());

    std::shared_ptr <std::vector<Material<double>>> material_pool(new std::vector<Material<double>>());
    std::shared_ptr <std::vector<ModelObj<double>>> model_pool(new std::vector<ModelObj<double>>());

    std::string err = load_obj<double>("../model/cubic/cubic.obj", obj_reader, material_pool, model_pool);
    if (err.size() != 0)
    {
        std::cout << err << "\n";
        return;
    }
    std::cout << "load success\n";

    TestExpect(material_pool->size(), (size_t)1, "mats");
    TestExpect(model_pool->size(), (size_t)1, "models");

    std::vector<Vertex<double>> vert_buf;

    for (size_t i = 0; i < model_pool->size(); i++)
    {
        model_pool->at(i).PushVertexBuffer(vert_buf);
    }
    TestExpect(vert_buf.size(), (size_t)36, "vertex_buf size");

}


void test_scene()
{
    std::shared_ptr <tinyobj::ObjReader> obj_reader(new tinyobj::ObjReader());

    std::shared_ptr <std::vector<Material<double>>> material_pool(new std::vector<Material<double>>());
    std::shared_ptr <std::vector<ModelObj<double>>> model_pool(new std::vector<ModelObj<double>>());

    std::string err = load_obj("../model/cubic/cubic.obj", obj_reader, material_pool, model_pool);
    if (err.size() != 0)
    {
        std::cout << err << "\n";
        return;
    }
    std::cout << "load success\n";

    std::vector<Vertex<double>> vert_buf;

    for (size_t i = 0; i < model_pool->size(); i++)
    {
        model_pool->at(i).PushVertexBuffer(vert_buf);
    }

    Scene scene_test = Scene();
    scene_test.meshes.emplace_back(vert_buf);

    TestExpect(scene_test.meshes.size(), (size_t)1, "meshes size");

    std::shared_ptr<PrintShader<double>> print_shader(new PrintShader<double>());
    Camera cma;
    cma.SetShader(print_shader);
    cma.UpdateVertexBufferFromScene(scene_test);
    cma.Render();

}


int main(int argc, char *argv[])
{
    test_obj();
    //test_scene();
}
