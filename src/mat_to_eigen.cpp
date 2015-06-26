#include "mat_to_eigen.hpp"

MatToEigen::MatToEigen()
{
    name = NULL;
    p_array = NULL;
    data = NULL;
}

MatToEigen::~MatToEigen()
{
    mxDestroyArray(p_array);
    // DO NOT call free(data). Doing that would cause core dump.
}

/**
 * Clear data and free memery
 * @brief MatToEigen::clearData
 * @return true if everything goes well
 */
bool MatToEigen::clearData()
{
    if(NULL != p_array)
        mxDestroyArray(p_array);

    p_array = NULL;
    data = NULL;
    name = NULL;
    return true;
}

/**
 * Open .mat file and read the first array
 * @brief MatToEigen::openMatFile
 * @param file_name
 * @return true if everything goes well
 */
bool MatToEigen::openMatFile(const char * file_name)
{
    if(NULL != p_array || NULL != data)
    {
        std::cerr << "Data from another file already exists. clearData first." << std::endl;
        return false;
    }
    MATFile * p_mat;
    if(NULL == (p_mat = matOpen(file_name, "r")))
    {
        std::cerr << "Cannot open " << file_name << std::endl;
        return false;
    } // open file

    int num_dirs;
    const char ** dirs = (const char **)matGetDir(p_mat, &num_dirs);
    if(dirs == NULL)
    {
        std::cerr << "Cannot get arrays from .mat file" << std::endl;
        return false;
    } // read arrays
    if(num_dirs > 1)
    {
        std::cerr << "File contains more than one arrays, only the first one would be used" << std::endl;
    }
    mxFree(dirs);

    if(0 != matClose(p_mat))
    {
        std::cerr << "Cannot close file after reading" << std::endl;
        return false;
    }
    if(NULL == (p_mat = matOpen(file_name, "r")))
    {
        std::cerr << "Cannot open " << file_name << std::endl;
        return false;
    } // close and re-open .mat file

    p_array = matGetNextVariableInfo(p_mat, &name);
    if(NULL == p_array)
    {
        std::cerr << "Cannot read array info" << std::endl;
        return false;
    }
    p_array = matGetVariable(p_mat, name);
    if(NULL == p_array)
    {
        std::cerr << "Cannot read array data" << std::endl;
        return false;
    } // read array to p_a

    if(0 != matClose(p_mat))
    {
        std::cerr << "Cannot close file after reading" << std::endl;
        return false;
    } // close file

    if(NULL == (data = mxGetData(p_array)))
    {
        std::cerr << "No data in this array" << std::endl;
        return false;
    }
    return true;
}

/**
 * If elements in the array have numerical type, convert them into float and fill in the matrix
 * This could cause precision lost
 * @brief MatToEigen::toMatrixXf
 * @param matrix
 * @return parameter itself
 */
Eigen::MatrixXf & MatToEigen::toMatrixXf(Eigen::MatrixXf & matrix) const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return matrix;
    }

    int rows = getRows();
    int cols = getCols();
    if(0 == rows || 0 == cols)
        return matrix;

    mwSize size = mxGetElementSize(p_array);
    mxClassID class_id = mxGetClassID(p_array);
    if(false == CLASS_ID_IS_NUMERIC(class_id))
    {
        std::cerr << "Cannot convert from " << mxGetClassName(p_array) << " to float" << std::endl;
        return matrix;
    }

    matrix.resize(rows, cols);

    void * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (char *)data + (m * cols + n) * size;

            switch(class_id)
            {
                case mxCHAR_CLASS: matrix(m, n) = (float)(*((char *)p_elem)); break;
                case mxINT8_CLASS: matrix(m, n) = (float)(*((int8_t *)p_elem)); break;
                case mxINT16_CLASS: matrix(m, n) = (float)(*((int16_t *)p_elem)); break;
                case mxINT32_CLASS: matrix(m, n) = (float)(*((int32_t *)p_elem)); break;
                case mxINT64_CLASS: matrix(m, n) = (float)(*((int64_t *)p_elem)); break;
                case mxUINT8_CLASS: matrix(m, n) = (float)(*((uint8_t *)p_elem)); break;
                case mxUINT16_CLASS: matrix(m, n) = (float)(*((uint16_t *)p_elem)); break;
                case mxUINT32_CLASS: matrix(m, n) = (float)(*((uint32_t *)p_elem)); break;
                case mxUINT64_CLASS: matrix(m, n) = (float)(*((uint64_t *)p_elem)); break;
                case mxSINGLE_CLASS: matrix(m, n) = (float)(*((float *)p_elem)); break;
                case mxDOUBLE_CLASS: matrix(m, n) = (float)(*((double *)p_elem)); break;
                default: break;
            }
        }
    }
    return matrix;
}

/**
 * If elements in the array have numerical type, convert them into double and fill in the matrix
 * This could cause precision lost
 * @brief MatToEigen::toMatrixXd
 * @param matrix
 * @return unchanged matrix if no data or type is invalid
 */
