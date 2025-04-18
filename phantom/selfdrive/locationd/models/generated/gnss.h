#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void gnss_update_6(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_update_20(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_update_7(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_update_21(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_err_fun(double *nom_x, double *delta_x, double *out_6959436757387446211);
void gnss_inv_err_fun(double *nom_x, double *true_x, double *out_8367266362370558965);
void gnss_H_mod_fun(double *state, double *out_831930405197755288);
void gnss_f_fun(double *state, double dt, double *out_373841461547170133);
void gnss_F_fun(double *state, double dt, double *out_8068440977411101506);
void gnss_h_6(double *state, double *sat_pos, double *out_7852871560996587817);
void gnss_H_6(double *state, double *sat_pos, double *out_243143003323188418);
void gnss_h_20(double *state, double *sat_pos, double *out_4551108956753681041);
void gnss_H_20(double *state, double *sat_pos, double *out_7318006895490437704);
void gnss_h_7(double *state, double *sat_pos_vel, double *out_2640292203359565546);
void gnss_H_7(double *state, double *sat_pos_vel, double *out_6911270637703440057);
void gnss_h_21(double *state, double *sat_pos_vel, double *out_2640292203359565546);
void gnss_H_21(double *state, double *sat_pos_vel, double *out_6911270637703440057);
void gnss_predict(double *in_x, double *in_P, double *in_Q, double dt);
}