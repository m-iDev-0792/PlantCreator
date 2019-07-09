#ifndef SKETCHVIEW_H
#define SKETCHVIEW_H

#include "painterview.h"

class SketchView : public QWidget
{
  Q_OBJECT
private:
  std::vector<Stroke> skeletons;
  std::vector<Stroke> profiles;
  std::vector<PetalStroke> leaves;
  std::vector<Floret> leafClusters;
  std::vector<cv::RotatedRect> ellipses;
  std::vector<SkeletonNodeInfo*> branchInfo;
  std::vector<PetalStroke> newLeaves;

  std::vector<Stroke> newBranches;
  std::vector<std::pair<QPoint,QPoint>> newLeafInfo;

protected:
  void paintEvent(QPaintEvent *event);
public:
  explicit SketchView(QWidget *parent = nullptr);
  void setSkeletons(const std::vector<Stroke> &sk);
  void setLeaves(const std::vector<PetalStroke> &ls);
  void setProfiles(const std::vector<Stroke> &pro);
  void setLeafClusters(const std::vector<Floret> &cluster);
  void clear();
signals:

public slots:
};

#endif // SKETCHVIEW_H
