# flake8: noqa

from collections import defaultdict
from typing import Dict

from cereal import car
from selfdrive.car import dbc_dict

Ecu = car.CarParams.Ecu
NetworkLocation = car.CarParams.NetworkLocation
TransmissionType = car.CarParams.TransmissionType
GearShifter = car.CarState.GearShifter

class CarControllerParams:
  HCA_STEP = 2                   # HCA_01 message frequency 50Hz
  LDW_STEP = 10                  # LDW_02 message frequency 10Hz
  GRA_ACC_STEP = 3               # GRA_ACC_01 message frequency 33Hz

  GRA_VBP_STEP = 100             # Send ACC virtual button presses once a second
  GRA_VBP_COUNT = 16             # Send VBP messages for ~0.5s (GRA_ACC_STEP * 16)

  # Observed documented MQB limits: 3.00 Nm max, rate of change 5.00 Nm/sec.
  # Limiting rate-of-change based on real-world testing and Comma's safety
  # requirements for minimum time to lane departure.
  STEER_MAX = 300                # Max heading control assist torque 3.00 Nm
  STEER_DELTA_UP = 4             # Max HCA reached in 1.50s (STEER_MAX / (50Hz * 1.50))
  STEER_DELTA_DOWN = 10          # Min HCA reached in 0.60s (STEER_MAX / (50Hz * 0.60))
  STEER_DRIVER_ALLOWANCE = 80
  STEER_DRIVER_MULTIPLIER = 3    # weight driver torque heavily
  STEER_DRIVER_FACTOR = 1        # from dbc

class CANBUS:
  pt = 0
  cam = 2

class DBC_FILES:
  mqb = "vw_mqb_2010"  # Used for all cars with MQB-style CAN messaging

DBC = defaultdict(lambda: dbc_dict(DBC_FILES.mqb, None))  # type: Dict[str, Dict[str, str]]

BUTTON_STATES = {
  "accelCruise": False,
  "decelCruise": False,
  "cancel": False,
  "setCruise": False,
  "resumeCruise": False,
  "gapAdjustCruise": False
}

MQB_LDW_MESSAGES = {
  "none": 0,                            # Nothing to display
  "laneAssistUnavailChime": 1,          # "Lane Assist currently not available." with chime
  "laneAssistUnavailNoSensorChime": 3,  # "Lane Assist not available. No sensor view." with chime
  "laneAssistTakeOverUrgent": 4,        # "Lane Assist: Please Take Over Steering" with urgent beep
  "emergencyAssistUrgent": 6,           # "Emergency Assist: Please Take Over Steering" with urgent beep
  "laneAssistTakeOverChime": 7,         # "Lane Assist: Please Take Over Steering" with chime
  "laneAssistTakeOverSilent": 8,        # "Lane Assist: Please Take Over Steering" silent
  "emergencyAssistChangingLanes": 9,    # "Emergency Assist: Changing lanes..." with urgent beep
  "laneAssistDeactivated": 10,          # "Lane Assist deactivated." silent with persistent icon afterward
}

# Check the 7th and 8th characters of the VIN before adding a new CAR. If the
# chassis code is already listed below, don't add a new CAR, just add to the
# FW_VERSIONS for that existing CAR.
# Exception: SEAT Leon and SEAT Ateca share a chassis code

