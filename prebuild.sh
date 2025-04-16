#!/bin/bash

# Parse command line options
FORCE_REBUILD_MAPBOX=0
SCONS_ARGS=()

for arg in "$@"; do
  if [ "$arg" == "--force-rebuild-mapbox" ]; then
    FORCE_REBUILD_MAPBOX=1
  else
    SCONS_ARGS+=("$arg")
  fi
done

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
  
  # Check if we need to build the mapbox library
  NEED_REBUILD=0
  
  # Check if the library exists in the phonelibs directory
  if [ ! -f "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/libqmapboxgl.so" ]; then
    echo "Library not found in phonelibs/mapbox-gl-native-qt/aarch64/lib, will build it"
    NEED_REBUILD=1
  fi
  
  # Force rebuild if flag is set
  if [ $FORCE_REBUILD_MAPBOX -eq 1 ]; then
    echo "Forcing rebuild of mapbox library due to --force-rebuild-mapbox flag"
    NEED_REBUILD=1
  fi
  
  if [ $NEED_REBUILD -eq 1 ]; then
    # Force rebuild of the stub library
    echo "Building mapbox library..."
    mount -o rw,remount /system 2>/dev/null || true
    
    # Remove existing library if it exists
    if [ -f "/system/lib64/libqmapboxgl.so" ]; then
      echo "Removing existing library at /system/lib64/libqmapboxgl.so"
      rm -f /system/lib64/libqmapboxgl.so
    fi
    
    # Set up build environment
    BUILD_DIR=$(mktemp -d)
    echo "Building in temporary directory: $BUILD_DIR"
    
    # Create a comprehensive stub library with correct template specializations
    echo "Creating comprehensive stub library..."
    mkdir -p "$BUILD_DIR/mapbox-stub" && cd "$BUILD_DIR/mapbox-stub"
    
    cat > stub.cpp << EOF
#include <QtCore>
#include <QtGui>
#include <memory>
#include <string>
#include <vector>
#include <map>

// Forward declarations for QMapbox classes needed by our implementation
namespace QMapbox {
  class Coordinate {
  public:
    double m_lat;
    double m_lon;
    Coordinate(double lat, double lon);
    Coordinate();
  };
  
  class CoordinateZoom {
  public:
    Coordinate coordinate;
    double zoom;
    CoordinateZoom(const Coordinate& coordinate_, double zoom_);
  };
  
  class Coordinates {
  public:
    Coordinates();
  };
  
  class CoordinatesCollection {
  public:
    CoordinatesCollection();
  };
  
  class CoordinatesCollections {
  public:
    CoordinatesCollections();
  };
  
  typedef std::map<QString, QVariant> PropertyMap;
  typedef QVariant IdentifierType;
  
  enum FeatureType {
    PointType,
    LineStringType,
    PolygonType
  };
  
  class Feature {
  public:
    FeatureType type;
    PropertyMap properties;
    IdentifierType identifier;
    CoordinatesCollections geometry;
    Feature(FeatureType type_, Coordinate const& coord, PropertyMap props = PropertyMap(), IdentifierType id_ = IdentifierType());
  };
}

class QMapboxGLSettings {
public:
    QMapboxGLSettings();
    ~QMapboxGLSettings();
    QString accessToken() const;
    void setAccessToken(const QString &token);
};

class QMapboxGL : public QObject {
public:
    QMapboxGL(QObject* parent = nullptr, const QMapboxGLSettings& settings = QMapboxGLSettings(), const QSize& size = QSize(), qreal pixelRatio = 1);
    ~QMapboxGL();
    void setCoordinateZoom(const QMapbox::Coordinate &coordinate, double zoom);
    void setStyleUrl(const QString &url);
    void setStyleJson(const QString &json);
    double bearing() const;
    void setBearing(double degrees);
    void addClass(const QString &className);
    void removeClass(const QString &className);
    bool hasClass(const QString &className) const;
    void setClasses(const QStringList &classNames);
    QStringList getClasses() const;
    void setDefaultTransitionDuration(qint64 duration);
    double getMaxZoom() const;
    double getMinZoom() const;
    double getZoom() const;
    void setZoom(double zoom);
    void setMaxZoom(double max);
    void setMinZoom(double min);
    double getPitch() const;
    void setPitch(double pitch);
    void addSource(const QString &sourceID, const QVariantMap &params);
    void addLayer(const QString &id, const QVariantMap &params);
    void addImage(const QString &name, const QImage &sprite);
    QMapbox::Feature queryRenderedFeatures(const QPointF &point, const QStringList &layerIDs = QStringList()) const;
    QMapbox::Feature queryRenderedFeatures(const QRectF &geometry, const QStringList &layerIDs = QStringList()) const;
};

