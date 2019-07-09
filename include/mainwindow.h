#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HJGWidget.h"
#include <QMainWindow>
#include <painterview.h>
#include <QFileDialog>
#include <QToolBar>
#include <QDir>
#include <QPixmap>
#include <QTimer>
#include "triangulationview.h"
namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  const float mapScale=0.05;
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_saveBtn_clicked();

  void on_thresholdSlider_valueChanged(int value);

  void on_saveReduced_clicked();

  void on_showReducedBtn_clicked();

  void on_delaunay_clicked();

  void on_petalTex_currentIndexChanged(int index);

  void on_branchTex_currentIndexChanged(int index);

  void on_leafTex_currentIndexChanged(int index);

  void on_actionLoad_Sketch_triggered();

  void on_actionExport_Sketch_triggered();

  void on_actionExport_Obj_triggered();

  void on_actionUndo_triggered();

  void on_actionAdd_Ref_triggered();

  void on_actionClearAll_triggered();

  void on_actionFrame_triggered();

  void on_actionClearModel_triggered();

  void on_actionConstruct_triggered();

  void on_showDebug_toggled(bool checked);

  void strokeTypeChanged(int index);

  void on_actiontest_triggered();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
