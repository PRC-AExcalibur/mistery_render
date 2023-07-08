# include "../test.h"

using namespace mistery_render::m_math;


void additionTest() 
{
    Vector3d vec1({1.0, 2.0, 3.0});
    Vector3d vec2({9.0, 8.0, 7.0});
    Vector3d vec_expect({10, 10, 10});

    Vector3d sum = vec1 + vec2;
    mistery_render::TestExpect(sum, vec_expect, "Addition Test op+");

    vec1 += vec2;
    mistery_render::TestExpect(vec1, vec_expect, "Addition & Test op+=");
}

void subtractionTest() 
{
    Vector3d vec1({1.0, 2.0, 3.0});
    Vector3d vec2({9.0, 8.0, 7.0});
    Vector3d vec_expect({-8, -6, -4});

    Vector3d diff = vec1 - vec2;
    mistery_render::TestExpect(diff, vec_expect, "Subtraction Test op-");

    vec1 -= vec2;
    mistery_render::TestExpect(vec1, vec_expect, "Subtraction & Test op-=");

}

void scalarMultiplicationTest() 
{
    Vector3d vec({1.0, 2.0, 3.0});
    double s = 2.0;
    Vector3d vec_expect({2.0, 4.0, 6.0});

    Vector3d scaled = s * vec;
    mistery_render::TestExpect(scaled, vec_expect, "Scalar Multiplication Test (left) op*");

    scaled = vec * s;
    mistery_render::TestExpect(scaled, vec_expect, "Scalar Multiplication Test (right) op*");

    vec *= s;
    mistery_render::TestExpect(vec, vec_expect, "Scalar Multiplication & Test  op*=");
}



void dotProductTest() 
{  
    Vector3d vec1({1.0, 2.0, 3.0});
    Vector3d vec2({4.0, 5.0, 6.0});
    double expectedDot = 32.0;

    double dot = vec1*(vec2);
    mistery_render::TestExpect(dot, expectedDot, "Dot Product Test op*");
    dot = Dot(vec1, vec2);
    mistery_render::TestExpect(dot, expectedDot, "Dot Product Test");
}  
  
void crossProductTest() 
{  
    Vector3d vec1({1.0, 2.0, 3.0});  
    Vector3d vec2({4.0, 5.0, 6.0});  
    Vector3d vec_expect({-3.0, 6.0, -3.0});
  
    Vector3d cross = Cross(vec1, vec2);
    mistery_render::TestExpect(cross, vec_expect, "Cross Product Test");
}  
  
void normTest() 
{  
    Vector3d vec({3.0, 4.0, 0.0});
    double expectedNorm = 5.0;
  
    double norm = vec.Norm();
    mistery_render::TestExpect(norm, expectedNorm, "Norm Test");

    Vector3d vec_expect({0.6, 0.8, 0});
    Vector3d normalized = vec.Normalize(); 
    mistery_render::TestExpect(normalized, vec_expect, "Normalize Test");

    vec.NormalizeInplace();
    mistery_render::TestExpect(vec, vec_expect, "NormalizeInplace Test");
}  

void IsZeroTest()   
{  
    Vector3d zeroVec({0.5*kDoubleAsZero, 0.0, 0.0});
    Vector3d nonZeroVec({1.0, 2.0, 3.0});
  
    mistery_render::TestExpect(zeroVec.IsZero(), true, "IsZero Test for zero vector");
    mistery_render::TestExpect(nonZeroVec.IsZero(), false, "IsZero Test for non-zero vector");
}  
  
void IsOrthogonalTest()   
{  
    Vector3d vec1({1.0, 0.0, 0.0});  
    Vector3d vec2({0.0, 1.0, 0.0});  
    Vector3d vec3({1.0, 1.0, 1.0});  
  
    mistery_render::TestExpect(vec1.IsOrthogonal(vec2), true, "IsOrthogonal Test for orthogonal vectors");
    mistery_render::TestExpect(vec1.IsOrthogonal(vec3), false, "IsOrthogonal Test for non-orthogonal vectors");
}  
  
void IsParallelTest()   
{  
    Vector3d vec1({1.0, 2.0, 3.0});  
    Vector3d vec2({2.0, 4.0, 6.0});  
    Vector3d vec3({7.0, 5.0, 3.0});  
  
    mistery_render::TestExpect(vec1.IsParallel(vec2), true, "IsParallel Test for parallel vectors");
    mistery_render::TestExpect(vec1.IsParallel(vec3), false, "IsParallel Test for non-parallel vectors");
}

int main() 
{
    additionTest();
    subtractionTest();
    scalarMultiplicationTest();

    dotProductTest();
    crossProductTest();
    normTest();

    IsZeroTest();
    IsOrthogonalTest();
    IsParallelTest();
    return 0;
}