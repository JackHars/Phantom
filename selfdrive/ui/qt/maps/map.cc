#include "selfdrive/ui/qt/maps/map.h"

#include <cmath>

#include <QDebug>

#include "selfdrive/common/swaglog.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/maps/map_helpers.h"
#include "selfdrive/ui/qt/request_repeater.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "selfdrive/ui/qt/maps/search_results.h"


const int PAN_TIMEOUT = 100;
const qreal REROUTE_DISTANCE = 25;
const float MANEUVER_TRANSITION_THRESHOLD = 10;

const float MAX_ZOOM = 17;
const float MIN_ZOOM = 14;
const float MAX_PITCH = 50;
const float MIN_PITCH = 0;
const float MAP_SCALE = 2;


MapWindow::MapWindow(const QMapboxGLSettings &settings) :
  m_settings(settings), velocity_filter(0, 10, 0.1) {
  sm = new SubMaster({"liveLocationKalman"});

  timer = new QTimer(this);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
  timer->start(100);

  recompute_timer = new QTimer(this);
  QObject::connect(recompute_timer, SIGNAL(timeout()), this, SLOT(recomputeRoute()));
  recompute_timer->start(1000);

  // Instructions
  map_instructions = new MapInstructions(this);
  QObject::connect(this, &MapWindow::instructionsChanged, map_instructions, &MapInstructions::updateInstructions);
  QObject::connect(this, &MapWindow::distanceChanged, map_instructions, &MapInstructions::updateDistance);
  map_instructions->setFixedWidth(width());
  map_instructions->setVisible(false);

  map_eta = new MapETA(this);
  QObject::connect(this, &MapWindow::ETAChanged, map_eta, &MapETA::updateETA);
  QObject::connect(map_eta, &MapETA::visibilityChanged, [this](bool visible) {
    // Reposition search button when ETA visibility changes
    const int margin = 20;
    if (visible) {
      int button_y = height() - search_button->height() - margin - map_eta->height() - bdr_s;
      search_button->move(width() - search_button->width() - margin, button_y);
    } else {
      search_button->move(width() - search_button->width() - margin, 
                         height() - search_button->height() - margin);
    }
  });

  const int h = 120;
  map_eta->setFixedHeight(h);
  map_eta->move(25, 1080 - h - bdr_s*2);
  map_eta->setVisible(false);

  // Add search button in bottom right corner
  search_button = new QPushButton(this);
  search_button->setFixedSize(120, 120);
  search_button->setStyleSheet(R"(
    QPushButton {
      background-color: #000000;
      border-radius: 15px;
      padding: 20px;
      border: 2px solid #333333;
    }
    QPushButton:pressed {
      background-color: #222222;
    }
  )");
  search_button->setIcon(QIcon("../assets/navigation/search.svg"));
  search_button->setIconSize(QSize(70, 70));
  QObject::connect(search_button, &QPushButton::clicked, this, &MapWindow::searchLocation);

  // Add toggle fullscreen button on the left side
  toggle_fullscreen_button = new QPushButton(this);
  toggle_fullscreen_button->setFixedSize(120, 120);
  toggle_fullscreen_button->setStyleSheet(R"(
    QPushButton {
      background-color: #000000;
      border-radius: 15px;
      padding: 20px;
      border: 2px solid #333333;
      z-index: 999;
    }
    QPushButton:pressed {
      background-color: #222222;
    }
  )");
  toggle_fullscreen_button->setIcon(QIcon("../assets/navigation/arrow_left.svg"));
  toggle_fullscreen_button->setIconSize(QSize(70, 70));
  toggle_fullscreen_button->raise(); // Make sure button is on top
  toggle_fullscreen_button->setCursor(Qt::PointingHandCursor); // Set cursor to hand pointer
  QObject::connect(toggle_fullscreen_button, &QPushButton::clicked, this, &MapWindow::toggleFullscreen);

  // Initialize geocoder
  geocoder = new Geocoder(this);
  QObject::connect(geocoder, &Geocoder::searchResult, this, &MapWindow::handleSearchResult);
  QObject::connect(geocoder, &Geocoder::searchResults, this, &MapWindow::handleSearchResults);
  QObject::connect(geocoder, &Geocoder::tokenVerified, this, &MapWindow::handleTokenVerification);
  
  // Verify token on startup
  geocoder->verifyToken();

  // Initialize the search results widget
  search_results_widget = new SearchResultsWidget(this);
  search_results_widget->setVisible(false);
  QObject::connect(search_results_widget, &SearchResultsWidget::resultSelected, this, &MapWindow::handleSearchResultSelected);
  QObject::connect(search_results_widget, &SearchResultsWidget::cancelled, [=]() {
      search_results_widget->setVisible(false);
  });

  // Routing
  QVariantMap parameters;
  parameters["mapbox.access_token"] = m_settings.accessToken();

  geoservice_provider = new QGeoServiceProvider("mapbox", parameters);
  routing_manager = geoservice_provider->routingManager();
  if (routing_manager == nullptr) {
    qDebug() << geoservice_provider->errorString();
    assert(routing_manager);
  }
  QObject::connect(routing_manager, &QGeoRoutingManager::finished, this, &MapWindow::routeCalculated);

  auto last_gps_position = coordinate_from_param("LastGPSPosition");
  if (last_gps_position) {
    last_position = *last_gps_position;
  }

  grabGesture(Qt::GestureType::PinchGesture);
}

