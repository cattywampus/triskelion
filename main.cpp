
#include <QApplication>
#include <QCheckBox>
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
    strokeLayout->addWidget(new QLabel("Pen Width: "));
    strokeLayout->addWidget(strokeSlider);
    strokeLayout->addWidget(strokeLabel);

    QSlider *radiusSlider = new QSlider;
    radiusSlider->setTickInterval(1);
    radiusSlider->setMinimum(1);
    radiusSlider->setMaximum(100);
    radiusSlider->setOrientation(Qt::Horizontal);
    QLabel *radiusLabel = new QLabel;
    QObject::connect(radiusSlider, SIGNAL(valueChanged(int)),
                    radiusLabel, SLOT(setNum(int)));
    QHBoxLayout *radiusLayout = new QHBoxLayout;
    radiusLayout->addWidget(new QLabel("Tangent Radius: "));
    radiusLayout->addWidget(radiusSlider);
    radiusLayout->addWidget(radiusLabel);

    QCheckBox *showMarkings = new QCheckBox("Show custom markings layer");

    QVBoxLayout *controlPanel = new QVBoxLayout;
    controlPanel->addLayout(strokeLayout);
    controlPanel->addLayout(radiusLayout);
    controlPanel->addWidget(showMarkings);
    controlPanel->addStretch();

    TattooCanvas *canvas = new TattooCanvas;
    QObject::connect(strokeSlider, SIGNAL(valueChanged(int)),
                     canvas, SLOT(setStroke(int)));
    QObject::connect(radiusSlider, SIGNAL(valueChanged(int)),
                     canvas, SLOT(setCurveRadius(int)));
    QObject::connect(showMarkings, SIGNAL(stateChanged(int)),
                     canvas, SLOT(setMarkingsVisible(int)));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(controlPanel);
    layout->addWidget(canvas);
    layout->setStretchFactor(canvas, 1);

    window->setLayout(layout);

    // Set default values
    strokeSlider->setSliderPosition(10);
    radiusSlider->setSliderPosition(30);
    showMarkings->setCheckState(Qt::Checked);

    window->show();

    return app.exec();
}