class CAR:
  ARTEON_MK1 = "VOLKSWAGEN ARTEON 1ST GEN"          # Chassis AN, Mk1 VW Arteon and variants
  ATLAS_MK1 = "VOLKSWAGEN ATLAS 1ST GEN"            # Chassis CA, Mk1 VW Atlas and Atlas Cross Sport
  GOLF_MK7 = "VOLKSWAGEN GOLF 7TH GEN"              # Chassis 5G/AU/BA/BE, Mk7 VW Golf and variants
  JETTA_MK7 = "VOLKSWAGEN JETTA 7TH GEN"            # Chassis BU, Mk7 Jetta
  PASSAT_MK8 = "VOLKSWAGEN PASSAT 8TH GEN"          # Chassis 3G, Mk8 Passat and variants
  TCROSS_MK1 = "VOLKSWAGEN T-CROSS 1ST GEN"         # Chassis C1, Mk1 VW T-Cross SWB and LWB variants
  TIGUAN_MK2 = "VOLKSWAGEN TIGUAN 2ND GEN"          # Chassis AD/BW, Mk2 VW Tiguan and variants
  TOURAN_MK2 = "VOLKSWAGEN TOURAN 2ND GEN"          # Chassis 1T, Mk2 VW Touran and variants
  TRANSPORTER_T61 = "VOLKSWAGEN TRANSPORTER T6.1"   # Chassis 7H/7L, T6-facelift Transporter/Multivan/Caravelle/California
  AUDI_A3_MK3 = "AUDI A3 3RD GEN"                   # Chassis 8V/FF, Mk3 Audi A3 and variants
  AUDI_Q2_MK1 = "AUDI Q2 1ST GEN"                   # Chassis GA, Mk1 Audi Q2 (RoW) and Q2L (China only)
  SEAT_ATECA_MK1 = "SEAT ATECA 1ST GEN"             # Chassis 5F, Mk1 SEAT Ateca and CUPRA Ateca
  SEAT_LEON_MK3 = "SEAT LEON 3RD GEN"               # Chassis 5F, Mk3 SEAT Leon and variants
  SKODA_KAMIQ_MK1 = "SKODA KAMIQ 1ST GEN"           # Chassis NW, Mk1 Skoda Kamiq
  SKODA_KAROQ_MK1 = "SKODA KAROQ 1ST GEN"           # Chassis NU, Mk1 Skoda Karoq
  SKODA_KODIAQ_MK1 = "SKODA KODIAQ 1ST GEN"         # Chassis NS, Mk1 Skoda Kodiaq
  SKODA_SCALA_MK1 = "SKODA SCALA 1ST GEN"           # Chassis NW, Mk1 Skoda Scala and Skoda Kamiq
  SKODA_SUPERB_MK3 = "SKODA SUPERB 3RD GEN"         # Chassis 3V/NP, Mk3 Skoda Superb and variants
  SKODA_OCTAVIA_MK3 = "SKODA OCTAVIA 3RD GEN"       # Chassis NE, Mk3 Skoda Octavia and variants

# All supported cars should return FW from the engine, srs, eps, and fwdRadar. Cars
# with a manual trans won't return transmission firmware, but all other cars will.
#
# The 0xF187 SW part number query should return in the form of N[NX][NX] NNN NNN [X[X]],
# where N=number, X=letter, and the trailing two letters are optional. Performance
# tuners sometimes tamper with that field (e.g. 8V0 9C0 BB0 1 from COBB/EQT). Tampered
# ECU SW part numbers are invalid for vehicle ID and compatibility checks. Try to have
# them repaired by the tuner before including them in openpilot.

