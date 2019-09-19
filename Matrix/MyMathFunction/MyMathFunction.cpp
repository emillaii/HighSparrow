//
// File: MyMathFunction.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Aug-2016 12:51:00
//

// Include Files
#include "rt_nonfinite.h"
#include "MyMathFunction.h"
#include <stdio.h>
#include <stdlib.h>

// Type Definitions
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_int32_T

typedef struct {
  int iFirst;
  int iLast;
  int lineNo;
  int colNo;
  const char * aName;
  const char * fName;
  const char * pName;
  int checkKind;
} rtBoundsCheckInfo;

typedef struct {
  int lineNo;
  int colNo;
  const char * fName;
  const char * pName;
} rtRunTimeErrorInfo;

// Variable Definitions
static rtRunTimeErrorInfo emlrtRTEI = { 11, 15, "det",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\det.m"
};

static rtBoundsCheckInfo emlrtBCI = { -1, -1, 21, 7, "", "det",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\det.m",
  0 };

static rtBoundsCheckInfo b_emlrtBCI = { -1, -1, 21, 9, "", "det",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\det.m",
  0 };

static rtBoundsCheckInfo c_emlrtBCI = { -1, -1, 27, 8, "", "det",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\det.m",
  0 };

static rtBoundsCheckInfo d_emlrtBCI = { -1, -1, 23, 13, "", "det",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\det.m",
  0 };

static rtBoundsCheckInfo e_emlrtBCI = { -1, -1, 23, 15, "", "det",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\det.m",
  0 };

static rtBoundsCheckInfo f_emlrtBCI = { -1, -1, 154, 5, "", "colon",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\colon.m",
  0 };

static rtBoundsCheckInfo g_emlrtBCI = { -1, -1, 158, 9, "", "colon",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\colon.m",
  0 };

static rtRunTimeErrorInfo b_emlrtRTEI = { 86, 15,
  "eml_int_forloop_overflow_check",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\eml\\eml_int_forloop_overflow_check.m"
};

static rtBoundsCheckInfo h_emlrtBCI = { -1, -1, 22, 11, "", "eml_ipiv2perm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\private\\eml_ipiv2perm.m",
  0 };

static rtRunTimeErrorInfo c_emlrtRTEI = { 14, 15, "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m"
};

