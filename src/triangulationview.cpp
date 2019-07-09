#include "triangulationview.h"

TriangulationView::TriangulationView(QWidget *parent) : QWidget(parent)
{

}
QPoint scalePoint(QPoint p,QPoint base,float scale){
  return QPoint((p.x()-base.x())*scale+20,(p.y()-base.y())*(scale)+50);
//  return QPoint(base.x()*(1-scale)+scale*p.x(),base.y()*(1-scale)+scale*p.y());
}

QPoint scalePoint(QPointF p,QPoint base,float scale){
  return QPoint((p.x()-base.x())*scale+20,(p.y()-base.y())*(scale)+50);
}
PetalStroke newMeshTessllationTest(const PetalStroke &contour,int axisNum,int perpendAxisNum,QPainter& painter,QPoint basePoint,float scale){

  PetalStroke tessellation=contour;
  if(contour.stroke.empty())return tessellation;
  std::vector<std::pair<QPoint,float>> left;
  std::vector<std::pair<QPoint,float>> right;
  QVector2D axisVec(contour.farPoint-contour.rootPoint);
  float axisGap=std::sqrt(QVector2D::dotProduct(axisVec,axisVec))/(axisNum+1);
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
  QPen bluePen(Qt::blue);
  QPen greenPen(Qt::green);
  QPen redPen(Qt::red);
  QPen leftPen(Qt::red);
  leftPen.setWidth(4);
  QPen rightPen(Qt::cyan);
  rightPen.setWidth(4);
  for(int i=1;i<=axisNum;++i){
      QPointF point(contour.rootPoint.x()+i*axisVec.x(),contour.rootPoint.y()+i*axisVec.y());
      int leftEndLarge,rightEndLarge,leftEndSmall,rightEndSmall;
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

      QPoint leftEnd,rightEnd;
//      if(std::abs(axisGap*i-left[leftEndLarge].second)>std::abs(axisGap*i-left[leftEndSmall].second))leftEnd=left[leftEndSmall].first;
//      else leftEnd=left[leftEndLarge].first;
//      if(std::abs(axisGap*i-right[rightEndLarge].second)>std::abs(axisGap*i-right[rightEndSmall].second))rightEnd=right[rightEndSmall].first;
//      else rightEnd=right[rightEndLarge].first;

      if(std::abs(axisGap*i-left[leftEndLarge].second)<0.001)leftEnd=left[leftEndLarge].first;
      else{

          QPoint large=left[leftEndLarge].first;
          QPoint small=left[leftEndSmall].first;
          QVector2D LS(large.x()-small.x(),large.y()-small.y());
          QVector2D SP(small.x()-point.x(),small.y()-point.y());
          float constant=-(QVector2D::dotProduct(SP,axisVec));
          float factor=QVector2D::dotProduct(LS,axisVec);
          if(factor==0)leftEnd=large;
          else {
              QVector2D endVec(constant/factor*LS+SP);
              leftEnd=QPoint(endVec.x()+point.x(),endVec.y()+point.y());
              qDebug()<<"left update: "<<leftEnd;
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
              rightEnd=QPoint(endVec.x()+point.x(),endVec.y()+point.y());
              qDebug()<<"right update: "<<rightEnd;
            }
        }


      painter.setPen(leftPen);
      for(auto& ppp:left){
          painter.drawPoint(scalePoint(ppp.first,basePoint,scale));
        }
      painter.setPen(rightPen);
      for(auto& ppp:right){
          painter.drawPoint(scalePoint(ppp.first,basePoint,scale));
        }



      painter.setPen(bluePen);
      painter.drawLine(scalePoint(point,basePoint,scale),scalePoint(left[leftEndLarge].first,basePoint,scale));
      painter.drawLine(scalePoint(point,basePoint,scale),scalePoint(right[rightEndLarge].first,basePoint,scale));

      painter.setPen(greenPen);
      painter.drawLine(scalePoint(point,basePoint,scale),scalePoint(left[leftEndSmall].first,basePoint,scale));
      painter.drawLine(scalePoint(point,basePoint,scale),scalePoint(right[rightEndSmall].first,basePoint,scale));

      painter.setPen(redPen);
      painter.drawLine(scalePoint(point,basePoint,scale),scalePoint(leftEnd,basePoint,scale));
      painter.drawLine(scalePoint(point,basePoint,scale),scalePoint(rightEnd,basePoint,scale));
    }

  return tessellation;
}

void TriangulationView::paintEvent(QPaintEvent *event){
  int x,y;x=y=100000;
  for(auto& ppp:points){
      if(ppp.x()<x)x=ppp.x();
      if(ppp.y()<y)y=ppp.y();
    }
  QPoint basePoint(x,y);
  QPainter p(this);
  QPen dotPen(Qt::red);
  dotPen.setWidth(4);
  QPen linePen(Qt::red);
  p.setPen(dotPen);
  float scale=4;
//  for(auto& ppp:points)p.drawPoint(scalePoint(ppp,basePoint,scale));

  p.setPen(linePen);
  newMeshTessllationTest(testPetal,5,1,p,basePoint,scale);
  return;
  for(auto& l:S.edges){
      p.drawLine(scalePoint(l.p1(),basePoint,scale),scalePoint(l.p2(),basePoint,scale));
    }
}