MapWindow::~MapWindow() {
  makeCurrent();
}

void MapWindow::initLayers() {
  // This doesn't work from initializeGL
  if (!m_map->layerExists("modelPathLayer")) {
    qDebug() << "Initializing modelPathLayer";
    QVariantMap modelPath;
    modelPath["id"] = "modelPathLayer";
    modelPath["type"] = "line";
    modelPath["source"] = "modelPathSource";
    m_map->addLayer(modelPath);
    m_map->setPaintProperty("modelPathLayer", "line-color", QColor("red"));
    m_map->setPaintProperty("modelPathLayer", "line-width", 5.0);
    m_map->setLayoutProperty("modelPathLayer", "line-cap", "round");
  }
  if (!m_map->layerExists("navLayer")) {
    qDebug() << "Initializing navLayer";
    QVariantMap nav;
    nav["id"] = "navLayer";
    nav["type"] = "line";
    nav["source"] = "navSource";
    m_map->addLayer(nav, "road-intersection");
    m_map->setPaintProperty("navLayer", "line-color", QColor("#31a1ee"));
    m_map->setPaintProperty("navLayer", "line-width", 7.5);
    m_map->setLayoutProperty("navLayer", "line-cap", "round");
  }
  if (!m_map->layerExists("carPosLayer")) {
    qDebug() << "Initializing carPosLayer";
    m_map->addImage("label-arrow", QImage("../assets/images/triangle.svg"));

    QVariantMap carPos;
    carPos["id"] = "carPosLayer";
    carPos["type"] = "symbol";
    carPos["source"] = "carPosSource";
    m_map->addLayer(carPos);
    m_map->setLayoutProperty("carPosLayer", "icon-pitch-alignment", "map");
    m_map->setLayoutProperty("carPosLayer", "icon-image", "label-arrow");
    m_map->setLayoutProperty("carPosLayer", "icon-size", 0.5);
    m_map->setLayoutProperty("carPosLayer", "icon-ignore-placement", true);
    m_map->setLayoutProperty("carPosLayer", "icon-allow-overlap", true);
    m_map->setLayoutProperty("carPosLayer", "symbol-sort-key", 0);
  }
}

void MapWindow::timerUpdate() {
  if (!QUIState::ui_state.scene.started) {
    return;
  }

  if (isVisible()) {
    update();
  }

  sm->update(0);
  if (sm->updated("liveLocationKalman")) {
    auto location = (*sm)["liveLocationKalman"].getLiveLocationKalman();
    gps_ok = location.getGpsOK();

    localizer_valid = location.getStatus() == cereal::LiveLocationKalman::Status::VALID;

    if (localizer_valid) {
      auto pos = location.getPositionGeodetic();
      auto orientation = location.getCalibratedOrientationNED();

      float velocity = location.getVelocityCalibrated().getValue()[0];
      float bearing = RAD2DEG(orientation.getValue()[2]);
      auto coordinate = QMapbox::Coordinate(pos.getValue()[0], pos.getValue()[1]);

      last_position = coordinate;
      last_bearing = bearing;
      velocity_filter.update(velocity);
    }
  }

  if (m_map.isNull()) {
    return;
  }

  loaded_once = loaded_once || m_map->isFullyLoaded();
  if (!loaded_once) {
    map_instructions->showError("Map Loading");
    return;
  }

  initLayers();

  if (!localizer_valid) {
    map_instructions->showError("Waiting for GPS");
    return;
  }

  if (pan_counter == 0) {
    if (last_position) m_map->setCoordinate(*last_position);
    if (last_bearing) m_map->setBearing(*last_bearing);
  } else {
    pan_counter--;
  }

  if (zoom_counter == 0) {
    m_map->setZoom(util::map_val<float>(velocity_filter.x(), 0, 30, MAX_ZOOM, MIN_ZOOM));
  } else {
    zoom_counter--;
  }

  // Update current location marker
  auto point = coordinate_to_collection(*last_position);
  QMapbox::Feature feature1(QMapbox::Feature::PointType, point, {}, {});
  QVariantMap carPosSource;
  carPosSource["type"] = "geojson";
  carPosSource["data"] = QVariant::fromValue<QMapbox::Feature>(feature1);
  m_map->updateSource("carPosSource", carPosSource);

  // Show route instructions
  if (segment.isValid()) {
    auto cur_maneuver = segment.maneuver();
    auto attrs = cur_maneuver.extendedAttributes();
    if (cur_maneuver.isValid() && attrs.contains("mapbox.banner_instructions")) {
      float along_geometry = distance_along_geometry(segment.path(), to_QGeoCoordinate(*last_position));
      float distance_to_maneuver = segment.distance() - along_geometry;
      emit distanceChanged(std::max(0.0f, distance_to_maneuver));

      m_map->setPitch(MAX_PITCH); // TODO: smooth pitching based on maneuver distance

      auto banner = attrs["mapbox.banner_instructions"].toList();
      if (banner.size()) {
        auto banner_0 = banner[0].toMap();
        float show_at = banner_0["distance_along_geometry"].toDouble();
        emit instructionsChanged(banner_0, distance_to_maneuver < show_at);
      }

      // Transition to next route segment
      if (!shouldRecompute() && (distance_to_maneuver < -MANEUVER_TRANSITION_THRESHOLD)) {
        auto next_segment = segment.nextRouteSegment();
        if (next_segment.isValid()) {
          segment = next_segment;

          recompute_backoff = std::max(0, recompute_backoff - 1);
          recompute_countdown = 0;
        } else {
          qWarning() << "Destination reached";
          // Commenting out automatic NavDestination removal
          // Params().remove("NavDestination");

          // We've reached the destination, but keep the route and instructions visible
          // Only update the UI to indicate arrival
          map_instructions->showError("You have arrived at your destination");
          
          // Don't clear the route - keep it visible on the map
          // We'll rely on the user to clear it when they want
        }
      }
    }
  }
}

