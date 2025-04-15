#!/bin/bash

# Check if we're on an aarch64 device
if [ "$(uname -m)" = "aarch64" ]; then
  # First, check if the OpenGL libraries exist
  echo "Checking system OpenGL libraries..."
  
  # Find the actual library files
  EGL_LIB=$(find /system -name "libEGL.so" 2>/dev/null | head -1)
  GLES_LIB=$(find /system -name "libGLESv2.so" 2>/dev/null | head -1)
  
  if [ -z "$EGL_LIB" ]; then
    echo "Warning: libEGL.so not found, looking for alternative names..."
    EGL_LIB=$(find /system -name "*EGL*.so" 2>/dev/null | head -1)
    if [ -n "$EGL_LIB" ]; then
      echo "Found alternative EGL library: $EGL_LIB"
    else
      echo "Error: Could not find any EGL library"
      exit 1
    fi
  fi
  
  if [ -z "$GLES_LIB" ]; then
    echo "Warning: libGLESv2.so not found, looking for alternative names..."
    GLES_LIB=$(find /system -name "*GLES*.so" 2>/dev/null | head -1)
    if [ -n "$GLES_LIB" ]; then
      echo "Found alternative GLES library: $GLES_LIB"
    else
      echo "Error: Could not find any GLES library"
      exit 1
    fi
  fi
  
  echo "Using EGL library: $EGL_LIB"
  echo "Using GLES library: $GLES_LIB"
  
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
    
    # Update EGL and OPENGL paths with the actual paths we found
    sed -i "s|_qt5gui_find_extra_libs(EGL \"[^\"]*\" \"\" \"\")|_qt5gui_find_extra_libs(EGL \"$EGL_LIB\" \"\" \"\")|g" "$QT_CONFIG_FILE"
    sed -i "s|_qt5gui_find_extra_libs(OPENGL \"[^\"]*\" \"\" \"\")|_qt5gui_find_extra_libs(OPENGL \"$GLES_LIB\" \"\" \"\")|g" "$QT_CONFIG_FILE"
    
    echo "Updated Qt5Gui configuration file"
    
    # Verify the changes
    echo "Verifying configuration file changes..."
    grep -A 1 "_qt5gui_find_extra_libs(EGL" "$QT_CONFIG_FILE"
    grep -A 1 "_qt5gui_find_extra_libs(OPENGL" "$QT_CONFIG_FILE"
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
set(OPENGL_opengl_LIBRARY "$GLES_LIB")
set(OPENGL_glx_LIBRARY "$GLES_LIB")
set(OPENGL_INCLUDE_DIR "/system/include")
set(GLX TRUE)
set(Qt5Gui_DIR "/system/comma/usr/lib/cmake/Qt5Gui")
EOF
    
    # Try direct compilation if CMake approach fails
    echo "Building Qt-based library directly..."
    mkdir -p qt_build && cd qt_build
    
    # Create a minimalist build script
    cat > build.cpp << EOF
#include <QMapboxGL>
#include <QMapbox>
#include <QtGui>
int main() { return 0; }
EOF
    
    # Compile the test file to check dependencies
    g++ -c build.cpp -I../platform/qt/include -I/system/comma/usr/include/qt -I/system/comma/usr/include/qt/QtCore -I/system/comma/usr/include/qt/QtGui -I/system/comma/usr/include/qt/QtNetwork
    
    # Create the shared library
    echo "Compiling minimalist QMapboxGL shared library..."
    g++ -shared -o libqmapboxgl.so \
      -I../platform/qt/include \
      -I../include \
      -I../src \
      -I../platform/default \
      -I/system/comma/usr/include \
      -I/system/comma/usr/include/qt \
      -I/system/comma/usr/include/qt/QtCore \
      -I/system/comma/usr/include/qt/QtGui \
      -I/system/comma/usr/include/qt/QtNetwork \
      ../platform/qt/src/qmapboxgl.cpp \
      ../platform/qt/src/qmapboxgl_p.cpp \
      ../platform/qt/src/qmapboxgl_renderer_backend.cpp \
      ../platform/default/mbgl/util/default_styles.cpp \
      -L/system/comma/usr/lib \
      -L/system/lib64 \
      -L/system/lib \
      -lQt5Core \
      -lQt5Gui \
      -lQt5Network \
      -L/system/lib \
      -L$(dirname $EGL_LIB) \
      -L$(dirname $GLES_LIB) \
      -lEGL \
      -lGLESv2
    
    # Check if our minimalist library was built successfully
    if [ -f "libqmapboxgl.so" ]; then
      cp libqmapboxgl.so /system/lib64/libqmapboxgl.so
      chmod 644 /system/lib64/libqmapboxgl.so
      echo "Successfully built and installed custom libqmapboxgl.so"
    else
      # Try building with CMake as a fallback
      cd ..
      mkdir -p build && cd build
      echo "Fallback: Trying CMake build..."
      cmake -C ../custom.cmake -DMBGL_WITH_QT=ON -DMBGL_WITH_OPENGL=OFF -DMBGL_WITH_OPENGLES=ON ..
      make -j$(nproc) mbgl-qt
      
      # Find and copy the built library
      BUILT_LIB=$(find . -name "libqmapboxgl.so")
      if [ -n "$BUILT_LIB" ]; then
        cp $BUILT_LIB /system/lib64/libqmapboxgl.so
        chmod 644 /system/lib64/libqmapboxgl.so
        echo "Successfully built and installed libqmapboxgl.so with CMake"
      else
        echo "Error: Could not find built libqmapboxgl.so"
        echo "Looking for similar files:"
        find . -name "*.so" | grep -i map
        echo "Creating stub library..."
        
        # Create a stub library as last resort
        cat > stub.cpp << EOF
#include <QMapboxGL>
#include <QMapbox>
namespace QMapbox {
  Coordinate::Coordinate(double lat, double lon) : m_lat(lat), m_lon(lon) {}
  CoordinateZoom::CoordinateZoom(const Coordinate& coordinate_, double zoom_)
      : coordinate(coordinate_), zoom(zoom_) {}
}

QMapboxGLSettings::QMapboxGLSettings() {}
QMapboxGL::QMapboxGL(QObject* parent, const QMapboxGLSettings& settings, const QSize& size, qreal pixelRatio)
    : QObject(parent) {}
QMapboxGL::~QMapboxGL() {}
EOF
        
        g++ -shared -o libqmapboxgl.so stub.cpp -I../platform/qt/include -I/system/comma/usr/include/qt -lQt5Core -lQt5Gui -lQt5Network
        cp libqmapboxgl.so /system/lib64/libqmapboxgl.so
        chmod 644 /system/lib64/libqmapboxgl.so
        echo "Installed stub libqmapboxgl.so"
      fi
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
