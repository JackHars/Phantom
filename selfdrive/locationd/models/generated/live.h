#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void live_update_3(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_19(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_2948596063703496241);
void live_err_fun(double *nom_x, double *delta_x, double *out_1914573480228048610);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_3339914784888489593);
void live_H_mod_fun(double *state, double *out_6505009830328729646);
void live_f_fun(double *state, double dt, double *out_3271748634968838254);
void live_F_fun(double *state, double dt, double *out_7057950871955488399);
void live_h_3(double *state, double *unused, double *out_6328017091412519298);
void live_H_3(double *state, double *unused, double *out_2853354366792570472);
void live_h_4(double *state, double *unused, double *out_1355457730909575665);
void live_H_4(double *state, double *unused, double *out_3836446576337338235);
void live_h_9(double *state, double *unused, double *out_3807612207790732406);
void live_H_9(double *state, double *unused, double *out_5532088256168374560);
void live_h_10(double *state, double *unused, double *out_2078649022526801553);
void live_H_10(double *state, double *unused, double *out_5735590666865728217);
void live_h_12(double *state, double *unused, double *out_5606888354492758240);
void live_H_12(double *state, double *unused, double *out_5316138224036728159);
void live_h_31(double *state, double *unused, double *out_5636291282168742980);
void live_H_31(double *state, double *unused, double *out_2641048448893308338);
void live_h_32(double *state, double *unused, double *out_653768358545988260);
void live_H_32(double *state, double *unused, double *out_3659317295367092848);
void live_h_13(double *state, double *unused, double *out_1876462356006730132);
void live_H_13(double *state, double *unused, double *out_444751983671985872);
void live_h_14(double *state, double *unused, double *out_3807612207790732406);
void live_H_14(double *state, double *unused, double *out_5532088256168374560);
void live_h_19(double *state, double *unused, double *out_6187063581850473412);
void live_H_19(double *state, double *unused, double *out_8481975966438745875);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}