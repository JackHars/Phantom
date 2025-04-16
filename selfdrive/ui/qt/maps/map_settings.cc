#include "map_settings.h"

#include <QDebug>
#include <QGridLayout>
#include <QTabWidget>

#include "selfdrive/common/util.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/request_repeater.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"

static QString shorten(const QString &str, int max_len) {
  return str.size() > max_len ? str.left(max_len).trimmed() + "…" : str;
}

MapPanel::MapPanel(QWidget* parent) : QWidget(parent) {
  stack = new QStackedWidget;

  QWidget *main_widget = new QWidget;
  QVBoxLayout *main_layout = new QVBoxLayout(main_widget);
  const int icon_size = 200;

  // Create tab widget for better organization
  mainTabWidget = new QTabWidget();
  mainTabWidget->setStyleSheet(R"(
    QTabWidget::pane {
      border: 0;
      background-color: #292929;
    }
    QTabBar::tab {
      background-color: #292929;
      color: #CCCCCC;
      padding: 15px 40px;
      border-top-left-radius: 15px;
      border-top-right-radius: 15px;
      font-size: 45px;
    }
    QTabBar::tab:selected {
      background-color: #404040;
      color: white;
    }
  )");

  // Create tab for destination search
  QWidget *destinationTab = new QWidget();
  QVBoxLayout *destinationLayout = new QVBoxLayout(destinationTab);
  
  // Add destination button - more prominently placed at the top
  QWidget *add_dest_container = new QWidget;
  QHBoxLayout *add_dest_layout = new QHBoxLayout(add_dest_container);
  add_dest_layout->setContentsMargins(40, 20, 40, 20);
  
  QPushButton *add_dest_button = new QPushButton("Add Destination");
  add_dest_button->setStyleSheet(R"(
    QPushButton {
      padding: 20px;
      border-radius: 10px;
      font-size: 55px;
      font-weight: 500;
      background-color: #364DEF;
    }
    QPushButton:pressed {
      background-color: #304AD0;
    }
  )");
  QObject::connect(add_dest_button, &QPushButton::clicked, [=]() {
    QString destination = InputDialog::getText("Enter your destination", this);
    if (!destination.isEmpty()) {
      params.put("Address", destination.toStdString());
      system("python3 /data/openpilot/selfdrive/ui/qt/maps/set_destination.py");
      updateCurrentRoute();
    }
  });
  
  // Set up voice search button
  setupVoiceSearchButton();
  
  QPushButton *map_search_button = new QPushButton();
  map_search_button->setIcon(QIcon("../assets/navigation/map_search.png"));
  map_search_button->setIconSize(QSize(64, 64));
  map_search_button->setStyleSheet(R"(
    QPushButton {
      padding: 20px;
      border-radius: 10px;
      background-color: #4D4D4D;
    }
    QPushButton:pressed {
      background-color: #404040;
    }
  )");
  map_search_button->setFixedSize(120, 120);
  QObject::connect(map_search_button, &QPushButton::clicked, [=]() {
    emit openMapSearch();
  });
  
  add_dest_layout->addWidget(add_dest_button);
  add_dest_layout->addSpacing(20);
  add_dest_layout->addWidget(voice_search_btn);
  add_dest_layout->addSpacing(20);
  add_dest_layout->addWidget(map_search_button);
  destinationLayout->addWidget(add_dest_container);
  destinationLayout->addSpacing(20);
  destinationLayout->addWidget(horizontal_line());
  destinationLayout->addSpacing(20);

  // Home
  QHBoxLayout *home_layout = new QHBoxLayout;
  home_button = new QPushButton;
  home_button->setIconSize(QSize(icon_size, icon_size));
  home_layout->addWidget(home_button);

  home_address = new QLabel;
  home_address->setWordWrap(true);
  home_layout->addSpacing(30);
  home_layout->addWidget(home_address);
  home_layout->addStretch();

  // Work
  QHBoxLayout *work_layout = new QHBoxLayout;
  work_button = new QPushButton;
  work_button->setIconSize(QSize(icon_size, icon_size));
  work_layout->addWidget(work_button);

  work_address = new QLabel;
  work_address->setWordWrap(true);
  work_layout->addSpacing(30);
  work_layout->addWidget(work_address);
  work_layout->addStretch();

  // Home & Work layout
  QHBoxLayout *home_work_layout = new QHBoxLayout;
  home_work_layout->addLayout(home_layout, 1);
  home_work_layout->addSpacing(50);
  home_work_layout->addLayout(work_layout, 1);

  destinationLayout->addLayout(home_work_layout);
  destinationLayout->addSpacing(20);
  destinationLayout->addWidget(horizontal_line());
  destinationLayout->addSpacing(20);

  // Current route
  {
    current_widget = new QWidget(this);
    QVBoxLayout *current_layout = new QVBoxLayout(current_widget);

    QLabel *title = new QLabel("Current Destination");
    title->setStyleSheet("font-size: 55px; font-weight: 500; margin-bottom: 10px;");
    current_layout->addWidget(title);

    QWidget *dest_container = new QWidget();
    QHBoxLayout *dest_layout = new QHBoxLayout(dest_container);
    dest_layout->setContentsMargins(20, 10, 20, 10);
    
    QLabel *dest_icon = new QLabel();
    dest_icon->setPixmap(QPixmap("../assets/navigation/destination.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    dest_layout->addWidget(dest_icon);
    dest_layout->addSpacing(20);
    
    QLabel *dest_text = new QLabel();
    dest_text->setWordWrap(true);
    dest_text->setStyleSheet("font-size: 45px; font-weight: 400;");
    dest_layout->addWidget(dest_text, 1);
    
    QPushButton *navigate_btn = new QPushButton("GO");
    navigate_btn->setStyleSheet(R"(
      QPushButton {
        padding: 15px 30px;
        border-radius: 8px;
        font-size: 45px;
        font-weight: 500;
        background-color: #25D366;
      }
      QPushButton:pressed {
        background-color: #20B355;
      }
    )");
    QObject::connect(navigate_btn, &QPushButton::clicked, [=]() {
      emit closeSettings();
    });
    dest_layout->addWidget(navigate_btn);
    
    QPushButton *clear_btn = new QPushButton("×");
    clear_btn->setStyleSheet(R"(
      QPushButton {
        padding: 15px;
        border-radius: 25px;
        font-size: 45px;
        background-color: #FF5A5A;
      }
      QPushButton:pressed {
        background-color: #D44A4A;
      }
    )");
    clear_btn->setFixedSize(60, 60);
    QObject::connect(clear_btn, &QPushButton::clicked, [=]() {
      params.remove("NavDestination");
      updateCurrentRoute();
    });
    dest_layout->addWidget(clear_btn);
    
    current_layout->addWidget(dest_container);
    
    // Store references to update later
    current_route_container = dest_container;
    current_route_text = dest_text;

    current_layout->addSpacing(10);
    current_layout->addWidget(horizontal_line());
    current_layout->addSpacing(20);
  }
  destinationLayout->addWidget(current_widget);

  // Recents
  QLabel *recents_title = new QLabel("Recent Destinations");
  recents_title->setStyleSheet("font-size: 55px");
  destinationLayout->addWidget(recents_title);
  destinationLayout->addSpacing(20);

  recent_layout = new QVBoxLayout;
  QWidget *recent_widget = new LayoutWidget(recent_layout, this);
  ScrollView *recent_scroller = new ScrollView(recent_widget, this);
  destinationLayout->addWidget(recent_scroller);
  
  // Add the destination tab to the main tab widget
  mainTabWidget->addTab(destinationTab, "Destinations");
  
  // Create and add other tabs
  setupPOICategories();
  setupRouteOptionsPanel();
  setupMapViewPanel();
  setupWaypointsPanel();
  
  // Add POI tab
  mainTabWidget->addTab(poi_panel, "Find Places");
  
  // Add route options tab
  mainTabWidget->addTab(route_options_panel, "Route Options");
  
  // Add map view tab
  mainTabWidget->addTab(map_view_panel, "Map Settings");
  
  // Add waypoints tab
  mainTabWidget->addTab(waypoints_panel, "Waypoints");
  
  // Add tab widget to main layout
  main_layout->addWidget(mainTabWidget);

  // No prime upsell
  QWidget * no_prime_widget = new QWidget;
  {
    QVBoxLayout *no_prime_layout = new QVBoxLayout(no_prime_widget);
    QLabel *signup_header = new QLabel("Try the Navigation Beta");
    signup_header->setStyleSheet(R"(font-size: 75px; color: white; font-weight:600;)");
    signup_header->setAlignment(Qt::AlignCenter);

    no_prime_layout->addWidget(signup_header);
    no_prime_layout->addSpacing(50);

    QLabel *screenshot = new QLabel;
    QPixmap pm = QPixmap("../assets/navigation/screenshot.png");
    screenshot->setPixmap(pm.scaledToWidth(vwp_w * 0.5, Qt::SmoothTransformation));
    no_prime_layout->addWidget(screenshot, 0, Qt::AlignHCenter);

    QLabel *signup = new QLabel("Get turn-by-turn directions displayed and more with a comma \nprime subscription. Sign up now: https://connect.comma.ai");
    signup->setStyleSheet(R"(font-size: 45px; color: white; font-weight:300;)");
    signup->setAlignment(Qt::AlignCenter);

    no_prime_layout->addSpacing(20);
    no_prime_layout->addWidget(signup);
    no_prime_layout->addStretch();
  }

  stack->addWidget(main_widget);
  stack->addWidget(no_prime_widget);
  stack->setCurrentIndex(0);

  QVBoxLayout *wrapper = new QVBoxLayout(this);
  wrapper->addWidget(stack);

  clear();

  if (auto dongle_id = getDongleId()) {
    // Fetch favorite and recent locations
    {
      QString url = CommaApi::BASE_URL + "/v1/navigation/" + *dongle_id + "/locations";
      RequestRepeater* repeater = new RequestRepeater(this, url, "ApiCache_NavDestinations", 30, true);
      QObject::connect(repeater, &RequestRepeater::receivedResponse, this, &MapPanel::parseResponse);
      QObject::connect(repeater, &RequestRepeater::failedResponse, this, &MapPanel::failedResponse);
    }

    // Destination set while offline
    {
      QString url = CommaApi::BASE_URL + "/v1/navigation/" + *dongle_id + "/next";
      RequestRepeater* repeater = new RequestRepeater(this, url, "", 10, true);
      HttpRequest* deleter = new HttpRequest(this);

      QObject::connect(repeater, &RequestRepeater::receivedResponse, [=](QString resp) {
        auto params = Params();
        if (resp != "null") {
          if (params.get("NavDestination").empty()) {
            qWarning() << "Setting NavDestination from /next" << resp;
            params.put("NavDestination", resp.toStdString());
          } else {
            qWarning() << "Got location from /next, but NavDestination already set";
          }

          // Send DELETE to clear destination server side
          deleter->sendRequest(url, HttpRequest::Method::DELETE);
        }
      });
    }
  }
}

void MapPanel::showEvent(QShowEvent *event) {
  updateCurrentRoute();
}

void MapPanel::clear() {
  home_button->setIcon(QPixmap("../assets/navigation/home_inactive.png"));
  home_address->setStyleSheet(R"(font-size: 50px; color: grey;)");
  home_address->setText("No home\nlocation set");
  home_button->disconnect();

  work_button->setIcon(QPixmap("../assets/navigation/work_inactive.png"));
  work_address->setStyleSheet(R"(font-size: 50px; color: grey;)");
  work_address->setText("No work\nlocation set");
  work_button->disconnect();

  clearLayout(recent_layout);
}

void MapPanel::updateCurrentRoute() {
  auto dest = QString::fromStdString(params.get("NavDestination"));
  QJsonDocument doc = QJsonDocument::fromJson(dest.trimmed().toUtf8());
  if (dest.size() && !doc.isNull()) {
    auto name = doc["place_name"].toString();
    auto details = doc["place_details"].toString();
    current_route_text->setText(shorten(name + " " + details, 42));
  }
  current_widget->setVisible(dest.size() && !doc.isNull());
}

void MapPanel::parseResponse(const QString &response) {
  QJsonDocument doc = QJsonDocument::fromJson(response.trimmed().toUtf8());
  if (doc.isNull()) {
    qDebug() << "JSON Parse failed on navigation locations";
    return;
  }

  clear();

  bool has_recents = false;
  for (auto &save_type: {"favorite", "recent"}) {
    for (auto location : doc.array()) {
      auto obj = location.toObject();

      auto type = obj["save_type"].toString();
      auto label = obj["label"].toString();
      auto name = obj["place_name"].toString();
      auto details = obj["place_details"].toString();

      if (type != save_type) continue;

      if (type == "favorite" && label == "home") {
        home_address->setText(name);
        home_address->setStyleSheet(R"(font-size: 50px; color: white;)");
        home_button->setIcon(QPixmap("../assets/navigation/home.png"));
        QObject::connect(home_button, &QPushButton::clicked, [=]() {
          navigateTo(obj);
          emit closeSettings();
        });
      } else if (type == "favorite" && label == "work") {
        work_address->setText(name);
        work_address->setStyleSheet(R"(font-size: 50px; color: white;)");
        work_button->setIcon(QPixmap("../assets/navigation/work.png"));
        QObject::connect(work_button, &QPushButton::clicked, [=]() {
          navigateTo(obj);
          emit closeSettings();
        });
      } else {
        ClickableWidget *widget = new ClickableWidget;
        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(15, 14, 40, 14);

        QLabel *star = new QLabel("★");
        auto sp = star->sizePolicy();
        sp.setRetainSizeWhenHidden(true);
        star->setSizePolicy(sp);

        star->setVisible(type == "favorite");
        star->setStyleSheet(R"(font-size: 60px;)");
        layout->addWidget(star);
        layout->addSpacing(10);


        QLabel *recent_label = new QLabel(shorten(name + " " + details, 45));
        recent_label->setStyleSheet(R"(font-size: 50px;)");

        layout->addWidget(recent_label);
        layout->addStretch();

        QLabel *arrow = new QLabel("→");
        arrow->setStyleSheet(R"(font-size: 60px;)");
        layout->addWidget(arrow);

        widget->setStyleSheet(R"(
          .ClickableWidget {
            border-radius: 10px;
            border-width: 1px;
            border-style: solid;
            border-color: gray;
          }
          QWidget {
            background-color: #393939;
            color: #9c9c9c;
          }
        )");

        QObject::connect(widget, &ClickableWidget::clicked, [=]() {
          navigateTo(obj);
          emit closeSettings();
        });

        recent_layout->addWidget(widget);
        recent_layout->addSpacing(10);
        has_recents = true;
      }
    }

  }

  if (!has_recents) {
    QLabel *no_recents = new QLabel("no recent destinations");
    no_recents->setStyleSheet(R"(font-size: 50px; color: #9c9c9c)");
    recent_layout->addWidget(no_recents);
  }

  recent_layout->addStretch();
  stack->setCurrentIndex(0);
  repaint();
}

void MapPanel::failedResponse(const QString &response) {
  stack->setCurrentIndex(1);
}

void MapPanel::navigateTo(const QJsonObject &place) {
  QJsonDocument doc(place);
  params.put("NavDestination", doc.toJson().toStdString());
}

// Setup the POI categories panel
void MapPanel::setupPOICategories() {
  poi_panel = new QWidget();
  QVBoxLayout *poi_layout = new QVBoxLayout(poi_panel);
  
  QLabel *poi_title = new QLabel("Find Places Nearby");
  poi_title->setStyleSheet("font-size: 55px; font-weight: 500; margin-bottom: 20px;");
  poi_layout->addWidget(poi_title);
  
  // Define POI categories
  poi_categories = {
    {"Gas Stations", "../assets/navigation/poi_gas.png", "gas station"},
    {"Restaurants", "../assets/navigation/poi_food.png", "restaurant"},
    {"Parking", "../assets/navigation/poi_parking.png", "parking"},
    {"Hotels", "../assets/navigation/poi_hotel.png", "hotel"},
    {"Hospitals", "../assets/navigation/poi_hospital.png", "hospital"},
    {"ATMs", "../assets/navigation/poi_atm.png", "atm"},
    {"Coffee", "../assets/navigation/poi_coffee.png", "coffee shop"},
    {"Supermarkets", "../assets/navigation/poi_grocery.png", "supermarket"},
    {"Shopping", "../assets/navigation/poi_shopping.png", "shopping center"}
  };
  
  // Create a grid layout for POI buttons
  poi_grid = new QGridLayout();
  poi_grid->setSpacing(20);
  
  // Create a button for each POI category
  for (int i = 0; i < poi_categories.size(); i++) {
    const auto &category = poi_categories[i];
    
    QPushButton *poi_btn = new QPushButton(category.name);
    poi_btn->setIcon(QIcon(category.icon));
    poi_btn->setIconSize(QSize(80, 80));
    poi_btn->setStyleSheet(R"(
      QPushButton {
        padding: 20px;
        border-radius: 10px;
        font-size: 40px;
        font-weight: 400;
        background-color: #393939;
        text-align: left;
      }
      QPushButton:pressed {
        background-color: #454545;
      }
    )");
    poi_btn->setMinimumHeight(150);
    
    QObject::connect(poi_btn, &QPushButton::clicked, [=]() {
      searchPOI(category.searchTerm);
      emit poiSelected(category.searchTerm);
    });
    
    poi_grid->addWidget(poi_btn, i/3, i%3);
  }
  
  poi_layout->addLayout(poi_grid);
  
  // Add a custom search field
  QHBoxLayout *custom_poi_layout = new QHBoxLayout();
  QLineEdit *custom_poi_input = new QLineEdit();
  custom_poi_input->setPlaceholderText("Search for other places...");
  custom_poi_input->setStyleSheet(R"(
    QLineEdit {
      padding: 15px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #393939;
    }
  )");
  
  QPushButton *search_poi_btn = new QPushButton("Search");
  search_poi_btn->setStyleSheet(R"(
    QPushButton {
      padding: 15px 30px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #364DEF;
    }
    QPushButton:pressed {
      background-color: #304AD0;
    }
  )");
  
  QObject::connect(search_poi_btn, &QPushButton::clicked, [=]() {
    QString searchTerm = custom_poi_input->text().trimmed();
    if (!searchTerm.isEmpty()) {
      searchPOI(searchTerm);
      emit poiSelected(searchTerm);
    }
  });
  
  custom_poi_layout->addWidget(custom_poi_input);
  custom_poi_layout->addWidget(search_poi_btn);
  
  poi_layout->addSpacing(30);
  poi_layout->addLayout(custom_poi_layout);
  poi_layout->addStretch();
}

// Setup route options panel
void MapPanel::setupRouteOptionsPanel() {
  route_options_panel = new QWidget();
  QVBoxLayout *options_layout = new QVBoxLayout(route_options_panel);
  
  QLabel *options_title = new QLabel("Route Options");
  options_title->setStyleSheet("font-size: 55px; font-weight: 500; margin-bottom: 20px;");
  options_layout->addWidget(options_title);
  
  // Create route preference selector
  QLabel *pref_label = new QLabel("Route Preference:");
  pref_label->setStyleSheet("font-size: 45px; margin-top: 20px;");
  options_layout->addWidget(pref_label);
  
  QComboBox *route_pref = new QComboBox();
  route_pref->addItem("Fastest Route");
  route_pref->addItem("Shortest Distance");
  route_pref->addItem("Eco-friendly Route");
  route_pref->setStyleSheet(R"(
    QComboBox {
      padding: 15px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #393939;
    }
    QComboBox::drop-down {
      width: 60px;
    }
    QComboBox QAbstractItemView {
      background-color: #393939;
      font-size: 45px;
      selection-background-color: #454545;
    }
  )");
  route_pref->setCurrentIndex(0);
  options_layout->addWidget(route_pref);
  
  // Create avoidance options
  QLabel *avoid_label = new QLabel("Avoid:");
  avoid_label->setStyleSheet("font-size: 45px; margin-top: 30px;");
  options_layout->addWidget(avoid_label);
  
  avoid_tolls_checkbox = new QCheckBox("Toll Roads");
  avoid_highways_checkbox = new QCheckBox("Highways");
  avoid_ferries_checkbox = new QCheckBox("Ferries");
  
  QString checkbox_style = R"(
    QCheckBox {
      font-size: 45px;
      margin: 10px 0px;
    }
    QCheckBox::indicator {
      width: 35px;
      height: 35px;
    }
  )";
  
  avoid_tolls_checkbox->setStyleSheet(checkbox_style);
  avoid_highways_checkbox->setStyleSheet(checkbox_style);
  avoid_ferries_checkbox->setStyleSheet(checkbox_style);
  
  options_layout->addWidget(avoid_tolls_checkbox);
  options_layout->addWidget(avoid_highways_checkbox);
  options_layout->addWidget(avoid_ferries_checkbox);
  
  // Connect signals for route options
  QObject::connect(avoid_tolls_checkbox, &QCheckBox::toggled, [=](bool checked) {
    setRouteOptions(avoid_tolls_checkbox->isChecked(), 
                    avoid_highways_checkbox->isChecked(), 
                    avoid_ferries_checkbox->isChecked());
  });
  
  QObject::connect(avoid_highways_checkbox, &QCheckBox::toggled, [=](bool checked) {
    setRouteOptions(avoid_tolls_checkbox->isChecked(), 
                    avoid_highways_checkbox->isChecked(), 
                    avoid_ferries_checkbox->isChecked());
  });
  
  QObject::connect(avoid_ferries_checkbox, &QCheckBox::toggled, [=](bool checked) {
    setRouteOptions(avoid_tolls_checkbox->isChecked(), 
                    avoid_highways_checkbox->isChecked(), 
                    avoid_ferries_checkbox->isChecked());
  });
  
  // Apply button
  QPushButton *apply_options_btn = new QPushButton("Apply Route Options");
  apply_options_btn->setStyleSheet(R"(
    QPushButton {
      padding: 20px;
      border-radius: 10px;
      font-size: 45px;
      font-weight: 500;
      background-color: #364DEF;
      margin-top: 30px;
    }
    QPushButton:pressed {
      background-color: #304AD0;
    }
  )");
  
  QObject::connect(apply_options_btn, &QPushButton::clicked, [=]() {
    setRouteOptions(avoid_tolls_checkbox->isChecked(), 
                    avoid_highways_checkbox->isChecked(), 
                    avoid_ferries_checkbox->isChecked());
    emit routeOptionsChanged(avoid_tolls_checkbox->isChecked(), 
                            avoid_highways_checkbox->isChecked(), 
                            avoid_ferries_checkbox->isChecked());
    
    // Store route preference in params
    int prefIndex = route_pref->currentIndex();
    params.putInt("NavRoutePreference", prefIndex);
  });
  
  options_layout->addWidget(apply_options_btn);
  options_layout->addStretch();
}

// Setup map view control panel
void MapPanel::setupMapViewPanel() {
  map_view_panel = new QWidget();
  QVBoxLayout *view_layout = new QVBoxLayout(map_view_panel);
  
  QLabel *view_title = new QLabel("Map Display Options");
  view_title->setStyleSheet("font-size: 55px; font-weight: 500; margin-bottom: 20px;");
  view_layout->addWidget(view_title);
  
  // Map mode options
  QLabel *mode_label = new QLabel("Map Mode:");
  mode_label->setStyleSheet("font-size: 45px; margin-top: 20px;");
  view_layout->addWidget(mode_label);
  
  map_mode_group = new QButtonGroup(this);
  
  day_mode_btn = new QRadioButton("Day Mode");
  night_mode_btn = new QRadioButton("Night Mode");
  satellite_mode_btn = new QRadioButton("Satellite View");
  
  QString radio_style = R"(
    QRadioButton {
      font-size: 45px;
      margin: 10px 0px;
    }
    QRadioButton::indicator {
      width: 35px;
      height: 35px;
    }
  )";
  
  day_mode_btn->setStyleSheet(radio_style);
  night_mode_btn->setStyleSheet(radio_style);
  satellite_mode_btn->setStyleSheet(radio_style);
  
  map_mode_group->addButton(day_mode_btn, 0);
  map_mode_group->addButton(night_mode_btn, 1);
  map_mode_group->addButton(satellite_mode_btn, 2);
  
  day_mode_btn->setChecked(true);
  
  view_layout->addWidget(day_mode_btn);
  view_layout->addWidget(night_mode_btn);
  view_layout->addWidget(satellite_mode_btn);
  
  // Layer options
  QLabel *layer_label = new QLabel("Map Layers:");
  layer_label->setStyleSheet("font-size: 45px; margin-top: 30px;");
  view_layout->addWidget(layer_label);
  
  traffic_checkbox = new QCheckBox("Show Traffic");
  show_poi_checkbox = new QCheckBox("Show Points of Interest");
  
  traffic_checkbox->setStyleSheet(R"(
    QCheckBox {
      font-size: 45px;
      margin: 10px 0px;
    }
    QCheckBox::indicator {
      width: 35px;
      height: 35px;
    }
  )");
  
  show_poi_checkbox->setStyleSheet(R"(
    QCheckBox {
      font-size: 45px;
      margin: 10px 0px;
    }
    QCheckBox::indicator {
      width: 35px;
      height: 35px;
    }
  )");
  
  view_layout->addWidget(traffic_checkbox);
  view_layout->addWidget(show_poi_checkbox);
  
  // Connect signals
  QObject::connect(map_mode_group, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id) {
    toggleMapMode(id);
    emit mapModeChanged(id);
  });
  
  QObject::connect(traffic_checkbox, &QCheckBox::toggled, [=](bool checked) {
    emit trafficToggled(checked);
  });
  
  QObject::connect(show_poi_checkbox, &QCheckBox::toggled, [=](bool checked) {
    emit poiVisibilityToggled(checked);
  });
  
  // Apply button
  QPushButton *apply_view_btn = new QPushButton("Apply Map Settings");
  apply_view_btn->setStyleSheet(R"(
    QPushButton {
      padding: 20px;
      border-radius: 10px;
      font-size: 45px;
      font-weight: 500;
      background-color: #364DEF;
      margin-top: 30px;
    }
    QPushButton:pressed {
      background-color: #304AD0;
    }
  )");
  
  QObject::connect(apply_view_btn, &QPushButton::clicked, [=]() {
    int mode = map_mode_group->checkedId();
    toggleMapMode(mode);
    emit mapModeChanged(mode);
    emit trafficToggled(traffic_checkbox->isChecked());
    emit poiVisibilityToggled(show_poi_checkbox->isChecked());
    
    // Store settings in params
    params.putBool("NavShowTraffic", traffic_checkbox->isChecked());
    params.putBool("NavShowPOI", show_poi_checkbox->isChecked());
    params.putInt("NavMapMode", mode);
  });
  
  view_layout->addWidget(apply_view_btn);
  view_layout->addStretch();
}

