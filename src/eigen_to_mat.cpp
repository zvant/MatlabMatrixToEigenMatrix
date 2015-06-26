#include "eigen_to_mat.hpp"

EigenToMat::EigenToMat(const char * name_str)
{
    name = name_str;
    p_array = NULL;
    rows = 0;
    cols = 0;
    size = 0;
    p_mat = NULL;
}

/**
 * Clear data and close file
 * @brief EigenToMat::~EigenToMat
 */
EigenToMat::~EigenToMat()
{
    if(NULL != p_mat)
        matClose(p_mat);

    if(NULL != p_array)
        mxDestroyArray(p_array);
}

/**
 * Clear mxArray
 * @brief EigenToMat::clearData
 * @return true if cleared
 */
bool EigenToMat::clearData()
{
    if(NULL != p_array)
        mxDestroyArray(p_array);
    p_array = NULL;

    return true;
}

/**
 * @brief EigenToMat::openMatFile
 * @param file_name
 * @return true if file opened in write mode
 */
bool EigenToMat::openMatFile(const char * file_name)
{
    if(NULL == (p_mat = matOpen(file_name, "w")))
    {
        std::cerr << "Cannot open file in write mode" << std::endl;
        return false;
    }
    return true;
}

/**
 * Write data in mxArray format to file
 * @brief EigenToMat::writeMatFile
 * @return true if data written to file
 */
bool EigenToMat::writeMatFile() const
{
    if(NULL == p_mat)
    {
        std::cerr << "No output file opened" << std::endl;
        return false;
    }
    if(NULL == p_array)
    {
        std::cerr << "No data in array" << std::endl;
        return false;
    }
    if(0 != matPutVariable(p_mat, name, p_array))
    {
        std::cerr << "Error writing array to file" << std::endl;
        return false;
    }
    return true;
}
