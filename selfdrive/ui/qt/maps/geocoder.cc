#include "geocoder.h"
#include "selfdrive/common/params.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <cmath> // For M_PI

Geocoder::Geocoder(QObject *parent) : QObject(parent) {
    network = new QNetworkAccessManager(this);
    connect(network, &QNetworkAccessManager::finished, this, &Geocoder::handleSearchResponse);
}

void Geocoder::verifyToken() {
    // Get Mapbox token from params
    std::string mapbox_token = Params().get("MapboxToken");
    if (mapbox_token.empty()) {
        emit tokenVerified(false, "No Mapbox token found. Please add your token to continue.");
        return;
    }

    // Trim any whitespace from the token
    QString token = QString::fromStdString(mapbox_token).trimmed();
    
    // Log the token length (without revealing the token itself)
    qWarning() << "Verifying token. Length:" << token.length();
    
    // Create a simple request to verify the token
    // We'll use the static geocoding API with a known location
    QUrl url("https://api.mapbox.com/geocoding/v5/mapbox.places/san%20francisco.json");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", token);
    url.setQuery(urlQuery);
    
    qWarning() << "Token verification URL (without token):" << url.toString().replace(token, "TOKEN_HIDDEN");

    // Make the request
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "OpenpilotGeocoder/1.0");
    
    // Use a different handler for token verification
    QNetworkReply *reply = network->get(request);
    disconnect(network, &QNetworkAccessManager::finished, this, &Geocoder::handleSearchResponse);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        handleTokenVerification(reply);
    });
}

void Geocoder::handleTokenVerification(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Token verification failed:" << reply->errorString();
        qWarning() << "Response status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        // Log headers and response for debugging
        QList<QByteArray> headerList = reply->rawHeaderList();
        foreach(QByteArray header, headerList) {
            qWarning() << "Header:" << header << ":" << reply->rawHeader(header);
        }
        
        QByteArray errorData = reply->readAll();
        if (!errorData.isEmpty()) {
            qWarning() << "Error response body:" << errorData;
            
            // Try to parse as JSON to get more detailed error
            QJsonDocument errorDoc = QJsonDocument::fromJson(errorData);
            if (!errorDoc.isNull() && errorDoc.isObject()) {
                QJsonObject errorObj = errorDoc.object();
                if (errorObj.contains("message")) {
                    QString message = errorObj["message"].toString();
                    emit tokenVerified(false, "Token error: " + message);
                    reply->deleteLater();
                    
                    // Reconnect the handler for normal searches
                    connect(network, &QNetworkAccessManager::finished, this, &Geocoder::handleSearchResponse);
                    return;
                }
            }
        }
        
        emit tokenVerified(false, "Token verification failed: " + reply->errorString());
    } else {
        // Parse successful response
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        if (!obj.contains("features") || obj["features"].toArray().isEmpty()) {
            emit tokenVerified(false, "Token verification failed: Unexpected API response");
        } else {
            emit tokenVerified(true, "Mapbox token is valid");
        }
    }
    
    reply->deleteLater();
    
    // Reconnect the handler for normal searches
    connect(network, &QNetworkAccessManager::finished, this, &Geocoder::handleSearchResponse);
}

void Geocoder::search(const QString &query, const QMapbox::Coordinate &near) {
    // Store the query and location for error handling and distance calculation
    m_lastQuery = query;
    m_lastLocation = near;

    // Get Mapbox token from params
    std::string mapbox_token = Params().get("MapboxToken");
    if (mapbox_token.empty()) {
        emit searchResult(false, {0, 0}, "", "No Mapbox token found. Please add your token to continue.");
        emit searchResults(false, {}, query, "No Mapbox token found. Please add your token to continue.");
        return;
    }

    // Trim any whitespace from the token
    QString token = QString::fromStdString(mapbox_token).trimmed();
    
    // Log the token length (without revealing the token itself)
    qWarning() << "Token length:" << token.length();
    
    // Build the request URL for the Mapbox Geocoding API
    // URL encode the query to handle special characters
    QString searchQuery = query;
    
    // For common restaurant chains, add "restaurant" to improve results
    // This helps with POI search accuracy
    if (searchQuery.toLower().contains("mcdonald") && !searchQuery.toLower().contains("restaurant")) {
        // Directly search for McDonald's restaurants - be very specific
        searchQuery = "McDonald's Fast Food Restaurant";
    }
    
    QString encodedQuery = QUrl::toPercentEncoding(searchQuery);
    QUrl url("https://api.mapbox.com/geocoding/v5/mapbox.places/" + encodedQuery + ".json");
    QUrlQuery urlQuery;
    
    // Add proximity parameter if we have current location
    if (near.first != 0 || near.second != 0) {
        urlQuery.addQueryItem("proximity", QString("%1,%2").arg(near.second).arg(near.first));
    }
    
    // Add other required parameters
    urlQuery.addQueryItem("access_token", token);
    urlQuery.addQueryItem("limit", "10"); // Increase limit to get more results for the dropdown
    urlQuery.addQueryItem("types", "poi,place,address");
    
    // Add country bias for US if searching from US coordinates
    if (near.first > 24 && near.first < 50 && near.second > -130 && near.second < -65) {
        urlQuery.addQueryItem("country", "us");
    }
    
    // Add bbox parameter to limit results to an area around current location
    // This creates a bounding box of approximately 50km around the current location
    if (near.first != 0 || near.second != 0) {
        double lat_offset = 0.5; // Roughly 50km 
        double lng_offset = 0.5 / cos(near.first * M_PI / 180.0); // Adjust for latitude
        QString bbox = QString("%1,%2,%3,%4")
                           .arg(near.second - lng_offset)
                           .arg(near.first - lat_offset)
                           .arg(near.second + lng_offset)
                           .arg(near.first + lat_offset);
        urlQuery.addQueryItem("bbox", bbox);
    }
    
    url.setQuery(urlQuery);
    
    qWarning() << "Geocoding URL (without token):" << url.toString().replace(token, "TOKEN_HIDDEN");

    // Make the request
    QNetworkRequest request(url);
    
    // Add headers that might be required
    request.setHeader(QNetworkRequest::UserAgentHeader, "OpenpilotGeocoder/1.0");
    request.setRawHeader("Accept", "application/json");
    
    network->get(request);
}

