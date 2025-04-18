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
 *                      Code generated with SymPy 1.13.3                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6959436757387446211) {
   out_6959436757387446211[0] = delta_x[0] + nom_x[0];
   out_6959436757387446211[1] = delta_x[1] + nom_x[1];
   out_6959436757387446211[2] = delta_x[2] + nom_x[2];
   out_6959436757387446211[3] = delta_x[3] + nom_x[3];
   out_6959436757387446211[4] = delta_x[4] + nom_x[4];
   out_6959436757387446211[5] = delta_x[5] + nom_x[5];
   out_6959436757387446211[6] = delta_x[6] + nom_x[6];
   out_6959436757387446211[7] = delta_x[7] + nom_x[7];
   out_6959436757387446211[8] = delta_x[8] + nom_x[8];
   out_6959436757387446211[9] = delta_x[9] + nom_x[9];
   out_6959436757387446211[10] = delta_x[10] + nom_x[10];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8367266362370558965) {
   out_8367266362370558965[0] = -nom_x[0] + true_x[0];
   out_8367266362370558965[1] = -nom_x[1] + true_x[1];
   out_8367266362370558965[2] = -nom_x[2] + true_x[2];
   out_8367266362370558965[3] = -nom_x[3] + true_x[3];
   out_8367266362370558965[4] = -nom_x[4] + true_x[4];
   out_8367266362370558965[5] = -nom_x[5] + true_x[5];
   out_8367266362370558965[6] = -nom_x[6] + true_x[6];
   out_8367266362370558965[7] = -nom_x[7] + true_x[7];
   out_8367266362370558965[8] = -nom_x[8] + true_x[8];
   out_8367266362370558965[9] = -nom_x[9] + true_x[9];
   out_8367266362370558965[10] = -nom_x[10] + true_x[10];
}
void H_mod_fun(double *state, double *out_831930405197755288) {
   out_831930405197755288[0] = 1.0;
   out_831930405197755288[1] = 0.0;
   out_831930405197755288[2] = 0.0;
   out_831930405197755288[3] = 0.0;
   out_831930405197755288[4] = 0.0;
   out_831930405197755288[5] = 0.0;
   out_831930405197755288[6] = 0.0;
   out_831930405197755288[7] = 0.0;
   out_831930405197755288[8] = 0.0;
   out_831930405197755288[9] = 0.0;
   out_831930405197755288[10] = 0.0;
   out_831930405197755288[11] = 0.0;
   out_831930405197755288[12] = 1.0;
   out_831930405197755288[13] = 0.0;
   out_831930405197755288[14] = 0.0;
   out_831930405197755288[15] = 0.0;
   out_831930405197755288[16] = 0.0;
   out_831930405197755288[17] = 0.0;
   out_831930405197755288[18] = 0.0;
   out_831930405197755288[19] = 0.0;
   out_831930405197755288[20] = 0.0;
   out_831930405197755288[21] = 0.0;
   out_831930405197755288[22] = 0.0;
   out_831930405197755288[23] = 0.0;
   out_831930405197755288[24] = 1.0;
   out_831930405197755288[25] = 0.0;
   out_831930405197755288[26] = 0.0;
   out_831930405197755288[27] = 0.0;
   out_831930405197755288[28] = 0.0;
   out_831930405197755288[29] = 0.0;
   out_831930405197755288[30] = 0.0;
   out_831930405197755288[31] = 0.0;
   out_831930405197755288[32] = 0.0;
   out_831930405197755288[33] = 0.0;
   out_831930405197755288[34] = 0.0;
   out_831930405197755288[35] = 0.0;
   out_831930405197755288[36] = 1.0;
   out_831930405197755288[37] = 0.0;
   out_831930405197755288[38] = 0.0;
   out_831930405197755288[39] = 0.0;
   out_831930405197755288[40] = 0.0;
   out_831930405197755288[41] = 0.0;
   out_831930405197755288[42] = 0.0;
   out_831930405197755288[43] = 0.0;
   out_831930405197755288[44] = 0.0;
   out_831930405197755288[45] = 0.0;
   out_831930405197755288[46] = 0.0;
   out_831930405197755288[47] = 0.0;
   out_831930405197755288[48] = 1.0;
   out_831930405197755288[49] = 0.0;
   out_831930405197755288[50] = 0.0;
   out_831930405197755288[51] = 0.0;
   out_831930405197755288[52] = 0.0;
   out_831930405197755288[53] = 0.0;
   out_831930405197755288[54] = 0.0;
   out_831930405197755288[55] = 0.0;
   out_831930405197755288[56] = 0.0;
   out_831930405197755288[57] = 0.0;
   out_831930405197755288[58] = 0.0;
   out_831930405197755288[59] = 0.0;
   out_831930405197755288[60] = 1.0;
   out_831930405197755288[61] = 0.0;
   out_831930405197755288[62] = 0.0;
   out_831930405197755288[63] = 0.0;
   out_831930405197755288[64] = 0.0;
   out_831930405197755288[65] = 0.0;
   out_831930405197755288[66] = 0.0;
   out_831930405197755288[67] = 0.0;
   out_831930405197755288[68] = 0.0;
   out_831930405197755288[69] = 0.0;
   out_831930405197755288[70] = 0.0;
   out_831930405197755288[71] = 0.0;
   out_831930405197755288[72] = 1.0;
   out_831930405197755288[73] = 0.0;
   out_831930405197755288[74] = 0.0;
   out_831930405197755288[75] = 0.0;
   out_831930405197755288[76] = 0.0;
   out_831930405197755288[77] = 0.0;
   out_831930405197755288[78] = 0.0;
   out_831930405197755288[79] = 0.0;
   out_831930405197755288[80] = 0.0;
   out_831930405197755288[81] = 0.0;
   out_831930405197755288[82] = 0.0;
   out_831930405197755288[83] = 0.0;
   out_831930405197755288[84] = 1.0;
   out_831930405197755288[85] = 0.0;
   out_831930405197755288[86] = 0.0;
   out_831930405197755288[87] = 0.0;
   out_831930405197755288[88] = 0.0;
   out_831930405197755288[89] = 0.0;
   out_831930405197755288[90] = 0.0;
   out_831930405197755288[91] = 0.0;
   out_831930405197755288[92] = 0.0;
   out_831930405197755288[93] = 0.0;
   out_831930405197755288[94] = 0.0;
   out_831930405197755288[95] = 0.0;
   out_831930405197755288[96] = 1.0;
   out_831930405197755288[97] = 0.0;
   out_831930405197755288[98] = 0.0;
   out_831930405197755288[99] = 0.0;
   out_831930405197755288[100] = 0.0;
   out_831930405197755288[101] = 0.0;
   out_831930405197755288[102] = 0.0;
   out_831930405197755288[103] = 0.0;
   out_831930405197755288[104] = 0.0;
   out_831930405197755288[105] = 0.0;
   out_831930405197755288[106] = 0.0;
   out_831930405197755288[107] = 0.0;
   out_831930405197755288[108] = 1.0;
   out_831930405197755288[109] = 0.0;
   out_831930405197755288[110] = 0.0;
   out_831930405197755288[111] = 0.0;
   out_831930405197755288[112] = 0.0;
   out_831930405197755288[113] = 0.0;
   out_831930405197755288[114] = 0.0;
   out_831930405197755288[115] = 0.0;
   out_831930405197755288[116] = 0.0;
   out_831930405197755288[117] = 0.0;
   out_831930405197755288[118] = 0.0;
   out_831930405197755288[119] = 0.0;
   out_831930405197755288[120] = 1.0;
}
void f_fun(double *state, double dt, double *out_373841461547170133) {
   out_373841461547170133[0] = dt*state[3] + state[0];
   out_373841461547170133[1] = dt*state[4] + state[1];
   out_373841461547170133[2] = dt*state[5] + state[2];
   out_373841461547170133[3] = state[3];
   out_373841461547170133[4] = state[4];
   out_373841461547170133[5] = state[5];
   out_373841461547170133[6] = dt*state[7] + state[6];
   out_373841461547170133[7] = dt*state[8] + state[7];
   out_373841461547170133[8] = state[8];
   out_373841461547170133[9] = state[9];
   out_373841461547170133[10] = state[10];
}
void F_fun(double *state, double dt, double *out_8068440977411101506) {
   out_8068440977411101506[0] = 1;
   out_8068440977411101506[1] = 0;
   out_8068440977411101506[2] = 0;
   out_8068440977411101506[3] = dt;
   out_8068440977411101506[4] = 0;
   out_8068440977411101506[5] = 0;
   out_8068440977411101506[6] = 0;
   out_8068440977411101506[7] = 0;
   out_8068440977411101506[8] = 0;
   out_8068440977411101506[9] = 0;
   out_8068440977411101506[10] = 0;
   out_8068440977411101506[11] = 0;
   out_8068440977411101506[12] = 1;
   out_8068440977411101506[13] = 0;
   out_8068440977411101506[14] = 0;
   out_8068440977411101506[15] = dt;
   out_8068440977411101506[16] = 0;
   out_8068440977411101506[17] = 0;
   out_8068440977411101506[18] = 0;
   out_8068440977411101506[19] = 0;
   out_8068440977411101506[20] = 0;
   out_8068440977411101506[21] = 0;
   out_8068440977411101506[22] = 0;
   out_8068440977411101506[23] = 0;
   out_8068440977411101506[24] = 1;
   out_8068440977411101506[25] = 0;
   out_8068440977411101506[26] = 0;
   out_8068440977411101506[27] = dt;
   out_8068440977411101506[28] = 0;
   out_8068440977411101506[29] = 0;
   out_8068440977411101506[30] = 0;
   out_8068440977411101506[31] = 0;
   out_8068440977411101506[32] = 0;
   out_8068440977411101506[33] = 0;
   out_8068440977411101506[34] = 0;
   out_8068440977411101506[35] = 0;
   out_8068440977411101506[36] = 1;
   out_8068440977411101506[37] = 0;
   out_8068440977411101506[38] = 0;
   out_8068440977411101506[39] = 0;
   out_8068440977411101506[40] = 0;
   out_8068440977411101506[41] = 0;
   out_8068440977411101506[42] = 0;
   out_8068440977411101506[43] = 0;
   out_8068440977411101506[44] = 0;
   out_8068440977411101506[45] = 0;
   out_8068440977411101506[46] = 0;
   out_8068440977411101506[47] = 0;
   out_8068440977411101506[48] = 1;
   out_8068440977411101506[49] = 0;
   out_8068440977411101506[50] = 0;
   out_8068440977411101506[51] = 0;
   out_8068440977411101506[52] = 0;
   out_8068440977411101506[53] = 0;
   out_8068440977411101506[54] = 0;
   out_8068440977411101506[55] = 0;
   out_8068440977411101506[56] = 0;
   out_8068440977411101506[57] = 0;
   out_8068440977411101506[58] = 0;
   out_8068440977411101506[59] = 0;
   out_8068440977411101506[60] = 1;
   out_8068440977411101506[61] = 0;
   out_8068440977411101506[62] = 0;
   out_8068440977411101506[63] = 0;
   out_8068440977411101506[64] = 0;
   out_8068440977411101506[65] = 0;
   out_8068440977411101506[66] = 0;
   out_8068440977411101506[67] = 0;
   out_8068440977411101506[68] = 0;
   out_8068440977411101506[69] = 0;
   out_8068440977411101506[70] = 0;
   out_8068440977411101506[71] = 0;
   out_8068440977411101506[72] = 1;
   out_8068440977411101506[73] = dt;
   out_8068440977411101506[74] = 0;
   out_8068440977411101506[75] = 0;
   out_8068440977411101506[76] = 0;
   out_8068440977411101506[77] = 0;
   out_8068440977411101506[78] = 0;
   out_8068440977411101506[79] = 0;
   out_8068440977411101506[80] = 0;
   out_8068440977411101506[81] = 0;
   out_8068440977411101506[82] = 0;
   out_8068440977411101506[83] = 0;
   out_8068440977411101506[84] = 1;
   out_8068440977411101506[85] = dt;
   out_8068440977411101506[86] = 0;
   out_8068440977411101506[87] = 0;
   out_8068440977411101506[88] = 0;
   out_8068440977411101506[89] = 0;
   out_8068440977411101506[90] = 0;
   out_8068440977411101506[91] = 0;
   out_8068440977411101506[92] = 0;
   out_8068440977411101506[93] = 0;
   out_8068440977411101506[94] = 0;
   out_8068440977411101506[95] = 0;
   out_8068440977411101506[96] = 1;
   out_8068440977411101506[97] = 0;
   out_8068440977411101506[98] = 0;
   out_8068440977411101506[99] = 0;
   out_8068440977411101506[100] = 0;
   out_8068440977411101506[101] = 0;
   out_8068440977411101506[102] = 0;
   out_8068440977411101506[103] = 0;
   out_8068440977411101506[104] = 0;
   out_8068440977411101506[105] = 0;
   out_8068440977411101506[106] = 0;
   out_8068440977411101506[107] = 0;
   out_8068440977411101506[108] = 1;
   out_8068440977411101506[109] = 0;
   out_8068440977411101506[110] = 0;
   out_8068440977411101506[111] = 0;
   out_8068440977411101506[112] = 0;
   out_8068440977411101506[113] = 0;
   out_8068440977411101506[114] = 0;
   out_8068440977411101506[115] = 0;
   out_8068440977411101506[116] = 0;
   out_8068440977411101506[117] = 0;
   out_8068440977411101506[118] = 0;
   out_8068440977411101506[119] = 0;
   out_8068440977411101506[120] = 1;
}
void h_6(double *state, double *sat_pos, double *out_7852871560996587817) {
   out_7852871560996587817[0] = sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2)) + state[6];
}
void H_6(double *state, double *sat_pos, double *out_243143003323188418) {
   out_243143003323188418[0] = (-sat_pos[0] + state[0])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_243143003323188418[1] = (-sat_pos[1] + state[1])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_243143003323188418[2] = (-sat_pos[2] + state[2])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_243143003323188418[3] = 0;
   out_243143003323188418[4] = 0;
   out_243143003323188418[5] = 0;
   out_243143003323188418[6] = 1;
   out_243143003323188418[7] = 0;
   out_243143003323188418[8] = 0;
   out_243143003323188418[9] = 0;
   out_243143003323188418[10] = 0;
}
void h_20(double *state, double *sat_pos, double *out_4551108956753681041) {
   out_4551108956753681041[0] = sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2)) + sat_pos[3]*state[10] + state[6] + state[9];
}
void H_20(double *state, double *sat_pos, double *out_7318006895490437704) {
   out_7318006895490437704[0] = (-sat_pos[0] + state[0])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_7318006895490437704[1] = (-sat_pos[1] + state[1])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_7318006895490437704[2] = (-sat_pos[2] + state[2])/sqrt(pow(-sat_pos[0] + state[0], 2) + pow(-sat_pos[1] + state[1], 2) + pow(-sat_pos[2] + state[2], 2));
   out_7318006895490437704[3] = 0;
   out_7318006895490437704[4] = 0;
   out_7318006895490437704[5] = 0;
   out_7318006895490437704[6] = 1;
   out_7318006895490437704[7] = 0;
   out_7318006895490437704[8] = 0;
   out_7318006895490437704[9] = 1;
   out_7318006895490437704[10] = sat_pos[3];
}
void h_7(double *state, double *sat_pos_vel, double *out_2640292203359565546) {
   out_2640292203359565546[0] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + state[7];
}
void H_7(double *state, double *sat_pos_vel, double *out_6911270637703440057) {
   out_6911270637703440057[0] = pow(sat_pos_vel[0] - state[0], 2)*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[1] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[1] - state[1], 2)*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[2] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[2] - state[2], 2)*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[3] = -(sat_pos_vel[0] - state[0])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[4] = -(sat_pos_vel[1] - state[1])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[5] = -(sat_pos_vel[2] - state[2])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[6] = 0;
   out_6911270637703440057[7] = 1;
   out_6911270637703440057[8] = 0;
   out_6911270637703440057[9] = 0;
   out_6911270637703440057[10] = 0;
}
void h_21(double *state, double *sat_pos_vel, double *out_2640292203359565546) {
   out_2640292203359565546[0] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + (sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2)) + state[7];
}
void H_21(double *state, double *sat_pos_vel, double *out_6911270637703440057) {
   out_6911270637703440057[0] = pow(sat_pos_vel[0] - state[0], 2)*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[3] - state[3])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[1] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[1] - state[1])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[1] - state[1], 2)*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[4] - state[4])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[2] = (sat_pos_vel[0] - state[0])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[3] - state[3])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + (sat_pos_vel[1] - state[1])*(sat_pos_vel[2] - state[2])*(sat_pos_vel[4] - state[4])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) + pow(sat_pos_vel[2] - state[2], 2)*(sat_pos_vel[5] - state[5])/pow(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2), 3.0/2.0) - (sat_pos_vel[5] - state[5])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[3] = -(sat_pos_vel[0] - state[0])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[4] = -(sat_pos_vel[1] - state[1])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[5] = -(sat_pos_vel[2] - state[2])/sqrt(pow(sat_pos_vel[0] - state[0], 2) + pow(sat_pos_vel[1] - state[1], 2) + pow(sat_pos_vel[2] - state[2], 2));
   out_6911270637703440057[6] = 0;
   out_6911270637703440057[7] = 1;
   out_6911270637703440057[8] = 0;
   out_6911270637703440057[9] = 0;
   out_6911270637703440057[10] = 0;
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
void gnss_err_fun(double *nom_x, double *delta_x, double *out_6959436757387446211) {
  err_fun(nom_x, delta_x, out_6959436757387446211);
}
void gnss_inv_err_fun(double *nom_x, double *true_x, double *out_8367266362370558965) {
  inv_err_fun(nom_x, true_x, out_8367266362370558965);
}
void gnss_H_mod_fun(double *state, double *out_831930405197755288) {
  H_mod_fun(state, out_831930405197755288);
}
void gnss_f_fun(double *state, double dt, double *out_373841461547170133) {
  f_fun(state,  dt, out_373841461547170133);
}
void gnss_F_fun(double *state, double dt, double *out_8068440977411101506) {
  F_fun(state,  dt, out_8068440977411101506);
}
void gnss_h_6(double *state, double *sat_pos, double *out_7852871560996587817) {
  h_6(state, sat_pos, out_7852871560996587817);
}
void gnss_H_6(double *state, double *sat_pos, double *out_243143003323188418) {
  H_6(state, sat_pos, out_243143003323188418);
}
void gnss_h_20(double *state, double *sat_pos, double *out_4551108956753681041) {
  h_20(state, sat_pos, out_4551108956753681041);
}
void gnss_H_20(double *state, double *sat_pos, double *out_7318006895490437704) {
  H_20(state, sat_pos, out_7318006895490437704);
}
void gnss_h_7(double *state, double *sat_pos_vel, double *out_2640292203359565546) {
  h_7(state, sat_pos_vel, out_2640292203359565546);
}
void gnss_H_7(double *state, double *sat_pos_vel, double *out_6911270637703440057) {
  H_7(state, sat_pos_vel, out_6911270637703440057);
}
void gnss_h_21(double *state, double *sat_pos_vel, double *out_2640292203359565546) {
  h_21(state, sat_pos_vel, out_2640292203359565546);
}
void gnss_H_21(double *state, double *sat_pos_vel, double *out_6911270637703440057) {
  H_21(state, sat_pos_vel, out_6911270637703440057);
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
