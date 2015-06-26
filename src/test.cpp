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
    m3.setRandom();
    std::cout << "--BEGIN--\n" << m3 << "\n---END---\n";
    EigenToMat mat("new_score");
    mat.openMatFile("new_score.mat");
    mat.setData<double>(m3, mxDOUBLE_CLASS);
    mat.writeMatFile();

    return 0;
}
