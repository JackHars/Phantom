#pragma once

#include <optional>
#include <eigen3/Eigen/Dense>
#include <QMapboxGL>
#include <QGeoCoordinate>

#include "common/transformations/coordinates.hpp"
#include "common/transformations/orientation.hpp"
#include "cereal/messaging/messaging.h"

const float METER_2_MILE = 0.000621371;
const float METER_2_FOOT = 3.28084;
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

QGeoCoordinate to_QGeoCoordinate(const QMapbox::Coordinate &in);
QMapbox::CoordinatesCollections model_to_collection(
  const cereal::LiveLocationKalman::Measurement::Reader &calibratedOrientationECEF,
  const cereal::LiveLocationKalman::Measurement::Reader &positionECEF,
  const cereal::ModelDataV2::XYZTData::Reader &line);
QMapbox::CoordinatesCollections coordinate_to_collection(QMapbox::Coordinate c);
QMapbox::CoordinatesCollections coordinate_list_to_collection(QList<QGeoCoordinate> coordinate_list);

float minimum_distance(QGeoCoordinate a, QGeoCoordinate b, QGeoCoordinate p);
std::optional<QMapbox::Coordinate> coordinate_from_param(std::string param);
float distance_along_geometry(QList<QGeoCoordinate> geometry, QGeoCoordinate pos);

// New helper functions for enhanced navigation
bool search_for_poi(const QString &category, const QGeoCoordinate &currentLocation, QList<QGeoCoordinate> &results);
QList<QGeoCoordinate> calculate_waypoint_route(const QGeoCoordinate &start, 
                                             const QGeoCoordinate &end, 
                                             const QList<QGeoCoordinate> &waypoints);
bool save_favorite_location(const QGeoCoordinate &location, const QString &name, const QString &label);
bool get_route_estimate(const QGeoCoordinate &start, const QGeoCoordinate &end, 
                       double &distance_meters, double &time_seconds,
                       bool avoid_tolls = false, bool avoid_highways = false, bool avoid_ferries = false);
