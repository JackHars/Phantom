#!/bin/bash

# Check if we're on an aarch64 device
if [ "$(uname -m)" = "aarch64" ]; then
  # Check if libqmapboxgl.so exists in /system/lib64
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
    
    # Create a custom toolchain file for Android/GLES
    cat > android-toolchain.cmake << EOF
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Configure OpenGL ES instead of desktop OpenGL
set(USE_GLES2 ON CACHE BOOL "Use OpenGL ES 2.0" FORCE)
set(GLES2_FOUND ON CACHE BOOL "Found OpenGL ES 2.0" FORCE)
set(OPENGL_FOUND ON CACHE BOOL "Found OpenGL" FORCE)
set(OPENGL_LIBRARIES "-lGLESv2" CACHE STRING "OpenGL ES 2.0 library" FORCE)
set(OPENGL_INCLUDE_DIR "/system/include" CACHE STRING "OpenGL ES 2.0 include directory" FORCE)
set(EGL_LIBRARY "-lEGL" CACHE STRING "EGL library" FORCE)

# Disable git repository checks
set(WITH_GIT_INFO OFF CACHE BOOL "Build with git information" FORCE)

# Specify Qt platform
set(QT_VERSION_MAJOR 5 CACHE STRING "Qt major version" FORCE)
EOF
    
    # Build mapbox
    mkdir -p build && cd build
    export CXXFLAGS="-I/system/include"
    export LDFLAGS="-L/system/lib64 -L/system/lib"
    cmake -DCMAKE_TOOLCHAIN_FILE=../android-toolchain.cmake -DMBGL_PLATFORM=linux -DMBGL_RENDERER=gles2 -DWITH_QT=ON -DWITH_NODE_BINDINGS=OFF ..
    
    # If cmake fails, try with a simplified target
    if [ $? -ne 0 ]; then
      echo "First CMake configuration failed, trying simplified build..."
      cat > ../minimal-build.cmake << EOF
cmake_minimum_required(VERSION 3.0)
project(mbgl-qt)
set(CMAKE_CXX_STANDARD 14)

add_library(qmapboxgl SHARED
    platform/qt/src/qmapboxgl.cpp
    platform/qt/src/qmapboxgl_p.cpp
    platform/qt/src/qmapboxgl_renderer_backend.cpp
    platform/default/mbgl/util/default_styles.cpp
    platform/default/mbgl/util/default_styles.hpp
)

target_include_directories(qmapboxgl PRIVATE
    platform/qt/include
    platform/default
    include
    src
    /system/comma/usr/include
    /system/comma/usr/include/qt
    /system/comma/usr/include/qt/QtCore
    /system/comma/usr/include/qt/QtGui
    /system/comma/usr/include/qt/QtNetwork
)

target_link_libraries(qmapboxgl
    -lQt5Core
    -lQt5Gui
    -lQt5Network
    -lGLESv2
    -lEGL
)
EOF
      cmake -P ../minimal-build.cmake
      g++ -shared -o libqmapboxgl.so -I../platform/qt/include -I../platform/default -I../include -I../src -I/system/comma/usr/include -I/system/comma/usr/include/qt -I/system/comma/usr/include/qt/QtCore -I/system/comma/usr/include/qt/QtGui -I/system/comma/usr/include/qt/QtNetwork ../platform/qt/src/qmapboxgl.cpp ../platform/qt/src/qmapboxgl_p.cpp ../platform/qt/src/qmapboxgl_renderer_backend.cpp ../platform/default/mbgl/util/default_styles.cpp -lQt5Core -lQt5Gui -lQt5Network -lGLESv2 -lEGL
    else
      # Build the qt mapbox library
      make -j$(nproc) mbgl-qt || make -j$(nproc) qmapboxgl
    fi
    
    # Find and copy the built library
    BUILT_LIB=$(find . -name "libqmapboxgl.so")
    if [ -z "$BUILT_LIB" ]; then
      # If we still can't find it, check if our direct build worked
      if [ -f "libqmapboxgl.so" ]; then
        BUILT_LIB="libqmapboxgl.so"
      else
        echo "Error: Could not find built libqmapboxgl.so"
        exit 1
      fi
    fi
    
    cp $BUILT_LIB /system/lib64/libqmapboxgl.so
    chmod 644 /system/lib64/libqmapboxgl.so
    echo "Successfully built and installed libqmapboxgl.so"
    
    # Clean up
    cd /tmp
    rm -rf $BUILD_DIR
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
    sed -i 's|_qt5gui_find_extra_libs(EGL "/home/builder/.termux-build/_cache/android5-r20-api-21-v2/sysroot/usr/lib/aarch64-linux-android/21/libEGL.so" "" "")|_qt5gui_find_extra_libs(EGL "/system/lib/libEGL.so" "" "")|g' "$QT_CONFIG_FILE"
    sed -i 's|_qt5gui_find_extra_libs(OPENGL "/home/builder/.termux-build/_cache/android5-r20-api-21-v2/sysroot/usr/lib/aarch64-linux-android/21/libGLESv2.so" "" "")|_qt5gui_find_extra_libs(OPENGL "/system/lib/libGLESv2.so" "" "")|g' "$QT_CONFIG_FILE"
    
    echo "Updated Qt5Gui configuration file"
  else
    echo "Warning: Qt5Gui configuration file not found at $QT_CONFIG_FILE"
  fi
fi

# Return to original directory
cd $(dirname $0)

# Continue with the build
scons "$@"
