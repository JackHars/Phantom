#!/usr/bin/env python3
import os
import subprocess
import time
import datetime
from pathlib import Path
from threading import Thread
import numpy as np
from common.params import Params

RECORDINGS_DIR = "/data/media/0/recordings"

def ensure_recordings_dir():
  """Make sure the recordings directory exists."""
  os.makedirs(RECORDINGS_DIR, exist_ok=True)

class ScreenRecorder:
  def __init__(self):
    self.recording = False
    self.recording_thread = None
    self.params = Params()
    
    # Make sure recordings directory exists
    ensure_recordings_dir()
    
  def start_recording(self):
    """Start screen recording using Android's screenrecord command."""
    if self.recording:
      return
      
    self.recording = True
    self.recording_thread = Thread(target=self._record_screen)
    self.recording_thread.daemon = True
    self.recording_thread.start()
    
  def stop_recording(self):
    """Stop the current recording by killing the screenrecord process."""
    if not self.recording:
      return
      
    self.recording = False
    try:
      subprocess.call(["pkill", "-f", "screenrecord"])
    except Exception as e:
      print(f"Error stopping recording: {e}")
    
    if self.recording_thread:
      self.recording_thread.join(timeout=2)
      self.recording_thread = None
  
  def _record_screen(self):
    """Recording thread that starts the Android screenrecord process."""
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
    filepath = os.path.join(RECORDINGS_DIR, f"drive_{timestamp}.mp4")
    
    try:
      # Run Android's screenrecord command
      cmd = ["screenrecord", "--verbose", "--bit-rate", "4000000", filepath]
      process = subprocess.Popen(cmd)
      
      # Monitor the recording process
      while self.recording and process.poll() is None:
        time.sleep(1)
        
      # Clean up if needed
      if process.poll() is None:
        process.terminate()
        process.wait(timeout=2)
    except Exception as e:
      print(f"Error during recording: {e}")
    finally:
      self.recording = False

def main():
  """Main function to run when script is executed directly."""
  recorder = ScreenRecorder()
  params = Params()
  
  record_last = False
  
  # Main monitoring loop
  while True:
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
  main() 