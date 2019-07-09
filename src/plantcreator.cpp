#include "plantcreator.h"




//-------------------SkeletonNodeInfo-------------------
QColor colorList[10]={QColor("#FF0000"),QColor("#0000FF"),QColor("#2E8B57"),QColor("#FFFF33"),QColor("#00868B"),QColor("#CD9B1D"),QColor("#A500CC"),QColor("#7FFFD4"),QColor("#DCDCDC"),QColor("#FFB3FF")};
void SkeletonNodeInfo::updateRadius(){
  if(offshoots.empty())return;
  float offshootR=r/std::sqrt(offshoots.size());
  qDebug()<<"father r="<<r<<" offshoot r="<<offshootR;
  for(int i=0;i<offshoots.size();++i){
      auto& o=offshoots[i];
      int offshootAnchorIndex=0;
      for(int j=0;j<skeleton.size();++j){
          if(anchorPoints[i]==skeleton[j]){
              offshootAnchorIndex=j;
              break;
            }
        }

      float anchorR=r*branchAttenuation((anchorPoint==skeleton.back()?skeleton.size()-offshootAnchorIndex:offshootAnchorIndex)/(float)skeleton.size());
      if(anchorR>offshootR){
          o->r=offshootR;//qDebug()<<"yes";
        }//else qDebug()<<"no";
      o->updateRadius();
    }
}

void SkeletonNodeInfo::drawSkeleton(QPainter &painter, int depth){

  QPen pen;
  pen.setWidth(1);
  pen.setColor(colorList[depth]);

  QPen dotPen;
  dotPen.setWidth(4);
  dotPen.setColor(colorList[depth]);
  const Skeleton& s=(this->skeleton);

  painter.setPen(dotPen);
  QPoint qoffset(offset.x,offset.y);
  painter.drawPoint(s[0]-qoffset);
  for(int i=1;i<s.size();++i){
      painter.setPen(pen);
      painter.drawLine(s[i-1]-qoffset, s[i]-qoffset);
      painter.setPen(dotPen);
      painter.drawPoint(s[i]-qoffset);
    }

  for(auto& o:offshoots)o->drawSkeleton(painter,depth+1);
}
void SkeletonNodeInfo::translate(float x, float y, float mapScale){
  for(auto& p:skeleton3D){
      p=glm::vec3((p.x-x)*mapScale,(y-p.y)*mapScale,p.z*mapScale);
    }
  offset=glm::vec2((offset.x)*mapScale,(-offset.y)*mapScale);
  for(auto& s:offshoots)s->translate(x,y,mapScale);
}
void SkeletonNodeInfo::saveSkeleton3D(std::vector<Skeleton3D>& sk3D,std::vector<float>& radius){
  sk3D.push_back(skeleton3D);
  radius.push_back(r);
  for(auto& s:offshoots)s->saveSkeleton3D(sk3D,radius);
}
void SkeletonNodeInfo::generateBranchModel(std::vector<Branch*>& branchModels,float baseRadius){
  branchModels.push_back(new Branch(skeleton3D,16,baseRadius*r,rootIndex));
  for(auto& s:offshoots)s->generateBranchModel(branchModels,baseRadius);
}

float solveZ(float x,float y,glm::vec3 axis,glm::vec3 anchorPoint,float rotateAngle){
  glm::vec3 polarAxis;
  if(axis!=glm::vec3(0,1,0))polarAxis=glm::normalize(glm::cross(glm::vec3(0,1,0),axis));
  else polarAxis=glm::vec3(0,0,1);

  x-=anchorPoint.x;
  y-=anchorPoint.y;
  float alpha=-axis.x*axis.z;
  float beta=-axis.y*axis.z;
  float gamma=1-axis.z*axis.z;
  float U=x-(axis.x*x+axis.y*y)*axis.x;
  float V=y-(axis.x*x+axis.y*y)*axis.y;
  float W=-(axis.x*x+axis.y*y)*axis.z;

  float G=alpha*polarAxis.x+beta*polarAxis.y+gamma*polarAxis.z;
  float H=U*polarAxis.x+V*polarAxis.y+W*polarAxis.z;

  float cosa=std::cos(rotateAngle);
  float A=cosa*cosa*(alpha*alpha+beta*beta+gamma*gamma)-G*G;
  float B=cosa*cosa*2*(alpha*U+beta*V+gamma*W)-2*G*H;
  float C=(U*U+V*V+W*W)*cosa*cosa-H*H;

  float positiveZ=((-B+std::sqrt(B*B-4*A*C))/(2*A) + anchorPoint.z);
  float negativeZ=((-B-std::sqrt(B*B-4*A*C))/(2*A) + anchorPoint.z);

  glm::vec3 branchVec(x,y,positiveZ-anchorPoint.z);
  glm::vec3 dir=branchVec-glm::dot(branchVec,axis)*axis;
  dir=glm::normalize(dir);
  if(std::abs(cosa-glm::dot(dir,polarAxis))<0.01){
      return positiveZ;
    }else{
      return negativeZ;
    }

}
//----------------------------------------------父亲的主轴-----------我的锚点-------------旋转角度
void SkeletonNodeInfo::constructSkeleton3D(glm::vec3 axis, glm::vec3 anchor3D,float rotateAngle,int myIndexNearAnchor,bool isRoot){
  QPoint terminus2D,anchor2D;
  glm::vec3 topPoint3D;
  if(isRoot){
      anchor3D=glm::vec3(skeleton.back().x(),skeleton.back().y(),0);
      topPoint3D=glm::vec3(skeleton.front().x(),skeleton.front().y(),0);

    }
  else{

      anchor2D=skeleton.at(myIndexNearAnchor);
      terminus2D=skeleton.at(std::abs(static_cast<int>(skeleton.size()-1-myIndexNearAnchor)));
      //BUG solveZ may return nan!!!!
      topPoint3D=glm::vec3(terminus2D.x(),terminus2D.y(),
                           solveZ(terminus2D.x(),terminus2D.y(),axis,anchor3D,rotateAngle));
      if(std::isnan(topPoint3D.z)){
        qDebug()<<" a Z is nan!";
      }
    }
  anchorPoint3D=anchor3D;
  //---------------------following codes have nothing to do with parameter--------------------------

  //对skeleton3D进行插值填充
  float totalDis=manhattanDistance(skeleton.front(),skeleton.back());
  std::map<QPoint,glm::vec3> map2DTo3D;
  skeleton3D.clear();
  //写入深度信息
  for(int i=0;i<skeleton.size();++i){
      float Dis=manhattanDistance(skeleton.at(i),anchor2D);
      skeleton3D.push_back(glm::vec3(skeleton.at(i).x(),skeleton.at(i).y(),Dis/totalDis*(topPoint3D.z-anchor3D.z)+anchor3D.z));
      map2DTo3D[skeleton.at(i)]=skeleton3D[i];
    }



  //分析子分支的信息，并递归调用函数
  //update anchor point info of subbranch
  anchorPoints3D.clear();
  for(int i=0;i<anchorPoints.size();++i)anchorPoints3D.push_back(map2DTo3D[anchorPoints.at(i)]);

  //calculate angle of each subbranch
  std::vector<bool> isUpperBranch(offshoots.size(),false);
  int upperBranchNum=0;int lowerBranchNum=0;
  bool criteria=terminus2D.x()>anchor2D.x();
  //叉乘为正的是上面 when criteria=true
  //叉乘为正的是下面 when criteria=false
  for(int i=0;i<offshoots.size();++i){
      //modified
      QPoint subEnd,subStart;
      subStart=offshoots[i]->skeleton[offshootRootIndex[i]];
      subEnd=offshoots[i]->skeleton.at(std::abs(static_cast<int>(offshoots[i]->skeleton.size()-1-offshootRootIndex[i])));

      if((QVector2DCross(QVector2D(terminus2D-anchor2D),QVector2D(subEnd-anchor2D))>0)==criteria){
          //上面
          ++upperBranchNum;
          isUpperBranch[i]=true;
        }else{
          //下面
          ++lowerBranchNum;
          isUpperBranch[i]=false;
        }

    }

  const float PI=3.1415926;
  const float upperUnitAngle=PI*0.83/(upperBranchNum+1);
  const float lowerUnitAngle=PI*0.83/(lowerBranchNum+1);
  float upperAngle=upperUnitAngle+0.0833*PI;
  float lowerAngle=lowerUnitAngle+0.0833*PI;
//  qDebug()<<"upperBranchNum: "<<upperBranchNum<<"   lowerBranchNum: "<<lowerBranchNum;
  std::random_device seed;
  std::default_random_engine randomEngine(seed());

  //for float random number
  std::uniform_real_distribution<> dis(-0.5, 0.5);
  float noiseAngle=PI/6;
  for(int i=0;i<offshoots.size();++i){
      float randomFloat=dis(randomEngine);
      if(isUpperBranch[i]){
          offshoots[i]->constructSkeleton3D(glm::normalize(glm::vec3(topPoint3D-anchor3D)),anchorPoints3D[i],upperAngle+randomFloat*noiseAngle,offshootRootIndex[i]);
          float aaa=upperAngle+randomFloat*noiseAngle;
//          qDebug()<<"upper branch angle: "<<aaa/3.1415926*180;
          upperAngle+=upperUnitAngle;
        }else{
          offshoots[i]->constructSkeleton3D(glm::normalize(glm::vec3(topPoint3D-anchor3D)),anchorPoints3D[i],lowerAngle+randomFloat*noiseAngle,offshootRootIndex[i]);
          float aaa=lowerAngle+randomFloat*noiseAngle;
//          qDebug()<<"lower branch angle: "<<aaa/3.1415926*180;
          lowerAngle+=lowerUnitAngle;
        }
    }

}
void SkeletonNodeInfo::generateLeaves(const std::vector<Stroke>& profiles,std::vector<std::pair<QPoint,QPoint>>& newPetalInfo,float growthPointGap,float growthRate,int iterationNum){
  const Stroke& s=skeleton;
  const int originalOffshootSize=offshoots.size();
  std::random_device seed;
  std::default_random_engine randomEngine(seed());
  std::uniform_int_distribution<> dis(1, 2);
  for(auto& p:profiles){
      QVector<QPoint> profilePoints;
      for(auto& propoint:p)profilePoints.push_back(propoint);
      QPolygon profilePoly(profilePoints);
      if(profilePoly.containsPoint(s.front(),Qt::OddEvenFill)||profilePoly.containsPoint(s.back(),Qt::OddEvenFill)){
          Stroke growthPoints;
          QPoint lastPoint=s[0];
          for(int i=1;i<s.size();++i){
              if((manhattanDistance(lastPoint,s[i])<growthPointGap)&&((i+1)!=s.size()))continue;
              growthPoints.push_back(s[i]);
              lastPoint=s[i];
            }
          Stroke left,right;
          float max=-100000086;
          QVector2D branchDir(s[endIndex]-s[rootIndex]);
          branchDir.normalize();
          int index;
          for(int i=0;i<p.size();++i){
              QVector2D dir(p[i]-s[endIndex]);
              dir.normalize();
              float d=QVector2D::dotProduct(branchDir,dir);
              if(d>max){max=d;index=i;}
            }
          for(int i=0;i<=index;++i)left.push_back(p[i]);
          for(int i=index;i<p.size();++i)right.push_back(p[i]);
          for(auto& gp:growthPoints){
              if(!profilePoly.containsPoint(gp,Qt::OddEvenFill))continue;
              Stroke newBranch;
              newBranch.push_back(gp);
              QPoint newPosition=gp;
              Stroke* candidate;
              int rand=dis(randomEngine);
              if(rand>1){candidate=&right;}
              else{candidate=&left;}
              for(int i=0;i<iterationNum;++i){
                  //find 3 closest points of profile
                  float dis1,dis2,dis3;QPoint p1,p2,p3;
                  dis1=100086,dis2=100087,dis3=100088;
                  for(auto& pp:*candidate){
                      float dis=euclidDistance(gp,pp);
                      if(dis<dis1){
                          dis1=dis;p1=pp;
                        }else if(dis<dis2){
                          dis2=dis;p2=pp;
                        }else if(dis<dis3){
                          dis3=dis;p3=pp;
                        }
                    }
                  QVector2D v1(p1.x()-newPosition.x(),p1.y()-newPosition.y());v1.normalize();
                  QVector2D v2(p2.x()-newPosition.x(),p2.y()-newPosition.y());v2.normalize();
                  QVector2D v3(p3.x()-newPosition.x(),p3.y()-newPosition.y());v3.normalize();
                  QVector2D branchVec(s[endIndex]-s[rootIndex]);
                  branchVec.normalize();
                  QVector2D finalVec;
                  finalVec=0.5*branchVec+0.2*v1+0.2*v2;//+0.1*v3;
                  finalVec.normalize();
                  finalVec*=growthRate;
                  newPosition+=finalVec.toPoint();
                  if(profilePoly.containsPoint(newPosition,Qt::OddEvenFill)&&dis1>4)newBranch.push_back(newPosition);
                  else break;
                }
              newPetalInfo.push_back(std::make_pair(newBranch.back(),newBranch.front()));//pair<EndPoint,RootPoint>
            }
        }
    }
  for(int i=0;i<originalOffshootSize;++i){
      SkeletonNodeInfo* o=offshoots[i];
      o->generateLeaves(profiles,newPetalInfo,growthPointGap,growthRate,iterationNum);
    }
}

