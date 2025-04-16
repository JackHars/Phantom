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
  
  # Check for existing mapbox libraries in different locations
  MAPBOX_LIB=$(find /system -name "libqmapboxgl.so" 2>/dev/null | head -1)
  
  # Create or symlink libqmapboxgl.so
  if [ -n "$MAPBOX_LIB" ] && [ "$MAPBOX_LIB" != "/system/lib64/libqmapboxgl.so" ]; then
    echo "Found existing mapbox library at $MAPBOX_LIB"
    mount -o rw,remount /system 2>/dev/null || true
    ln -sf "$MAPBOX_LIB" /system/lib64/libqmapboxgl.so
    echo "Created symlink from $MAPBOX_LIB to /system/lib64/libqmapboxgl.so"
  elif [ ! -f "/system/lib64/libqmapboxgl.so" ]; then
    echo "Building and installing libqmapboxgl.so..."
    
    # Mount system as writable if needed
    mount -o rw,remount /system 2>/dev/null || true
    
    # Set up build environment
    BUILD_DIR=$(mktemp -d)
    echo "Building in temporary directory: $BUILD_DIR"
    
    # Copy the local mapbox-gl-native to the build directory
    cp -r "${ORIGINAL_DIR}/third_party/mapbox-gl-native" "$BUILD_DIR/" 2>/dev/null || echo "No local mapbox-gl-native directory found"
    
    # Create a more comprehensive stub library
    echo "Creating comprehensive stub library..."
    mkdir -p "$BUILD_DIR/mapbox-stub" && cd "$BUILD_DIR/mapbox-stub"
    
    cat > stub.cpp << EOF
#include <QMapboxGL>
#include <QMapbox>
#include <QtGui>
#include <memory>
#include <string>
#include <vector>
#include <map>

// Forward declarations for mbgl classes
namespace mbgl {
    class Size;
    struct ProgramParameters;
    class DebugProgram;
    class SymbolSDFIconProgram;
    class HeatmapProgram;
    class FillExtrusionPatternProgram;
    class SymbolSDFTextProgram;
    class LineSDFProgram;
    class LinePatternProgram;
    class HeatmapTextureProgram;
    class HillshadeProgram;
    class ClippingMaskProgram;
    class RasterProgram;
    class HillshadePrepareProgram;
    class SymbolIconProgram;
    class FillProgram;
    class CollisionCircleProgram;
    class SymbolTextAndIconProgram;
    class CollisionBoxProgram;
    class FillPatternProgram;
    class LineGradientProgram;
    class FillExtrusionProgram;
    class BackgroundProgram;
    class FillOutlinePatternProgram;
    class CircleProgram;
    class FillOutlineProgram;
    class BackgroundPatternProgram;
    class LineProgram;
    
    namespace style {
        class CustomLayerHost;
        class CustomLayer {
        public:
            CustomLayer(const std::string& id, std::unique_ptr<CustomLayerHost>) {}
        };
    }
    
    namespace gl {
        class RendererBackend {
        public:
            RendererBackend(mbgl::gfx::ContextMode) {}
            virtual ~RendererBackend() {}
            void createContext() {}
            void assumeFramebufferBinding(unsigned int) {}
            void setFramebufferBinding(unsigned int) {}
            void setViewport(int, int, const Size&) {}
            void assumeViewport(int, int, const Size&) {}
        };
    }
    
    namespace gfx {
        enum class ContextMode { Unique, Shared };
        enum class Backend { Type };
        
        template <typename T>
        class Program {
        public:
            Program() {}
            virtual ~Program() {}
        };
        
