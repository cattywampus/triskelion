#ifndef TATTOO_CANVAS_H
#define TATTOO_CANVAS_H

#include <QPixmap>
#include <QWidget>

class TattooCanvas : public QWidget {
    Q_OBJECT

public:
    TattooCanvas(QWidget *parent = 0);

public slots:
    void setCurveRadius(int radius);
    void setStroke(int stroke);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawCircles(QPainter *painter);
    void drawCustomLayer(QPainter *painter);
    void drawSpiral(QPainter *painter, int radius, int rotate);
    int getRadius() const;
    QPointF rotatePoint(const QPointF &point, int angle);
    void refreshCanvas();


    enum { Margin = 20 };

    double revolution;
    int curveRadius;
    QPixmap canvas;
    int stroke;
};

#endif

