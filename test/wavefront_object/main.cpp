
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

template<class shader_t>
void test_scene(std::shared_ptr<shader_t> shader, const std::string& path)
{
    std::shared_ptr <tinyobj::ObjReader> obj_reader(new tinyobj::ObjReader());

    std::shared_ptr <std::vector<Material<double>>> material_pool(new std::vector<Material<double>>());
    std::shared_ptr <std::vector<ModelObj<double>>> model_pool(new std::vector<ModelObj<double>>());

    std::string err = load_obj(path, obj_reader, material_pool, model_pool);

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

    // cubic settings
    // scene_test.meshes[0].transform_origin.trans = m_math::Vector3d({200,150,0});
    // scene_test.meshes[0].transform_origin.rot = m_math::Vector3d({1.57,1.57,0})*0.5;
    // scene_test.meshes[0].transform_origin.scal = m_math::Vector3d({75,50,50});

    scene_test.meshes[0].transform_origin.trans = m_math::Vector3d({400,550,0});
    scene_test.meshes[0].transform_origin.rot = m_math::Vector3d({0,0,3.14});
    scene_test.meshes[0].transform_origin.scal = m_math::Vector3d({1,1,1})*300;

    // std::shared_ptr<shader_t> shader(new shader_t());
    Image<ColorRGB_d> res_img(800, 600);

    Camera cma0;
    cma0.transform_origin.scal = m_math::Vector3d({1,1,1});

    CameraRender cma(&res_img, &cma0);
    cma.SetShader(shader);
    cma.UpdateVertexBufferFromScene(scene_test);
    cma.Render();

    std::ofstream file_render("output/test/render_test.ppm");
    Save2ppm(res_img, file_render);
    file_render.close();

    TestExpect(" ", " ", "Scene Render Test");
}


int main(int argc, char *argv[])
{
    test_obj();

    // std::shared_ptr<PrintShader<double, ColorRGB_d>> print_shader(new PrintShader<double, ColorRGB_d>());
    // test_scene<PrintShader<double, ColorRGB_d>>(print_shader, "../model/cubic/cubic.obj");

    // std::shared_ptr<FlatShader<double, ColorRGB_d>> flat_shader(new FlatShader<double, ColorRGB_d>(ColorRGB_d(1,1,1)));
    // test_scene<FlatShader<double, ColorRGB_d>>(flat_shader, "../model/cubic/cubic.obj");

    // std::shared_ptr<RandomFlatShader<double, ColorRGB_d>> flat_shader(new RandomFlatShader<double, ColorRGB_d>(ColorRGB_d(1,1,1)));
    // test_scene<RandomFlatShader<double, ColorRGB_d>>(flat_shader, "../model/cubic/cubic.obj");

    std::shared_ptr<RandomFlatShader<double, ColorRGB_d>> flat_shader(new RandomFlatShader<double, ColorRGB_d>(ColorRGB_d(1,1,1)));
    test_scene<RandomFlatShader<double, ColorRGB_d>>(flat_shader, "../model/keqing/keqing_from_fbx.obj");
}