Eigen::MatrixXd & MatToEigen::toMatrixXd(Eigen::MatrixXd & matrix) const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return matrix;
    }

    int rows = getRows();
    int cols = getCols();
    if(0 == rows || 0 == cols)
        return matrix;

    mwSize size = mxGetElementSize(p_array);
    mxClassID class_id = mxGetClassID(p_array);
    if(false == CLASS_ID_IS_NUMERIC(class_id))
    {
        std::cerr << "Cannot convert from " << mxGetClassName(p_array) << " to double" << std::endl;
        return matrix;
    }

    matrix.resize(rows, cols);

    void * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (char *)data + (m * cols + n) * size;

            switch(class_id)
            {
                case mxCHAR_CLASS: matrix(m, n) = (double)(*((char *)p_elem)); break;
                case mxINT8_CLASS: matrix(m, n) = (double)(*((int8_t *)p_elem)); break;
                case mxINT16_CLASS: matrix(m, n) = (double)(*((int16_t *)p_elem)); break;
                case mxINT32_CLASS: matrix(m, n) = (double)(*((int32_t *)p_elem)); break;
                case mxINT64_CLASS: matrix(m, n) = (double)(*((int64_t *)p_elem)); break;
                case mxUINT8_CLASS: matrix(m, n) = (double)(*((uint8_t *)p_elem)); break;
                case mxUINT16_CLASS: matrix(m, n) = (double)(*((uint16_t *)p_elem)); break;
                case mxUINT32_CLASS: matrix(m, n) = (double)(*((uint32_t *)p_elem)); break;
                case mxUINT64_CLASS: matrix(m, n) = (double)(*((uint64_t *)p_elem)); break;
                case mxSINGLE_CLASS: matrix(m, n) = (double)(*((float *)p_elem)); break;
                case mxDOUBLE_CLASS: matrix(m, n) = (double)(*((double *)p_elem)); break;
                default: break;
            }
        }
    }
    return matrix;
}

/**
 * If elements in the array have signed integer type, convert them into int and fill in the matrix
 * This could cause precision lost or overflow
 * @brief MatToEigen::toMatrixXi
 * @param matrix
 * @return unchanged matrix if no data or type is invalid
 */
Eigen::MatrixXi & MatToEigen::toMatrixXi(Eigen::MatrixXi & matrix) const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return matrix;
    }

    int rows = getRows();
    int cols = getCols();
    if(0 == rows || 0 == cols)
        return matrix;

    mwSize size = mxGetElementSize(p_array);
    mxClassID class_id = mxGetClassID(p_array);
    if(false == (mxCHAR_CLASS == class_id || mxINT8_CLASS == class_id || mxINT16_CLASS == class_id || mxINT32_CLASS == class_id))
    {
        std::cerr << "Cannot convert from " << mxGetClassName(p_array) << " to signed int" << std::endl;
        return matrix;
    }

    matrix.resize(rows, cols);

    void * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (char *)data + (m * cols + n) * size;

            switch(class_id)
            {
                case mxCHAR_CLASS: matrix(m, n) = (int)(*((char *)p_elem)); break;
                case mxINT8_CLASS: matrix(m, n) = (int)(*((int8_t *)p_elem)); break;
                case mxINT16_CLASS: matrix(m, n) = (int)(*((int16_t *)p_elem)); break;
                case mxINT32_CLASS: matrix(m, n) = (int)(*((int32_t *)p_elem)); break;
                default: break;
            }
        }
    }
    return matrix;
}

/**
 * Print some brief description lines about the array
 * @brief MatToEigen::showArray
 */
void MatToEigen::showArray() const
{
    using std::cout;
    using std::endl;
    using std::cerr;

    if(NULL == p_array)
    {
        cerr << "No data read from file yet." << endl;
        return;
    }

    cout << "Array Name      : " << name << endl;
    cout << "Dimension(s)    : " << mxGetNumberOfDimensions(p_array) << endl;
    cout << "Rows X Columns  : " << mxGetM(p_array) << " X " << mxGetN(p_array) << endl;
    cout << "Elements Size   : " << mxGetElementSize(p_array) << endl;
    cout << "Elements Type   : " << mxGetClassName(p_array) << endl;
    cout << "Elements Counts : " << mxGetNumberOfElements(p_array) << endl;
}

/**
 * Print elements in the array in matrix form, if elements type is char, int or double
 * @brief MatToEigen::showElements
 */
void MatToEigen::showElementsNumeric() const
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
    mxClassID class_id = mxGetClassID(p_array);

    void * p_elem;
    for(int m = 0; m < rows; m ++)
    {
        for(int n = 0; n < cols; n ++)
        {
            p_elem = (char *)data + (m * cols + n) * size;

            switch(class_id)
            {
                case mxCHAR_CLASS: cout << *((char *)p_elem) << ","; break;
                case mxINT8_CLASS: cout << *((int8_t *)p_elem) << ","; break;
                case mxINT16_CLASS: cout << *((int16_t *)p_elem) << ","; break;
                case mxINT32_CLASS: cout << *((int32_t *)p_elem) << ","; break;
                case mxINT64_CLASS: cout << *((int64_t *)p_elem) << ","; break;
                case mxUINT8_CLASS: cout << *((uint8_t *)p_elem) << ","; break;
                case mxUINT16_CLASS: cout << *((uint16_t *)p_elem) << ","; break;
                case mxUINT32_CLASS: cout << *((uint32_t *)p_elem) << ","; break;
                case mxUINT64_CLASS: cout << *((uint64_t *)p_elem) << ","; break;
                case mxSINGLE_CLASS: cout << *((float *)p_elem) << ","; break;
                case mxDOUBLE_CLASS: cout << *((double *)p_elem) << ","; break;
                default: cout << "Elements type not supported" << endl; return;
            }
        }
        cout << endl;
    }
}

/**
 * @brief MatToEigen::getRows
 * @return 0 if there is no data
 */
int MatToEigen::getRows() const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return 0;
    }
    return mxGetM(p_array);
}

/**
 * @brief MatToEigen::getCols
 * @return 0 if there is no data
 */
int MatToEigen::getCols() const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return 0;
    }
    return mxGetN(p_array);
}

/**
 * @brief MatToEigen::getName
 * @return NULL if there is no data
 */
const char * MatToEigen::getName() const
{
    if(NULL == p_array || NULL == data)
    {
        std::cerr << "No data read from file yet." << std::endl;
        return NULL;
    }
    return name;
}
