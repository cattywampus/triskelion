#ifndef TATTOO_CANVAS_H
#define TATTOO_CANVAS_H

#include <QPixmap>
#include <QWidget>

class TattooCanvas : public QWidget {
    Q_OBJECT

public:
    TattooCanvas(QWidget *parent = 0);

public slots:
    void setStroke(int stroke);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawCircles(QPainter *painter);
    void drawCurves(QPainter *painter, int radius, int curve1, int curve2);
    void drawSpiral(QPainter *painter, int radius, int rotate);
    QPointF rotatePoint(const QPointF &point, int angle);
    void refreshCanvas();

    enum { Margin = 20 };

    double revolution;
    QPixmap canvas;
    int stroke;
};

#endif

