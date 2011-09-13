#include <algorithm>
#include <cmath>
#include <iostream>

#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QPointF>
#include <QPolygonF>
#include <QRect>
#include <QStylePainter>
#include <QVector>

#include "tattoo_canvas.h"

static double PI = 3.1415926535;

static double deg2rad(double angle) {
    return angle / 180.0 * PI;
}

static double rad2deg(double angle) {
    return angle / PI * 180.0;
}

TattooCanvas::TattooCanvas(QWidget *parent) {
    revolution = 180.0;
    stroke = 1;
    curveRadius = 30;
    markingsVisible = false;

    setMinimumSize(400, 400);
}

void TattooCanvas::setCurveRadius(int radius) {
    this->curveRadius = radius;

    update();
}

void TattooCanvas::setStroke(int stroke) {
    this->stroke = stroke;

    std::cout << "New stroke is " << stroke << std::endl;

    update();
}

void TattooCanvas::setMarkingsVisible(int enableMarkings) {
    markingsVisible = enableMarkings == Qt::Checked;

    update();
}

double TattooCanvas::computeSpiralRadius(double angle) {
    double a = 0;
    double b = (getRadius() + getOriginOffset() - a) / deg2rad(revolution);
    return a + b * angle;
    
}

QPointF TattooCanvas::convertToCartesian(double angle, double radius) {
    return QPointF(std::cos(angle) * radius, std::sin(angle) * radius);

}

void TattooCanvas::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    QPen pen(Qt::black, stroke, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Times"));

    int radius = (std::min(width(), height()) - (2 * Margin)) / 2;
    QPoint center(width() / 2, height() / 2);
    painter.drawEllipse(center, radius, radius);

    if (markingsVisible) {
        drawCustomLayer(&painter);
    }
    drawCircles(&painter);
}

void TattooCanvas::drawCircles(QPainter *painter) {
    painter->translate(width() / 2.0, height() / 2.0);

    double tHeight = std::sqrt(3.0 * std::pow((double) curveRadius, 2.0));
    double y1 = 2.0 / 3.0 * tHeight;
    double y2 = y1 - tHeight;

    //painter->drawEllipse(QPoint(0, y1), radius, radius);
    painter->drawArc(0 - curveRadius, y1 - curveRadius, curveRadius * 2.0, curveRadius * 2.0, 60 * 16, 60 * 16);
    //painter->drawEllipse(QPointF(radius, y2), radius, radius);
    painter->drawArc(0, y2 - curveRadius, curveRadius * 2.0, curveRadius * 2.0, 180 * 16, 60 * 16);
    //painter->drawEllipse(QPointF(-radius, y2), radius, radius);
    painter->drawArc(0 - 2.0 * curveRadius, y2 - curveRadius, curveRadius * 2.0, curveRadius * 2.0, 0, -60 * 16);

    painter->translate(0, y2);
    int tatRadius = (std::min(width(), height()) - (2 * Margin)) / 2;
    drawSpiral(painter, tatRadius - y2, 270);
    
    double xOffset = (1.0 / 3.0 * tHeight) * std::cos(deg2rad(30.0));
    double yOffset = (1.0 / 3.0 * tHeight) * std::sin(deg2rad(30.0)); 

    painter->translate(xOffset, y1 - yOffset);
    drawSpiral(painter, tatRadius - y2, 30);

    painter->translate(-2.0 * xOffset, 0);
    drawSpiral(painter, tatRadius - y2, 150);

    painter->resetTransform();
}