void MapWindow::resizeGL(int w, int h) {
  m_map->resize(size() / MAP_SCALE);
  map_instructions->setFixedWidth(width());
  
  // Position the search button in bottom right corner
  const int margin = 20;

  // If the ETA widget is visible, position the search button above it
  if (map_eta->isVisible()) {
    int button_y = height() - search_button->height() - margin - map_eta->height() - bdr_s;
    search_button->move(width() - search_button->width() - margin, button_y);
  } else {
    // Default position in bottom right when no ETA widget
    search_button->move(width() - search_button->width() - margin, 
                     height() - search_button->height() - margin);
  }
  
  // Position the toggle fullscreen button on the left side
  toggle_fullscreen_button->move(margin, height() / 2 - toggle_fullscreen_button->height() / 2);
                     
  // Reposition search results if visible
  if (search_results_widget && search_results_widget->isVisible()) {
    search_results_widget->setGeometry(0, 0, width(), height());
  }
}

void MapWindow::initializeGL() {
  m_map.reset(new QMapboxGL(this, m_settings, size(), 1));

  if (last_position) {
    m_map->setCoordinateZoom(*last_position, MAX_ZOOM);
  } else {
    m_map->setCoordinateZoom(QMapbox::Coordinate(64.31990695292795, -149.79038934046247), MIN_ZOOM);
  }

  m_map->setMargins({0, 350, 0, 50});
  m_map->setPitch(MIN_PITCH);
  // m_map->setStyleUrl("mapbox://styles/commaai/ckr64tlwp0azb17nqvr9fj13s");
  m_map->setStyleUrl("mapbox://styles/mapbox/navigation-night-v1");
  connect(m_map.data(), SIGNAL(needsRendering()), this, SLOT(update()));
  QObject::connect(m_map.data(), &QMapboxGL::mapChanged, [=](QMapboxGL::MapChange change) {
    if (change == QMapboxGL::MapChange::MapChangeDidFinishLoadingMap) {
      loaded_once = true;
    }
  });
}

void MapWindow::paintGL() {
  if (!isVisible()) return;
  m_map->render();
}

static float get_time_typical(const QGeoRouteSegment &segment) {
  auto maneuver = segment.maneuver();
  auto attrs = maneuver.extendedAttributes();
  return attrs.contains("mapbox.duration_typical") ? attrs["mapbox.duration_typical"].toDouble() : segment.travelTime();
}


void MapWindow::recomputeRoute() {
  if (!QUIState::ui_state.scene.started) {
    qWarning() << "recomputeRoute: car not started, returning";
    return;
  }

  if (!last_position) {
    qWarning() << "recomputeRoute: no position, returning";
    return;
  }

  auto new_destination = coordinate_from_param("NavDestination");
  qWarning() << "recomputeRoute: checking NavDestination:" << (new_destination.has_value() ? "Set" : "Not set");
  
  if (!new_destination) {
    // Just clear the visual route, don't remove the NavDestination param
    qWarning() << "recomputeRoute: no destination, clearing route visual only";
    clearRouteVisual();
    return;
  }

  bool should_recompute = shouldRecompute();
  if (*new_destination != nav_destination) {
    qWarning() << "Got new destination from NavDestination param" << *new_destination;

    // Only open the map on setting destination the first time
    if (allow_open) {
      setVisible(true); // Show map on destination set/change
      allow_open = false;
    }

    // TODO: close sidebar

    should_recompute = true;
  }

  if (!should_recompute) updateETA(); // ETA is updated after recompute

  // Don't recompute when gps drifts in tunnels or when we already have a valid route
  if ((!gps_ok || segment.isValid()) && !should_recompute) {
    return;
  } 

  // Don't recompute too often - use exponential backoff
  // Only do API request when map is fully loaded
  if (loaded_once) {
    if (recompute_countdown == 0 && should_recompute) {
      recompute_countdown = std::pow(2, recompute_backoff);
      recompute_backoff = std::min(7, recompute_backoff + 1);
      calculateRoute(*new_destination);
    } else {
      recompute_countdown = std::max(0, recompute_countdown - 1);
    }
  }
}

