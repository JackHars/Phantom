#include "selfdrive/ui/qt/maps/map_helpers.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "selfdrive/common/params.h"


QGeoCoordinate to_QGeoCoordinate(const QMapbox::Coordinate &in) {
  return QGeoCoordinate(in.first, in.second);
}

QMapbox::CoordinatesCollections model_to_collection(
  const cereal::LiveLocationKalman::Measurement::Reader &calibratedOrientationECEF,
  const cereal::LiveLocationKalman::Measurement::Reader &positionECEF,
  const cereal::ModelDataV2::XYZTData::Reader &line){

  Eigen::Vector3d ecef(positionECEF.getValue()[0], positionECEF.getValue()[1], positionECEF.getValue()[2]);
  Eigen::Vector3d orient(calibratedOrientationECEF.getValue()[0], calibratedOrientationECEF.getValue()[1], calibratedOrientationECEF.getValue()[2]);
  Eigen::Matrix3d ecef_from_local = euler2rot(orient);

  QMapbox::Coordinates coordinates;
  auto x = line.getX();
  auto y = line.getY();
  auto z = line.getZ();
  for (int i = 0; i < x.size(); i++) {
    Eigen::Vector3d point_ecef = ecef_from_local * Eigen::Vector3d(x[i], y[i], z[i]) + ecef;
    Geodetic point_geodetic = ecef2geodetic((ECEF){.x = point_ecef[0], .y = point_ecef[1], .z = point_ecef[2]});
    QMapbox::Coordinate coordinate(point_geodetic.lat, point_geodetic.lon);
    coordinates.push_back(coordinate);
  }

  QMapbox::CoordinatesCollection collection;
  collection.push_back(coordinates);

  QMapbox::CoordinatesCollections collections;
  collections.push_back(collection);
  return collections;
}

QMapbox::CoordinatesCollections coordinate_to_collection(QMapbox::Coordinate c) {
  QMapbox::Coordinates coordinates;
  coordinates.push_back(c);

  QMapbox::CoordinatesCollection collection;
  collection.push_back(coordinates);

  QMapbox::CoordinatesCollections collections;
  collections.push_back(collection);
  return collections;
}

QMapbox::CoordinatesCollections coordinate_list_to_collection(QList<QGeoCoordinate> coordinate_list) {
  QMapbox::Coordinates coordinates;

  for (auto &c : coordinate_list) {
    QMapbox::Coordinate coordinate(c.latitude(), c.longitude());
    coordinates.push_back(coordinate);
  }

  QMapbox::CoordinatesCollection collection;
  collection.push_back(coordinates);

  QMapbox::CoordinatesCollections collections;
  collections.push_back(collection);
  return collections;
}

static QGeoCoordinate sub(QGeoCoordinate v, QGeoCoordinate w) {
  return QGeoCoordinate(v.latitude() - w.latitude(), v.longitude() - w.longitude());
}

static QGeoCoordinate add(QGeoCoordinate v, QGeoCoordinate w) {
  return QGeoCoordinate(v.latitude() + w.latitude(), v.longitude() + w.longitude());
}

static QGeoCoordinate mul(QGeoCoordinate v, float c) {
  return QGeoCoordinate(c * v.latitude(), c * v.longitude());
}

static float dot(QGeoCoordinate v, QGeoCoordinate w) {
  return v.latitude() * w.latitude() + v.longitude() * w.longitude();
}

float minimum_distance(QGeoCoordinate a, QGeoCoordinate b, QGeoCoordinate p) {
  const QGeoCoordinate ap = sub(p, a);
  const QGeoCoordinate ab = sub(b, a);
  const float t = std::clamp(dot(ap, ab) / dot(ab, ab), 0.0f, 1.0f);
  const QGeoCoordinate projection = add(a, mul(ab, t));
  return projection.distanceTo(p);
}

float distance_along_geometry(QList<QGeoCoordinate> geometry, QGeoCoordinate pos) {
  if (geometry.size() <= 2) {
    return geometry[0].distanceTo(pos);
  }

  // 1. Find segment that is closest to current position
  // 2. Total distance is sum of distance to start of closest segment
  //    + all previous segments
  double total_distance = 0;
  double total_distance_closest = 0;
  double closest_distance = std::numeric_limits<double>::max();

  for (int i = 0; i < geometry.size() - 1; i++) {
    double d = minimum_distance(geometry[i], geometry[i+1], pos);
    if (d < closest_distance) {
      closest_distance = d;
      total_distance_closest = total_distance + geometry[i].distanceTo(pos);
    }
    total_distance += geometry[i].distanceTo(geometry[i+1]);
  }

  return total_distance_closest;
}

std::optional<QMapbox::Coordinate> coordinate_from_param(std::string param) {
  QString json_str = QString::fromStdString(Params().get(param));
  if (json_str.isEmpty()) return {};

  QJsonDocument doc = QJsonDocument::fromJson(json_str.toUtf8());
  if (doc.isNull()) return {};

  QJsonObject json = doc.object();
  if (json["latitude"].isDouble() && json["longitude"].isDouble()) {
    QMapbox::Coordinate coord(json["latitude"].toDouble(), json["longitude"].toDouble());
    return coord;
  } else {
    return {};
  }
}

// Add new helper functions for POI search
bool search_for_poi(const QString &category, const QGeoCoordinate &currentLocation, QList<QGeoCoordinate> &results) {
  // In a real implementation, this would query a mapping service for POIs
  // For now, this is just a stub that simulates finding POIs
  
  // Simulate finding 3 POIs by slightly offsetting the current location
  for (int i = 0; i < 3; i++) {
    double lat_offset = (i * 0.001) + (rand() % 100) * 0.0001;
    double lon_offset = (i * 0.001) + (rand() % 100) * 0.0001;
    
    QGeoCoordinate poi(
      currentLocation.latitude() + lat_offset,
      currentLocation.longitude() + lon_offset
    );
    
    results.append(poi);
  }
  
  return !results.isEmpty();
}

// Calculate a route with waypoints
QList<QGeoCoordinate> calculate_waypoint_route(const QGeoCoordinate &start, 
                                              const QGeoCoordinate &end, 
                                              const QList<QGeoCoordinate> &waypoints) {
  QList<QGeoCoordinate> route;
  
  // Add starting point
  route.append(start);
  
  // Add all waypoints in order
  for (const auto &waypoint : waypoints) {
    route.append(waypoint);
  }
  
  // Add destination
  route.append(end);
  
  return route;
}

// Helper for saving favorite locations
bool save_favorite_location(const QGeoCoordinate &location, const QString &name, const QString &label) {
  // In a real implementation, this would save to a database or remote service
  // For now, just return success
  return true;
}

// Get distance and time estimate between two points
bool get_route_estimate(const QGeoCoordinate &start, const QGeoCoordinate &end, 
                       double &distance_meters, double &time_seconds,
                       bool avoid_tolls, bool avoid_highways, bool avoid_ferries) {
  // In a real implementation, this would call a routing service API
  // For now, calculate a simple straight-line distance and estimate time
  
  distance_meters = start.distanceTo(end);
  
  // Assume average speed of 60 km/h (16.7 m/s)
  double avg_speed_mps = 16.7;
  
  // Adjust for route options (in a real implementation, this would be more sophisticated)
  if (avoid_highways) {
    avg_speed_mps = 12.0; // Slower if avoiding highways
  }
  
  time_seconds = distance_meters / avg_speed_mps;
  
  // Add some randomness to make it more realistic
  time_seconds *= (0.9 + ((rand() % 20) / 100.0));
  
  return true;
}
