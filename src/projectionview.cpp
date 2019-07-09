#include "projectionview.h"
using namespace glm;
void projDiv(vec4& v,int w,int h){
  v.x=v.x/v.w*w;
  v.y=v.y/v.w*h;
  v.z/=v.w;
  v.w=1;
}
void showVec4(const vec4& v){
  qDebug()<<"x:"<<v.x<<"    y:"<<v.y<<"    z:"<<v.z<<"    w:"<<v.w;
}

ProjectionView::ProjectionView(QWidget *parent) : QWidget(parent)
{

}
void ProjectionView::paintEvent(QPaintEvent *event){
  QPainter painter(this);
  painter.fillRect(this->rect(),Qt::white);
  QPen dotPen(Qt::red);
  dotPen.setWidth(4);
  QPen linePen(Qt::black);
  mat4 viewMat=lookAt(vec3(5,5,5),vec3(0,0,0),vec3(0,1,0));
  mat4 projectionMat=perspective<float>(glm::radians(45.0f),1.0f,0.1,100.0);
  vec4 origin=projectionMat*viewMat*vec4(0,0,0,1);
  vec4 x=projectionMat*viewMat*vec4(3,0,0,1);
  vec4 y=projectionMat*viewMat*vec4(0,3,0,1);
  vec4 z=projectionMat*viewMat*vec4(0,0,3,1);
  int halfWidth=this->width()/2;
  int halfHeight=this->height()/2;
  projDiv(x,halfWidth,halfHeight);projDiv(y,halfWidth,halfHeight);projDiv(z,halfWidth,halfHeight);projDiv(origin,halfWidth,halfHeight);
//  showVec4(x);showVec4(y);showVec4(z);showVec4(origin);
  painter.setPen(linePen);
  painter.drawLine(origin.x+halfWidth,halfHeight-origin.y,halfWidth+x.x,halfHeight-x.y);
  painter.drawLine(origin.x+halfWidth,halfHeight-origin.y,halfWidth+y.x,halfHeight-y.y);
  painter.drawLine(origin.x+halfWidth,halfHeight-origin.y,halfWidth+z.x,halfHeight-z.y);
  painter.setPen(dotPen);
  painter.drawPoint(halfWidth+x.x,halfHeight-x.y);
  painter.drawPoint(halfWidth+y.x,halfHeight-y.y);
  painter.drawPoint(halfWidth+z.x,halfHeight-z.y);
  dotPen.setColor(Qt::blue);
  painter.setPen(dotPen);
  painter.drawPoint(origin.x+halfWidth,halfHeight-origin.y);

}
