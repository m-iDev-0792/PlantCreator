#ifndef HJGWIDGET_H
#define HJGWIDGET_H

#include "OpenGLHeader.h"
#include <QOpenGLWidget>
#include "Scene.h"
#include "painterview.h"
#include "branch.h"
#include "petal.h"
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <string>
#include <fstream>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
class HJGWidget : public QOpenGLWidget, protected QtOpenGLBaseClass
{
  Q_OBJECT
public:
  bool frameMode;

  std::vector<HJGraphics::Texture2D> petalTex;

  std::vector<HJGraphics::Texture2D> leafTex;

  std::vector<HJGraphics::Texture2D> branchTex;

  HJGWidget()=default;

  explicit HJGWidget(QWidget* parent=nullptr);

  void switchScene(int index);

  void addScene(HJGraphics::Scene& scene);

  void setFPS(int f){fps=f;}

  int getFPS(){return fps;}

  void addLeafStrokes(const std::vector<PetalStroke3D>& leafstroke3ds);

  void addLeaf(const PetalStroke& points2D,const PetalStroke3D& points3D,bool isFloretPetal=true,float thickness=0.0);

  void addFloret(const Floret3D,float thickness=0.0);

  void addBranch(Branch* branch);

  void clearCurrentScene();

  void changeTexture(int type,int index);

  void exportOBJ();

  HJGraphics::Scene* getCurrentScene();

protected:
  std::vector<HJGraphics::Scene *> scenes;
  HJGraphics::Scene *currentScene;

  float fov;
  bool firstMouse;
  bool mouseDown;
  float mouseSensitivity;
  float lastX;
  float lastY;
  float yaw;
  float pitch;
  float moveSpeed;
  glm::vec3 originalDirection;
  int fps;

  std::vector<PlantObject*> plantObjects;
  std::vector<Petal*> petals;
  std::vector<Petal*> leaves;
  std::vector<Branch*> branches;
  int floretNum;

  //Inherited Funtions from QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent* event) override;


};

#endif // HJGWIDGET_H
