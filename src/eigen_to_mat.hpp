#ifndef EIGEN_TO_MAT_HPP
#define EIGEN_TO_MAT_HPP

#include "mat_eigen_common.h"

/**
 * Write data into .mat file. Data in Eigen::Matrix
 * Note that elements are stored in column-major order in data
 * @brief Write Eigen::Matrix objects to Matlab .mat files
 */
class EigenToMat
{
private:
    /// name of array
    const char * name;
    mxArray * p_array;
    size_t rows;
    size_t cols;
    size_t size;
    /// file pointer to the output file
    MATFile * p_mat;

public:
    EigenToMat(const char * name_str = "anonymous");
    ~EigenToMat();

    void setName(const char * name_str);
    bool clearData();
    bool openMatFile(const char * file_name);
    bool closeMatFile();
    bool writeMatFile() const;

    /*
     * !!!! Use templates with great caution !!!!
     * Read the comments before using or program might crash
     */
    template <typename T>
    bool setData(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, mxClassID class_id);
};

/**
 * Copy data in Eigen::Matrix into mxArray
 * @remark Make sure typename T matches mxClassID class_id
 * @remark PREREQUIRED IMPLEMENTATION <br>
 * T & operator=(const T &);
 * @brief EigenToMat::setData
 * @return true if success
 */
template <typename T>
bool EigenToMat::setData(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & matrix, mxClassID class_id)
{
    if(NULL != p_array)
    {
        std::cerr << "Data already exists, clearData first" << std::endl;
        return false;
    }
    rows = matrix.rows();
    cols = matrix.cols();
    size = sizeof(T);
    if(0 == rows || 0 == cols)
    {
        std::cerr << "Empty matrix, nothing would be changed" << std::endl;
        return false;
    }

    if(NULL == (p_array = mxCreateNumericMatrix(rows, cols, class_id, mxREAL)))
    {
        std::cerr << "Cannot create mxArray" << std::endl;
        return false;
    }

    T * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (T *)((char *)(mxGetPr(p_array)) + (m + rows * n) * size);
            * p_elem = matrix(m, n);
        }
    }
    return true;
}

#endif // EIGEN_TO_MAT_HPP
