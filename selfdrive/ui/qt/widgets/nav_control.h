#pragma once

#include <QPushButton>

#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/qt/widgets/controls.h"

// nav
class NavControl : public ButtonControl {
  Q_OBJECT

public:
  NavControl();

private:
  Params params;

  QLabel destination_label;

  void refresh();
  void getDest(const QString &destination);
};
