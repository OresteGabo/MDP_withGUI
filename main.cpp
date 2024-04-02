#include <QApplication>
#include <QPushButton>
#include <random>
#include <iostream>

#include "Widget.h"




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget widget;
    widget.show();

    return app.exec();
}