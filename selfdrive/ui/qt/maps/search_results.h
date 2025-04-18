#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPainter>
#include "geocoder.h"

class SearchResultItem : public QWidget {
    Q_OBJECT

public:
    explicit SearchResultItem(const LocationResult &result, QWidget *parent = nullptr);
    const LocationResult &getResult() const { return m_result; }
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void resultSelected(const LocationResult &result);
    void clicked();

private:
    LocationResult m_result;
    QLabel *nameLabel;
    QLabel *addressLabel;
    QLabel *distanceLabel;
    bool m_hovered = false;
};

class SearchResultsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchResultsWidget(QWidget *parent = nullptr);
    void setResults(const QVector<LocationResult> &results, const QString &query);
    void clear();
    void setVisible(bool visible) override;
    QSize sizeHint() const override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void resultSelected(const LocationResult &result);
    void cancelled();

private:
    QVBoxLayout *mainLayout;
    QVBoxLayout *resultsLayout;
    QWidget *resultsContainer;
    QScrollArea *scrollArea;
    QLabel *titleLabel;
    QLabel *noResultsLabel;
    QPushButton *cancelButton;
}; 