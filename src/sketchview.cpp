#include "sketchview.h"
QColor colors[10]={QColor("#FF44AA"),QColor("#FF3333"),QColor("#FFAA33"),QColor("#FFFF33"),QColor("#99FF33"),QColor("#5555FF"),QColor("#A500CC"),QColor("#7FFFD4"),QColor("#DCDCDC"),QColor("#FFB3FF")};
SketchView::SketchView(QWidget *parent) : QWidget(parent)
{

}
void SketchView::setSkeletons(const std::vector<Stroke> &sk){
  skeletons=sk;
  newLeaves.clear();newLeafInfo.clear();
  std::sort(skeletons.begin(),skeletons.end(),[](const Skeleton& s1,const Skeleton& s2)->bool{
      return s1.back().y()>s2.back().y();
    });
  branchInfo=branchAnalyse(skeletons,8);
    for(auto& b:branchInfo)b->generateSubBranch(profiles,newBranches);


//  for(auto& b:branchInfo)b->generateLeaves(profiles,newLeafInfo);
//  std::vector<PetalStroke> leafLib;
//  for(int i=0;i<leafClusters.size();++i){
//      if(leafClusters[i].size()>=5)continue;
//     for(int k=0;k<leafClusters[i].size();++k){
//         leafLib.push_back(leafClusters[i][k]);
//       }
//    }
//  copyAndPasteLeaves(leafLib,newLeaves,newLeafInfo);

}
void SketchView::setProfiles(const std::vector<Stroke> &pro){
  profiles=pro;
}

void SketchView::clear(){
  skeletons.clear();
  leaves.clear();
  leafClusters.clear();
  ellipses.clear();
  branchInfo.clear();//TODO.memory leak!
  newBranches.clear();
  newLeaves.clear();
  newLeafInfo.clear();
  profiles.clear();
  update();
}

void SketchView::setLeaves(const std::vector<PetalStroke> &ls){
  //NOTE: deprecated
  leaves=ls;
  //set ellipses
  std::vector<QPoint> farPoints;
  for(auto &l:ls)farPoints.push_back(l.farPoint);
  ellipses.clear();
  ellipses.push_back(getFittedEllipse(farPoints));
}
void SketchView::setLeafClusters(const std::vector<Floret> &cluster){
  leafClusters=cluster;
  ellipses.clear();
  for(int i=0;i<leafClusters.size();++i){
      std::vector<QPoint> farPoints;
      for(auto &l:leafClusters[i])farPoints.push_back(l.farPoint);
      if(farPoints.size()>=5)ellipses.push_back(getFittedEllipse(farPoints));
    }
}


void SketchView::paintEvent(QPaintEvent *event){
  QPainter painter(this);
  painter.fillRect(this->rect(),Qt::white);
  QPen pen;
  pen.setWidth(1);
  pen.setColor(Qt::black);

  QPen dotPen;
  dotPen.setWidth(4);
  dotPen.setColor(Qt::red);

  for(int i=0;i<branchInfo.size();++i){
      branchInfo[i]->drawSkeleton(painter,0);
    }

  //-------------------------draw leaves-------------------------
  for(int i=0;i<leafClusters.size();++i){
      pen.setColor(colors[i%10]);
      painter.setPen(pen);
      for(int j=0;j<leafClusters[i].size();++j){
         for(int k=1;k<leafClusters[i][j].stroke.size();++k){
             painter.drawLine(leafClusters[i][j].stroke[k-1],leafClusters[i][j].stroke[k]);
           }
        }
    }
  //-------------------------draw point-------------------------
  for(int i=0;i<leafClusters.size();++i){
      painter.setPen(dotPen);
      for(int j=0;j<leafClusters[i].size();++j){
         for(int k=0;k<leafClusters[i][j].stroke.size();++k){
             painter.drawPoint(leafClusters[i][j].stroke[k]);
           }
        }
    }
  for(int i=0;i<newLeaves.size();++i){
      for(int k=0;k<newLeaves[i].stroke.size();++k){
          painter.drawPoint(newLeaves[i].stroke[k]);
        }
    }
  //-------------------------draw newBranches---------------------
  QPen blackPen(Qt::black);
  painter.setPen(blackPen);
  for(int i=0;i<newBranches.size();++i){
      drawStrokeLine(newBranches[i],painter);
    }
  QPen dotPen2(Qt::black);
  dotPen2.setWidth(4);
  painter.setPen(dotPen2);
  for(int i=0;i<newBranches.size();++i){
      for(int j=0;j<newBranches[i].size();++j)
        painter.drawPoint(newBranches[i][j]);
    }
  //-------------------------draw newLeaf-------------------------
//  QPen blackPen(Qt::black);
//  painter.setPen(blackPen);
//  for(auto& p:newLeafInfo){
//      painter.drawLine(p.first,p.second);
//    }
  //-------------------------draw profiles-------------------------
  QPen proPen(Qt::cyan);
  pen.setWidth(2);
  painter.setPen(proPen);
  for(auto& s:profiles){
      drawStrokeLine(s,painter);
    }
  //-------------------------draw information-------------------------

  //draw rootPoint
  dotPen.setColor(Qt::blue);
  painter.setPen(dotPen);
  for(int i=0;i<leafClusters.size();++i){
      painter.setPen(dotPen);
      QPointF flowerCenter(0,0);
      for(int j=0;j<leafClusters[i].size();++j){
         painter.drawPoint(leafClusters[i][j].rootPoint);
         flowerCenter.setX(flowerCenter.x()+leafClusters[i][j].rootPoint.x());
         flowerCenter.setY(flowerCenter.y()+leafClusters[i][j].rootPoint.y());
        }
      flowerCenter.setX(flowerCenter.x()/leafClusters[i].size());
      flowerCenter.setY(flowerCenter.y()/leafClusters[i].size());

      pen.setColor(Qt::cyan);
      painter.setPen(pen);
      for(int j=0;j<leafClusters[i].size();++j){
          painter.drawLine(leafClusters[i][j].farPoint,flowerCenter);
        }
    }

  //draw ellipse
  pen.setColor(Qt::magenta);
  painter.setPen(pen);
  for(auto &e:ellipses){
      painter.resetTransform();
      painter.translate(e.center.x,e.center.y);
      painter.rotate(e.angle);
      painter.drawEllipse(-e.size.width/2,-e.size.height/2,e.size.width,e.size.height);
    }
  painter.resetTransform();
  //draw ellipse center
  dotPen.setColor(Qt::black);
  painter.setPen(dotPen);
  for(auto &e:ellipses){
      painter.drawPoint(e.center.x,e.center.y);
    }

}
