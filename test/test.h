# include "../include/mistery_render.h"


template <class T>
inline bool TestExpect(T source, T expect, std::string test_name)
{
    bool eq = mistery_render::m_math::IsEqual(source, expect);
    if (eq)
    {
        std::cout << test_name << " : -> passed\n";
    }
    else
    {
        std::cout << test_name << " : -> failed\n";
        std::cout <<"    expect:\n" << expect <<"\n";
        std::cout <<"    result:\n" << source <<"\n";
    }
    std::cout<<"------------------------------\n";
    return eq;
}