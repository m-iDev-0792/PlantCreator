#ifndef UTILITY_H
#define UTILITY_H
#include <QPoint>
#include <QDebug>
#include <QVector2D>
#include <QPainter>
#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//NOTE change texRootDir for right texture directory!
const QString GlobalTexturePath=QString("/Users/hezhenbang/Desktop/");

const int POINT_STEP=8;
const int FIRST_POINT=0;
const int SECOND_POINT=POINT_STEP*1;
const int THIRD_POINT=POINT_STEP*2;
const int FACE_STEP=POINT_STEP*3;

inline void setVec2Data(float* data,glm::vec2& uv){
  data[0]=uv.x;
  data[1]=uv.y;
}

inline void setVec3Data(float* data,glm::vec3& p){
  data[0]=p.x;data[1]=p.y;data[2]=p.z;
}

inline void setFacePositionData(float* data,glm::vec3 p1,glm::vec3 p2,glm::vec3 p3){
  setVec3Data(data,p1);
  setVec3Data(data+SECOND_POINT,p2);
  setVec3Data(data+THIRD_POINT,p3);
}
inline void setFaceNormalData(float* data,glm::vec3 normal){
  setVec3Data(data+3,normal);
  setVec3Data(data+SECOND_POINT+3,normal);
  setVec3Data(data+THIRD_POINT+3,normal);
}
inline void setFaceUVData(float* data,glm::vec2 p1,glm::vec2 p2,glm::vec2 p3){
  setVec2Data(data+6,p1);
  setVec2Data(data+SECOND_POINT+6,p2);
  setVec2Data(data+THIRD_POINT+6,p3);
}
inline glm::vec3 getFaceNormal(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3){
  glm::vec3 e1=p2-p1;
  glm::vec3 e2=p3-p1;
  return glm::normalize(glm::cross(e1,e2));
}
inline bool closeToZeroVec(glm::vec3 vec,float bias=0.001){
  if((std::abs(vec.x)+std::abs(vec.y)+std::abs(vec.z))<bias)return true;
  return false;
}
inline void debugVec3(glm::vec3 v,QString tag){
  qDebug()<<tag<<" x:"<<v.x<<" y:"<<v.y<<" z:"<<v.z;
}
inline void debugQPoint(QPoint v,QString tag){
  qDebug()<<tag<<" x:"<<v.x()<<" y:"<<v.y();
}
inline bool isVecNan(glm::vec3 v){
  return std::isnan(v.x)||std::isnan(v.y)||std::isnan(v.z);
}

void getRotateAxisAndAngle(glm::vec3 v,glm::vec3& axis,float& radian);

int manhattanDistance(QPoint p1,QPoint p2);

float euclidDistance(QPointF p1,QPointF p2);

cv::RotatedRect getFittedEllipse(std::vector<QPoint> points);

glm::vec2 getCircumCenter(glm::vec2 p1,glm::vec2 p2,glm::vec2 p3);

QPointF getCircumCenter(QPointF p1,QPointF p2,QPointF p3);

bool linesIntersect(QPointF a,QPointF b,QPointF c,QPointF d);

bool inCircumCircle(QPointF p1,QPointF p2,QPointF p3,QPointF p);

void sortPointsAsXAscend(std::vector<QPoint> &ps);

bool operator < (QPoint p1,QPoint p2);

float QVector2DCross(QVector2D p1,QVector2D p2);

void drawStrokeLine(std::vector<QPoint>& s,QPainter& p);
#endif // UTILITY_H
