#ifndef MAT_EIGEN_COMMON_H
#define MAT_EIGEN_COMMON_H

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cinttypes>

#include "mat.h"
#include "mex.h"
#include "matrix.h"
#include <eigen3/Eigen/Dense>

#define CLASS_ID_IS_NUMERIC(class_id) \
( \
    (class_id) == mxCHAR_CLASS || \
    (class_id) == mxINT8_CLASS || \
    (class_id) == mxINT16_CLASS || \
    (class_id) == mxINT32_CLASS || \
    (class_id) == mxINT64_CLASS || \
    (class_id) == mxUINT8_CLASS || \
    (class_id) == mxUINT16_CLASS || \
    (class_id) == mxUINT32_CLASS || \
    (class_id) == mxUINT64_CLASS || \
    (class_id) == mxSINGLE_CLASS || \
    (class_id) == mxDOUBLE_CLASS \
)

#endif
