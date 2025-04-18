#include "search_results.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPalette>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include "selfdrive/ui/qt/util.h"

// SearchResultItem implementation
SearchResultItem::SearchResultItem(const LocationResult &result, QWidget *parent) 
    : QWidget(parent), m_result(result) {
    
    // No stylesheet - we'll do custom painting
    setAttribute(Qt::WA_StyledBackground, false);
    
    // Make the item selectable through mouse clicks
    setCursor(Qt::PointingHandCursor);
    setMinimumHeight(180);
    setMinimumWidth(300);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);
    
    // Top row with name and icon
    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->setContentsMargins(0, 0, 0, 0);
    topRow->setSpacing(15);
    
    // Icon on the left
    QLabel *icon = new QLabel();
    QPixmap pixmap("../assets/navigation/destination.png");
    icon->setPixmap(pixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    icon->setFixedSize(60, 60);
    topRow->addWidget(icon);
    
    // Name and address in the center
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);
    
    // Main name label
    nameLabel = new QLabel(result.name);
    nameLabel->setStyleSheet("font-size: 45px; font-weight: bold; color: white;");
    nameLabel->setWordWrap(true);
    infoLayout->addWidget(nameLabel);
    
    // Address label
    addressLabel = new QLabel(result.address);
    addressLabel->setStyleSheet("font-size: 35px; color: #BBBBBB;");
    addressLabel->setWordWrap(true);
    infoLayout->addWidget(addressLabel);
    
    topRow->addLayout(infoLayout, 1);
    mainLayout->addLayout(topRow);
    
    // Add distance at the bottom of the card
    if (result.distance > 0) {
        // Add a subtle separator line
        QFrame *line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); min-height: 1px;");
        mainLayout->addWidget(line);
        
        // Create distance row
        QHBoxLayout *distanceRow = new QHBoxLayout();
        distanceRow->setContentsMargins(0, 5, 0, 0);
        
        QString distanceText;
        if (result.distance < 1.0) {
            int meters = static_cast<int>(result.distance * 1000);
            distanceText = QString("%1 meters away").arg(meters);
        } else {
            distanceText = QString("%1 km away").arg(result.distance, 0, 'f', 1);
        }
        
        distanceLabel = new QLabel(distanceText);
        distanceLabel->setStyleSheet("font-size: 35px; color: #33A8FF; font-weight: bold;");
        distanceRow->addWidget(distanceLabel, 0, Qt::AlignRight);
        
        mainLayout->addLayout(distanceRow);
    }
    
    // Apply shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setOffset(0, 5);
    setGraphicsEffect(shadow);
    
    // Connect the click event
    connect(this, &SearchResultItem::clicked, [this]() {
        emit resultSelected(m_result);
    });
}

void SearchResultItem::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Create rounded rectangle path
    QPainterPath path;
    path.addRoundedRect(rect(), 15, 15);
    
    // Clip to the path
    painter.setClipPath(path);
    
    // Paint gradient background
    QLinearGradient gradient(0, 0, width(), height());
    
    if (m_hovered) {
        gradient.setColorAt(0, QColor(60, 60, 70));
        gradient.setColorAt(1, QColor(80, 80, 90));
    } else {
        gradient.setColorAt(0, QColor(45, 45, 55));
        gradient.setColorAt(1, QColor(60, 60, 70));
    }
    
    painter.fillPath(path, gradient);
    
    // Draw border
    if (m_hovered) {
        painter.setPen(QPen(QColor(80, 170, 255), 2));
    } else {
        painter.setPen(QPen(QColor(70, 70, 80), 1));
    }
    painter.drawPath(path);
    
    QWidget::paintEvent(event);
}

void SearchResultItem::enterEvent(QEvent *event) {
    m_hovered = true;
    update();
    QWidget::enterEvent(event);
}

void SearchResultItem::leaveEvent(QEvent *event) {
    m_hovered = false;
    update();
    QWidget::leaveEvent(event);
}

void SearchResultItem::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    emit clicked();
}

