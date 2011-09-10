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

void TattooCanvas::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    QPen pen(Qt::black, stroke, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    int radius = (std::min(width(), height()) - (2 * Margin)) / 2;
    QPoint center(width() / 2, height() / 2);
    painter.drawEllipse(center, radius, radius);

    drawCircles(&painter);
    /*drawSpiral(&painter, radius, 0);
    drawSpiral(&painter, radius, 120);
    drawSpiral(&painter, radius, 240);

    drawCurves(&painter, radius, 0, 120);
    drawCurves(&painter, radius, 120, 240);
    drawCurves(&painter, radius, 240, 0);*/
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
    painter->rotate(270);

    int tatRadius = (std::min(width(), height()) - (2 * Margin)) / 2;
    drawSpiral(painter, tatRadius - y2, 0);
    painter->rotate(-270);
    
    double xOffset = (1.0 / 3.0 * tHeight) * std::cos(deg2rad(30.0));
    double yOffset = (1.0 / 3.0 * tHeight) * std::sin(deg2rad(30.0)); 

    painter->resetTransform();
    painter->translate(width() / 2.0, height() / 2.0);
    painter->translate(xOffset, yOffset);
    painter->rotate(270);
    drawSpiral(painter, tatRadius - y2, 120);

    painter->resetTransform();
    painter->translate(width() / 2.0, height() / 2.0);
    painter->translate(-xOffset, yOffset);
    painter->rotate(270);
    drawSpiral(painter, tatRadius - y2, 240);
}

void TattooCanvas::drawCurves(QPainter *painter, int radius, int curve1, int curve2) {
    double a = 0;
    double b = radius / deg2rad(revolution);
    double r = 22.0;
    double angle = r / (b + a);

    QPointF center(width() / 2.0, height() / 2.0);
    
    QPoint temp(std::cos(angle) * r, std::sin(angle) * r);
    QPointF p1 = rotatePoint(temp, curve1);
    QPointF p2 = rotatePoint(temp, curve2);

    p1 += center;
    p2 += center;

    double slope1 = (p1.y() - center.y()) / (p1.x() - center.x());
    double slope2 = (p2.y() - center.y()) / (p2.x() - center.x());
    double y = (p2.y() * slope2 - p1.y() * slope1 + p2.x() - p1.x()) / 
               (slope2 - slope1);
    double x = p1.y() * slope1 - y * slope1 + p1.x();

    double curveRadius = std::sqrt(std::pow(x - p1.x(), 2) + 
                                   std::pow(y - p1.y(), 2));
    std::cout << "Curve Radius = " << curveRadius << std::endl;
    double chord = std::sqrt(std::pow(p1.x() - p2.x(), 2) + 
                             std::pow(p1.y() - p2.y(), 2));
    
    double refAngle = rad2deg(std::asin((y - p1.y()) / curveRadius));
    double startAngle = 0.0;

    if ( (p1.x() - x) >= 0) {
        // Quandrants 1 & 4
        startAngle = refAngle;
    } else {
        // Quandrants 2 & 3
        startAngle = 180.0 - refAngle;
    }

    double spanAngle = 2 * rad2deg(std::asin(chord / 2.0 / curveRadius));

    QRect rect(x - curveRadius, 
               y - curveRadius, 
               curveRadius * 2, 
               curveRadius * 2);
    painter->drawArc(rect, startAngle*16, spanAngle*16);
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
        
        QPointF point = rotatePoint(
                QPointF(std::cos(angle) * r, std::sin(angle) * r), 
                rotate
        );
        points.append(QPointF(point.x(), point.y()));

        angle += 0.1;
    };
    QPolygonF polyline(points);
    painter->drawPolyline(polyline);
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

