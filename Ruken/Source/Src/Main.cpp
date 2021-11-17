#include "Core/Kernel.hpp"

#include "Maths/Matrix/Matrix.hpp"

#include <iostream>

USING_RUKEN_NAMESPACE

int main()
{
    auto model = Matrix<4, 4>::TranslationMatrix({1_m, 2_m, 3_m});
    auto view  = Matrix<4, 4>::LookAtMatrix({2_m, 2_m, 2_m}, Constants<Vector3m>::zero, Constants<Vector3m>::up);
    auto proj  = Matrix<4, 4>::PerspectiveProjectionMatrix(45_deg, 16.0F / 9.0F, 0.1_m, 10_m);

    std::cout << model << std::endl << std::endl;
    std::cout << view << std::endl << std::endl;
    std::cout << proj << std::endl << std::endl;

    Kernel kernel;

    return kernel.Run();
}