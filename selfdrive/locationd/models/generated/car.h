#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_7902506106427431264);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5110972235504517756);
void car_H_mod_fun(double *state, double *out_7718531475209591520);
void car_f_fun(double *state, double dt, double *out_5834922400930391556);
void car_F_fun(double *state, double dt, double *out_7718750476685544942);
void car_h_25(double *state, double *unused, double *out_5257902179626811817);
void car_H_25(double *state, double *unused, double *out_2248600704475429518);
void car_h_24(double *state, double *unused, double *out_3967815841241831722);
void car_H_24(double *state, double *unused, double *out_3510989768671169650);
void car_h_30(double *state, double *unused, double *out_5415088847977940962);
void car_H_30(double *state, double *unused, double *out_7365298206473753762);
void car_h_26(double *state, double *unused, double *out_676942798471127032);
void car_H_26(double *state, double *unused, double *out_3554836476430270663);
void car_h_27(double *state, double *unused, double *out_8294688231071144502);
void car_H_27(double *state, double *unused, double *out_8652880194310379074);
void car_h_29(double *state, double *unused, double *out_3381220616288056178);
void car_H_29(double *state, double *unused, double *out_4143067686967728890);
void car_h_28(double *state, double *unused, double *out_2191592646470270554);
void car_H_28(double *state, double *unused, double *out_871892538167030275);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}