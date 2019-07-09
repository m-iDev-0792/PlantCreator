#include "modelview.h"

ModelView::ModelView(QWidget *parent):QOpenGLWidget(parent)
{

}
void ModelView::initializeGL(){
  initializeOpenGLFunctions();
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void ModelView::paintGL(){

}
void ModelView::resizeGL(int w, int h){
  glViewport(0,0,w,h);
}
