#ifndef TRIANGULATIONVIEW_H
#define TRIANGULATIONVIEW_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include "plantcreator.h"
class TriangulationView : public QWidget
{
  Q_OBJECT
public:
  explicit TriangulationView(QWidget *parent = nullptr);
  TriangleNet S;
  Stroke points;
  PetalStroke testPetal;
protected:
  void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // TRIANGULATIONVIEW_H