void SkeletonNodeInfo::generateSubBranch(const std::vector<Stroke>& profiles,float growthPointGap,float growthRate,int iterationNum){

  const Stroke& s=skeleton;
  const int originalOffshootSize=offshoots.size();
  std::random_device seed;
  std::default_random_engine randomEngine(seed());
  std::uniform_int_distribution<> dis(1, 2);

  for(auto& p:profiles){
      QVector<QPoint> profilePoints;
      for(auto& propoint:p)profilePoints.push_back(propoint);
      QPolygon profilePoly(profilePoints);
      if(profilePoly.containsPoint(s.front(),Qt::OddEvenFill)||profilePoly.containsPoint(s.back(),Qt::OddEvenFill)){
          Stroke growthPoints;
          QPoint lastPoint=s[0];
          for(int i=1;i<s.size();++i){
              if((manhattanDistance(lastPoint,s[i])<growthPointGap)&&((i+1)!=s.size()))continue;
              growthPoints.push_back(s[i]);
              lastPoint=s[i];
            }
          Stroke left,right;
          float max=-100000086;
          QVector2D branchDir(s[endIndex]-s[rootIndex]);
          branchDir.normalize();
          int index;
          for(int i=0;i<p.size();++i){
              QVector2D dir(p[i]-s[endIndex]);
              dir.normalize();
              float d=QVector2D::dotProduct(branchDir,dir);
              if(d>max){
                  max=d;
                  index=i;
                }
            }
          for(int i=0;i<=index;++i)left.push_back(p[i]);
          for(int i=index;i<p.size();++i)right.push_back(p[i]);
          qDebug()<<"index = "<<index<<" profile size="<<p.size();

          //generate subbranch for each growth point
          for(auto& gp:growthPoints){
              if(!profilePoly.containsPoint(gp,Qt::OddEvenFill))continue;
              Stroke newBranch;
              newBranch.push_back(gp);
              QPoint newPosition=gp;
              Stroke* candidate;
              int rand=dis(randomEngine);
              if(rand>1){
                  candidate=&right;
                }else{
                  candidate=&left;
                }
              for(int i=0;i<iterationNum;++i){

                  //find 3 closest points of profile
                  float dis1,dis2,dis3;QPoint p1,p2,p3;
                  dis1=100086,dis2=100087,dis3=100088;
                  for(auto& pp:*candidate){
                      float dis=euclidDistance(gp,pp);
                      if(dis<dis1){
                          dis1=dis;
                          p1=pp;
                        }else if(dis<dis2){
                          dis2=dis;
                          p2=pp;
                        }else if(dis<dis3){
                          dis3=dis;
                          p3=pp;
                        }
                    }
                  //calculate 3 direction
                  QVector2D v1(p1.x()-newPosition.x(),p1.y()-newPosition.y());
                  v1.normalize();
                  QVector2D v2(p2.x()-newPosition.x(),p2.y()-newPosition.y());
                  v2.normalize();
                  QVector2D v3(p3.x()-newPosition.x(),p3.y()-newPosition.y());
                  v3.normalize();
                  QVector2D branchVec(s[endIndex]-s[rootIndex]);
                  branchVec.normalize();
                  QVector2D finalVec;
                  finalVec=0.5*branchVec+0.2*v1+0.2*v2;//+0.1*v3;
                  finalVec.normalize();
                  finalVec*=growthRate;
                  //get new position
                  newPosition+=finalVec.toPoint();
                  if(profilePoly.containsPoint(newPosition,Qt::OddEvenFill)&&dis1>4)newBranch.push_back(newPosition);
                  else break;
                }

              if(newBranch.size()>3){
                  //create new branch
                  SkeletonNodeInfo* newNode=new SkeletonNodeInfo(newBranch);
                  offshoots.push_back(newNode);
                  //BUG rootIndex and endIndex are inverse!!!
                  //what supposed to be are (rootIndex=0,rootIndex=newBranch.size()-1)
                  newNode->rootIndex=newBranch.size()-1;
                  newNode->endIndex=0;
                  newNode->anchorPoint=gp;
                  newNode->r=r*0.25;
                  anchorPoints.push_back(gp);
                  offshootRootIndex.push_back(0);
                }
              else qDebug()<<"too short";
            }
        }
    }
  for(int i=0;i<originalOffshootSize;++i){
      SkeletonNodeInfo* o=offshoots[i];
      o->generateSubBranch(profiles,growthPointGap,growthRate,iterationNum);
    }
}
//for debug only
void SkeletonNodeInfo::generateSubBranch(const std::vector<Stroke>& profiles,std::vector<Stroke>& newBranches,float growthPointGap,float growthRate,int iterationNum){

  const Stroke& s=skeleton;
  const int originalOffshootSize=offshoots.size();
  std::random_device seed;
  std::default_random_engine randomEngine(seed());
  std::uniform_int_distribution<> dis(1, 2);

  for(auto& p:profiles){
      QVector<QPoint> profilePoints;
      for(auto& propoint:p)profilePoints.push_back(propoint);
      QPolygon profilePoly(profilePoints);
      if(profilePoly.containsPoint(s.front(),Qt::OddEvenFill)||profilePoly.containsPoint(s.back(),Qt::OddEvenFill)){
          Stroke growthPoints;
          QPoint lastPoint=s[0];
          for(int i=1;i<s.size();++i){
              if((manhattanDistance(lastPoint,s[i])<growthPointGap)&&((i+1)!=s.size()))continue;
              growthPoints.push_back(s[i]);
              lastPoint=s[i];
            }
          Stroke left,right;
          float max=-100000086;
          QVector2D branchDir(s[endIndex]-s[rootIndex]);
          branchDir.normalize();
          int index;
          for(int i=0;i<p.size();++i){
              QVector2D dir(p[i]-s[endIndex]);
              dir.normalize();
              float d=QVector2D::dotProduct(branchDir,dir);
              if(d>max){
                  max=d;
                  index=i;
                }
            }
          for(int i=0;i<=index;++i)left.push_back(p[i]);
          for(int i=index;i<p.size();++i)right.push_back(p[i]);
          qDebug()<<"index = "<<index<<" profile size="<<p.size();

          //generate subbranch for each growth point
          for(auto& gp:growthPoints){
              if(!profilePoly.containsPoint(gp,Qt::OddEvenFill))continue;
              Stroke newBranch;
              newBranch.push_back(gp);
              QPoint newPosition=gp;
              Stroke* candidate;
              int rand=dis(randomEngine);
              if(rand>1){
                  candidate=&right;
                }else{
                  candidate=&left;
                }
              for(int i=0;i<iterationNum;++i){

                  //find 3 closest points of profile
                  float dis1,dis2,dis3;QPoint p1,p2,p3;
                  dis1=100086,dis2=100087,dis3=100088;
                  for(auto& pp:*candidate){
                      float dis=euclidDistance(gp,pp);
                      if(dis<dis1){
                          dis1=dis;
                          p1=pp;
                        }else if(dis<dis2){
                          dis2=dis;
                          p2=pp;
                        }else if(dis<dis3){
                          dis3=dis;
                          p3=pp;
                        }
                    }
                  //calculate 3 direction
                  QVector2D v1(p1.x()-newPosition.x(),p1.y()-newPosition.y());
                  v1.normalize();
                  QVector2D v2(p2.x()-newPosition.x(),p2.y()-newPosition.y());
                  v2.normalize();
                  QVector2D v3(p3.x()-newPosition.x(),p3.y()-newPosition.y());
                  v3.normalize();
                  QVector2D branchVec(s[endIndex]-s[rootIndex]);
                  branchVec.normalize();
                  QVector2D finalVec;
                  finalVec=0.6*branchVec+0.2*v1+0.2*v2;//+0.1*v3;
                  finalVec.normalize();
                  finalVec*=growthRate;
                  //get new position
                  newPosition+=finalVec.toPoint();
                  if(profilePoly.containsPoint(newPosition,Qt::OddEvenFill)&&dis1>4)newBranch.push_back(newPosition);
                  else break;
                }

              if(newBranch.size()>3){
                  newBranches.push_back(newBranch);
                }
              else qDebug()<<"too short";
            }
        }
    }
  for(int i=0;i<originalOffshootSize;++i){
      SkeletonNodeInfo* o=offshoots[i];
      o->generateSubBranch(profiles,growthPointGap,growthRate,iterationNum);
    }
}