// Setup voice search button
void MapPanel::setupVoiceSearchButton() {
  voice_search_btn = new QPushButton();
  voice_search_btn->setIcon(QIcon("../assets/navigation/voice_search.png"));
  voice_search_btn->setIconSize(QSize(64, 64));
  voice_search_btn->setStyleSheet(R"(
    QPushButton {
      padding: 20px;
      border-radius: 10px;
      background-color: #4D4D4D;
    }
    QPushButton:pressed {
      background-color: #404040;
    }
  )");
  voice_search_btn->setFixedSize(120, 120);
  QObject::connect(voice_search_btn, &QPushButton::clicked, [=]() {
    emit voiceSearchRequested();
  });
}

// Setup waypoints panel
void MapPanel::setupWaypointsPanel() {
  waypoints_panel = new QWidget();
  QVBoxLayout *waypoints_layout = new QVBoxLayout(waypoints_panel);
  
  QLabel *waypoints_title = new QLabel("Multi-Stop Route");
  waypoints_title->setStyleSheet("font-size: 55px; font-weight: 500; margin-bottom: 20px;");
  waypoints_layout->addWidget(waypoints_title);
  
  waypoints_list = new QListWidget();
  waypoints_list->setStyleSheet(R"(
    QListWidget {
      background-color: #292929;
      font-size: 45px;
      border-radius: 10px;
    }
    QListWidget::item {
      padding: 15px;
      border-bottom: 1px solid #393939;
    }
    QListWidget::item:selected {
      background-color: #404040;
    }
  )");
  
  waypoints_layout->addWidget(waypoints_list);
  
  // Add waypoint controls
  QHBoxLayout *add_waypoint_layout = new QHBoxLayout();
  
  waypoint_input = new QLineEdit();
  waypoint_input->setPlaceholderText("Enter intermediate stop...");
  waypoint_input->setStyleSheet(R"(
    QLineEdit {
      padding: 15px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #393939;
    }
  )");
  
  QPushButton *add_waypoint_btn = new QPushButton("Add Stop");
  add_waypoint_btn->setStyleSheet(R"(
    QPushButton {
      padding: 15px 30px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #364DEF;
    }
    QPushButton:pressed {
      background-color: #304AD0;
    }
  )");
  
  QObject::connect(add_waypoint_btn, &QPushButton::clicked, [=]() {
    QString waypoint = waypoint_input->text().trimmed();
    if (!waypoint.isEmpty()) {
      waypoints_list->addItem(waypoint);
      waypoint_input->clear();
      emit addWaypoint(waypoint);
    }
  });
  
  add_waypoint_layout->addWidget(waypoint_input);
  add_waypoint_layout->addWidget(add_waypoint_btn);
  
  waypoints_layout->addLayout(add_waypoint_layout);
  
  // Remove and Clear buttons
  QHBoxLayout *waypoint_actions_layout = new QHBoxLayout();
  
  QPushButton *remove_waypoint_btn = new QPushButton("Remove Selected");
  remove_waypoint_btn->setStyleSheet(R"(
    QPushButton {
      padding: 15px 30px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #FF5A5A;
    }
    QPushButton:pressed {
      background-color: #D44A4A;
    }
  )");
  
  QPushButton *clear_waypoints_btn = new QPushButton("Clear All");
  clear_waypoints_btn->setStyleSheet(R"(
    QPushButton {
      padding: 15px 30px;
      border-radius: 8px;
      font-size: 45px;
      background-color: #4D4D4D;
    }
    QPushButton:pressed {
      background-color: #404040;
    }
  )");
  
  QObject::connect(remove_waypoint_btn, &QPushButton::clicked, [=]() {
    QListWidgetItem *item = waypoints_list->currentItem();
    if (item) {
      delete waypoints_list->takeItem(waypoints_list->row(item));
    }
  });
  
  QObject::connect(clear_waypoints_btn, &QPushButton::clicked, [=]() {
    waypoints_list->clear();
    emit clearWaypoints();
  });
  
  waypoint_actions_layout->addWidget(remove_waypoint_btn);
  waypoint_actions_layout->addWidget(clear_waypoints_btn);
  
  waypoints_layout->addLayout(waypoint_actions_layout);
  
  // Apply waypoints button
  QPushButton *apply_waypoints_btn = new QPushButton("Calculate Route with Stops");
  apply_waypoints_btn->setStyleSheet(R"(
    QPushButton {
      padding: 20px;
      border-radius: 10px;
      font-size: 45px;
      font-weight: 500;
      background-color: #25D366;
      margin-top: 30px;
    }
    QPushButton:pressed {
      background-color: #20B355;
    }
  )");
  
  waypoints_layout->addWidget(apply_waypoints_btn);
  waypoints_layout->addStretch();
}

