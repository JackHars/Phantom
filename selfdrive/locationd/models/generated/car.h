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
void car_err_fun(double *nom_x, double *delta_x, double *out_2865416645807171237);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_635870908016697860);
void car_H_mod_fun(double *state, double *out_7722955180222182331);
void car_f_fun(double *state, double dt, double *out_4596261882080719910);
void car_F_fun(double *state, double dt, double *out_6851550815964785237);
void car_h_25(double *state, double *unused, double *out_6075642558317368199);
void car_H_25(double *state, double *unused, double *out_2389774409591852645);
void car_h_24(double *state, double *unused, double *out_9164421185510050170);
void car_H_24(double *state, double *unused, double *out_2078411724020229925);
void car_h_30(double *state, double *unused, double *out_5600266289041832481);
void car_H_30(double *state, double *unused, double *out_7224124501357330635);
void car_h_26(double *state, double *unused, double *out_3196043175224164659);
void car_H_26(double *state, double *unused, double *out_8094367564531061918);
void car_h_27(double *state, double *unused, double *out_160011804583505863);
void car_H_27(double *state, double *unused, double *out_8511706489193955947);
void car_h_29(double *state, double *unused, double *out_1610643197212389030);
void car_H_29(double *state, double *unused, double *out_396463401133062365);
void car_h_28(double *state, double *unused, double *out_8616681029188511821);
void car_H_28(double *state, double *unused, double *out_9108965936508918547);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}