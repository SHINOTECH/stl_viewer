#include "mainwindow.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow mainWin;
  mainWin.show();

  mainWin.loadExampleIfExists();

  return app.exec();
}

// eof
