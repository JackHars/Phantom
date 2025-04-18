#include "map_settings.h"

#include <QDebug>

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

  QWidget * main_widget = new QWidget;
  QVBoxLayout *main_layout = new QVBoxLayout(main_widget);
  const int icon_size = 200;

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
    // Launch the map search interface or send a signal to activate map mode
    emit openMapSearch();
  });
  
  add_dest_layout->addWidget(add_dest_button);
  add_dest_layout->addSpacing(20);
  add_dest_layout->addWidget(map_search_button);
  main_layout->addWidget(add_dest_container);
  main_layout->addSpacing(20);
  main_layout->addWidget(horizontal_line());
  main_layout->addSpacing(20);

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

  main_layout->addLayout(home_work_layout);
  main_layout->addSpacing(20);
  main_layout->addWidget(horizontal_line());
  main_layout->addSpacing(20);

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
      // Instead of removing NavDestination, emit a signal to clear the route visually
      emit clearRoute();
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
  main_layout->addWidget(current_widget);

  // Recents
  QLabel *recents_title = new QLabel("Recent Destinations");
  recents_title->setStyleSheet("font-size: 55px");
  main_layout->addWidget(recents_title);
  main_layout->addSpacing(20);

  recent_layout = new QVBoxLayout;
  QWidget *recent_widget = new LayoutWidget(recent_layout, this);
  ScrollView *recent_scroller = new ScrollView(recent_widget, this);
  main_layout->addWidget(recent_scroller);

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
  // stack->addWidget(no_prime_widget);
  stack->setCurrentIndex(1);

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