void MapWindow::updateETA() {
  if (segment.isValid()) {
    float progress = distance_along_geometry(segment.path(), to_QGeoCoordinate(*last_position)) / segment.distance();
    float total_distance = segment.distance() * (1.0 - progress);
    float total_time = segment.travelTime() * (1.0 - progress);
    float total_time_typical = get_time_typical(segment) * (1.0 - progress);

    auto s = segment.nextRouteSegment();
    while (s.isValid()) {
      total_distance += s.distance();
      total_time += s.travelTime();
      total_time_typical += get_time_typical(s);

      s = s.nextRouteSegment();
    }

    emit ETAChanged(total_time, total_time_typical, total_distance);
    
    // Reposition search button when ETA becomes visible
    const int margin = 20;
    int button_y = height() - search_button->height() - margin - map_eta->height() - bdr_s;
    search_button->move(width() - search_button->width() - margin, button_y);
  }
}

void MapWindow::calculateRoute(QMapbox::Coordinate destination) {
  qWarning() << "Calculating route" << *last_position << "->" << destination;

  nav_destination = destination;
  QGeoRouteRequest request(to_QGeoCoordinate(*last_position), to_QGeoCoordinate(destination));
  request.setFeatureWeight(QGeoRouteRequest::TrafficFeature, QGeoRouteRequest::AvoidFeatureWeight);

  if (last_bearing) {
    QVariantMap params;
    int bearing = ((int)(*last_bearing) + 360) % 360;
    params["bearing"] = bearing;
    request.setWaypointsMetadata({params});
  }

  routing_manager->calculateRoute(request);
}

void MapWindow::routeCalculated(QGeoRouteReply *reply) {
  bool got_route = false;
  if (reply->error() == QGeoRouteReply::NoError) {
    if (reply->routes().size() != 0) {
      qWarning() << "Got route response";

      route = reply->routes().at(0);
      segment = route.firstRouteSegment();

      auto route_points = coordinate_list_to_collection(route.path());
      QMapbox::Feature feature(QMapbox::Feature::LineStringType, route_points, {}, {});
      QVariantMap navSource;
      navSource["type"] = "geojson";
      navSource["data"] = QVariant::fromValue<QMapbox::Feature>(feature);
      m_map->updateSource("navSource", navSource);
      m_map->setLayoutProperty("navLayer", "visibility", "visible");
      got_route = true;

      updateETA();
    } else {
      qWarning() << "Got empty route response";
    }
  } else {
    qWarning() << "Got error in route reply" << reply->errorString();
  }

  if (!got_route) {
    map_instructions->showError("Failed to Route");
  }

  reply->deleteLater();
}

void MapWindow::clearRoute(bool removeDestination) {
  qWarning() << "MapWindow::clearRoute called with removeDestination=" << removeDestination;
  segment = QGeoRouteSegment();
  nav_destination = QMapbox::Coordinate();

  if (!m_map.isNull()) {
    m_map->setLayoutProperty("navLayer", "visibility", "none");
    m_map->setPitch(MIN_PITCH);
  }

  // Only remove the NavDestination parameter if explicitly requested
  if (removeDestination) {
    qWarning() << "Explicitly removing NavDestination parameter";
    Params().remove("NavDestination");
  }

  map_instructions->hideIfNoError();
  map_eta->setVisible(false);
  allow_open = true;
}


bool MapWindow::shouldRecompute() {
  if (!segment.isValid()) {
    return true;
  }

  // Even if far from the route, don't recompute if the user purposely
  // navigated away (e.g. looking around the map)
  if (pan_counter > 0 || zoom_counter > 0) {
    return false;
  }

  // Compute closest distance to all line segments in the current path
  float min_d = REROUTE_DISTANCE + 1;
  auto path = segment.path();
  auto cur = to_QGeoCoordinate(*last_position);
  for (size_t i = 0; i < path.size() - 1; i++) {
    auto a = path[i];
    auto b = path[i+1];
    if (a.distanceTo(b) < 1.0) {
      continue;
    }
    min_d = std::min(min_d, minimum_distance(a, b, cur));
  }

  // Only trigger reroute if we're far off route AND moving away from destination
  if (min_d > REROUTE_DISTANCE) {
    auto dest = to_QGeoCoordinate(nav_destination);
    double currentDistToDest = cur.distanceTo(dest);
    
    // Store the distance to destination to track if we're moving away
    static double lastDistToDest = currentDistToDest;
    
    // Check if we're moving away from destination over time
    bool movingAway = (currentDistToDest > lastDistToDest + 5); // 5m buffer
    lastDistToDest = currentDistToDest;
    
    return movingAway;
  }
  
  return false;
}

