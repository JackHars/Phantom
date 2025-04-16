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
  
  # Check for precompiled library in our repo
  REPO_LIB="$(dirname $0)/phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so"
  
  # If we already have the library in our repo, just copy it to the system
  if [ -f "$REPO_LIB" ]; then
    echo "Using prebuilt libqmapboxgl.so from repository..."
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Copy to system
    cp "$REPO_LIB" /system/lib64/libqmapboxgl.so
    chmod 644 /system/lib64/libqmapboxgl.so
    echo "Installed libqmapboxgl.so from repository"
  # If not in our repo and not in the system, build it
  elif [ ! -f "/system/lib64/libqmapboxgl.so" ]; then
    echo "Building and installing libqmapboxgl.so..."
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Ensure the phonelibs directory exists
    mkdir -p "$(dirname $0)/phonelibs/mapbox-gl-native-qt/aarch64"
    
    # Build directly in the third_party directory
    cd "$(dirname $0)/third_party/mapbox-gl-native"
    
    # Use direct paths for libraries
    EGL_LIB="/system/lib/libEGL.so"
    GLES_LIB="/system/lib/libGLESv2.so"
    
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
    
    # Use direct path to the built library
    BUILT_QT_LIB="./platform/qt/libqmapboxgl.so"
    
    if [ -f "$BUILT_QT_LIB" ]; then
      # Copy to system
      cp "$BUILT_QT_LIB" /system/lib64/libqmapboxgl.so
      chmod 644 /system/lib64/libqmapboxgl.so
      
      # Also copy to our phonelibs directory for future use
      cp "$BUILT_QT_LIB" "$(dirname $0)/phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so"
      
      echo "Successfully built and installed libqmapboxgl.so"
      echo "The library has also been saved to the repository for future use"
    else
      echo "Error: Could not find built libqmapboxgl.so at expected location: $BUILT_QT_LIB"
      echo "Current directory contents:"
      ls -R ./platform/
      exit 1
    fi
    
    # Return to original directory
    cd "$(dirname $0)"
  else
    echo "libqmapboxgl.so already exists in /system/lib64/"
  fi
fi

# Return to original directory
cd $(dirname $0)

# Continue with the build
scons "$@"