static rtBoundsCheckInfo i_emlrtBCI = { -1, -1, 177, 9, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo j_emlrtBCI = { -1, -1, 178, 7, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo k_emlrtBCI = { -1, -1, 178, 9, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo l_emlrtBCI = { -1, -1, 180, 14, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo m_emlrtBCI = { -1, -1, 180, 16, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo n_emlrtBCI = { -1, -1, 183, 28, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo o_emlrtBCI = { -1, -1, 183, 30, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo p_emlrtBCI = { -1, -1, 183, 37, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo q_emlrtBCI = { -1, -1, 183, 39, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo r_emlrtBCI = { -1, -1, 183, 44, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo s_emlrtBCI = { -1, -1, 183, 46, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo t_emlrtBCI = { -1, -1, 183, 19, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo u_emlrtBCI = { -1, -1, 183, 21, "", "inv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\inv.m",
  0 };

static rtBoundsCheckInfo v_emlrtBCI = { -1, -1, 25, 16, "", "eml_ipiv2perm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\private\\eml_ipiv2perm.m",
  0 };

static rtBoundsCheckInfo w_emlrtBCI = { -1, -1, 26, 21, "", "eml_ipiv2perm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\private\\eml_ipiv2perm.m",
  0 };

static rtBoundsCheckInfo x_emlrtBCI = { -1, -1, 26, 9, "", "eml_ipiv2perm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\private\\eml_ipiv2perm.m",
  0 };

static rtBoundsCheckInfo y_emlrtBCI = { -1, -1, 27, 9, "", "eml_ipiv2perm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\private\\eml_ipiv2perm.m",
  0 };

static rtBoundsCheckInfo ab_emlrtBCI = { -1, -1, 66, 23, "", "norm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\norm.m",
  0 };

static rtBoundsCheckInfo bb_emlrtBCI = { -1, -1, 66, 25, "", "norm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\norm.m",
  0 };

static rtBoundsCheckInfo cb_emlrtBCI = { -1, -1, 191, 21, "", "norm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\norm.m",
  0 };

static rtRunTimeErrorInfo d_emlrtRTEI = { 1, 1, "mldivide",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\mldivide.p"
};

static rtBoundsCheckInfo db_emlrtBCI = { -1, -1, 67, 24, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo eb_emlrtBCI = { -1, -1, 72, 44, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo fb_emlrtBCI = { -1, -1, 72, 59, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo gb_emlrtBCI = { -1, -1, 72, 73, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo hb_emlrtBCI = { -1, -1, 72, 29, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo ib_emlrtBCI = { -1, -1, 132, 12, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo jb_emlrtBCI = { -1, -1, 133, 18, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo kb_emlrtBCI = { -1, -1, 135, 26, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo lb_emlrtBCI = { -1, -1, 135, 29, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo mb_emlrtBCI = { -1, -1, 136, 30, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo nb_emlrtBCI = { -1, -1, 136, 33, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo ob_emlrtBCI = { -1, -1, 136, 19, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo pb_emlrtBCI = { -1, -1, 136, 22, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo qb_emlrtBCI = { -1, -1, 137, 19, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo rb_emlrtBCI = { -1, -1, 137, 22, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo sb_emlrtBCI = { -1, -1, 21, 63, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo tb_emlrtBCI = { -1, -1, 21, 61, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo ub_emlrtBCI = { -1, -1, 21, 56, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo vb_emlrtBCI = { -1, -1, 21, 54, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo wb_emlrtBCI = { -1, -1, 16, 17, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo xb_emlrtBCI = { -1, -1, 88, 50, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo yb_emlrtBCI = { -1, -1, 88, 48, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo ac_emlrtBCI = { -1, -1, 88, 43, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo bc_emlrtBCI = { -1, -1, 88, 40, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo cc_emlrtBCI = { -1, -1, 87, 14, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo dc_emlrtBCI = { -1, -1, 115, 60, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo ec_emlrtBCI = { -1, -1, 115, 56, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo fc_emlrtBCI = { -1, -1, 114, 68, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo gc_emlrtBCI = { -1, -1, 114, 66, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo hc_emlrtBCI = { -1, -1, 88, 11, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo ic_emlrtBCI = { -1, -1, 88, 14, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo jc_emlrtBCI = { -1, -1, 90, 30, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo kc_emlrtBCI = { -1, -1, 90, 38, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo lc_emlrtBCI = { -1, -1, 90, 45, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo mc_emlrtBCI = { -1, -1, 90, 48, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo nc_emlrtBCI = { -1, -1, 90, 53, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo oc_emlrtBCI = { -1, -1, 90, 55, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo pc_emlrtBCI = { -1, -1, 90, 15, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo qc_emlrtBCI = { -1, -1, 90, 23, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo rc_emlrtBCI = { -1, -1, 83, 30, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo sc_emlrtBCI = { -1, -1, 83, 32, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo tc_emlrtBCI = { -1, -1, 83, 15, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo uc_emlrtBCI = { -1, -1, 83, 23, "", "qrsolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\qrsolve.m",
  0 };

static rtBoundsCheckInfo vc_emlrtBCI = { -1, -1, 19, 25, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo wc_emlrtBCI = { -1, -1, 19, 27, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo xc_emlrtBCI = { -1, -1, 25, 28, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo yc_emlrtBCI = { -1, -1, 25, 30, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo ad_emlrtBCI = { -1, -1, 25, 19, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo bd_emlrtBCI = { -1, -1, 25, 21, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo cd_emlrtBCI = { -1, -1, 27, 32, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo dd_emlrtBCI = { -1, -1, 27, 34, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo ed_emlrtBCI = { -1, -1, 27, 41, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo fd_emlrtBCI = { -1, -1, 27, 43, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo gd_emlrtBCI = { -1, -1, 27, 23, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtBoundsCheckInfo hd_emlrtBCI = { -1, -1, 27, 25, "", "xzunormqr",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzunormqr.m",
  0 };

static rtRunTimeErrorInfo e_emlrtRTEI = { 17, 9, "error",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\error.m"
};

static rtBoundsCheckInfo id_emlrtBCI = { -1, -1, 21, 13, "", "xnrm2",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xnrm2.m",
  0 };

static rtBoundsCheckInfo jd_emlrtBCI = { -1, -1, 37, 22, "", "xnrm2",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xnrm2.m",
  0 };

static rtBoundsCheckInfo kd_emlrtBCI = { -1, -1, 23, 46, "", "ixamax",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\ixamax.m",
  0 };

static rtBoundsCheckInfo ld_emlrtBCI = { -1, -1, 26, 47, "", "ixamax",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\ixamax.m",
  0 };

static rtBoundsCheckInfo md_emlrtBCI = { -1, -1, 88, 36, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtRunTimeErrorInfo f_emlrtRTEI = { 98, 23, "eml_mtimes_helper",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"
};

static rtRunTimeErrorInfo g_emlrtRTEI = { 103, 23, "eml_mtimes_helper",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"
};

static rtBoundsCheckInfo nd_emlrtBCI = { -1, -1, 87, 20, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo od_emlrtBCI = { -1, -1, 92, 33, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo pd_emlrtBCI = { -1, -1, 92, 47, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo qd_emlrtBCI = { -1, -1, 92, 25, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo rd_emlrtBCI = { -1, -1, 65, 13, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo sd_emlrtBCI = { -1, -1, 132, 41, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo td_emlrtBCI = { -1, -1, 14, 18, "", "svd",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\svd.m",
  0 };

static rtBoundsCheckInfo ud_emlrtBCI = { -1, -1, 64, 66, "", "pinv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\pinv.m",
  0 };

static rtBoundsCheckInfo vd_emlrtBCI = { -1, -1, 64, 64, "", "pinv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\pinv.m",
  0 };

static rtBoundsCheckInfo wd_emlrtBCI = { -1, -1, 47, 27, "", "pinv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\pinv.m",
  0 };

static rtBoundsCheckInfo xd_emlrtBCI = { -1, -1, 47, 25, "", "pinv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\pinv.m",
  0 };

static rtBoundsCheckInfo yd_emlrtBCI = { -1, -1, 52, 12, "", "pinv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\pinv.m",
  0 };

static rtBoundsCheckInfo ae_emlrtBCI = { -1, -1, 52, 14, "", "pinv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\matfun\\pinv.m",
  0 };

static rtBoundsCheckInfo be_emlrtBCI = { -1, -1, 130, 16, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo ce_emlrtBCI = { -1, -1, 139, 29, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo de_emlrtBCI = { -1, -1, 139, 43, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo ee_emlrtBCI = { -1, -1, 139, 21, "", "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemm.m",
  0 };

static rtBoundsCheckInfo fe_emlrtBCI = { -1, -1, 26, 16, "",
  "scaleVectorByRecip",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\scaleVectorByRecip.m",
  0 };

static rtBoundsCheckInfo ge_emlrtBCI = { -1, -1, 368, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo he_emlrtBCI = { -1, -1, 367, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ie_emlrtBCI = { -1, -1, 366, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo je_emlrtBCI = { -1, -1, 365, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ke_emlrtBCI = { -1, -1, 364, 27, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo le_emlrtBCI = { -1, -1, 363, 46, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo me_emlrtBCI = { -1, -1, 363, 30, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ne_emlrtBCI = { -1, -1, 362, 30, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo oe_emlrtBCI = { -1, -1, 361, 55, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo pe_emlrtBCI = { -1, -1, 361, 39, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo qe_emlrtBCI = { -1, -1, 347, 31, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo re_emlrtBCI = { -1, -1, 328, 31, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo se_emlrtBCI = { -1, -1, 306, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo te_emlrtBCI = { -1, -1, 304, 44, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ue_emlrtBCI = { -1, -1, 301, 44, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ve_emlrtBCI = { -1, -1, 283, 31, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo we_emlrtBCI = { -1, -1, 282, 49, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo xe_emlrtBCI = { -1, -1, 282, 30, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ye_emlrtBCI = { -1, -1, 259, 58, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo af_emlrtBCI = { -1, -1, 259, 41, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo bf_emlrtBCI = { -1, -1, 242, 24, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo cf_emlrtBCI = { -1, -1, 241, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo df_emlrtBCI = { -1, -1, 230, 24, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ef_emlrtBCI = { -1, -1, 227, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ff_emlrtBCI = { -1, -1, 226, 22, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo gf_emlrtBCI = { -1, -1, 212, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo hf_emlrtBCI = { -1, -1, 181, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo if_emlrtBCI = { -1, -1, 134, 42, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo jf_emlrtBCI = { -1, -1, 134, 36, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo kf_emlrtBCI = { -1, -1, 114, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo lf_emlrtBCI = { -1, -1, 91, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo mf_emlrtBCI = { -1, -1, 91, 27, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo nf_emlrtBCI = { -1, -1, 78, 61, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo of_emlrtBCI = { -1, -1, 76, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo pf_emlrtBCI = { -1, -1, 152, 22, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo qf_emlrtBCI = { -1, -1, 152, 28, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo rf_emlrtBCI = { -1, -1, 152, 9, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo sf_emlrtBCI = { -1, -1, 155, 9, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo tf_emlrtBCI = { -1, -1, 158, 22, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo uf_emlrtBCI = { -1, -1, 158, 28, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo vf_emlrtBCI = { -1, -1, 158, 9, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo wf_emlrtBCI = { -1, -1, 160, 5, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo xf_emlrtBCI = { -1, -1, 286, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo yf_emlrtBCI = { -1, -1, 308, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ag_emlrtBCI = { -1, -1, 416, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo bg_emlrtBCI = { -1, -1, 417, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo cg_emlrtBCI = { -1, -1, 417, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo dg_emlrtBCI = { -1, -1, 426, 41, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo eg_emlrtBCI = { -1, -1, 426, 54, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo fg_emlrtBCI = { -1, -1, 427, 31, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo gg_emlrtBCI = { -1, -1, 428, 33, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo hg_emlrtBCI = { -1, -1, 428, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ig_emlrtBCI = { -1, -1, 429, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo jg_emlrtBCI = { -1, -1, 325, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo kg_emlrtBCI = { -1, -1, 326, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo lg_emlrtBCI = { -1, -1, 330, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo mg_emlrtBCI = { -1, -1, 333, 40, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ng_emlrtBCI = { -1, -1, 334, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo og_emlrtBCI = { -1, -1, 334, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo pg_emlrtBCI = { -1, -1, 344, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo qg_emlrtBCI = { -1, -1, 345, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo rg_emlrtBCI = { -1, -1, 349, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo sg_emlrtBCI = { -1, -1, 350, 36, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo tg_emlrtBCI = { -1, -1, 351, 28, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ug_emlrtBCI = { -1, -1, 351, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo vg_emlrtBCI = { -1, -1, 412, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo wg_emlrtBCI = { -1, -1, 389, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo xg_emlrtBCI = { -1, -1, 391, 33, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo yg_emlrtBCI = { -1, -1, 391, 49, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ah_emlrtBCI = { -1, -1, 392, 31, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo bh_emlrtBCI = { -1, -1, 392, 41, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ch_emlrtBCI = { -1, -1, 392, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo dh_emlrtBCI = { -1, -1, 393, 35, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo eh_emlrtBCI = { -1, -1, 394, 30, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo fh_emlrtBCI = { -1, -1, 394, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo gh_emlrtBCI = { -1, -1, 401, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo hh_emlrtBCI = { -1, -1, 402, 33, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ih_emlrtBCI = { -1, -1, 402, 49, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo jh_emlrtBCI = { -1, -1, 403, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo kh_emlrtBCI = { -1, -1, 403, 44, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo lh_emlrtBCI = { -1, -1, 403, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo mh_emlrtBCI = { -1, -1, 404, 35, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo nh_emlrtBCI = { -1, -1, 405, 30, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo oh_emlrtBCI = { -1, -1, 405, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ph_emlrtBCI = { -1, -1, 225, 12, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo qh_emlrtBCI = { -1, -1, 228, 13, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo rh_emlrtBCI = { -1, -1, 230, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo sh_emlrtBCI = { -1, -1, 240, 16, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo th_emlrtBCI = { -1, -1, 243, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo uh_emlrtBCI = { -1, -1, 244, 28, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo vh_emlrtBCI = { -1, -1, 244, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo wh_emlrtBCI = { -1, -1, 204, 31, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo xh_emlrtBCI = { -1, -1, 220, 16, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo yh_emlrtBCI = { -1, -1, 220, 18, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ai_emlrtBCI = { -1, -1, 218, 20, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo bi_emlrtBCI = { -1, -1, 218, 23, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ci_emlrtBCI = { -1, -1, 176, 16, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo di_emlrtBCI = { -1, -1, 197, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ei_emlrtBCI = { -1, -1, 195, 23, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo fi_emlrtBCI = { -1, -1, 195, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo gi_emlrtBCI = { -1, -1, 189, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo hi_emlrtBCI = { -1, -1, 189, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ii_emlrtBCI = { -1, -1, 191, 23, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ji_emlrtBCI = { -1, -1, 191, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ki_emlrtBCI = { -1, -1, 187, 34, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo li_emlrtBCI = { -1, -1, 187, 37, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo mi_emlrtBCI = { -1, -1, 187, 23, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ni_emlrtBCI = { -1, -1, 187, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo oi_emlrtBCI = { -1, -1, 168, 19, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo pi_emlrtBCI = { -1, -1, 168, 22, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo qi_emlrtBCI = { -1, -1, 166, 23, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ri_emlrtBCI = { -1, -1, 166, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo si_emlrtBCI = { -1, -1, 82, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ti_emlrtBCI = { -1, -1, 76, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ui_emlrtBCI = { -1, -1, 26, 9, "", "scaleVectorByRecip",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\scaleVectorByRecip.m",
  0 };

static rtBoundsCheckInfo vi_emlrtBCI = { -1, -1, 79, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo wi_emlrtBCI = { -1, -1, 79, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo xi_emlrtBCI = { -1, -1, 80, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo yi_emlrtBCI = { -1, -1, 80, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo aj_emlrtBCI = { -1, -1, 114, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo bj_emlrtBCI = { -1, -1, 116, 59, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo cj_emlrtBCI = { -1, -1, 117, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo dj_emlrtBCI = { -1, -1, 117, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ej_emlrtBCI = { -1, -1, 119, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo fj_emlrtBCI = { -1, -1, 119, 21, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo gj_emlrtBCI = { -1, -1, 131, 62, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo hj_emlrtBCI = { -1, -1, 126, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ij_emlrtBCI = { -1, -1, 21, 15, "", "xscal",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xscal.m",
  0 };

static rtBoundsCheckInfo jj_emlrtBCI = { -1, -1, 21, 5, "", "xscal",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xscal.m",
  0 };

static rtBoundsCheckInfo kj_emlrtBCI = { -1, -1, 112, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo lj_emlrtBCI = { -1, -1, 144, 32, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo mj_emlrtBCI = { -1, -1, 144, 24, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo nj_emlrtBCI = { -1, -1, 144, 27, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo oj_emlrtBCI = { -1, -1, 102, 29, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo pj_emlrtBCI = { -1, -1, 102, 32, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo qj_emlrtBCI = { -1, -1, 102, 19, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo rj_emlrtBCI = { -1, -1, 102, 22, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo sj_emlrtBCI = { -1, -1, 97, 26, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo tj_emlrtBCI = { -1, -1, 97, 13, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo uj_emlrtBCI = { -1, -1, 61, 22, "", "svd",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\svd.m",
  0 };

static rtBoundsCheckInfo vj_emlrtBCI = { -1, -1, 61, 11, "", "svd",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\svd.m",
  0 };

static rtBoundsCheckInfo wj_emlrtBCI = { -1, -1, 61, 15, "", "svd",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\svd.m",
  0 };

static rtBoundsCheckInfo xj_emlrtBCI = { -1, -1, 459, 25, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo yj_emlrtBCI = { -1, -1, 459, 27, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ak_emlrtBCI = { -1, -1, 459, 15, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo bk_emlrtBCI = { -1, -1, 459, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ck_emlrtBCI = { -1, -1, 453, 17, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo dk_emlrtBCI = { -1, -1, 453, 5, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ek_emlrtBCI = { -1, -1, 52, 16, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo fk_emlrtBCI = { -1, -1, 52, 19, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo gk_emlrtBCI = { -1, -1, 47, 15, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo hk_emlrtBCI = { -1, -1, 47, 18, "", "xzsvdc",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzsvdc.m",
  0 };

static rtBoundsCheckInfo ik_emlrtBCI = { -1, -1, 46, 21, "", "xdotx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xdotx.m",
  0 };

static rtBoundsCheckInfo jk_emlrtBCI = { -1, -1, 46, 28, "", "xdotx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xdotx.m",
  0 };

static rtRunTimeErrorInfo h_emlrtRTEI = { 1, 1, "mrdivide",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\mrdivide.p"
};

static rtBoundsCheckInfo kk_emlrtBCI = { -1, -1, 119, 12, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo lk_emlrtBCI = { -1, -1, 120, 18, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo mk_emlrtBCI = { -1, -1, 122, 26, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo nk_emlrtBCI = { -1, -1, 122, 29, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo ok_emlrtBCI = { -1, -1, 123, 30, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo pk_emlrtBCI = { -1, -1, 123, 33, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo qk_emlrtBCI = { -1, -1, 123, 19, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo rk_emlrtBCI = { -1, -1, 123, 22, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo sk_emlrtBCI = { -1, -1, 124, 19, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo tk_emlrtBCI = { -1, -1, 124, 22, "", "lusolve",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\lusolve.m",
  0 };

static rtBoundsCheckInfo uk_emlrtBCI = { -1, -1, 169, 24, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo vk_emlrtBCI = { -1, -1, 171, 44, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo wk_emlrtBCI = { -1, -1, 171, 59, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo xk_emlrtBCI = { -1, -1, 171, 73, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo yk_emlrtBCI = { -1, -1, 171, 29, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo al_emlrtBCI = { -1, -1, 152, 52, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo bl_emlrtBCI = { -1, -1, 154, 46, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo cl_emlrtBCI = { -1, -1, 154, 25, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo dl_emlrtBCI = { -1, -1, 145, 24, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo el_emlrtBCI = { -1, -1, 147, 44, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo fl_emlrtBCI = { -1, -1, 147, 59, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo gl_emlrtBCI = { -1, -1, 147, 73, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo hl_emlrtBCI = { -1, -1, 147, 29, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo il_emlrtBCI = { -1, -1, 51, 48, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo jl_emlrtBCI = { -1, -1, 51, 43, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo kl_emlrtBCI = { -1, -1, 62, 31, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo ll_emlrtBCI = { -1, -1, 62, 33, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo ml_emlrtBCI = { -1, -1, 38, 9, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo nl_emlrtBCI = { -1, -1, 41, 13, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo ol_emlrtBCI = { -1, -1, 43, 15, "", "xgerx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgerx.m",
  0 };

static rtBoundsCheckInfo pl_emlrtBCI = { -1, -1, 56, 26, "", "xgerx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgerx.m",
  0 };

static rtBoundsCheckInfo ql_emlrtBCI = { -1, -1, 56, 35, "", "xgerx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgerx.m",
  0 };

static rtBoundsCheckInfo rl_emlrtBCI = { -1, -1, 56, 17, "", "xgerx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgerx.m",
  0 };

static rtBoundsCheckInfo sl_emlrtBCI = { -1, -1, 51, 17, "", "xzgetrf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgetrf.m",
  0 };

static rtBoundsCheckInfo tl_emlrtBCI = { -1, -1, 21, 12, "", "xswap",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xswap.m",
  0 };

static rtBoundsCheckInfo ul_emlrtBCI = { -1, -1, 23, 17, "", "xswap",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xswap.m",
  0 };

static rtBoundsCheckInfo vl_emlrtBCI = { -1, -1, 23, 9, "", "xswap",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xswap.m",
  0 };

static rtBoundsCheckInfo wl_emlrtBCI = { -1, -1, 24, 9, "", "xswap",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xswap.m",
  0 };

static rtBoundsCheckInfo xl_emlrtBCI = { -1, -1, 47, 24, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo yl_emlrtBCI = { -1, -1, 49, 44, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo am_emlrtBCI = { -1, -1, 49, 58, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo bm_emlrtBCI = { -1, -1, 49, 29, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo cm_emlrtBCI = { -1, -1, 52, 44, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo dm_emlrtBCI = { -1, -1, 52, 59, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo em_emlrtBCI = { -1, -1, 52, 73, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo fm_emlrtBCI = { -1, -1, 52, 29, "", "xtrsm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xtrsm.m",
  0 };

static rtBoundsCheckInfo gm_emlrtBCI = { -1, -1, 45, 15, "", "xgerx",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgerx.m",
  0 };

static rtBoundsCheckInfo hm_emlrtBCI = { -1, -1, 83, 54, "", "xgemv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemv.m",
  0 };

static rtBoundsCheckInfo im_emlrtBCI = { -1, -1, 83, 60, "", "xgemv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemv.m",
  0 };

static rtBoundsCheckInfo jm_emlrtBCI = { -1, -1, 88, 45, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo km_emlrtBCI = { -1, -1, 88, 43, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo lm_emlrtBCI = { -1, -1, 78, 80, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo mm_emlrtBCI = { -1, -1, 63, 16, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo nm_emlrtBCI = { -1, -1, 52, 21, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo om_emlrtBCI = { -1, -1, 53, 25, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo pm_emlrtBCI = { -1, -1, 53, 13, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo qm_emlrtBCI = { -1, -1, 54, 13, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo rm_emlrtBCI = { -1, -1, 55, 24, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo sm_emlrtBCI = { -1, -1, 55, 13, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo tm_emlrtBCI = { -1, -1, 56, 24, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo um_emlrtBCI = { -1, -1, 56, 13, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo vm_emlrtBCI = { -1, -1, 67, 9, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo wm_emlrtBCI = { -1, -1, 64, 17, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo xm_emlrtBCI = { -1, -1, 65, 9, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo ym_emlrtBCI = { -1, -1, 75, 16, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo an_emlrtBCI = { -1, -1, 76, 9, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo bn_emlrtBCI = { -1, -1, 38, 31, "", "xzlarf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzlarf.m",
  0 };

static rtBoundsCheckInfo cn_emlrtBCI = { -1, -1, 104, 12, "", "xzlarf",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzlarf.m",
  0 };

static rtBoundsCheckInfo dn_emlrtBCI = { -1, -1, 99, 17, "", "xgemv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemv.m",
  0 };

static rtBoundsCheckInfo en_emlrtBCI = { -1, -1, 99, 9, "", "xgemv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemv.m",
  0 };

static rtBoundsCheckInfo fn_emlrtBCI = { -1, -1, 37, 13, "", "xgemv",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xgemv.m",
  0 };

static rtBoundsCheckInfo gn_emlrtBCI = { -1, -1, 79, 9, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo hn_emlrtBCI = { -1, -1, 85, 16, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo in_emlrtBCI = { -1, -1, 88, 49, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo jn_emlrtBCI = { -1, -1, 93, 37, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo kn_emlrtBCI = { -1, -1, 93, 44, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo ln_emlrtBCI = { -1, -1, 104, 30, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo mn_emlrtBCI = { -1, -1, 104, 21, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo nn_emlrtBCI = { -1, -1, 100, 25, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo on_emlrtBCI = { -1, -1, 101, 25, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo pn_emlrtBCI = { -1, -1, 97, 25, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo qn_emlrtBCI = { -1, -1, 98, 34, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo rn_emlrtBCI = { -1, -1, 98, 25, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo sn_emlrtBCI = { -1, -1, 32, 9, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo tn_emlrtBCI = { -1, -1, 33, 18, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo un_emlrtBCI = { -1, -1, 33, 9, "", "xzgeqp3",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+reflapack\\xzgeqp3.m",
  0 };

static rtBoundsCheckInfo vn_emlrtBCI = { -1, -1, 35, 17, "", "xaxpy",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xaxpy.m",
  0 };

static rtBoundsCheckInfo wn_emlrtBCI = { -1, -1, 35, 56, "", "xaxpy",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xaxpy.m",
  0 };

static rtBoundsCheckInfo xn_emlrtBCI = { -1, -1, 34, 13, "", "xaxpy",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xaxpy.m",
  0 };

static rtBoundsCheckInfo yn_emlrtBCI = { -1, -1, 38, 17, "", "xaxpy",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xaxpy.m",
  0 };

static rtBoundsCheckInfo ao_emlrtBCI = { -1, -1, 38, 56, "", "xaxpy",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xaxpy.m",
  0 };

static rtBoundsCheckInfo bo_emlrtBCI = { -1, -1, 37, 13, "", "xaxpy",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xaxpy.m",
  0 };

static rtBoundsCheckInfo co_emlrtBCI = { -1, -1, 26, 18, "", "xrot",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xrot.m",
  0 };

static rtBoundsCheckInfo do_emlrtBCI = { -1, -1, 26, 28, "", "xrot",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xrot.m",
  0 };

static rtBoundsCheckInfo eo_emlrtBCI = { -1, -1, 27, 19, "", "xrot",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xrot.m",
  0 };

static rtBoundsCheckInfo fo_emlrtBCI = { -1, -1, 27, 29, "", "xrot",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xrot.m",
  0 };

static rtBoundsCheckInfo go_emlrtBCI = { -1, -1, 27, 9, "", "xrot",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xrot.m",
  0 };

static rtBoundsCheckInfo ho_emlrtBCI = { -1, -1, 32, 5, "", "xrot",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+refblas\\xrot.m",
  0 };

// Function Declarations
static void b_error();
static void b_rtErrorWithMessageID(const int b, const char *c, const
  rtRunTimeErrorInfo *aInfo);
static double b_sqrt(double x);
static void b_svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *
                  S, emxArray_real_T *V);
static void b_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0);
static void b_xgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T
                     *ipiv, int *info);
static double b_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void c_error();
static void c_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo);
static void c_sqrt(double *x);
static void c_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0);
static double c_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void check_forloop_overflow_error();
static void d_rtErrorWithMessageID(const int b, const char *c, const
  rtRunTimeErrorInfo *aInfo);
static double d_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void e_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo);
static void eml_pinv(const emxArray_real_T *A, emxArray_real_T *X);
static void eml_signed_integer_colon(int b, emxArray_int32_T *y);
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
static void emxFree_int32_T(emxArray_int32_T **pEmxArray);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);
static void error();
static void f_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo);
static void g_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo);
static void h_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo);
static int ixamax(int n, const emxArray_real_T *x, int ix0);
static double norm(const emxArray_real_T *x);
static void qrsolve(const emxArray_real_T *A, const emxArray_real_T *B,
                    emxArray_real_T *Y);
static void rtDynamicBoundsError(int aIndexValue, int aLoBound, int aHiBound,
  const rtBoundsCheckInfo *aInfo);
static void rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo);
static boolean_T rtIsNullOrEmptyString(const char *aString);
static void rtReportErrorLocation(const char * aFcnName, const int aLineNo);
static double rt_hypotd_snf(double u0, double u1);
static void svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *S,
                emxArray_real_T *V);
static void xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0);
static double xdotc(int n, const emxArray_real_T *x, int ix0, const
                    emxArray_real_T *y, int iy0);
static void xgeqp3(emxArray_real_T *A, emxArray_real_T *tau, emxArray_int32_T
                   *jpvt);
static void xgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T
                   *ipiv);
static double xnrm2(int n, const emxArray_real_T *x, int ix0);
static void xrot(int n, emxArray_real_T *x, int ix0, int iy0, double c, double s);
static void xrotg(double *a, double *b, double *c, double *s);
static void xscal(int n, double a, emxArray_real_T *x, int ix0);
static void xswap(int n, emxArray_real_T *x, int ix0, int iy0);
static void xtrsm(int m, int n, const emxArray_real_T *A, int lda,
                  emxArray_real_T *B, int ldb);
static void xzgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T *
                    ipiv, int *info);

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
static void b_error()
{
  g_rtErrorWithMessageID(&e_emlrtRTEI);
}

//
// Arguments    : const int b
//                const char *c
//                const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void b_rtErrorWithMessageID(const int b, const char *c, const
  rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr,
          "The loop variable of class %.*s might overflow on the last iteration of the for loop. This could lead to an infinite loop.",
          b, c);
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : double x
// Return Type  : double
//
static double b_sqrt(double x)
{
  double b_x;
  b_x = x;
  c_sqrt(&b_x);
  return b_x;
}

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *U
//                emxArray_real_T *S
//                emxArray_real_T *V
// Return Type  : void
//
static void b_svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *
                  S, emxArray_real_T *V)
{
  emxArray_real_T *b_A;
  int i32;
  int ns;
  int n;
  int p;
  int minnp;
  emxArray_real_T *s;
  emxArray_real_T *e;
  emxArray_real_T *work;
  int qs;
  int nrt;
  int nct;
  int q;
  int pmq;
  int m;
  int nmq;
  boolean_T overflow;
  int k;
  double ztest0;
  int mm;
  unsigned int uv0[2];
  double snorm;
  boolean_T exitg1;
  double ztest;
  boolean_T exitg5;
  boolean_T exitg4;
  double f;
  boolean_T exitg3;
  double varargin_1[5];
  double mtmp;
  boolean_T exitg2;
  double sqds;
  double b;
  emxInit_real_T(&b_A, 2);
  i32 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = A->size[1];
  emxEnsureCapacity((emxArray__common *)b_A, i32, (int)sizeof(double));
  ns = A->size[0] * A->size[1];
  for (i32 = 0; i32 < ns; i32++) {
    b_A->data[i32] = A->data[i32];
  }

  n = A->size[0];
  p = A->size[1];
  if (A->size[0] + 1 <= A->size[1]) {
    ns = A->size[0] + 1;
  } else {
    ns = A->size[1];
  }

  if (A->size[0] <= A->size[1]) {
    minnp = A->size[0];
  } else {
    minnp = A->size[1];
  }

  emxInit_real_T1(&s, 1);
  i32 = s->size[0];
  s->size[0] = ns;
  emxEnsureCapacity((emxArray__common *)s, i32, (int)sizeof(double));
  for (i32 = 0; i32 < ns; i32++) {
    s->data[i32] = 0.0;
  }

  emxInit_real_T1(&e, 1);
  ns = A->size[1];
  i32 = e->size[0];
  e->size[0] = ns;
  emxEnsureCapacity((emxArray__common *)e, i32, (int)sizeof(double));
  for (i32 = 0; i32 < ns; i32++) {
    e->data[i32] = 0.0;
  }

  emxInit_real_T1(&work, 1);
  ns = A->size[0];
  i32 = work->size[0];
  work->size[0] = ns;
  emxEnsureCapacity((emxArray__common *)work, i32, (int)sizeof(double));
  for (i32 = 0; i32 < ns; i32++) {
    work->data[i32] = 0.0;
  }

  ns = A->size[0];
  qs = A->size[0];
  i32 = U->size[0] * U->size[1];
  U->size[0] = ns;
  U->size[1] = qs;
  emxEnsureCapacity((emxArray__common *)U, i32, (int)sizeof(double));
  ns *= qs;
  for (i32 = 0; i32 < ns; i32++) {
    U->data[i32] = 0.0;
  }

  ns = A->size[1];
  qs = A->size[1];
  i32 = V->size[0] * V->size[1];
  V->size[0] = ns;
  V->size[1] = qs;
  emxEnsureCapacity((emxArray__common *)V, i32, (int)sizeof(double));
  ns *= qs;
  for (i32 = 0; i32 < ns; i32++) {
    V->data[i32] = 0.0;
  }

  if ((A->size[0] == 0) || (A->size[1] == 0)) {
    qs = A->size[0];
    if ((!(1 > qs)) && (qs > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (ns = 1; ns <= qs; ns++) {
      i32 = U->size[0];
      if (!((ns >= 1) && (ns <= i32))) {
        rtDynamicBoundsError(ns, 1, i32, &gk_emlrtBCI);
      }

      i32 = U->size[1];
      if (!((ns >= 1) && (ns <= i32))) {
        rtDynamicBoundsError(ns, 1, i32, &hk_emlrtBCI);
      }

      U->data[(ns + U->size[0] * (ns - 1)) - 1] = 1.0;
    }

    qs = A->size[1];
    if ((!(1 > qs)) && (qs > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (ns = 1; ns <= qs; ns++) {
      i32 = V->size[0];
      if (!((ns >= 1) && (ns <= i32))) {
        rtDynamicBoundsError(ns, 1, i32, &ek_emlrtBCI);
      }

      i32 = V->size[1];
      if (!((ns >= 1) && (ns <= i32))) {
        rtDynamicBoundsError(ns, 1, i32, &fk_emlrtBCI);
      }

      V->data[(ns + V->size[0] * (ns - 1)) - 1] = 1.0;
    }
  } else {
    if (A->size[1] > 2) {
      ns = A->size[1] - 2;
    } else {
      ns = 0;
    }

    if (ns <= A->size[0]) {
      nrt = ns;
    } else {
      nrt = A->size[0];
    }

    ns = A->size[0];
    if (ns >= 1) {
    } else {
      ns = 1;
    }

    if (ns - 1 <= A->size[1]) {
      nct = ns - 1;
    } else {
      nct = A->size[1];
    }

    if (nct >= nrt) {
      qs = nct;
    } else {
      qs = nrt;
    }

    for (q = 1; q <= qs; q++) {
      pmq = q + n * (q - 1);
      nmq = n - q;
      overflow = false;
      if (q <= nct) {
        ztest0 = c_xnrm2(nmq + 1, b_A, pmq);
        if (ztest0 > 0.0) {
          overflow = true;
          i32 = b_A->size[0] * b_A->size[1];
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &of_emlrtBCI);
          }

          if (b_A->data[pmq - 1] < 0.0) {
            ztest0 = -ztest0;
          }

          i32 = s->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &ti_emlrtBCI);
          }

          s->data[q - 1] = ztest0;
          i32 = s->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &nf_emlrtBCI);
          }

          if (fabs(s->data[q - 1]) >= 1.0020841800044864E-292) {
            xscal(nmq + 1, 1.0 / s->data[q - 1], b_A, pmq);
          } else {
            ns = pmq + nmq;
            if ((!(pmq > ns)) && (ns > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (k = pmq; k <= ns; k++) {
              i32 = b_A->size[0] * b_A->size[1];
              if (!((k >= 1) && (k <= i32))) {
                rtDynamicBoundsError(k, 1, i32, &fe_emlrtBCI);
              }

              i32 = b_A->size[0] * b_A->size[1];
              if (!((k >= 1) && (k <= i32))) {
                rtDynamicBoundsError(k, 1, i32, &ui_emlrtBCI);
              }

              b_A->data[k - 1] /= s->data[q - 1];
            }
          }

          i32 = b_A->size[0] * b_A->size[1];
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &vi_emlrtBCI);
          }

          i32 = b_A->size[0] * b_A->size[1];
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &wi_emlrtBCI);
          }

          b_A->data[pmq - 1]++;
          i32 = s->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &xi_emlrtBCI);
          }

          i32 = s->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &yi_emlrtBCI);
          }

          s->data[q - 1] = -s->data[q - 1];
        } else {
          i32 = s->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &si_emlrtBCI);
          }

          s->data[q - 1] = 0.0;
        }
      }

      if ((!(q + 1 > p)) && (p > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (mm = q + 1; mm <= p; mm++) {
        ns = q + n * (mm - 1);
        if (overflow) {
          ztest0 = xdotc(nmq + 1, b_A, pmq, b_A, ns);
          i32 = b_A->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &mf_emlrtBCI);
          }

          i32 = b_A->size[1];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &lf_emlrtBCI);
          }

          ztest0 = -(ztest0 / b_A->data[(q + b_A->size[0] * (q - 1)) - 1]);
          xaxpy(nmq + 1, ztest0, pmq, b_A, ns);
        }

        i32 = b_A->size[0] * b_A->size[1];
        if (!((ns >= 1) && (ns <= i32))) {
          rtDynamicBoundsError(ns, 1, i32, &sj_emlrtBCI);
        }

        i32 = e->size[0];
        if (!((mm >= 1) && (mm <= i32))) {
          rtDynamicBoundsError(mm, 1, i32, &tj_emlrtBCI);
        }

        e->data[mm - 1] = b_A->data[ns - 1];
      }

      if (q <= nct) {
        if ((!(q > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = q; ns <= n; ns++) {
          i32 = b_A->size[0];
          if (!((ns >= 1) && (ns <= i32))) {
            rtDynamicBoundsError(ns, 1, i32, &oj_emlrtBCI);
          }

          i32 = b_A->size[1];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &pj_emlrtBCI);
          }

          i32 = U->size[0];
          if (!((ns >= 1) && (ns <= i32))) {
            rtDynamicBoundsError(ns, 1, i32, &qj_emlrtBCI);
          }

          i32 = U->size[1];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &rj_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (q - 1)) - 1] = b_A->data[(ns + b_A->size[0]
            * (q - 1)) - 1];
        }
      }

      if (q <= nrt) {
        pmq = p - q;
        ztest0 = d_xnrm2(pmq, e, q + 1);
        if (ztest0 == 0.0) {
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &kj_emlrtBCI);
          }

          e->data[q - 1] = 0.0;
        } else {
          i32 = e->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i32))) {
            rtDynamicBoundsError(q + 1, 1, i32, &kf_emlrtBCI);
          }

          if (e->data[q] < 0.0) {
            ztest0 = -ztest0;
          }

          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &aj_emlrtBCI);
          }

          e->data[q - 1] = ztest0;
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &bj_emlrtBCI);
          }

          ztest0 = e->data[q - 1];
          if (fabs(ztest0) >= 1.0020841800044864E-292) {
            ztest0 = 1.0 / ztest0;
            ns = q + pmq;
            if ((!(q + 1 > ns)) && (ns > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (k = q + 1; k <= ns; k++) {
              i32 = e->size[0];
              if (!((k >= 1) && (k <= i32))) {
                rtDynamicBoundsError(k, 1, i32, &ij_emlrtBCI);
              }

              i32 = e->size[0];
              if (!((k >= 1) && (k <= i32))) {
                rtDynamicBoundsError(k, 1, i32, &jj_emlrtBCI);
              }

              e->data[k - 1] *= ztest0;
            }
          } else {
            ns = q + pmq;
            if ((!(q + 1 > ns)) && (ns > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (k = q + 1; k <= ns; k++) {
              i32 = e->size[0];
              if (!((k >= 1) && (k <= i32))) {
                rtDynamicBoundsError(k, 1, i32, &fe_emlrtBCI);
              }

              i32 = e->size[0];
              if (!((k >= 1) && (k <= i32))) {
                rtDynamicBoundsError(k, 1, i32, &ui_emlrtBCI);
              }

              e->data[k - 1] /= ztest0;
            }
          }

          i32 = e->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i32))) {
            rtDynamicBoundsError(q + 1, 1, i32, &cj_emlrtBCI);
          }

          i32 = e->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i32))) {
            rtDynamicBoundsError(q + 1, 1, i32, &dj_emlrtBCI);
          }

          e->data[q]++;
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &ej_emlrtBCI);
          }

          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &fj_emlrtBCI);
          }

          e->data[q - 1] = -e->data[q - 1];
          if (q + 1 <= n) {
            if ((!(q + 1 > n)) && (n > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (ns = q + 1; ns <= n; ns++) {
              i32 = work->size[0];
              if (!((ns >= 1) && (ns <= i32))) {
                rtDynamicBoundsError(ns, 1, i32, &hj_emlrtBCI);
              }

              work->data[ns - 1] = 0.0;
            }

            for (mm = q + 1; mm <= p; mm++) {
              i32 = e->size[0];
              if (!((mm >= 1) && (mm <= i32))) {
                rtDynamicBoundsError(mm, 1, i32, &gj_emlrtBCI);
              }

              b_xaxpy(nmq, e->data[mm - 1], b_A, (q + n * (mm - 1)) + 1, work, q
                      + 1);
            }

            for (mm = q; mm + 1 <= p; mm++) {
              i32 = e->size[0];
              if (!((mm + 1 >= 1) && (mm + 1 <= i32))) {
                rtDynamicBoundsError(mm + 1, 1, i32, &jf_emlrtBCI);
              }

              i32 = e->size[0];
              if (!((q + 1 >= 1) && (q + 1 <= i32))) {
                rtDynamicBoundsError(q + 1, 1, i32, &if_emlrtBCI);
              }

              c_xaxpy(nmq, -e->data[mm] / e->data[q], work, q + 1, b_A, (q + n *
                       mm) + 1);
            }
          }
        }

        for (ns = q + 1; ns <= p; ns++) {
          i32 = e->size[0];
          if (!((ns >= 1) && (ns <= i32))) {
            rtDynamicBoundsError(ns, 1, i32, &lj_emlrtBCI);
          }

          i32 = V->size[0];
          if (!((ns >= 1) && (ns <= i32))) {
            rtDynamicBoundsError(ns, 1, i32, &mj_emlrtBCI);
          }

          i32 = V->size[1];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &nj_emlrtBCI);
          }

          V->data[(ns + V->size[0] * (q - 1)) - 1] = e->data[ns - 1];
        }
      }
    }

    if (A->size[1] <= A->size[0] + 1) {
      m = A->size[1];
    } else {
      m = A->size[0] + 1;
    }

    if (nct < A->size[1]) {
      i32 = b_A->size[0];
      if (!((nct + 1 >= 1) && (nct + 1 <= i32))) {
        rtDynamicBoundsError(nct + 1, 1, i32, &pf_emlrtBCI);
      }

      i32 = b_A->size[1];
      if (!((nct + 1 >= 1) && (nct + 1 <= i32))) {
        rtDynamicBoundsError(nct + 1, 1, i32, &qf_emlrtBCI);
      }

      i32 = s->size[0];
      if (!((nct + 1 >= 1) && (nct + 1 <= i32))) {
        rtDynamicBoundsError(nct + 1, 1, i32, &rf_emlrtBCI);
      }

      s->data[nct] = b_A->data[nct + b_A->size[0] * nct];
    }

    if (A->size[0] < m) {
      i32 = s->size[0];
      if (!((m >= 1) && (m <= i32))) {
        rtDynamicBoundsError(m, 1, i32, &sf_emlrtBCI);
      }

      s->data[m - 1] = 0.0;
    }

    if (nrt + 1 < m) {
      i32 = b_A->size[0];
      if (!((nrt + 1 >= 1) && (nrt + 1 <= i32))) {
        rtDynamicBoundsError(nrt + 1, 1, i32, &tf_emlrtBCI);
      }

      i32 = b_A->size[1];
      if (!((m >= 1) && (m <= i32))) {
        rtDynamicBoundsError(m, 1, i32, &uf_emlrtBCI);
      }

      i32 = e->size[0];
      if (!((nrt + 1 >= 1) && (nrt + 1 <= i32))) {
        rtDynamicBoundsError(nrt + 1, 1, i32, &vf_emlrtBCI);
      }

      e->data[nrt] = b_A->data[nrt + b_A->size[0] * (m - 1)];
    }

    i32 = e->size[0];
    if (!((m >= 1) && (m <= i32))) {
      rtDynamicBoundsError(m, 1, i32, &wf_emlrtBCI);
    }

    e->data[m - 1] = 0.0;
    if (nct + 1 <= A->size[0]) {
      overflow = ((!(nct + 1 > A->size[0])) && (A->size[0] > 2147483646));
      if (overflow) {
        check_forloop_overflow_error();
      }

      for (mm = nct + 1; mm <= n; mm++) {
        if ((!(1 > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = 1; ns <= n; ns++) {
          i32 = U->size[0];
          if (!((ns >= 1) && (ns <= i32))) {
            rtDynamicBoundsError(ns, 1, i32, &qi_emlrtBCI);
          }

          i32 = U->size[1];
          if (!((mm >= 1) && (mm <= i32))) {
            rtDynamicBoundsError(mm, 1, i32, &ri_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (mm - 1)) - 1] = 0.0;
        }

        i32 = U->size[0];
        if (!((mm >= 1) && (mm <= i32))) {
          rtDynamicBoundsError(mm, 1, i32, &oi_emlrtBCI);
        }

        i32 = U->size[1];
        if (!((mm >= 1) && (mm <= i32))) {
          rtDynamicBoundsError(mm, 1, i32, &pi_emlrtBCI);
        }

        U->data[(mm + U->size[0] * (mm - 1)) - 1] = 1.0;
      }
    }

    while (nct > 0) {
      nmq = (n - nct) + 1;
      pmq = nct + n * (nct - 1);
      i32 = s->size[0];
      if (!(nct <= i32)) {
        rtDynamicBoundsError(nct, 1, i32, &ci_emlrtBCI);
      }

      if (s->data[nct - 1] != 0.0) {
        if ((!(nct + 1 > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (mm = nct; mm + 1 <= n; mm++) {
          ns = nct + n * mm;
          ztest0 = xdotc(nmq, U, pmq, U, ns);
          i32 = U->size[0] * U->size[1];
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &hf_emlrtBCI);
          }

          ztest0 = -(ztest0 / U->data[pmq - 1]);
          xaxpy(nmq, ztest0, pmq, U, ns);
        }

        for (ns = nct; ns <= n; ns++) {
          i32 = U->size[0];
          if (!(ns <= i32)) {
            rtDynamicBoundsError(ns, 1, i32, &ki_emlrtBCI);
          }

          i32 = U->size[1];
          if (!(nct <= i32)) {
            rtDynamicBoundsError(nct, 1, i32, &li_emlrtBCI);
          }

          i32 = U->size[0];
          if (!(ns <= i32)) {
            rtDynamicBoundsError(ns, 1, i32, &mi_emlrtBCI);
          }

          i32 = U->size[1];
          if (!(nct <= i32)) {
            rtDynamicBoundsError(nct, 1, i32, &ni_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (nct - 1)) - 1] = -U->data[(ns + U->size[0]
            * (nct - 1)) - 1];
        }

        i32 = U->size[0] * U->size[1];
        if (!((pmq >= 1) && (pmq <= i32))) {
          rtDynamicBoundsError(pmq, 1, i32, &gi_emlrtBCI);
        }

        i32 = U->size[0] * U->size[1];
        if (!((pmq >= 1) && (pmq <= i32))) {
          rtDynamicBoundsError(pmq, 1, i32, &hi_emlrtBCI);
        }

        U->data[pmq - 1]++;
        for (ns = 1; ns < nct; ns++) {
          i32 = U->size[0];
          if (!(ns <= i32)) {
            rtDynamicBoundsError(ns, 1, i32, &ii_emlrtBCI);
          }

          i32 = U->size[1];
          if (!(nct <= i32)) {
            rtDynamicBoundsError(nct, 1, i32, &ji_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (nct - 1)) - 1] = 0.0;
        }
      } else {
        if ((!(1 > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = 1; ns <= n; ns++) {
          i32 = U->size[0];
          if (!((ns >= 1) && (ns <= i32))) {
            rtDynamicBoundsError(ns, 1, i32, &ei_emlrtBCI);
          }

          i32 = U->size[1];
          if (!(nct <= i32)) {
            rtDynamicBoundsError(nct, 1, i32, &fi_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (nct - 1)) - 1] = 0.0;
        }

        i32 = U->size[0] * U->size[1];
        if (!((pmq >= 1) && (pmq <= i32))) {
          rtDynamicBoundsError(pmq, 1, i32, &di_emlrtBCI);
        }

        U->data[pmq - 1] = 1.0;
      }

      nct--;
    }

    for (q = A->size[1]; q > 0; q--) {
      if (q <= nrt) {
        i32 = e->size[0];
        if (!(q <= i32)) {
          rtDynamicBoundsError(q, 1, i32, &wh_emlrtBCI);
        }

        if (e->data[q - 1] != 0.0) {
          pmq = p - q;
          ns = (q + p * (q - 1)) + 1;
          if ((!(q + 1 > p)) && (p > 2147483646)) {
            check_forloop_overflow_error();
          }

          for (mm = q; mm + 1 <= p; mm++) {
            qs = (q + p * mm) + 1;
            ztest0 = xdotc(pmq, V, ns, V, qs);
            i32 = V->size[0] * V->size[1];
            if (!((ns >= 1) && (ns <= i32))) {
              rtDynamicBoundsError(ns, 1, i32, &gf_emlrtBCI);
            }

            ztest0 = -(ztest0 / V->data[ns - 1]);
            xaxpy(pmq, ztest0, ns, V, qs);
          }
        }
      }

      if ((!(1 > p)) && (p > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (ns = 1; ns <= p; ns++) {
        i32 = V->size[0];
        if (!((ns >= 1) && (ns <= i32))) {
          rtDynamicBoundsError(ns, 1, i32, &ai_emlrtBCI);
        }

        i32 = V->size[1];
        if (!(q <= i32)) {
          rtDynamicBoundsError(q, 1, i32, &bi_emlrtBCI);
        }

        V->data[(ns + V->size[0] * (q - 1)) - 1] = 0.0;
      }

      i32 = V->size[0];
      if (!(q <= i32)) {
        rtDynamicBoundsError(q, 1, i32, &xh_emlrtBCI);
      }

      i32 = V->size[1];
      if (!(q <= i32)) {
        rtDynamicBoundsError(q, 1, i32, &yh_emlrtBCI);
      }

      V->data[(q + V->size[0] * (q - 1)) - 1] = 1.0;
    }

    for (q = 1; q <= m; q++) {
      i32 = s->size[0];
      if (!((q >= 1) && (q <= i32))) {
        rtDynamicBoundsError(q, 1, i32, &ph_emlrtBCI);
      }

      if (s->data[q - 1] != 0.0) {
        i32 = s->size[0];
        if (!((q >= 1) && (q <= i32))) {
          rtDynamicBoundsError(q, 1, i32, &ff_emlrtBCI);
        }

        ztest = fabs(s->data[q - 1]);
        i32 = s->size[0];
        if (!((q >= 1) && (q <= i32))) {
          rtDynamicBoundsError(q, 1, i32, &ef_emlrtBCI);
        }

        ztest0 = s->data[q - 1] / ztest;
        i32 = s->size[0];
        if (!((q >= 1) && (q <= i32))) {
          rtDynamicBoundsError(q, 1, i32, &qh_emlrtBCI);
        }

        s->data[q - 1] = ztest;
        if (q < m) {
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &df_emlrtBCI);
          }

          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &rh_emlrtBCI);
          }

          e->data[q - 1] /= ztest0;
        }

        if (q <= n) {
          xscal(n, ztest0, U, 1 + n * (q - 1));
        }
      }

      if (q < m) {
        i32 = e->size[0];
        if (!((q >= 1) && (q <= i32))) {
          rtDynamicBoundsError(q, 1, i32, &sh_emlrtBCI);
        }

        if (e->data[q - 1] != 0.0) {
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &cf_emlrtBCI);
          }

          ztest = fabs(e->data[q - 1]);
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &bf_emlrtBCI);
          }

          ztest0 = ztest / e->data[q - 1];
          i32 = e->size[0];
          if (!((q >= 1) && (q <= i32))) {
            rtDynamicBoundsError(q, 1, i32, &th_emlrtBCI);
          }

          e->data[q - 1] = ztest;
          i32 = s->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i32))) {
            rtDynamicBoundsError(q + 1, 1, i32, &uh_emlrtBCI);
          }

          i32 = s->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i32))) {
            rtDynamicBoundsError(q + 1, 1, i32, &vh_emlrtBCI);
          }

          s->data[q] *= ztest0;
          xscal(p, ztest0, V, 1 + p * q);
        }
      }
    }

    mm = m;
    nct = 0;
    snorm = 0.0;
    for (ns = 1; ns <= m; ns++) {
      i32 = s->size[0];
      if (!(ns <= i32)) {
        rtDynamicBoundsError(ns, 1, i32, &af_emlrtBCI);
      }

      ztest0 = fabs(s->data[ns - 1]);
      i32 = e->size[0];
      if (!(ns <= i32)) {
        rtDynamicBoundsError(ns, 1, i32, &ye_emlrtBCI);
      }

      ztest = fabs(e->data[ns - 1]);
      if ((ztest0 >= ztest) || rtIsNaN(ztest)) {
      } else {
        ztest0 = ztest;
      }

      if ((snorm >= ztest0) || rtIsNaN(ztest0)) {
      } else {
        snorm = ztest0;
      }
    }

    exitg1 = false;
    while ((!exitg1) && (m > 0)) {
      if (nct >= 75) {
        c_error();
      } else {
        q = m - 1;
        exitg5 = false;
        while (!(exitg5 || (q == 0))) {
          i32 = s->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &xe_emlrtBCI);
          }

          i32 = s->size[0];
          if (!(q + 1 <= i32)) {
            rtDynamicBoundsError(q + 1, 1, i32, &we_emlrtBCI);
          }

          i32 = e->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &ve_emlrtBCI);
          }

          ztest0 = fabs(e->data[q - 1]);
          if ((ztest0 <= 2.2204460492503131E-16 * (fabs(s->data[q - 1]) + fabs
                (s->data[q]))) || (ztest0 <= 1.0020841800044864E-292) || ((nct >
                20) && (ztest0 <= 2.2204460492503131E-16 * snorm))) {
            i32 = e->size[0];
            if (!(q <= i32)) {
              rtDynamicBoundsError(q, 1, i32, &xf_emlrtBCI);
            }

            e->data[q - 1] = 0.0;
            exitg5 = true;
          } else {
            q--;
          }
        }

        if (q == m - 1) {
          ns = 4;
        } else {
          qs = m;
          ns = m;
          exitg4 = false;
          while ((!exitg4) && (ns >= q)) {
            qs = ns;
            if (ns == q) {
              exitg4 = true;
            } else {
              ztest0 = 0.0;
              if (ns < m) {
                i32 = e->size[0];
                if (!((ns >= 1) && (ns <= i32))) {
                  rtDynamicBoundsError(ns, 1, i32, &ue_emlrtBCI);
                }

                ztest0 = fabs(e->data[ns - 1]);
              }

              if (ns > q + 1) {
                i32 = e->size[0];
                pmq = ns - 1;
                if (!(pmq <= i32)) {
                  rtDynamicBoundsError(pmq, 1, i32, &te_emlrtBCI);
                }

                ztest0 += fabs(e->data[ns - 2]);
              }

              i32 = s->size[0];
              if (!((ns >= 1) && (ns <= i32))) {
                rtDynamicBoundsError(ns, 1, i32, &se_emlrtBCI);
              }

              ztest = fabs(s->data[ns - 1]);
              if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
                   1.0020841800044864E-292)) {
                i32 = s->size[0];
                if (!((ns >= 1) && (ns <= i32))) {
                  rtDynamicBoundsError(ns, 1, i32, &yf_emlrtBCI);
                }

                s->data[ns - 1] = 0.0;
                exitg4 = true;
              } else {
                ns--;
              }
            }
          }

          if (qs == q) {
            ns = 3;
          } else if (qs == m) {
            ns = 1;
          } else {
            ns = 2;
            q = qs;
          }
        }

        q++;
        switch (ns) {
         case 1:
          i32 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &jg_emlrtBCI);
          }

          f = e->data[pmq - 1];
          i32 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &kg_emlrtBCI);
          }

          e->data[pmq - 1] = 0.0;
          for (k = m - 1; k >= q; k--) {
            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &re_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &lg_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &lg_emlrtBCI);
            }

            xrotg(&s->data[k - 1], &f, &ztest0, &ztest);
            if (k > q) {
              i32 = e->size[0];
              pmq = k - 1;
              if (!(pmq <= i32)) {
                rtDynamicBoundsError(pmq, 1, i32, &mg_emlrtBCI);
              }

              f = -ztest * e->data[pmq - 1];
              i32 = e->size[0];
              pmq = k - 1;
              if (!(pmq <= i32)) {
                rtDynamicBoundsError(pmq, 1, i32, &ng_emlrtBCI);
              }

              i32 = e->size[0];
              ns = k - 1;
              if (!(ns <= i32)) {
                rtDynamicBoundsError(ns, 1, i32, &og_emlrtBCI);
              }

              e->data[ns - 1] = e->data[pmq - 1] * ztest0;
            }

            xrot(p, V, 1 + p * (k - 1), 1 + p * (m - 1), ztest0, ztest);
          }
          break;

         case 2:
          i32 = e->size[0];
          pmq = q - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &pg_emlrtBCI);
          }

          f = e->data[pmq - 1];
          i32 = e->size[0];
          pmq = q - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &qg_emlrtBCI);
          }

          e->data[pmq - 1] = 0.0;
          for (k = q; k <= m; k++) {
            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &qe_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &rg_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &rg_emlrtBCI);
            }

            xrotg(&s->data[k - 1], &f, &ztest0, &ztest);
            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &sg_emlrtBCI);
            }

            f = -ztest * e->data[k - 1];
            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &tg_emlrtBCI);
            }

            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &ug_emlrtBCI);
            }

            e->data[k - 1] *= ztest0;
            xrot(n, U, 1 + n * (k - 1), 1 + n * (q - 2), ztest0, ztest);
          }
          break;

         case 3:
          i32 = s->size[0];
          if (!(m <= i32)) {
            rtDynamicBoundsError(m, 1, i32, &pe_emlrtBCI);
          }

          i32 = s->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &oe_emlrtBCI);
          }

          i32 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &ne_emlrtBCI);
          }

          i32 = s->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &me_emlrtBCI);
          }

          i32 = e->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &le_emlrtBCI);
          }

          varargin_1[0] = fabs(s->data[m - 1]);
          varargin_1[1] = fabs(s->data[m - 2]);
          varargin_1[2] = fabs(e->data[m - 2]);
          varargin_1[3] = fabs(s->data[q - 1]);
          varargin_1[4] = fabs(e->data[q - 1]);
          ns = 1;
          mtmp = varargin_1[0];
          if (rtIsNaN(varargin_1[0])) {
            qs = 2;
            exitg2 = false;
            while ((!exitg2) && (qs < 6)) {
              ns = qs;
              if (!rtIsNaN(varargin_1[qs - 1])) {
                mtmp = varargin_1[qs - 1];
                exitg2 = true;
              } else {
                qs++;
              }
            }
          }

          if (ns < 5) {
            while (ns + 1 < 6) {
              if (varargin_1[ns] > mtmp) {
                mtmp = varargin_1[ns];
              }

              ns++;
            }
          }

          i32 = s->size[0];
          if (!(m <= i32)) {
            rtDynamicBoundsError(m, 1, i32, &ke_emlrtBCI);
          }

          f = s->data[m - 1] / mtmp;
          i32 = s->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &je_emlrtBCI);
          }

          ztest0 = s->data[m - 2] / mtmp;
          i32 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &ie_emlrtBCI);
          }

          ztest = e->data[m - 2] / mtmp;
          i32 = s->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &he_emlrtBCI);
          }

          sqds = s->data[q - 1] / mtmp;
          i32 = e->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &ge_emlrtBCI);
          }

          b = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
          ztest0 = f * ztest;
          ztest0 *= ztest0;
          if ((b != 0.0) || (ztest0 != 0.0)) {
            ztest = b * b + ztest0;
            c_sqrt(&ztest);
            if (b < 0.0) {
              ztest = -ztest;
            }

            ztest = ztest0 / (b + ztest);
          } else {
            ztest = 0.0;
          }

          f = (sqds + f) * (sqds - f) + ztest;
          b = sqds * (e->data[q - 1] / mtmp);
          for (k = q; k < m; k++) {
            xrotg(&f, &b, &ztest0, &ztest);
            if (k > q) {
              i32 = e->size[0];
              pmq = k - 1;
              if (!(pmq <= i32)) {
                rtDynamicBoundsError(pmq, 1, i32, &wg_emlrtBCI);
              }

              e->data[pmq - 1] = f;
            }

            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &xg_emlrtBCI);
            }

            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &yg_emlrtBCI);
            }

            f = ztest0 * s->data[k - 1] + ztest * e->data[k - 1];
            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &ah_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &bh_emlrtBCI);
            }

            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &ch_emlrtBCI);
            }

            e->data[k - 1] = ztest0 * e->data[k - 1] - ztest * s->data[k - 1];
            i32 = s->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &dh_emlrtBCI);
            }

            b = ztest * s->data[k];
            i32 = s->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &eh_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &fh_emlrtBCI);
            }

            s->data[k] *= ztest0;
            xrot(p, V, 1 + p * (k - 1), 1 + p * k, ztest0, ztest);
            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &gh_emlrtBCI);
            }

            s->data[k - 1] = f;
            i32 = s->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &gh_emlrtBCI);
            }

            xrotg(&s->data[k - 1], &b, &ztest0, &ztest);
            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &hh_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &ih_emlrtBCI);
            }

            f = ztest0 * e->data[k - 1] + ztest * s->data[k];
            i32 = e->size[0];
            if (!(k <= i32)) {
              rtDynamicBoundsError(k, 1, i32, &jh_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &kh_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &lh_emlrtBCI);
            }

            s->data[k] = -ztest * e->data[k - 1] + ztest0 * s->data[k];
            i32 = e->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &mh_emlrtBCI);
            }

            b = ztest * e->data[k];
            i32 = e->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &nh_emlrtBCI);
            }

            i32 = e->size[0];
            if (!(k + 1 <= i32)) {
              rtDynamicBoundsError(k + 1, 1, i32, &oh_emlrtBCI);
            }

            e->data[k] *= ztest0;
            if (k < n) {
              xrot(n, U, 1 + n * (k - 1), 1 + n * k, ztest0, ztest);
            }
          }

          i32 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i32))) {
            rtDynamicBoundsError(pmq, 1, i32, &vg_emlrtBCI);
          }

          e->data[pmq - 1] = f;
          nct++;
          break;

         default:
          i32 = s->size[0];
          if (!(q <= i32)) {
            rtDynamicBoundsError(q, 1, i32, &ag_emlrtBCI);
          }

          if (s->data[q - 1] < 0.0) {
            i32 = s->size[0];
            if (!(q <= i32)) {
              rtDynamicBoundsError(q, 1, i32, &bg_emlrtBCI);
            }

            i32 = s->size[0];
            if (!(q <= i32)) {
              rtDynamicBoundsError(q, 1, i32, &cg_emlrtBCI);
            }

            s->data[q - 1] = -s->data[q - 1];
            xscal(p, -1.0, V, 1 + p * (q - 1));
          }

          ns = q + 1;
          exitg3 = false;
          while ((!exitg3) && (q < mm)) {
            i32 = s->size[0];
            if (!((q >= 1) && (q <= i32))) {
              rtDynamicBoundsError(q, 1, i32, &dg_emlrtBCI);
            }

            i32 = s->size[0];
            if (!((ns >= 1) && (ns <= i32))) {
              rtDynamicBoundsError(ns, 1, i32, &eg_emlrtBCI);
            }

            if (s->data[q - 1] < s->data[ns - 1]) {
              i32 = s->size[0];
              if (!((q >= 1) && (q <= i32))) {
                rtDynamicBoundsError(q, 1, i32, &fg_emlrtBCI);
              }

              ztest = s->data[q - 1];
              i32 = s->size[0];
              if (!((ns >= 1) && (ns <= i32))) {
                rtDynamicBoundsError(ns, 1, i32, &gg_emlrtBCI);
              }

              i32 = s->size[0];
              if (!((q >= 1) && (q <= i32))) {
                rtDynamicBoundsError(q, 1, i32, &hg_emlrtBCI);
              }

              s->data[q - 1] = s->data[ns - 1];
              i32 = s->size[0];
              if (!((ns >= 1) && (ns <= i32))) {
                rtDynamicBoundsError(ns, 1, i32, &ig_emlrtBCI);
              }

              s->data[ns - 1] = ztest;
              if (q < p) {
                xswap(p, V, 1 + p * (q - 1), 1 + p * q);
              }

              if (q < n) {
                xswap(n, U, 1 + n * (q - 1), 1 + n * q);
              }

              q = ns;
              ns++;
            } else {
              exitg3 = true;
            }
          }

          nct = 0;
          m--;
          break;
        }
      }
    }
  }

  emxFree_real_T(&work);
  emxFree_real_T(&b_A);
  i32 = e->size[0];
  e->size[0] = minnp;
  emxEnsureCapacity((emxArray__common *)e, i32, (int)sizeof(double));
  if ((!(1 > minnp)) && (minnp > 2147483646)) {
    check_forloop_overflow_error();
  }

  for (k = 1; k <= minnp; k++) {
    i32 = s->size[0];
    if (!((k >= 1) && (k <= i32))) {
      rtDynamicBoundsError(k, 1, i32, &ck_emlrtBCI);
    }

    i32 = e->size[0];
    if (!((k >= 1) && (k <= i32))) {
      rtDynamicBoundsError(k, 1, i32, &dk_emlrtBCI);
    }

    e->data[k - 1] = s->data[k - 1];
  }

  emxFree_real_T(&s);
  for (i32 = 0; i32 < 2; i32++) {
    uv0[i32] = (unsigned int)A->size[i32];
  }

  i32 = S->size[0] * S->size[1];
  S->size[0] = (int)uv0[0];
  S->size[1] = (int)uv0[1];
  emxEnsureCapacity((emxArray__common *)S, i32, (int)sizeof(double));
  ns = (int)uv0[0] * (int)uv0[1];
  for (i32 = 0; i32 < ns; i32++) {
    S->data[i32] = 0.0;
  }

  for (k = 1; k - 1 < e->size[0]; k++) {
    i32 = e->size[0];
    if (!((k >= 1) && (k <= i32))) {
      rtDynamicBoundsError(k, 1, i32, &uj_emlrtBCI);
    }

    i32 = S->size[0];
    if (!((k >= 1) && (k <= i32))) {
      rtDynamicBoundsError(k, 1, i32, &vj_emlrtBCI);
    }

    i32 = S->size[1];
    if (!((k >= 1) && (k <= i32))) {
      rtDynamicBoundsError(k, 1, i32, &wj_emlrtBCI);
    }

    S->data[(k + S->size[0] * (k - 1)) - 1] = e->data[k - 1];
  }

  emxFree_real_T(&e);
}

//
// Arguments    : int n
//                double a
//                const emxArray_real_T *x
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void b_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0)
{
  int ix;
  int iy;
  int k;
  int i43;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0;
    iy = iy0;
    for (k = 0; k < n; k++) {
      i43 = y->size[0];
      if (!((iy >= 1) && (iy <= i43))) {
        rtDynamicBoundsError(iy, 1, i43, &yn_emlrtBCI);
      }

      i43 = x->size[0] * x->size[1];
      if (!((ix >= 1) && (ix <= i43))) {
        rtDynamicBoundsError(ix, 1, i43, &ao_emlrtBCI);
      }

      i43 = y->size[0];
      if (!((iy >= 1) && (iy <= i43))) {
        rtDynamicBoundsError(iy, 1, i43, &bo_emlrtBCI);
      }

      y->data[iy - 1] += a * x->data[ix - 1];
      ix++;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                emxArray_real_T *A
//                int lda
//                emxArray_int32_T *ipiv
//                int *info
// Return Type  : void
//
static void b_xgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T
                     *ipiv, int *info)
{
  xzgetrf(m, n, A, lda, ipiv, info);
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
static double b_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int i16;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    i16 = x->size[0] * x->size[1];
    if (!((ix0 >= 1) && (ix0 <= i16))) {
      rtDynamicBoundsError(ix0, 1, i16, &id_emlrtBCI);
    }

    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    if ((!(ix0 > kend)) && (kend > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (k = ix0; k <= kend; k++) {
      i16 = x->size[0] * x->size[1];
      if (!((k >= 1) && (k <= i16))) {
        rtDynamicBoundsError(k, 1, i16, &jd_emlrtBCI);
      }

      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// Arguments    : void
// Return Type  : void
//
static void c_error()
{
  h_rtErrorWithMessageID(&e_emlrtRTEI);
}

//
// Arguments    : const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void c_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr, "Matrix dimensions must agree.");
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : double *x
// Return Type  : void
//
static void c_sqrt(double *x)
{
  if (*x < 0.0) {
    error();
  }

  *x = sqrt(*x);
}

//
// Arguments    : int n
//                double a
//                const emxArray_real_T *x
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void c_xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
                    emxArray_real_T *y, int iy0)
{
  int ix;
  int iy;
  int k;
  int i44;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0;
    iy = iy0;
    for (k = 0; k < n; k++) {
      i44 = y->size[0] * y->size[1];
      if (!((iy >= 1) && (iy <= i44))) {
        rtDynamicBoundsError(iy, 1, i44, &yn_emlrtBCI);
      }

      i44 = x->size[0];
      if (!((ix >= 1) && (ix <= i44))) {
        rtDynamicBoundsError(ix, 1, i44, &ao_emlrtBCI);
      }

      i44 = y->size[0] * y->size[1];
      if (!((iy >= 1) && (iy <= i44))) {
        rtDynamicBoundsError(iy, 1, i44, &bo_emlrtBCI);
      }

      y->data[iy - 1] += a * x->data[ix - 1];
      ix++;
      iy++;
    }
  }
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
static double c_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int i17;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    i17 = x->size[0] * x->size[1];
    if (!((ix0 >= 1) && (ix0 <= i17))) {
      rtDynamicBoundsError(ix0, 1, i17, &id_emlrtBCI);
    }

    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    if ((!(ix0 > kend)) && (kend > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (k = ix0; k <= kend; k++) {
      i17 = x->size[0] * x->size[1];
      if (!((k >= 1) && (k <= i17))) {
        rtDynamicBoundsError(k, 1, i17, &jd_emlrtBCI);
      }

      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// Arguments    : void
// Return Type  : void
//
static void check_forloop_overflow_error()
{
  b_rtErrorWithMessageID(5, "int32", &b_emlrtRTEI);
}

//
// Arguments    : const int b
//                const char *c
//                const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void d_rtErrorWithMessageID(const int b, const char *c, const
  rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr,
          "Domain error. To compute complex results from real x, use \'%.*s(complex(x))\'.",
          b, c);
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
static double d_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int i24;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    i24 = x->size[0];
    if (!((ix0 >= 1) && (ix0 <= i24))) {
      rtDynamicBoundsError(ix0, 1, i24, &id_emlrtBCI);
    }

    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    if ((!(ix0 > kend)) && (kend > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (k = ix0; k <= kend; k++) {
      i24 = x->size[0];
      if (!((k >= 1) && (k <= i24))) {
        rtDynamicBoundsError(k, 1, i24, &jd_emlrtBCI);
      }

      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// Arguments    : const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void e_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr,
          "Inner dimensions must agree. Generated code for a general matrix multiplication at this call site. If this should have been a sc"
          "alar times a variable-size matrix, the scalar input must be fixed-size.");
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *X
// Return Type  : void
//
static void eml_pinv(const emxArray_real_T *A, emxArray_real_T *X)
{
  int m;
  int n;
  int vcol;
  int k;
  int i21;
  emxArray_real_T *V;
  emxArray_real_T *U;
  emxArray_real_T *S;
  double tol;
  int r;
  boolean_T overflow;
  boolean_T exitg1;
  int br;
  int b;
  int ic;
  int ar;
  int ib;
  int ia;
  int b_b;
  m = A->size[0];
  n = A->size[1];
  vcol = A->size[1];
  k = A->size[0];
  i21 = X->size[0] * X->size[1];
  X->size[0] = vcol;
  X->size[1] = k;
  emxEnsureCapacity((emxArray__common *)X, i21, (int)sizeof(double));
  vcol *= k;
  for (i21 = 0; i21 < vcol; i21++) {
    X->data[i21] = 0.0;
  }

  if ((A->size[0] == 0) || (A->size[1] == 0)) {
  } else {
    i21 = A->size[0] * A->size[1] - 1;
    for (k = 0; k <= i21; k++) {
      vcol = A->size[0] * A->size[1];
      if (!((k + 1 >= 1) && (k + 1 <= vcol))) {
        rtDynamicBoundsError(k + 1, 1, vcol, &td_emlrtBCI);
      }

      if (!((!rtIsInf(A->data[k])) && (!rtIsNaN(A->data[k])))) {
        b_error();
      }
    }

    emxInit_real_T(&V, 2);
    emxInit_real_T(&U, 2);
    emxInit_real_T(&S, 2);
    svd(A, U, S, V);
    i21 = S->size[0];
    if (!(1 <= i21)) {
      rtDynamicBoundsError(1, 1, i21, &xd_emlrtBCI);
    }

    i21 = S->size[1];
    if (!(1 <= i21)) {
      rtDynamicBoundsError(1, 1, i21, &wd_emlrtBCI);
    }

    tol = (double)A->size[0] * S->data[0] * 2.2204460492503131E-16;
    r = 0;
    overflow = (A->size[1] > 2147483646);
    if (overflow) {
      check_forloop_overflow_error();
    }

    k = 1;
    exitg1 = false;
    while ((!exitg1) && (k <= n)) {
      i21 = S->size[0];
      if (!((k >= 1) && (k <= i21))) {
        rtDynamicBoundsError(k, 1, i21, &yd_emlrtBCI);
      }

      i21 = S->size[1];
      if (!((k >= 1) && (k <= i21))) {
        rtDynamicBoundsError(k, 1, i21, &ae_emlrtBCI);
      }

      if (!(S->data[(k + S->size[0] * (k - 1)) - 1] > tol)) {
        exitg1 = true;
      } else {
        r++;
        k++;
      }
    }

    if (r > 0) {
      vcol = 1;
      if (r > 2147483646) {
        check_forloop_overflow_error();
      }

      for (k = 1; k <= r; k++) {
        i21 = S->size[0];
        if (!((k >= 1) && (k <= i21))) {
          rtDynamicBoundsError(k, 1, i21, &vd_emlrtBCI);
        }

        i21 = S->size[1];
        if (!((k >= 1) && (k <= i21))) {
          rtDynamicBoundsError(k, 1, i21, &ud_emlrtBCI);
        }

        xscal(n, 1.0 / S->data[(k + S->size[0] * (k - 1)) - 1], V, vcol);
        vcol += n;
      }

      k = A->size[1] * (A->size[0] - 1);
      overflow = ((!(0 > k)) && (k > MAX_int32_T - A->size[1]));
      if (overflow) {
        check_forloop_overflow_error();
      }

      vcol = 0;
      while ((n > 0) && (vcol <= k)) {
        b = vcol + n;
        if ((!(vcol + 1 > b)) && (b > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ic = vcol + 1; ic <= b; ic++) {
          i21 = X->size[0] * X->size[1];
          if (!((ic >= 1) && (ic <= i21))) {
            rtDynamicBoundsError(ic, 1, i21, &rd_emlrtBCI);
          }

          X->data[ic - 1] = 0.0;
        }

        vcol += n;
      }

      br = 0;
      overflow = ((!(0 > k)) && (k > MAX_int32_T - A->size[1]));
      if (overflow) {
        check_forloop_overflow_error();
      }

      vcol = 0;
      while ((n > 0) && (vcol <= k)) {
        ar = 0;
        br++;
        b = br + m * (r - 1);
        if ((m == 0) || (br > b)) {
          overflow = false;
        } else {
          overflow = (b > MAX_int32_T - m);
        }

        if (overflow) {
          check_forloop_overflow_error();
        }

        ib = br;
        while ((m > 0) && (ib <= b)) {
          i21 = U->size[0] * U->size[1];
          if (!((ib >= 1) && (ib <= i21))) {
            rtDynamicBoundsError(ib, 1, i21, &be_emlrtBCI);
          }

          if (U->data[ib - 1] != 0.0) {
            i21 = U->size[0] * U->size[1];
            if (!((ib >= 1) && (ib <= i21))) {
              rtDynamicBoundsError(ib, 1, i21, &sd_emlrtBCI);
            }

            ia = ar;
            b_b = vcol + n;
            if ((!(vcol + 1 > b_b)) && (b_b > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (ic = vcol + 1; ic <= b_b; ic++) {
              ia++;
              i21 = X->size[0] * X->size[1];
              if (!((ic >= 1) && (ic <= i21))) {
                rtDynamicBoundsError(ic, 1, i21, &ce_emlrtBCI);
              }

              i21 = V->size[0] * V->size[1];
              if (!((ia >= 1) && (ia <= i21))) {
                rtDynamicBoundsError(ia, 1, i21, &de_emlrtBCI);
              }

              i21 = X->size[0] * X->size[1];
              if (!((ic >= 1) && (ic <= i21))) {
                rtDynamicBoundsError(ic, 1, i21, &ee_emlrtBCI);
              }

              X->data[ic - 1] += U->data[ib - 1] * V->data[ia - 1];
            }
          }

          ar += n;
          ib += m;
        }

        vcol += n;
      }
    }

    emxFree_real_T(&S);
    emxFree_real_T(&U);
    emxFree_real_T(&V);
  }
}

//
// Arguments    : int b
//                emxArray_int32_T *y
// Return Type  : void
//
static void eml_signed_integer_colon(int b, emxArray_int32_T *y)
{
  int n;
  int i3;
  int yk;
  int k;
  if (b < 1) {
    n = 0;
  } else {
    n = b;
  }

  i3 = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity((emxArray__common *)y, i3, (int)sizeof(int));
  if (n > 0) {
    i3 = y->size[1];
    if (!(1 <= i3)) {
      rtDynamicBoundsError(1, 1, i3, &f_emlrtBCI);
    }

    y->data[0] = 1;
    yk = 1;
    for (k = 2; k <= n; k++) {
      yk++;
      i3 = y->size[1];
      if (!((k >= 1) && (k <= i3))) {
        rtDynamicBoundsError(k, 1, i3, &g_emlrtBCI);
      }

      y->data[k - 1] = yk;
    }
  }
}

//
// Arguments    : emxArray__common *emxArray
//                int oldNumel
//                int elementSize
// Return Type  : void
//
static void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize)
{
  int newNumel;
  int i;
  void *newData;
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }

    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i <<= 1;
      }
    }

    newData = calloc((unsigned int)i, (unsigned int)elementSize);
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, (unsigned int)(elementSize * oldNumel));
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

//
// Arguments    : emxArray_int32_T **pEmxArray
// Return Type  : void
//
static void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int32_T *)NULL) {
    if (((*pEmxArray)->data != (int *)NULL) && (*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_int32_T *)NULL;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
// Return Type  : void
//
static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (double *)NULL) && (*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

//
// Arguments    : emxArray_int32_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions)
{
  emxArray_int32_T *emxArray;
  int i;
  *pEmxArray = (emxArray_int32_T *)malloc(sizeof(emxArray_int32_T));
  emxArray = *pEmxArray;
  emxArray->data = (int *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxArray_real_T *emxArray;
  int i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (double *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
static void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxArray_real_T *emxArray;
  int i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (double *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)malloc((unsigned int)(sizeof(int) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : void
// Return Type  : void
//
static void error()
{
  d_rtErrorWithMessageID(4, "sqrt", &e_emlrtRTEI);
}

//
// Arguments    : const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void f_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr, "Inner matrix dimensions must agree.");
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void g_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr, "Input to SVD must not contain NaN or Inf.");
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void h_rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr, "SVD fails to converge");
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : int
//
static int ixamax(int n, const emxArray_real_T *x, int ix0)
{
  int idxmax;
  int ix;
  int i15;
  double smax;
  int k;
  double s;
  if (n < 1) {
    idxmax = 0;
  } else {
    idxmax = 1;
    if (n > 1) {
      ix = ix0;
      i15 = x->size[0];
      if (!((ix0 >= 1) && (ix0 <= i15))) {
        rtDynamicBoundsError(ix0, 1, i15, &kd_emlrtBCI);
      }

      smax = fabs(x->data[ix0 - 1]);
      for (k = 2; k <= n; k++) {
        ix++;
        i15 = x->size[0];
        if (!((ix >= 1) && (ix <= i15))) {
          rtDynamicBoundsError(ix, 1, i15, &ld_emlrtBCI);
        }

        s = fabs(x->data[ix - 1]);
        if (s > smax) {
          idxmax = k;
          smax = s;
        }
      }
    }
  }

  return idxmax;
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
static double norm(const emxArray_real_T *x)
{
  double y;
  int j;
  int i6;
  boolean_T exitg1;
  double s;
  int i7;
  int i;
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    y = 0.0;
  } else if ((x->size[0] == 1) || (x->size[1] == 1)) {
    y = 0.0;
    i6 = x->size[0] * x->size[1];
    for (j = 0; j < i6; j++) {
      i7 = x->size[0] * x->size[1];
      i = j + 1;
      if (!((i >= 1) && (i <= i7))) {
        rtDynamicBoundsError(i, 1, i7, &cb_emlrtBCI);
      }

      y += fabs(x->data[j]);
    }
  } else {
    y = 0.0;
    j = 0;
    exitg1 = false;
    while ((!exitg1) && (j <= x->size[1] - 1)) {
      s = 0.0;
      for (i = 0; i < x->size[0]; i++) {
        i6 = x->size[0];
        i7 = i + 1;
        if (!((i7 >= 1) && (i7 <= i6))) {
          rtDynamicBoundsError(i7, 1, i6, &ab_emlrtBCI);
        }

        i6 = x->size[1];
        i7 = j + 1;
        if (!((i7 >= 1) && (i7 <= i6))) {
          rtDynamicBoundsError(i7, 1, i6, &bb_emlrtBCI);
        }

        s += fabs(x->data[i + x->size[0] * j]);
      }

      if (rtIsNaN(s)) {
        y = rtNaN;
        exitg1 = true;
      } else {
        if (s > y) {
          y = s;
        }

        j++;
      }
    }
  }

  return y;
}

//
// Arguments    : const emxArray_real_T *A
//                const emxArray_real_T *B
//                emxArray_real_T *Y
// Return Type  : void
//
static void qrsolve(const emxArray_real_T *A, const emxArray_real_T *B,
                    emxArray_real_T *Y)
{
  emxArray_real_T *b_A;
  int i13;
  int minmn;
  emxArray_real_T *tau;
  emxArray_int32_T *jpvt;
  int rankR;
  int maxmn;
  double tol;
  boolean_T exitg1;
  emxArray_real_T *b_B;
  int nb;
  int mn;
  int j;
  boolean_T overflow;
  int k;
  int i;
  emxInit_real_T(&b_A, 2);
  i13 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = A->size[1];
  emxEnsureCapacity((emxArray__common *)b_A, i13, (int)sizeof(double));
  minmn = A->size[0] * A->size[1];
  for (i13 = 0; i13 < minmn; i13++) {
    b_A->data[i13] = A->data[i13];
  }

  emxInit_real_T1(&tau, 1);
  emxInit_int32_T(&jpvt, 2);
  xgeqp3(b_A, tau, jpvt);
  rankR = 0;
  if (b_A->size[0] < b_A->size[1]) {
    minmn = b_A->size[0];
    maxmn = b_A->size[1];
  } else {
    minmn = b_A->size[1];
    maxmn = b_A->size[0];
  }

  if (minmn > 0) {
    i13 = b_A->size[0];
    if (!(1 <= i13)) {
      rtDynamicBoundsError(1, 1, i13, &gc_emlrtBCI);
    }

    i13 = b_A->size[1];
    if (!(1 <= i13)) {
      rtDynamicBoundsError(1, 1, i13, &fc_emlrtBCI);
    }

    tol = (double)maxmn * fabs(b_A->data[0]) * 2.2204460492503131E-16;
    exitg1 = false;
    while ((!exitg1) && (rankR < minmn)) {
      i13 = b_A->size[0];
      if (!(rankR + 1 <= i13)) {
        rtDynamicBoundsError(rankR + 1, 1, i13, &ec_emlrtBCI);
      }

      i13 = b_A->size[1];
      if (!(rankR + 1 <= i13)) {
        rtDynamicBoundsError(rankR + 1, 1, i13, &dc_emlrtBCI);
      }

      if (fabs(b_A->data[rankR + b_A->size[0] * rankR]) >= tol) {
        rankR++;
      } else {
        exitg1 = true;
      }
    }
  }

  minmn = b_A->size[1];
  maxmn = B->size[1];
  i13 = Y->size[0] * Y->size[1];
  Y->size[0] = minmn;
  Y->size[1] = maxmn;
  emxEnsureCapacity((emxArray__common *)Y, i13, (int)sizeof(double));
  minmn *= maxmn;
  for (i13 = 0; i13 < minmn; i13++) {
    Y->data[i13] = 0.0;
  }

  emxInit_real_T(&b_B, 2);
  i13 = b_B->size[0] * b_B->size[1];
  b_B->size[0] = B->size[0];
  b_B->size[1] = B->size[1];
  emxEnsureCapacity((emxArray__common *)b_B, i13, (int)sizeof(double));
  minmn = B->size[0] * B->size[1];
  for (i13 = 0; i13 < minmn; i13++) {
    b_B->data[i13] = B->data[i13];
  }

  maxmn = b_A->size[0];
  nb = B->size[1];
  minmn = b_A->size[0];
  mn = b_A->size[1];
  if (minmn <= mn) {
    mn = minmn;
  }

  if ((!(1 > mn)) && (mn > 2147483646)) {
    check_forloop_overflow_error();
  }

  for (j = 1; j <= mn; j++) {
    i13 = tau->size[0];
    if (!((j >= 1) && (j <= i13))) {
      rtDynamicBoundsError(j, 1, i13, &wb_emlrtBCI);
    }

    if (tau->data[j - 1] != 0.0) {
      if ((!(1 > nb)) && (nb > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (k = 1; k <= nb; k++) {
        i13 = b_B->size[0];
        if (!((j >= 1) && (j <= i13))) {
          rtDynamicBoundsError(j, 1, i13, &vc_emlrtBCI);
        }

        i13 = b_B->size[1];
        if (!((k >= 1) && (k <= i13))) {
          rtDynamicBoundsError(k, 1, i13, &wc_emlrtBCI);
        }

        tol = b_B->data[(j + b_B->size[0] * (k - 1)) - 1];
        if ((!(j + 1 > maxmn)) && (maxmn > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (i = j + 1; i <= maxmn; i++) {
          i13 = b_A->size[0];
          if (!((i >= 1) && (i <= i13))) {
            rtDynamicBoundsError(i, 1, i13, &vb_emlrtBCI);
          }

          i13 = b_A->size[1];
          if (!((j >= 1) && (j <= i13))) {
            rtDynamicBoundsError(j, 1, i13, &ub_emlrtBCI);
          }

          i13 = b_B->size[0];
          if (!((i >= 1) && (i <= i13))) {
            rtDynamicBoundsError(i, 1, i13, &tb_emlrtBCI);
          }

          i13 = b_B->size[1];
          if (!((k >= 1) && (k <= i13))) {
            rtDynamicBoundsError(k, 1, i13, &sb_emlrtBCI);
          }

          tol += b_A->data[(i + b_A->size[0] * (j - 1)) - 1] * b_B->data[(i +
            b_B->size[0] * (k - 1)) - 1];
        }

        tol *= tau->data[j - 1];
        if (tol != 0.0) {
          i13 = b_B->size[0];
          if (!((j >= 1) && (j <= i13))) {
            rtDynamicBoundsError(j, 1, i13, &xc_emlrtBCI);
          }

          i13 = b_B->size[1];
          if (!((k >= 1) && (k <= i13))) {
            rtDynamicBoundsError(k, 1, i13, &yc_emlrtBCI);
          }

          i13 = b_B->size[0];
          if (!((j >= 1) && (j <= i13))) {
            rtDynamicBoundsError(j, 1, i13, &ad_emlrtBCI);
          }

          i13 = b_B->size[1];
          if (!((k >= 1) && (k <= i13))) {
            rtDynamicBoundsError(k, 1, i13, &bd_emlrtBCI);
          }

          b_B->data[(j + b_B->size[0] * (k - 1)) - 1] -= tol;
          for (i = j + 1; i <= maxmn; i++) {
            i13 = b_B->size[0];
            if (!((i >= 1) && (i <= i13))) {
              rtDynamicBoundsError(i, 1, i13, &cd_emlrtBCI);
            }

            i13 = b_B->size[1];
            if (!((k >= 1) && (k <= i13))) {
              rtDynamicBoundsError(k, 1, i13, &dd_emlrtBCI);
            }

            i13 = b_A->size[0];
            if (!((i >= 1) && (i <= i13))) {
              rtDynamicBoundsError(i, 1, i13, &ed_emlrtBCI);
            }

            i13 = b_A->size[1];
            if (!((j >= 1) && (j <= i13))) {
              rtDynamicBoundsError(j, 1, i13, &fd_emlrtBCI);
            }

            i13 = b_B->size[0];
            if (!((i >= 1) && (i <= i13))) {
              rtDynamicBoundsError(i, 1, i13, &gd_emlrtBCI);
            }

            i13 = b_B->size[1];
            if (!((k >= 1) && (k <= i13))) {
              rtDynamicBoundsError(k, 1, i13, &hd_emlrtBCI);
            }

            b_B->data[(i + b_B->size[0] * (k - 1)) - 1] -= b_A->data[(i +
              b_A->size[0] * (j - 1)) - 1] * tol;
          }
        }
      }
    }
  }

  emxFree_real_T(&tau);
  overflow = ((!(1 > B->size[1])) && (B->size[1] > 2147483646));
  if (overflow) {
    check_forloop_overflow_error();
  }

  for (k = 1; k <= B->size[1]; k++) {
    if ((!(1 > rankR)) && (rankR > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (i = 1; i <= rankR; i++) {
      i13 = b_B->size[0];
      if (!((i >= 1) && (i <= i13))) {
        rtDynamicBoundsError(i, 1, i13, &rc_emlrtBCI);
      }

      i13 = b_B->size[1];
      if (!((k >= 1) && (k <= i13))) {
        rtDynamicBoundsError(k, 1, i13, &sc_emlrtBCI);
      }

      i13 = Y->size[0];
      minmn = jpvt->size[1];
      if (!((i >= 1) && (i <= minmn))) {
        rtDynamicBoundsError(i, 1, minmn, &tc_emlrtBCI);
      }

      minmn = jpvt->data[i - 1];
      if (!((minmn >= 1) && (minmn <= i13))) {
        rtDynamicBoundsError(minmn, 1, i13, &tc_emlrtBCI);
      }

      i13 = Y->size[1];
      if (!((k >= 1) && (k <= i13))) {
        rtDynamicBoundsError(k, 1, i13, &uc_emlrtBCI);
      }

      Y->data[(minmn + Y->size[0] * (k - 1)) - 1] = b_B->data[(i + b_B->size[0] *
        (k - 1)) - 1];
    }

    for (j = rankR - 1; j + 1 > 0; j--) {
      i13 = jpvt->size[1];
      if (!((j + 1 >= 1) && (j + 1 <= i13))) {
        rtDynamicBoundsError(j + 1, 1, i13, &cc_emlrtBCI);
      }

      i13 = Y->size[0];
      minmn = jpvt->data[j];
      if (!((minmn >= 1) && (minmn <= i13))) {
        rtDynamicBoundsError(minmn, 1, i13, &bc_emlrtBCI);
      }

      i13 = Y->size[1];
      if (!((k >= 1) && (k <= i13))) {
        rtDynamicBoundsError(k, 1, i13, &ac_emlrtBCI);
      }

      i13 = b_A->size[0];
      if (!((j + 1 >= 1) && (j + 1 <= i13))) {
        rtDynamicBoundsError(j + 1, 1, i13, &yb_emlrtBCI);
      }

      i13 = b_A->size[1];
      if (!((j + 1 >= 1) && (j + 1 <= i13))) {
        rtDynamicBoundsError(j + 1, 1, i13, &xb_emlrtBCI);
      }

      i13 = Y->size[0];
      minmn = jpvt->data[j];
      if (!((minmn >= 1) && (minmn <= i13))) {
        rtDynamicBoundsError(minmn, 1, i13, &hc_emlrtBCI);
      }

      i13 = Y->size[1];
      if (!((k >= 1) && (k <= i13))) {
        rtDynamicBoundsError(k, 1, i13, &ic_emlrtBCI);
      }

      Y->data[(minmn + Y->size[0] * (k - 1)) - 1] = Y->data[(jpvt->data[j] +
        Y->size[0] * (k - 1)) - 1] / b_A->data[j + b_A->size[0] * j];
      for (i = 1; i <= j; i++) {
        i13 = Y->size[0];
        minmn = jpvt->size[1];
        if (!((i >= 1) && (i <= minmn))) {
          rtDynamicBoundsError(i, 1, minmn, &jc_emlrtBCI);
        }

        minmn = jpvt->data[i - 1];
        if (!((minmn >= 1) && (minmn <= i13))) {
          rtDynamicBoundsError(minmn, 1, i13, &jc_emlrtBCI);
        }

        i13 = Y->size[1];
        if (!((k >= 1) && (k <= i13))) {
          rtDynamicBoundsError(k, 1, i13, &kc_emlrtBCI);
        }

        i13 = Y->size[0];
        maxmn = jpvt->data[j];
        if (!((maxmn >= 1) && (maxmn <= i13))) {
          rtDynamicBoundsError(maxmn, 1, i13, &lc_emlrtBCI);
        }

        i13 = Y->size[1];
        if (!((k >= 1) && (k <= i13))) {
          rtDynamicBoundsError(k, 1, i13, &mc_emlrtBCI);
        }

        i13 = b_A->size[0];
        if (!((i >= 1) && (i <= i13))) {
          rtDynamicBoundsError(i, 1, i13, &nc_emlrtBCI);
        }

        i13 = b_A->size[1];
        if (!((j + 1 >= 1) && (j + 1 <= i13))) {
          rtDynamicBoundsError(j + 1, 1, i13, &oc_emlrtBCI);
        }

        i13 = Y->size[0];
        nb = jpvt->size[1];
        if (!((i >= 1) && (i <= nb))) {
          rtDynamicBoundsError(i, 1, nb, &pc_emlrtBCI);
        }

        nb = jpvt->data[i - 1];
        if (!((nb >= 1) && (nb <= i13))) {
          rtDynamicBoundsError(nb, 1, i13, &pc_emlrtBCI);
        }

        i13 = Y->size[1];
        if (!((k >= 1) && (k <= i13))) {
          rtDynamicBoundsError(k, 1, i13, &qc_emlrtBCI);
        }

        Y->data[(nb + Y->size[0] * (k - 1)) - 1] = Y->data[(minmn + Y->size[0] *
          (k - 1)) - 1] - Y->data[(maxmn + Y->size[0] * (k - 1)) - 1] *
          b_A->data[(i + b_A->size[0] * j) - 1];
      }
    }
  }

  emxFree_real_T(&b_B);
  emxFree_int32_T(&jpvt);
  emxFree_real_T(&b_A);
}

//
// Arguments    : int aIndexValue
//                int aLoBound
//                int aHiBound
//                const rtBoundsCheckInfo *aInfo
// Return Type  : void
//
static void rtDynamicBoundsError(int aIndexValue, int aLoBound, int aHiBound,
  const rtBoundsCheckInfo *aInfo)
{
  if (aLoBound == 0) {
    aIndexValue++;
    aLoBound = 1;
    aHiBound++;
  }

  if (rtIsNullOrEmptyString(aInfo->aName)) {
    fprintf(stderr,
            "Index exceeds array dimensions.  Index value %d exceeds valid range [%d-%d].",
            aIndexValue, aLoBound, aHiBound);
    fprintf(stderr, "\n");
  } else {
    fprintf(stderr,
            "Index exceeds array dimensions.  Index value %d exceeds valid range [%d-%d] of array %s.",
            aIndexValue, aLoBound, aHiBound, aInfo->aName);
    fprintf(stderr, "\n");
  }

  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : const rtRunTimeErrorInfo *aInfo
// Return Type  : void
//
static void rtErrorWithMessageID(const rtRunTimeErrorInfo *aInfo)
{
  fprintf(stderr, "Matrix must be square.");
  fprintf(stderr, "\n");
  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

//
// Arguments    : const char *aString
// Return Type  : boolean_T
//
static boolean_T rtIsNullOrEmptyString(const char *aString)
{
  return (aString == NULL) || (*aString == '\x00');
}

//
// Arguments    : const char * aFcnName
//                const int aLineNo
// Return Type  : void
//
static void rtReportErrorLocation(const char * aFcnName, const int aLineNo)
{
  fprintf(stderr, "Error in %s (line %d)", aFcnName, aLineNo);
  fprintf(stderr, "\n");
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *U
//                emxArray_real_T *S
//                emxArray_real_T *V
// Return Type  : void
//
static void svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *S,
                emxArray_real_T *V)
{
  emxArray_real_T *b_A;
  int i22;
  int ns;
  int n;
  int p;
  int minnp;
  emxArray_real_T *s;
  emxArray_real_T *e;
  emxArray_real_T *work;
  emxArray_real_T *Vf;
  int qs;
  int nrt;
  int nct;
  int q;
  int pmq;
  int m;
  int nmq;
  boolean_T apply_transform;
  double ztest0;
  int k;
  int mm;
  double snorm;
  boolean_T exitg1;
  double ztest;
  boolean_T exitg5;
  boolean_T exitg4;
  double f;
  boolean_T exitg3;
  double varargin_1[5];
  double mtmp;
  boolean_T exitg2;
  double sqds;
  double b;
  emxInit_real_T(&b_A, 2);
  i22 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = A->size[1];
  emxEnsureCapacity((emxArray__common *)b_A, i22, (int)sizeof(double));
  ns = A->size[0] * A->size[1];
  for (i22 = 0; i22 < ns; i22++) {
    b_A->data[i22] = A->data[i22];
  }

  n = A->size[0];
  p = A->size[1];
  if (A->size[0] + 1 <= A->size[1]) {
    ns = A->size[0] + 1;
  } else {
    ns = A->size[1];
  }

  if (A->size[0] <= A->size[1]) {
    minnp = A->size[0];
  } else {
    minnp = A->size[1];
  }

  emxInit_real_T1(&s, 1);
  i22 = s->size[0];
  s->size[0] = ns;
  emxEnsureCapacity((emxArray__common *)s, i22, (int)sizeof(double));
  for (i22 = 0; i22 < ns; i22++) {
    s->data[i22] = 0.0;
  }

  emxInit_real_T1(&e, 1);
  ns = A->size[1];
  i22 = e->size[0];
  e->size[0] = ns;
  emxEnsureCapacity((emxArray__common *)e, i22, (int)sizeof(double));
  for (i22 = 0; i22 < ns; i22++) {
    e->data[i22] = 0.0;
  }

  emxInit_real_T1(&work, 1);
  ns = A->size[0];
  i22 = work->size[0];
  work->size[0] = ns;
  emxEnsureCapacity((emxArray__common *)work, i22, (int)sizeof(double));
  for (i22 = 0; i22 < ns; i22++) {
    work->data[i22] = 0.0;
  }

  ns = A->size[0];
  i22 = U->size[0] * U->size[1];
  U->size[0] = ns;
  U->size[1] = minnp;
  emxEnsureCapacity((emxArray__common *)U, i22, (int)sizeof(double));
  ns *= minnp;
  for (i22 = 0; i22 < ns; i22++) {
    U->data[i22] = 0.0;
  }

  emxInit_real_T(&Vf, 2);
  ns = A->size[1];
  qs = A->size[1];
  i22 = Vf->size[0] * Vf->size[1];
  Vf->size[0] = ns;
  Vf->size[1] = qs;
  emxEnsureCapacity((emxArray__common *)Vf, i22, (int)sizeof(double));
  ns *= qs;
  for (i22 = 0; i22 < ns; i22++) {
    Vf->data[i22] = 0.0;
  }

  if ((A->size[0] == 0) || (A->size[1] == 0)) {
    if (A->size[0] <= minnp) {
      qs = A->size[0];
    } else {
      qs = minnp;
    }

    if ((!(1 > qs)) && (qs > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (ns = 1; ns <= qs; ns++) {
      i22 = U->size[0];
      if (!((ns >= 1) && (ns <= i22))) {
        rtDynamicBoundsError(ns, 1, i22, &gk_emlrtBCI);
      }

      i22 = U->size[1];
      if (!((ns >= 1) && (ns <= i22))) {
        rtDynamicBoundsError(ns, 1, i22, &hk_emlrtBCI);
      }

      U->data[(ns + U->size[0] * (ns - 1)) - 1] = 1.0;
    }

    qs = A->size[1];
    if ((!(1 > qs)) && (qs > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (ns = 1; ns <= qs; ns++) {
      i22 = Vf->size[0];
      if (!((ns >= 1) && (ns <= i22))) {
        rtDynamicBoundsError(ns, 1, i22, &ek_emlrtBCI);
      }

      i22 = Vf->size[1];
      if (!((ns >= 1) && (ns <= i22))) {
        rtDynamicBoundsError(ns, 1, i22, &fk_emlrtBCI);
      }

      Vf->data[(ns + Vf->size[0] * (ns - 1)) - 1] = 1.0;
    }
  } else {
    if (A->size[1] > 2) {
      ns = A->size[1] - 2;
    } else {
      ns = 0;
    }

    if (ns <= A->size[0]) {
      nrt = ns;
    } else {
      nrt = A->size[0];
    }

    ns = A->size[0];
    if (ns >= 1) {
    } else {
      ns = 1;
    }

    if (ns - 1 <= A->size[1]) {
      nct = ns - 1;
    } else {
      nct = A->size[1];
    }

    if (nct >= nrt) {
      qs = nct;
    } else {
      qs = nrt;
    }

    for (q = 1; q <= qs; q++) {
      pmq = q + n * (q - 1);
      nmq = n - q;
      apply_transform = false;
      if (q <= nct) {
        ztest0 = c_xnrm2(nmq + 1, b_A, pmq);
        if (ztest0 > 0.0) {
          apply_transform = true;
          i22 = b_A->size[0] * b_A->size[1];
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &of_emlrtBCI);
          }

          if (b_A->data[pmq - 1] < 0.0) {
            ztest0 = -ztest0;
          }

          i22 = s->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &ti_emlrtBCI);
          }

          s->data[q - 1] = ztest0;
          i22 = s->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &nf_emlrtBCI);
          }

          if (fabs(s->data[q - 1]) >= 1.0020841800044864E-292) {
            xscal(nmq + 1, 1.0 / s->data[q - 1], b_A, pmq);
          } else {
            ns = pmq + nmq;
            if ((!(pmq > ns)) && (ns > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (k = pmq; k <= ns; k++) {
              i22 = b_A->size[0] * b_A->size[1];
              if (!((k >= 1) && (k <= i22))) {
                rtDynamicBoundsError(k, 1, i22, &fe_emlrtBCI);
              }

              i22 = b_A->size[0] * b_A->size[1];
              if (!((k >= 1) && (k <= i22))) {
                rtDynamicBoundsError(k, 1, i22, &ui_emlrtBCI);
              }

              b_A->data[k - 1] /= s->data[q - 1];
            }
          }

          i22 = b_A->size[0] * b_A->size[1];
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &vi_emlrtBCI);
          }

          i22 = b_A->size[0] * b_A->size[1];
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &wi_emlrtBCI);
          }

          b_A->data[pmq - 1]++;
          i22 = s->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &xi_emlrtBCI);
          }

          i22 = s->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &yi_emlrtBCI);
          }

          s->data[q - 1] = -s->data[q - 1];
        } else {
          i22 = s->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &si_emlrtBCI);
          }

          s->data[q - 1] = 0.0;
        }
      }

      if ((!(q + 1 > p)) && (p > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (mm = q + 1; mm <= p; mm++) {
        ns = q + n * (mm - 1);
        if (apply_transform) {
          ztest0 = xdotc(nmq + 1, b_A, pmq, b_A, ns);
          i22 = b_A->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &mf_emlrtBCI);
          }

          i22 = b_A->size[1];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &lf_emlrtBCI);
          }

          ztest0 = -(ztest0 / b_A->data[(q + b_A->size[0] * (q - 1)) - 1]);
          xaxpy(nmq + 1, ztest0, pmq, b_A, ns);
        }

        i22 = b_A->size[0] * b_A->size[1];
        if (!((ns >= 1) && (ns <= i22))) {
          rtDynamicBoundsError(ns, 1, i22, &sj_emlrtBCI);
        }

        i22 = e->size[0];
        if (!((mm >= 1) && (mm <= i22))) {
          rtDynamicBoundsError(mm, 1, i22, &tj_emlrtBCI);
        }

        e->data[mm - 1] = b_A->data[ns - 1];
      }

      if (q <= nct) {
        if ((!(q > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = q; ns <= n; ns++) {
          i22 = b_A->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &oj_emlrtBCI);
          }

          i22 = b_A->size[1];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &pj_emlrtBCI);
          }

          i22 = U->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &qj_emlrtBCI);
          }

          i22 = U->size[1];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &rj_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (q - 1)) - 1] = b_A->data[(ns + b_A->size[0]
            * (q - 1)) - 1];
        }
      }

      if (q <= nrt) {
        pmq = p - q;
        ztest0 = d_xnrm2(pmq, e, q + 1);
        if (ztest0 == 0.0) {
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &kj_emlrtBCI);
          }

          e->data[q - 1] = 0.0;
        } else {
          i22 = e->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i22))) {
            rtDynamicBoundsError(q + 1, 1, i22, &kf_emlrtBCI);
          }

          if (e->data[q] < 0.0) {
            ztest0 = -ztest0;
          }

          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &aj_emlrtBCI);
          }

          e->data[q - 1] = ztest0;
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &bj_emlrtBCI);
          }

          ztest0 = e->data[q - 1];
          if (fabs(ztest0) >= 1.0020841800044864E-292) {
            ztest0 = 1.0 / ztest0;
            ns = q + pmq;
            if ((!(q + 1 > ns)) && (ns > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (k = q + 1; k <= ns; k++) {
              i22 = e->size[0];
              if (!((k >= 1) && (k <= i22))) {
                rtDynamicBoundsError(k, 1, i22, &ij_emlrtBCI);
              }

              i22 = e->size[0];
              if (!((k >= 1) && (k <= i22))) {
                rtDynamicBoundsError(k, 1, i22, &jj_emlrtBCI);
              }

              e->data[k - 1] *= ztest0;
            }
          } else {
            ns = q + pmq;
            if ((!(q + 1 > ns)) && (ns > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (k = q + 1; k <= ns; k++) {
              i22 = e->size[0];
              if (!((k >= 1) && (k <= i22))) {
                rtDynamicBoundsError(k, 1, i22, &fe_emlrtBCI);
              }

              i22 = e->size[0];
              if (!((k >= 1) && (k <= i22))) {
                rtDynamicBoundsError(k, 1, i22, &ui_emlrtBCI);
              }

              e->data[k - 1] /= ztest0;
            }
          }

          i22 = e->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i22))) {
            rtDynamicBoundsError(q + 1, 1, i22, &cj_emlrtBCI);
          }

          i22 = e->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i22))) {
            rtDynamicBoundsError(q + 1, 1, i22, &dj_emlrtBCI);
          }

          e->data[q]++;
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &ej_emlrtBCI);
          }

          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &fj_emlrtBCI);
          }

          e->data[q - 1] = -e->data[q - 1];
          if (q + 1 <= n) {
            if ((!(q + 1 > n)) && (n > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (ns = q + 1; ns <= n; ns++) {
              i22 = work->size[0];
              if (!((ns >= 1) && (ns <= i22))) {
                rtDynamicBoundsError(ns, 1, i22, &hj_emlrtBCI);
              }

              work->data[ns - 1] = 0.0;
            }

            for (mm = q + 1; mm <= p; mm++) {
              i22 = e->size[0];
              if (!((mm >= 1) && (mm <= i22))) {
                rtDynamicBoundsError(mm, 1, i22, &gj_emlrtBCI);
              }

              b_xaxpy(nmq, e->data[mm - 1], b_A, (q + n * (mm - 1)) + 1, work, q
                      + 1);
            }

            for (mm = q; mm + 1 <= p; mm++) {
              i22 = e->size[0];
              if (!((mm + 1 >= 1) && (mm + 1 <= i22))) {
                rtDynamicBoundsError(mm + 1, 1, i22, &jf_emlrtBCI);
              }

              i22 = e->size[0];
              if (!((q + 1 >= 1) && (q + 1 <= i22))) {
                rtDynamicBoundsError(q + 1, 1, i22, &if_emlrtBCI);
              }

              c_xaxpy(nmq, -e->data[mm] / e->data[q], work, q + 1, b_A, (q + n *
                       mm) + 1);
            }
          }
        }

        for (ns = q + 1; ns <= p; ns++) {
          i22 = e->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &lj_emlrtBCI);
          }

          i22 = Vf->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &mj_emlrtBCI);
          }

          i22 = Vf->size[1];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &nj_emlrtBCI);
          }

          Vf->data[(ns + Vf->size[0] * (q - 1)) - 1] = e->data[ns - 1];
        }
      }
    }

    if (A->size[1] <= A->size[0] + 1) {
      m = A->size[1];
    } else {
      m = A->size[0] + 1;
    }

    if (nct < A->size[1]) {
      i22 = b_A->size[0];
      if (!((nct + 1 >= 1) && (nct + 1 <= i22))) {
        rtDynamicBoundsError(nct + 1, 1, i22, &pf_emlrtBCI);
      }

      i22 = b_A->size[1];
      if (!((nct + 1 >= 1) && (nct + 1 <= i22))) {
        rtDynamicBoundsError(nct + 1, 1, i22, &qf_emlrtBCI);
      }

      i22 = s->size[0];
      if (!((nct + 1 >= 1) && (nct + 1 <= i22))) {
        rtDynamicBoundsError(nct + 1, 1, i22, &rf_emlrtBCI);
      }

      s->data[nct] = b_A->data[nct + b_A->size[0] * nct];
    }

    if (A->size[0] < m) {
      i22 = s->size[0];
      if (!((m >= 1) && (m <= i22))) {
        rtDynamicBoundsError(m, 1, i22, &sf_emlrtBCI);
      }

      s->data[m - 1] = 0.0;
    }

    if (nrt + 1 < m) {
      i22 = b_A->size[0];
      if (!((nrt + 1 >= 1) && (nrt + 1 <= i22))) {
        rtDynamicBoundsError(nrt + 1, 1, i22, &tf_emlrtBCI);
      }

      i22 = b_A->size[1];
      if (!((m >= 1) && (m <= i22))) {
        rtDynamicBoundsError(m, 1, i22, &uf_emlrtBCI);
      }

      i22 = e->size[0];
      if (!((nrt + 1 >= 1) && (nrt + 1 <= i22))) {
        rtDynamicBoundsError(nrt + 1, 1, i22, &vf_emlrtBCI);
      }

      e->data[nrt] = b_A->data[nrt + b_A->size[0] * (m - 1)];
    }

    i22 = e->size[0];
    if (!((m >= 1) && (m <= i22))) {
      rtDynamicBoundsError(m, 1, i22, &wf_emlrtBCI);
    }

    e->data[m - 1] = 0.0;
    if (nct + 1 <= minnp) {
      if ((!(nct + 1 > minnp)) && (minnp > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (mm = nct + 1; mm <= minnp; mm++) {
        if ((!(1 > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = 1; ns <= n; ns++) {
          i22 = U->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &qi_emlrtBCI);
          }

          i22 = U->size[1];
          if (!((mm >= 1) && (mm <= i22))) {
            rtDynamicBoundsError(mm, 1, i22, &ri_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (mm - 1)) - 1] = 0.0;
        }

        i22 = U->size[0];
        if (!((mm >= 1) && (mm <= i22))) {
          rtDynamicBoundsError(mm, 1, i22, &oi_emlrtBCI);
        }

        i22 = U->size[1];
        if (!((mm >= 1) && (mm <= i22))) {
          rtDynamicBoundsError(mm, 1, i22, &pi_emlrtBCI);
        }

        U->data[(mm + U->size[0] * (mm - 1)) - 1] = 1.0;
      }
    }

    while (nct > 0) {
      nmq = (n - nct) + 1;
      pmq = nct + n * (nct - 1);
      i22 = s->size[0];
      if (!(nct <= i22)) {
        rtDynamicBoundsError(nct, 1, i22, &ci_emlrtBCI);
      }

      if (s->data[nct - 1] != 0.0) {
        for (mm = nct; mm + 1 <= minnp; mm++) {
          ns = nct + n * mm;
          ztest0 = xdotc(nmq, U, pmq, U, ns);
          i22 = U->size[0] * U->size[1];
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &hf_emlrtBCI);
          }

          ztest0 = -(ztest0 / U->data[pmq - 1]);
          xaxpy(nmq, ztest0, pmq, U, ns);
        }

        if ((!(nct > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = nct; ns <= n; ns++) {
          i22 = U->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &ki_emlrtBCI);
          }

          i22 = U->size[1];
          if (!(nct <= i22)) {
            rtDynamicBoundsError(nct, 1, i22, &li_emlrtBCI);
          }

          i22 = U->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &mi_emlrtBCI);
          }

          i22 = U->size[1];
          if (!(nct <= i22)) {
            rtDynamicBoundsError(nct, 1, i22, &ni_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (nct - 1)) - 1] = -U->data[(ns + U->size[0]
            * (nct - 1)) - 1];
        }

        i22 = U->size[0] * U->size[1];
        if (!((pmq >= 1) && (pmq <= i22))) {
          rtDynamicBoundsError(pmq, 1, i22, &gi_emlrtBCI);
        }

        i22 = U->size[0] * U->size[1];
        if (!((pmq >= 1) && (pmq <= i22))) {
          rtDynamicBoundsError(pmq, 1, i22, &hi_emlrtBCI);
        }

        U->data[pmq - 1]++;
        for (ns = 1; ns < nct; ns++) {
          i22 = U->size[0];
          if (!(ns <= i22)) {
            rtDynamicBoundsError(ns, 1, i22, &ii_emlrtBCI);
          }

          i22 = U->size[1];
          if (!(nct <= i22)) {
            rtDynamicBoundsError(nct, 1, i22, &ji_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (nct - 1)) - 1] = 0.0;
        }
      } else {
        if ((!(1 > n)) && (n > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ns = 1; ns <= n; ns++) {
          i22 = U->size[0];
          if (!((ns >= 1) && (ns <= i22))) {
            rtDynamicBoundsError(ns, 1, i22, &ei_emlrtBCI);
          }

          i22 = U->size[1];
          if (!(nct <= i22)) {
            rtDynamicBoundsError(nct, 1, i22, &fi_emlrtBCI);
          }

          U->data[(ns + U->size[0] * (nct - 1)) - 1] = 0.0;
        }

        i22 = U->size[0] * U->size[1];
        if (!((pmq >= 1) && (pmq <= i22))) {
          rtDynamicBoundsError(pmq, 1, i22, &di_emlrtBCI);
        }

        U->data[pmq - 1] = 1.0;
      }

      nct--;
    }

    for (q = A->size[1]; q > 0; q--) {
      if (q <= nrt) {
        i22 = e->size[0];
        if (!(q <= i22)) {
          rtDynamicBoundsError(q, 1, i22, &wh_emlrtBCI);
        }

        if (e->data[q - 1] != 0.0) {
          pmq = p - q;
          ns = (q + p * (q - 1)) + 1;
          if ((!(q + 1 > p)) && (p > 2147483646)) {
            check_forloop_overflow_error();
          }

          for (mm = q; mm + 1 <= p; mm++) {
            qs = (q + p * mm) + 1;
            ztest0 = xdotc(pmq, Vf, ns, Vf, qs);
            i22 = Vf->size[0] * Vf->size[1];
            if (!((ns >= 1) && (ns <= i22))) {
              rtDynamicBoundsError(ns, 1, i22, &gf_emlrtBCI);
            }

            ztest0 = -(ztest0 / Vf->data[ns - 1]);
            xaxpy(pmq, ztest0, ns, Vf, qs);
          }
        }
      }

      if ((!(1 > p)) && (p > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (ns = 1; ns <= p; ns++) {
        i22 = Vf->size[0];
        if (!((ns >= 1) && (ns <= i22))) {
          rtDynamicBoundsError(ns, 1, i22, &ai_emlrtBCI);
        }

        i22 = Vf->size[1];
        if (!(q <= i22)) {
          rtDynamicBoundsError(q, 1, i22, &bi_emlrtBCI);
        }

        Vf->data[(ns + Vf->size[0] * (q - 1)) - 1] = 0.0;
      }

      i22 = Vf->size[0];
      if (!(q <= i22)) {
        rtDynamicBoundsError(q, 1, i22, &xh_emlrtBCI);
      }

      i22 = Vf->size[1];
      if (!(q <= i22)) {
        rtDynamicBoundsError(q, 1, i22, &yh_emlrtBCI);
      }

      Vf->data[(q + Vf->size[0] * (q - 1)) - 1] = 1.0;
    }

    for (q = 1; q <= m; q++) {
      i22 = s->size[0];
      if (!((q >= 1) && (q <= i22))) {
        rtDynamicBoundsError(q, 1, i22, &ph_emlrtBCI);
      }

      if (s->data[q - 1] != 0.0) {
        i22 = s->size[0];
        if (!((q >= 1) && (q <= i22))) {
          rtDynamicBoundsError(q, 1, i22, &ff_emlrtBCI);
        }

        ztest = fabs(s->data[q - 1]);
        i22 = s->size[0];
        if (!((q >= 1) && (q <= i22))) {
          rtDynamicBoundsError(q, 1, i22, &ef_emlrtBCI);
        }

        ztest0 = s->data[q - 1] / ztest;
        i22 = s->size[0];
        if (!((q >= 1) && (q <= i22))) {
          rtDynamicBoundsError(q, 1, i22, &qh_emlrtBCI);
        }

        s->data[q - 1] = ztest;
        if (q < m) {
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &df_emlrtBCI);
          }

          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &rh_emlrtBCI);
          }

          e->data[q - 1] /= ztest0;
        }

        if (q <= n) {
          xscal(n, ztest0, U, 1 + n * (q - 1));
        }
      }

      if (q < m) {
        i22 = e->size[0];
        if (!((q >= 1) && (q <= i22))) {
          rtDynamicBoundsError(q, 1, i22, &sh_emlrtBCI);
        }

        if (e->data[q - 1] != 0.0) {
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &cf_emlrtBCI);
          }

          ztest = fabs(e->data[q - 1]);
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &bf_emlrtBCI);
          }

          ztest0 = ztest / e->data[q - 1];
          i22 = e->size[0];
          if (!((q >= 1) && (q <= i22))) {
            rtDynamicBoundsError(q, 1, i22, &th_emlrtBCI);
          }

          e->data[q - 1] = ztest;
          i22 = s->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i22))) {
            rtDynamicBoundsError(q + 1, 1, i22, &uh_emlrtBCI);
          }

          i22 = s->size[0];
          if (!((q + 1 >= 1) && (q + 1 <= i22))) {
            rtDynamicBoundsError(q + 1, 1, i22, &vh_emlrtBCI);
          }

          s->data[q] *= ztest0;
          xscal(p, ztest0, Vf, 1 + p * q);
        }
      }
    }

    mm = m;
    nct = 0;
    snorm = 0.0;
    for (ns = 1; ns <= m; ns++) {
      i22 = s->size[0];
      if (!(ns <= i22)) {
        rtDynamicBoundsError(ns, 1, i22, &af_emlrtBCI);
      }

      ztest0 = fabs(s->data[ns - 1]);
      i22 = e->size[0];
      if (!(ns <= i22)) {
        rtDynamicBoundsError(ns, 1, i22, &ye_emlrtBCI);
      }

      ztest = fabs(e->data[ns - 1]);
      if ((ztest0 >= ztest) || rtIsNaN(ztest)) {
      } else {
        ztest0 = ztest;
      }

      if ((snorm >= ztest0) || rtIsNaN(ztest0)) {
      } else {
        snorm = ztest0;
      }
    }

    exitg1 = false;
    while ((!exitg1) && (m > 0)) {
      if (nct >= 75) {
        c_error();
      } else {
        q = m - 1;
        exitg5 = false;
        while (!(exitg5 || (q == 0))) {
          i22 = s->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &xe_emlrtBCI);
          }

          i22 = s->size[0];
          if (!(q + 1 <= i22)) {
            rtDynamicBoundsError(q + 1, 1, i22, &we_emlrtBCI);
          }

          i22 = e->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &ve_emlrtBCI);
          }

          ztest0 = fabs(e->data[q - 1]);
          if ((ztest0 <= 2.2204460492503131E-16 * (fabs(s->data[q - 1]) + fabs
                (s->data[q]))) || (ztest0 <= 1.0020841800044864E-292) || ((nct >
                20) && (ztest0 <= 2.2204460492503131E-16 * snorm))) {
            i22 = e->size[0];
            if (!(q <= i22)) {
              rtDynamicBoundsError(q, 1, i22, &xf_emlrtBCI);
            }

            e->data[q - 1] = 0.0;
            exitg5 = true;
          } else {
            q--;
          }
        }

        if (q == m - 1) {
          ns = 4;
        } else {
          qs = m;
          ns = m;
          exitg4 = false;
          while ((!exitg4) && (ns >= q)) {
            qs = ns;
            if (ns == q) {
              exitg4 = true;
            } else {
              ztest0 = 0.0;
              if (ns < m) {
                i22 = e->size[0];
                if (!((ns >= 1) && (ns <= i22))) {
                  rtDynamicBoundsError(ns, 1, i22, &ue_emlrtBCI);
                }

                ztest0 = fabs(e->data[ns - 1]);
              }

              if (ns > q + 1) {
                i22 = e->size[0];
                pmq = ns - 1;
                if (!(pmq <= i22)) {
                  rtDynamicBoundsError(pmq, 1, i22, &te_emlrtBCI);
                }

                ztest0 += fabs(e->data[ns - 2]);
              }

              i22 = s->size[0];
              if (!((ns >= 1) && (ns <= i22))) {
                rtDynamicBoundsError(ns, 1, i22, &se_emlrtBCI);
              }

              ztest = fabs(s->data[ns - 1]);
              if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
                   1.0020841800044864E-292)) {
                i22 = s->size[0];
                if (!((ns >= 1) && (ns <= i22))) {
                  rtDynamicBoundsError(ns, 1, i22, &yf_emlrtBCI);
                }

                s->data[ns - 1] = 0.0;
                exitg4 = true;
              } else {
                ns--;
              }
            }
          }

          if (qs == q) {
            ns = 3;
          } else if (qs == m) {
            ns = 1;
          } else {
            ns = 2;
            q = qs;
          }
        }

        q++;
        switch (ns) {
         case 1:
          i22 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &jg_emlrtBCI);
          }

          f = e->data[pmq - 1];
          i22 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &kg_emlrtBCI);
          }

          e->data[pmq - 1] = 0.0;
          for (k = m - 1; k >= q; k--) {
            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &re_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &lg_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &lg_emlrtBCI);
            }

            xrotg(&s->data[k - 1], &f, &ztest0, &ztest);
            if (k > q) {
              i22 = e->size[0];
              pmq = k - 1;
              if (!(pmq <= i22)) {
                rtDynamicBoundsError(pmq, 1, i22, &mg_emlrtBCI);
              }

              f = -ztest * e->data[pmq - 1];
              i22 = e->size[0];
              pmq = k - 1;
              if (!(pmq <= i22)) {
                rtDynamicBoundsError(pmq, 1, i22, &ng_emlrtBCI);
              }

              i22 = e->size[0];
              ns = k - 1;
              if (!(ns <= i22)) {
                rtDynamicBoundsError(ns, 1, i22, &og_emlrtBCI);
              }

              e->data[ns - 1] = e->data[pmq - 1] * ztest0;
            }

            xrot(p, Vf, 1 + p * (k - 1), 1 + p * (m - 1), ztest0, ztest);
          }
          break;

         case 2:
          i22 = e->size[0];
          pmq = q - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &pg_emlrtBCI);
          }

          f = e->data[pmq - 1];
          i22 = e->size[0];
          pmq = q - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &qg_emlrtBCI);
          }

          e->data[pmq - 1] = 0.0;
          for (k = q; k <= m; k++) {
            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &qe_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &rg_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &rg_emlrtBCI);
            }

            xrotg(&s->data[k - 1], &f, &ztest0, &ztest);
            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &sg_emlrtBCI);
            }

            f = -ztest * e->data[k - 1];
            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &tg_emlrtBCI);
            }

            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &ug_emlrtBCI);
            }

            e->data[k - 1] *= ztest0;
            xrot(n, U, 1 + n * (k - 1), 1 + n * (q - 2), ztest0, ztest);
          }
          break;

         case 3:
          i22 = s->size[0];
          if (!(m <= i22)) {
            rtDynamicBoundsError(m, 1, i22, &pe_emlrtBCI);
          }

          i22 = s->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &oe_emlrtBCI);
          }

          i22 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &ne_emlrtBCI);
          }

          i22 = s->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &me_emlrtBCI);
          }

          i22 = e->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &le_emlrtBCI);
          }

          varargin_1[0] = fabs(s->data[m - 1]);
          varargin_1[1] = fabs(s->data[m - 2]);
          varargin_1[2] = fabs(e->data[m - 2]);
          varargin_1[3] = fabs(s->data[q - 1]);
          varargin_1[4] = fabs(e->data[q - 1]);
          ns = 1;
          mtmp = varargin_1[0];
          if (rtIsNaN(varargin_1[0])) {
            qs = 2;
            exitg2 = false;
            while ((!exitg2) && (qs < 6)) {
              ns = qs;
              if (!rtIsNaN(varargin_1[qs - 1])) {
                mtmp = varargin_1[qs - 1];
                exitg2 = true;
              } else {
                qs++;
              }
            }
          }

          if (ns < 5) {
            while (ns + 1 < 6) {
              if (varargin_1[ns] > mtmp) {
                mtmp = varargin_1[ns];
              }

              ns++;
            }
          }

          i22 = s->size[0];
          if (!(m <= i22)) {
            rtDynamicBoundsError(m, 1, i22, &ke_emlrtBCI);
          }

          f = s->data[m - 1] / mtmp;
          i22 = s->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &je_emlrtBCI);
          }

          ztest0 = s->data[m - 2] / mtmp;
          i22 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &ie_emlrtBCI);
          }

          ztest = e->data[m - 2] / mtmp;
          i22 = s->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &he_emlrtBCI);
          }

          sqds = s->data[q - 1] / mtmp;
          i22 = e->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &ge_emlrtBCI);
          }

          b = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
          ztest0 = f * ztest;
          ztest0 *= ztest0;
          if ((b != 0.0) || (ztest0 != 0.0)) {
            ztest = b * b + ztest0;
            c_sqrt(&ztest);
            if (b < 0.0) {
              ztest = -ztest;
            }

            ztest = ztest0 / (b + ztest);
          } else {
            ztest = 0.0;
          }

          f = (sqds + f) * (sqds - f) + ztest;
          b = sqds * (e->data[q - 1] / mtmp);
          for (k = q; k < m; k++) {
            xrotg(&f, &b, &ztest0, &ztest);
            if (k > q) {
              i22 = e->size[0];
              pmq = k - 1;
              if (!(pmq <= i22)) {
                rtDynamicBoundsError(pmq, 1, i22, &wg_emlrtBCI);
              }

              e->data[pmq - 1] = f;
            }

            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &xg_emlrtBCI);
            }

            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &yg_emlrtBCI);
            }

            f = ztest0 * s->data[k - 1] + ztest * e->data[k - 1];
            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &ah_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &bh_emlrtBCI);
            }

            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &ch_emlrtBCI);
            }

            e->data[k - 1] = ztest0 * e->data[k - 1] - ztest * s->data[k - 1];
            i22 = s->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &dh_emlrtBCI);
            }

            b = ztest * s->data[k];
            i22 = s->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &eh_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &fh_emlrtBCI);
            }

            s->data[k] *= ztest0;
            xrot(p, Vf, 1 + p * (k - 1), 1 + p * k, ztest0, ztest);
            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &gh_emlrtBCI);
            }

            s->data[k - 1] = f;
            i22 = s->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &gh_emlrtBCI);
            }

            xrotg(&s->data[k - 1], &b, &ztest0, &ztest);
            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &hh_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &ih_emlrtBCI);
            }

            f = ztest0 * e->data[k - 1] + ztest * s->data[k];
            i22 = e->size[0];
            if (!(k <= i22)) {
              rtDynamicBoundsError(k, 1, i22, &jh_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &kh_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &lh_emlrtBCI);
            }

            s->data[k] = -ztest * e->data[k - 1] + ztest0 * s->data[k];
            i22 = e->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &mh_emlrtBCI);
            }

            b = ztest * e->data[k];
            i22 = e->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &nh_emlrtBCI);
            }

            i22 = e->size[0];
            if (!(k + 1 <= i22)) {
              rtDynamicBoundsError(k + 1, 1, i22, &oh_emlrtBCI);
            }

            e->data[k] *= ztest0;
            if (k < n) {
              xrot(n, U, 1 + n * (k - 1), 1 + n * k, ztest0, ztest);
            }
          }

          i22 = e->size[0];
          pmq = m - 1;
          if (!((pmq >= 1) && (pmq <= i22))) {
            rtDynamicBoundsError(pmq, 1, i22, &vg_emlrtBCI);
          }

          e->data[pmq - 1] = f;
          nct++;
          break;

         default:
          i22 = s->size[0];
          if (!(q <= i22)) {
            rtDynamicBoundsError(q, 1, i22, &ag_emlrtBCI);
          }

          if (s->data[q - 1] < 0.0) {
            i22 = s->size[0];
            if (!(q <= i22)) {
              rtDynamicBoundsError(q, 1, i22, &bg_emlrtBCI);
            }

            i22 = s->size[0];
            if (!(q <= i22)) {
              rtDynamicBoundsError(q, 1, i22, &cg_emlrtBCI);
            }

            s->data[q - 1] = -s->data[q - 1];
            xscal(p, -1.0, Vf, 1 + p * (q - 1));
          }

          ns = q + 1;
          exitg3 = false;
          while ((!exitg3) && (q < mm)) {
            i22 = s->size[0];
            if (!((q >= 1) && (q <= i22))) {
              rtDynamicBoundsError(q, 1, i22, &dg_emlrtBCI);
            }

            i22 = s->size[0];
            if (!((ns >= 1) && (ns <= i22))) {
              rtDynamicBoundsError(ns, 1, i22, &eg_emlrtBCI);
            }

            if (s->data[q - 1] < s->data[ns - 1]) {
              i22 = s->size[0];
              if (!((q >= 1) && (q <= i22))) {
                rtDynamicBoundsError(q, 1, i22, &fg_emlrtBCI);
              }

              ztest = s->data[q - 1];
              i22 = s->size[0];
              if (!((ns >= 1) && (ns <= i22))) {
                rtDynamicBoundsError(ns, 1, i22, &gg_emlrtBCI);
              }

              i22 = s->size[0];
              if (!((q >= 1) && (q <= i22))) {
                rtDynamicBoundsError(q, 1, i22, &hg_emlrtBCI);
              }

              s->data[q - 1] = s->data[ns - 1];
              i22 = s->size[0];
              if (!((ns >= 1) && (ns <= i22))) {
                rtDynamicBoundsError(ns, 1, i22, &ig_emlrtBCI);
              }

              s->data[ns - 1] = ztest;
              if (q < p) {
                xswap(p, Vf, 1 + p * (q - 1), 1 + p * q);
              }

              if (q < n) {
                xswap(n, U, 1 + n * (q - 1), 1 + n * q);
              }

              q = ns;
              ns++;
            } else {
              exitg3 = true;
            }
          }

          nct = 0;
          m--;
          break;
        }
      }
    }
  }

  emxFree_real_T(&work);
  emxFree_real_T(&b_A);
  i22 = e->size[0];
  e->size[0] = minnp;
  emxEnsureCapacity((emxArray__common *)e, i22, (int)sizeof(double));
  if ((!(1 > minnp)) && (minnp > 2147483646)) {
    check_forloop_overflow_error();
  }

  for (k = 1; k <= minnp; k++) {
    i22 = s->size[0];
    if (!((k >= 1) && (k <= i22))) {
      rtDynamicBoundsError(k, 1, i22, &ck_emlrtBCI);
    }

    i22 = e->size[0];
    if (!((k >= 1) && (k <= i22))) {
      rtDynamicBoundsError(k, 1, i22, &dk_emlrtBCI);
    }

    e->data[k - 1] = s->data[k - 1];
  }

  emxFree_real_T(&s);
  ns = A->size[1];
  i22 = V->size[0] * V->size[1];
  V->size[0] = ns;
  V->size[1] = minnp;
  emxEnsureCapacity((emxArray__common *)V, i22, (int)sizeof(double));
  for (ns = 1; ns <= minnp; ns++) {
    if ((!(1 > p)) && (p > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (qs = 1; qs <= p; qs++) {
      i22 = Vf->size[0];
      if (!((qs >= 1) && (qs <= i22))) {
        rtDynamicBoundsError(qs, 1, i22, &xj_emlrtBCI);
      }

      i22 = Vf->size[1];
      if (!(ns <= i22)) {
        rtDynamicBoundsError(ns, 1, i22, &yj_emlrtBCI);
      }

      i22 = V->size[0];
      if (!((qs >= 1) && (qs <= i22))) {
        rtDynamicBoundsError(qs, 1, i22, &ak_emlrtBCI);
      }

      i22 = V->size[1];
      if (!(ns <= i22)) {
        rtDynamicBoundsError(ns, 1, i22, &bk_emlrtBCI);
      }

      V->data[(qs + V->size[0] * (ns - 1)) - 1] = Vf->data[(qs + Vf->size[0] *
        (ns - 1)) - 1];
    }
  }

  emxFree_real_T(&Vf);
  i22 = S->size[0] * S->size[1];
  S->size[0] = e->size[0];
  S->size[1] = e->size[0];
  emxEnsureCapacity((emxArray__common *)S, i22, (int)sizeof(double));
  ns = e->size[0] * e->size[0];
  for (i22 = 0; i22 < ns; i22++) {
    S->data[i22] = 0.0;
  }

  for (k = 1; k - 1 < e->size[0]; k++) {
    i22 = e->size[0];
    if (!((k >= 1) && (k <= i22))) {
      rtDynamicBoundsError(k, 1, i22, &uj_emlrtBCI);
    }

    i22 = S->size[0];
    if (!((k >= 1) && (k <= i22))) {
      rtDynamicBoundsError(k, 1, i22, &vj_emlrtBCI);
    }

    i22 = S->size[1];
    if (!((k >= 1) && (k <= i22))) {
      rtDynamicBoundsError(k, 1, i22, &wj_emlrtBCI);
    }

    S->data[(k + S->size[0] * (k - 1)) - 1] = e->data[k - 1];
  }

  emxFree_real_T(&e);
}

//
// Arguments    : int n
//                double a
//                int ix0
//                emxArray_real_T *y
//                int iy0
// Return Type  : void
//
static void xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0)
{
  int ix;
  int iy;
  int k;
  int i42;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0;
    iy = iy0;
    for (k = 0; k < n; k++) {
      i42 = y->size[0] * y->size[1];
      if (!((iy >= 1) && (iy <= i42))) {
        rtDynamicBoundsError(iy, 1, i42, &vn_emlrtBCI);
      }

      i42 = y->size[0] * y->size[1];
      if (!((ix >= 1) && (ix <= i42))) {
        rtDynamicBoundsError(ix, 1, i42, &wn_emlrtBCI);
      }

      i42 = y->size[0] * y->size[1];
      if (!((iy >= 1) && (iy <= i42))) {
        rtDynamicBoundsError(iy, 1, i42, &xn_emlrtBCI);
      }

      y->data[iy - 1] += a * y->data[ix - 1];
      ix++;
      iy++;
    }
  }
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
//                const emxArray_real_T *y
//                int iy0
// Return Type  : double
//
static double xdotc(int n, const emxArray_real_T *x, int ix0, const
                    emxArray_real_T *y, int iy0)
{
  double d;
  int ix;
  int iy;
  int k;
  int i23;
  d = 0.0;
  if (n < 1) {
  } else {
    ix = ix0;
    iy = iy0;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }

    for (k = 1; k <= n; k++) {
      i23 = x->size[0] * x->size[1];
      if (!((ix >= 1) && (ix <= i23))) {
        rtDynamicBoundsError(ix, 1, i23, &ik_emlrtBCI);
      }

      i23 = y->size[0] * y->size[1];
      if (!((iy >= 1) && (iy <= i23))) {
        rtDynamicBoundsError(iy, 1, i23, &jk_emlrtBCI);
      }

      d += x->data[ix - 1] * y->data[iy - 1];
      ix++;
      iy++;
    }
  }

  return d;
}

//
// Arguments    : emxArray_real_T *A
//                emxArray_real_T *tau
//                emxArray_int32_T *jpvt
// Return Type  : void
//
static void xgeqp3(emxArray_real_T *A, emxArray_real_T *tau, emxArray_int32_T
                   *jpvt)
{
  int m;
  int n;
  int mn;
  int i39;
  emxArray_real_T *work;
  int pvt;
  emxArray_real_T *vn1;
  emxArray_real_T *vn2;
  int k;
  boolean_T overflow;
  int j;
  int i;
  int i_i;
  int nmi;
  int mmi;
  int b;
  double atmp;
  double d0;
  double xnorm;
  double beta1;
  int i_ip1;
  int lastv;
  int exitg3;
  int exitg2;
  int ix;
  int exitg1;
  int ia;
  m = A->size[0];
  n = A->size[1];
  if (A->size[0] <= A->size[1]) {
    mn = A->size[0];
  } else {
    mn = A->size[1];
  }

  i39 = tau->size[0];
  tau->size[0] = mn;
  emxEnsureCapacity((emxArray__common *)tau, i39, (int)sizeof(double));
  eml_signed_integer_colon(A->size[1], jpvt);
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
  } else {
    emxInit_real_T1(&work, 1);
    pvt = A->size[1];
    i39 = work->size[0];
    work->size[0] = pvt;
    emxEnsureCapacity((emxArray__common *)work, i39, (int)sizeof(double));
    for (i39 = 0; i39 < pvt; i39++) {
      work->data[i39] = 0.0;
    }

    emxInit_real_T1(&vn1, 1);
    emxInit_real_T1(&vn2, 1);
    b_sqrt(2.2204460492503131E-16);
    pvt = A->size[1];
    i39 = vn1->size[0];
    vn1->size[0] = pvt;
    emxEnsureCapacity((emxArray__common *)vn1, i39, (int)sizeof(double));
    i39 = vn2->size[0];
    vn2->size[0] = vn1->size[0];
    emxEnsureCapacity((emxArray__common *)vn2, i39, (int)sizeof(double));
    k = 1;
    overflow = ((!(1 > A->size[1])) && (A->size[1] > 2147483646));
    if (overflow) {
      check_forloop_overflow_error();
    }

    for (j = 1; j <= n; j++) {
      i39 = vn1->size[0];
      if (!((j >= 1) && (j <= i39))) {
        rtDynamicBoundsError(j, 1, i39, &sn_emlrtBCI);
      }

      vn1->data[j - 1] = xnrm2(m, A, k);
      i39 = vn1->size[0];
      if (!((j >= 1) && (j <= i39))) {
        rtDynamicBoundsError(j, 1, i39, &tn_emlrtBCI);
      }

      i39 = vn2->size[0];
      if (!((j >= 1) && (j <= i39))) {
        rtDynamicBoundsError(j, 1, i39, &un_emlrtBCI);
      }

      vn2->data[j - 1] = vn1->data[j - 1];
      k += m;
    }

    if ((!(1 > mn)) && (mn > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (i = 1; i <= mn; i++) {
      i_i = i + (i - 1) * m;
      nmi = n - i;
      mmi = m - i;
      b = ixamax(1 + nmi, vn1, i);
      pvt = (i + b) - 1;
      if (pvt != i) {
        xswap(m, A, 1 + m * (pvt - 1), 1 + m * (i - 1));
        i39 = jpvt->size[1];
        if (!((pvt >= 1) && (pvt <= i39))) {
          rtDynamicBoundsError(pvt, 1, i39, &nm_emlrtBCI);
        }

        k = jpvt->data[pvt - 1];
        i39 = jpvt->size[1];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &om_emlrtBCI);
        }

        i39 = jpvt->size[1];
        if (!((pvt >= 1) && (pvt <= i39))) {
          rtDynamicBoundsError(pvt, 1, i39, &pm_emlrtBCI);
        }

        jpvt->data[pvt - 1] = jpvt->data[i - 1];
        i39 = jpvt->size[1];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &qm_emlrtBCI);
        }

        jpvt->data[i - 1] = k;
        i39 = vn1->size[0];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &rm_emlrtBCI);
        }

        i39 = vn1->size[0];
        if (!((pvt >= 1) && (pvt <= i39))) {
          rtDynamicBoundsError(pvt, 1, i39, &sm_emlrtBCI);
        }

        vn1->data[pvt - 1] = vn1->data[i - 1];
        i39 = vn2->size[0];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &tm_emlrtBCI);
        }

        i39 = vn2->size[0];
        if (!((pvt >= 1) && (pvt <= i39))) {
          rtDynamicBoundsError(pvt, 1, i39, &um_emlrtBCI);
        }

        vn2->data[pvt - 1] = vn2->data[i - 1];
      }

      if (i < m) {
        i39 = A->size[0] * A->size[1];
        if (!((i_i >= 1) && (i_i <= i39))) {
          rtDynamicBoundsError(i_i, 1, i39, &mm_emlrtBCI);
        }

        atmp = A->data[i_i - 1];
        d0 = 0.0;
        if (1 + mmi <= 0) {
        } else {
          xnorm = b_xnrm2(mmi, A, i_i + 1);
          if (xnorm != 0.0) {
            beta1 = rt_hypotd_snf(A->data[i_i - 1], xnorm);
            if (A->data[i_i - 1] >= 0.0) {
              beta1 = -beta1;
            }

            if (fabs(beta1) < 1.0020841800044864E-292) {
              pvt = 0;
              do {
                pvt++;
                xscal(mmi, 9.9792015476736E+291, A, i_i + 1);
                beta1 *= 9.9792015476736E+291;
                atmp *= 9.9792015476736E+291;
              } while (!(fabs(beta1) >= 1.0020841800044864E-292));

              xnorm = b_xnrm2(mmi, A, i_i + 1);
              beta1 = rt_hypotd_snf(atmp, xnorm);
              if (atmp >= 0.0) {
                beta1 = -beta1;
              }

              d0 = (beta1 - atmp) / beta1;
              xscal(mmi, 1.0 / (atmp - beta1), A, i_i + 1);
              if ((!(1 > pvt)) && (pvt > 2147483646)) {
                check_forloop_overflow_error();
              }

              for (k = 1; k <= pvt; k++) {
                beta1 *= 1.0020841800044864E-292;
              }

              atmp = beta1;
            } else {
              d0 = (beta1 - A->data[i_i - 1]) / beta1;
              xscal(mmi, 1.0 / (A->data[i_i - 1] - beta1), A, i_i + 1);
              atmp = beta1;
            }
          }
        }

        i39 = tau->size[0];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &wm_emlrtBCI);
        }

        tau->data[i - 1] = d0;
        i39 = A->size[0] * A->size[1];
        if (!((i_i >= 1) && (i_i <= i39))) {
          rtDynamicBoundsError(i_i, 1, i39, &xm_emlrtBCI);
        }

        A->data[i_i - 1] = atmp;
      } else {
        i39 = tau->size[0];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &vm_emlrtBCI);
        }

        tau->data[i - 1] = 0.0;
      }

      if (i < n) {
        i39 = A->size[0] * A->size[1];
        if (!((i_i >= 1) && (i_i <= i39))) {
          rtDynamicBoundsError(i_i, 1, i39, &ym_emlrtBCI);
        }

        atmp = A->data[i_i - 1];
        i39 = A->size[0] * A->size[1];
        if (!((i_i >= 1) && (i_i <= i39))) {
          rtDynamicBoundsError(i_i, 1, i39, &an_emlrtBCI);
        }

        A->data[i_i - 1] = 1.0;
        i_ip1 = i + i * m;
        i39 = tau->size[0];
        if (!((i >= 1) && (i <= i39))) {
          rtDynamicBoundsError(i, 1, i39, &lm_emlrtBCI);
        }

        if (tau->data[i - 1] != 0.0) {
          lastv = mmi;
          pvt = i_i + mmi;
          do {
            exitg3 = 0;
            if (lastv + 1 > 0) {
              i39 = A->size[0] * A->size[1];
              if (!((pvt >= 1) && (pvt <= i39))) {
                rtDynamicBoundsError(pvt, 1, i39, &bn_emlrtBCI);
              }

              if (A->data[pvt - 1] == 0.0) {
                lastv--;
                pvt--;
              } else {
                exitg3 = 1;
              }
            } else {
              exitg3 = 2;
            }
          } while (exitg3 == 0);

          do {
            exitg2 = 0;
            if (nmi > 0) {
              pvt = i_ip1 + (nmi - 1) * m;
              k = pvt + lastv;
              if ((!(pvt > k)) && (k > 2147483646)) {
                check_forloop_overflow_error();
              }

              do {
                exitg1 = 0;
                if (pvt <= k) {
                  i39 = A->size[0] * A->size[1];
                  if (!((pvt >= 1) && (pvt <= i39))) {
                    rtDynamicBoundsError(pvt, 1, i39, &cn_emlrtBCI);
                  }

                  if (A->data[pvt - 1] != 0.0) {
                    exitg1 = 1;
                  } else {
                    pvt++;
                  }
                } else {
                  nmi--;
                  exitg1 = 2;
                }
              } while (exitg1 == 0);

              if (exitg1 == 1) {
                exitg2 = 1;
              }
            } else {
              exitg2 = 1;
            }
          } while (exitg2 == 0);
        } else {
          lastv = -1;
          nmi = 0;
        }

        if (lastv + 1 > 0) {
          if (nmi == 0) {
          } else {
            if ((!(1 > nmi)) && (nmi > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (pvt = 1; pvt <= nmi; pvt++) {
              i39 = work->size[0];
              if (!((pvt >= 1) && (pvt <= i39))) {
                rtDynamicBoundsError(pvt, 1, i39, &fn_emlrtBCI);
              }

              work->data[pvt - 1] = 0.0;
            }

            pvt = 1;
            b = i_ip1 + m * (nmi - 1);
            if ((m == 0) || (i_ip1 > b)) {
              overflow = false;
            } else {
              overflow = (b > MAX_int32_T - m);
            }

            if (overflow) {
              check_forloop_overflow_error();
            }

            k = i_ip1;
            while ((m > 0) && (k <= b)) {
              ix = i_i;
              xnorm = 0.0;
              j = k + lastv;
              if ((!(k > j)) && (j > 2147483646)) {
                check_forloop_overflow_error();
              }

              for (ia = k; ia <= j; ia++) {
                i39 = A->size[0] * A->size[1];
                if (!((ia >= 1) && (ia <= i39))) {
                  rtDynamicBoundsError(ia, 1, i39, &hm_emlrtBCI);
                }

                i39 = A->size[0] * A->size[1];
                if (!((ix >= 1) && (ix <= i39))) {
                  rtDynamicBoundsError(ix, 1, i39, &im_emlrtBCI);
                }

                xnorm += A->data[ia - 1] * A->data[ix - 1];
                ix++;
              }

              i39 = work->size[0];
              if (!((pvt >= 1) && (pvt <= i39))) {
                rtDynamicBoundsError(pvt, 1, i39, &dn_emlrtBCI);
              }

              i39 = work->size[0];
              if (!((pvt >= 1) && (pvt <= i39))) {
                rtDynamicBoundsError(pvt, 1, i39, &en_emlrtBCI);
              }

              work->data[pvt - 1] += xnorm;
              pvt++;
              k += m;
            }
          }

          if (-tau->data[i - 1] == 0.0) {
          } else {
            pvt = 0;
            for (j = 1; j <= nmi; j++) {
              i39 = work->size[0];
              if (!((pvt + 1 >= 1) && (pvt + 1 <= i39))) {
                rtDynamicBoundsError(pvt + 1, 1, i39, &gm_emlrtBCI);
              }

              if (work->data[pvt] != 0.0) {
                xnorm = work->data[pvt] * -tau->data[i - 1];
                ix = i_i;
                b = lastv + i_ip1;
                if ((!(i_ip1 > b)) && (b > 2147483646)) {
                  check_forloop_overflow_error();
                }

                for (k = i_ip1; k <= b; k++) {
                  i39 = A->size[0] * A->size[1];
                  if (!((k >= 1) && (k <= i39))) {
                    rtDynamicBoundsError(k, 1, i39, &pl_emlrtBCI);
                  }

                  i39 = A->size[0] * A->size[1];
                  if (!((ix >= 1) && (ix <= i39))) {
                    rtDynamicBoundsError(ix, 1, i39, &ql_emlrtBCI);
                  }

                  i39 = A->size[0] * A->size[1];
                  if (!((k >= 1) && (k <= i39))) {
                    rtDynamicBoundsError(k, 1, i39, &rl_emlrtBCI);
                  }

                  A->data[k - 1] += A->data[ix - 1] * xnorm;
                  ix++;
                }
              }

              pvt++;
              i_ip1 += m;
            }
          }
        }

        i39 = A->size[0] * A->size[1];
        if (!((i_i >= 1) && (i_i <= i39))) {
          rtDynamicBoundsError(i_i, 1, i39, &gn_emlrtBCI);
        }

        A->data[i_i - 1] = atmp;
      }

      for (j = i + 1; j <= n; j++) {
        i39 = vn1->size[0];
        if (!((j >= 1) && (j <= i39))) {
          rtDynamicBoundsError(j, 1, i39, &hn_emlrtBCI);
        }

        if (vn1->data[j - 1] != 0.0) {
          i39 = A->size[0];
          if (!((i >= 1) && (i <= i39))) {
            rtDynamicBoundsError(i, 1, i39, &km_emlrtBCI);
          }

          i39 = A->size[1];
          if (!((j >= 1) && (j <= i39))) {
            rtDynamicBoundsError(j, 1, i39, &jm_emlrtBCI);
          }

          i39 = vn1->size[0];
          if (!((j >= 1) && (j <= i39))) {
            rtDynamicBoundsError(j, 1, i39, &in_emlrtBCI);
          }

          xnorm = fabs(A->data[(i + A->size[0] * (j - 1)) - 1]) / vn1->data[j -
            1];
          xnorm = 1.0 - xnorm * xnorm;
          if (xnorm < 0.0) {
            xnorm = 0.0;
          }

          i39 = vn1->size[0];
          if (!((j >= 1) && (j <= i39))) {
            rtDynamicBoundsError(j, 1, i39, &jn_emlrtBCI);
          }

          i39 = vn2->size[0];
          if (!((j >= 1) && (j <= i39))) {
            rtDynamicBoundsError(j, 1, i39, &kn_emlrtBCI);
          }

          beta1 = vn1->data[j - 1] / vn2->data[j - 1];
          beta1 = xnorm * (beta1 * beta1);
          if (beta1 <= 1.4901161193847656E-8) {
            if (i < m) {
              i39 = vn1->size[0];
              if (!((j >= 1) && (j <= i39))) {
                rtDynamicBoundsError(j, 1, i39, &pn_emlrtBCI);
              }

              vn1->data[j - 1] = c_xnrm2(mmi, A, (i + m * (j - 1)) + 1);
              i39 = vn1->size[0];
              if (!((j >= 1) && (j <= i39))) {
                rtDynamicBoundsError(j, 1, i39, &qn_emlrtBCI);
              }

              i39 = vn2->size[0];
              if (!((j >= 1) && (j <= i39))) {
                rtDynamicBoundsError(j, 1, i39, &rn_emlrtBCI);
              }

              vn2->data[j - 1] = vn1->data[j - 1];
            } else {
              i39 = vn1->size[0];
              if (!((j >= 1) && (j <= i39))) {
                rtDynamicBoundsError(j, 1, i39, &nn_emlrtBCI);
              }

              vn1->data[j - 1] = 0.0;
              i39 = vn2->size[0];
              if (!((j >= 1) && (j <= i39))) {
                rtDynamicBoundsError(j, 1, i39, &on_emlrtBCI);
              }

              vn2->data[j - 1] = 0.0;
            }
          } else {
            i39 = vn1->size[0];
            if (!((j >= 1) && (j <= i39))) {
              rtDynamicBoundsError(j, 1, i39, &ln_emlrtBCI);
            }

            i39 = vn1->size[0];
            if (!((j >= 1) && (j <= i39))) {
              rtDynamicBoundsError(j, 1, i39, &mn_emlrtBCI);
            }

            vn1->data[j - 1] *= sqrt(xnorm);
          }
        }
      }
    }

    emxFree_real_T(&vn2);
    emxFree_real_T(&vn1);
    emxFree_real_T(&work);
  }
}

//
// Arguments    : int m
//                int n
//                emxArray_real_T *A
//                int lda
//                emxArray_int32_T *ipiv
// Return Type  : void
//
static void xgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T
                   *ipiv)
{
  int info;
  xzgetrf(m, n, A, lda, ipiv, &info);
}

//
// Arguments    : int n
//                const emxArray_real_T *x
//                int ix0
// Return Type  : double
//
static double xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int i14;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    i14 = x->size[0] * x->size[1];
    if (!((ix0 >= 1) && (ix0 <= i14))) {
      rtDynamicBoundsError(ix0, 1, i14, &id_emlrtBCI);
    }

    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    if ((!(ix0 > kend)) && (kend > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (k = ix0; k <= kend; k++) {
      i14 = x->size[0] * x->size[1];
      if (!((k >= 1) && (k <= i14))) {
        rtDynamicBoundsError(k, 1, i14, &jd_emlrtBCI);
      }

      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

//
// Arguments    : int n
//                emxArray_real_T *x
//                int ix0
//                int iy0
//                double c
//                double s
// Return Type  : void
//
static void xrot(int n, emxArray_real_T *x, int ix0, int iy0, double c, double s)
{
  int ix;
  int iy;
  int k;
  int i45;
  double temp;
  if (n < 1) {
  } else {
    ix = ix0;
    iy = iy0;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }

    for (k = 1; k <= n; k++) {
      i45 = x->size[0] * x->size[1];
      if (!((ix >= 1) && (ix <= i45))) {
        rtDynamicBoundsError(ix, 1, i45, &co_emlrtBCI);
      }

      i45 = x->size[0] * x->size[1];
      if (!((iy >= 1) && (iy <= i45))) {
        rtDynamicBoundsError(iy, 1, i45, &do_emlrtBCI);
      }

      temp = c * x->data[ix - 1] + s * x->data[iy - 1];
      i45 = x->size[0] * x->size[1];
      if (!((iy >= 1) && (iy <= i45))) {
        rtDynamicBoundsError(iy, 1, i45, &eo_emlrtBCI);
      }

      i45 = x->size[0] * x->size[1];
      if (!((ix >= 1) && (ix <= i45))) {
        rtDynamicBoundsError(ix, 1, i45, &fo_emlrtBCI);
      }

      i45 = x->size[0] * x->size[1];
      if (!((iy >= 1) && (iy <= i45))) {
        rtDynamicBoundsError(iy, 1, i45, &go_emlrtBCI);
      }

      x->data[iy - 1] = c * x->data[iy - 1] - s * x->data[ix - 1];
      i45 = x->size[0] * x->size[1];
      if (!((ix >= 1) && (ix <= i45))) {
        rtDynamicBoundsError(ix, 1, i45, &ho_emlrtBCI);
      }

      x->data[ix - 1] = temp;
      iy++;
      ix++;
    }
  }
}

//
// Arguments    : double *a
//                double *b
//                double *c
//                double *s
// Return Type  : void
//
static void xrotg(double *a, double *b, double *c, double *s)
{
  double roe;
  double absa;
  double absb;
  double scale;
  double ads;
  double bds;
  roe = *b;
  absa = fabs(*a);
  absb = fabs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    ads = 0.0;
    *b = 0.0;
  } else {
    ads = absa / scale;
    bds = absb / scale;
    ads = ads * ads + bds * bds;
    if (ads < 0.0) {
      error();
    }

    ads = scale * sqrt(ads);
    if (roe < 0.0) {
      ads = -ads;
    }

    *c = *a / ads;
    *s = *b / ads;
    if (absa > absb) {
      *b = *s;
    } else if (*c != 0.0) {
      *b = 1.0 / *c;
    } else {
      *b = 1.0;
    }
  }

  *a = ads;
}

//
// Arguments    : int n
//                double a
//                emxArray_real_T *x
//                int ix0
// Return Type  : void
//
static void xscal(int n, double a, emxArray_real_T *x, int ix0)
{
  int b;
  int k;
  int i41;
  b = (ix0 + n) - 1;
  if ((!(ix0 > b)) && (b > 2147483646)) {
    check_forloop_overflow_error();
  }

  for (k = ix0; k <= b; k++) {
    i41 = x->size[0] * x->size[1];
    if (!((k >= 1) && (k <= i41))) {
      rtDynamicBoundsError(k, 1, i41, &ij_emlrtBCI);
    }

    i41 = x->size[0] * x->size[1];
    if (!((k >= 1) && (k <= i41))) {
      rtDynamicBoundsError(k, 1, i41, &jj_emlrtBCI);
    }

    x->data[k - 1] *= a;
  }
}

//
// Arguments    : int n
//                emxArray_real_T *x
//                int ix0
//                int iy0
// Return Type  : void
//
static void xswap(int n, emxArray_real_T *x, int ix0, int iy0)
{
  int ix;
  int iy;
  int k;
  int i40;
  double temp;
  ix = ix0;
  iy = iy0;
  if ((!(1 > n)) && (n > 2147483646)) {
    check_forloop_overflow_error();
  }

  for (k = 1; k <= n; k++) {
    i40 = x->size[0] * x->size[1];
    if (!((ix >= 1) && (ix <= i40))) {
      rtDynamicBoundsError(ix, 1, i40, &tl_emlrtBCI);
    }

    temp = x->data[ix - 1];
    i40 = x->size[0] * x->size[1];
    if (!((iy >= 1) && (iy <= i40))) {
      rtDynamicBoundsError(iy, 1, i40, &ul_emlrtBCI);
    }

    i40 = x->size[0] * x->size[1];
    if (!((ix >= 1) && (ix <= i40))) {
      rtDynamicBoundsError(ix, 1, i40, &vl_emlrtBCI);
    }

    x->data[ix - 1] = x->data[iy - 1];
    i40 = x->size[0] * x->size[1];
    if (!((iy >= 1) && (iy <= i40))) {
      rtDynamicBoundsError(iy, 1, i40, &wl_emlrtBCI);
    }

    x->data[iy - 1] = temp;
    ix++;
    iy++;
  }
}

//
// Arguments    : int m
//                int n
//                const emxArray_real_T *A
//                int lda
//                emxArray_real_T *B
//                int ldb
// Return Type  : void
//
static void xtrsm(int m, int n, const emxArray_real_T *A, int lda,
                  emxArray_real_T *B, int ldb)
{
  int j;
  int jBcol;
  int k;
  int kAcol;
  int i34;
  int i35;
  double x;
  double y;
  int i;
  int i36;
  int i37;
  int i38;
  if ((n == 0) || ((B->size[0] == 0) || (B->size[1] == 0))) {
  } else {
    if ((!(1 > n)) && (n > 2147483646)) {
      check_forloop_overflow_error();
    }

    for (j = 1; j <= n; j++) {
      jBcol = ldb * (j - 1);
      for (k = m; k > 0; k--) {
        kAcol = lda * (k - 1);
        i34 = B->size[0] * B->size[1];
        i35 = k + jBcol;
        if (!((i35 >= 1) && (i35 <= i34))) {
          rtDynamicBoundsError(i35, 1, i34, &xl_emlrtBCI);
        }

        if (B->data[i35 - 1] != 0.0) {
          i34 = B->size[0] * B->size[1];
          i35 = k + jBcol;
          if (!((i35 >= 1) && (i35 <= i34))) {
            rtDynamicBoundsError(i35, 1, i34, &yl_emlrtBCI);
          }

          x = B->data[i35 - 1];
          i34 = A->size[0] * A->size[1];
          i35 = k + kAcol;
          if (!((i35 >= 1) && (i35 <= i34))) {
            rtDynamicBoundsError(i35, 1, i34, &am_emlrtBCI);
          }

          y = A->data[i35 - 1];
          i34 = B->size[0] * B->size[1];
          i35 = k + jBcol;
          if (!((i35 >= 1) && (i35 <= i34))) {
            rtDynamicBoundsError(i35, 1, i34, &bm_emlrtBCI);
          }

          B->data[i35 - 1] = x / y;
          for (i = 1; i < k; i++) {
            i34 = B->size[0] * B->size[1];
            i35 = i + jBcol;
            if (!((i35 >= 1) && (i35 <= i34))) {
              rtDynamicBoundsError(i35, 1, i34, &cm_emlrtBCI);
            }

            i34 = B->size[0] * B->size[1];
            i36 = k + jBcol;
            if (!((i36 >= 1) && (i36 <= i34))) {
              rtDynamicBoundsError(i36, 1, i34, &dm_emlrtBCI);
            }

            i34 = A->size[0] * A->size[1];
            i37 = i + kAcol;
            if (!((i37 >= 1) && (i37 <= i34))) {
              rtDynamicBoundsError(i37, 1, i34, &em_emlrtBCI);
            }

            i34 = B->size[0] * B->size[1];
            i38 = i + jBcol;
            if (!((i38 >= 1) && (i38 <= i34))) {
              rtDynamicBoundsError(i38, 1, i34, &fm_emlrtBCI);
            }

            B->data[i38 - 1] = B->data[i35 - 1] - B->data[i36 - 1] * A->data[i37
              - 1];
          }
        }
      }
    }
  }
}

//
// Arguments    : int m
//                int n
//                emxArray_real_T *A
//                int lda
//                emxArray_int32_T *ipiv
//                int *info
// Return Type  : void
//
static void xzgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T *
                    ipiv, int *info)
{
  int b_m;
  int b;
  int j;
  int mmj;
  int i33;
  int c;
  int jA;
  int ix;
  int iy;
  double smax;
  int jy;
  int b_b;
  int b_j;
  double s;
  int ijA;
  if (m <= n) {
    b_m = m;
  } else {
    b_m = n;
  }

  eml_signed_integer_colon(b_m, ipiv);
  *info = 0;
  if ((m < 1) || (n < 1)) {
  } else {
    if (m - 1 <= n) {
      b = m - 1;
    } else {
      b = n;
    }

    for (j = 1; j <= b; j++) {
      mmj = (m - j) + 1;
      c = (j - 1) * (lda + 1);
      if (mmj < 1) {
        jA = -1;
      } else {
        jA = 0;
        if (mmj > 1) {
          ix = c;
          i33 = A->size[0] * A->size[1];
          if (!((c + 1 >= 1) && (c + 1 <= i33))) {
            rtDynamicBoundsError(c + 1, 1, i33, &kd_emlrtBCI);
          }

          smax = fabs(A->data[c]);
          for (jy = 1; jy + 1 <= mmj; jy++) {
            ix++;
            i33 = A->size[0] * A->size[1];
            if (!((ix + 1 >= 1) && (ix + 1 <= i33))) {
              rtDynamicBoundsError(ix + 1, 1, i33, &ld_emlrtBCI);
            }

            s = fabs(A->data[ix]);
            if (s > smax) {
              jA = jy;
              smax = s;
            }
          }
        }
      }

      i33 = A->size[0] * A->size[1];
      iy = (c + jA) + 1;
      if (!((iy >= 1) && (iy <= i33))) {
        rtDynamicBoundsError(iy, 1, i33, &ml_emlrtBCI);
      }

      if (A->data[iy - 1] != 0.0) {
        if (jA != 0) {
          i33 = ipiv->size[1];
          if (!((j >= 1) && (j <= i33))) {
            rtDynamicBoundsError(j, 1, i33, &nl_emlrtBCI);
          }

          ipiv->data[j - 1] = j + jA;
          ix = j;
          iy = j + jA;
          if (n > 2147483646) {
            check_forloop_overflow_error();
          }

          for (jy = 1; jy <= n; jy++) {
            i33 = A->size[0] * A->size[1];
            if (!((ix >= 1) && (ix <= i33))) {
              rtDynamicBoundsError(ix, 1, i33, &tl_emlrtBCI);
            }

            smax = A->data[ix - 1];
            i33 = A->size[0] * A->size[1];
            if (!((iy >= 1) && (iy <= i33))) {
              rtDynamicBoundsError(iy, 1, i33, &ul_emlrtBCI);
            }

            i33 = A->size[0] * A->size[1];
            if (!((ix >= 1) && (ix <= i33))) {
              rtDynamicBoundsError(ix, 1, i33, &vl_emlrtBCI);
            }

            A->data[ix - 1] = A->data[iy - 1];
            i33 = A->size[0] * A->size[1];
            if (!((iy >= 1) && (iy <= i33))) {
              rtDynamicBoundsError(iy, 1, i33, &wl_emlrtBCI);
            }

            A->data[iy - 1] = smax;
            ix += lda;
            iy += lda;
          }
        }

        b_b = c + mmj;
        if ((!(c + 2 > b_b)) && (b_b > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (iy = c + 2; iy <= b_b; iy++) {
          i33 = A->size[0] * A->size[1];
          if (!((iy >= 1) && (iy <= i33))) {
            rtDynamicBoundsError(iy, 1, i33, &jl_emlrtBCI);
          }

          i33 = A->size[0] * A->size[1];
          if (!((c + 1 >= 1) && (c + 1 <= i33))) {
            rtDynamicBoundsError(c + 1, 1, i33, &il_emlrtBCI);
          }

          i33 = A->size[0] * A->size[1];
          if (!((iy >= 1) && (iy <= i33))) {
            rtDynamicBoundsError(iy, 1, i33, &sl_emlrtBCI);
          }

          A->data[iy - 1] /= A->data[c];
        }
      } else {
        *info = j;
      }

      iy = n - j;
      jA = c + lda;
      jy = (c + lda) + 1;
      for (b_j = 1; b_j <= iy; b_j++) {
        i33 = A->size[0] * A->size[1];
        if (!((jy >= 1) && (jy <= i33))) {
          rtDynamicBoundsError(jy, 1, i33, &ol_emlrtBCI);
        }

        smax = A->data[jy - 1];
        if (A->data[jy - 1] != 0.0) {
          ix = c + 2;
          b_b = mmj + jA;
          if ((!(jA + 2 > b_b)) && (b_b > 2147483646)) {
            check_forloop_overflow_error();
          }

          for (ijA = jA + 2; ijA <= b_b; ijA++) {
            i33 = A->size[0] * A->size[1];
            if (!((ijA >= 1) && (ijA <= i33))) {
              rtDynamicBoundsError(ijA, 1, i33, &pl_emlrtBCI);
            }

            i33 = A->size[0] * A->size[1];
            if (!((ix >= 1) && (ix <= i33))) {
              rtDynamicBoundsError(ix, 1, i33, &ql_emlrtBCI);
            }

            i33 = A->size[0] * A->size[1];
            if (!((ijA >= 1) && (ijA <= i33))) {
              rtDynamicBoundsError(ijA, 1, i33, &rl_emlrtBCI);
            }

            A->data[ijA - 1] += A->data[ix - 1] * -smax;
            ix++;
          }
        }

        jy += lda;
        jA += lda;
      }
    }

    if ((*info == 0) && (m <= n)) {
      i33 = A->size[0];
      if (!(m <= i33)) {
        rtDynamicBoundsError(m, 1, i33, &kl_emlrtBCI);
      }

      i33 = A->size[1];
      if (!(m <= i33)) {
        rtDynamicBoundsError(m, 1, i33, &ll_emlrtBCI);
      }

      if (!(A->data[(m + A->size[0] * (m - 1)) - 1] != 0.0)) {
        *info = m;
      }
    }
  }
}

//
// Arguments    : void
// Return Type  : void
//
void MyMathFunction_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void MyMathFunction_terminate()
{
  // (no terminate code required)
}

//
// Arguments    : int numDimensions
//                int *size
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : double *data
//                int numDimensions
//                int *size
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateWrapperND_real_T(double *data, int numDimensions, int *
  size)
{
  emxArray_real_T *emx;
  int numEl;
  int i;
  emxInit_real_T(&emx, numDimensions);
  numEl = 1;
  for (i = 0; i < numDimensions; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : double *data
//                int rows
//                int cols
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols)
{
  emxArray_real_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_real_T(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  emx->canFreeData = false;
  return emx;
}

//
// Arguments    : int rows
//                int cols
// Return Type  : emxArray_real_T *
//
emxArray_real_T *emxCreate_real_T(int rows, int cols)
{
  emxArray_real_T *emx;
  int size[2];
  int numEl;
  int i;
  size[0] = rows;
  size[1] = cols;
  emxInit_real_T(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (double *)calloc((unsigned int)numEl, sizeof(double));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

//
// Arguments    : emxArray_real_T *emxArray
// Return Type  : void
//
void emxDestroyArray_real_T(emxArray_real_T *emxArray)
{
  emxFree_real_T(&emxArray);
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxInit_real_T(pEmxArray, numDimensions);
}

//
// Arguments    : const emxArray_real_T *M
// Return Type  : double
//
double my_det(const emxArray_real_T *M)
{
  double y;
  boolean_T isodd;
  emxArray_real_T *x;
  int i0;
  int k;
  emxArray_int32_T *ipiv;
  int i1;
  int i2;

  // 'my_det:2' y = det( M );
  isodd = (M->size[0] == M->size[1]);
  if (isodd) {
  } else {
    rtErrorWithMessageID(&emlrtRTEI);
  }

  if ((M->size[0] == 0) || (M->size[1] == 0)) {
    y = 1.0;
  } else {
    emxInit_real_T(&x, 2);
    i0 = x->size[0] * x->size[1];
    x->size[0] = M->size[0];
    x->size[1] = M->size[1];
    emxEnsureCapacity((emxArray__common *)x, i0, (int)sizeof(double));
    k = M->size[0] * M->size[1];
    for (i0 = 0; i0 < k; i0++) {
      x->data[i0] = M->data[i0];
    }

    emxInit_int32_T(&ipiv, 2);
    xgetrf(M->size[0], M->size[1], x, M->size[0], ipiv);
    i0 = x->size[0];
    if (!(1 <= i0)) {
      rtDynamicBoundsError(1, 1, i0, &emlrtBCI);
    }

    i0 = x->size[1];
    if (!(1 <= i0)) {
      rtDynamicBoundsError(1, 1, i0, &b_emlrtBCI);
    }

    y = x->data[0];
    for (k = 0; k < (int)((double)x->size[0] + -1.0); k++) {
      i0 = x->size[0];
      i1 = (int)(2.0 + (double)k);
      if (!((i1 >= 1) && (i1 <= i0))) {
        rtDynamicBoundsError(i1, 1, i0, &d_emlrtBCI);
      }

      i0 = x->size[1];
      i2 = (int)(2.0 + (double)k);
      if (!((i2 >= 1) && (i2 <= i0))) {
        rtDynamicBoundsError(i2, 1, i0, &e_emlrtBCI);
      }

      y *= x->data[(i1 + x->size[0] * (i2 - 1)) - 1];
    }

    emxFree_real_T(&x);
    isodd = false;
    for (k = 0; k <= ipiv->size[1] - 2; k++) {
      i0 = ipiv->size[1];
      i1 = k + 1;
      if (!((i1 >= 1) && (i1 <= i0))) {
        rtDynamicBoundsError(i1, 1, i0, &c_emlrtBCI);
      }

      if (ipiv->data[k] > 1 + k) {
        isodd = !isodd;
      }
    }

    emxFree_int32_T(&ipiv);
    if (isodd) {
      y = -y;
    }
  }

  return y;
}

//
// Arguments    : const emxArray_real_T *M
//                emxArray_real_T *y
// Return Type  : void
//
void my_inv(const emxArray_real_T *M, emxArray_real_T *y)
{
  boolean_T overflow;
  int n;
  int i4;
  int pipk;
  emxArray_real_T *x;
  emxArray_int32_T *p;
  emxArray_int32_T *ipiv;
  int k;
  int i5;
  int c;
  int i;

  // 'my_inv:2' y = inv( M );
  overflow = (M->size[0] == M->size[1]);
  if (overflow) {
  } else {
    rtErrorWithMessageID(&c_emlrtRTEI);
  }

  if ((M->size[0] == 0) || (M->size[1] == 0)) {
    i4 = y->size[0] * y->size[1];
    y->size[0] = M->size[0];
    y->size[1] = M->size[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    pipk = M->size[0] * M->size[1];
    for (i4 = 0; i4 < pipk; i4++) {
      y->data[i4] = M->data[i4];
    }
  } else {
    n = M->size[0];
    i4 = y->size[0] * y->size[1];
    y->size[0] = M->size[0];
    y->size[1] = M->size[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    pipk = M->size[0] * M->size[1];
    for (i4 = 0; i4 < pipk; i4++) {
      y->data[i4] = 0.0;
    }

    emxInit_real_T(&x, 2);
    i4 = x->size[0] * x->size[1];
    x->size[0] = M->size[0];
    x->size[1] = M->size[1];
    emxEnsureCapacity((emxArray__common *)x, i4, (int)sizeof(double));
    pipk = M->size[0] * M->size[1];
    for (i4 = 0; i4 < pipk; i4++) {
      x->data[i4] = M->data[i4];
    }

    emxInit_int32_T(&p, 2);
    emxInit_int32_T(&ipiv, 2);
    xgetrf(M->size[0], M->size[0], x, M->size[0], ipiv);
    eml_signed_integer_colon(M->size[0], p);
    for (k = 0; k < ipiv->size[1]; k++) {
      i4 = ipiv->size[1];
      i5 = k + 1;
      if (!((i5 >= 1) && (i5 <= i4))) {
        rtDynamicBoundsError(i5, 1, i4, &h_emlrtBCI);
      }

      if (ipiv->data[k] > 1 + k) {
        i4 = p->size[1];
        i5 = ipiv->data[k];
        if (!((i5 >= 1) && (i5 <= i4))) {
          rtDynamicBoundsError(i5, 1, i4, &v_emlrtBCI);
        }

        pipk = p->data[i5 - 1];
        i4 = p->size[1];
        i5 = 1 + k;
        if (!((i5 >= 1) && (i5 <= i4))) {
          rtDynamicBoundsError(i5, 1, i4, &w_emlrtBCI);
        }

        i4 = p->size[1];
        i = ipiv->data[k];
        if (!((i >= 1) && (i <= i4))) {
          rtDynamicBoundsError(i, 1, i4, &x_emlrtBCI);
        }

        p->data[i - 1] = p->data[i5 - 1];
        i4 = p->size[1];
        i5 = 1 + k;
        if (!((i5 >= 1) && (i5 <= i4))) {
          rtDynamicBoundsError(i5, 1, i4, &y_emlrtBCI);
        }

        p->data[i5 - 1] = pipk;
      }
    }

    emxFree_int32_T(&ipiv);
    overflow = (M->size[0] > 2147483646);
    if (overflow) {
      check_forloop_overflow_error();
    }

    for (k = 1; k <= n; k++) {
      i4 = p->size[1];
      if (!((k >= 1) && (k <= i4))) {
        rtDynamicBoundsError(k, 1, i4, &i_emlrtBCI);
      }

      c = p->data[k - 1];
      i4 = y->size[0];
      if (!((k >= 1) && (k <= i4))) {
        rtDynamicBoundsError(k, 1, i4, &j_emlrtBCI);
      }

      i4 = y->size[1];
      i5 = p->data[k - 1];
      if (!((i5 >= 1) && (i5 <= i4))) {
        rtDynamicBoundsError(i5, 1, i4, &k_emlrtBCI);
      }

      y->data[(k + y->size[0] * (i5 - 1)) - 1] = 1.0;
      if ((!(k > n)) && (n > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (pipk = k; pipk <= n; pipk++) {
        i4 = y->size[0];
        if (!((pipk >= 1) && (pipk <= i4))) {
          rtDynamicBoundsError(pipk, 1, i4, &l_emlrtBCI);
        }

        i4 = y->size[1];
        if (!((c >= 1) && (c <= i4))) {
          rtDynamicBoundsError(c, 1, i4, &m_emlrtBCI);
        }

        if (y->data[(pipk + y->size[0] * (c - 1)) - 1] != 0.0) {
          if ((!(pipk + 1 > n)) && (n > 2147483646)) {
            check_forloop_overflow_error();
          }

          for (i = pipk + 1; i <= n; i++) {
            i4 = y->size[0];
            if (!((i >= 1) && (i <= i4))) {
              rtDynamicBoundsError(i, 1, i4, &n_emlrtBCI);
            }

            i4 = y->size[1];
            if (!((c >= 1) && (c <= i4))) {
              rtDynamicBoundsError(c, 1, i4, &o_emlrtBCI);
            }

            i4 = y->size[0];
            if (!((pipk >= 1) && (pipk <= i4))) {
              rtDynamicBoundsError(pipk, 1, i4, &p_emlrtBCI);
            }

            i4 = y->size[1];
            if (!((c >= 1) && (c <= i4))) {
              rtDynamicBoundsError(c, 1, i4, &q_emlrtBCI);
            }

            i4 = x->size[0];
            if (!((i >= 1) && (i <= i4))) {
              rtDynamicBoundsError(i, 1, i4, &r_emlrtBCI);
            }

            i4 = x->size[1];
            if (!((pipk >= 1) && (pipk <= i4))) {
              rtDynamicBoundsError(pipk, 1, i4, &s_emlrtBCI);
            }

            i4 = y->size[0];
            if (!((i >= 1) && (i <= i4))) {
              rtDynamicBoundsError(i, 1, i4, &t_emlrtBCI);
            }

            i4 = y->size[1];
            if (!((c >= 1) && (c <= i4))) {
              rtDynamicBoundsError(c, 1, i4, &u_emlrtBCI);
            }

            y->data[(i + y->size[0] * (c - 1)) - 1] -= y->data[(pipk + y->size[0]
              * (c - 1)) - 1] * x->data[(i + x->size[0] * (pipk - 1)) - 1];
          }
        }
      }
    }

    emxFree_int32_T(&p);
    xtrsm(M->size[0], M->size[0], x, M->size[0], y, M->size[0]);
    norm(M);
    norm(y);
    emxFree_real_T(&x);
  }
}

//
// Arguments    : const emxArray_real_T *M
//                const emxArray_real_T *divM
//                emxArray_real_T *y
// Return Type  : void
//
void my_left_div(const emxArray_real_T *M, const emxArray_real_T *divM,
                 emxArray_real_T *y)
{
  emxArray_real_T *A;
  emxArray_int32_T *ipiv;
  unsigned int unnamed_idx_0;
  int n;
  unsigned int unnamed_idx_1;
  int i8;
  int info;
  int nb;
  boolean_T overflow;
  int jBcol;
  int k;
  int kAcol;
  int i9;
  double temp;
  int i;
  int i10;
  int i11;
  int i12;

  // 'my_left_div:2' y = divM \ M;
  if (M->size[0] == divM->size[0]) {
  } else {
    c_rtErrorWithMessageID(&d_emlrtRTEI);
  }

  emxInit_real_T(&A, 2);
  emxInit_int32_T(&ipiv, 2);
  if ((divM->size[0] == 0) || (divM->size[1] == 0) || ((M->size[0] == 0) ||
       (M->size[1] == 0))) {
    unnamed_idx_0 = (unsigned int)divM->size[1];
    unnamed_idx_1 = (unsigned int)M->size[1];
    i8 = y->size[0] * y->size[1];
    y->size[0] = (int)unnamed_idx_0;
    y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)y, i8, (int)sizeof(double));
    info = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i8 = 0; i8 < info; i8++) {
      y->data[i8] = 0.0;
    }
  } else if (divM->size[0] == divM->size[1]) {
    n = divM->size[1];
    i8 = A->size[0] * A->size[1];
    A->size[0] = divM->size[0];
    A->size[1] = divM->size[1];
    emxEnsureCapacity((emxArray__common *)A, i8, (int)sizeof(double));
    info = divM->size[0] * divM->size[1];
    for (i8 = 0; i8 < info; i8++) {
      A->data[i8] = divM->data[i8];
    }

    b_xgetrf(divM->size[1], divM->size[1], A, divM->size[1], ipiv, &info);
    i8 = y->size[0] * y->size[1];
    y->size[0] = M->size[0];
    y->size[1] = M->size[1];
    emxEnsureCapacity((emxArray__common *)y, i8, (int)sizeof(double));
    info = M->size[0] * M->size[1];
    for (i8 = 0; i8 < info; i8++) {
      y->data[i8] = M->data[i8];
    }

    nb = M->size[1];
    for (info = 1; info < n; info++) {
      i8 = ipiv->size[1];
      if (!((info >= 1) && (info <= i8))) {
        rtDynamicBoundsError(info, 1, i8, &ib_emlrtBCI);
      }

      if (ipiv->data[info - 1] != info) {
        i8 = ipiv->size[1];
        if (!((info >= 1) && (info <= i8))) {
          rtDynamicBoundsError(info, 1, i8, &jb_emlrtBCI);
        }

        jBcol = ipiv->data[info - 1];
        if (nb > 2147483646) {
          check_forloop_overflow_error();
        }

        for (k = 1; k <= nb; k++) {
          i8 = y->size[0];
          if (!((info >= 1) && (info <= i8))) {
            rtDynamicBoundsError(info, 1, i8, &kb_emlrtBCI);
          }

          i8 = y->size[1];
          if (!((k >= 1) && (k <= i8))) {
            rtDynamicBoundsError(k, 1, i8, &lb_emlrtBCI);
          }

          temp = y->data[(info + y->size[0] * (k - 1)) - 1];
          i8 = y->size[0];
          if (!((jBcol >= 1) && (jBcol <= i8))) {
            rtDynamicBoundsError(jBcol, 1, i8, &mb_emlrtBCI);
          }

          i8 = y->size[1];
          if (!((k >= 1) && (k <= i8))) {
            rtDynamicBoundsError(k, 1, i8, &nb_emlrtBCI);
          }

          i8 = y->size[0];
          if (!((info >= 1) && (info <= i8))) {
            rtDynamicBoundsError(info, 1, i8, &ob_emlrtBCI);
          }

          i8 = y->size[1];
          if (!((k >= 1) && (k <= i8))) {
            rtDynamicBoundsError(k, 1, i8, &pb_emlrtBCI);
          }

          y->data[(info + y->size[0] * (k - 1)) - 1] = y->data[(jBcol + y->size
            [0] * (k - 1)) - 1];
          i8 = y->size[0];
          if (!((jBcol >= 1) && (jBcol <= i8))) {
            rtDynamicBoundsError(jBcol, 1, i8, &qb_emlrtBCI);
          }

          i8 = y->size[1];
          if (!((k >= 1) && (k <= i8))) {
            rtDynamicBoundsError(k, 1, i8, &rb_emlrtBCI);
          }

          y->data[(jBcol + y->size[0] * (k - 1)) - 1] = temp;
        }
      }
    }

    overflow = (M->size[1] > 2147483646);
    if (overflow) {
      check_forloop_overflow_error();
    }

    for (info = 1; info <= nb; info++) {
      jBcol = n * (info - 1);
      if (n > 2147483646) {
        check_forloop_overflow_error();
      }

      for (k = 1; k <= n; k++) {
        kAcol = n * (k - 1);
        i8 = y->size[0] * y->size[1];
        i9 = k + jBcol;
        if (!((i9 >= 1) && (i9 <= i8))) {
          rtDynamicBoundsError(i9, 1, i8, &db_emlrtBCI);
        }

        if (y->data[i9 - 1] != 0.0) {
          if ((!(k + 1 > n)) && (n > 2147483646)) {
            check_forloop_overflow_error();
          }

          for (i = k + 1; i <= n; i++) {
            i8 = y->size[0] * y->size[1];
            i9 = i + jBcol;
            if (!((i9 >= 1) && (i9 <= i8))) {
              rtDynamicBoundsError(i9, 1, i8, &eb_emlrtBCI);
            }

            i8 = y->size[0] * y->size[1];
            i10 = k + jBcol;
            if (!((i10 >= 1) && (i10 <= i8))) {
              rtDynamicBoundsError(i10, 1, i8, &fb_emlrtBCI);
            }

            i8 = A->size[0] * A->size[1];
            i11 = i + kAcol;
            if (!((i11 >= 1) && (i11 <= i8))) {
              rtDynamicBoundsError(i11, 1, i8, &gb_emlrtBCI);
            }

            i8 = y->size[0] * y->size[1];
            i12 = i + jBcol;
            if (!((i12 >= 1) && (i12 <= i8))) {
              rtDynamicBoundsError(i12, 1, i8, &hb_emlrtBCI);
            }

            y->data[i12 - 1] = y->data[i9 - 1] - y->data[i10 - 1] * A->data[i11
              - 1];
          }
        }
      }
    }

    xtrsm(divM->size[1], M->size[1], A, divM->size[1], y, divM->size[1]);
  } else {
    qrsolve(divM, M, y);
  }

  emxFree_int32_T(&ipiv);
  emxFree_real_T(&A);
}

//
// Arguments    : const emxArray_real_T *M1
//                const emxArray_real_T *M2
//                emxArray_real_T *y
// Return Type  : void
//
void my_mul(const emxArray_real_T *M1, const emxArray_real_T *M2,
            emxArray_real_T *y)
{
  int k;
  int i18;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  int br;
  int m;
  int ar;
  int b;
  int c;
  int cr;
  boolean_T overflow;
  int ic;
  int ib;
  int ia;
  int b_b;

  // 'my_mul:2' y = M1 * M2;
  if (!(M1->size[1] == M2->size[0])) {
    if (((M1->size[0] == 1) && (M1->size[1] == 1)) || ((M2->size[0] == 1) &&
         (M2->size[1] == 1))) {
      e_rtErrorWithMessageID(&f_emlrtRTEI);
    } else {
      f_rtErrorWithMessageID(&g_emlrtRTEI);
    }
  }

  if ((M1->size[1] == 1) || (M2->size[0] == 1)) {
    i18 = y->size[0] * y->size[1];
    y->size[0] = M1->size[0];
    y->size[1] = M2->size[1];
    emxEnsureCapacity((emxArray__common *)y, i18, (int)sizeof(double));
    br = M1->size[0];
    for (i18 = 0; i18 < br; i18++) {
      ar = M2->size[1];
      for (b = 0; b < ar; b++) {
        y->data[i18 + y->size[0] * b] = 0.0;
        c = M1->size[1];
        for (cr = 0; cr < c; cr++) {
          y->data[i18 + y->size[0] * b] += M1->data[i18 + M1->size[0] * cr] *
            M2->data[cr + M2->size[0] * b];
        }
      }
    }
  } else {
    k = M1->size[1];
    unnamed_idx_0 = (unsigned int)M1->size[0];
    unnamed_idx_1 = (unsigned int)M2->size[1];
    i18 = y->size[0] * y->size[1];
    y->size[0] = (int)unnamed_idx_0;
    y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)y, i18, (int)sizeof(double));
    m = M1->size[0];
    i18 = y->size[0] * y->size[1];
    emxEnsureCapacity((emxArray__common *)y, i18, (int)sizeof(double));
    br = y->size[1];
    for (i18 = 0; i18 < br; i18++) {
      ar = y->size[0];
      for (b = 0; b < ar; b++) {
        y->data[b + y->size[0] * i18] = 0.0;
      }
    }

    if ((M1->size[0] == 0) || (M2->size[1] == 0)) {
    } else {
      c = M1->size[0] * (M2->size[1] - 1);
      overflow = ((!(0 > c)) && (c > MAX_int32_T - M1->size[0]));
      if (overflow) {
        check_forloop_overflow_error();
      }

      cr = 0;
      while ((m > 0) && (cr <= c)) {
        b = cr + m;
        if ((!(cr + 1 > b)) && (b > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ic = cr + 1; ic <= b; ic++) {
          i18 = y->size[0] * y->size[1];
          if (!((ic >= 1) && (ic <= i18))) {
            rtDynamicBoundsError(ic, 1, i18, &rd_emlrtBCI);
          }

          y->data[ic - 1] = 0.0;
        }

        cr += m;
      }

      br = 1;
      overflow = ((!(0 > c)) && (c > MAX_int32_T - M1->size[0]));
      if (overflow) {
        check_forloop_overflow_error();
      }

      cr = 0;
      while ((m > 0) && (cr <= c)) {
        ar = 0;
        b = (br + k) - 1;
        if ((!(br > b)) && (b > 2147483646)) {
          check_forloop_overflow_error();
        }

        for (ib = br; ib <= b; ib++) {
          i18 = M2->size[0] * M2->size[1];
          if (!((ib >= 1) && (ib <= i18))) {
            rtDynamicBoundsError(ib, 1, i18, &nd_emlrtBCI);
          }

          if (M2->data[ib - 1] != 0.0) {
            i18 = M2->size[0] * M2->size[1];
            if (!((ib >= 1) && (ib <= i18))) {
              rtDynamicBoundsError(ib, 1, i18, &md_emlrtBCI);
            }

            ia = ar;
            b_b = cr + m;
            if ((!(cr + 1 > b_b)) && (b_b > 2147483646)) {
              check_forloop_overflow_error();
            }

            for (ic = cr + 1; ic <= b_b; ic++) {
              ia++;
              i18 = y->size[0] * y->size[1];
              if (!((ic >= 1) && (ic <= i18))) {
                rtDynamicBoundsError(ic, 1, i18, &od_emlrtBCI);
              }

              i18 = M1->size[0] * M1->size[1];
              if (!((ia >= 1) && (ia <= i18))) {
                rtDynamicBoundsError(ia, 1, i18, &pd_emlrtBCI);
              }

              i18 = y->size[0] * y->size[1];
              if (!((ic >= 1) && (ic <= i18))) {
                rtDynamicBoundsError(ic, 1, i18, &qd_emlrtBCI);
              }

              y->data[ic - 1] += M2->data[ib - 1] * M1->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }
}

//
// Arguments    : const emxArray_real_T *M
//                emxArray_real_T *y
// Return Type  : void
//
void my_pinv(const emxArray_real_T *M, emxArray_real_T *y)
{
  emxArray_real_T *b_M;
  int i19;
  int loop_ub;
  emxArray_real_T *r0;
  int b_loop_ub;
  int i20;

  // 'my_pinv:2' y = pinv(M);
  if (M->size[0] < M->size[1]) {
    emxInit_real_T(&b_M, 2);
    i19 = b_M->size[0] * b_M->size[1];
    b_M->size[0] = M->size[1];
    b_M->size[1] = M->size[0];
    emxEnsureCapacity((emxArray__common *)b_M, i19, (int)sizeof(double));
    loop_ub = M->size[0];
    for (i19 = 0; i19 < loop_ub; i19++) {
      b_loop_ub = M->size[1];
      for (i20 = 0; i20 < b_loop_ub; i20++) {
        b_M->data[i20 + b_M->size[0] * i19] = M->data[i19 + M->size[0] * i20];
      }
    }

    emxInit_real_T(&r0, 2);
    eml_pinv(b_M, r0);
    i19 = y->size[0] * y->size[1];
    y->size[0] = r0->size[1];
    y->size[1] = r0->size[0];
    emxEnsureCapacity((emxArray__common *)y, i19, (int)sizeof(double));
    loop_ub = r0->size[0];
    emxFree_real_T(&b_M);
    for (i19 = 0; i19 < loop_ub; i19++) {
      b_loop_ub = r0->size[1];
      for (i20 = 0; i20 < b_loop_ub; i20++) {
        y->data[i20 + y->size[0] * i19] = r0->data[i19 + r0->size[0] * i20];
      }
    }

    emxFree_real_T(&r0);
  } else {
    eml_pinv(M, y);
  }
}

//
// Arguments    : const emxArray_real_T *M
//                const emxArray_real_T *divM
//                emxArray_real_T *y
// Return Type  : void
//
void my_right_div(const emxArray_real_T *M, const emxArray_real_T *divM,
                  emxArray_real_T *y)
{
  emxArray_real_T *A;
  emxArray_int32_T *ipiv;
  emxArray_real_T *b_divM;
  emxArray_real_T *b_M;
  unsigned int unnamed_idx_0;
  int n;
  unsigned int unnamed_idx_1;
  int i25;
  int jp;
  int info;
  int i26;
  int nb;
  boolean_T overflow;
  int jAcol;
  int k;
  int kBcol;
  double temp;
  int i;
  int i27;
  int i28;
  int i29;

  // 'my_right_div:2' y = M / divM;
  if (divM->size[1] == M->size[1]) {
  } else {
    c_rtErrorWithMessageID(&h_emlrtRTEI);
  }

  emxInit_real_T(&A, 2);
  emxInit_int32_T(&ipiv, 2);
  emxInit_real_T(&b_divM, 2);
  emxInit_real_T(&b_M, 2);
  if ((M->size[0] == 0) || (M->size[1] == 0) || ((divM->size[0] == 0) ||
       (divM->size[1] == 0))) {
    unnamed_idx_0 = (unsigned int)M->size[0];
    unnamed_idx_1 = (unsigned int)divM->size[0];
    i25 = y->size[0] * y->size[1];
    y->size[0] = (int)unnamed_idx_0;
    y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)y, i25, (int)sizeof(double));
    jp = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i25 = 0; i25 < jp; i25++) {
      y->data[i25] = 0.0;
    }
  } else if (divM->size[0] == divM->size[1]) {
    n = divM->size[1];
    i25 = A->size[0] * A->size[1];
    A->size[0] = divM->size[0];
    A->size[1] = divM->size[1];
    emxEnsureCapacity((emxArray__common *)A, i25, (int)sizeof(double));
    jp = divM->size[0] * divM->size[1];
    for (i25 = 0; i25 < jp; i25++) {
      A->data[i25] = divM->data[i25];
    }

    b_xgetrf(divM->size[1], divM->size[1], A, divM->size[1], ipiv, &info);
    nb = M->size[0];
    i25 = y->size[0] * y->size[1];
    y->size[0] = M->size[0];
    y->size[1] = M->size[1];
    emxEnsureCapacity((emxArray__common *)y, i25, (int)sizeof(double));
    jp = M->size[0] * M->size[1];
    for (i25 = 0; i25 < jp; i25++) {
      y->data[i25] = M->data[i25];
    }

    overflow = (divM->size[1] > 2147483646);
    if (overflow) {
      check_forloop_overflow_error();
    }

    for (info = 0; info + 1 <= n; info++) {
      jp = nb * info;
      jAcol = n * info;
      if ((!(1 > info)) && (info > 2147483646)) {
        check_forloop_overflow_error();
      }

      for (k = 1; k <= info; k++) {
        kBcol = nb * (k - 1);
        i25 = A->size[0] * A->size[1];
        i26 = k + jAcol;
        if (!((i26 >= 1) && (i26 <= i25))) {
          rtDynamicBoundsError(i26, 1, i25, &dl_emlrtBCI);
        }

        if (A->data[i26 - 1] != 0.0) {
          if (nb > 2147483646) {
            check_forloop_overflow_error();
          }

          for (i = 1; i <= nb; i++) {
            i25 = y->size[0] * y->size[1];
            i26 = i + jp;
            if (!((i26 >= 1) && (i26 <= i25))) {
              rtDynamicBoundsError(i26, 1, i25, &el_emlrtBCI);
            }

            i25 = A->size[0] * A->size[1];
            i27 = k + jAcol;
            if (!((i27 >= 1) && (i27 <= i25))) {
              rtDynamicBoundsError(i27, 1, i25, &fl_emlrtBCI);
            }

            i25 = y->size[0] * y->size[1];
            i28 = i + kBcol;
            if (!((i28 >= 1) && (i28 <= i25))) {
              rtDynamicBoundsError(i28, 1, i25, &gl_emlrtBCI);
            }

            i25 = y->size[0] * y->size[1];
            i29 = i + jp;
            if (!((i29 >= 1) && (i29 <= i25))) {
              rtDynamicBoundsError(i29, 1, i25, &hl_emlrtBCI);
            }

            y->data[i29 - 1] = y->data[i26 - 1] - A->data[i27 - 1] * y->data[i28
              - 1];
          }
        }
      }

      i25 = A->size[0] * A->size[1];
      i26 = (info + jAcol) + 1;
      if (!((i26 >= 1) && (i26 <= i25))) {
        rtDynamicBoundsError(i26, 1, i25, &al_emlrtBCI);
      }

      temp = A->data[i26 - 1];
      temp = 1.0 / temp;
      if (nb > 2147483646) {
        check_forloop_overflow_error();
      }

      for (i = 1; i <= nb; i++) {
        i25 = y->size[0] * y->size[1];
        i26 = i + jp;
        if (!((i26 >= 1) && (i26 <= i25))) {
          rtDynamicBoundsError(i26, 1, i25, &bl_emlrtBCI);
        }

        i25 = y->size[0] * y->size[1];
        i27 = i + jp;
        if (!((i27 >= 1) && (i27 <= i25))) {
          rtDynamicBoundsError(i27, 1, i25, &cl_emlrtBCI);
        }

        y->data[i27 - 1] = temp * y->data[i26 - 1];
      }
    }

    for (info = divM->size[1]; info > 0; info--) {
      jp = nb * (info - 1);
      jAcol = n * (info - 1);
      for (k = info + 1; k <= n; k++) {
        kBcol = nb * (k - 1);
        i25 = A->size[0] * A->size[1];
        i26 = k + jAcol;
        if (!((i26 >= 1) && (i26 <= i25))) {
          rtDynamicBoundsError(i26, 1, i25, &uk_emlrtBCI);
        }

        if (A->data[i26 - 1] != 0.0) {
          if (nb > 2147483646) {
            check_forloop_overflow_error();
          }

          for (i = 1; i <= nb; i++) {
            i25 = y->size[0] * y->size[1];
            i26 = i + jp;
            if (!((i26 >= 1) && (i26 <= i25))) {
              rtDynamicBoundsError(i26, 1, i25, &vk_emlrtBCI);
            }

            i25 = A->size[0] * A->size[1];
            i27 = k + jAcol;
            if (!((i27 >= 1) && (i27 <= i25))) {
              rtDynamicBoundsError(i27, 1, i25, &wk_emlrtBCI);
            }

            i25 = y->size[0] * y->size[1];
            i28 = i + kBcol;
            if (!((i28 >= 1) && (i28 <= i25))) {
              rtDynamicBoundsError(i28, 1, i25, &xk_emlrtBCI);
            }

            i25 = y->size[0] * y->size[1];
            i29 = i + jp;
            if (!((i29 >= 1) && (i29 <= i25))) {
              rtDynamicBoundsError(i29, 1, i25, &yk_emlrtBCI);
            }

            y->data[i29 - 1] = y->data[i26 - 1] - A->data[i27 - 1] * y->data[i28
              - 1];
          }
        }
      }
    }

    for (info = divM->size[1] - 1; info > 0; info--) {
      i25 = ipiv->size[1];
      if (!(info <= i25)) {
        rtDynamicBoundsError(info, 1, i25, &kk_emlrtBCI);
      }

      if (ipiv->data[info - 1] != info) {
        i25 = ipiv->size[1];
        if (!(info <= i25)) {
          rtDynamicBoundsError(info, 1, i25, &lk_emlrtBCI);
        }

        jp = ipiv->data[info - 1];
        if (nb > 2147483646) {
          check_forloop_overflow_error();
        }

        for (jAcol = 1; jAcol <= nb; jAcol++) {
          i25 = y->size[0];
          if (!((jAcol >= 1) && (jAcol <= i25))) {
            rtDynamicBoundsError(jAcol, 1, i25, &mk_emlrtBCI);
          }

          i25 = y->size[1];
          if (!(info <= i25)) {
            rtDynamicBoundsError(info, 1, i25, &nk_emlrtBCI);
          }

          temp = y->data[(jAcol + y->size[0] * (info - 1)) - 1];
          i25 = y->size[0];
          if (!((jAcol >= 1) && (jAcol <= i25))) {
            rtDynamicBoundsError(jAcol, 1, i25, &ok_emlrtBCI);
          }

          i25 = y->size[1];
          if (!((jp >= 1) && (jp <= i25))) {
            rtDynamicBoundsError(jp, 1, i25, &pk_emlrtBCI);
          }

          i25 = y->size[0];
          if (!((jAcol >= 1) && (jAcol <= i25))) {
            rtDynamicBoundsError(jAcol, 1, i25, &qk_emlrtBCI);
          }

          i25 = y->size[1];
          if (!(info <= i25)) {
            rtDynamicBoundsError(info, 1, i25, &rk_emlrtBCI);
          }

          y->data[(jAcol + y->size[0] * (info - 1)) - 1] = y->data[(jAcol +
            y->size[0] * (jp - 1)) - 1];
          i25 = y->size[0];
          if (!((jAcol >= 1) && (jAcol <= i25))) {
            rtDynamicBoundsError(jAcol, 1, i25, &sk_emlrtBCI);
          }

          i25 = y->size[1];
          if (!((jp >= 1) && (jp <= i25))) {
            rtDynamicBoundsError(jp, 1, i25, &tk_emlrtBCI);
          }

          y->data[(jAcol + y->size[0] * (jp - 1)) - 1] = temp;
        }
      }
    }
  } else {
    i25 = b_divM->size[0] * b_divM->size[1];
    b_divM->size[0] = divM->size[1];
    b_divM->size[1] = divM->size[0];
    emxEnsureCapacity((emxArray__common *)b_divM, i25, (int)sizeof(double));
    jp = divM->size[0];
    for (i25 = 0; i25 < jp; i25++) {
      info = divM->size[1];
      for (i26 = 0; i26 < info; i26++) {
        b_divM->data[i26 + b_divM->size[0] * i25] = divM->data[i25 + divM->size
          [0] * i26];
      }
    }

    i25 = b_M->size[0] * b_M->size[1];
    b_M->size[0] = M->size[1];
    b_M->size[1] = M->size[0];
    emxEnsureCapacity((emxArray__common *)b_M, i25, (int)sizeof(double));
    jp = M->size[0];
    for (i25 = 0; i25 < jp; i25++) {
      info = M->size[1];
      for (i26 = 0; i26 < info; i26++) {
        b_M->data[i26 + b_M->size[0] * i25] = M->data[i25 + M->size[0] * i26];
      }
    }

    qrsolve(b_divM, b_M, A);
    i25 = y->size[0] * y->size[1];
    y->size[0] = A->size[1];
    y->size[1] = A->size[0];
    emxEnsureCapacity((emxArray__common *)y, i25, (int)sizeof(double));
    jp = A->size[0];
    for (i25 = 0; i25 < jp; i25++) {
      info = A->size[1];
      for (i26 = 0; i26 < info; i26++) {
        y->data[i26 + y->size[0] * i25] = A->data[i25 + A->size[0] * i26];
      }
    }
  }

  emxFree_real_T(&b_M);
  emxFree_real_T(&b_divM);
  emxFree_int32_T(&ipiv);
  emxFree_real_T(&A);
}

//
// Arguments    : const emxArray_real_T *M
//                emxArray_real_T *U
//                emxArray_real_T *S
//                emxArray_real_T *V
// Return Type  : void
//
void my_svd(const emxArray_real_T *M, emxArray_real_T *U, emxArray_real_T *S,
            emxArray_real_T *V)
{
  int i30;
  int k;
  int i31;

  // 'my_svd:2' [U,S,V] = svd(M);
  i30 = M->size[0] * M->size[1] - 1;
  for (k = 0; k <= i30; k++) {
    i31 = M->size[0] * M->size[1];
    if (!((k + 1 >= 1) && (k + 1 <= i31))) {
      rtDynamicBoundsError(k + 1, 1, i31, &td_emlrtBCI);
    }

    if (!((!rtIsInf(M->data[k])) && (!rtIsNaN(M->data[k])))) {
      b_error();
    }
  }

  b_svd(M, U, S, V);
}

//
// File trailer for MyMathFunction.cpp
//
// [EOF]
//
