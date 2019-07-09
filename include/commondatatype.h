#ifndef COMMONDATATYPE_H
#define COMMONDATATYPE_H
#include <QLine>
#include <QVector2D>
#include <QPainter>
#include "utility.h"
#include <algorithm>
#include <map>
#include <random>
#include <utility>
typedef std::vector<QPoint> Stroke;
typedef Stroke Skeleton;
typedef std::vector<glm::vec3> Stroke3D;
typedef Stroke3D Skeleton3D;
class PetalStroke;
typedef std::vector<PetalStroke> Floret;

struct ConeInfo{
  glm::vec3 mainAxis;
  glm::vec3 rotateAxis;
  float rotateRadian;
  float radius;
  float height;
};
struct PetalStroke{
  PetalStroke(const Stroke& s);
  PetalStroke()=default;
  Stroke stroke;
  QPoint rootPoint;
  QPoint farPoint;
  Stroke contour;
  std::map<QPoint,std::pair<float,float>> tVtU;
  std::vector<glm::vec2> uvs;
  void translate(QPoint newOrigin);//convert positions of all points into a new coordinate whose origin starts at 'newOrigin'
  void generateUVCoord();
};

struct PetalStroke3D{
  PetalStroke3D(const Stroke3D& s);
  PetalStroke3D()=default;
  Stroke3D stroke;
  glm::vec3 rootPoint;
  glm::vec3 farPoint;
  glm::vec3 worldOffset;
  glm::vec3 mainOrient;
  glm::mat4 mat;
  Stroke3D contour;
};
struct TriangleNet{
  std::vector<QPoint> points;
  std::vector<QLine> edges;
};
struct Triangle{
  std::vector<int> pointIndices;
  Triangle(int i1,int i2,int i3);
};
inline float branchAttenuation(float x){
  float m=0.1;
  return (std::pow(9,-x)+m)/(1+m);
  return std::pow(1.1,-x*0.3);
}

void strokeFilter(Stroke& stroke);

void strokeFilter(Stroke3D& stroke);

#endif // COMMONDATATYPE_H
