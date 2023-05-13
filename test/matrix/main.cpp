# include "../../include/mistery_render.h"

using namespace mistery_render::m_math;

template<class Tmatrix>
std::string get_string(Tmatrix m)
{
    std::string res;
    for (auto i : m.get_elements())
    {
        for (auto j : i)
        {
            res = res + std::to_string(j) + " ";
        }
        res += "\n";
    }
    return res;
}


void additionTest() 
{
    matrix3d mat1({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    matrix3d mat2({9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0});
    matrix3d sum = mat1 + mat2;
    std::string sumStr = get_string(sum);
    std::cout << "Addition Test:\n" << sumStr << std::endl;
}

void subtractionTest() 
{
    matrix3d mat1({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    matrix3d mat2({9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0});
    matrix3d difference = mat1 - mat2;
    std::string differenceStr = get_string(difference);
    std::cout << "Subtraction Test:\n" << differenceStr << std::endl;
}

void scalarMultiplicationTest() 
{
    matrix3d mat({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    double s = 2.0;
    matrix3d scaled = s * mat;
    std::string scaledStr = get_string(scaled);
    std::cout << "Scalar Multiplication Test (left):\n" << scaledStr << std::endl;

    scaled = mat * s;
    std::string scaledStr2 = get_string(scaled);
    std::cout << "Scalar Multiplication Test (right):\n" << scaledStr2 << std::endl;
}

void matrixMultiplicationTest() {
    // 定义一个 3x4 矩阵
    matrix_3x4d mat3x4d ({
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0
    });

    // 定义一个 4x3 矩阵
    matrix_4x3d mat4x3d ({
        1.0, 5.0, 9.0,
        2.0, 6.0, 10.0,
        3.0, 7.0, 11.0,
        4.0, 8.0, 12.0
    });

    std::cout << "Matrix 3x4:\n" << get_string(mat3x4d) << std::endl;
    std::cout << "Matrix 4x3:\n" << get_string(mat4x3d) << std::endl;

    matrix3d res = mat3x4d * mat4x3d;
    std::cout << "Result of 3x4 * 4x3:\n" << get_string(res) << std::endl;

}



int main() 
{
    additionTest();
    subtractionTest();
    scalarMultiplicationTest();
    matrixMultiplicationTest();
    return 0;
}