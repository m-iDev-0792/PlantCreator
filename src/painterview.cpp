#include "painterview.h"


PainterView::PainterView(QWidget *parent) : QWidget(parent)
{
  setAutoFillBackground(true);
  setPalette(QPalette(Qt::white));
//  auto g=this->geometry();
//  this->setGeometry(g.x(),g.y(),541,461);

}
void PainterView::clear(){
  originalStrokes.clear();
  skeletons.clear();
  leaves.clear();
  profiles.clear();
  branchProfiles.clear();
  leafProfiles.clear();
  history.clear();
  update();
}
void PainterView::undo(){
  if(history.empty())return;

  if(history.back()==PETAL){
      leaves.pop_back();
      originalStrokes.pop_back();
    }
  else if(history.back()==SKELETON){
      skeletons.pop_back();
      originalStrokes.pop_back();
    }
  else if(history.back()==BRANCH_PROFILE){
      profiles.pop_back();
      branchProfiles.pop_back();
    }
  else if(history.back()==LEAF_PROFILE){
      profiles.pop_back();
      leafProfiles.pop_back();
    }
  history.pop_back();
  update();
}

void PainterView::mousePressEvent(QMouseEvent *event){
  tempStroke.push_back(event->pos());
}
void PainterView::mouseReleaseEvent(QMouseEvent *event){
  if(currentMode==BRANCH_PROFILE){
      strokeFilter(tempStroke);
      profiles.push_back(tempStroke);
      branchProfiles.push_back(tempStroke);
      history.push_back(BRANCH_PROFILE);
    }
  else if(currentMode==LEAF_PROFILE){
      strokeFilter(tempStroke);
      profiles.push_back(tempStroke);
      leafProfiles.push_back(tempStroke);
      history.push_back(LEAF_PROFILE);
    }else{

      originalStrokes.push_back(tempStroke);

      //classify stroke
      if(manhattanDistance(tempStroke[0],tempStroke[tempStroke.size()-1])>leafStartEndThreshold){
          skeletons.push_back(tempStroke);
          history.push_back(SKELETON);
        }
      else{
          leaves.emplace_back(tempStroke);
          history.push_back(PETAL);
        }
    }
  //clean tempStroke data
  tempStroke.clear();
}
void PainterView::loadSketch(std::vector<Stroke>& sketch,std::vector<int>& tHistory){
  originalStrokes.clear();
  skeletons.clear();
  history.clear();
  leaves.clear();
  profiles.clear();
  branchProfiles.clear();
  leafProfiles.clear();
  //clean tempStroke data
  tempStroke.clear();

  for(int i=0;i<sketch.size();++i){
      int h=tHistory[i];
      Stroke& temp=sketch[i];
      if(h==PETAL){
          leaves.emplace_back(temp);
          originalStrokes.push_back(temp);
        }else if(h==SKELETON){
          skeletons.push_back(temp);
          originalStrokes.push_back(temp);
        }else if(h==BRANCH_PROFILE){
          profiles.push_back(temp);
          branchProfiles.push_back(temp);
        }else if(h==LEAF_PROFILE){
          profiles.push_back(temp);
          leafProfiles.push_back(temp);
        }
    }

  history=tHistory;
  qDebug()<<" stroke num:"<<originalStrokes.size();
  update();
}

void PainterView::mouseMoveEvent(QMouseEvent *event){
  setCursor(Qt::CrossCursor);
  tempStroke.push_back(event->pos());
  this->update();
}

