#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Demo06-DualSceneFusion");
    app.setApplicationVersion("1.0.0");
    MainWindow w;
    w.show();
    return app.exec();
}
