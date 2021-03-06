#ifndef MAT_TO_EIGEN_HPP
#define MAT_TO_EIGEN_HPP

#include "mat_eigen_common.h"

/**
 * Read data from .mat file. Then fill data into Eigen::Matrix.
 * Note that elements are stored in column-major order in .mat file data.
 * @brief Matlab .mat files to Eigen::Matrix templates
 */
class MatToEigen
{
private:
    /// name of array
    const char * name;
    /// pointer to data
    mxArray * p_array;
    /// data in array
    void * data;

public:
    MatToEigen();
    ~MatToEigen();

    bool openMatFile(const char * file_name);
    bool clearData();
    void showArray() const;
    void showElementsNumeric() const;
    int getRows() const;
    int getCols() const;
    const char * getName() const;

    Eigen::MatrixXf & toMatrixXf(Eigen::MatrixXf & matrix) const;
    Eigen::MatrixXd & toMatrixXd(Eigen::MatrixXd & matrix) const;
    Eigen::MatrixXi & toMatrixXi(Eigen::MatrixXi & matrix) const;

    /*
     * !!!! Use templates with great caution !!!!
     * Read the comments before using or program might crash
     */
    template <typename T>
    void showElements() const;
    template <typename T>
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & toMatrix(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix) const;
};

/**
 * Print elements in the array in matrix form
 * @remark PREREQUIRED IMPLEMENTATION<br>
 * std::ostream & operator<<(std::ostream &, const T &);
 */
template <typename T>
void MatToEigen::showElements() const
{
    using std::cout;
    using std::endl;
    using std::cerr;

    if(NULL == p_array || NULL == data)
    {
        cerr << "No data read from file yet." << endl;
        return;
    }

    int rows = mxGetM(p_array);
    int cols = mxGetN(p_array);
    mwSize size = mxGetElementSize(p_array);

    void * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (char *)data + (m + rows * n) * size;
            cout << *((T *)p_elem) << ",";
        }
        cout << endl;
    }
}

/**
 * Convert elements in the array into specified type and fill in the matrix
 * @remark Unpreditable things could happen if types do not match
 * @remark PREREQUIRED IMPLEMENTATION<br>
 * T & operator=(const T &);
 * @brief MatToEigen::toMatrix
 * @param matrix
 */
template <typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & MatToEigen::toMatrix(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix) const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return matrix;
    }

    int rows = mxGetM(p_array);
    int cols = mxGetN(p_array);
    if(0 == rows || 0 == cols)
        return matrix;

    mwSize size = mxGetElementSize(p_array);
    if(size != sizeof(T))
    {
        std::cerr << "Size of types do not match" << std::endl;
        return matrix;
    }

    matrix.resize(rows, cols);

    void * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (char *)data + (m + rows * n) * size;
            matrix(m, n) = (T)(*((T *)p_elem));
        }
    }
    return matrix;
}

#endif // MAT_TO_EIGEN_HPP
