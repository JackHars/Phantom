#!/bin/bash
# Check if we're on an aarch64 device
if [ "$(uname -m)" = "aarch64" ]; then
  # Check if libqmapboxgl.so exists in /system/lib64
  if [ ! -f "/system/lib64/libqmapboxgl.so" ]; then
    echo "Copying libqmapboxgl.so to /system/lib64/"
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Copy the library
    cp phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so /system/lib64/
    
    # Set proper permissions
    chmod 644 /system/lib64/libqmapboxgl.so
    
    echo "Library copied successfully"
  else
    echo "libqmapboxgl.so already exists in /system/lib64/"
  fi
fi

# Continue with the build
scons "$@"
