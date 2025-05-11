#include "startwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    StartWindow *startWindow = new StartWindow();
    startWindow->setAttribute(Qt::WA_DeleteOnClose);
    startWindow->show();
    return a.exec();
}
