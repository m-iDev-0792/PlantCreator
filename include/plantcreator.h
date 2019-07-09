#ifndef PLANTCREATOR_H
#define PLANTCREATOR_H
#include <QLine>
#include <QVector2D>
#include <QPainter>
#include "utility.h"
#include <algorithm>
#include <map>
#include <random>
#include <utility>
#include "branch.h"

struct Floret3D{
  std::vector<PetalStroke3D> petals3D;
  std::vector<PetalStroke> petals2D;
  ConeInfo coneInfo;
  glm::vec3 floretCenter;
  Floret3D()=default;
  Floret3D(std::vector<PetalStroke> _petals2D,std::vector<PetalStroke3D> _petal3D,ConeInfo _coneInfo,glm::vec3 _floretCenter):
  petals2D(_petals2D),petals3D(_petal3D),coneInfo(_coneInfo),floretCenter(_floretCenter){}
  Floret3D(Floret floret2D,QPointF worldOrigin,float mapScale);
  void petalRandomRotate();
  void bendPetal(float factor=1.0);
  void cavePetal(float factor=1.0);
  void bendAndCavePetal(float bendFactor=1.0,float caveFactor=1.0);
  void adjustOrient();
};



struct SkeletonNodeInfo{
  Skeleton skeleton;
  float r;
  glm::vec2 offset;
  Skeleton3D skeleton3D;
  std::vector<SkeletonNodeInfo*> offshoots;
  std::vector<int> offshootRootIndex;
  std::vector<QPoint> anchorPoints;
  std::vector<glm::vec3> anchorPoints3D;
  std::vector<PetalStroke3D*> leaves;
  //added on 05-10
  glm::vec3 anchorPoint3D;
  QPoint anchorPoint;
  int rootIndex;
  int endIndex;

  SkeletonNodeInfo()=default;
  SkeletonNodeInfo(const Skeleton& s,float _r=1.0,QPoint _offset=QPoint(0,0)){
    skeleton=s;
    r=_r;
    offset=glm::vec2(_offset.x(),_offset.y());
  }
  void drawSkeleton(QPainter& painter,int depth);

  void constructSkeleton3D(glm::vec3 axis,glm::vec3 anchor3D,float rotateAngle,int myIndexNearAnchor,bool isRoot=false);

  void translate(float x,float y,float mapScale);

  void saveSkeleton3D(std::vector<Skeleton3D>& sk3D,std::vector<float>& radius);

  void generateSubBranch(const std::vector<Stroke>& profiles,std::vector<Stroke>& newBranches,float growthPointGap=20,float growthRate=5,int iterationNum=10);

  void generateSubBranch(const std::vector<Stroke>& profiles,float growthPointGap=20,float growthRate=5,int iterationNum=10);

  void generateLeaves(const std::vector<Stroke>& profiles,std::vector<std::pair<QPoint,QPoint>>& newPetalInfo,float growthPointGap=10,float growthRate=5,int iterationNum=10);

  void updateRadius();

  void generateBranchModel(std::vector<Branch*>& branchModels,float baseRadius=0.2);

};




std::vector<Floret> meanShiftClassify(const std::vector<PetalStroke>& leaves,float r);

std::vector<PetalStroke3D> constructFloret(std::vector<PetalStroke>& leaves,QPointF worldOrigin);

PetalStroke meshTessellation2D(const PetalStroke& contour,float axisGap, float perpendAxisGap);

PetalStroke meshTessellationAsGap(const PetalStroke &contour,float axisGap, float perpendAxisGap);

PetalStroke meshTessellationAsDiv(const PetalStroke &contour,int axisPointNum, int perpendAxisPointNum);

TriangleNet delaunayTriangulation(const std::vector<QPoint>& points,std::map<QPoint,int>& indexMap,std::vector<std::vector<bool>> &adjMatrix);

std::vector<Triangle> getTriangleFromNet(TriangleNet net,std::map<QPoint,int>& indexMap,std::vector<std::vector<bool>> &adjMatrix);

std::vector<SkeletonNodeInfo*> branchAnalyse(std::vector<Skeleton>& branches,float attachThreshold);

bool existPointBelow(QPointF leftMost,QPointF bottomMost,std::vector<QPoint> points);

float solveZ(float x,float y,glm::vec3 axis,glm::vec3 anchorPoint,float rotateAngle);

bool floretBranchMatch(Floret3D& floret, SkeletonNodeInfo* node);

bool leafBranchMatch(PetalStroke3D& leaf, SkeletonNodeInfo* node);

PetalStroke3D constructLeaf(PetalStroke& leaf,QPointF newOrigin,float mapScale);

void copyAndPasteLeaves(const std::vector<PetalStroke>& lib,std::vector<PetalStroke>& newLeaves,const std::vector<std::pair<QPoint,QPoint>> leafInfo);

std::vector<std::pair<float,float>> getPetalWidthInfo(const PetalStroke& petal);

std::vector<std::pair<float,float>> getPetalWidthInfo3D(const PetalStroke3D& petal);

void bendLeaf(PetalStroke3D& leaf,float factor=1.0f,glm::vec3 movingVec=glm::vec3(0,0,1));

void orderPreserveUnique(Stroke& stroke);

#endif // PLANTCREATOR_H
