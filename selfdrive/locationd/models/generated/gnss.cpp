#include "gnss.h"

namespace {
#define DIM 11
#define EDIM 11
#define MEDIM 11
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_6 = 3.8414588206941227;
const static double MAHA_THRESH_20 = 3.8414588206941227;
const static double MAHA_THRESH_7 = 3.8414588206941227;
const static double MAHA_THRESH_21 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with sympy 1.8                        *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1171563384315072499) {
   out_1171563384315072499[0] = delta_x[0] + nom_x[0];
   out_1171563384315072499[1] = delta_x[1] + nom_x[1];
   out_1171563384315072499[2] = delta_x[2] + nom_x[2];
   out_1171563384315072499[3] = delta_x[3] + nom_x[3];
   out_1171563384315072499[4] = delta_x[4] + nom_x[4];
   out_1171563384315072499[5] = delta_x[5] + nom_x[5];
   out_1171563384315072499[6] = delta_x[6] + nom_x[6];
   out_1171563384315072499[7] = delta_x[7] + nom_x[7];
   out_1171563384315072499[8] = delta_x[8] + nom_x[8];
   out_1171563384315072499[9] = delta_x[9] + nom_x[9];
   out_1171563384315072499[10] = delta_x[10] + nom_x[10];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7175360854741032365) {
   out_7175360854741032365[0] = -nom_x[0] + true_x[0];
   out_7175360854741032365[1] = -nom_x[1] + true_x[1];
   out_7175360854741032365[2] = -nom_x[2] + true_x[2];
   out_7175360854741032365[3] = -nom_x[3] + true_x[3];
   out_7175360854741032365[4] = -nom_x[4] + true_x[4];
   out_7175360854741032365[5] = -nom_x[5] + true_x[5];
   out_7175360854741032365[6] = -nom_x[6] + true_x[6];
   out_7175360854741032365[7] = -nom_x[7] + true_x[7];
   out_7175360854741032365[8] = -nom_x[8] + true_x[8];
   out_7175360854741032365[9] = -nom_x[9] + true_x[9];
   out_7175360854741032365[10] = -nom_x[10] + true_x[10];
}
void H_mod_fun(double *state, double *out_6019779973188466781) {
   out_6019779973188466781[0] = 1.0;
   out_6019779973188466781[1] = 0.0;
   out_6019779973188466781[2] = 0.0;
   out_6019779973188466781[3] = 0.0;
   out_6019779973188466781[4] = 0.0;
   out_6019779973188466781[5] = 0.0;
   out_6019779973188466781[6] = 0.0;
   out_6019779973188466781[7] = 0.0;
   out_6019779973188466781[8] = 0.0;
   out_6019779973188466781[9] = 0.0;
   out_6019779973188466781[10] = 0.0;
   out_6019779973188466781[11] = 0.0;
   out_6019779973188466781[12] = 1.0;
   out_6019779973188466781[13] = 0.0;
   out_6019779973188466781[14] = 0.0;
   out_6019779973188466781[15] = 0.0;
   out_6019779973188466781[16] = 0.0;
   out_6019779973188466781[17] = 0.0;
   out_6019779973188466781[18] = 0.0;
   out_6019779973188466781[19] = 0.0;
   out_6019779973188466781[20] = 0.0;
   out_6019779973188466781[21] = 0.0;
   out_6019779973188466781[22] = 0.0;
   out_6019779973188466781[23] = 0.0;
   out_6019779973188466781[24] = 1.0;
   out_6019779973188466781[25] = 0.0;
   out_6019779973188466781[26] = 0.0;
   out_6019779973188466781[27] = 0.0;
   out_6019779973188466781[28] = 0.0;
   out_6019779973188466781[29] = 0.0;
   out_6019779973188466781[30] = 0.0;
   out_6019779973188466781[31] = 0.0;
   out_6019779973188466781[32] = 0.0;
   out_6019779973188466781[33] = 0.0;
   out_6019779973188466781[34] = 0.0;
   out_6019779973188466781[35] = 0.0;
   out_6019779973188466781[36] = 1.0;
   out_6019779973188466781[37] = 0.0;
   out_6019779973188466781[38] = 0.0;
   out_6019779973188466781[39] = 0.0;
   out_6019779973188466781[40] = 0.0;
   out_6019779973188466781[41] = 0.0;
   out_6019779973188466781[42] = 0.0;
   out_6019779973188466781[43] = 0.0;
   out_6019779973188466781[44] = 0.0;
   out_6019779973188466781[45] = 0.0;
   out_6019779973188466781[46] = 0.0;
   out_6019779973188466781[47] = 0.0;
   out_6019779973188466781[48] = 1.0;
   out_6019779973188466781[49] = 0.0;
   out_6019779973188466781[50] = 0.0;
   out_6019779973188466781[51] = 0.0;
   out_6019779973188466781[52] = 0.0;
   out_6019779973188466781[53] = 0.0;
   out_6019779973188466781[54] = 0.0;
   out_6019779973188466781[55] = 0.0;
   out_6019779973188466781[56] = 0.0;
   out_6019779973188466781[57] = 0.0;
   out_6019779973188466781[58] = 0.0;
   out_6019779973188466781[59] = 0.0;
   out_6019779973188466781[60] = 1.0;
   out_6019779973188466781[61] = 0.0;
   out_6019779973188466781[62] = 0.0;
   out_6019779973188466781[63] = 0.0;
   out_6019779973188466781[64] = 0.0;
   out_6019779973188466781[65] = 0.0;
   out_6019779973188466781[66] = 0.0;
   out_6019779973188466781[67] = 0.0;
   out_6019779973188466781[68] = 0.0;
   out_6019779973188466781[69] = 0.0;
   out_6019779973188466781[70] = 0.0;
   out_6019779973188466781[71] = 0.0;
   out_6019779973188466781[72] = 1.0;
   out_6019779973188466781[73] = 0.0;
   out_6019779973188466781[74] = 0.0;
   out_6019779973188466781[75] = 0.0;
   out_6019779973188466781[76] = 0.0;
   out_6019779973188466781[77] = 0.0;
   out_6019779973188466781[78] = 0.0;
   out_6019779973188466781[79] = 0.0;
   out_6019779973188466781[80] = 0.0;
   out_6019779973188466781[81] = 0.0;
   out_6019779973188466781[82] = 0.0;
   out_6019779973188466781[83] = 0.0;
   out_6019779973188466781[84] = 1.0;
   out_6019779973188466781[85] = 0.0;
   out_6019779973188466781[86] = 0.0;
   out_6019779973188466781[87] = 0.0;
   out_6019779973188466781[88] = 0.0;
   out_6019779973188466781[89] = 0.0;
   out_6019779973188466781[90] = 0.0;
   out_6019779973188466781[91] = 0.0;
   out_6019779973188466781[92] = 0.0;
   out_6019779973188466781[93] = 0.0;
   out_6019779973188466781[94] = 0.0;
   out_6019779973188466781[95] = 0.0;
   out_6019779973188466781[96] = 1.0;
   out_6019779973188466781[97] = 0.0;
   out_6019779973188466781[98] = 0.0;
   out_6019779973188466781[99] = 0.0;
   out_6019779973188466781[100] = 0.0;
   out_6019779973188466781[101] = 0.0;
   out_6019779973188466781[102] = 0.0;
   out_6019779973188466781[103] = 0.0;
   out_6019779973188466781[104] = 0.0;
   out_6019779973188466781[105] = 0.0;
   out_6019779973188466781[106] = 0.0;
   out_6019779973188466781[107] = 0.0;
   out_6019779973188466781[108] = 1.0;
   out_6019779973188466781[109] = 0.0;
   out_6019779973188466781[110] = 0.0;
   out_6019779973188466781[111] = 0.0;
   out_6019779973188466781[112] = 0.0;
   out_6019779973188466781[113] = 0.0;
   out_6019779973188466781[114] = 0.0;
   out_6019779973188466781[115] = 0.0;
   out_6019779973188466781[116] = 0.0;
   out_6019779973188466781[117] = 0.0;
   out_6019779973188466781[118] = 0.0;
   out_6019779973188466781[119] = 0.0;
   out_6019779973188466781[120] = 1.0;
}
void f_fun(double *state, double dt, double *out_1734095868312900332) {
   out_1734095868312900332[0] = dt*state[3] + state[0];
   out_1734095868312900332[1] = dt*state[4] + state[1];
   out_1734095868312900332[2] = dt*state[5] + state[2];
   out_1734095868312900332[3] = state[3];
   out_1734095868312900332[4] = state[4];
   out_1734095868312900332[5] = state[5];
   out_1734095868312900332[6] = dt*state[7] + state[6];
   out_1734095868312900332[7] = dt*state[8] + state[7];
   out_1734095868312900332[8] = state[8];
   out_1734095868312900332[9] = state[9];
   out_1734095868312900332[10] = state[10];
}
void F_fun(double *state, double dt, double *out_7391177847777738587) {
   out_7391177847777738587[0] = 1;
   out_7391177847777738587[1] = 0;
   out_7391177847777738587[2] = 0;
   out_7391177847777738587[3] = dt;
   out_7391177847777738587[4] = 0;
   out_7391177847777738587[5] = 0;
   out_7391177847777738587[6] = 0;
   out_7391177847777738587[7] = 0;
   out_7391177847777738587[8] = 0;
   out_7391177847777738587[9] = 0;
   out_7391177847777738587[10] = 0;
   out_7391177847777738587[11] = 0;
   out_7391177847777738587[12] = 1;
   out_7391177847777738587[13] = 0;
   out_7391177847777738587[14] = 0;
   out_7391177847777738587[15] = dt;
   out_7391177847777738587[16] = 0;
   out_7391177847777738587[17] = 0;
   out_7391177847777738587[18] = 0;
   out_7391177847777738587[19] = 0;
   out_7391177847777738587[20] = 0;
   out_7391177847777738587[21] = 0;
   out_7391177847777738587[22] = 0;
   out_7391177847777738587[23] = 0;
   out_7391177847777738587[24] = 1;
   out_7391177847777738587[25] = 0;
   out_7391177847777738587[26] = 0;
   out_7391177847777738587[27] = dt;
   out_7391177847777738587[28] = 0;
   out_7391177847777738587[29] = 0;
   out_7391177847777738587[30] = 0;
   out_7391177847777738587[31] = 0;
   out_7391177847777738587[32] = 0;
   out_7391177847777738587[33] = 0;
   out_7391177847777738587[34] = 0;
   out_7391177847777738587[35] = 0;
   out_7391177847777738587[36] = 1;
   out_7391177847777738587[37] = 0;
   out_7391177847777738587[38] = 0;
   out_7391177847777738587[39] = 0;
   out_7391177847777738587[40] = 0;
   out_7391177847777738587[41] = 0;
   out_7391177847777738587[42] = 0;
   out_7391177847777738587[43] = 0;
   out_7391177847777738587[44] = 0;
   out_7391177847777738587[45] = 0;
   out_7391177847777738587[46] = 0;
   out_7391177847777738587[47] = 0;
   out_7391177847777738587[48] = 1;
   out_7391177847777738587[49] = 0;
   out_7391177847777738587[50] = 0;
   out_7391177847777738587[51] = 0;
   out_7391177847777738587[52] = 0;
   out_7391177847777738587[53] = 0;
   out_7391177847777738587[54] = 0;
   out_7391177847777738587[55] = 0;
   out_7391177847777738587[56] = 0;
   out_7391177847777738587[57] = 0;
   out_7391177847777738587[58] = 0;
   out_7391177847777738587[59] = 0;
   out_7391177847777738587[60] = 1;
   out_7391177847777738587[61] = 0;
   out_7391177847777738587[62] = 0;
   out_7391177847777738587[63] = 0;
   out_7391177847777738587[64] = 0;
   out_7391177847777738587[65] = 0;
   out_7391177847777738587[66] = 0;
   out_7391177847777738587[67] = 0;
   out_7391177847777738587[68] = 0;
   out_7391177847777738587[69] = 0;
   out_7391177847777738587[70] = 0;
   out_7391177847777738587[71] = 0;
   out_7391177847777738587[72] = 1;
   out_7391177847777738587[73] = dt;
   out_7391177847777738587[74] = 0;
   out_7391177847777738587[75] = 0;
   out_7391177847777738587[76] = 0;
   out_7391177847777738587[77] = 0;
   out_7391177847777738587[78] = 0;
   out_7391177847777738587[79] = 0;
   out_7391177847777738587[80] = 0;
   out_7391177847777738587[81] = 0;
   out_7391177847777738587[82] = 0;
   out_7391177847777738587[83] = 0;
   out_7391177847777738587[84] = 1;
   out_7391177847777738587[85] = dt;
   out_7391177847777738587[86] = 0;
   out_7391177847777738587[87] = 0;
   out_7391177847777738587[88] = 0;
   out_7391177847777738587[89] = 0;
   out_7391177847777738587[90] = 0;
   out_7391177847777738587[91] = 0;
   out_7391177847777738587[92] = 0;
   out_7391177847777738587[93] = 0;
   out_7391177847777738587[94] = 0;
   out_7391177847777738587[95] = 0;
   out_7391177847777738587[96] = 1;
   out_7391177847777738587[97] = 0;
   out_7391177847777738587[98] = 0;
   out_7391177847777738587[99] = 0;
   out_7391177847777738587[100] = 0;
   out_7391177847777738587[101] = 0;
   out_7391177847777738587[102] = 0;
   out_7391177847777738587[103] = 0;
   out_7391177847777738587[104] = 0;
   out_7391177847777738587[105] = 0;
   out_7391177847777738587[106] = 0;
   out_7391177847777738587[107] = 0;
   out_7391177847777738587[108] = 1;
   out_7391177847777738587[109] = 0;
   out_7391177847777738587[110] = 0;
   out_7391177847777738587[111] = 0;
   out_7391177847777738587[112] = 0;
   out_7391177847777738587[113] = 0;
   out_7391177847777738587[114] = 0;
   out_7391177847777738587[115] = 0;
   out_7391177847777738587[116] = 0;
   out_7391177847777738587[117] = 0;
   out_7391177847777738587[118] = 0;
   out_7391177847777738587[119] = 0;
   out_7391177847777738587[120] = 1;
}
void h_6(double *state, double *sat_pos, double *out_4559242126941663844) {
   out_4559242126941663844[0] = sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2)) + state[6];
}
void H_6(double *state, double *sat_pos, double *out_6360356332846862462) {
   out_6360356332846862462[0] = (-sat_pos[0] + state[0])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_6360356332846862462[1] = (-sat_pos[1] + state[1])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_6360356332846862462[2] = (-sat_pos[2] + state[2])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_6360356332846862462[3] = 0;
   out_6360356332846862462[4] = 0;
   out_6360356332846862462[5] = 0;
   out_6360356332846862462[6] = 1;
   out_6360356332846862462[7] = 0;
   out_6360356332846862462[8] = 0;
   out_6360356332846862462[9] = 0;
   out_6360356332846862462[10] = 0;
}
void h_20(double *state, double *sat_pos, double *out_4943814902343407025) {
   out_4943814902343407025[0] = sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2)) + sat_pos[3]*state[10] + state[6] + state[9];
}
void H_20(double *state, double *sat_pos, double *out_6549302345954285139) {
   out_6549302345954285139[0] = (-sat_pos[0] + state[0])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_6549302345954285139[1] = (-sat_pos[1] + state[1])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_6549302345954285139[2] = (-sat_pos[2] + state[2])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_6549302345954285139[3] = 0;
   out_6549302345954285139[4] = 0;
   out_6549302345954285139[5] = 0;
   out_6549302345954285139[6] = 1;
   out_6549302345954285139[7] = 0;
   out_6549302345954285139[8] = 0;
   out_6549302345954285139[9] = 1;
   out_6549302345954285139[10] = sat_pos[3];
}
void h_7(double *state, double *sat_pos_vel, double *out_4389469270519108389) {
   out_4389469270519108389[0] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + state[7];
}
void H_7(double *state, double *sat_pos_vel, double *out_4901559713253712807) {
   out_4901559713253712807[0] = pow(sat_pos_vel[0] - state[0], 2)*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[1] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[1] - state[1], 2)*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[2] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[2] - state[2], 2)*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[3] = -(sat_pos_vel[0] - state[0])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[4] = -(sat_pos_vel[1] - state[1])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[5] = -(sat_pos_vel[2] - state[2])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[6] = 0;
   out_4901559713253712807[7] = 1;
   out_4901559713253712807[8] = 0;
   out_4901559713253712807[9] = 0;
   out_4901559713253712807[10] = 0;
}
void h_21(double *state, double *sat_pos_vel, double *out_4389469270519108389) {
   out_4389469270519108389[0] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + state[7];
}
void H_21(double *state, double *sat_pos_vel, double *out_4901559713253712807) {
   out_4901559713253712807[0] = pow(sat_pos_vel[0] - state[0], 2)*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[1] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[1] - state[1], 2)*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[2] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[2] - state[2], 2)*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[3] = -(sat_pos_vel[0] - state[0])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[4] = -(sat_pos_vel[1] - state[1])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[5] = -(sat_pos_vel[2] - state[2])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_4901559713253712807[6] = 0;
   out_4901559713253712807[7] = 1;
   out_4901559713253712807[8] = 0;
   out_4901559713253712807[9] = 0;
   out_4901559713253712807[10] = 0;
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

