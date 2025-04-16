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
    
    # Update EGL and OPENGL paths - fixing capitalization issues
    sed -i 's|_qt5gui_find_extra_libs(EGL ".*libEGL.so" "" "")|_qt5gui_find_extra_libs(EGL "/system/lib/libEGL.so" "" "")|g' "$QT_CONFIG_FILE"
    sed -i 's|_qt5gui_find_extra_libs(OPENGL ".*libGLESv2.so" "" "")|_qt5gui_find_extra_libs(OPENGL "/system/lib/libGLESv2.so" "" "")|g' "$QT_CONFIG_FILE"
    
    # Double-check the actual paths
    echo "Verifying actual library paths:"
    ls -la /system/lib/libEGL.so /system/lib/libGLESv2.so 2>/dev/null || true
    
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
    
    # Find the actual library paths
    EGL_LIB=$(find /system -name "libEGL.so" | head -1)
    GLES_LIB=$(find /system -name "libGLESv2.so" | head -1)
    
    if [ -z "$EGL_LIB" ]; then
      echo "Warning: Could not find libEGL.so, using default path"
      EGL_LIB="/system/lib/libEGL.so"
    fi
    
    if [ -z "$GLES_LIB" ]; then
      echo "Warning: Could not find libGLESv2.so, using default path"
      GLES_LIB="/system/lib/libGLESv2.so"
    fi
    
    echo "Using EGL library: $EGL_LIB"
    echo "Using GLES library: $GLES_LIB"
    
    # Create custom CMake settings to handle OpenGL
    echo "Creating custom CMake configuration..."
    cat > custom.cmake << EOF
set(OPENGL_opengl_LIBRARY "${GLES_LIB}")
set(OPENGL_glx_LIBRARY "${GLES_LIB}")
set(OPENGL_INCLUDE_DIR /system/include)
set(GLX TRUE)
set(Qt5Gui_DIR /system/comma/usr/lib/cmake/Qt5Gui)
EOF
    
    # Build mapbox with custom configuration
    mkdir -p build && cd build
    cmake -C ../custom.cmake -DMBGL_WITH_QT=ON -DMBGL_WITH_OPENGL=OFF -DMBGL_WITH_OPENGLES=ON ..
    make -j$(nproc) mbgl-qt
    
    # The built library should be in a specific location based on the build system
    # Qt build typically puts the library in a specific location
    EXPECTED_QT_LIB="./platform/qt/libqmapboxgl.so"
    
    if [ -f "$EXPECTED_QT_LIB" ]; then
      cp "$EXPECTED_QT_LIB" /system/lib64/libqmapboxgl.so
      chmod 644 /system/lib64/libqmapboxgl.so
      echo "Successfully built and installed libqmapboxgl.so"
    else
      echo "Error: Could not find built libqmapboxgl.so at expected location: $EXPECTED_QT_LIB"
      echo "Current directory contents:"
      ls -R ./platform/
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