// SearchResultsWidget implementation
SearchResultsWidget::SearchResultsWidget(QWidget *parent) : QWidget(parent) {
    // Make sure we're completely opaque and cover everything
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, false);
    setAutoFillBackground(true);
    
    // Solid black background palette
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(0, 0, 0));
    setPalette(pal);
    
    // Style for the entire widget - force solid black background
    setStyleSheet(R"(
        SearchResultsWidget {
            background-color: black;
        }
        QLabel {
            color: white;
        }
        QScrollArea {
            border: none;
            background-color: transparent;
        }
    )");
    
    // Take up the entire parent widget
    if (parent) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent);
        if (parentWidget) {
            resize(parentWidget->size());
        }
    }
    
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);
    
    // Header with title
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 10);
    
    titleLabel = new QLabel("Search Results");
    titleLabel->setStyleSheet("font-size: 60px; font-weight: bold; margin-bottom: 10px;");
    headerLayout->addWidget(titleLabel, 1);
    
    mainLayout->addLayout(headerLayout);
    
    // Separator line
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #444444;");
    mainLayout->addWidget(line);
    
    // No results label (hidden by default)
    noResultsLabel = new QLabel("No results found");
    noResultsLabel->setStyleSheet("font-size: 45px; color: #AAAAAA; padding: 100px 50px; font-style: italic;");
    noResultsLabel->setAlignment(Qt::AlignCenter);
    noResultsLabel->setVisible(false);
    mainLayout->addWidget(noResultsLabel);
    
    // Scroll area for results
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("background-color: transparent;");
    
    // Customize scrollbar
    scrollArea->verticalScrollBar()->setStyleSheet(R"(
        QScrollBar:vertical {
            border: none;
            background: #333333;
            width: 12px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #666666;
            min-height: 30px;
            border-radius: 6px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");
    
    // Container for results
    resultsContainer = new QWidget(scrollArea);
    resultsContainer->setStyleSheet("background-color: transparent;");
    resultsLayout = new QVBoxLayout(resultsContainer);
    resultsLayout->setContentsMargins(10, 10, 10, 10);
    resultsLayout->setSpacing(15); 
    resultsLayout->addStretch();
    
    scrollArea->setWidget(resultsContainer);
    mainLayout->addWidget(scrollArea, 1);
    
    // Hide by default
    setVisible(false);
}

void SearchResultsWidget::setResults(const QVector<LocationResult> &results, const QString &query) {
    // Clear previous results
    clear();
    
    // Update title with query
    titleLabel->setText("Results for: " + query);
    
    if (results.isEmpty()) {
        // Show no results message
        noResultsLabel->setVisible(true);
        scrollArea->setVisible(false);
    } else {
        // Hide no results message
        noResultsLabel->setVisible(false);
        scrollArea->setVisible(true);
        
        // Add result items
        for (const auto &result : results) {
            SearchResultItem *item = new SearchResultItem(result, resultsContainer);
            connect(item, &SearchResultItem::resultSelected, this, &SearchResultsWidget::resultSelected);
            resultsLayout->insertWidget(resultsLayout->count() - 1, item);
        }
    }
}

void SearchResultsWidget::clear() {
    // Remove all result items but keep the stretch at the end
    while (resultsLayout->count() > 1) {
        QLayoutItem *item = resultsLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    
    noResultsLabel->setVisible(false);
}

void SearchResultsWidget::setVisible(bool visible) {
    // If parent exists, resize to fill parent
    if (visible) {
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget) {
            // Make sure we cover the entire parent
            resize(parentWidget->size());
            move(0, 0); // Position at top-left
            raise(); // Ensure it's on top
            
            // Force repaint
            update();
        }
    }
    
    QWidget::setVisible(visible);
    
    // Reset scroll position when showing
    if (visible && scrollArea) {
        scrollArea->verticalScrollBar()->setValue(0);
    }
}

QSize SearchResultsWidget::sizeHint() const {
    // If parent exists, return parent size
    if (parent()) {
        QWidget* parentWidget = qobject_cast<QWidget*>(parent());
        if (parentWidget) {
            return parentWidget->size();
        }
    }
    return QSize(1000, 800);
}

bool SearchResultsWidget::eventFilter(QObject *obj, QEvent *event) {
    return QWidget::eventFilter(obj, event);
}

void SearchResultsWidget::mouseReleaseEvent(QMouseEvent *event) {
    // Get the position of the click
    QPoint pos = event->pos();
    
    // If the click is not on a search result, dismiss the results
    QWidget *clickedWidget = QWidget::childAt(pos);
    if (clickedWidget && clickedWidget != scrollArea && !scrollArea->isAncestorOf(clickedWidget)) {
        emit cancelled();
    }
    
    QWidget::mouseReleaseEvent(event);
}

// Override paintEvent to ensure black background is drawn
void SearchResultsWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    QWidget::paintEvent(event);
} 