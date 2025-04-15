#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void gnss_update_6(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_update_20(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_update_7(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_update_21(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void gnss_err_fun(double *nom_x, double *delta_x, double *out_1171563384315072499);
void gnss_inv_err_fun(double *nom_x, double *true_x, double *out_7175360854741032365);
void gnss_H_mod_fun(double *state, double *out_6019779973188466781);
void gnss_f_fun(double *state, double dt, double *out_1734095868312900332);
void gnss_F_fun(double *state, double dt, double *out_7391177847777738587);
void gnss_h_6(double *state, double *sat_pos, double *out_4559242126941663844);
void gnss_H_6(double *state, double *sat_pos, double *out_6360356332846862462);
void gnss_h_20(double *state, double *sat_pos, double *out_4943814902343407025);
void gnss_H_20(double *state, double *sat_pos, double *out_6549302345954285139);
void gnss_h_7(double *state, double *sat_pos_vel, double *out_4389469270519108389);
void gnss_H_7(double *state, double *sat_pos_vel, double *out_4901559713253712807);
void gnss_h_21(double *state, double *sat_pos_vel, double *out_4389469270519108389);
void gnss_H_21(double *state, double *sat_pos_vel, double *out_4901559713253712807);
void gnss_predict(double *in_x, double *in_P, double *in_Q, double dt);
}