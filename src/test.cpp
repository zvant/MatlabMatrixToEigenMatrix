#include "mat_to_eigen.hpp"

int main(int argc, char * argv[])
{
    MatToEigen matrix;
    // first read a .mat file
    if(argc != 2)
    {
        std::cout << "Specify one .mat file" << std::endl;
        return -1;
    }
    matrix.openMatFile(argv[1]);
    // display array info
    matrix.showArray();
    matrix.showElementsNumeric();
    // template way to do this
    matrix.showElements<float>();

    // convert to Eigen::Matrix
    Eigen::MatrixXf m1;
    m1 = matrix.toMatrixXf(m1);
    std::cout << "--BEGIN--\n" << m1 << "\n---END---\n";

    // template way
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m2;
    m2 = matrix.toMatrix(m2);
    std::cout << "--BEGIN--\n" << m2 << "\n---END---\n";

    return 0;
}