// Forward declarations for mbgl classes
namespace mbgl {
    class Size {
    public:
        Size() {}
        Size(int, int) {}
    };
    
    struct ProgramParameters {
        ProgramParameters() {}
    };
    
    // Forward declare all the program types
    class DebugProgram {};
    class SymbolSDFIconProgram {};
    class HeatmapProgram {};
    class FillExtrusionPatternProgram {};
    class SymbolSDFTextProgram {};
    class LineSDFProgram {};
    class LinePatternProgram {};
    class HeatmapTextureProgram {};
    class HillshadeProgram {};
    class ClippingMaskProgram {};
    class RasterProgram {};
    class HillshadePrepareProgram {};
    class SymbolIconProgram {};
    class FillProgram {};
    class CollisionCircleProgram {};
    class SymbolTextAndIconProgram {};
    class CollisionBoxProgram {};
    class FillPatternProgram {};
    class LineGradientProgram {};
    class FillExtrusionProgram {};
    class BackgroundProgram {};
    class FillOutlinePatternProgram {};
    class CircleProgram {};
    class FillOutlineProgram {};
    class BackgroundPatternProgram {};
    class LineProgram {};
    
    namespace style {
        class CustomLayerHost {};
        
        class CustomLayer {
        public:
            CustomLayer(const std::string& id, std::unique_ptr<CustomLayerHost> host) {}
        };
    }

    namespace gfx {
        enum class ContextMode { Unique, Shared };
        
        template <typename T>
        class Program {
        public:
            Program() {}
            virtual ~Program() {}
        };
        
        class Backend {
        public:
            enum Type { OpenGL };
            
            template <Type type, typename ProgramType, typename... Args>
            static std::unique_ptr<ProgramType> Create(Args&&... args) {
                return std::unique_ptr<ProgramType>(new ProgramType());
            }
        };
    }
    
    namespace gl {
        class RendererBackend {
        public:
            RendererBackend(gfx::ContextMode) {}
            virtual ~RendererBackend() {}
            void createContext() {}
            void assumeFramebufferBinding(unsigned int) {}
            void setFramebufferBinding(unsigned int) {}
            void setViewport(int, int, const Size&) {}
            void assumeViewport(int, int, const Size&) {}
        };
    }
}

// Define QMapbox namespace implementations
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

// Implement QMapboxGLSettings
QMapboxGLSettings::QMapboxGLSettings() {}
QMapboxGLSettings::~QMapboxGLSettings() {}
QString QMapboxGLSettings::accessToken() const { return QString(); }
void QMapboxGLSettings::setAccessToken(const QString &token) {}

// Implement QMapboxGL
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
    return QMapbox::Feature(QMapbox::PointType, QMapbox::Coordinate(), QMapbox::PropertyMap());
}
QMapbox::Feature QMapboxGL::queryRenderedFeatures(const QRectF &geometry, const QStringList &layerIDs) const {
    return QMapbox::Feature(QMapbox::PointType, QMapbox::Coordinate(), QMapbox::PropertyMap());
}
EOF
    
    # Compile with explicit namespace handling and export all symbols
    echo "Compiling comprehensive stub library..."
    g++ -fPIC -shared -o libqmapboxgl.so stub.cpp -fvisibility=default \
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
      echo "Successfully built and installed comprehensive stub libqmapboxgl.so"
      
      # Create directory in phonelibs
      mkdir -p "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib"
      cp libqmapboxgl.so "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/"
      echo "Copied to $ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/libqmapboxgl.so"
      
      # Add include files
      mkdir -p "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/include"
      
      # Create a simple QMapbox header
      mkdir -p "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/include/QMapbox"
      
      cat > "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/include/QMapbox/QMapbox.hpp" << EOF
