#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QGeoCoordinate>
#include <QMapboxGL>
#include <QVector>

// Structure to hold location search results
struct LocationResult {
    QMapbox::Coordinate coordinate;
    QString name;
    QString address;
    double distance; // Distance from current location (for sorting)
    QString type; // POI, address, etc.
    double relevance; // Relevance score from Mapbox

    LocationResult() : coordinate({0, 0}), distance(0), relevance(0) {}
    
    LocationResult(QMapbox::Coordinate coord, QString n, QString addr, double dist = 0, 
                  QString t = "", double rel = 0) 
        : coordinate(coord), name(n), address(addr), distance(dist), type(t), relevance(rel) {}
};

class Geocoder : public QObject {
    Q_OBJECT

public:
    explicit Geocoder(QObject *parent = nullptr);
    void search(const QString &query, const QMapbox::Coordinate &near);
    void verifyToken();

signals:
    // Original signal for backward compatibility
    void searchResult(bool success, QMapbox::Coordinate coordinate, QString name, QString address);
    
    // New signal that emits multiple results
    void searchResults(bool success, const QVector<LocationResult> &results, const QString &query, const QString &errorMessage = "");
    
    void tokenVerified(bool valid, QString message);

private slots:
    void handleSearchResponse(QNetworkReply *reply);
    void handleTokenVerification(QNetworkReply *reply);

private:
    QNetworkAccessManager *network;
    QString m_lastQuery;
    QMapbox::Coordinate m_lastLocation;
    
    // Helper to calculate distance between coordinates
    double calculateDistance(const QMapbox::Coordinate &p1, const QMapbox::Coordinate &p2);
}; 