        class Backend {
        public:
            template <typename... Args>
            static std::unique_ptr<Program<DebugProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<DebugProgram>>(new Program<DebugProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<SymbolSDFIconProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<SymbolSDFIconProgram>>(new Program<SymbolSDFIconProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<HeatmapProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<HeatmapProgram>>(new Program<HeatmapProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<FillExtrusionPatternProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<FillExtrusionPatternProgram>>(new Program<FillExtrusionPatternProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<SymbolSDFTextProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<SymbolSDFTextProgram>>(new Program<SymbolSDFTextProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<LineSDFProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<LineSDFProgram>>(new Program<LineSDFProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<LinePatternProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<LinePatternProgram>>(new Program<LinePatternProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<HeatmapTextureProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<HeatmapTextureProgram>>(new Program<HeatmapTextureProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<HillshadeProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<HillshadeProgram>>(new Program<HillshadeProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<ClippingMaskProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<ClippingMaskProgram>>(new Program<ClippingMaskProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<RasterProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<RasterProgram>>(new Program<RasterProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<HillshadePrepareProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<HillshadePrepareProgram>>(new Program<HillshadePrepareProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<SymbolIconProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<SymbolIconProgram>>(new Program<SymbolIconProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<FillProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<FillProgram>>(new Program<FillProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<CollisionCircleProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<CollisionCircleProgram>>(new Program<CollisionCircleProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<SymbolTextAndIconProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<SymbolTextAndIconProgram>>(new Program<SymbolTextAndIconProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<CollisionBoxProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<CollisionBoxProgram>>(new Program<CollisionBoxProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<FillPatternProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<FillPatternProgram>>(new Program<FillPatternProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<LineGradientProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<LineGradientProgram>>(new Program<LineGradientProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<FillExtrusionProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<FillExtrusionProgram>>(new Program<FillExtrusionProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<BackgroundProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<BackgroundProgram>>(new Program<BackgroundProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<FillOutlinePatternProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<FillOutlinePatternProgram>>(new Program<FillOutlinePatternProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<CircleProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<CircleProgram>>(new Program<CircleProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<FillOutlineProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<FillOutlineProgram>>(new Program<FillOutlineProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<BackgroundPatternProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<BackgroundPatternProgram>>(new Program<BackgroundPatternProgram>()); 
            }
            
            template <typename... Args>
            static std::unique_ptr<Program<LineProgram>> Create(const ProgramParameters&) { 
                return std::unique_ptr<Program<LineProgram>>(new Program<LineProgram>()); 
            }
        };
    }
    
    struct Size {
        Size() {}
        Size(int, int) {}
    };
    
    struct ProgramParameters {
        ProgramParameters() {}
    };
}

// Define QMapbox namespace classes and functions
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

// Define QMapboxGLSettings class methods
QMapboxGLSettings::QMapboxGLSettings() {}
QMapboxGLSettings::~QMapboxGLSettings() {}
QString QMapboxGLSettings::accessToken() const { return QString(); }
void QMapboxGLSettings::setAccessToken(const QString &token) {}

// Define QMapboxGL class methods
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
    
    echo "Compiling comprehensive stub library..."
    g++ -fPIC -shared -o libqmapboxgl.so stub.cpp \
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
      
      # Create symlink in phonelibs directory
      mkdir -p "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib"
      cp libqmapboxgl.so "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/"
      echo "Copied to $ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/libqmapboxgl.so"
    else
      echo "Error: Failed to build stub library."
      exit 1
    fi
    
    # Clean up
    cd /tmp
    rm -rf $BUILD_DIR
  else
    echo "libqmapboxgl.so already exists in /system/lib64/"
    
    # Create symlink in phonelibs directory
    mkdir -p "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib"
    cp /system/lib64/libqmapboxgl.so "$ORIGINAL_DIR/phonelibs/mapbox-gl-native-qt/aarch64/lib/"
    echo "Copied existing /system/lib64/libqmapboxgl.so to phonelibs/mapbox-gl-native-qt/aarch64/lib/"
  fi
fi

# Return to original directory
cd "$ORIGINAL_DIR"
echo "Returning to original directory: $ORIGINAL_DIR"

# Continue with the build
echo "Running scons with arguments: $@"
scons "$@"