//---------------------Floret3D-------------------------
Floret3D::Floret3D(Floret petalStrokes2D,QPointF worldOrigin,float mapScale){
  qDebug()<<"<<---------------------construct Floret3D------------------------>>";
  qDebug()<<"there are "<<petalStrokes2D.size()<<" petal(s) in this floret";
  //calculate ellipse formula and flower center
  std::vector<QPoint> farPoints;
  float cx,cy;
  cx=cy=0;
  int count=0;
  for(auto &l:petalStrokes2D){
      farPoints.push_back(l.farPoint);
      cx+=l.rootPoint.x();
      cy+=l.rootPoint.y();
      ++count;
  }
  qDebug()<<"------------getEllipse------------";
  cv::RotatedRect ellipse;
  ellipse=getFittedEllipse(farPoints);

  //get flowerCenter(relative coordinate) and worldOffset(global coordinate)
  QPointF flowerCenter(cx/count,cy/count);
  glm::vec3 worldOffset(flowerCenter.x()-worldOrigin.x(),worldOrigin.y()-flowerCenter.y(),0);


  //calculate 3D cone
  ConeInfo cone;
  cone.radius=std::min(ellipse.size.width,ellipse.size.height)/2;
  float mainAxisGroundAngle=std::asin(std::min(ellipse.size.width,ellipse.size.height)/std::max(ellipse.size.width,ellipse.size.height));
  cone.height=std::sqrt((ellipse.center.x-flowerCenter.x())*(ellipse.center.x-flowerCenter.x())+(ellipse.center.y-flowerCenter.y())*(ellipse.center.y-flowerCenter.y()))/std::cos(mainAxisGroundAngle);
  cone.mainAxis=glm::vec3(ellipse.center.x-flowerCenter.x(),flowerCenter.y()-ellipse.center.y,cone.height*std::sin(mainAxisGroundAngle));
  getRotateAxisAndAngle(cone.mainAxis,cone.rotateAxis,cone.rotateRadian);

  qDebug()<<"height: "<<cone.height<<"  radius: "<<cone.radius;
  qDebug()<<"angle in degree: "<<mainAxisGroundAngle/3.1415926*180;
  qDebug()<<"mainAxis: "<<cone.mainAxis.x<<" , "<<cone.mainAxis.y<<" , "<<cone.mainAxis.z;
  glm::mat4 rotateMat(1.0f);
  rotateMat=glm::rotate(rotateMat,cone.rotateRadian,cone.rotateAxis);
  //rotateMat=
  // | A B C 0 |
  // | D E F 0 |
  // | G H I 0 |
  // | 0 0 0 1 |
  float A,B,C,D,E,F,G,H,I,alpha,gamma,beta,U,V,W;
  const float h=cone.height;
  const float r=cone.radius;
  A=rotateMat[0].x; D=rotateMat[0].y; G=rotateMat[0].z;
  B=rotateMat[1].x; E=rotateMat[1].y; H=rotateMat[1].z;
  C=rotateMat[2].x; F=rotateMat[2].y; I=rotateMat[2].z;
  //formula: (Gx+Hy+Iz)^2=(h/r)^2*((Ax+By+Cz)^2+(Dx+Ey+Fz)^2)
  //         (gamma+Iz)^2=(h/r)^2*((alpha+Cz)^2+(beta+Fz)^2)
  //         gamma= Gx+Hy       alpha=Ax+By        beta=Dx+Ey
  //reduce formula
  //         let alpha=(h/r)*alpha, C=(h/r)*C, beta=(h/r)*beta, F=(h/r)*F
  //         (gamma+Iz)^2=(alpha+Cz)^2+(beta+Fz)^2
  //          gamma^2 + 2gammaIz + I^2*z^2=alpha^2 + 2alpha*Cz + C^2*z^2+  beta^2 + 2betaFz+ F^2*z^2
  //          ---->  (C^2+F^2-I^2)*z^2 + (2alpha*C + 2betaF - 2gammaI)*z + (alpha^2 + beta^2 - gamma^2 ) = 0
  //reduce formula
  //          let U=(C^2+F^2-I^2)    V=(2alpha*C + 2betaF - 2gammaI)  W=(alpha^2 + beta^2 - gamma^2 )
  //          ---->   U*z^2 + V*z + W = 0

  for(auto &l:petalStrokes2D)l.translate(QPoint(flowerCenter.x(),flowerCenter.y()));
  for(auto &l:petalStrokes2D){
//      l=meshTessellationAsGap(l,4,4);
      l=meshTessellationAsDiv(l,10,5);
  }
  std::vector<PetalStroke3D> petalStrokes3D;
  for(auto& l:petalStrokes2D){
      PetalStroke3D stroke3D;
      for(auto& p:l.stroke){
          gamma= G*p.x()+H*p.y();alpha=A*p.x()+B*p.y();beta=D*p.x()+E*p.y();
          alpha=(h/r)*alpha; C=(h/r)*C; beta=(h/r)*beta; F=(h/r)*F;
          U=(C*C+F*F-I*I);    V=(2*alpha*C + 2*beta*F - 2*gamma*I);  W=(alpha*alpha + beta*beta - gamma*gamma );
          float z=-(-V + sqrt(V*V-4*U*W) ) / (2*U);
//          qDebug()<<" x:"<<p.x()<<" y:"<<p.y()<<" z:"<<z;
          stroke3D.stroke.push_back(glm::vec3(p.x(),p.y(), z)*mapScale+worldOffset*mapScale);
        }
      //get vec3 of contour
      for(auto& p:l.contour){
          gamma= G*p.x()+H*p.y();alpha=A*p.x()+B*p.y();beta=D*p.x()+E*p.y();
          alpha=(h/r)*alpha; C=(h/r)*C; beta=(h/r)*beta; F=(h/r)*F;
          U=(C*C+F*F-I*I);    V=(2*alpha*C + 2*beta*F - 2*gamma*I);  W=(alpha*alpha + beta*beta - gamma*gamma );
          float z=-(-V + sqrt(V*V-4*U*W) ) / (2*U);
          stroke3D.contour.push_back(glm::vec3(p.x(),p.y(), z)*mapScale+worldOffset*mapScale);
        }
      {//get vec3 of rootPoint
        QPoint p=l.rootPoint;
        gamma= G*p.x()+H*p.y();alpha=A*p.x()+B*p.y();beta=D*p.x()+E*p.y();
        alpha=(h/r)*alpha; C=(h/r)*C; beta=(h/r)*beta; F=(h/r)*F;
        U=(C*C+F*F-I*I);    V=(2*alpha*C + 2*beta*F - 2*gamma*I);  W=(alpha*alpha + beta*beta - gamma*gamma );
        float z=-(-V + sqrt(V*V-4*U*W) ) / (2*U);
        stroke3D.rootPoint=glm::vec3(p.x(),p.y(), z)*mapScale+worldOffset*mapScale;
      }
      {//get vec3 of farPoint
        QPoint p=l.farPoint;
        gamma= G*p.x()+H*p.y();alpha=A*p.x()+B*p.y();beta=D*p.x()+E*p.y();
        alpha=(h/r)*alpha; C=(h/r)*C; beta=(h/r)*beta; F=(h/r)*F;
        U=(C*C+F*F-I*I);    V=(2*alpha*C + 2*beta*F - 2*gamma*I);  W=(alpha*alpha + beta*beta - gamma*gamma );
        float z=-(-V + sqrt(V*V-4*U*W) ) / (2*U);
        stroke3D.farPoint=glm::vec3(p.x(),p.y(), z)*mapScale+worldOffset*mapScale;
      }
      stroke3D.worldOffset=worldOffset*mapScale;
      //NOTE may need to convert mainAxis to 3D world coordinate
      stroke3D.mainOrient=cone.mainAxis;
      petalStrokes3D.push_back(stroke3D);
    }
  //make petals closer to flower center
  glm::vec3 flowerCenter3D(0);
  for(auto& petal:petalStrokes3D){
      flowerCenter3D+=petal.rootPoint;
    }
  flowerCenter3D/=petalStrokes3D.size();
  for(auto& petal:petalStrokes3D){
      glm::vec3 offset=(flowerCenter3D-petal.rootPoint);
      offset*=0.9;
      for(auto& p:petal.stroke){
          p+=offset;
        }
      for(auto& p:petal.contour){
          p+=offset;
        }
      petal.farPoint+=offset;
      petal.rootPoint+=offset;
    }

  //save result
  petals2D=petalStrokes2D;
  petals3D=petalStrokes3D;
  coneInfo=cone;
  floretCenter=worldOffset*mapScale;
  qDebug()<<"------------now random rotate------------";
  petalRandomRotate();
  qDebug()<<"<<------------construction complete------------>>";
}
void Floret3D::adjustOrient(){
  const float PI=3.14;
  glm::vec3 orient=coneInfo.mainAxis;
  glm::vec2 orientXZ(orient.x,orient.z);
  orientXZ=glm::normalize(orientXZ);
  float angleXZ=std::acos(orientXZ.x);


  glm::vec2 orientYZ(orient.y,orient.z);
  orientYZ=glm::normalize(orientYZ);
  float angleYZ=std::acos(orientYZ.x);


  for(auto& petal:petals3D){
      petal.mat=glm::rotate(petal.mat,glm::radians(90.0f),glm::vec3(-1,0,0));
    }
}

void Floret3D::bendPetal(float factor){
  glm::vec3 movingVec=glm::normalize(coneInfo.mainAxis);
  for(auto& petal3D:petals3D){
      glm::vec3 axis=petal3D.farPoint-petal3D.rootPoint;
      glm::vec3 axisNormalized=glm::normalize(axis);
      float dis=glm::dot(axis,axisNormalized);
      for(auto& p:petal3D.stroke){
          glm::vec3 dir=p-petal3D.rootPoint;
          float x=glm::dot(axisNormalized,dir)/dis;
          float offset=0.1*(x*x*x*x+x*x)*dis*factor;
          glm::mat4 transMat(1.0f);
          transMat=glm::translate(transMat,-movingVec*offset);
          p=glm::vec3(transMat*glm::vec4(p,1.0));
        }
    }
}

