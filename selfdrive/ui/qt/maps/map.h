#pragma once

#include <optional>

#include <QGeoCoordinate>
#include <QGeoManeuver>
#include <QGeoRouteRequest>
#include <QGeoRouteSegment>
#include <QGeoRoutingManager>
#include <QGeoServiceProvider>
#include <QGestureEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMapboxGL>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QScopedPointer>
#include <QString>
#include <QtGlobal>
#include <QTimer>
#include <QWheelEvent>
#include <QMap>
#include <QPixmap>
#include <QPushButton>
#include <QQuickWidget>

#include "selfdrive/common/params.h"
#include "selfdrive/common/util.h"
#include "cereal/messaging/messaging.h"
#include "geocoder.h"
#include "selfdrive/ui/ui.h"
#include "search_results.h"

class MapInstructions : public QWidget {
  Q_OBJECT

private:
  QLabel *distance;
  QLabel *primary;
  QLabel *secondary;
  QLabel *icon_01;
  QHBoxLayout *lane_layout;
  QMap<QString, QVariant> last_banner;
  bool error = false;

public:
  MapInstructions(QWidget * parent=nullptr);
  void showError(QString error);
  void hideIfNoError();

public slots:
  void updateDistance(float d);
  void updateInstructions(QMap<QString, QVariant> banner, bool full);
};

class MapETA : public QWidget {
  Q_OBJECT

private:
  QLabel *eta;
  QLabel *eta_unit;
  QLabel *time;
  QLabel *time_unit;
  QLabel *distance;
  QLabel *distance_unit;
  Params params;

public:
  MapETA(QWidget * parent=nullptr);

public slots:
  void updateETA(float seconds, float seconds_typical, float distance);
  
signals:
  void visibilityChanged(bool visible);
};

class MapWindow : public QOpenGLWidget {
  Q_OBJECT

public:
  MapWindow(const QMapboxGLSettings &);
  ~MapWindow();

  void setVisible(bool visible) override;
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseDoubleClickEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;
  void wheelEvent(QWheelEvent *ev) override;
  bool event(QEvent *event) override;

private:
  QMapboxGLSettings m_settings;
  QScopedPointer<QMapboxGL> m_map;

  void initLayers();

  bool gestureEvent(QGestureEvent *event);
  void pinchTriggered(QPinchGesture *gesture);

  bool m_sourceAdded = false;
  SubMaster *sm;
  QTimer* timer;

  bool loaded_once = false;

  // Panning
  QPointF m_lastPos;
  int pan_counter = 0;
  int zoom_counter = 0;

  // Position
  std::optional<QMapbox::Coordinate> last_position;
  std::optional<float> last_bearing;
  FirstOrderFilter velocity_filter;
  bool localizer_valid = false;

  // Route
  bool allow_open = true;
  bool gps_ok = false;
  QGeoServiceProvider *geoservice_provider;
  QGeoRoutingManager *routing_manager;
  QGeoRoute route;
  QGeoRouteSegment segment;

  MapInstructions* map_instructions;
  MapETA* map_eta;
  QPushButton* search_button;
  QPushButton* toggle_fullscreen_button;
  bool is_fullscreen = false;
  QRect original_geometry;
  void toggleFullscreen();
  void updateToggleButton();
  
  // Geocoding and search
  Geocoder* geocoder;
  bool token_verified = false;
  void searchLocation();
  void handleSearchResult(bool success, QMapbox::Coordinate coordinate, QString name, QString address);
  void handleSearchResults(bool success, const QVector<LocationResult> &results, const QString &query, const QString &errorMessage);
  void handleTokenVerification(bool valid, QString message);
  void handleSearchResultSelected(const LocationResult &result);

  // Search results UI
  SearchResultsWidget* search_results_widget;

  QMapbox::Coordinate nav_destination;

  // Route recompute
  QTimer* recompute_timer;
  int recompute_backoff = 0;
  int recompute_countdown = 0;
  void calculateRoute(QMapbox::Coordinate destination);
  void clearRoute(bool removeDestination = false);
  bool shouldRecompute();
  void updateETA();

private slots:
  void timerUpdate();
  void routeCalculated(QGeoRouteReply *reply);
  void recomputeRoute();

public slots:
  void offroadTransition(bool offroad);
  void clearRouteVisual();

signals:
  void distanceChanged(float distance);
  void instructionsChanged(QMap<QString, QVariant> banner, bool full);
  void ETAChanged(float seconds, float seconds_typical, float distance);
  void fullscreenToggled(bool is_fullscreen);
};

