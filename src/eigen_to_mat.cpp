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

EigenToMat::~EigenToMat()
{
    if(NULL != p_mat)
        matClose(p_mat);

    if(NULL != p_array)
        mxDestroyArray(p_array);
}

bool EigenToMat::clearData()
{
    if(NULL != p_array)
        mxDestroyArray(p_array);
    p_array = NULL;

    return true;
}

bool EigenToMat::openMatFile(const char * file_name)
{
    if(NULL == (p_mat = matOpen(file_name, "w")))
    {
        std::cerr << "Cannot open file in write mode" << std::endl;
        return false;
    }
    return true;
}

bool EigenToMat::writeMatFile() const
{
    if(0 != matPutVariable(p_mat, name, p_array))
    {
        std::cerr << "Error writing array to file" << std::endl;
        return false;
    }
    return true;
}
