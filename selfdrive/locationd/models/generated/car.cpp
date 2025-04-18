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
 *                      Code generated with SymPy 1.13.3                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7902506106427431264) {
   out_7902506106427431264[0] = delta_x[0] + nom_x[0];
   out_7902506106427431264[1] = delta_x[1] + nom_x[1];
   out_7902506106427431264[2] = delta_x[2] + nom_x[2];
   out_7902506106427431264[3] = delta_x[3] + nom_x[3];
   out_7902506106427431264[4] = delta_x[4] + nom_x[4];
   out_7902506106427431264[5] = delta_x[5] + nom_x[5];
   out_7902506106427431264[6] = delta_x[6] + nom_x[6];
   out_7902506106427431264[7] = delta_x[7] + nom_x[7];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5110972235504517756) {
   out_5110972235504517756[0] = -nom_x[0] + true_x[0];
   out_5110972235504517756[1] = -nom_x[1] + true_x[1];
   out_5110972235504517756[2] = -nom_x[2] + true_x[2];
   out_5110972235504517756[3] = -nom_x[3] + true_x[3];
   out_5110972235504517756[4] = -nom_x[4] + true_x[4];
   out_5110972235504517756[5] = -nom_x[5] + true_x[5];
   out_5110972235504517756[6] = -nom_x[6] + true_x[6];
   out_5110972235504517756[7] = -nom_x[7] + true_x[7];
}
void H_mod_fun(double *state, double *out_7718531475209591520) {
   out_7718531475209591520[0] = 1.0;
   out_7718531475209591520[1] = 0.0;
   out_7718531475209591520[2] = 0.0;
   out_7718531475209591520[3] = 0.0;
   out_7718531475209591520[4] = 0.0;
   out_7718531475209591520[5] = 0.0;
   out_7718531475209591520[6] = 0.0;
   out_7718531475209591520[7] = 0.0;
   out_7718531475209591520[8] = 0.0;
   out_7718531475209591520[9] = 1.0;
   out_7718531475209591520[10] = 0.0;
   out_7718531475209591520[11] = 0.0;
   out_7718531475209591520[12] = 0.0;
   out_7718531475209591520[13] = 0.0;
   out_7718531475209591520[14] = 0.0;
   out_7718531475209591520[15] = 0.0;
   out_7718531475209591520[16] = 0.0;
   out_7718531475209591520[17] = 0.0;
   out_7718531475209591520[18] = 1.0;
   out_7718531475209591520[19] = 0.0;
   out_7718531475209591520[20] = 0.0;
   out_7718531475209591520[21] = 0.0;
   out_7718531475209591520[22] = 0.0;
   out_7718531475209591520[23] = 0.0;
   out_7718531475209591520[24] = 0.0;
   out_7718531475209591520[25] = 0.0;
   out_7718531475209591520[26] = 0.0;
   out_7718531475209591520[27] = 1.0;
   out_7718531475209591520[28] = 0.0;
   out_7718531475209591520[29] = 0.0;
   out_7718531475209591520[30] = 0.0;
   out_7718531475209591520[31] = 0.0;
   out_7718531475209591520[32] = 0.0;
   out_7718531475209591520[33] = 0.0;
   out_7718531475209591520[34] = 0.0;
   out_7718531475209591520[35] = 0.0;
   out_7718531475209591520[36] = 1.0;
   out_7718531475209591520[37] = 0.0;
   out_7718531475209591520[38] = 0.0;
   out_7718531475209591520[39] = 0.0;
   out_7718531475209591520[40] = 0.0;
   out_7718531475209591520[41] = 0.0;
   out_7718531475209591520[42] = 0.0;
   out_7718531475209591520[43] = 0.0;
   out_7718531475209591520[44] = 0.0;
   out_7718531475209591520[45] = 1.0;
   out_7718531475209591520[46] = 0.0;
   out_7718531475209591520[47] = 0.0;
   out_7718531475209591520[48] = 0.0;
   out_7718531475209591520[49] = 0.0;
   out_7718531475209591520[50] = 0.0;
   out_7718531475209591520[51] = 0.0;
   out_7718531475209591520[52] = 0.0;
   out_7718531475209591520[53] = 0.0;
   out_7718531475209591520[54] = 1.0;
   out_7718531475209591520[55] = 0.0;
   out_7718531475209591520[56] = 0.0;
   out_7718531475209591520[57] = 0.0;
   out_7718531475209591520[58] = 0.0;
   out_7718531475209591520[59] = 0.0;
   out_7718531475209591520[60] = 0.0;
   out_7718531475209591520[61] = 0.0;
   out_7718531475209591520[62] = 0.0;
   out_7718531475209591520[63] = 1.0;
}
void f_fun(double *state, double dt, double *out_5834922400930391556) {
   out_5834922400930391556[0] = state[0];
   out_5834922400930391556[1] = state[1];
   out_5834922400930391556[2] = state[2];
   out_5834922400930391556[3] = state[3];
   out_5834922400930391556[4] = state[4];
   out_5834922400930391556[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5834922400930391556[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5834922400930391556[7] = state[7];
}
void F_fun(double *state, double dt, double *out_7718750476685544942) {
   out_7718750476685544942[0] = 1;
   out_7718750476685544942[1] = 0;
   out_7718750476685544942[2] = 0;
   out_7718750476685544942[3] = 0;
   out_7718750476685544942[4] = 0;
   out_7718750476685544942[5] = 0;
   out_7718750476685544942[6] = 0;
   out_7718750476685544942[7] = 0;
   out_7718750476685544942[8] = 0;
   out_7718750476685544942[9] = 1;
   out_7718750476685544942[10] = 0;
   out_7718750476685544942[11] = 0;
   out_7718750476685544942[12] = 0;
   out_7718750476685544942[13] = 0;
   out_7718750476685544942[14] = 0;
   out_7718750476685544942[15] = 0;
   out_7718750476685544942[16] = 0;
   out_7718750476685544942[17] = 0;
   out_7718750476685544942[18] = 1;
   out_7718750476685544942[19] = 0;
   out_7718750476685544942[20] = 0;
   out_7718750476685544942[21] = 0;
   out_7718750476685544942[22] = 0;
   out_7718750476685544942[23] = 0;
   out_7718750476685544942[24] = 0;
   out_7718750476685544942[25] = 0;
   out_7718750476685544942[26] = 0;
   out_7718750476685544942[27] = 1;
   out_7718750476685544942[28] = 0;
   out_7718750476685544942[29] = 0;
   out_7718750476685544942[30] = 0;
   out_7718750476685544942[31] = 0;
   out_7718750476685544942[32] = 0;
   out_7718750476685544942[33] = 0;
   out_7718750476685544942[34] = 0;
   out_7718750476685544942[35] = 0;
   out_7718750476685544942[36] = 1;
   out_7718750476685544942[37] = 0;
   out_7718750476685544942[38] = 0;
   out_7718750476685544942[39] = 0;
   out_7718750476685544942[40] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7718750476685544942[41] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7718750476685544942[42] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7718750476685544942[43] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7718750476685544942[44] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7718750476685544942[45] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7718750476685544942[46] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7718750476685544942[47] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7718750476685544942[48] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7718750476685544942[49] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7718750476685544942[50] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7718750476685544942[51] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7718750476685544942[52] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7718750476685544942[53] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7718750476685544942[54] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7718750476685544942[55] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7718750476685544942[56] = 0;
   out_7718750476685544942[57] = 0;
   out_7718750476685544942[58] = 0;
   out_7718750476685544942[59] = 0;
   out_7718750476685544942[60] = 0;
   out_7718750476685544942[61] = 0;
   out_7718750476685544942[62] = 0;
   out_7718750476685544942[63] = 1;
}
void h_25(double *state, double *unused, double *out_5257902179626811817) {
   out_5257902179626811817[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2248600704475429518) {
   out_2248600704475429518[0] = 0;
   out_2248600704475429518[1] = 0;
   out_2248600704475429518[2] = 0;
   out_2248600704475429518[3] = 0;
   out_2248600704475429518[4] = 0;
   out_2248600704475429518[5] = 0;
   out_2248600704475429518[6] = 1;
   out_2248600704475429518[7] = 0;
}
void h_24(double *state, double *unused, double *out_3967815841241831722) {
   out_3967815841241831722[0] = state[4];
   out_3967815841241831722[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3510989768671169650) {
   out_3510989768671169650[0] = 0;
   out_3510989768671169650[1] = 0;
   out_3510989768671169650[2] = 0;
   out_3510989768671169650[3] = 0;
   out_3510989768671169650[4] = 1;
   out_3510989768671169650[5] = 0;
   out_3510989768671169650[6] = 0;
   out_3510989768671169650[7] = 0;
   out_3510989768671169650[8] = 0;
   out_3510989768671169650[9] = 0;
   out_3510989768671169650[10] = 0;
   out_3510989768671169650[11] = 0;
   out_3510989768671169650[12] = 0;
   out_3510989768671169650[13] = 1;
   out_3510989768671169650[14] = 0;
   out_3510989768671169650[15] = 0;
}
void h_30(double *state, double *unused, double *out_5415088847977940962) {
   out_5415088847977940962[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7365298206473753762) {
   out_7365298206473753762[0] = 0;
   out_7365298206473753762[1] = 0;
   out_7365298206473753762[2] = 0;
   out_7365298206473753762[3] = 0;
   out_7365298206473753762[4] = 1;
   out_7365298206473753762[5] = 0;
   out_7365298206473753762[6] = 0;
   out_7365298206473753762[7] = 0;
}
void h_26(double *state, double *unused, double *out_676942798471127032) {
   out_676942798471127032[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3554836476430270663) {
   out_3554836476430270663[0] = 0;
   out_3554836476430270663[1] = 0;
   out_3554836476430270663[2] = 0;
   out_3554836476430270663[3] = 0;
   out_3554836476430270663[4] = 0;
   out_3554836476430270663[5] = 0;
   out_3554836476430270663[6] = 0;
   out_3554836476430270663[7] = 1;
}
void h_27(double *state, double *unused, double *out_8294688231071144502) {
   out_8294688231071144502[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8652880194310379074) {
   out_8652880194310379074[0] = 0;
   out_8652880194310379074[1] = 0;
   out_8652880194310379074[2] = 0;
   out_8652880194310379074[3] = 1;
   out_8652880194310379074[4] = 0;
   out_8652880194310379074[5] = 0;
   out_8652880194310379074[6] = 0;
   out_8652880194310379074[7] = 0;
}
void h_29(double *state, double *unused, double *out_3381220616288056178) {
   out_3381220616288056178[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4143067686967728890) {
   out_4143067686967728890[0] = 0;
   out_4143067686967728890[1] = 1;
   out_4143067686967728890[2] = 0;
   out_4143067686967728890[3] = 0;
   out_4143067686967728890[4] = 0;
   out_4143067686967728890[5] = 0;
   out_4143067686967728890[6] = 0;
   out_4143067686967728890[7] = 0;
}
void h_28(double *state, double *unused, double *out_2191592646470270554) {
   out_2191592646470270554[0] = state[5];
   out_2191592646470270554[1] = state[6];
}
void H_28(double *state, double *unused, double *out_871892538167030275) {
   out_871892538167030275[0] = 0;
   out_871892538167030275[1] = 0;
   out_871892538167030275[2] = 0;
   out_871892538167030275[3] = 0;
   out_871892538167030275[4] = 0;
   out_871892538167030275[5] = 1;
   out_871892538167030275[6] = 0;
   out_871892538167030275[7] = 0;
   out_871892538167030275[8] = 0;
   out_871892538167030275[9] = 0;
   out_871892538167030275[10] = 0;
   out_871892538167030275[11] = 0;
   out_871892538167030275[12] = 0;
   out_871892538167030275[13] = 0;
   out_871892538167030275[14] = 1;
   out_871892538167030275[15] = 0;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7902506106427431264) {
  err_fun(nom_x, delta_x, out_7902506106427431264);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5110972235504517756) {
  inv_err_fun(nom_x, true_x, out_5110972235504517756);
}
void car_H_mod_fun(double *state, double *out_7718531475209591520) {
  H_mod_fun(state, out_7718531475209591520);
}
void car_f_fun(double *state, double dt, double *out_5834922400930391556) {
  f_fun(state,  dt, out_5834922400930391556);
}
void car_F_fun(double *state, double dt, double *out_7718750476685544942) {
  F_fun(state,  dt, out_7718750476685544942);
}
void car_h_25(double *state, double *unused, double *out_5257902179626811817) {
  h_25(state, unused, out_5257902179626811817);
}
void car_H_25(double *state, double *unused, double *out_2248600704475429518) {
  H_25(state, unused, out_2248600704475429518);
}
void car_h_24(double *state, double *unused, double *out_3967815841241831722) {
  h_24(state, unused, out_3967815841241831722);
}
void car_H_24(double *state, double *unused, double *out_3510989768671169650) {
  H_24(state, unused, out_3510989768671169650);
}
void car_h_30(double *state, double *unused, double *out_5415088847977940962) {
  h_30(state, unused, out_5415088847977940962);
}
void car_H_30(double *state, double *unused, double *out_7365298206473753762) {
  H_30(state, unused, out_7365298206473753762);
}
void car_h_26(double *state, double *unused, double *out_676942798471127032) {
  h_26(state, unused, out_676942798471127032);
}
void car_H_26(double *state, double *unused, double *out_3554836476430270663) {
  H_26(state, unused, out_3554836476430270663);
}
void car_h_27(double *state, double *unused, double *out_8294688231071144502) {
  h_27(state, unused, out_8294688231071144502);
}
void car_H_27(double *state, double *unused, double *out_8652880194310379074) {
  H_27(state, unused, out_8652880194310379074);
}
void car_h_29(double *state, double *unused, double *out_3381220616288056178) {
  h_29(state, unused, out_3381220616288056178);
}
void car_H_29(double *state, double *unused, double *out_4143067686967728890) {
  H_29(state, unused, out_4143067686967728890);
}
void car_h_28(double *state, double *unused, double *out_2191592646470270554) {
  h_28(state, unused, out_2191592646470270554);
}
void car_H_28(double *state, double *unused, double *out_871892538167030275) {
  H_28(state, unused, out_871892538167030275);
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