// Search for POIs in the specified category
void MapPanel::searchPOI(const QString &category) {
  // Construct a search query for points of interest
  QString destination = "poi:" + category;
  
  if (!destination.isEmpty()) {
    // Store the search query and category
    params.put("Address", destination.toStdString());
    params.put("POICategory", category.toStdString());
    
    // Initiate the search (in a real implementation, this would search and display results)
    // For now, we'll just show a message and simulate setting a destination
    QString dummyLocation = category + " nearby";
    QJsonObject dummyPlace;
    dummyPlace["place_name"] = dummyLocation;
    dummyPlace["place_details"] = "Finding nearest location";
    dummyPlace["latitude"] = 0.0;  // These would be real coordinates in actual implementation
    dummyPlace["longitude"] = 0.0;
    
    QJsonDocument doc(dummyPlace);
    params.put("POISearch", doc.toJson().toStdString());
    
    // Update the current route widget to show searching status
    current_route_text->setText("Searching for " + category + "...");
    current_widget->setVisible(true);
    
    // In a real implementation, we would:
    // 1. Query a maps API for nearby POIs matching the category
    // 2. Display results in a list
    // 3. Allow user to select one
    // 4. Set that as the destination
    
    // Switch to the destinations tab to show the status
    mainTabWidget->setCurrentIndex(0);
  }
}

