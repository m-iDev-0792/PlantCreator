#include "commondatatype.h"
//-----------------PetalStroke-------------------------
PetalStroke::PetalStroke(const Stroke &s){
  //remove first point and last point,then add their average point
  stroke.insert(stroke.end(),s.begin()+1,s.end()-1);
  rootPoint.setX((s.front().x()+s.back().x())/2);
  rootPoint.setY((s.front().y()+s.back().y())/2);
  stroke.push_back(rootPoint);//add average point
  contour=stroke;
  //now find farPoint
  int maxDis=-1;
  int tempDis;
  for(auto &p:s){
      tempDis=euclidDistance(rootPoint,p);
      if(tempDis>=maxDis){
          maxDis=tempDis;
          farPoint=p;
        }
    }
}
void PetalStroke::generateUVCoord(){
  glm::vec2 axis(farPoint.x()-rootPoint.x(),farPoint.y()-rootPoint.y());
  glm::vec2 axisNormalized=glm::normalize(axis);
  auto cross2=[](const glm::vec2& v1,const glm::vec2& v2)->float{return v1.x*v2.y-v1.y*v2.x;};
  float axisDis=glm::dot(axis,axisNormalized);
  uvs.clear();
  float Umax,Umin,Vmax,Vmin;
  Umax=Vmax=-10086;Umin=Vmin=10086;
  for(auto& p:stroke){
      glm::vec2 vec(p.x()-rootPoint.x(),p.y()-rootPoint.y());
      float V=glm::dot(vec,axisNormalized);
      float U=std::sqrt(std::abs(glm::dot(vec,vec)-glm::dot(vec,axisNormalized)*glm::dot(vec,axisNormalized)));
      if(cross2(axisNormalized,vec)<0)U=-U;
      if(U>Umax)Umax=U;
      if(U<Umin)Umin=U;
      if(V>Vmax)Vmax=V;
      if(V<Vmin)Vmin=V;
      uvs.push_back(glm::vec2(U,V));
    }
  float Ugap=Umax-Umin;
  float Vgap=Vmax-Vmin;
  for(auto& c:uvs){
      c.x=(c.x-Umin)/Ugap;
      c.y=1-(c.y-Vmin)/Vgap;
    }
}

void PetalStroke::translate(QPoint newOrigin){
  for(auto& s:stroke){
      s.setX(s.x()-newOrigin.x());
      s.setY(newOrigin.y()-s.y());
    }
  for(auto& s:contour){
      s.setX(s.x()-newOrigin.x());
      s.setY(newOrigin.y()-s.y());
    }
  rootPoint.setX(rootPoint.x()-newOrigin.x());
  rootPoint.setY(newOrigin.y()-rootPoint.y());

  farPoint.setX(farPoint.x()-newOrigin.x());
  farPoint.setY(newOrigin.y()-farPoint.y());
}
//-----------------------PetalStroke3D-------------------
PetalStroke3D::PetalStroke3D(const Stroke3D &s){
  stroke=s;
}

//-----------------------Triangle-----------------------
Triangle::Triangle(int i1, int i2, int i3){
  pointIndices.push_back(i1);
  pointIndices.push_back(i2);
  pointIndices.push_back(i3);
}

void strokeFilter(Stroke& stroke){
  if(stroke.size()<5)return;
  Stroke origin=stroke;
  const int size=stroke.size();
  for(int i=2;i<size-2;++i){
      stroke[i].setX((origin[i-2].x()+origin[i-1].x()+origin[i].x()+origin[i+1].x()+origin[i+2].x())/5.0);
      stroke[i].setY((origin[i-2].y()+origin[i-1].y()+origin[i].y()+origin[i+1].y()+origin[i+2].y())/5.0);
    }
  stroke[1].setX((stroke[0].x()+stroke[1].x()+stroke[2].x())/3);
  stroke[1].setY((stroke[0].y()+stroke[1].y()+stroke[2].y())/3);

  stroke[size-2].setX((stroke[size-3].x()+stroke[size-2].x()+stroke[size-1].x())/3.0);
  stroke[size-2].setY((stroke[size-3].y()+stroke[size-2].y()+stroke[size-1].y())/3.0);
}
void strokeFilter(Stroke3D& stroke){
  if(stroke.size()<5)return;
  Stroke3D origin=stroke;
  const int size=stroke.size();
  for(int i=2;i<size-2;++i){
      stroke[i].x=((origin[i-2].x+origin[i-1].x+origin[i].x+origin[i+1].x+origin[i+2].x)/5.0);
      stroke[i].y=((origin[i-2].y+origin[i-1].y+origin[i].y+origin[i+1].y+origin[i+2].y)/5.0);
      stroke[i].z=((origin[i-2].z+origin[i-1].z+origin[i].z+origin[i+1].z+origin[i+2].z)/5.0);
    }
  stroke[1].x=((stroke[0].x+stroke[1].x+stroke[2].x)/3);
  stroke[1].y=((stroke[0].y+stroke[1].y+stroke[2].y)/3);
  stroke[1].z=((stroke[0].z+stroke[1].z+stroke[2].z)/3);

  stroke[size-2].x=((stroke[size-3].x+stroke[size-2].x+stroke[size-1].x)/3);
  stroke[size-2].y=((stroke[size-3].y+stroke[size-2].y+stroke[size-1].y)/3);
  stroke[size-2].z=((stroke[size-3].z+stroke[size-2].z+stroke[size-1].z)/3);

}
