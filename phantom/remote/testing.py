#!/usr/bin/env python                                                                                                                                                                                              
from time import sleep                                                                                                                                                                                             
import zmq                                                                                                                                                                                                         
import selfdrive.messaging as messaging                                                                                                                                                                            
from selfdrive.services import service_list               
from common.params import Params                                                                                                                                                         
from selfdrive.boardd.boardd import can_list_to_can_capnp                                                                                                                                                          
from selfdrive.car.toyota.toyotacan import make_can_msg                                                                                                                                                            
from cereal import car                                                                                                                                                                                             

sendcan = messaging.pub_sock(service_list['sendcan'].port)
carparams = messaging.pub_sock(service_list['carParams'].port)      
#carstate = messaging.pub_sock(service_list['carState'].port)
                                                                                                                                                                                            
                                                                                                                                                                                                                   
#set panda safety                                                                                                                                                                                                  
def set_panda_safety():   
  ret = car.CarParams.new_message()                   
  ret.safetyModel = car.CarParams.SafetyModel.gm 
  return ret                                                                                                                                                                                                       

  params = Params()
  CP = set_panda_safety()

  # Write CarParams for radard and boardd safety mode
  params.put("CarParams", CP.to_bytes())
  cp_send = messaging.new_message()
  cp_send.init('carParams')
  cp_send.carParams = CP
  carparams.send(cp_send.to_bytes())                                                                                                                                                                                                                                                                                                                                                           

  cs_send = messaging.new_message()
  cs_send.init('carState')
  cs_send.carState = copy(CS)
  carstate.send(cs_send.to_bytes())

  poller = zmq.Poller()


command = []                                                                                                                                                                                                       

def lock_door(sendcan):
  print("locking door")
  command.append(make_can_msg(0x750, '\x40\x05\x30\x11\x00\x80\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  del command[:]


def unlock_door(sendcan):
  print("unlocking door")
  command.append(make_can_msg(0x750, '\x40\x05\x30\x11\x00\x40\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  del command[:]

def buzzer(sendcan, t):
  print("turning on buzzer for %s seconds" % t)
  command.append(make_can_msg(0x750, '\x40\x04\x30\x14\x00\x80\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  del command[:]
  sleep(t)
  command.append(make_can_msg(0x750, '\x40\x04\x30\x14\x00\x20\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  del command[:]

def headlights(sendcan, t):
  print("turning on headlights for %s seconds" % t)
  command.append(make_can_msg(0x750, '\x40\x05\x30\x15\x00\x40\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  sleep(t)
  del command[:]
  command.append(make_can_msg(0x750, '\x40\x05\x30\x15\x00\x00\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  del command[:]

def horn(sendcan, t):
  print("honking horn for %s seconds" % t)
  command.append(make_can_msg(0x750, '\x40\x04\x30\x06\x00\x20\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  sleep(t)
  del command[:]
  command.append(make_can_msg(0x750, '\x40\x04\x30\x06\x00\x00\x00\x00', 0, False))
  sendcan.send(can_list_to_can_capnp(command, msgtype='sendcan'))
  del command[:]

set_panda_safety()
panda_safety_thread()
headlights(sendcan, 1)