void TattooCanvas::drawCustomLayer(QPainter *painter) {
    painter->translate(width() / 2.0, height() / 2.0);
    QPen originalPen = painter->pen();
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    double tHeight = std::sqrt(3.0 * std::pow((double) curveRadius, 2.0));
    double y = 1.0 / 3.0 * tHeight;

    int radius = curveRadius - (stroke / 2);
    painter->translate(-curveRadius, -y);
    painter->drawEllipse(QPoint(0, 0), radius, radius);
    painter->drawPie(-radius, -radius, 2 * radius, 2 * radius, 0 * 16, -60 * 16);
    painter->setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPoint(0, 0); 
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    
    // Draw curve angle
    painter->drawPie(-10, -10, 20, 20, 0 * 16, -60 * 16);
    QChar phi(0x03D5);
    int charWidth = painter->fontMetrics().width(phi);
    int charHeight = painter->fontMetrics().height();
    //painter->drawText(charWidth, charHeight - 3, QString(phi));
   
    // Add visible theta & r guides
    painter->translate(curveRadius, y);
    painter->translate(0, 2 * y - tHeight);
    painter->drawLine(0, 0, 0, -50);
    painter->rotate(270);
    painter->setPen(QPen(Qt::gray, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    int maxAngle = 120;
    for (int angle = 30; angle <= maxAngle; angle += 10) {
        double r = computeSpiralRadius(deg2rad(angle)) - stroke / 2;
        QPointF pointOnSpiral = convertToCartesian(deg2rad(angle), r);
        if (angle == maxAngle) {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        painter->drawLine(QPointF(0, 0), pointOnSpiral);
    }
    painter->drawArc(-40, -40, 80, 80, 0, -maxAngle * 16);
    painter->rotate(-270);
    QString theta(QString("") + QChar(0xD835) + QChar(0xDF03));
    painter->drawText(40 + charWidth / 2, 0, theta);
    QString rChar = QString("") + QChar(0xD835) + QChar(0xDC5F);
    painter->drawText(60, 50, rChar);

    // recenter transform
    painter->translate(0, -2 * y + tHeight);
    QString aChar = QString("") + QChar(0xD835) + QChar(0xDC4E);
    QString pi = QString("") + QChar(0xD835) + QChar(0xDF0B);
    QString formula = rChar + QString(" = ") + aChar + theta;
    formula.append('\n');
    formula.append(aChar + QString(" = ") + QChar(0x2113) + QString(" / ") + pi);
    formula.append('\n');
    formula.append(theta + QString(" = [0, ") + pi + QString("]"));
    painter->drawText(15, -120, 100, 100, Qt::AlignLeft | Qt::TextWordWrap, formula);
 
    //𝜋
    //MATHEMATICAL ITALIC SMALL PI
    //Unicode: U+1D70B (U+D835 U+DF0B), UTF-8: F0 9D 9C 8B
 
    painter->save();
    painter->setPen(QPen(Qt::gray, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(0, 0, getRadius(), 0);
    painter->restore();
    painter->drawText(getRadius() - 40, -2, QChar(0x2113));

    for (int i = 0; i < 360; i += 90) {
        painter->rotate(i);
        int x = getRadius() + stroke / 2 + 2;
        painter->drawLine(x, 0, x + 5, 0);
        painter->rotate(-i);
    }

    painter->setPen(originalPen);
    painter->resetTransform();
}

void TattooCanvas::drawSpiral(QPainter *painter, int radius, int rotate) {
    double a = 0;
    double b = (radius - a) / deg2rad(revolution);
    double angle = 0.0;
    double r = 0.0;

    QVector<QPointF> points;
    if (!(a > 0)) {
        //points.append(QPointF(width() / 2, height() / 2));
    }

    while (r < radius) {
        r = a + b * angle;
        if (r > radius) {
            r = radius;
            angle = r / (a + b);
        }
        
        points.append(convertToCartesian(angle, r));

        angle += 0.1;
    };
    QPolygonF polyline(points);
    painter->rotate(rotate);
    painter->drawPolyline(polyline);
    painter->rotate(-rotate);
}

int TattooCanvas::getOriginOffset() const {
    return std::sqrt(3.0 * std::pow((double) curveRadius, 2.0)) / 3.0;
}

int TattooCanvas::getRadius() const {
    return (std::min(width(), height()) - (2 * Margin)) / 2;
}

QPointF TattooCanvas::rotatePoint(const QPointF &point, int angle) {
    double radians = deg2rad(angle);
    return QPointF(
            point.x() * std::cos(radians) - point.y() * std::sin(radians),
            point.x() * std::sin(radians) + point.y() * std::cos(radians)
    );
}

void TattooCanvas::refreshCanvas() {

}

