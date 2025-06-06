#include "common_dbc.h"

namespace {

const Signal sigs_35[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "STEER_ANGLE",
      .b1 = 2,
      .b2 = 14,
      .bo = 48,
      .is_signed = false,
      .factor = 0.5,
      .offset = -2048.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "TORQUE_MOTOR",
      .b1 = 16,
      .b2 = 16,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = -4096.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_37[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "ENGINE_RPM",
      .b1 = 0,
      .b2 = 16,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ENGINE_TRQ_REQ",
      .b1 = 16,
      .b2 = 12,
      .bo = 36,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_39[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "UNKNOWN_1",
      .b1 = 4,
      .b2 = 12,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "UNKNOWN_2",
      .b1 = 20,
      .b2 = 12,
      .bo = 32,
      .is_signed = false,
      .factor = 2,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_41[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
};
const Signal sigs_43[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "YAW_RATE",
      .b1 = 4,
      .b2 = 12,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = -2048.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ACCEL_LONGITUDINAL",
      .b1 = 20,
      .b2 = 12,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = -2048.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ACCEL_LATERAL",
      .b1 = 36,
      .b2 = 12,
      .bo = 16,
      .is_signed = false,
      .factor = 1,
      .offset = -2048.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_47[] = {
    {
      .name = "UNKNOWN_SPEED_1",
      .b1 = 0,
      .b2 = 16,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "UNKNOWN_SPEED_2",
      .b1 = 16,
      .b2 = 16,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "UNKNOWN_LOAD",
      .b1 = 40,
      .b2 = 8,
      .bo = 16,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "UNKNOWN_BRAKE",
      .b1 = 48,
      .b2 = 8,
      .bo = 8,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_49[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "TORQUE_DRIVER",
      .b1 = 4,
      .b2 = 12,
      .bo = 48,
      .is_signed = true,
      .factor = 1,
      .offset = 1024,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "TORQUE_EPS_TOTAL",
      .b1 = 20,
      .b2 = 12,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = -2000.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "XXX_OTHER_TORQUE_DRIVER",
      .b1 = 36,
      .b2 = 12,
      .bo = 16,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "EPS_STATUS",
      .b1 = 32,
      .b2 = 4,
      .bo = 28,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_53[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "ENG_TORQUE_REQ",
      .b1 = 4,
      .b2 = 12,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ENG_TORQUE_OUT",
      .b1 = 20,
      .b2 = 12,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_121[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "BRK_PRESSURE",
      .b1 = 4,
      .b2 = 12,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "BRAKE_PEDAL",
      .b1 = 20,
      .b2 = 12,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_129[] = {
    {
      .name = "THROTTLE_POSITION",
      .b1 = 40,
      .b2 = 8,
      .bo = 16,
      .is_signed = false,
      .factor = 0.5,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_133[] = {
    {
      .name = "SHIFT_PENDING",
      .b1 = 5,
      .b2 = 1,
      .bo = 58,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ACTUAL_GEAR",
      .b1 = 12,
      .b2 = 4,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "DESIRED_GEAR",
      .b1 = 8,
      .b2 = 4,
      .bo = 52,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "SPEED_TURBINE",
      .b1 = 28,
      .b2 = 12,
      .bo = 24,
      .is_signed = false,
      .factor = 0.04,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_139[] = {
    {
      .name = "WHEEL_SPEED_RL",
      .b1 = 4,
      .b2 = 12,
      .bo = 48,
      .is_signed = false,
      .factor = 0.02,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "WHEEL_SPEED_RR",
      .b1 = 20,
      .b2 = 12,
      .bo = 32,
      .is_signed = false,
      .factor = 0.02,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "WHEEL_SPEED_FL",
      .b1 = 36,
      .b2 = 12,
      .bo = 16,
      .is_signed = false,
      .factor = 0.02,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "WHEEL_SPEED_FR",
      .b1 = 52,
      .b2 = 12,
      .bo = 0,
      .is_signed = false,
      .factor = 0.02,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_153[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "ACC_ENG_TRQ_REQ",
      .b1 = 4,
      .b2 = 8,
      .bo = 52,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ACC_GAS_CONTROL",
      .b1 = 0,
      .b2 = 1,
      .bo = 63,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "NEW_SIGNAL_3",
      .b1 = 8,
      .b2 = 4,
      .bo = 52,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = true,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "NEW_SIGNAL_2",
      .b1 = 16,
      .b2 = 4,
      .bo = 44,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = true,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ACC_STATUS",
      .b1 = 18,
      .b2 = 2,
      .bo = 44,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "NEW_SIGNAL_1",
      .b1 = 24,
      .b2 = 8,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = true,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_163[] = {
    {
      .name = "MAYBE_ACC_TARGET_SPEED",
      .b1 = 24,
      .b2 = 8,
      .bo = 32,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_166[] = {
    {
      .name = "COUNTER",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 56,
      .b2 = 8,
      .bo = 0,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "LKAS_COMMAND",
      .b1 = 8,
      .b2 = 16,
      .bo = 40,
      .is_signed = false,
      .factor = 1,
      .offset = -1024.0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "LKAS_CONTROL_BIT",
      .b1 = 30,
      .b2 = 1,
      .bo = 33,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_173[] = {
    {
      .name = "COUNTER",
      .b1 = 24,
      .b2 = 4,
      .bo = 36,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 32,
      .b2 = 8,
      .bo = 24,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "SHIFTER_POSITION",
      .b1 = 4,
      .b2 = 4,
      .bo = 56,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_177[] = {
    {
      .name = "COUNTER",
      .b1 = 8,
      .b2 = 4,
      .bo = 52,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CHECKSUM",
      .b1 = 16,
      .b2 = 8,
      .bo = 40,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::CHRYSLER_CHECKSUM,
    },
    {
      .name = "CANCEL",
      .b1 = 7,
      .b2 = 1,
      .bo = 56,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "SET_PLUS",
      .b1 = 5,
      .b2 = 1,
      .bo = 58,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "SET_MINUS",
      .b1 = 4,
      .b2 = 1,
      .bo = 59,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "RESUME",
      .b1 = 3,
      .b2 = 1,
      .bo = 60,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "CRUISE_CONTROL",
      .b1 = 1,
      .b2 = 1,
      .bo = 62,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_221[] = {
    {
      .name = "TRAC_OFF",
      .b1 = 49,
      .b2 = 1,
      .bo = 14,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_232[] = {
    {
      .name = "ACC_SET_SPEED_KPH",
      .b1 = 8,
      .b2 = 8,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "ACC_SET_SPEED_MPH",
      .b1 = 16,
      .b2 = 8,
      .bo = 40,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_250[] = {
    {
      .name = "LKAS_HUD_1",
      .b1 = 12,
      .b2 = 4,
      .bo = 48,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "UNKNOWN",
      .b1 = 32,
      .b2 = 4,
      .bo = 28,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = true,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "HIGH_BEAM_CONTROL",
      .b1 = 40,
      .b2 = 1,
      .bo = 23,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "LKAS_HUD_2",
      .b1 = 48,
      .b2 = 4,
      .bo = 12,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = true,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_464[] = {
    {
      .name = "DRIVER_SEATBELT_STATUS",
      .b1 = 23,
      .b2 = 1,
      .bo = 40,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_657[] = {
    {
      .name = "DOOR_OPEN_FL",
      .b1 = 22,
      .b2 = 1,
      .bo = 41,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "DOOR_OPEN_FR",
      .b1 = 18,
      .b2 = 1,
      .bo = 45,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = true,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "DOOR_OPEN_RL",
      .b1 = 20,
      .b2 = 1,
      .bo = 43,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "DOOR_OPEN_RR",
      .b1 = 19,
      .b2 = 1,
      .bo = 44,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};
const Signal sigs_792[] = {
    {
      .name = "BLINKER_LEFT",
      .b1 = 7,
      .b2 = 1,
      .bo = 56,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "BLINKER_RIGHT",
      .b1 = 6,
      .b2 = 1,
      .bo = 57,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "AUTO_HIGH_BEAM",
      .b1 = 5,
      .b2 = 1,
      .bo = 58,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
    {
      .name = "HIGH_BEAM_FLASH",
      .b1 = 4,
      .b2 = 1,
      .bo = 59,
      .is_signed = false,
      .factor = 1,
      .offset = 0,
      .is_little_endian = false,
      .type = SignalType::DEFAULT,
    },
};

const Msg msgs[] = {
  {
    .name = "EPS_1",
    .address = 0x23,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_35),
    .sigs = sigs_35,
  },
  {
    .name = "PCM_1",
    .address = 0x25,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_37),
    .sigs = sigs_37,
  },
  {
    .name = "UNKNOWN_MSG_1",
    .address = 0x27,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_39),
    .sigs = sigs_39,
  },
  {
    .name = "UNKNOWN_MSG_2",
    .address = 0x29,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_41),
    .sigs = sigs_41,
  },
  {
    .name = "ABS_2",
    .address = 0x2B,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_43),
    .sigs = sigs_43,
  },
  {
    .name = "ABS_3",
    .address = 0x2F,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_47),
    .sigs = sigs_47,
  },
  {
    .name = "EPS_2",
    .address = 0x31,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_49),
    .sigs = sigs_49,
  },
  {
    .name = "PCM_2",
    .address = 0x35,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_53),
    .sigs = sigs_53,
  },
  {
    .name = "ABS_1",
    .address = 0x79,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_121),
    .sigs = sigs_121,
  },
  {
    .name = "TPS_1",
    .address = 0x81,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_129),
    .sigs = sigs_129,
  },
  {
    .name = "TCM_1",
    .address = 0x85,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_133),
    .sigs = sigs_133,
  },
  {
    .name = "ABS_4",
    .address = 0x8B,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_139),
    .sigs = sigs_139,
  },
  {
    .name = "DASM_ACC_CMD_1",
    .address = 0x99,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_153),
    .sigs = sigs_153,
  },
  {
    .name = "DASM_ACC_CMD_2",
    .address = 0xA3,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_163),
    .sigs = sigs_163,
  },
  {
    .name = "DASM_LKAS_CMD",
    .address = 0xA6,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_166),
    .sigs = sigs_166,
  },
  {
    .name = "SHIFTER_ASSM",
    .address = 0xAD,
    .size = 5,
    .num_sigs = ARRAYSIZE(sigs_173),
    .sigs = sigs_173,
  },
  {
    .name = "CSWC",
    .address = 0xB1,
    .size = 3,
    .num_sigs = ARRAYSIZE(sigs_177),
    .sigs = sigs_177,
  },
  {
    .name = "ICS",
    .address = 0xDD,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_221),
    .sigs = sigs_221,
  },
  {
    .name = "DASM_ACC_HUD",
    .address = 0xE8,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_232),
    .sigs = sigs_232,
  },
  {
    .name = "DASM_LKAS_HUD",
    .address = 0xFA,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_250),
    .sigs = sigs_250,
  },
  {
    .name = "ORM",
    .address = 0x1D0,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_464),
    .sigs = sigs_464,
  },
  {
    .name = "BCM",
    .address = 0x291,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_657),
    .sigs = sigs_657,
  },
  {
    .name = "SCCM",
    .address = 0x318,
    .size = 8,
    .num_sigs = ARRAYSIZE(sigs_792),
    .sigs = sigs_792,
  },
};

const Val vals[] = {
    {
      .name = "SHIFTER_POSITION",
      .address = 0xAD,
      .def_val = "4 D 3 N 2 R 1 P",
      .sigs = sigs_173,
    },
};

}

const DBC stellantis_dasm = {
  .name = "stellantis_dasm",
  .num_msgs = ARRAYSIZE(msgs),
  .msgs = msgs,
  .vals = vals,
  .num_vals = ARRAYSIZE(vals),
};

dbc_init(stellantis_dasm)