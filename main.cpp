
#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>

#include "tattoo_canvas.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget *window = new QWidget;
    window->setWindowTitle("Archimedes Tattoo Design");

    TattooCanvas *canvas = new TattooCanvas;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(canvas);
    window->setLayout(layout);
    
    window->show();

    return app.exec();
}