void MapWindow::mousePressEvent(QMouseEvent *ev) {
  m_lastPos = ev->localPos();
  ev->accept();
}

void MapWindow::mouseDoubleClickEvent(QMouseEvent *ev) {
  if (last_position) m_map->setCoordinate(*last_position);
  if (last_bearing) m_map->setBearing(*last_bearing);
  m_map->setZoom(util::map_val<float>(velocity_filter.x(), 0, 30, MAX_ZOOM, MIN_ZOOM));

  pan_counter = 0;
  zoom_counter = 0;
}

void MapWindow::mouseMoveEvent(QMouseEvent *ev) {
  QPointF delta = ev->localPos() - m_lastPos;

  if (!delta.isNull()) {
    pan_counter = PAN_TIMEOUT;
    m_map->moveBy(delta / MAP_SCALE);
  }

  m_lastPos = ev->localPos();
  ev->accept();
}

void MapWindow::wheelEvent(QWheelEvent *ev) {
  if (ev->orientation() == Qt::Horizontal) {
      return;
  }

  float factor = ev->delta() / 1200.;
  if (ev->delta() < 0) {
      factor = factor > -1 ? factor : 1 / factor;
  }

  m_map->scaleBy(1 + factor, ev->pos() / MAP_SCALE);
  zoom_counter = PAN_TIMEOUT;
  ev->accept();
}

bool MapWindow::event(QEvent *event) {
  if (event->type() == QEvent::Gesture) {
    return gestureEvent(static_cast<QGestureEvent*>(event));
  }

  return QWidget::event(event);
}

bool MapWindow::gestureEvent(QGestureEvent *event) {
  if (QGesture *pinch = event->gesture(Qt::PinchGesture)) {
    pinchTriggered(static_cast<QPinchGesture *>(pinch));
  }
  return true;
}

void MapWindow::pinchTriggered(QPinchGesture *gesture) {
  QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
  if (changeFlags & QPinchGesture::ScaleFactorChanged) {
    // TODO: figure out why gesture centerPoint doesn't work
    m_map->scaleBy(gesture->scaleFactor(), {width() / 2.0 / MAP_SCALE, height() / 2.0 / MAP_SCALE});
    zoom_counter = PAN_TIMEOUT;
  }
}

void MapWindow::offroadTransition(bool offroad) {
  qWarning() << "MapWindow::offroadTransition called with offroad=" << offroad;
  if (!offroad) {
    auto dest = coordinate_from_param("NavDestination");
    qWarning() << "Current NavDestination:" << (dest.has_value() ? "Set" : "Not set");
    // Always show map when onroad, regardless of destination status
    setVisible(true);
  }
  last_bearing = {};
}

MapInstructions::MapInstructions(QWidget * parent) : QWidget(parent) {
  QHBoxLayout *main_layout = new QHBoxLayout(this);
  main_layout->setContentsMargins(11, 50, 11, 11);
  {
    QVBoxLayout *layout = new QVBoxLayout;
    icon_01 = new QLabel;
    layout->addWidget(icon_01);
    layout->addStretch();
    main_layout->addLayout(layout);
  }

  {
    QWidget *w = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(w);

    distance = new QLabel;
    distance->setStyleSheet(R"(font-size: 90px;)");
    layout->addWidget(distance);

    primary = new QLabel;
    primary->setStyleSheet(R"(font-size: 60px;)");
    primary->setWordWrap(true);
    layout->addWidget(primary);

    secondary = new QLabel;
    secondary->setStyleSheet(R"(font-size: 50px;)");
    secondary->setWordWrap(true);
    layout->addWidget(secondary);

    lane_layout = new QHBoxLayout;
    layout->addLayout(lane_layout);

    main_layout->addWidget(w);
  }

  setStyleSheet(R"(
    * {
      color: white;
      font-family: "Inter";
    }
  )");

  QPalette pal = palette();
  pal.setColor(QPalette::Background, QColor(0, 0, 0, 150));
  setAutoFillBackground(true);
  setPalette(pal);
}

void MapInstructions::updateDistance(float d) {
  QString distance_str;

  if (QUIState::ui_state.scene.is_metric) {
    if (d > 500) {
      distance_str.setNum(d / 1000, 'f', 1);
      distance_str += " km";
    } else {
      distance_str.setNum(50 * int(d / 50));
      distance_str += " m";
    }
  } else {
    float miles = d * METER_2_MILE;
    float feet = d * METER_2_FOOT;

    if (feet > 500) {
      distance_str.setNum(miles, 'f', 1);
      distance_str += " mi";
    } else {
      distance_str.setNum(50 * int(feet / 50));
      distance_str += " ft";
    }
  }

  distance->setAlignment(Qt::AlignLeft);
  distance->setText(distance_str);
}

void MapInstructions::showError(QString error) {
  primary->setText("");
  distance->setText(error);
  distance->setAlignment(Qt::AlignCenter);

  secondary->setVisible(false);
  icon_01->setVisible(false);

  last_banner = {};
  error = true;

  setVisible(true);
  adjustSize();
}