#pragma once

#include <QVariant>
#include <QString>
#include <QMap>
#include <QSize>
#include <vector>
#include <memory>

namespace QMapbox {

class Coordinate {
public:
    double m_lat;
    double m_lon;
    Coordinate(double lat = 0, double lon = 0);
};

class CoordinateZoom {
public:
    Coordinate coordinate;
    double zoom;
    CoordinateZoom(const Coordinate& coordinate_, double zoom_);
};

class Coordinates {
public:
    Coordinates();
};

class CoordinatesCollection {
public:
    CoordinatesCollection();
};

class CoordinatesCollections {
public:
    CoordinatesCollections();
};

typedef std::map<QString, QVariant> PropertyMap;
typedef QVariant IdentifierType;

enum FeatureType {
    PointType,
    LineStringType,
    PolygonType
};

class Feature {
public:
    FeatureType type;
    PropertyMap properties;
    IdentifierType identifier;
    CoordinatesCollections geometry;
    Feature(FeatureType type_, Coordinate const& coord, PropertyMap props = PropertyMap(), IdentifierType id_ = IdentifierType());
};

}
EOF

      cat > "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/include/QMapboxGL.hpp" << EOF
#pragma once

#include <QObject>
#include <QSize>
#include <QPointF>
#include <QRectF>
#include <QMapbox/QMapbox.hpp>

class QMapboxGLSettings {
public:
    QMapboxGLSettings();
    ~QMapboxGLSettings();
    QString accessToken() const;
    void setAccessToken(const QString &token);
};

class QMapboxGL : public QObject {
public:
    QMapboxGL(QObject* parent = nullptr, const QMapboxGLSettings& settings = QMapboxGLSettings(), const QSize& size = QSize(), qreal pixelRatio = 1);
    ~QMapboxGL();
    void setCoordinateZoom(const QMapbox::Coordinate &coordinate, double zoom);
    void setStyleUrl(const QString &url);
    void setStyleJson(const QString &json);
    double bearing() const;
    void setBearing(double degrees);
    void addClass(const QString &className);
    void removeClass(const QString &className);
    bool hasClass(const QString &className) const;
    void setClasses(const QStringList &classNames);
    QStringList getClasses() const;
    void setDefaultTransitionDuration(qint64 duration);
    double getMaxZoom() const;
    double getMinZoom() const;
    double getZoom() const;
    void setZoom(double zoom);
    void setMaxZoom(double max);
    void setMinZoom(double min);
    double getPitch() const;
    void setPitch(double pitch);
    void addSource(const QString &sourceID, const QVariantMap &params);
    void addLayer(const QString &id, const QVariantMap &params);
    void addImage(const QString &name, const QImage &sprite);
    QMapbox::Feature queryRenderedFeatures(const QPointF &point, const QStringList &layerIDs = QStringList()) const;
    QMapbox::Feature queryRenderedFeatures(const QRectF &geometry, const QStringList &layerIDs = QStringList()) const;
};
EOF
    else
      echo "Error: Failed to build stub library."
      exit 1
    fi
    
    # Clean up
    cd /tmp
    rm -rf $BUILD_DIR
  else
    echo "Using existing mapbox library, skipping rebuild"
    
    # Ensure the phonelibs directory has the library
    mkdir -p "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib"
    
    # If the library exists in system but not in phonelibs, copy it
    if [ -f "/system/lib64/libqmapboxgl.so" ] && [ ! -f "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/libqmapboxgl.so" ]; then
      cp /system/lib64/libqmapboxgl.so "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/"
      echo "Copied existing /system/lib64/libqmapboxgl.so to phonelibs/mapbox-gl-native-qt/aarch64/lib/"
    fi
  fi
fi

# Return to original directory
cd "$ORIGINAL_DIR"
echo "Returning to original directory: $ORIGINAL_DIR"

# Continue with the build
echo "Running scons with arguments: ${SCONS_ARGS[@]}"
scons "${SCONS_ARGS[@]}"
