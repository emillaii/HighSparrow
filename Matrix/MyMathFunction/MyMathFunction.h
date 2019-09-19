//
// File: MyMathFunction.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Aug-2016 12:51:00
//
#ifndef MYMATHFUNCTION_H
#define MYMATHFUNCTION_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "MyMathFunction_types.h"

// Function Declarations
extern void MyMathFunction_initialize();
extern void MyMathFunction_terminate();
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern double my_det(const emxArray_real_T *M);
extern void my_inv(const emxArray_real_T *M, emxArray_real_T *y);
extern void my_left_div(const emxArray_real_T *M, const emxArray_real_T *divM,
  emxArray_real_T *y);
extern void my_mul(const emxArray_real_T *M1, const emxArray_real_T *M2,
                   emxArray_real_T *y);
extern void my_pinv(const emxArray_real_T *M, emxArray_real_T *y);
extern void my_right_div(const emxArray_real_T *M, const emxArray_real_T *divM,
  emxArray_real_T *y);
extern void my_svd(const emxArray_real_T *M, emxArray_real_T *U, emxArray_real_T
                   *S, emxArray_real_T *V);

#endif

//
// File trailer for MyMathFunction.h
//
// [EOF]
//
