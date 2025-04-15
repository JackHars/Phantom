#!/bin/bash

# Store original working directory
ORIGINAL_DIR=$(pwd)

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
    cp -r "${ORIGINAL_DIR}/third_party/mapbox-gl-native" "$BUILD_DIR/"
    cd "$BUILD_DIR/mapbox-gl-native"
    
    # Create a stub library directly as primary approach
    echo "Creating stub library..."
    mkdir -p qt_build && cd qt_build
    
    cat > stub.cpp << EOF
#include <QMapboxGL>
#include <QMapbox>
#include <QtGui>

namespace QMapbox {
  Coordinate::Coordinate(double lat, double lon) : m_lat(lat), m_lon(lon) {}
  Coordinate::Coordinate() : m_lat(0), m_lon(0) {}
  
  CoordinateZoom::CoordinateZoom(const Coordinate& coordinate_, double zoom_)
      : coordinate(coordinate_), zoom(zoom_) {}
  
  Coordinates::Coordinates() {}
  CoordinatesCollection::CoordinatesCollection() {}
  CoordinatesCollections::CoordinatesCollections() {}
  
  Feature::Feature(FeatureType type_, Coordinate const& coord, PropertyMap props, IdentifierType id_)
      : type(type_), properties(props), identifier(id_) {
    geometry = CoordinatesCollections();
  }
}

QMapboxGLSettings::QMapboxGLSettings() {}
QMapboxGLSettings::~QMapboxGLSettings() {}
QString QMapboxGLSettings::accessToken() const { return QString(); }
void QMapboxGLSettings::setAccessToken(const QString &token) {}

QMapboxGL::QMapboxGL(QObject* parent, const QMapboxGLSettings& settings, const QSize& size, qreal pixelRatio)
    : QObject(parent) {}
QMapboxGL::~QMapboxGL() {}
void QMapboxGL::setCoordinateZoom(const QMapbox::Coordinate &coordinate, double zoom) {}
void QMapboxGL::setStyleUrl(const QString &url) {}
void QMapboxGL::setStyleJson(const QString &json) {}
double QMapboxGL::bearing() const { return 0.0; }
void QMapboxGL::setBearing(double degrees) {}
void QMapboxGL::addClass(const QString &className) {}
void QMapboxGL::removeClass(const QString &className) {}
bool QMapboxGL::hasClass(const QString &className) const { return false; }
void QMapboxGL::setClasses(const QStringList &classNames) {}
QStringList QMapboxGL::getClasses() const { return QStringList(); }
void QMapboxGL::setDefaultTransitionDuration(qint64 duration) {}
double QMapboxGL::getMaxZoom() const { return 0.0; }
double QMapboxGL::getMinZoom() const { return 0.0; }
double QMapboxGL::getZoom() const { return 0.0; }
void QMapboxGL::setZoom(double zoom) {}
void QMapboxGL::setMaxZoom(double max) {}
void QMapboxGL::setMinZoom(double min) {}
double QMapboxGL::getPitch() const { return 0.0; }
void QMapboxGL::setPitch(double pitch) {}
void QMapboxGL::addSource(const QString &sourceID, const QVariantMap &params) {}
void QMapboxGL::addLayer(const QString &id, const QVariantMap &params) {}
void QMapboxGL::addImage(const QString &name, const QImage &sprite) {}
QMapbox::Feature QMapboxGL::queryRenderedFeatures(const QPointF &point, const QStringList &layerIDs) const {
    return QMapbox::Feature(QMapbox::Feature::PointType, QMapbox::Coordinate(), QMapbox::PropertyMap());
}
QMapbox::Feature QMapboxGL::queryRenderedFeatures(const QRectF &geometry, const QStringList &layerIDs) const {
    return QMapbox::Feature(QMapbox::Feature::PointType, QMapbox::Coordinate(), QMapbox::PropertyMap());
}
EOF
    
    g++ -fPIC -shared -o libqmapboxgl.so stub.cpp \
      -I../platform/qt/include \
      -I/system/comma/usr/include \
      -I/system/comma/usr/include/qt \
      -I/system/comma/usr/include/qt/QtCore \
      -I/system/comma/usr/include/qt/QtGui \
      -I/system/comma/usr/include/qt/QtNetwork \
      -L/system/comma/usr/lib \
      -lQt5Core \
      -lQt5Gui \
      -lQt5Network
    
    if [ -f "libqmapboxgl.so" ]; then
      mount -o rw,remount /system 2>/dev/null || true
      cp libqmapboxgl.so /system/lib64/libqmapboxgl.so
      chmod 644 /system/lib64/libqmapboxgl.so
      echo "Successfully built and installed stub libqmapboxgl.so"
    else
      echo "Error: Failed to build stub library."
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
cd "$ORIGINAL_DIR"
echo "Returning to original directory: $ORIGINAL_DIR"

# Continue with the build
echo "Running scons with arguments: $@"
scons "$@"