void MapInstructions::updateInstructions(QMap<QString, QVariant> banner, bool full) {
  // Need multiple calls to adjustSize for it to properly resize
  // seems like it takes a little bit of time for the images to change and
  // the size can only be changed afterwards
  adjustSize();

  // Word wrap widgets need fixed width
  primary->setFixedWidth(width() - 250);
  secondary->setFixedWidth(width() - 250);

  if (banner == last_banner) return;
  QString primary_str, secondary_str;

  auto p = banner["primary"].toMap();
  primary_str += p["text"].toString();

  // Show arrow with direction
  if (p.contains("type")) {
    QString fn = "../assets/navigation/direction_" + p["type"].toString();
    if (p.contains("modifier")) {
      fn += "_" + p["modifier"].toString();
    }
    fn +=  + ".png";
    fn = fn.replace(' ', '_');

    QPixmap pix(fn);
    icon_01->setPixmap(pix.scaledToWidth(200, Qt::SmoothTransformation));
    icon_01->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    icon_01->setVisible(true);
  }

  // Parse components (e.g. lanes, exit number)
  auto components = p["components"].toList();
  QString icon_fn;
  for (auto &c : components) {
    auto cc = c.toMap();
    if (cc["type"].toString() == "icon") {
      icon_fn = cc["imageBaseURL"].toString() + "@3x.png";
    }
  }

  if (banner.contains("secondary") && full) {
    auto s = banner["secondary"].toMap();
    secondary_str += s["text"].toString();
  }

  clearLayout(lane_layout);
  bool has_lanes = false;

  if (banner.contains("sub") && full) {
    auto s = banner["sub"].toMap();
    auto components = s["components"].toList();
    for (auto &c : components) {
      auto cc = c.toMap();
      if (cc["type"].toString() == "lane") {
        has_lanes = true;

        bool left = false;
        bool straight = false;
        bool right = false;
        bool active = cc["active"].toBool();

        for (auto &dir : cc["directions"].toList()) {
          auto d = dir.toString();
          left |= d.contains("left");
          straight |= d.contains("straight");
          right |= d.contains("right");
        }

        // TODO: Make more images based on active direction and combined directions
        QString fn = "../assets/navigation/direction_";
        if (left) {
          fn += "turn_left";
        } else if (right) {
          fn += "turn_right";
        } else if (straight) {
          fn += "turn_straight";
        }

        QPixmap pix(fn + ".png");
        auto icon = new QLabel;
        icon->setPixmap(pix.scaledToWidth(active ? 125 : 75, Qt::SmoothTransformation));
        icon->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        lane_layout->addWidget(icon);
      }
    }
  }

  primary->setText(primary_str);
  secondary->setVisible(secondary_str.length() > 0);
  secondary->setText(secondary_str);

  last_banner = banner;
  error = false;

  show();
  adjustSize();
}

void MapInstructions::hideIfNoError() {
  if (!error) {
    hide();
  }
}

MapETA::MapETA(QWidget * parent) : QWidget(parent) {
  QHBoxLayout *main_layout = new QHBoxLayout(this);
  main_layout->setContentsMargins(40, 25, 40, 25);

  {
    QHBoxLayout *layout = new QHBoxLayout;
    eta = new QLabel;
    eta->setAlignment(Qt::AlignCenter);
    eta->setStyleSheet("font-weight:600");

    eta_unit = new QLabel;
    eta_unit->setAlignment(Qt::AlignCenter);

    layout->addWidget(eta);
    layout->addWidget(eta_unit);
    main_layout->addLayout(layout);
  }
  main_layout->addSpacing(40);
  {
    QHBoxLayout *layout = new QHBoxLayout;
    time = new QLabel;
    time->setAlignment(Qt::AlignCenter);

    time_unit = new QLabel;
    time_unit->setAlignment(Qt::AlignCenter);

    layout->addWidget(time);
    layout->addWidget(time_unit);
    main_layout->addLayout(layout);
  }
  main_layout->addSpacing(40);
  {
    QHBoxLayout *layout = new QHBoxLayout;
    distance = new QLabel;
    distance->setAlignment(Qt::AlignCenter);
    distance->setStyleSheet("font-weight:600");

    distance_unit = new QLabel;
    distance_unit->setAlignment(Qt::AlignCenter);

    layout->addWidget(distance);
    layout->addWidget(distance_unit);
    main_layout->addLayout(layout);
  }

  setStyleSheet(R"(
    * {
      color: white;
      font-family: "Inter";
      font-size: 70px;
    }
  )");

  QPalette pal = palette();
  pal.setColor(QPalette::Background, QColor(0, 0, 0, 150));
  setAutoFillBackground(true);
  setPalette(pal);
}


