#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>
#include <QComboBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QTabWidget>
#include <QListWidget>
#include <QLineEdit>

#include "selfdrive/common/params.h"
#include "selfdrive/ui/qt/widgets/controls.h"

// POI category struct
struct POICategory {
  QString name;
  QString icon;
  QString searchTerm;
};

class MapPanel : public QWidget {
  Q_OBJECT
public:
  explicit MapPanel(QWidget* parent = nullptr);

  void navigateTo(const QJsonObject &place);
  void parseResponse(const QString &response);
  void failedResponse(const QString &response);
  void updateCurrentRoute();
  void clear();
  void searchPOI(const QString &category);
  void saveAsFavorite(const QJsonObject &place, const QString &label);
  void setRouteOptions(bool avoidTolls, bool avoidHighways, bool avoidFerries);
  void toggleMapMode(int mode); // 0=day, 1=night, 2=satellite

private:
  void showEvent(QShowEvent *event) override;
  void setupPOICategories();
  void setupRouteOptionsPanel();
  void setupMapViewPanel();
  void setupVoiceSearchButton();
  void setupWaypointsPanel();

  Params params;
  QStackedWidget *stack;
  QTabWidget *mainTabWidget;
  
  // Search & Current route widgets
  QPushButton *home_button, *work_button;
  QLabel *home_address, *work_address;
  QVBoxLayout *recent_layout;
  QWidget *current_widget;
  QWidget *current_route_container;
  QLabel *current_route_text;
  
  // POI search widgets
  QWidget *poi_panel;
  QGridLayout *poi_grid;
  QList<POICategory> poi_categories;
  
  // Route options widgets
  QWidget *route_options_panel;
  QCheckBox *avoid_tolls_checkbox;
  QCheckBox *avoid_highways_checkbox;
  QCheckBox *avoid_ferries_checkbox;
  
  // Map view control widgets
  QWidget *map_view_panel;
  QButtonGroup *map_mode_group;
  QRadioButton *day_mode_btn;
  QRadioButton *night_mode_btn;
  QRadioButton *satellite_mode_btn;
  QCheckBox *traffic_checkbox;
  QCheckBox *show_poi_checkbox;
  
  // Voice search
  QPushButton *voice_search_btn;
  
  // Waypoints
  QWidget *waypoints_panel;
  QListWidget *waypoints_list;
  QLineEdit *waypoint_input;

signals:
  void closeSettings();
  void openMapSearch();
  void poiSelected(QString category);
  void routeOptionsChanged(bool avoidTolls, bool avoidHighways, bool avoidFerries);
  void mapModeChanged(int mode);
  void trafficToggled(bool enabled);
  void poiVisibilityToggled(bool visible);
  void addWaypoint(QString address);
  void clearWaypoints();
  void voiceSearchRequested();
};
