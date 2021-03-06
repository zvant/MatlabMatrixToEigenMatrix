#include "eigen_to_mat.hpp"

/**
 * If no name specified, construct with "anonymous"
 * @brief EigenToMat::EigenToMat
 * @param name_str
 */
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
    clearData();
    closeMatFile();
}

/**
 * Set name of the array
 * @brief EigenToMat::setName
 * @param name_str
 */
void EigenToMat::setName(const char *name_str)
{
    name = name_str;
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
 * Open the output file
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
 * Close the output file
 * @brief EigenToMat::closeMatFile
 * @return true if file closed
 */
bool EigenToMat::closeMatFile()
{
    if(0 != matClose(p_mat))
    {
        std::cerr << "Cannot close file" << std::endl;
        return false;
    }
    p_mat = NULL;
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
