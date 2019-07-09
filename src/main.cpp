#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QSurfaceFormat format;
  format.setVersion(3,3);
  format.setSamples(4);
  format.setProfile( QSurfaceFormat::CoreProfile );
  format.setDepthBufferSize(24);
  QSurfaceFormat::setDefaultFormat(format);
  MainWindow w;
  w.show();

  return a.exec();
}