void Floret3D::cavePetal(float factor){
  auto f=[](float t)->float{return t*(1-t)*0.7;};
  auto g=[](float t)->float{return (-t*t+1)*0.6;};
  auto clamp=[](float v)->float{if(v>1)return 1;else if(v<0)return 0;else return v;};

  glm::vec3 movingVec=glm::normalize(coneInfo.mainAxis);
  for(int i=0;i<petals3D.size();++i){
      auto& petal3D=petals3D[i];
      glm::vec3 axis(petal3D.farPoint-petal3D.rootPoint);
      float VMax=std::sqrt(glm::dot(axis,axis));
      float maxDepth=0.4*VMax;

      auto& petal2D=petals2D[i];
      for(int j=0;j<petal3D.stroke.size();++j){
          auto& p=petal3D.stroke[j];
          float tV,tU;
          if(petal2D.tVtU.find(petal2D.stroke[j])!=petal2D.tVtU.end()){
              tV=clamp(petal2D.tVtU[petal2D.stroke[j]].first);
              tU=clamp(petal2D.tVtU[petal2D.stroke[j]].second);
            }else{
              continue;
            }
          float offset=f(tV)*maxDepth*g(tU)*factor;
          glm::mat4 transMat(1.0f);
          transMat=glm::translate(transMat,-movingVec*offset);
          p=glm::vec3(transMat*glm::vec4(p,1.0));
        }
    }
}
void Floret3D::bendAndCavePetal(float bendFactor,float caveFactor){
  //for cave
  auto f=[](float t)->float{return t*(1-t)*0.6;};
  auto g=[](float t)->float{return (-t*t+1)*0.6;};
  auto clamp=[](float v)->float{if(v>1)return 1;else if(v<0)return 0;else return v;};
  //for common
  glm::vec3 movingVec=glm::normalize(coneInfo.mainAxis);

  for(int i=0;i<petals3D.size();++i){
      auto& petal3D=petals3D[i];
      glm::vec3 axis=petal3D.farPoint-petal3D.rootPoint;
      glm::vec3 axisNormalized=glm::normalize(axis);
      float dis=glm::dot(axis,axisNormalized);
      //for cave
      float maxDepth=0.4*dis;

      auto& petal2D=petals2D[i];
      for(int j=0;j<petal3D.stroke.size();++j){
          float offset=0;
          //for cave
          auto& p=petal3D.stroke[j];
          float tV,tU;
          if(petal2D.tVtU.find(petal2D.stroke[j])!=petal2D.tVtU.end()){
              tV=clamp(petal2D.tVtU[petal2D.stroke[j]].first);
              tU=clamp(petal2D.tVtU[petal2D.stroke[j]].second);
              offset=f(tV)*maxDepth*g(tU)*caveFactor;
            }
          //for bend
          glm::vec3 dir=p-petal3D.rootPoint;
          float x=glm::dot(axisNormalized,dir)/dis;

          offset+=0.1*(x*x*x*x+x*x)*dis*bendFactor;

          glm::mat4 transMat(1.0f);
          transMat=glm::translate(transMat,-movingVec*offset);
          p=glm::vec3(transMat*glm::vec4(p,1.0));
        }
    }
}

std::vector<std::pair<float,float>> getPetalWidthInfo(const PetalStroke& petal){
  std::vector<std::pair<float,float>> widthInfo;
  glm::vec2 axis(petal.farPoint.x()-petal.rootPoint.x(),petal.farPoint.y()-petal.rootPoint.y());
  glm::vec2 axisNormalized=glm::normalize(axis);

  for(auto& p:petal.contour){
      glm::vec2 dir(p.x()-petal.rootPoint.x(),p.y()-petal.rootPoint.y());
      float V=glm::dot(dir,axisNormalized);
      float U=std::sqrt(glm::dot(dir,dir)-V*V);
      widthInfo.push_back(std::make_pair(V,U));
    }
  std::sort(widthInfo.begin(),widthInfo.end(),[](const std::pair<float,float>& p1,const std::pair<float,float>& p2)->bool{
      return p1.first<p2.first;
    });
  return widthInfo;
}
std::vector<std::pair<float,float>> getPetalWidthInfo3D(const PetalStroke3D& petal){
  std::vector<std::pair<float,float>> widthInfo;
  glm::vec3 axis(petal.farPoint-petal.rootPoint);
  glm::vec3 axisNormalized=glm::normalize(axis);

  for(auto& p:petal.contour){
      glm::vec3 dir(p.x-petal.rootPoint.x,p.y-petal.rootPoint.y,p.z-petal.rootPoint.z);
      float V=glm::dot(dir,axisNormalized);
      float U=std::sqrt(glm::dot(dir,dir)-V*V);
      widthInfo.push_back(std::make_pair(V,U));
    }
  std::sort(widthInfo.begin(),widthInfo.end(),[](const std::pair<float,float>& p1,const std::pair<float,float>& p2)->bool{
      return p1.first<p2.first;
    });
  return widthInfo;
}
void Floret3D::petalRandomRotate(){
  glm::vec3 coneAxis=coneInfo.mainAxis;

  std::random_device seed;
  std::default_random_engine randomEngine(seed());
  std::uniform_real_distribution<> dis(-0.1, 0.8);

  for(auto& petal:petals3D){
      glm::vec3 petalAxis=petal.farPoint-petal.rootPoint;
      float randomNum=dis(randomEngine);
      float randomAngle=3.1415926/9*randomNum;
      qDebug()<<"random angle: "<<randomNum*30;
      glm::mat4 rotateMat(1);
      rotateMat=glm::rotate(rotateMat,randomAngle,glm::cross(petalAxis,coneAxis));
      for(auto& p:petal.stroke){
         auto temp=rotateMat*glm::vec4(p-petal.rootPoint,1.0f);
         p=glm::vec3(temp)+petal.rootPoint;
        }
    }
}

//-----------------------FUNCTIONS----------------------
std::vector<Floret> meanShiftClassify(const std::vector<PetalStroke>& leaves,float r){
  constexpr int CLASS_MAX_NUM=50;//max cluster num
  const float CLASS_BIAS=5;
  const int POINT_NUM=leaves.size();

  int(*belongings)[CLASS_MAX_NUM]=new int[POINT_NUM][CLASS_MAX_NUM];
  std::vector<QPoint> rootPoints;
  std::vector<QPoint> classCenters;//convergence center of each class
  std::vector<bool> visited;
  int visitedNum=0;
  int classNum=0;



  for(int i=0;i<POINT_NUM;++i){
      rootPoints.push_back(leaves[i].rootPoint);
      for(int j=0;j<CLASS_MAX_NUM;++j)belongings[i][j]=0;
      visited.push_back(false);
    }

  while(visitedNum<POINT_NUM){
      QPoint center;
      for(int i=0;i<POINT_NUM;++i){
          if(!visited[i]){
              center=rootPoints[i];
              break;
            }
        }

      int pointNumInROld=0;
      int pointNumInRNew=0;
      QPoint centerOld;
      std::vector<int> pointIndexInClass;
      do{
          pointNumInROld=pointNumInRNew;
          centerOld=center;

          std::vector<QPoint> pointInR;
          //find points near startIndex point
          for(int i=0;i<POINT_NUM;++i){
              if(euclidDistance(center,rootPoints[i])<r){
                  visitedNum+=visited[i]?0:1;
                  visited[i]=true;
                  pointInR.push_back(rootPoints[i]);
                  pointIndexInClass.push_back(i);
                }
            }
          //get new center
          float x,y;x=y=0;
          for(auto p:pointInR){
              x+=p.x();
              y+=p.y();
            }
          center=QPoint(x/pointInR.size(),y/pointInR.size());
          pointNumInRNew=pointInR.size();
        }while(pointNumInRNew>pointNumInROld);

      int classIndex=classNum++;
      if(classCenters.empty()){
          classCenters.push_back(centerOld);
        }
      else{
          for(int i=0;i<classCenters.size();++i){
              if(euclidDistance(centerOld,classCenters[i])<CLASS_BIAS){//found similar classes, merge them
                  classIndex=i;
                  --classNum;
                  break;
                }
            }

        }
      for(int j=0;j<pointIndexInClass.size();++j){
          ++belongings[pointIndexInClass[j]][classIndex];
        }
    }
  //classify strokes!
  std::vector<Floret> cluster;
  cluster.resize(classNum);
  for(int i=0;i<POINT_NUM;++i){
      int maxNum=0;
      int maxClass=0;
      for(int j=0;j<classNum;++j){
          if(belongings[i][j]>maxNum){
              maxNum=belongings[i][j];
              maxClass=j;
            }
        }
      cluster[maxClass].push_back(leaves[i]);
    }
  qDebug()<<classNum<<" florets found!";
  delete[] belongings;
  return cluster;
}



