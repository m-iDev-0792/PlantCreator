#include "utility.h"

int manhattanDistance(QPoint p1,QPoint p2){
  return abs(p1.x()-p2.x())+abs(p1.y()-p2.y());
}
float euclidDistance(QPointF p1,QPointF p2){
  return sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
}
void getRotateAxisAndAngle(glm::vec3 v,glm::vec3& axis,float& radian){
	glm::vec3 z(0,0,1);
	v=glm::normalize(v);
	axis=glm::cross(z,v);
	radian=std::acos(glm::dot(z,v));
}
cv::RotatedRect getFittedEllipse(std::vector<QPoint> points){
  std::vector<cv::Point> cvPoints;
  for(auto &p:points)cvPoints.emplace_back(p.x(),p.y());
  return cv::fitEllipse(cvPoints);
}
glm::vec2 getCircumCenter(glm::vec2 p1,glm::vec2 p2,glm::vec2 p3){
  float A,B,C,D,E,F,G;
  A=(p3.x-p1.x);
  B=(p3.y-p1.y);
  C=(p3.x*p3.x+p3.y*p3.y-p1.x*p1.x-p1.y*p1.y)/2.0;
  D=(p2.x-p1.x);
  E=(p2.y-p1.y);
  F=(p2.x*p2.x+p2.y*p2.y-p1.x*p1.x-p1.y*p1.y)/2.0;
  float x=(B*F-E*C)/(B*D-A*E);
  return glm::vec2(x,B?(C-A*x)/B:(F-D*x)/E);
}
QPointF getCircumCenter(QPointF p1,QPointF p2,QPointF p3){
  glm::vec2 p=getCircumCenter(glm::vec2(p1.x(),p1.y()),glm::vec2(p2.x(),p2.y()),glm::vec2(p3.x(),p3.y()));
  return QPointF(p.x,p.y);
}
/**
 * @brief linesIntersect
 * @param a :End point of line A
 * @param b :End point of line A
 * @param c :End point of line B
 * @param d :End point of line B
 * @return whether 2 lines are intersected,NOTE that if one of line A's end points is overlapped
 * with one of line B's end points return false, we don't think them are intersected.
 */
bool linesIntersect(QPointF a,QPointF b,QPointF c,QPointF d)
{
  //an exception
  if((a==c)||(a==d)||(b==c)||(b==d))return false;

  //fast detection
  if(!(std::min(a.x(),b.x())<=std::max(c.x(),d.x()) && std::min(c.y(),d.y())<=std::max(a.y(),b.y())&&std::min(c.x(),d.x())<=std::max(a.x(),b.x()) && std::min(a.y(),b.y())<=std::max(c.y(),d.y())))
    return false;

  double u,v,w,z;//分别记录两个向量
  u=(c.x()-a.x())*(b.y()-a.y())-(b.x()-a.x())*(c.y()-a.y());
  v=(d.x()-a.x())*(b.y()-a.y())-(b.x()-a.x())*(d.y()-a.y());
  w=(a.x()-c.x())*(d.y()-c.y())-(d.x()-c.x())*(a.y()-c.y());
  z=(b.x()-c.x())*(d.y()-c.y())-(d.x()-c.x())*(b.y()-c.y());
  return (u*v<=0.0000001 && w*z<=0.0000001);
}

bool inCircumCircle(QPointF p1,QPointF p2,QPointF p3,QPointF p){
  QPointF center=getCircumCenter(p1,p2,p3);
  float dis=euclidDistance(center,p);
  return euclidDistance(center,p1)>dis;
}
float det3x3(float data[3][3]){
  return data[0][0]*(data[1][1]*data[2][2]-data[1][2]*data[2][1])
      -data[0][1]*(data[1][0]*data[2][2]-data[1][2]*data[2][0]
      +data[0][2]*(data[1][0]*data[2][1]-data[1][1]*data[2][0]));
}
bool isInCircumCircle(QPointF p1, QPointF p2, QPointF p3, QPointF p){
  float det1[3][3];
  det1[0][0]=p1.x()-p2.x();    det1[0][1]=p1.y()-p2.y();    det1[0][2]=p1.x()*p1.x()+p1.y()*p1.y() - p2.x()*p2.x()-p2.y()*p2.y();
  det1[1][0]=p3.x();           det1[1][1]=p3.y();           det1[1][2]=p3.x()*p3.x()+p3.y()*p3.y(),
  det1[2][0]=p.x();            det1[2][1]=p.y();            det1[2][2]=p.x()*p.x()+p.y()*p.y();

  float det2[3][3];
  det2[0][0]=p1.x();           det2[0][1]=p1.y();           det2[0][2]=p1.x()*p1.x()+p1.y()*p1.y();
  det2[1][0]=p2.x();           det2[1][1]=p2.y();           det2[1][2]=p2.x()*p2.x()+p2.y()*p2.y();
  det2[2][0]=p3.x()-p.x();     det2[2][1]=p3.y()-p.y();     det2[2][2]=p3.x()*p3.x()+p3.y()*p3.y() - p.x()*p.x()-p.y()*p.y();
  return det3x3(det1)+det3x3(det2)>0;
}

void sortPointsAsXAscend(std::vector<QPoint> &ps){
  std::sort(ps.begin(),ps.end(),[](const QPoint &p1,const QPoint &p2)->bool{if(p1.x()==p2.x())return p1.y()<p2.y();else return p1.x()<p2.x();});
}


bool operator < (QPoint p1,QPoint p2){
  if(p1.x()==p2.x())return p1.y()<p2.y();
  else return p1.x()<p2.x();
}
float QVector2DCross(QVector2D p1,QVector2D p2){
  return p1.x()*p2.y()-p1.y()*p2.x();
}
void drawStrokeLine(std::vector<QPoint>& s,QPainter& p){
  if(s.size()<2)return;
  QPoint startPoint=s[0];
  for(int i=1;i<s.size();++i){
      p.drawLine(startPoint,s[i]);
      startPoint=s[i];
    }
}