void MapETA::updateETA(float s, float s_typical, float d) {
  // Store previous visibility state
  bool was_visible = isVisible();
  
  if (d < MANEUVER_TRANSITION_THRESHOLD) {
    hide();
    return;
  }

  // ETA
  auto eta_time = QDateTime::currentDateTime().addSecs(s).time();
  if (params.getBool("NavSettingTime24h")) {
    eta->setText(eta_time.toString("HH:mm"));
    eta_unit->setText("eta");
  } else {
    auto t = eta_time.toString("h:mm a").split(' ');
    eta->setText(t[0]);
    eta_unit->setText(t[1]);
  }

  // Remaining time
  if (s < 3600) {
    time->setText(QString::number(int(s / 60)));
    time_unit->setText("min");
  } else {
    int hours = int(s) / 3600;
    time->setText(QString::number(hours) + ":" + QString::number(int((s - hours * 3600) / 60)).rightJustified(2, '0'));
    time_unit->setText("hr");
  }

  QString color;
  if (s / s_typical > 1.5) {
    color = "#DA3025";
  } else if (s / s_typical > 1.2) {
    color = "#DAA725";
  } else {
    color = "#25DA6E";
  }

  time->setStyleSheet(QString(R"(color: %1; font-weight:600;)").arg(color));
  time_unit->setStyleSheet(QString(R"(color: %1;)").arg(color));

  // Distance
  QString distance_str;
  float num = 0;
  if (QUIState::ui_state.scene.is_metric) {
    num = d / 1000.0;
    distance_unit->setText("km");
  } else {
    num = d * METER_2_MILE;
    distance_unit->setText("mi");
  }

  distance_str.setNum(num, 'f', num < 100 ? 1 : 0);
  distance->setText(distance_str);

  show();
  adjustSize();
  repaint();
  adjustSize();

  // Rounded corners
  const int radius = 25;
  const auto r = rect();

  // Top corners rounded
  QPainterPath path;
  path.setFillRule(Qt::WindingFill);
  path.addRoundedRect(r, radius, radius);

  // Bottom corners not rounded
  path.addRect(r.marginsRemoved(QMargins(0, radius, 0, 0)));

  // Set clipping mask
  QRegion mask = QRegion(path.simplified().toFillPolygon().toPolygon());
  setMask(mask);

  // Center
  move(static_cast<QWidget*>(parent())->width() / 2 - width() / 2, 1080 - height() - bdr_s*2);
  
  // If visibility changed, notify MapWindow to update layout
  if (!was_visible && isVisible()) {
    emit visibilityChanged(true);
  }
}

void MapWindow::handleTokenVerification(bool valid, QString message) {
  token_verified = valid;
  qWarning() << "Mapbox token verification:" << (valid ? "Success" : "Failed") << "-" << message;
  
  // Show message if token is invalid
  if (!valid) {
    map_instructions->showError(message);
  }
}

void MapWindow::searchLocation() {
  if (!localizer_valid) {
    ConfirmationDialog::alert("GPS signal required for navigation", this);
    return;
  }
  
  // Verify token first if not already verified
  if (!token_verified) {
    map_instructions->showError("Verifying Mapbox token...");
    geocoder->verifyToken();
    QTimer::singleShot(2000, [this]() {
      // Retry search after verification attempt
      if (token_verified) {
        searchLocation();
      }
    });
    return;
  }

  QString location = InputDialog::getText("Enter destination", this);
  if (!location.isEmpty()) {
    // Save the query to params for possible future use
    Params().put("Address", location.toStdString());
    
    // Show loading message
    map_instructions->showError("Searching for " + location + "...");
    
    // Log the search
    qWarning() << "Searching for location:" << location;
    
    // Use our geocoder to search for the location
    if (last_position) {
      geocoder->search(location, *last_position);
    } else {
      // If we don't have a current position, use a default
      // (this won't be as accurate but better than not searching)
      geocoder->search(location, {0, 0});
    }
  }
}

void MapWindow::handleSearchResult(bool success, QMapbox::Coordinate coordinate, QString name, QString address) {
  if (success) {
    qWarning() << "Found location:" << name << "at" << coordinate.first << "," << coordinate.second;
    map_instructions->hideIfNoError();
    
    // Show a brief success message
    QString routingMsg = "Routing to: " + name;
    map_instructions->showError(routingMsg);
    
    // Store the destination in params as JSON
    QJsonObject json;
    json["latitude"] = coordinate.first;
    json["longitude"] = coordinate.second;
    QJsonDocument doc(json);
    QString json_str = doc.toJson(QJsonDocument::Compact);
    
    qWarning() << "Storing NavDestination in JSON format:" << json_str;
    Params().put("NavDestination", json_str.toStdString());
    
    // Check if params was set successfully
    auto check_dest = coordinate_from_param("NavDestination");
    qWarning() << "Verified NavDestination is now:" << (check_dest.has_value() ? "Set" : "Not set");
    
    // Calculate route to the found location
    calculateRoute(coordinate);
    
    // Set visibility for ETA
    map_eta->setVisible(true);
  } else {
    // Show error message
    map_instructions->showError("Search failed: " + address);
    qWarning() << "Search failed:" << address;
    
    // If it could be a token issue, show a specific message and retry verification
    if (address.contains("authentication", Qt::CaseInsensitive)) {
      map_instructions->showError("API token error. Verifying token again...");
      token_verified = false;
      geocoder->verifyToken();
    }
  }
}