void setConnected(const QPoint& p1,const QPoint& p2,std::map<QPoint, int> &indexMap, std::vector<std::vector<bool> > &adjMatrix){
  adjMatrix[indexMap[p1]][indexMap[p2]]=true;
  adjMatrix[indexMap[p2]][indexMap[p1]]=true;
//  qDebug()<<"line(["<<p1.x()<<","<<p2.x()<<"],["<<p1.y()<<","<<p2.y()<<"],\'color\',\'r\');";
//  qDebug()<<"connect "<<p1<<" and "<<p2;
}
void setDisconnected(const QPoint& p1,const QPoint& p2,std::map<QPoint, int> &indexMap, std::vector<std::vector<bool> > &adjMatrix){
  adjMatrix[indexMap[p1]][indexMap[p2]]=false;
  adjMatrix[indexMap[p2]][indexMap[p1]]=false;
//  qDebug()<<"line(["<<p1.x()<<","<<p2.x()<<"],["<<p1.y()<<","<<p2.y()<<"],\'color\',\'w\');";
//  qDebug()<<"disconnect "<<p1<<" and "<<p2;
}
TriangleNet delaunayTriangulation(const std::vector<QPoint> &points, std::map<QPoint, int> &indexMap, std::vector<std::vector<bool>> &adjMatrix){
  if(points.size()==2){
      TriangleNet s;
      for(auto& p:points)s.points.push_back(p);
      s.edges.push_back(QLine(points[0],points[1]));
      setConnected(points[0],points[1],indexMap,adjMatrix);
      return s;
    }else if(points.size()==3){
      TriangleNet s;
      for(auto& p:points)s.points.push_back(p);

      //collinear case  k=+infinite
      if(points[0].x()==points[2].x()){//NOTE that p0.x<=p1.x<=p2.x,if p0.x=p2.x thus p p0.x=p1.x=p2.x
          s.edges.push_back(QLine(points[0],points[1]));
          s.edges.push_back(QLine(points[1],points[2]));

          setConnected(points[0],points[1],indexMap,adjMatrix);
          setConnected(points[1],points[2],indexMap,adjMatrix);
        }else{
          float k1=(points[1].y()-points[0].y())/static_cast<float>(points[1].x()-points[0].x());
          float k2=(points[2].y()-points[0].y())/static_cast<float>(points[2].x()-points[0].x());
          if(k1==k2){//collinear case k!=infinite
              s.edges.push_back(QLine(points[0],points[1]));
              s.edges.push_back(QLine(points[1],points[2]));

              setConnected(points[0],points[1],indexMap,adjMatrix);
              setConnected(points[1],points[2],indexMap,adjMatrix);
            }else{//normal case
              s.edges.push_back(QLine(points[0],points[1]));
              s.edges.push_back(QLine(points[0],points[2]));
              s.edges.push_back(QLine(points[1],points[2]));

              setConnected(points[0],points[1],indexMap,adjMatrix);
              setConnected(points[0],points[2],indexMap,adjMatrix);
              setConnected(points[1],points[2],indexMap,adjMatrix);
            }
        }

      return s;
    }else if(points.size()<2)return TriangleNet();

  int mid;
  if(points.size()&0x1)mid=points.size()/2+1;
  else mid=points.size()/2;
  std::vector<QPoint> leftPoints(points.begin(),points.begin()+mid);
  std::vector<QPoint> rightPoints(points.begin()+mid,points.end());

  TriangleNet left=delaunayTriangulation(leftPoints,indexMap,adjMatrix);
  TriangleNet right=delaunayTriangulation(rightPoints,indexMap,adjMatrix);



  //at same y value , outer is priority,      left-----------> + <-----------right
  std::sort(leftPoints.begin(),leftPoints.end(),[](QPoint p1,QPoint p2)->bool{ if(p1.y()==p2.y())return p1.x()<p2.x();else return p1.y()<p2.y();});
  std::sort(rightPoints.begin(),rightPoints.end(),[](QPoint p1,QPoint p2)->bool{if(p1.y()==p2.y())return p1.x()>p2.x();else return p1.y()<p2.y();});

  QPoint baselineLeft(-10000000,1000000),baselineRight(10000000,1000000);

  //find bottom most LR edge new version
  float leftYMin=10e10;
  float rightYMin=10e10;
  for(auto& p:leftPoints)if(leftYMin>p.y())leftYMin=p.y();
  for(auto& p:rightPoints)if(rightYMin>p.y())rightYMin=p.y();

  if(leftYMin>rightYMin){
      QVector2D toLeft(-1,0);
      QPoint base(10e10,10e10);
      bool minAngleInLeft=true;
      //set base as bottom most and left most point of rightPoints
      for(auto& p:rightPoints){
          if((p.y()<base.y())||((p.y()==base.y())&&(p.x()<base.x())))base=p;
        }
      QPoint candidate=base;
      do{
          float maxCos=-10;
          base=candidate;
          for(auto& p:leftPoints){
              float a=QVector2D::dotProduct(toLeft,QVector2D(p-base).normalized());
              if(a>maxCos){
                  maxCos=a;
                  candidate=p;
                  minAngleInLeft=true;
                }
            }
          for(auto& p:rightPoints){
              //避免right和left共线的情况，right的x与left的x相同
              if((p.x()>base.x())||(p==base)||(p.y()<base.y()))continue;
              float a=QVector2D::dotProduct(toLeft,QVector2D(p-base).normalized());
              if(a>=maxCos){
                  maxCos=a;
                  candidate=p;
                  minAngleInLeft=false;
                }
            }
        }while(!minAngleInLeft);
      baselineRight=base;
      baselineLeft=candidate;

    }else if(leftYMin<rightYMin){
      QVector2D toRight(1,0);
      QPoint base(10e10,10e10);
      bool minAngleInRight=true;
      //set base as bottom most and right most point of leftPoints
      for(auto& p:leftPoints){
          if((p.y()<base.y())||((p.y()==base.y())&&(p.x()>base.x())))base=p;
        }
      QPoint candidate=base;
      do{
          float maxCos=-10;
          base=candidate;
          for(auto& p:rightPoints){
              float a=QVector2D::dotProduct(toRight,QVector2D(p-base).normalized());
              if(a>maxCos){
                  maxCos=a;
                  candidate=p;
                  minAngleInRight=true;
                }
            }
          for(auto& p:leftPoints){
              //避免right和left共线的情况，right的x与left的x相同
              if((p.x()<base.x())||(p==base)||(p.y()<base.y()))continue;
              float a=QVector2D::dotProduct(toRight,QVector2D(p-base).normalized());
              if(a>=maxCos){
                  maxCos=a;
                  candidate=p;
                  minAngleInRight=false;
                }
            }
        }while(!minAngleInRight);
      baselineLeft=base;
      baselineRight=candidate;
    }else{
      // ymin(left)=ymin(right)
      //finde right and bottom most of leftPoints  ->
      for(auto& p:leftPoints){
          if(p.y()<baselineLeft.y())baselineLeft=p;
          else if((p.y()==baselineLeft.y())&&(p.x()>baselineLeft.x()))baselineLeft=p;
        }
      //find left and bottom most of RightPoints   <-
      for(auto& p:rightPoints){
          if(p.y()<baselineRight.y())baselineRight=p;
          else if((p.y()==baselineRight.y())&&(p.x()<baselineRight.x()))baselineRight=p;
        }
    }

  TriangleNet s;
  s.points.insert(s.points.end(),left.points.begin(),left.points.end());
  s.points.insert(s.points.end(),right.points.begin(),right.points.end());

  s.edges.insert(s.edges.end(),left.edges.begin(),left.edges.end());
  s.edges.insert(s.edges.end(),right.edges.begin(),right.edges.end());

  s.edges.push_back(QLine(baselineLeft,baselineRight));
  setConnected(baselineLeft,baselineRight,indexMap,adjMatrix);


  do{
      QVector2D leafToRight(baselineRight-baselineLeft);
      QVector2D rightToLeft=-leafToRight;

      //find candidate in left
      std::vector<QPoint> leftCandidates;
      for(int i=0;i<left.points.size();++i){
          if(adjMatrix[indexMap[leftPoints[i]]][indexMap[baselineLeft]]){
              QVector2D leftToCandidate(leftPoints[i]-baselineLeft);
              if(QVector2DCross(leafToRight,leftToCandidate)<=0)continue;
              leftCandidates.push_back(leftPoints[i]);
            }
        }

      //sort candidate
      sort(leftCandidates.begin(),leftCandidates.end(),[&](QPoint p1,QPoint p2)->bool{
        QVector2D lc1(p1-baselineLeft);QVector2D lc2(p2-baselineLeft);
        lc1.normalize();lc2.normalize();
        return QVector2D::dotProduct(leafToRight,lc1)>QVector2D::dotProduct(leafToRight,lc2);
      });
      //exclude bad candidate
      std::vector<QPoint> finalLeftCandidate;
      if(leftCandidates.size()>=2){
          int candIndex;
          for(candIndex=0;candIndex<leftCandidates.size()-1;++candIndex){
              if(inCircumCircle(baselineLeft,baselineRight,leftCandidates[candIndex],leftCandidates[candIndex+1])){
                  //delete edge(bad candidate----LR endpoint)
                  setDisconnected(leftCandidates[candIndex],baselineLeft,indexMap,adjMatrix);
                  for(int i=0;i<s.edges.size();++i){
                      if((s.edges[i].p1()==leftCandidates[candIndex]&&s.edges[i].p2()==baselineLeft)||(s.edges[i].p2()==leftCandidates[candIndex]&&s.edges[i].p1()==baselineLeft))
                        {
                          s.edges.erase(s.edges.begin()+i);
                          break;
                        }
                    }//TODO. may need delete this edge in left.edges
                }else break;
            }
          finalLeftCandidate.push_back(leftCandidates[candIndex]);
        }else finalLeftCandidate=leftCandidates;//finalLeftCandidate may be empty may contain only one point


      //find candidate in right
      std::vector<QPoint> rightCandidates;
      for(int i=0;i<right.points.size();++i){
          if(adjMatrix[indexMap[rightPoints[i]]][indexMap[baselineRight]]){
              QVector2D rightToCandidate(rightPoints[i]-baselineRight);
              if(QVector2DCross(rightToLeft,rightToCandidate)>=0)continue;
              rightCandidates.push_back(rightPoints[i]);
            }
        }
      //sort candidate
      sort(rightCandidates.begin(),rightCandidates.end(),[&](QPoint p1,QPoint p2)->bool{
        QVector2D rc1(p1-baselineRight);QVector2D rc2(p2-baselineRight);
        rc1.normalize();rc2.normalize();
        return QVector2D::dotProduct(rightToLeft,rc1)>QVector2D::dotProduct(rightToLeft,rc2);
      });
      //exclude bad candidate
      std::vector<QPoint> finalRightCandidate;
      if(rightCandidates.size()>=2){
          int candIndex;
          for(candIndex=0;candIndex<rightCandidates.size()-1;++candIndex){
              if(inCircumCircle(baselineLeft,baselineRight,rightCandidates[candIndex],rightCandidates[candIndex+1])){
                  //delete edge(bad candidate----LR endpoint)
                  setDisconnected(rightCandidates[candIndex],baselineRight,indexMap,adjMatrix);
                  for(int i=0;i<s.edges.size();++i){
                      if((s.edges[i].p1()==rightCandidates[candIndex]&&s.edges[i].p2()==baselineRight)||(s.edges[i].p2()==rightCandidates[candIndex]&&s.edges[i].p1()==baselineRight))
                        {
                          s.edges.erase(s.edges.begin()+i);
                          break;
                        }
                    }//TODO. may need delete this edge in right.edges
                }else break;
            }
          finalRightCandidate.push_back(rightCandidates[candIndex]);
        }else finalRightCandidate=rightCandidates;//finalRightCandidate may be empty may contain only one point


//      qDebug()<<"finalRightCandidate size: "<<finalRightCandidate.size()<<"finalLeftCandidate size: "<<finalLeftCandidate.size();
//      qDebug()<<" LR left: "<<baselineLeft<<" LR right: "<<baselineRight;
      //now find the new LR edge
      if(finalRightCandidate.empty()&&finalLeftCandidate.empty()){
          //rightCandidate and leftCandidate are both empty, then we terminate loop and return
          return s;
        }else if(finalLeftCandidate.empty()){
          //there is only rightCandidate
          s.edges.push_back(QLine(finalRightCandidate.front(),baselineLeft));
          setConnected(finalRightCandidate.front(),baselineLeft,indexMap,adjMatrix);
          baselineRight=finalRightCandidate.front();
        }else if(finalRightCandidate.empty()){
          //there is only leftCandidate
          s.edges.push_back(QLine(finalLeftCandidate.front(),baselineRight));
          setConnected(finalLeftCandidate.front(),baselineRight,indexMap,adjMatrix);
          baselineLeft=finalLeftCandidate.front();
        }else{
          if(inCircumCircle(baselineLeft,baselineRight,finalLeftCandidate.front(),finalRightCandidate.front())){
              s.edges.push_back(QLine(finalRightCandidate.front(),baselineLeft));
              setConnected(finalRightCandidate.front(),baselineLeft,indexMap,adjMatrix);
              baselineRight=finalRightCandidate.front();
            }else{
              s.edges.push_back(QLine(finalLeftCandidate.front(),baselineRight));
              setConnected(finalLeftCandidate.front(),baselineRight,indexMap,adjMatrix);
              baselineLeft=finalLeftCandidate.front();

            }
        }

    }while(true);

  return s;
}