void gnss_update_6(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_6, H_6, NULL, in_z, in_R, in_ea, MAHA_THRESH_6);
}
void gnss_update_20(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_20, H_20, NULL, in_z, in_R, in_ea, MAHA_THRESH_20);
}
void gnss_update_7(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_7, H_7, NULL, in_z, in_R, in_ea, MAHA_THRESH_7);
}
void gnss_update_21(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_21, H_21, NULL, in_z, in_R, in_ea, MAHA_THRESH_21);
}
void gnss_err_fun(double *nom_x, double *delta_x, double *out_1171563384315072499) {
  err_fun(nom_x, delta_x, out_1171563384315072499);
}
void gnss_inv_err_fun(double *nom_x, double *true_x, double *out_7175360854741032365) {
  inv_err_fun(nom_x, true_x, out_7175360854741032365);
}
void gnss_H_mod_fun(double *state, double *out_6019779973188466781) {
  H_mod_fun(state, out_6019779973188466781);
}
void gnss_f_fun(double *state, double dt, double *out_1734095868312900332) {
  f_fun(state,  dt, out_1734095868312900332);
}
void gnss_F_fun(double *state, double dt, double *out_7391177847777738587) {
  F_fun(state,  dt, out_7391177847777738587);
}
void gnss_h_6(double *state, double *sat_pos, double *out_4559242126941663844) {
  h_6(state, sat_pos, out_4559242126941663844);
}
void gnss_H_6(double *state, double *sat_pos, double *out_6360356332846862462) {
  H_6(state, sat_pos, out_6360356332846862462);
}
void gnss_h_20(double *state, double *sat_pos, double *out_4943814902343407025) {
  h_20(state, sat_pos, out_4943814902343407025);
}
void gnss_H_20(double *state, double *sat_pos, double *out_6549302345954285139) {
  H_20(state, sat_pos, out_6549302345954285139);
}
void gnss_h_7(double *state, double *sat_pos_vel, double *out_4389469270519108389) {
  h_7(state, sat_pos_vel, out_4389469270519108389);
}
void gnss_H_7(double *state, double *sat_pos_vel, double *out_4901559713253712807) {
  H_7(state, sat_pos_vel, out_4901559713253712807);
}
void gnss_h_21(double *state, double *sat_pos_vel, double *out_4389469270519108389) {
  h_21(state, sat_pos_vel, out_4389469270519108389);
}
void gnss_H_21(double *state, double *sat_pos_vel, double *out_4901559713253712807) {
  H_21(state, sat_pos_vel, out_4901559713253712807);
}
void gnss_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF gnss = {
  .name = "gnss",
  .kinds = { 6, 20, 7, 21 },
  .feature_kinds = {  },
  .f_fun = gnss_f_fun,
  .F_fun = gnss_F_fun,
  .err_fun = gnss_err_fun,
  .inv_err_fun = gnss_inv_err_fun,
  .H_mod_fun = gnss_H_mod_fun,
  .predict = gnss_predict,
  .hs = {
    { 6, gnss_h_6 },
    { 20, gnss_h_20 },
    { 7, gnss_h_7 },
    { 21, gnss_h_21 },
  },
  .Hs = {
    { 6, gnss_H_6 },
    { 20, gnss_H_20 },
    { 7, gnss_H_7 },
    { 21, gnss_H_21 },
  },
  .updates = {
    { 6, gnss_update_6 },
    { 20, gnss_update_20 },
    { 7, gnss_update_7 },
    { 21, gnss_update_21 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_init(gnss);