void MapWindow::handleSearchResults(bool success, const QVector<LocationResult> &results, const QString &query, const QString &errorMessage) {
    if (success && !results.isEmpty()) {
        // Hide loading message
        map_instructions->hideIfNoError();
        
        // Show the search results widget
        search_results_widget->setResults(results, query);
        search_results_widget->setGeometry(0, 0, width(), height());
        search_results_widget->setVisible(true);
        search_results_widget->raise();
    } else {
        // Show error message if the search failed
        map_instructions->showError(errorMessage.isEmpty() ? "Search failed" : errorMessage);
    }
}

void MapWindow::handleSearchResultSelected(const LocationResult &result) {
    // Hide the search results
    search_results_widget->setVisible(false);
    
    // Show a brief success message
    QString routingMsg = "Routing to: " + result.name;
    map_instructions->showError(routingMsg);
    
    // Store the destination in params as JSON
    QJsonObject json;
    json["latitude"] = result.coordinate.first;
    json["longitude"] = result.coordinate.second;
    json["place_name"] = result.name;
    json["place_details"] = result.address;
    QJsonDocument doc(json);
    QString json_str = doc.toJson(QJsonDocument::Compact);
    
    qWarning() << "Storing NavDestination in JSON format:" << json_str;
    Params().put("NavDestination", json_str.toStdString());
    
    // Check if params was set successfully
    auto check_dest = coordinate_from_param("NavDestination");
    qWarning() << "Verified NavDestination is now:" << (check_dest.has_value() ? "Set" : "Not set");
    
    // Calculate route to the found location
    calculateRoute(result.coordinate);
    
    // Set visibility for ETA
    map_eta->setVisible(true);
}

void MapWindow::clearRouteVisual() {
  qWarning() << "MapWindow::clearRouteVisual called - clearing visuals only, not removing NavDestination";
  // Clear the visual route but don't remove the NavDestination parameter
  if (!m_map.isNull()) {
    m_map->setLayoutProperty("navLayer", "visibility", "none");
    m_map->setPitch(MIN_PITCH);
  }
  
  // Reset segment without removing NavDestination
  segment = QGeoRouteSegment();
  nav_destination = QMapbox::Coordinate();
  
  // Hide the instructions and ETA
  map_instructions->hideIfNoError();
  map_eta->setVisible(false);
  allow_open = true;

  // Add debug check to see if NavDestination is still in params
  auto dest_after = coordinate_from_param("NavDestination");
  qWarning() << "After clearRouteVisual, NavDestination param is:" << (dest_after.has_value() ? "Still set" : "Not set");
}

// Override setVisible to add logging
void MapWindow::setVisible(bool visible) {
  qWarning() << "MapWindow::setVisible called with visible=" << visible;
  QOpenGLWidget::setVisible(visible);
  
  // If being shown, check current NavDestination
  if (visible) {
    auto dest = coordinate_from_param("NavDestination");
    qWarning() << "When showing map, NavDestination is:" << (dest.has_value() ? "Set" : "Not set");
  }
}

void MapWindow::toggleFullscreen() {
  is_fullscreen = !is_fullscreen;
  
  QWidget* parentWidget = qobject_cast<QWidget*>(parent());
  if (!parentWidget) return;
  
  if (is_fullscreen) {
    // Save original geometry before going fullscreen
    original_geometry = geometry();
    
    // Expand to full screen (covering the entire parent widget)
    setFixedSize(parentWidget->width(), parentWidget->height());
    move(0, 0);
    raise(); // Bring to front
    
    // Remove margins from map
    m_map->setMargins({0, 0, 0, 0});
  } else {
    // Restore original size and position
    setFixedSize(original_geometry.width(), original_geometry.height());
    move(original_geometry.x(), original_geometry.y());
    
    // Restore original margins
    m_map->setMargins({0, 350, 0, 50});
  }
  
  // Resize the map view to match
  m_map->resize(size() / MAP_SCALE);
  
  // Update button icon and position
  updateToggleButton();
  
  // Emit signal that fullscreen state changed
  emit fullscreenToggled(is_fullscreen);
}

void MapWindow::updateToggleButton() {
  // Update button icon based on current state
  if (is_fullscreen) {
    toggle_fullscreen_button->setIcon(QIcon("../assets/navigation/arrow_right.svg"));
  } else {
    toggle_fullscreen_button->setIcon(QIcon("../assets/navigation/arrow_left.svg"));
  }
  
  // Position the button on the left side and ensure it's visible
  const int margin = 20;
  toggle_fullscreen_button->move(margin, height() / 2 - toggle_fullscreen_button->height() / 2);
  toggle_fullscreen_button->raise(); // Make sure button stays on top
  toggle_fullscreen_button->setVisible(true);
}