void PainterView::paintEvent(QPaintEvent *event){
  QPainter painter(this);
  painter.setRenderHint( QPainter::Antialiasing, true );

  QPen pen;
  pen.setWidth(2);
  painter.setPen(pen);
  if(!referenceImage.isNull())painter.drawPixmap(this->rect(),referenceImage);
  for(auto& s:originalStrokes){
      drawStrokeLine(s,painter);
    }

  QPen branchProPen(QColor(120,79,43));
  branchProPen.setWidth(2);
  painter.setPen(branchProPen);
  for(auto& s:branchProfiles){
      drawStrokeLine(s,painter);
    }

  QPen leafProPen(QColor(106,194,52));
  leafProPen.setWidth(2);
  painter.setPen(leafProPen);
  for(auto& s:leafProfiles){
      drawStrokeLine(s,painter);
    }

  if(currentMode==BRANCH_PROFILE)painter.setPen(branchProPen);
  else if(currentMode==LEAF_PROFILE)painter.setPen(leafProPen);
  else painter.setPen(pen);
  if(!tempStroke.empty())drawStrokeLine(tempStroke,painter);
}
void PainterView::keyPressEvent(QKeyEvent *event){

}
void PainterView::setReferenceImage(QPixmap pixmap){
  referenceImage=pixmap;
  referenceImage.scaled(this->width(),this->height());
  update();
}
std::vector<Stroke> PainterView::getProfiles(int threshold){
  std::vector<Stroke> result;
  for(auto &s:profiles){
      Stroke temp;
      temp.push_back(s.front());
      QPoint lastPoint=s[0];
      int threshold=20;
      for(int i=1;i<s.size();++i){
          if((manhattanDistance(lastPoint,s[i])<threshold)&&((i+1)!=s.size()))continue;//use manhattan distance
          temp.push_back(s[i]);
          lastPoint=s[i];
        }
      result.push_back(temp);
    }
  return result;
}
std::vector<Stroke> PainterView::getBranchProfiles(int threshold){
  std::vector<Stroke> result;
  for(auto &s:branchProfiles){
      Stroke temp;
      temp.push_back(s.front());
      QPoint lastPoint=s[0];
      int threshold=20;
      for(int i=1;i<s.size();++i){
          if((manhattanDistance(lastPoint,s[i])<threshold)&&((i+1)!=s.size()))continue;//use manhattan distance
          temp.push_back(s[i]);
          lastPoint=s[i];
        }
      result.push_back(temp);
    }
  return result;
}
std::vector<Stroke> PainterView::getLeafProfiles(int threshold){
  std::vector<Stroke> result;
  for(auto &s:leafProfiles){
      Stroke temp;
      temp.push_back(s.front());
      QPoint lastPoint=s[0];
      int threshold=20;
      for(int i=1;i<s.size();++i){
          if((manhattanDistance(lastPoint,s[i])<threshold)&&((i+1)!=s.size()))continue;//use manhattan distance
          temp.push_back(s[i]);
          lastPoint=s[i];
        }
      result.push_back(temp);
    }
  return result;
}
std::vector<Stroke> PainterView::getSkeletons(int threshold){
  std::vector<Stroke> result;
  for(auto &s:skeletons){
      Stroke temp;
      temp.push_back(s.front());
      QPoint lastPoint=s[0];
//      for(int i=1;i<s.size();++i){
//          if((manhattanDistance(lastPoint,s[i])<threshold)&&((i+1)!=s.size()))continue;//use manhattan distance
//          temp.push_back(s[i]);
//          lastPoint=s[i];
//        }

      for(int i=1;i<s.size();++i){
          float distance=manhattanDistance(lastPoint,s[i]);
          if(distance<threshold*0.8f)continue;
          else if(distance<threshold*1.2){
              temp.push_back(s[i]);
              lastPoint=s[i];
            }else{
              QPoint newPoint=s[i]-lastPoint;
              newPoint=QPoint(newPoint.x()*threshold/(float)distance + lastPoint.x(),newPoint.y()*threshold/(float)distance + lastPoint.y());
              temp.push_back(newPoint);
              lastPoint=newPoint;
              --i;
            }
        }

      if(temp.front().y()>temp.back().y()){//if first point is not at the top, make sure that first point is always the top point
          std::reverse(temp.begin(),temp.end());
        }
      strokeFilter(temp);
      result.push_back(temp);
    }
  return result;
}
std::vector<PetalStroke> PainterView::getLeaves(int threshold){
  std::vector<PetalStroke> result;
  for(auto &l:leaves){
      const Stroke &s=l.stroke;
      Stroke temp;
      temp.push_back(s.front());
      QPoint lastPoint=s[0];
      for(int i=1;i<s.size();++i){
          if((manhattanDistance(lastPoint,s[i])<threshold)&&((i+1)!=s.size()))continue;//use manhattan distance
          temp.push_back(s[i]);
          lastPoint=s[i];
        }
      strokeFilter(temp);
      result.emplace_back(temp);
    }
  return result;
}

QPixmap PainterView::getReducedPixmap(int threshold){
  QPixmap pixmap(this->size());
  pixmap.fill(Qt::white);
  QPainter painter;
  QPen pen;
  pen.setWidth(1);
  pen.setColor(Qt::black);

  QPen dotPen;
  dotPen.setWidth(4);
  dotPen.setColor(Qt::red);
  painter.begin(&pixmap);

  QPoint lastPoint;
  qDebug()<<"there are "<<originalStrokes.size()<<" strokes";
  for(auto &s:originalStrokes){
      qDebug()<<s.size()<<" in this stroke";
      lastPoint=s[0];
      painter.setPen(dotPen);
      painter.drawPoint(lastPoint);
      for(int i=1;i<s.size();++i){
          if((manhattanDistance(lastPoint,s[i])<threshold)&&((i+1)!=s.size()))continue;//use manhattan distance
          painter.setPen(pen);
          painter.drawLine(lastPoint, s[i]);
          painter.setPen(dotPen);
          painter.drawPoint(s[i]);
          lastPoint=s[i];
        }
    }
  painter.end();
  return pixmap;
}
