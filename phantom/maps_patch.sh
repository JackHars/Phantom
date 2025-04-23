#!/bin/bash

if [ "$(uname -m)" = "aarch64" ]; then
  # Ensure aarch64 is included in the maps architecture list in SConscript
  SCONSCRIPT_FILE="$(dirname $0)/selfdrive/ui/SConscript"
  if [ -f "$SCONSCRIPT_FILE" ]; then
    echo "Checking SConscript file for aarch64 support..."
    if ! grep -q "maps = arch in \['larch64', 'x86_64', 'aarch64'\]" "$SCONSCRIPT_FILE"; then
      echo "Adding aarch64 to maps architecture list in SConscript"
      sed -i 's/maps = arch in \[\(.*\)\]/maps = arch in \[\1, '\''aarch64'\''\]/g' "$SCONSCRIPT_FILE"
    fi
  fi
  
  # First, check and modify Qt5Gui configuration file
  QT_CONFIG_FILE="/system/comma/usr/lib/cmake/Qt5Gui/Qt5GuiConfigExtras.cmake"
  if [ -f "$QT_CONFIG_FILE" ]; then
    echo "Checking Qt5Gui configuration file..."
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Create backup if it doesn't exist
    if [ ! -f "${QT_CONFIG_FILE}.bak" ]; then
      cp "$QT_CONFIG_FILE" "${QT_CONFIG_FILE}.bak"
    fi
    
    # Update EGL and OPENGL paths - fixing capitalization issues
    sed -i 's|_qt5gui_find_extra_libs(EGL ".*libEGL.so" "" "")|_qt5gui_find_extra_libs(EGL "/system/lib/libEGL.so" "" "")|g' "$QT_CONFIG_FILE"
    sed -i 's|_qt5gui_find_extra_libs(OPENGL ".*libGLESv2.so" "" "")|_qt5gui_find_extra_libs(OPENGL "/system/lib/libGLESv2.so" "" "")|g' "$QT_CONFIG_FILE"
    
    echo "Updated Qt5Gui configuration file"
  fi
  
  # Check if libqmapboxgl is already installed in /system/lib64
  MAPBOX_LIB="/system/lib64/libqmapboxgl.so"
  PHONELIBS_MAPBOX_LIB="$(dirname $0)/phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so"
  
  # Mount system as writable if needed
  mount -o rw,remount /system 2>/dev/null || true
  
  if [ -f "$MAPBOX_LIB" ]; then
    echo "libqmapboxgl.so already exists in /system/lib64"
  elif [ -f "$PHONELIBS_MAPBOX_LIB" ]; then
    # Copy from phonelibs
    echo "Copying libqmapboxgl.so from phonelibs to /system/lib64"
    cp "$PHONELIBS_MAPBOX_LIB" "$MAPBOX_LIB"
    chmod 644 "$MAPBOX_LIB"
    echo "Successfully installed libqmapboxgl.so from phonelibs"
  else
    # Build mapbox
    echo "Building mapbox-gl-native..."
    cd "$(dirname $0)/third_party/mapbox-gl-native"
    mkdir -p build && cd build
    cmake -DMBGL_WITH_QT=ON ..
    make -j$(nproc) mbgl-qt
    
    # Create the phonelibs directory if it doesn't exist
    PHONELIBS_DIR="$(dirname $0)/phonelibs/mapbox-gl-native-qt/aarch64"
    mkdir -p "$PHONELIBS_DIR"
    
    # Copy library to phonelibs and system
    if [ -f "./qt/libqmapboxgl.so" ]; then
      # Copy to phonelibs
      cp "./qt/libqmapboxgl.so" "$PHONELIBS_MAPBOX_LIB"
      
      # Copy to system
      cp "./qt/libqmapboxgl.so" "$MAPBOX_LIB"
      chmod 644 "$MAPBOX_LIB"
      echo "Successfully built and installed libqmapboxgl.so"
    else
      echo "Failed to build libqmapboxgl.so"
    fi
  fi
fi

# Return to original directory
cd "$(dirname $0)"

# Continue with the build
scons "$@"
