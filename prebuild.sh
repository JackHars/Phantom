#!/bin/bash

# Check if we're on an aarch64 device
if [ "$(uname -m)" = "aarch64" ]; then
  # First, check and modify Qt5Gui configuration file
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
    sed -i 's|_qt5gui_find_extra_libs(EGL "/home/builder/.termux-build/_cache/android5-r20-api-21-v2/sysroot/usr/lib/aarch64-linux-android/21/libEGL.so" "" "")|_qt5gui_find_extra_libs(EGL "/system/lib/libEGL.so" "" "")|g' "$QT_CONFIG_FILE"
    sed -i 's|_qt5gui_find_extra_libs(OPENGL "/home/builder/.termux-build/_cache/android5-r20-api-21-v2/sysroot/usr/lib/aarch64-linux-android/21/libGLESv2.so" "" "")|_qt5gui_find_extra_libs(OPENGL "/system/lib/libGLESv2.so" "" "")|g' "$QT_CONFIG_FILE"
    
    echo "Updated Qt5Gui configuration file"
  else
    echo "Warning: Qt5Gui configuration file not found at $QT_CONFIG_FILE"
  fi
  
  # Now check if libqmapboxgl.so exists in /system/lib64
  if [ ! -f "/system/lib64/libqmapboxgl.so" ]; then
    echo "Building and installing libqmapboxgl.so..."
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Set up build environment
    BUILD_DIR=$(mktemp -d)
    echo "Building in temporary directory: $BUILD_DIR"
    
    # Copy the local mapbox-gl-native to the build directory
    cp -r "$(dirname $0)/third_party/mapbox-gl-native" "$BUILD_DIR/"
    cd "$BUILD_DIR/mapbox-gl-native"
    
    # Create custom CMake settings to handle OpenGL
    echo "Creating custom CMake configuration..."
    cat > custom.cmake << EOF
set(OPENGL_opengl_LIBRARY /system/lib/libGLESv2.so)
set(OPENGL_glx_LIBRARY /system/lib/libGLESv2.so)
set(OPENGL_INCLUDE_DIR /system/include)
set(GLX TRUE)
set(Qt5Gui_DIR /system/comma/usr/lib/cmake/Qt5Gui)
EOF
    
    # Build mapbox with custom configuration
    mkdir -p build && cd build
    cmake -C ../custom.cmake -DMBGL_WITH_QT=ON -DMBGL_WITH_OPENGL=OFF -DMBGL_WITH_OPENGLES=ON ..
    make -j$(nproc) mbgl-qt
    
    # Find and copy the built library
    BUILT_LIB=$(find . -name "libqmapboxgl.so")
    if [ -n "$BUILT_LIB" ]; then
      cp $BUILT_LIB /system/lib64/libqmapboxgl.so
      chmod 644 /system/lib64/libqmapboxgl.so
      echo "Successfully built and installed libqmapboxgl.so"
    else
      echo "Error: Could not find built libqmapboxgl.so"
      echo "Looking for similar files:"
      find . -name "*.so" | grep -i map
      exit 1
    fi
    
    # Clean up
    cd /tmp
    rm -rf $BUILD_DIR
  else
    echo "libqmapboxgl.so already exists in /system/lib64/"
  fi
fi

# Return to original directory
cd $(dirname $0)

# Continue with the build
scons "$@"