FW_VERSIONS = {
  CAR.ARTEON_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x873G0906259P \xf1\x890001',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x8709G927158L \xf1\x893611',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655DL\xf1\x890732\xf1\x82\0161812141812171105141123052J00',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0910143C \xf1\x892211\xf1\x82\00567B0020800',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572T \xf1\x890383',
    ],
  },
  CAR.ATLAS_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8703H906026AA\xf1\x899970',
      b'\xf1\x8703H906026F \xf1\x896696',
      b'\xf1\x8703H906026F \xf1\x899970',
      b'\xf1\x8703H906026S \xf1\x896693',
      b'\xf1\x8703H906026S \xf1\x899970',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x8709G927158A \xf1\x893387',
      b'\xf1\x8709G927158DR\xf1\x893536',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655BC\xf1\x890503\xf1\x82\0161914151912001103111122031200',
      b'\xf1\x873Q0959655BN\xf1\x890713\xf1\x82\0162214152212001105141122052900',
      b'\xf1\x873Q0959655DB\xf1\x890720\xf1\x82\0162214152212001105141122052900',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x873QF909144B \xf1\x891582\xf1\x82\00571B60924A1',
      b'\xf1\x875Q0909143P \xf1\x892051\xf1\x820528B6090105',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x875Q0907572H \xf1\x890620',
      b'\xf1\x875Q0907572J \xf1\x890654',
      b'\xf1\x875Q0907572P \xf1\x890682',
    ],
  },
  CAR.GOLF_MK7: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906016A \xf1\x897697',
      b'\xf1\x8704E906016AD\xf1\x895758',
      b'\xf1\x8704E906023AG\xf1\x891726',
      b'\xf1\x8704E906023BN\xf1\x894518',
      b'\xf1\x8704E906024K \xf1\x896811',
      b'\xf1\x8704E906027GR\xf1\x892394',
      b'\xf1\x8704E906027HD\xf1\x893742',
      b'\xf1\x8704E906027MA\xf1\x894958',
      b'\xf1\x8704L906021DT\xf1\x895520',
      b'\xf1\x8704L906021N \xf1\x895518',
      b'\xf1\x8704L906026BP\xf1\x897608',
      b'\xf1\x8704L906026NF\xf1\x899528',
      b'\xf1\x8704L906056CL\xf1\x893823',
      b'\xf1\x8704L906056CR\xf1\x895813',
      b'\xf1\x8704L906056HE\xf1\x893758',
      b'\xf1\x870EA906016A \xf1\x898343',
      b'\xf1\x870EA906016F \xf1\x895002',
      b'\xf1\x870EA906016S \xf1\x897207',
      b'\xf1\x875G0906259  \xf1\x890007',
      b'\xf1\x875G0906259J \xf1\x890002',
      b'\xf1\x875G0906259L \xf1\x890002',
      b'\xf1\x875G0906259N \xf1\x890003',
      b'\xf1\x875G0906259Q \xf1\x890002',
      b'\xf1\x875G0906259Q \xf1\x892313',
      b'\xf1\x878V0906259H \xf1\x890002',
      b'\xf1\x878V0906259J \xf1\x890003',
      b'\xf1\x878V0906259K \xf1\x890001',
      b'\xf1\x878V0906259P \xf1\x890001',
      b'\xf1\x878V0906259Q \xf1\x890002',
      b'\xf1\x878V0906264F \xf1\x890003',
      b'\xf1\x878V0906264L \xf1\x890002',
      b'\xf1\x878V0906264M \xf1\x890001',
      b'\xf1\x878V09C0BB01 \xf1\x890001',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x8709G927749AP\xf1\x892943',
      b'\xf1\x8709S927158A \xf1\x893585',
      b'\xf1\x870CW300041H \xf1\x891010',
      b'\xf1\x870CW300042F \xf1\x891604',
      b'\xf1\x870CW300043B \xf1\x891601',
      b'\xf1\x870CW300044S \xf1\x894530',
      b'\xf1\x870CW300045  \xf1\x894531',
      b'\xf1\x870CW300047D \xf1\x895261',
      b'\xf1\x870CW300048J \xf1\x890611',
      b'\xf1\x870D9300012  \xf1\x894904',
      b'\xf1\x870D9300012  \xf1\x894913',
      b'\xf1\x870D9300012  \xf1\x894937',
      b'\xf1\x870D9300012  \xf1\x895045',
      b'\xf1\x870D9300014M \xf1\x895004',
      b'\xf1\x870D9300020S \xf1\x895201',
      b'\xf1\x870D9300040A \xf1\x893613',
      b'\xf1\x870D9300040S \xf1\x894311',
      b'\xf1\x870D9300041H \xf1\x895220',
      b'\xf1\x870DD300045K \xf1\x891120',
      b'\xf1\x870DD300046F \xf1\x891601',
      b'\xf1\x870GC300012A \xf1\x891403',
      b'\xf1\x870GC300014B \xf1\x892401',
      b'\xf1\x870GC300014B \xf1\x892405',
      b'\xf1\x870GC300020G \xf1\x892401',
      b'\xf1\x870GC300020G \xf1\x892403',
      b'\xf1\x870GC300020G \xf1\x892404',
      b'\xf1\x870GC300043T \xf1\x899999',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655AA\xf1\x890386\xf1\x82\0211413001113120043114317121C111C9113',
      b'\xf1\x875Q0959655AA\xf1\x890386\xf1\x82\0211413001113120053114317121C111C9113',
      b'\xf1\x875Q0959655AA\xf1\x890388\xf1\x82\0211413001113120043114317121C111C9113',
      b'\xf1\x875Q0959655AA\xf1\x890388\xf1\x82\0211413001113120043114417121411149113',
      b'\xf1\x875Q0959655AA\xf1\x890388\xf1\x82\0211413001113120053114317121C111C9113',
      b'\xf1\x875Q0959655BH\xf1\x890336\xf1\x82\02314160011123300314211012230229333463100',
      b'\xf1\x875Q0959655BT\xf1\x890403\xf1\x82\023141600111233003142404A2252229333463100',
      b'\xf1\x875Q0959655BT\xf1\x890403\xf1\x82\023141600111233003142405A2252229333463100',
      b'\xf1\x875Q0959655C \xf1\x890361\xf1\x82\0211413001112120004110415121610169112',
      b'\xf1\x875Q0959655D \xf1\x890388\xf1\x82\0211413001113120006110417121A101A9113',
      b'\xf1\x875Q0959655J \xf1\x890830\xf1\x82\023271112111312--071104171825102591131211',
      b'\xf1\x875Q0959655J \xf1\x890830\xf1\x82\023271212111312--071104171838103891131211',
      b'\xf1\x875Q0959655J \xf1\x890830\xf1\x82\023341512112212--071104172328102891131211',
      b'\xf1\x875Q0959655J \xf1\x890830\xf1\x82\x13272512111312--07110417182C102C91131211',
      b'\xf1\x875Q0959655M \xf1\x890361\xf1\x82\0211413001112120041114115121611169112',
      b'\xf1\x875Q0959655S \xf1\x890870\xf1\x82\02315120011211200621143171717111791132111',
      b'\xf1\x875Q0959655S \xf1\x890870\xf1\x82\02324230011211200061104171724102491132111',
      b'\xf1\x875Q0959655S \xf1\x890870\xf1\x82\02324230011211200621143171724112491132111',
      b'\xf1\x875Q0959655S \xf1\x890870\xf1\x82\x1315120011211200061104171717101791132111',
      b'\xf1\x875Q0959655T \xf1\x890825\xf1\x82\023271200111312--071104171837103791132111',
      b'\xf1\x875Q0959655T \xf1\x890830\xf1\x82\x13271100111312--071104171826102691131211',
      b'\xf1\x875QD959655  \xf1\x890388\xf1\x82\x111413001113120006110417121D101D9112',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x873Q0909144F \xf1\x895043\xf1\x82\00561A01612A0',
      b'\xf1\x873Q0909144H \xf1\x895061\xf1\x82\00566A0J612A1',
      b'\xf1\x873Q0909144J \xf1\x895063\xf1\x82\00566A00514A1',
      b'\xf1\x873Q0909144J \xf1\x895063\xf1\x82\00566A0J712A1',
      b'\xf1\x873Q0909144K \xf1\x895072\xf1\x82\00571A0J714A1',
      b'\xf1\x873Q0909144L \xf1\x895081\xf1\x82\x0571A0JA15A1',
      b'\xf1\x873Q0909144M \xf1\x895082\xf1\x82\00571A01A18A1',
      b'\xf1\x873Q0909144M \xf1\x895082\xf1\x82\00571A0JA16A1',
      b'\xf1\x875Q0909143K \xf1\x892033\xf1\x820519A9040203',
      b'\xf1\x875Q0909144AA\xf1\x891081\xf1\x82\00521A00441A1',
      b'\xf1\x875Q0909144AA\xf1\x891081\xf1\x82\x0521A00641A1',
      b'\xf1\x875Q0909144AB\xf1\x891082\xf1\x82\00521A00442A1',
      b'\xf1\x875Q0909144AB\xf1\x891082\xf1\x82\00521A00642A1',
      b'\xf1\x875Q0909144AB\xf1\x891082\xf1\x82\00521A07B05A1',
      b'\xf1\x875Q0909144L \xf1\x891021\xf1\x82\00521A00602A0',
      b'\xf1\x875Q0909144L \xf1\x891021\xf1\x82\00522A00402A0',
      b'\xf1\x875Q0909144P \xf1\x891043\xf1\x82\00511A00403A0',
      b'\xf1\x875Q0909144R \xf1\x891061\xf1\x82\00516A00604A1',
      b'\xf1\x875Q0909144S \xf1\x891063\xf1\x82\00516A00604A1',
      b'\xf1\x875Q0909144S \xf1\x891063\xf1\x82\00516A07A02A1',
      b'\xf1\x875Q0909144T \xf1\x891072\xf1\x82\00521A00507A1',
      b'\xf1\x875Q0909144T \xf1\x891072\xf1\x82\00521A20B03A1',
      b'\xf1\x875QD909144B \xf1\x891072\xf1\x82\x0521A00507A1',
      b'\xf1\x875QM909144A \xf1\x891072\xf1\x82\x0521A20B03A1',
      b'\xf1\x875QM909144B \xf1\x891081\xf1\x82\00521A00442A1',
      b'\xf1\x875QN909144A \xf1\x895081\xf1\x82\00571A01A16A1',
      b'\xf1\x875QN909144A \xf1\x895081\xf1\x82\00571A01A18A1',
      b'\xf1\x875QN909144A \xf1\x895081\xf1\x82\x0571A01A17A1',
      b'\xf1\x875QN909144B \xf1\x895082\xf1\x82\00571A01A18A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x875Q0907567G \xf1\x890390\xf1\x82\00101',
      b'\xf1\x875Q0907572A \xf1\x890141\xf1\x82\00101',
      b'\xf1\x875Q0907572B \xf1\x890200\xf1\x82\00101',
      b'\xf1\x875Q0907572C \xf1\x890210\xf1\x82\00101',
      b'\xf1\x875Q0907572D \xf1\x890304\xf1\x82\00101',
      b'\xf1\x875Q0907572E \xf1\x89X310\xf1\x82\00101',
      b'\xf1\x875Q0907572F \xf1\x890400\xf1\x82\00101',
      b'\xf1\x875Q0907572G \xf1\x890571',
      b'\xf1\x875Q0907572H \xf1\x890620',
      b'\xf1\x875Q0907572J \xf1\x890654',
      b'\xf1\x875Q0907572P \xf1\x890682',
      b'\xf1\x875Q0907572R \xf1\x890771',
    ],
  },
  CAR.JETTA_MK7: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906024AK\xf1\x899937',
      b'\xf1\x8704E906024AS\xf1\x899912',
      b'\xf1\x8704E906024B \xf1\x895594',
      b'\xf1\x8704E906024L \xf1\x895595',
      b'\xf1\x8704E906027MS\xf1\x896223',
      b'\xf1\x875G0906259T \xf1\x890003',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x8709G927158BQ\xf1\x893545',
      b'\xf1\x8709S927158BS\xf1\x893642',
      b'\xf1\x8709S927158R \xf1\x893552',
      b'\xf1\x8709S927158R \xf1\x893587',
      b'\xf1\x870GC300020N \xf1\x892803',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655AG\xf1\x890336\xf1\x82\02314171231313500314611011630169333463100',
      b'\xf1\x875Q0959655BM\xf1\x890403\xf1\x82\02314171231313500314642011650169333463100',
      b'\xf1\x875Q0959655BM\xf1\x890403\xf1\x82\02314171231313500314643011650169333463100',
      b'\xf1\x875Q0959655BR\xf1\x890403\xf1\x82\02311170031313300314240011150119333433100',
      b'\xf1\x875Q0959655BR\xf1\x890403\xf1\x82\02319170031313300314240011550159333463100',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875QM909144B \xf1\x891081\xf1\x82\00521A10A01A1',
      b'\xf1\x875QM909144B \xf1\x891081\xf1\x82\x0521B00404A1',
      b'\xf1\x875QM909144C \xf1\x891082\xf1\x82\00521A00642A1',
      b'\xf1\x875QM909144C \xf1\x891082\xf1\x82\00521A10A01A1',
      b'\xf1\x875QN909144B \xf1\x895082\xf1\x82\00571A10A11A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x875Q0907572N \xf1\x890681',
      b'\xf1\x875Q0907572R \xf1\x890771',
    ],
  },
  CAR.PASSAT_MK8: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906023AH\xf1\x893379',
      b'\xf1\x8704L906026GA\xf1\x892013',
      b'\xf1\x873G0906264  \xf1\x890004',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300048R \xf1\x890610',
      b'\xf1\x870D9300014L \xf1\x895002',
      b'\xf1\x870DD300045T \xf1\x891601',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655AN\xf1\x890306\xf1\x82\r58160058140013036914110311',
      b'\xf1\x873Q0959655BB\xf1\x890195\xf1\x82\r56140056130012026612120211',
      b'\xf1\x875Q0959655S \xf1\x890870\xf1\x82\02315120011111200631145171716121691132111',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909143M \xf1\x892041\xf1\x820522B0080803',
      b'\xf1\x875Q0909144S \xf1\x891063\xf1\x82\00516B00501A1',
      b'\xf1\x875Q0909144T \xf1\x891072\xf1\x82\00521B00703A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x873Q0907572B \xf1\x890192',
      b'\xf1\x873Q0907572C \xf1\x890195',
      b'\xf1\x875Q0907572R \xf1\x890771',
    ],
  },
  CAR.TCROSS_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704C906025AK\xf1\x897053',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300050E \xf1\x891903',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x872Q0959655AJ\xf1\x890250\xf1\x82\02212130411110411--04041104141311152H14',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x872Q1909144M \xf1\x896041',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572T \xf1\x890383',
    ],
  },
  CAR.TIGUAN_MK2: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704L906026EJ\xf1\x893661',
      b'\xf1\x8704L906027G \xf1\x899893',
      b'\xf1\x875N0906259  \xf1\x890002',
      b'\xf1\x8783A907115B \xf1\x890005',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x8709G927158DT\xf1\x893698',
      b'\xf1\x870DL300011N \xf1\x892001',
      b'\xf1\x870DL300011N \xf1\x892012',
      b'\xf1\x870DL300013A \xf1\x893005',
      b'\xf1\x870DL300013G \xf1\x892120',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655AR\xf1\x890317\xf1\x82\02331310031333334313132573732379333313100',
      b'\xf1\x875Q0959655BM\xf1\x890403\xf1\x82\02316143231313500314641011750179333423100',
      b'\xf1\x875Q0959655BT\xf1\x890403\xf1\x82\02312110031333300314240583752379333423100',
      b'\xf1\x875Q0959655BT\xf1\x890403\xf1\x82\02331310031333336313140013950399333423100',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909143M \xf1\x892041\xf1\x820529A6060603',
      b'\xf1\x875QF909144B \xf1\x895582\xf1\x82\00571A60634A1',
      b'\xf1\x875QM909144B \xf1\x891081\xf1\x82\x0521A60604A1',
      b'\xf1\x875QM909144C \xf1\x891082\xf1\x82\00521A60804A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572J \xf1\x890156',
      b'\xf1\x872Q0907572Q \xf1\x890342',
      b'\xf1\x872Q0907572R \xf1\x890372',
    ],
  },
  CAR.TOURAN_MK2: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704L906026HM\xf1\x893017',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300041E \xf1\x891005',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655AS\xf1\x890318\xf1\x82\023363500213533353141324C4732479333313100',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909143P \xf1\x892051\xf1\x820531B0062105',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x873Q0907572C \xf1\x890195',
    ],
  },
  CAR.TRANSPORTER_T61: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704L906057N \xf1\x890413',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870BT300012G \xf1\x893102',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x872Q0959655AE\xf1\x890506\xf1\x82\02316170411110411--04041704161611152S1411',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x877LA909144F \xf1\x897150\xf1\x82\005323A5519A2',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572R \xf1\x890372',
    ],
  },
  CAR.AUDI_A3_MK3: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906023AN\xf1\x893695',
      b'\xf1\x8704E906023AR\xf1\x893440',
      b'\xf1\x8704E906023BL\xf1\x895190',
      b'\xf1\x8704E906027CJ\xf1\x897798',
      b'\xf1\x8704L997022N \xf1\x899459',
      b'\xf1\x875G0906259L \xf1\x890002',
      b'\xf1\x875G0906259Q \xf1\x890002',
      b'\xf1\x878V0906264B \xf1\x890003',
      b'\xf1\x878V0907115B \xf1\x890007',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300044T \xf1\x895245',
      b'\xf1\x870CW300048  \xf1\x895201',
      b'\xf1\x870D9300013B \xf1\x894931',
      b'\xf1\x870D9300041N \xf1\x894512',
      b'\xf1\x870DD300046A \xf1\x891602',
      b'\xf1\x870DD300046F \xf1\x891602',
      b'\xf1\x870DD300046G \xf1\x891601',
      b'\xf1\x870GC300013M \xf1\x892402',
      b'\xf1\x870GC300042J \xf1\x891402',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655AM\xf1\x890315\xf1\x82\x1311111111111111311411011231129321212100',
      b'\xf1\x875Q0959655J \xf1\x890825\xf1\x82\023111112111111--171115141112221291163221',
      b'\xf1\x875Q0959655J \xf1\x890830\xf1\x82\023121111111211--261117141112231291163221',
      b'\xf1\x875Q0959655J \xf1\x890830\xf1\x82\x13121111111111--341117141212231291163221',
      b'\xf1\x875Q0959655N \xf1\x890361\xf1\x82\0211212001112110004110411111421149114',
      b'\xf1\x875Q0959655N \xf1\x890361\xf1\x82\0211212001112111104110411111521159114',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909144AB\xf1\x891082\xf1\x82\00521G0G809A1',
      b'\xf1\x875Q0909144P \xf1\x891043\xf1\x82\00503G00303A0',
      b'\xf1\x875Q0909144P \xf1\x891043\xf1\x82\00503G00803A0',
      b'\xf1\x875Q0909144R \xf1\x891061\xf1\x82\00516G00804A1',
      b'\xf1\x875Q0909144T \xf1\x891072\xf1\x82\00521G00807A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x875Q0907572D \xf1\x890304\xf1\x82\00101',
      b'\xf1\x875Q0907572G \xf1\x890571',
      b'\xf1\x875Q0907572H \xf1\x890620',
      b'\xf1\x875Q0907572P \xf1\x890682',
    ],
  },
  CAR.AUDI_Q2_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906027JT\xf1\x894145',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300041F \xf1\x891006',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655BD\xf1\x890336\xf1\x82\x1311111111111100311211011231129321312111',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x873Q0909144K \xf1\x895072\xf1\x82\x0571F60511A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572M \xf1\x890233',
    ],
  },
  CAR.SEAT_ATECA_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906027KA\xf1\x893749',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870D9300014S \xf1\x895202',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655BH\xf1\x890703\xf1\x82\0161212001211001305121211052900',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x873Q0909144L \xf1\x895081\xf1\x82\00571N60511A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572M \xf1\x890233',
    ],
  },
  CAR.SEAT_LEON_MK3: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704L906021EL\xf1\x897542',
      b'\xf1\x8704L906026BP\xf1\x891198',
      b'\xf1\x8704L906026BP\xf1\x897608',
      b'\xf1\x8705E906018AS\xf1\x899596',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300050J \xf1\x891908',
      b'\xf1\x870D9300042M \xf1\x895016',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655AC\xf1\x890189\xf1\x82\r11110011110011021511110200',
      b'\xf1\x873Q0959655AS\xf1\x890200\xf1\x82\r12110012120012021612110200',
      b'\xf1\x873Q0959655CM\xf1\x890720\xf1\x82\0161312001313001305171311052900',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909144AB\xf1\x891082\xf1\x82\00521N01342A1',
      b'\xf1\x875Q0909144P \xf1\x891043\xf1\x82\00511N01805A0',
      b'\xf1\x875Q0909144T \xf1\x891072\xf1\x82\00521N05808A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x875Q0907572B \xf1\x890200\xf1\x82\00101',
      b'\xf1\x875Q0907572H \xf1\x890620',
      b'\xf1\x875Q0907572P \xf1\x890682',
    ],
  },
  CAR.SKODA_KAMIQ_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8705C906032M \xf1\x891333',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300020  \xf1\x891906',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x872Q0959655AM\xf1\x890351\xf1\x82\0222221042111042121040404042E2711152H14',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x872Q1909144M \xf1\x896041',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572T \xf1\x890383',
    ],
  },
  CAR.SKODA_KAROQ_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8705E906018P \xf1\x896020',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300041S \xf1\x891615',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655BH\xf1\x890712\xf1\x82\0161213001211001101131122012100',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0910143C \xf1\x892211\xf1\x82\00567T6100500',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572M \xf1\x890233',
    ],
  },
  CAR.SKODA_KODIAQ_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906027DD\xf1\x893123',
      b'\xf1\x8704L906026DE\xf1\x895418',
      b'\xf1\x875NA907115E \xf1\x890003',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870D9300043  \xf1\x895202',
      b'\xf1\x870DL300012M \xf1\x892107',
      b'\xf1\x870DL300012N \xf1\x892110',
      b'\xf1\x870DL300013G \xf1\x892119',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655BJ\xf1\x890703\xf1\x82\0161213001211001205212111052100',
      b'\xf1\x873Q0959655CN\xf1\x890720\xf1\x82\0161213001211001205212112052100',
      b'\xf1\x873Q0959655CQ\xf1\x890720\xf1\x82\x0e1213111211001205212112052111',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909143P \xf1\x892051\xf1\x820527T6050405',
      b'\xf1\x875Q0909143P \xf1\x892051\xf1\x820527T6060405',
      b'\xf1\x875Q0910143C \xf1\x892211\xf1\x82\x0567T600G600',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572Q \xf1\x890342',
      b'\xf1\x872Q0907572R \xf1\x890372',
    ],
  },
  CAR.SKODA_OCTAVIA_MK3: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704E906016ER\xf1\x895823',
      b'\xf1\x8704E906027HD\xf1\x893742',
      b'\xf1\x8704L906021DT\xf1\x898127',
      b'\xf1\x8704L906026BS\xf1\x891541',
      b'\xf1\x875G0906259C \xf1\x890002',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300041N \xf1\x891605',
      b'\xf1\x870CW300043B \xf1\x891601',
      b'\xf1\x870D9300041C \xf1\x894936',
      b'\xf1\x870D9300041J \xf1\x894902',
      b'\xf1\x870D9300041P \xf1\x894507',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x873Q0959655AC\xf1\x890200\xf1\x82\r11120011100010022212110200',
      b'\xf1\x873Q0959655AQ\xf1\x890200\xf1\x82\r11120011100010312212113100',
      b'\xf1\x873Q0959655AS\xf1\x890200\xf1\x82\r11120011100010022212110200',
      b'\xf1\x873Q0959655BH\xf1\x890703\xf1\x82\0163221003221002105755331052100',
      b'\xf1\x873Q0959655CN\xf1\x890720\xf1\x82\x0e3221003221002105755331052100',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x873Q0909144J \xf1\x895063\xf1\x82\00566A01513A1',
      b'\xf1\x875Q0909144AA\xf1\x891081\xf1\x82\00521T00403A1',
      b'\xf1\x875Q0909144AB\xf1\x891082\xf1\x82\x0521T00403A1',
      b'\xf1\x875Q0909144R \xf1\x891061\xf1\x82\x0516A00604A1',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x875Q0907572D \xf1\x890304\xf1\x82\x0101',
      b'\xf1\x875Q0907572F \xf1\x890400\xf1\x82\00101',
      b'\xf1\x875Q0907572J \xf1\x890654',
      b'\xf1\x875Q0907572P \xf1\x890682',
    ],
  },
  CAR.SKODA_SCALA_MK1: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704C906025AK\xf1\x897053',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300050  \xf1\x891709',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x872Q0959655AM\xf1\x890351\xf1\x82\022111104111104112104040404111111112H14',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x872Q1909144M \xf1\x896041',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x872Q0907572R \xf1\x890372',
    ],
  },
  CAR.SKODA_SUPERB_MK3: {
    (Ecu.engine, 0x7e0, None): [
      b'\xf1\x8704L906026FP\xf1\x891196',
      b'\xf1\x8704L906026KB\xf1\x894071',
      b'\xf1\x873G0906259B \xf1\x890002',
      b'\xf1\x873G0906264A \xf1\x890002',
    ],
    (Ecu.transmission, 0x7e1, None): [
      b'\xf1\x870CW300042H \xf1\x891601',
      b'\xf1\x870D9300011T \xf1\x894801',
      b'\xf1\x870D9300012  \xf1\x894940',
    ],
    (Ecu.srs, 0x715, None): [
      b'\xf1\x875Q0959655AE\xf1\x890130\xf1\x82\022111200111121001121118112231292221111',
      b'\xf1\x875Q0959655AK\xf1\x890130\xf1\x82\022111200111121001121110012211292221111',
      b'\xf1\x875Q0959655BH\xf1\x890336\xf1\x82\02331310031313100313131013141319331413100',
    ],
    (Ecu.eps, 0x712, None): [
      b'\xf1\x875Q0909143K \xf1\x892033\xf1\x820514UZ070203',
      b'\xf1\x875Q0909143M \xf1\x892041\xf1\x820522UZ070303',
      b'\xf1\x875Q0910143B \xf1\x892201\xf1\x82\00563UZ060700',
      b'\xf1\x875Q0910143B \xf1\x892201\xf1\x82\x0563UZ060600',
    ],
    (Ecu.fwdRadar, 0x757, None): [
      b'\xf1\x873Q0907572B \xf1\x890192',
      b'\xf1\x873Q0907572B \xf1\x890194',
      b'\xf1\x873Q0907572C \xf1\x890195',
    ],
  },
}
