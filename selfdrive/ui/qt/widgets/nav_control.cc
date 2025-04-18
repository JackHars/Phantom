#include "selfdrive/ui/qt/widgets/nav_control.h"

#include "selfdrive/common/params.h"
#include "selfdrive/ui/qt/api.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include <stdio.h>
#include <stdlib.h>

NavControl::NavControl() : ButtonControl("Navigation Destination", "", "Enter address to begin navigation") {
  destination_label.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  destination_label.setStyleSheet("color: #aaaaaa");
  hlayout->insertWidget(1, &destination_label);

  QObject::connect(this, &ButtonControl::clicked, [=]() {
    if (text() == "ADD") {
      QString destination = InputDialog::getText("Enter your destination", this);
      if (destination.length() > 0) {
        setText("LOADING");
        setEnabled(false);
        getDest(destination);
      }
    } else {
      params.remove("Address");
      // Don't remove NavDestination, just clear address
      // params.remove("NavDestination");
      refresh();
      emit clearRoute();
    }
  });

  refresh();
}

void NavControl::refresh() {
  QString param = QString::fromStdString(params.get("Address"));
  if (param.length()) {
    destination_label.setText(QString::fromStdString(params.get("Address")));
    setText("REMOVE");
  } else {
    destination_label.setText("");
    setText("ADD");
  }
  setEnabled(true);
}

void NavControl::getDest(const QString &destination) {
    if (!destination.isEmpty()) {
      params.put("Address", destination.toStdString());
      system("python3 /data/openpilot/selfdrive/ui/qt/widgets/set_destination.py");
      refresh();
    } else {
      ConfirmationDialog::alert("No destination set.", this);
      refresh();
    }
    refresh();
}

