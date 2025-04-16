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
    
    # Create a custom patch to bypass platform/linux/linux.cmake
    echo "Creating temporary patch to bypass Linux platform checks..."
    cat > linux_bypass.patch << 'EOF'
--- CMakeLists.txt	2023-04-16 06:00:00.000000000 +0000
+++ CMakeLists.txt.new	2023-04-16 06:00:00.000000000 +0000
@@ -1037,7 +1037,7 @@
     message("-- Configuring GL-Native with Direct3D renderer backend")
     include(platform/windows/windows.cmake)
 elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
-    include(platform/linux/linux.cmake)
+    # Skip Linux platform checks
 elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
     include(platform/macos/macos.cmake)
 elseif(MBGL_PLATFORM STREQUAL "ios")
EOF
    
    # Apply the patch
    patch -p0 < linux_bypass.patch
    
    # Simple build process
    echo "Building mapbox-gl-native..."
    mkdir -p build && cd build
    
    # Configure cmake with Android-specific settings and export compile commands
    echo "Configuring cmake with OpenGLES..."
    cmake -DMBGL_WITH_OPENGL=OFF \
          -DMBGL_WITH_OPENGLES=ON \
          -DMBGL_PLATFORM=android \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
          ..
    
    # Build the Qt mapbox lib
    make -j$(nproc) mbgl-qt || {
        echo "First build attempt failed, trying alternative build approach..."
        cd ..
        cd platform/qt
        qmake
        make -j$(nproc)
    }
    
    # Try to find the library in multiple possible locations
    echo "Searching for built library..."
    BUILT_QT_LIB=""
    for POSSIBLE_PATH in "./platform/qt/libqmapboxgl.so" "../platform/qt/libqmapboxgl.so" "../platform/qt/release/libqmapboxgl.so"; do
        if [ -f "$POSSIBLE_PATH" ]; then
            BUILT_QT_LIB="$POSSIBLE_PATH"
            break
        fi
    done
    
    if [ -n "$BUILT_QT_LIB" ]; then
        echo "Found library at: $BUILT_QT_LIB"
        # Copy to system
        cp "$BUILT_QT_LIB" /system/lib64/libqmapboxgl.so
        chmod 644 /system/lib64/libqmapboxgl.so
        
        # Also copy to our phonelibs directory for future use
        cp "$BUILT_QT_LIB" "$(dirname $0)/phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so"
        
        echo "Successfully built and installed libqmapboxgl.so"
        echo "The library has also been saved to the repository for future use"
    else
        echo "Error: Could not find built libqmapboxgl.so"
        echo "Current directory structure:"
        find .. -name "*.so" | grep -i map
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
