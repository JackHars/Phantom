#include "car.h"

namespace {
#define DIM 8
#define EDIM 8
#define MEDIM 8
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 5.991464547107981;

/******************************************************************************
 *                       Code generated with sympy 1.8                        *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2865416645807171237) {
   out_2865416645807171237[0] = delta_x[0] + nom_x[0];
   out_2865416645807171237[1] = delta_x[1] + nom_x[1];
   out_2865416645807171237[2] = delta_x[2] + nom_x[2];
   out_2865416645807171237[3] = delta_x[3] + nom_x[3];
   out_2865416645807171237[4] = delta_x[4] + nom_x[4];
   out_2865416645807171237[5] = delta_x[5] + nom_x[5];
   out_2865416645807171237[6] = delta_x[6] + nom_x[6];
   out_2865416645807171237[7] = delta_x[7] + nom_x[7];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_635870908016697860) {
   out_635870908016697860[0] = -nom_x[0] + true_x[0];
   out_635870908016697860[1] = -nom_x[1] + true_x[1];
   out_635870908016697860[2] = -nom_x[2] + true_x[2];
   out_635870908016697860[3] = -nom_x[3] + true_x[3];
   out_635870908016697860[4] = -nom_x[4] + true_x[4];
   out_635870908016697860[5] = -nom_x[5] + true_x[5];
   out_635870908016697860[6] = -nom_x[6] + true_x[6];
   out_635870908016697860[7] = -nom_x[7] + true_x[7];
}
void H_mod_fun(double *state, double *out_7722955180222182331) {
   out_7722955180222182331[0] = 1.0;
   out_7722955180222182331[1] = 0.0;
   out_7722955180222182331[2] = 0.0;
   out_7722955180222182331[3] = 0.0;
   out_7722955180222182331[4] = 0.0;
   out_7722955180222182331[5] = 0.0;
   out_7722955180222182331[6] = 0.0;
   out_7722955180222182331[7] = 0.0;
   out_7722955180222182331[8] = 0.0;
   out_7722955180222182331[9] = 1.0;
   out_7722955180222182331[10] = 0.0;
   out_7722955180222182331[11] = 0.0;
   out_7722955180222182331[12] = 0.0;
   out_7722955180222182331[13] = 0.0;
   out_7722955180222182331[14] = 0.0;
   out_7722955180222182331[15] = 0.0;
   out_7722955180222182331[16] = 0.0;
   out_7722955180222182331[17] = 0.0;
   out_7722955180222182331[18] = 1.0;
   out_7722955180222182331[19] = 0.0;
   out_7722955180222182331[20] = 0.0;
   out_7722955180222182331[21] = 0.0;
   out_7722955180222182331[22] = 0.0;
   out_7722955180222182331[23] = 0.0;
   out_7722955180222182331[24] = 0.0;
   out_7722955180222182331[25] = 0.0;
   out_7722955180222182331[26] = 0.0;
   out_7722955180222182331[27] = 1.0;
   out_7722955180222182331[28] = 0.0;
   out_7722955180222182331[29] = 0.0;
   out_7722955180222182331[30] = 0.0;
   out_7722955180222182331[31] = 0.0;
   out_7722955180222182331[32] = 0.0;
   out_7722955180222182331[33] = 0.0;
   out_7722955180222182331[34] = 0.0;
   out_7722955180222182331[35] = 0.0;
   out_7722955180222182331[36] = 1.0;
   out_7722955180222182331[37] = 0.0;
   out_7722955180222182331[38] = 0.0;
   out_7722955180222182331[39] = 0.0;
   out_7722955180222182331[40] = 0.0;
   out_7722955180222182331[41] = 0.0;
   out_7722955180222182331[42] = 0.0;
   out_7722955180222182331[43] = 0.0;
   out_7722955180222182331[44] = 0.0;
   out_7722955180222182331[45] = 1.0;
   out_7722955180222182331[46] = 0.0;
   out_7722955180222182331[47] = 0.0;
   out_7722955180222182331[48] = 0.0;
   out_7722955180222182331[49] = 0.0;
   out_7722955180222182331[50] = 0.0;
   out_7722955180222182331[51] = 0.0;
   out_7722955180222182331[52] = 0.0;
   out_7722955180222182331[53] = 0.0;
   out_7722955180222182331[54] = 1.0;
   out_7722955180222182331[55] = 0.0;
   out_7722955180222182331[56] = 0.0;
   out_7722955180222182331[57] = 0.0;
   out_7722955180222182331[58] = 0.0;
   out_7722955180222182331[59] = 0.0;
   out_7722955180222182331[60] = 0.0;
   out_7722955180222182331[61] = 0.0;
   out_7722955180222182331[62] = 0.0;
   out_7722955180222182331[63] = 1.0;
}
void f_fun(double *state, double dt, double *out_4596261882080719910) {
   out_4596261882080719910[0] = state[0];
   out_4596261882080719910[1] = state[1];
   out_4596261882080719910[2] = state[2];
   out_4596261882080719910[3] = state[3];
   out_4596261882080719910[4] = state[4];
   out_4596261882080719910[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_4596261882080719910[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_4596261882080719910[7] = state[7];
}
void F_fun(double *state, double dt, double *out_6851550815964785237) {
   out_6851550815964785237[0] = 1;
   out_6851550815964785237[1] = 0;
   out_6851550815964785237[2] = 0;
   out_6851550815964785237[3] = 0;
   out_6851550815964785237[4] = 0;
   out_6851550815964785237[5] = 0;
   out_6851550815964785237[6] = 0;
   out_6851550815964785237[7] = 0;
   out_6851550815964785237[8] = 0;
   out_6851550815964785237[9] = 1;
   out_6851550815964785237[10] = 0;
   out_6851550815964785237[11] = 0;
   out_6851550815964785237[12] = 0;
   out_6851550815964785237[13] = 0;
   out_6851550815964785237[14] = 0;
   out_6851550815964785237[15] = 0;
   out_6851550815964785237[16] = 0;
   out_6851550815964785237[17] = 0;
   out_6851550815964785237[18] = 1;
   out_6851550815964785237[19] = 0;
   out_6851550815964785237[20] = 0;
   out_6851550815964785237[21] = 0;
   out_6851550815964785237[22] = 0;
   out_6851550815964785237[23] = 0;
   out_6851550815964785237[24] = 0;
   out_6851550815964785237[25] = 0;
   out_6851550815964785237[26] = 0;
   out_6851550815964785237[27] = 1;
   out_6851550815964785237[28] = 0;
   out_6851550815964785237[29] = 0;
   out_6851550815964785237[30] = 0;
   out_6851550815964785237[31] = 0;
   out_6851550815964785237[32] = 0;
   out_6851550815964785237[33] = 0;
   out_6851550815964785237[34] = 0;
   out_6851550815964785237[35] = 0;
   out_6851550815964785237[36] = 1;
   out_6851550815964785237[37] = 0;
   out_6851550815964785237[38] = 0;
   out_6851550815964785237[39] = 0;
   out_6851550815964785237[40] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6851550815964785237[41] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6851550815964785237[42] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6851550815964785237[43] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6851550815964785237[44] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6851550815964785237[45] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6851550815964785237[46] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6851550815964785237[47] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6851550815964785237[48] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6851550815964785237[49] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6851550815964785237[50] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6851550815964785237[51] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6851550815964785237[52] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6851550815964785237[53] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6851550815964785237[54] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6851550815964785237[55] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6851550815964785237[56] = 0;
   out_6851550815964785237[57] = 0;
   out_6851550815964785237[58] = 0;
   out_6851550815964785237[59] = 0;
   out_6851550815964785237[60] = 0;
   out_6851550815964785237[61] = 0;
   out_6851550815964785237[62] = 0;
   out_6851550815964785237[63] = 1;
}
void h_25(double *state, double *unused, double *out_6075642558317368199) {
   out_6075642558317368199[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2389774409591852645) {
   out_2389774409591852645[0] = 0;
   out_2389774409591852645[1] = 0;
   out_2389774409591852645[2] = 0;
   out_2389774409591852645[3] = 0;
   out_2389774409591852645[4] = 0;
   out_2389774409591852645[5] = 0;
   out_2389774409591852645[6] = 1;
   out_2389774409591852645[7] = 0;
}
void h_24(double *state, double *unused, double *out_9164421185510050170) {
   out_9164421185510050170[0] = state[4];
   out_9164421185510050170[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2078411724020229925) {
   out_2078411724020229925[0] = 0;
   out_2078411724020229925[1] = 0;
   out_2078411724020229925[2] = 0;
   out_2078411724020229925[3] = 0;
   out_2078411724020229925[4] = 1;
   out_2078411724020229925[5] = 0;
   out_2078411724020229925[6] = 0;
   out_2078411724020229925[7] = 0;
   out_2078411724020229925[8] = 0;
   out_2078411724020229925[9] = 0;
   out_2078411724020229925[10] = 0;
   out_2078411724020229925[11] = 0;
   out_2078411724020229925[12] = 0;
   out_2078411724020229925[13] = 1;
   out_2078411724020229925[14] = 0;
   out_2078411724020229925[15] = 0;
}
void h_30(double *state, double *unused, double *out_5600266289041832481) {
   out_5600266289041832481[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7224124501357330635) {
   out_7224124501357330635[0] = 0;
   out_7224124501357330635[1] = 0;
   out_7224124501357330635[2] = 0;
   out_7224124501357330635[3] = 0;
   out_7224124501357330635[4] = 1;
   out_7224124501357330635[5] = 0;
   out_7224124501357330635[6] = 0;
   out_7224124501357330635[7] = 0;
}
void h_26(double *state, double *unused, double *out_3196043175224164659) {
   out_3196043175224164659[0] = state[7];
}
void H_26(double *state, double *unused, double *out_8094367564531061918) {
   out_8094367564531061918[0] = 0;
   out_8094367564531061918[1] = 0;
   out_8094367564531061918[2] = 0;
   out_8094367564531061918[3] = 0;
   out_8094367564531061918[4] = 0;
   out_8094367564531061918[5] = 0;
   out_8094367564531061918[6] = 0;
   out_8094367564531061918[7] = 1;
}
void h_27(double *state, double *unused, double *out_160011804583505863) {
   out_160011804583505863[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8511706489193955947) {
   out_8511706489193955947[0] = 0;
   out_8511706489193955947[1] = 0;
   out_8511706489193955947[2] = 0;
   out_8511706489193955947[3] = 1;
   out_8511706489193955947[4] = 0;
   out_8511706489193955947[5] = 0;
   out_8511706489193955947[6] = 0;
   out_8511706489193955947[7] = 0;
}
void h_29(double *state, double *unused, double *out_1610643197212389030) {
   out_1610643197212389030[0] = state[1];
}
void H_29(double *state, double *unused, double *out_396463401133062365) {
   out_396463401133062365[0] = 0;
   out_396463401133062365[1] = 1;
   out_396463401133062365[2] = 0;
   out_396463401133062365[3] = 0;
   out_396463401133062365[4] = 0;
   out_396463401133062365[5] = 0;
   out_396463401133062365[6] = 0;
   out_396463401133062365[7] = 0;
}
void h_28(double *state, double *unused, double *out_8616681029188511821) {
   out_8616681029188511821[0] = state[5];
   out_8616681029188511821[1] = state[6];
}
void H_28(double *state, double *unused, double *out_9108965936508918547) {
   out_9108965936508918547[0] = 0;
   out_9108965936508918547[1] = 0;
   out_9108965936508918547[2] = 0;
   out_9108965936508918547[3] = 0;
   out_9108965936508918547[4] = 0;
   out_9108965936508918547[5] = 1;
   out_9108965936508918547[6] = 0;
   out_9108965936508918547[7] = 0;
   out_9108965936508918547[8] = 0;
   out_9108965936508918547[9] = 0;
   out_9108965936508918547[10] = 0;
   out_9108965936508918547[11] = 0;
   out_9108965936508918547[12] = 0;
   out_9108965936508918547[13] = 0;
   out_9108965936508918547[14] = 1;
   out_9108965936508918547[15] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_2865416645807171237) {
  err_fun(nom_x, delta_x, out_2865416645807171237);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_635870908016697860) {
  inv_err_fun(nom_x, true_x, out_635870908016697860);
}
void car_H_mod_fun(double *state, double *out_7722955180222182331) {
  H_mod_fun(state, out_7722955180222182331);
}
void car_f_fun(double *state, double dt, double *out_4596261882080719910) {
  f_fun(state,  dt, out_4596261882080719910);
}
void car_F_fun(double *state, double dt, double *out_6851550815964785237) {
  F_fun(state,  dt, out_6851550815964785237);
}
void car_h_25(double *state, double *unused, double *out_6075642558317368199) {
  h_25(state, unused, out_6075642558317368199);
}
void car_H_25(double *state, double *unused, double *out_2389774409591852645) {
  H_25(state, unused, out_2389774409591852645);
}
void car_h_24(double *state, double *unused, double *out_9164421185510050170) {
  h_24(state, unused, out_9164421185510050170);
}
void car_H_24(double *state, double *unused, double *out_2078411724020229925) {
  H_24(state, unused, out_2078411724020229925);
}
void car_h_30(double *state, double *unused, double *out_5600266289041832481) {
  h_30(state, unused, out_5600266289041832481);
}
void car_H_30(double *state, double *unused, double *out_7224124501357330635) {
  H_30(state, unused, out_7224124501357330635);
}
void car_h_26(double *state, double *unused, double *out_3196043175224164659) {
  h_26(state, unused, out_3196043175224164659);
}
void car_H_26(double *state, double *unused, double *out_8094367564531061918) {
  H_26(state, unused, out_8094367564531061918);
}
void car_h_27(double *state, double *unused, double *out_160011804583505863) {
  h_27(state, unused, out_160011804583505863);
}
void car_H_27(double *state, double *unused, double *out_8511706489193955947) {
  H_27(state, unused, out_8511706489193955947);
}
void car_h_29(double *state, double *unused, double *out_1610643197212389030) {
  h_29(state, unused, out_1610643197212389030);
}
void car_H_29(double *state, double *unused, double *out_396463401133062365) {
  H_29(state, unused, out_396463401133062365);
}
void car_h_28(double *state, double *unused, double *out_8616681029188511821) {
  h_28(state, unused, out_8616681029188511821);
}
void car_H_28(double *state, double *unused, double *out_9108965936508918547) {
  H_28(state, unused, out_9108965936508918547);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_init(car);