std::vector<Triangle> getTriangleFromNet(TriangleNet net, std::map<QPoint, int> &indexMap, std::vector<std::vector<bool> > &adjMatrix){
  std::vector<Triangle> triangles;
//  sortPointsAsXAscend(net.points);
  std::sort(net.points.begin(),net.points.end(),[](const QPoint& p1,const QPoint& p2)->bool{
      if(p1.y()==p2.y())return p1.x()<p2.x();
      else return p1.y()>p2.y();
    });
  const std::vector<QPoint>& points=net.points;
//  qDebug()<<"----------------start find triangles-------------------";
  for(int i=0;i<points.size();++i){
      //find neighbors
      std::vector<QPoint> neighbors;
      QVector2D toRight(1,0);
      std::map<QPoint,float> cosMap;
      for(int j=0;j<points.size();++j)
        if(adjMatrix[indexMap[points[i]]][indexMap[points[j]]]&&(i!=j)){

            neighbors.push_back(points[j]);
            QVector2D vec(points[j].x()-points[i].x(),points[j].y()-points[i].y());
            vec.normalize();
            cosMap[points[j]]=QVector2D::dotProduct(vec,toRight);
          }
      auto currentPoint=points[i];
      //sort neighbors as y ascend then x ascend
      sort(neighbors.begin(),neighbors.end(),[&](QPoint p1,QPoint p2)->bool{return cosMap[p1]<cosMap[p2];});
      for(int j=0;j<neighbors.size();++j){
          auto currentNeighbor=points[j];

          for(int k=j+1;k<neighbors.size();++k){
              if(adjMatrix[indexMap[neighbors[j]]][indexMap[neighbors[k]]]){
                int i1,i2,i3;
                i1=indexMap[points[i]];
                i2=indexMap[neighbors[j]];
                i3=indexMap[neighbors[k]];
                if(i1==i2||i1==i3||i2==i3){
                    qDebug()<<" same vertex found in a face!!!";
                    qDebug()<<"i1 "<<i1<<" i2 "<<i2<<" i3 "<<i3<<"-- i="<<i<<" j="<<j<<" k="<<k;
                    debugQPoint(points[i],"points i");
                    debugQPoint(neighbors[j],"neighbor[j]");
                    debugQPoint(neighbors[k],"neighbors[k]");
                  }else triangles.emplace_back(i1,i2,i3);
                }
            }
          setDisconnected(points[i],neighbors[j],indexMap,adjMatrix);
        }

    }
  return triangles;
}
bool existPointBelow(QPointF leftMost, QPointF bottomMost, std::vector<QPoint> points){
  if(leftMost.y()==bottomMost.y()){
      for(auto& p:points){
          if(p.y()<bottomMost.y())return true;
        }
      return false;
    }
  else{
      const float k=(bottomMost.y()-leftMost.y())/(bottomMost.x()-leftMost.x());
      const float b=bottomMost.y()-k*bottomMost.x();
      //y=k*x+b
      for(auto& p:points){
          if(p.y()<k*p.x()+b)return true;
        }
      return false;
    }
}
PetalStroke meshTessellationAsDiv(const PetalStroke &contour,int axisPointNum, int perpendAxisPointNum){
  PetalStroke tessellation=contour;
  if(contour.stroke.empty())return tessellation;
  std::vector<std::pair<QPoint,float>> left;
  std::vector<std::pair<QPoint,float>> right;
  QVector2D axisVec(contour.farPoint-contour.rootPoint);
  float axisLength=std::sqrt(QVector2D::dotProduct(axisVec,axisVec));
  float axisGap=axisLength/axisPointNum;
  axisVec.normalize();
  for(auto& p:contour.stroke){
      float cross=QVector2DCross(axisVec,QVector2D(p-contour.rootPoint));
      float dot=QVector2D::dotProduct(axisVec,QVector2D(p-contour.rootPoint));
      const float zero=0.0001;
      if(cross>zero){//left
          left.push_back(std::make_pair(p,dot));
        }else if(cross<zero){//right
          right.push_back(std::make_pair(p,dot));
        }
    }
  axisVec*=axisGap;
  auto cmp=[](const std::pair<QPoint,float>& p1,const std::pair<QPoint,float>& p2)->bool{return p1.second<p2.second;};
  std::sort(left.begin(),left.end(),cmp);
  std::sort(right.begin(),right.end(),cmp);

  for(int i=1;i<=axisPointNum;++i){
      QPointF point(contour.rootPoint.x()+i*axisVec.x(),contour.rootPoint.y()+i*axisVec.y());

      int leftEndLarge,rightEndLarge,leftEndSmall,rightEndSmall;
      leftEndSmall=rightEndSmall=0;
      for(int j=0;j<left.size();++j){
          if(left[j].second>=axisGap*i){
              leftEndLarge=j;
              break;
            }else leftEndSmall=j;
        }
      for(int j=0;j<right.size();++j){
          if(right[j].second>=axisGap*i){
              rightEndLarge=j;
              break;
            }else rightEndSmall=j;
        }

      QPointF leftEnd,rightEnd;
      if(std::abs(axisGap*i-left[leftEndLarge].second)<0.1)leftEnd=left[leftEndLarge].first;
      else{

          QPoint large=left[leftEndLarge].first;
          QPoint small=left[leftEndSmall].first;//always crash here
          QVector2D LS(large.x()-small.x(),large.y()-small.y());
          QVector2D SP(small.x()-point.x(),small.y()-point.y());
          float constant=-(QVector2D::dotProduct(SP,axisVec));
          float factor=QVector2D::dotProduct(LS,axisVec);
          if(factor==0)leftEnd=large;
          else {
              QVector2D endVec(constant/factor*LS+SP);
              leftEnd=QPointF(endVec.x()+point.x(),endVec.y()+point.y());
            }

        }
      if(std::abs(axisGap*i-right[rightEndLarge].second)<0.001)rightEnd=right[rightEndLarge].first;
      else{

          QPoint large=right[rightEndLarge].first;
          QPoint small=right[rightEndSmall].first;
          QVector2D LS(large.x()-small.x(),large.y()-small.y());
          QVector2D SP(small.x()-point.x(),small.y()-point.y());
          float constant=-(QVector2D::dotProduct(SP,axisVec));
          float factor=QVector2D::dotProduct(LS,axisVec);
          if(factor==0)rightEnd=large;
          else {
              QVector2D endVec(constant/factor*LS+SP);
              rightEnd=QPointF(endVec.x()+point.x(),endVec.y()+point.y());
            }
        }


      //add points
      float tV=i*axisGap/axisLength;
      tessellation.stroke.push_back(QPoint(point.x(),point.y()));//center
      tessellation.tVtU[QPoint(point.x(),point.y())]=std::make_pair(tV,0);

      auto getPortionNum=[](float length,int refNum)->int{
          float minGap=2;
          float gap=length/refNum;
          if(minGap>gap){
              int candiNum= length/minGap;
              float off=length-minGap*candiNum;
              if(off/minGap>0.5)return candiNum+1;
              else return candiNum;
            }else{
              return refNum;
            }
        };

      //left
      float leftLength;
      leftLength=euclidDistance(point,leftEnd);
      int leftNum=getPortionNum(leftLength,perpendAxisPointNum);
      float perpendAxisGap=leftLength/leftNum;
      QPointF leftVec=(leftEnd-point)/leftLength*perpendAxisGap;
      for(int i=1;i<leftNum;++i){
          tessellation.stroke.push_back(QPoint(leftVec.x()*i+point.x(),leftVec.y()*i+point.y()));
          tessellation.tVtU[QPoint(leftVec.x()*i+point.x(),leftVec.y()*i+point.y())]=std::make_pair(tV,i*perpendAxisGap/leftLength);
        }
      //right
      float rightLength;
      rightLength=euclidDistance(point,rightEnd);
      int rightNum=getPortionNum(rightLength,perpendAxisPointNum);
      perpendAxisGap=rightLength/rightNum;
      QPointF rightVec=(rightEnd-point)/rightLength*perpendAxisGap;
      for(int i=1;i<rightNum;++i){
          tessellation.stroke.push_back(QPoint(rightVec.x()*i+point.x(),rightVec.y()*i+point.y()));
          tessellation.tVtU[QPoint(rightVec.x()*i+point.x(),rightVec.y()*i+point.y())]=std::make_pair(tV,i*perpendAxisGap/rightLength);
        }

    }
  orderPreserveUnique(tessellation.stroke);
  return tessellation;
}