// Save a location as a favorite with a label
void MapPanel::saveAsFavorite(const QJsonObject &place, const QString &label) {
  // In a real implementation, this would save to the user's favorites
  // For now, we'll just store it in params
  QJsonObject favoriteObj = place;
  favoriteObj["label"] = label;
  favoriteObj["save_type"] = "favorite";
  
  QJsonDocument doc(favoriteObj);
  
  // Store the favorite - in a real implementation we would send this to the server
  QString favKey = "NavFavorite_" + label;
  params.put(favKey.toStdString(), doc.toJson().toStdString());
  
  // If the location is one of the special labels (home/work), update the display
  if (label == "home") {
    home_address->setText(place["place_name"].toString());
    home_address->setStyleSheet(R"(font-size: 50px; color: white;)");
    home_button->setIcon(QPixmap("../assets/navigation/home.png"));
    home_button->disconnect();
    QObject::connect(home_button, &QPushButton::clicked, [=]() {
      navigateTo(place);
      emit closeSettings();
    });
  } else if (label == "work") {
    work_address->setText(place["place_name"].toString());
    work_address->setStyleSheet(R"(font-size: 50px; color: white;)");
    work_button->setIcon(QPixmap("../assets/navigation/work.png"));
    work_button->disconnect();
    QObject::connect(work_button, &QPushButton::clicked, [=]() {
      navigateTo(place);
      emit closeSettings();
    });
  }
}