// Calculate distance between two coordinates in kilometers using Haversine formula
double Geocoder::calculateDistance(const QMapbox::Coordinate &p1, const QMapbox::Coordinate &p2) {
    const double earthRadius = 6371.0; // Earth radius in kilometers
    double lat1 = p1.first * M_PI / 180.0;
    double lon1 = p1.second * M_PI / 180.0;
    double lat2 = p2.first * M_PI / 180.0;
    double lon2 = p2.second * M_PI / 180.0;
    
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    
    double a = sin(dLat/2) * sin(dLat/2) + 
               cos(lat1) * cos(lat2) * 
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    
    return earthRadius * c;
}

void Geocoder::handleSearchResponse(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Geocoding error:" << reply->errorString();
        qWarning() << "Response status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        // Log response headers for debugging
        QList<QByteArray> headerList = reply->rawHeaderList();
        foreach(QByteArray header, headerList) {
            qWarning() << "Header:" << header << ":" << reply->rawHeader(header);
        }
        
        // Try to read any response body that might contain more error details
        QByteArray errorData = reply->readAll();
        if (!errorData.isEmpty()) {
            qWarning() << "Error response body:" << errorData;
        }
        
        emit searchResult(false, {0, 0}, m_lastQuery, "Error: " + reply->errorString());
        emit searchResults(false, {}, m_lastQuery, "Error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    // Parse the JSON response
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    
    // Check for features in the response
    if (!obj.contains("features") || obj["features"].toArray().isEmpty()) {
        emit searchResult(false, {0, 0}, m_lastQuery, "No results found for: " + m_lastQuery);
        emit searchResults(false, {}, m_lastQuery, "No results found for: " + m_lastQuery);
        reply->deleteLater();
        return;
    }

    // Get the features array
    QJsonArray features = obj["features"].toArray();
    
    // Create vector to store all results
    QVector<LocationResult> results;
    
    // Process all features into location results
    for (int i = 0; i < features.size(); i++) {
        QJsonObject feature = features[i].toObject();
        
        // Get basic properties
        QString name = feature["text"].toString();
        QString placeName = feature["place_name"].toString();
        
        // Get place type if available
        QString type = "";
        if (feature.contains("place_type") && feature["place_type"].isArray()) {
            type = feature["place_type"].toArray()[0].toString();
        }
        
        // Get coordinates
        QJsonArray coords = feature["center"].toArray();
        if (coords.size() < 2) {
            continue; // Skip invalid coordinates
        }
        
        // Mapbox returns [longitude, latitude]
        double lng = coords[0].toDouble();
        double lat = coords[1].toDouble();
        QMapbox::Coordinate coordinate = {lat, lng};
        
        // Calculate distance from current location if available
        double distance = 0;
        if (m_lastLocation.first != 0 || m_lastLocation.second != 0) {
            distance = calculateDistance(m_lastLocation, coordinate);
        }
        
        // Get relevance score
        double relevance = feature["relevance"].toDouble();
        
        // Create and add the result
        LocationResult result(coordinate, name, placeName, distance, type, relevance);
        results.append(result);
        
        qWarning() << "Result" << i << ":" << name << "(" << placeName << ")" 
                  << "at" << lat << "," << lng 
                  << "distance:" << distance << "km"
                  << "type:" << type
                  << "relevance:" << relevance;
    }
    
    // Sort results by distance if we have a current location
    if (m_lastLocation.first != 0 || m_lastLocation.second != 0) {
        std::sort(results.begin(), results.end(), [](const LocationResult &a, const LocationResult &b) {
            return a.distance < b.distance;
        });
    } else {
        // Otherwise sort by relevance
        std::sort(results.begin(), results.end(), [](const LocationResult &a, const LocationResult &b) {
            return a.relevance > b.relevance;
        });
    }
    
    // Emit the full list of results
    emit searchResults(true, results, m_lastQuery);
    
    // For backward compatibility, also emit the first/best result
    if (!results.isEmpty()) {
        const LocationResult &bestResult = results.first();
        emit searchResult(true, bestResult.coordinate, bestResult.name, bestResult.address);
    }
    
    reply->deleteLater();
} 