PetalStroke meshTessellationAsGap(const PetalStroke &contour,float axisGap,float perpendAxisGap){

  PetalStroke tessellation=contour;
  if(contour.stroke.empty())return tessellation;
  std::vector<std::pair<QPoint,float>> left;
  std::vector<std::pair<QPoint,float>> right;
  QVector2D axisVec(contour.farPoint-contour.rootPoint);
  float axisLength;
  int axisNum=(axisLength=std::sqrt(QVector2D::dotProduct(axisVec,axisVec)))/axisGap;
  axisVec.normalize();
  for(auto& p:contour.stroke){
      float cross=QVector2DCross(axisVec,QVector2D(p-contour.rootPoint));
      float dot=QVector2D::dotProduct(axisVec,QVector2D(p-contour.rootPoint));
      const float zero=0.0001;
      if(cross>zero){//left
          left.push_back(std::make_pair(p,dot));
        }else if(cross<zero){//right
          right.push_back(std::make_pair(p,dot));
        }
    }
  axisVec*=axisGap;
  auto cmp=[](const std::pair<QPoint,float>& p1,const std::pair<QPoint,float>& p2)->bool{return p1.second<p2.second;};
  std::sort(left.begin(),left.end(),cmp);
  std::sort(right.begin(),right.end(),cmp);

  for(int i=1;i<=axisNum;++i){
      QPointF point(contour.rootPoint.x()+i*axisVec.x(),contour.rootPoint.y()+i*axisVec.y());

      int leftEndLarge,rightEndLarge,leftEndSmall,rightEndSmall;
      leftEndSmall=rightEndSmall=0;
      for(int j=0;j<left.size();++j){
          if(left[j].second>=axisGap*i){
              leftEndLarge=j;
              break;
            }else leftEndSmall=j;
        }
      for(int j=0;j<right.size();++j){
          if(right[j].second>=axisGap*i){
              rightEndLarge=j;
              break;
            }else rightEndSmall=j;
        }

      QPointF leftEnd,rightEnd;
      if(std::abs(axisGap*i-left[leftEndLarge].second)<0.1)leftEnd=left[leftEndLarge].first;
      else{

          QPoint large=left[leftEndLarge].first;
          QPoint small=left[leftEndSmall].first;//always crash here
          QVector2D LS(large.x()-small.x(),large.y()-small.y());
          QVector2D SP(small.x()-point.x(),small.y()-point.y());
          float constant=-(QVector2D::dotProduct(SP,axisVec));
          float factor=QVector2D::dotProduct(LS,axisVec);
          if(factor==0)leftEnd=large;
          else {
              QVector2D endVec(constant/factor*LS+SP);
              leftEnd=QPointF(endVec.x()+point.x(),endVec.y()+point.y());
            }

        }
      if(std::abs(axisGap*i-right[rightEndLarge].second)<0.001)rightEnd=right[rightEndLarge].first;
      else{

          QPoint large=right[rightEndLarge].first;
          QPoint small=right[rightEndSmall].first;
          QVector2D LS(large.x()-small.x(),large.y()-small.y());
          QVector2D SP(small.x()-point.x(),small.y()-point.y());
          float constant=-(QVector2D::dotProduct(SP,axisVec));
          float factor=QVector2D::dotProduct(LS,axisVec);
          if(factor==0)rightEnd=large;
          else {
              QVector2D endVec(constant/factor*LS+SP);
              rightEnd=QPointF(endVec.x()+point.x(),endVec.y()+point.y());
            }
        }


      //add points
      float tV=i*axisGap/axisLength;
      tessellation.stroke.push_back(QPoint(point.x(),point.y()));//center
      tessellation.tVtU[QPoint(point.x(),point.y())]=std::make_pair(tV,0);
      //left
      float leftLength;
      int leftNum=(leftLength=euclidDistance(point,leftEnd))/perpendAxisGap;
      QPointF leftVec=(leftEnd-point)/leftLength*perpendAxisGap;
      for(int i=1;i<leftNum;++i){
          tessellation.stroke.push_back(QPoint(leftVec.x()*i+point.x(),leftVec.y()*i+point.y()));
          tessellation.tVtU[QPoint(leftVec.x()*i+point.x(),leftVec.y()*i+point.y())]=std::make_pair(tV,i*perpendAxisGap/leftLength);
        }
      //right
      float rightLength;
      int rightNum=(rightLength=euclidDistance(point,rightEnd))/perpendAxisGap;
      QPointF rightVec=(rightEnd-point)/rightLength*perpendAxisGap;
      for(int i=1;i<rightNum;++i){
          tessellation.stroke.push_back(QPoint(rightVec.x()*i+point.x(),rightVec.y()*i+point.y()));
          tessellation.tVtU[QPoint(rightVec.x()*i+point.x(),rightVec.y()*i+point.y())]=std::make_pair(tV,i*perpendAxisGap/rightLength);
        }

    }
  orderPreserveUnique(tessellation.stroke);
  return tessellation;
}

PetalStroke meshTessellation2D(const PetalStroke &contour, float axisGap, float perpendAxisGap){
  QVector<QPointF> contourStroke;
  for(auto& p:contour.stroke)contourStroke.push_back(p);
  QPolygonF poly(contourStroke);

  PetalStroke tessellation=contour;
  QVector2D vec(contour.farPoint-contour.rootPoint);
  vec.normalize();
  QPointF axis(vec.x(),vec.y());
  QPointF perpendAxis(axis.y(),-axis.x());
  axis*=axisGap;
  perpendAxis*=perpendAxisGap;
  QPointF startPoint(contour.rootPoint.x()+axis.x(),contour.rootPoint.y()+axis.y());
  const float axisManhattan=(contour.farPoint-contour.rootPoint).manhattanLength();

  while((startPoint-contour.rootPoint).manhattanLength()<axisManhattan){
      tessellation.stroke.push_back(QPoint(startPoint.x(),startPoint.y()));
      QPointF newPoint=startPoint+perpendAxis;
      while(poly.containsPoint(newPoint,Qt::OddEvenFill)){
          qDebug()<<"add a new point(up): "<<newPoint;
          tessellation.stroke.push_back(QPoint(newPoint.x(),newPoint.y()));
          newPoint+=perpendAxis;
        }
      newPoint=startPoint-perpendAxis;
      while(poly.containsPoint(newPoint,Qt::OddEvenFill)){
          qDebug()<<"add a new point(down): "<<newPoint;
          tessellation.stroke.push_back(QPoint(newPoint.x(),newPoint.y()));
          newPoint-=perpendAxis;
        }
      startPoint+=axis;
    }
  Stroke& stroke=tessellation.stroke;
  sortPointsAsXAscend(stroke);
  stroke.erase(std::unique(stroke.begin(),stroke.end()),stroke.end());
  return tessellation;
}
//NOTE this function also return anchorPoint , anchorIndex , branchStartRadius, offset
bool preciseSkeletonIntersectJudge(const Skeleton& main,Skeleton& offshoot,float attachThreshold,int mainRootIndex,QPoint& anchorPoint,int& index,float& radius,QPoint& offset){
  QPoint& p1=offshoot.back();
  QPoint& p2=offshoot.front();
  int pIndex=0;
  for(auto& p:main){
      radius=branchAttenuation(std::abs(pIndex-mainRootIndex)/(float)main.size());
      if(euclidDistance(p1,p)<=attachThreshold){
          anchorPoint=p;
          offset=p-p1;
          for(auto& o:offshoot)o+=offset;
          index=offshoot.size()-1;
          return true;
        }else if(euclidDistance(p2,p)<=attachThreshold){
          anchorPoint=p;
          offset=p-p2;
          for(auto& o:offshoot)o+=offset;
          index=0;
          return true;
        }
      ++pIndex;
    }
  return false;
}
inline int maxY(const Skeleton& s){
  return s.back().y();
}
bool isBelongToNode(SkeletonNodeInfo* node,Skeleton& s,float attachThreshold){
  if(node==nullptr)return false;
  QPoint anchor;int index;float radius;QPoint offset;
  if(preciseSkeletonIntersectJudge(node->skeleton,s,attachThreshold,node->rootIndex,anchor,index,radius,offset)){
//      qDebug()<<"father radius: "<<node->r<<"  my radius: "<<radius*node->r;
      node->offshoots.push_back(new SkeletonNodeInfo(s,radius*node->r,offset));
      node->offshoots.back()->rootIndex=index;
      node->offshoots.back()->endIndex=s.size()-1-index;
      node->offshoots.back()->anchorPoint=anchor;
      node->anchorPoints.push_back(anchor);
      node->offshootRootIndex.push_back(index);
      return true;
    }
  else{
      for(auto& offshoot:node->offshoots){
          if(isBelongToNode(offshoot,s,attachThreshold))return true;
        }
    }
  return false;
}

std::vector<SkeletonNodeInfo*> branchAnalyse(std::vector<Skeleton>& branches,float attachThreshold){
  //ASSERT: first point of each element of branches is the top point
  //         /  -> index 0,top point which has min y value
  //        /
  //       /
  //       \  ->index size-1, bottom point which has max y value
  //NOTE! VERY IMPORTANT! sort branches before calling this function(branchAnalyse)

  std::sort(branches.begin(),branches.end(),[](const Skeleton& s1,const Skeleton& s2)->bool{
        return maxY(s1)>maxY(s2);
      });

  std::vector<SkeletonNodeInfo*> rootBranches;
  for(int i=0;i<branches.size();++i){
      bool isRootBranch=true;
      for(int j=0;j<rootBranches.size();++j){
          if(!(isRootBranch=!isBelongToNode(rootBranches[j],branches[i],attachThreshold)))break;
        }
      if(isRootBranch){
          rootBranches.push_back(new SkeletonNodeInfo(branches[i]));
          rootBranches.back()->endIndex=0;
          rootBranches.back()->rootIndex=branches[i].size()-1;
          rootBranches.back()->anchorPoint=branches[i].back();
        }
    }
  return rootBranches;
}