// Set route options
void MapPanel::setRouteOptions(bool avoidTolls, bool avoidHighways, bool avoidFerries) {
  // Store route options in params
  params.putBool("NavAvoidTolls", avoidTolls);
  params.putBool("NavAvoidHighways", avoidHighways);
  params.putBool("NavAvoidFerries", avoidFerries);
  
  // Update UI to reflect settings
  avoid_tolls_checkbox->setChecked(avoidTolls);
  avoid_highways_checkbox->setChecked(avoidHighways);
  avoid_ferries_checkbox->setChecked(avoidFerries);
  
  // In a real implementation, we would recalculate the route if one is active
  auto dest = QString::fromStdString(params.get("NavDestination"));
  if (dest.size()) {
    // Simulate route recalculation
    current_route_text->setText(current_route_text->text() + "\n(Recalculating with new options...)");
  }
}

// Toggle map mode (day/night/satellite)
void MapPanel::toggleMapMode(int mode) {
  // Store the selected mode in params
  params.putInt("NavMapMode", mode);
  
  // Update UI to reflect current mode
  day_mode_btn->setChecked(mode == 0);
  night_mode_btn->setChecked(mode == 1);
  satellite_mode_btn->setChecked(mode == 2);
  
  // In a real implementation, we would change the map display mode
  QString modeName;
  switch (mode) {
    case 0: modeName = "Day Mode"; break;
    case 1: modeName = "Night Mode"; break;
    case 2: modeName = "Satellite View"; break;
    default: modeName = "Unknown Mode";
  }
  
  // For now, just log the change
  qDebug() << "Map mode changed to:" << modeName;
}
