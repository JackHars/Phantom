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
void live_H(double *in_vec, double *out_4608734394942937268);
void live_err_fun(double *nom_x, double *delta_x, double *out_8998781008598066513);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_4662763525816703417);
void live_H_mod_fun(double *state, double *out_6374681961959799107);
void live_f_fun(double *state, double dt, double *out_2456371683165485830);
void live_F_fun(double *state, double dt, double *out_3863583991672395700);
void live_h_3(double *state, double *unused, double *out_6765820075159274791);
void live_H_3(double *state, double *unused, double *out_397744536035736533);
void live_h_4(double *state, double *unused, double *out_3073383699819698190);
void live_H_4(double *state, double *unused, double *out_5456110413973079036);
void live_h_9(double *state, double *unused, double *out_110858870840770519);
void live_H_9(double *state, double *unused, double *out_3622098827230759785);
void live_h_10(double *state, double *unused, double *out_3415126757513083640);
void live_H_10(double *state, double *unused, double *out_2911384318805587576);
void live_h_12(double *state, double *unused, double *out_3031973687550768630);
void live_H_12(double *state, double *unused, double *out_7562665925712288605);
void live_h_31(double *state, double *unused, double *out_8663222276390609010);
void live_H_31(double *state, double *unused, double *out_9160810540156314704);
void live_h_32(double *state, double *unused, double *out_918055889808159767);
void live_H_32(double *state, double *unused, double *out_1412789593691532402);
void live_h_13(double *state, double *unused, double *out_855494310541384711);
void live_H_13(double *state, double *unused, double *out_4716386551728464637);
void live_h_14(double *state, double *unused, double *out_110858870840770519);
void live_H_14(double *state, double *unused, double *out_3622098827230759785);
void live_h_19(double *state, double *unused, double *out_2752073823674241136);
void live_H_19(double *state, double *unused, double *out_5208938406856039524);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}