bool floretBranchMatch(Floret3D &floret, SkeletonNodeInfo *node){
  //the flowerCenter.z is 0 now
  float attachDis=0.05*5;
  if(euclidDistance(QPointF(floret.floretCenter.x,floret.floretCenter.y),QPointF(node->skeleton3D.front().x,node->skeleton3D.front().y))<attachDis){
      const float Z=node->skeleton3D.front().z;
      const glm::vec3 offset=node->skeleton3D.front()-floret.floretCenter;
      for(auto& petal:floret.petals3D){
          for(auto& point:petal.stroke){
              point+=offset;//.z+=Z;
            }
          petal.worldOffset+=offset;//.z+=Z;
        }
      floret.floretCenter+=offset;//.z+=Z;
      return true;
    }else if(euclidDistance(QPointF(floret.floretCenter.x,floret.floretCenter.y),QPointF(node->skeleton3D.back().x,node->skeleton3D.back().y))<attachDis){
      const float Z=node->skeleton3D.back().z;
      const glm::vec3 offset=node->skeleton3D.back()-floret.floretCenter;
      for(auto& petal:floret.petals3D){
          for(auto& point:petal.stroke){
              point+=offset;//.z+=Z;
            }
          petal.worldOffset+=offset;//.z+=Z;
        }
      floret.floretCenter+=offset;//.z+=Z;
      return true;
    }else{
      for(auto& n:node->offshoots){
          if(floretBranchMatch(floret,n))return true;
        }
    }
  return false;
}
bool leafBranchMatch(PetalStroke3D &leaf, SkeletonNodeInfo *node){
  //all z of glm::vec3  in leaf are zero now
  float attachDis=0.05*5;
  const float PI=3.14;
  for(auto& p:node->skeleton3D){
      if(euclidDistance(QPointF(leaf.rootPoint.x+node->offset.x,leaf.rootPoint.y+node->offset.y),QPointF(p.x,p.y))<attachDis){
          glm::vec3 offset(p.x-leaf.rootPoint.x ,
                           p.y-leaf.rootPoint.y,
                           p.z);
          for(auto& lp:leaf.stroke){
              lp+=offset;
            }
          leaf.farPoint+=offset;
          leaf.rootPoint+=offset;
          leaf.worldOffset+=offset;
          glm::mat4 trans(1.0f);
          //-----------------place leaf on the sides of branch-----------------
          //align leaf to branch in xz plane
          glm::vec3 branchDir3D=node->skeleton3D.at(node->endIndex)-node->skeleton3D.at(node->rootIndex);
          glm::vec2 branchDirXZ(branchDir3D.x,branchDir3D.z);
          branchDirXZ=glm::normalize(branchDirXZ);
          glm::vec2 leafDirXZ(leaf.farPoint.x-leaf.rootPoint.x,leaf.farPoint.z-leaf.rootPoint.z);
          leafDirXZ=glm::normalize(leafDirXZ);

          float rotateAngleAlignToBranch;
          rotateAngleAlignToBranch=std::acos(glm::dot(leafDirXZ,branchDirXZ));
          if(rotateAngleAlignToBranch>PI*2/3)rotateAngleAlignToBranch=std::abs(rotateAngleAlignToBranch-PI*2/3);
          float rotateSign=branchDirXZ.x*leafDirXZ.y-branchDirXZ.y*leafDirXZ.x;

          if(rotateSign<0){
              rotateAngleAlignToBranch=-rotateAngleAlignToBranch;
            }
//          qDebug()<<"rotate angle xz: "<<rotateAngleAlignToBranch<<" degree: "<<180*rotateAngleAlignToBranch/3.14;

          //calculate 2D relative angle
          glm::vec2 leafDir=glm::vec2(glm::normalize(leaf.farPoint-p));
          glm::vec2 branchDir;
          branchDir3D=node->skeleton3D.at(node->endIndex)-p;

          if(glm::length(branchDir3D)<0.01)branchDir=glm::vec2(0);//leaf is located on the end point of branch
          else branchDir=glm::normalize(glm::vec2(branchDir3D));

          float orient=branchDir.x*leafDir.y-branchDir.y*leafDir.x;
          if(orient>0)orient=1;else if(orient<0)orient=-1;
          float rotateAttenuation;
          if(branchDir.x==0)rotateAttenuation=0;
          else{
              rotateAttenuation=std::pow(PI,-std::abs(branchDir.y/branchDir.x));
            }
          float rotateAngleLeafToBranch=std::acos(glm::dot(branchDir,leafDir))*orient*rotateAttenuation;
          if(std::isnan(rotateAngleLeafToBranch)){
              qDebug()<<" rotateAngleLeafToBranch is nan!!!!";
              qDebug()<<" orient= "<<orient;
              qDebug()<<" rotateAttenuation="<<rotateAttenuation;
              qDebug()<<" branchDir3D = "<<branchDir3D.x<<" "<<branchDir3D.y<<" "<<branchDir3D.z;
              qDebug()<<" branchDir = "<<branchDir.x<<" "<<branchDir.y;
              qDebug()<<" leafDir = "<<leafDir.x<<" "<<leafDir.y;
              rotateAngleLeafToBranch=PI/6;
            }
          std::random_device seed;
          std::default_random_engine randomEngine(seed());
          std::uniform_real_distribution<> dist(-0.3, 0.3);
          float randomFloat=1+dist(randomEngine);
          rotateAngleLeafToBranch*=randomFloat;
          trans=glm::rotate(trans,rotateAngleAlignToBranch+rotateAngleLeafToBranch,glm::vec3(0,1,0));
          //BUG rotateAngleLeafToBranch may be nan!!!!!!!!!!!
//          qDebug()<<"total angle xz: "<<(rotateAngleAlignToBranch+rotateAngleLeafToBranch)<<" degree: "<<180*(rotateAngleAlignToBranch+rotateAngleLeafToBranch)/3.14;
          //-------------------align with xz plane-----------------
          glm::vec2 leafDirXY(leaf.farPoint.x-leaf.rootPoint.x,leaf.farPoint.y-leaf.rootPoint.y);
          leafDirXY=glm::normalize(leafDirXY);
          float rotateAngleXY;
          std::uniform_real_distribution<> dist2(-0.3, 0);
          randomFloat=0.8+dist2(randomEngine);
          if(leafDirXY.y>0){
              rotateAngleXY=std::acos(leafDirXY.x);
              if(rotateAngleXY>1.5708){
                  rotateAngleXY=PI-rotateAngleXY;
                  trans=glm::rotate(trans,rotateAngleXY*randomFloat,glm::vec3(0,0,1));
                }
              else
                trans=glm::rotate(trans,-rotateAngleXY*randomFloat,glm::vec3(0,0,1));
            }else{
              rotateAngleXY=std::acos(leafDirXY.x);
              if(rotateAngleXY>1.5708){
                  rotateAngleXY=PI-rotateAngleXY;
                  trans=glm::rotate(trans,-rotateAngleXY*randomFloat,glm::vec3(0,0,1));
                }
              else
                trans=glm::rotate(trans,rotateAngleXY*randomFloat,glm::vec3(0,0,1));
            }
//          qDebug()<<"rotateAngleXY: "<<rotateAngleXY;
          //-------------------self rotate-----------------
          if(leaf.farPoint.x<leaf.rootPoint.x)
            trans=glm::rotate(trans,glm::radians(90.0f),leaf.farPoint-leaf.rootPoint);
          else if(leaf.farPoint.x>leaf.rootPoint.x)
            trans=glm::rotate(trans,glm::radians(-90.0f),leaf.farPoint-leaf.rootPoint);

          //-------------------bend leaf-----------------
          bendLeaf(leaf);

          leaf.mat=trans;
          leaf.mainOrient=glm::vec3(0,1,0);
          //-------------------add leaf to vector------------
          node->leaves.push_back(&leaf);
          return true;
        }
    }
  for(auto& n:node->offshoots){
      if(leafBranchMatch(leaf,n)){
          return true;
        }
    }
  return false;
}

PetalStroke3D constructLeaf(PetalStroke& leaf,QPointF newOrigin,float mapScale){
  leaf=meshTessellationAsDiv(leaf,10,5);//leaf=meshTessellationAsGap(leaf,4,4);
  PetalStroke3D leaf3D;
  for(auto& p:leaf.stroke)leaf3D.stroke.push_back(glm::vec3((p.x()-newOrigin.x())*mapScale,(newOrigin.y()-p.y())*mapScale,0));
  leaf3D.rootPoint=glm::vec3(glm::vec3((leaf.rootPoint.x()-newOrigin.x())*mapScale,(newOrigin.y()-leaf.rootPoint.y())*mapScale,0));
  leaf3D.farPoint=glm::vec3(glm::vec3((leaf.farPoint.x()-newOrigin.x())*mapScale,(newOrigin.y()-leaf.farPoint.y())*mapScale,0));
  leaf3D.worldOffset=leaf3D.rootPoint;
  leaf3D.mainOrient=glm::vec3(0,0,1);
  return leaf3D;
}
void bendLeaf(PetalStroke3D& leaf,float factor,glm::vec3 movingVec){
  std::random_device seed;
  std::default_random_engine randomEngine(seed());
  std::uniform_real_distribution<> dist(-0.4, 1.0);
  float randomFloat=dist(randomEngine);
  factor+=randomFloat;

  glm::vec3 axis=leaf.farPoint-leaf.rootPoint;
  glm::vec3 axisNormalized=glm::normalize(axis);
  float dis=glm::dot(axis,axisNormalized);
  for(auto& p:leaf.stroke){
      glm::vec3 dir=p-leaf.rootPoint;
      float x=glm::dot(axisNormalized,dir)/dis;
      float offset=0.1*(x*x*x*x+x*x)*dis*factor;
      glm::mat4 transMat(1.0f);
      transMat=glm::translate(transMat,-movingVec*offset);
      p=glm::vec3(transMat*glm::vec4(p,1.0));
    }
}
void orderPreserveUnique(Stroke& stroke){
  Stroke backup=stroke;
  stroke.clear();
  for(auto& p:backup){
      bool tag=true;
      for(auto& s:stroke){
          if(s==p){
              tag=false;
              break;
            }
        }
      if(tag)stroke.push_back(p);
    }
}
void copyAndPasteLeaves(const std::vector<PetalStroke>& lib,std::vector<PetalStroke>& newLeaves,const std::vector<std::pair<QPoint,QPoint>> leafInfo){
  std::random_device seed;
  std::default_random_engine engine(seed());
  if(lib.empty()){
      qDebug()<<"lib is empty";
      return;
    }
  std::uniform_int_distribution<> dis(0,lib.size()-1);
  for(int i=0;i<leafInfo.size();++i){
      QPoint endPoint=leafInfo[i].first;
      QPoint rootPoint=leafInfo[i].second;
      int num=dis(engine);//qDebug()<<"choose number "<<num;
      const PetalStroke& refPetal=lib[num];

      float scale=euclidDistance(endPoint,rootPoint)/euclidDistance(refPetal.farPoint,refPetal.rootPoint);
      QTransform transform;
      QVector2D vecTarget(endPoint-rootPoint);
      vecTarget.normalize();
      QVector2D vecSource(refPetal.farPoint-refPetal.rootPoint);
      vecSource.normalize();
      float angle=std::acos(QVector2D::dotProduct(vecTarget,vecSource))*180/3.1415926;
      if(QVector2DCross(vecSource,vecTarget)<0)angle=-angle;
      transform=QTransform().rotate(angle)*QTransform().scale(scale,scale);
      PetalStroke newLeaf;
      for(auto& p:refPetal.stroke){
          QPointF point(p.x()-refPetal.rootPoint.x(),p.y()-refPetal.rootPoint.y());
          point=transform.map(point);
          newLeaf.stroke.push_back(QPoint(point.x()+rootPoint.x(),point.y()+rootPoint.y()));
        }
      newLeaf.rootPoint=rootPoint;
      newLeaf.farPoint=endPoint;
      newLeaf.contour=newLeaf.stroke;
      newLeaves.push_back(newLeaf);
    }
}

