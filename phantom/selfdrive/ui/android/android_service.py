#!/usr/bin/env python3
import os
import time
import subprocess
from selfdrive.ui.android.screenrecord import ScreenRecorder
from common.params import Params
import threading
import signal

def run_android_services():
  """Main entry point to run all Android-specific services"""
  
  # Initialize the recorder
  recorder = ScreenRecorder()
  params = Params()
  
  record_last = False
  running = True
  
  # Set up signal handler for graceful shutdown
  def handle_signal(sig, frame):
    nonlocal running
    running = False
    if recorder.recording:
      recorder.stop_recording()
  
  signal.signal(signal.SIGINT, handle_signal)
  signal.signal(signal.SIGTERM, handle_signal)
  
  # Main monitoring loop
  while running:
    record_enabled = params.get_bool("RecordDrive")
    
    # Start recording if just enabled
    if record_enabled and not record_last:
      recorder.start_recording()
    
    # Stop recording if just disabled
    elif not record_enabled and record_last:
      recorder.stop_recording()
    
    record_last = record_enabled
    time.sleep(1)

if __name__ == "__main__":
  run_android_services() 