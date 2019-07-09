#ifndef PAINTERVIEW_H
#define PAINTERVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include "plantcreator.h"

const int leafStartEndThreshold=20;

class PainterView : public QWidget
{
  Q_OBJECT
public:
  explicit PainterView(QWidget *parent = nullptr);
  QPixmap getReducedPixmap(int threshold);

  std::vector<Stroke> getSkeletons(int threshold);
  std::vector<PetalStroke> getLeaves(int threshold);
  std::vector<Stroke> getOriginal(){return originalStrokes;}
  std::vector<Stroke> getProfiles(int threshold=20);
  std::vector<Stroke> getBranchProfiles(int threshold=20);
  std::vector<Stroke> getLeafProfiles(int threshold=20);
  std::vector<int> getHistroy(){return history;}
  void setMode(int mode){currentMode=mode;}
  void loadSketch(std::vector<Stroke>& sketch,std::vector<int>& tHisotry);
  void clear();
  void undo();
  enum{
    PETAL,
    SKELETON,
    BRANCH_PROFILE,
    LEAF_PROFILE
  };
signals:
protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event);
public slots:
  void setReferenceImage(QPixmap pixmap);
private:
  QPixmap referenceImage;
  std::vector<Stroke> originalStrokes;
  Stroke tempStroke;

  std::vector<Stroke> skeletons;
  std::vector<PetalStroke> leaves;
  bool lastIsLeaves;
  std::vector<int> history;
  std::vector<Stroke> profiles;
  std::vector<Stroke> branchProfiles;
  std::vector<Stroke> leafProfiles;
  int currentMode;

};

#endif // PAINTERVIEW_H
