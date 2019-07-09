#ifndef PROJECTIONVIEW_H
#define PROJECTIONVIEW_H
#include <QDebug>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class ProjectionView : public QWidget
{
  Q_OBJECT
public:
  explicit ProjectionView(QWidget *parent = nullptr);
protected:
  void paintEvent(QPaintEvent *event) override;
signals:

public slots:
};

#endif // PROJECTIONVIEW_H
