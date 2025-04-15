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
  
  # Check and modify Qt5Gui configuration file
  QT_CONFIG_FILE="/system/comma/usr/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake"
  if [ -f "$QT_CONFIG_FILE" ]; then
    echo "Checking Qt5Gui configuration file..."
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Create backup if it doesn't exist
    if [ ! -f "${QT_CONFIG_FILE}.bak" ]; then
      cp "$QT_CONFIG_FILE" "${QT_CONFIG_FILE}.bak"
      echo "Created backup at ${QT_CONFIG_FILE}.bak"
    fi
    
    # Update EGL and OPENGL paths
    sed -i 's|_qt5gui_find_extra_libs(EGL "/home/builder/.termux-build/_cache/android5-r20-api-21-v2/sysroot/usr/lib/aarch64-linux-android/21/libEGL.so" "" "")|_qt5gui_find_extra_libs(EGL "/system/lib/LibEGL.so" "" "")|g' "$QT_CONFIG_FILE"
    sed -i 's|_qt5gui_find_extra_libs(OPENGL "/home/builder/.termux-build/_cache/android5-r20-api-21-v2/sysroot/usr/lib/aarch64-linux-android/21/libGLESv2.so" "" "")|_qt5gui_find_extra_libs(OPENGL "/system/lib/ibGLESv2.so" "" "")|g' "$QT_CONFIG_FILE"
    
    echo "Updated Qt5Gui configuration file"
  else
    echo "Warning: Qt5Gui configuration file not found at $QT_CONFIG_FILE"
  fi
fi

# Continue with the build
scons "$@"
