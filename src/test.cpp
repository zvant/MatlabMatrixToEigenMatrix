#include "mat_to_eigen.hpp"
#include "eigen_to_mat.hpp"

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
    matrix.showElements<double>();

    // convert to Eigen::Matrix
    Eigen::MatrixXd m1;
    m1 = matrix.toMatrixXd(m1);
    std::cout << "--BEGIN--\n" << m1 << "\n---END---\n";

    // template way
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> m2;
    m2 = matrix.toMatrix(m2);
    std::cout << "--BEGIN--\n" << m2 << "\n---END---\n";

    // write a .mat file
    Eigen::Matrix<double, 3, 7> m3;
    m3.setZero();
    m3(0, 0) = 1.231;
    m3(1, 1) = 3.122;
    m3(2, 2) = -2.123;
    for(int i = 4; i < 7; i ++)
    {
        m3(0, i) = i / 2.5;
        m3(2, i) = i / 3.0;
    }
    std::cout << "--BEGIN--\n" << m3 << "\n---END---\n";
    EigenToMat mat("new_score");
    mat.openMatFile("new_score.mat");
    mat.setData<double>(m3, mxDOUBLE_CLASS);
    mat.writeMatFile();

    return 0;
}
