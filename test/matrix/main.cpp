# include "../test.h"

using namespace mistery_render::m_math;


void additionTest() 
{
    Matrix3d mat1({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    Matrix3d mat2({9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0});
    Matrix3d mat_expect({10, 10, 10, 10, 10, 10, 10, 10, 10});

    Matrix3d sum = mat1 + mat2;
    TestExpect(sum, mat_expect, "Addition Test op+");

    mat1 += mat2;
    TestExpect(mat1, mat_expect, "Addition & Test op+=");
}

void subtractionTest() 
{
    Matrix3d mat1({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    Matrix3d mat2({9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0});
    Matrix3d mat_expect({-8, -6, -4, -2, 0, 2, 4, 6, 8});

    Matrix3d diff = mat1 - mat2;
    TestExpect(diff, mat_expect, "Subtraction Test op-");

    mat1 -= mat2;
    TestExpect(mat1, mat_expect, "Subtraction & Test op-=");
}

void scalarMultiplicationTest() 
{
    Matrix3d mat({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    double s = 2.0;
    Matrix3d mat_expect({2.0, 4.0, 6.0, 8.0, 10.0, 12, 14, 16, 18});

    Matrix3d scaled = s * mat;
    TestExpect(scaled, mat_expect, "Scalar Multiplication Test (left) op*");

    scaled = mat * s;
    TestExpect(scaled, mat_expect, "Scalar Multiplication Test (right) op*");

    mat *= s;
    TestExpect(mat, mat_expect, "Scalar Multiplication & Test  op*=");
}

void scalarDivideTest() 
{
    Matrix3d mat({1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0});
    double s = 0.5;
    Matrix3d mat_expect({2.0, 4.0, 6.0, 8.0, 10.0, 12, 14, 16, 18});

    Matrix3d scaled = mat / s;
    TestExpect(scaled, mat_expect, "Scalar Divide Test op/");

    mat /= s;
    TestExpect(mat, mat_expect, "Scalar Divide & Test op/=");
}

void matrixMultiplicationTest() 
{

    Matrix_3x4d mat3x4d ({
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0
    });

    Matrix_4x3d mat4x3d ({
        1.0, 5.0, 9.0,
        2.0, 6.0, 10.0,
        3.0, 7.0, 11.0,
        4.0, 8.0, 12.0
    });

    Matrix3d mat_expect({
        30.000000, 70.000000, 110.000000, 
        70.000000, 174.000000, 278.000000, 
        110.000000, 278.000000, 446.000000 
    });

    Matrix3d mat = mat3x4d * mat4x3d;
    TestExpect(mat, mat_expect, "Matrix Multiplication Test op*");
}


void matrixTranspose()
{
    Matrix_3x4d mat3x4d ({
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0
    });

    Matrix_4x3d mat4x3d ({
        1.0, 5.0, 9.0,
        2.0, 6.0, 10.0,
        3.0, 7.0, 11.0,
        4.0, 8.0, 12.0
    });
    TestExpect(mat4x3d.Transpose(), mat3x4d, "Matrix Transpose Test");
}


int main() 
{
    additionTest();
    subtractionTest();
    scalarMultiplicationTest();
    scalarDivideTest();
    matrixMultiplicationTest();
    matrixTranspose();
    return 0;
}