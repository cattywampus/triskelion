
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include "tattoo_canvas.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setWindowTitle("Archimedes Tattoo Design");

    QSlider *strokeSlider = new QSlider;
    strokeSlider->setTickInterval(1);
    strokeSlider->setMinimum(1);
    strokeSlider->setMaximum(20);
    strokeSlider->setOrientation(Qt::Horizontal);

    QLabel *strokeLabel = new QLabel;

    QObject::connect(strokeSlider, SIGNAL(valueChanged(int)),
                     strokeLabel, SLOT(setNum(int)));

    QHBoxLayout *strokeLayout = new QHBoxLayout;
    strokeLayout->addWidget(strokeSlider);
    strokeLayout->addWidget(strokeLabel);
    

    QVBoxLayout *controlPanel = new QVBoxLayout;
    controlPanel->addLayout(strokeLayout);
    controlPanel->addStretch();

    TattooCanvas *canvas = new TattooCanvas;
    QObject::connect(strokeSlider, SIGNAL(valueChanged(int)),
                     canvas, SLOT(setStroke(int)));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(controlPanel);
    layout->addWidget(canvas);

    window->setLayout(layout);

    // Set default values
    strokeSlider->setSliderPosition(3);

    window->show();

    return app.exec();
}
