#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>
//using namespace glm;
const int NORMAL_HEIGHT=510;
const int DEBUG_HEIGHT=940;
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  ui->bendFactor->setText("1.0");ui->caveFactor->setText("1.0");
  QRect rect=this->geometry();
  rect.setHeight(NORMAL_HEIGHT);
  this->setGeometry(rect);

  for(int i=0;i<10;++i)ui->petalCombo->addItem(QString::number(i));

  //load petal texture name
  {
    QDir dir(GlobalTexturePath+QString("PlantCreatorTexture/Petal"));
    QStringList nameFilters;
    nameFilters << "*.jpg";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(auto f:files)ui->petalTex->addItem(f);
    ui->petalTex->setCurrentText("default.jpg");
  }
  {
    QDir dir(GlobalTexturePath+QString("PlantCreatorTexture/Leaf"));
    QStringList nameFilters;
    nameFilters << "*.jpg";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(auto f:files)ui->leafTex->addItem(f);
  }
  {
    QDir dir(GlobalTexturePath+QString("PlantCreatorTexture/Branch"));
    QStringList nameFilters;
    nameFilters << "*.jpg";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(auto f:files)ui->branchTex->addItem(f);
  }

  this->setFocus();
  auto firstAction=ui->actionLoad_Sketch;
  QLabel* strokeTypeLabel=new QLabel("Stroke\nType");
  ui->toolBar->insertWidget(firstAction,strokeTypeLabel);
  QComboBox* strokeTypeCombo=new QComboBox;
  strokeTypeCombo->addItem("Element");
  strokeTypeCombo->addItem("BranchPro");
  strokeTypeCombo->addItem("LeafPro");
  connect(strokeTypeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(strokeTypeChanged(int)));
  ui->toolBar->insertWidget(firstAction,strokeTypeCombo);
  ui->toolBar->insertSeparator(firstAction);
}

MainWindow::~MainWindow()
{
  delete ui;
}
void MainWindow::strokeTypeChanged(int index){
  if(index==0){
      ui->widget->setMode(PainterView::SKELETON);
    }else if(index==1){
      ui->widget->setMode(PainterView::BRANCH_PROFILE);
    }else if(index==2){
      ui->widget->setMode(PainterView::LEAF_PROFILE);
    }
}

void MainWindow::on_saveBtn_clicked()
{
    QString path=QFileDialog::getSaveFileName(nullptr,QString("save image at..."),QString("image.png"));
    if(path.isNull())return;
    auto grab=ui->widget->grab();
    if(!grab.isNull()){
        grab.save(path,"png");
      }
}

void MainWindow::on_thresholdSlider_valueChanged(int value)
{
    ui->thresholdLabel->setText(QString::number(value));
}

void MainWindow::on_saveReduced_clicked()
{
  QString path=QFileDialog::getSaveFileName(nullptr,QString("save image at..."),QString("reducedImage@%1.png").arg(ui->thresholdSlider->value()));
  if(path.isNull())return;
  auto grab=ui->widget->getReducedPixmap(ui->thresholdSlider->value());
  if(!grab.isNull()){
      grab.save(path,"png");
    }
}

void MainWindow::on_showReducedBtn_clicked()
{
  ui->reducedWidget->clear();
  ui->reducedWidget->setProfiles(ui->widget->getProfiles(40));
  ui->reducedWidget->setLeafClusters(meanShiftClassify(ui->widget->getLeaves(ui->thresholdSlider->value()),10));
  ui->reducedWidget->setSkeletons(ui->widget->getSkeletons(ui->thresholdSlider->value()));
  ui->reducedWidget->update();
}


void MainWindow::on_delaunay_clicked()
{
  std::vector<Floret> clusters=meanShiftClassify(ui->widget->getLeaves(ui->thresholdSlider->value()),10);
  Floret firstCluster=clusters.front();
  int index=ui->petalCombo->currentIndex();
  PetalStroke firstPetal=firstCluster.at(index);
//  firstPetal=meshTessellationAsDiv(firstPetal,10,5);
  if(ui->printEdge->isChecked()){
      qDebug()<<"---------original points-----------";
    for(int i=0;i<firstPetal.stroke.size();++i){
        qDebug()<<firstPetal.stroke[i]<<",";
      }
  }

  ui->triangulationView->testPetal=firstPetal;
  ui->triangulationView->points=firstPetal.contour;
  ui->triangulationView->update();

  auto firstPetalStroke=firstPetal.stroke;
  qDebug()<<"delaunay points size: "<<firstPetalStroke.size();
  ui->triangulationView->points=firstPetalStroke;

  std::map<QPoint,int> indexMap;
  std::vector<std::vector<bool>> adjMatrix;
  std::vector<bool> temp;

  sortPointsAsXAscend(firstPetalStroke);
  //init indexMap and adjMatrix
  for(int i=0;i<firstPetalStroke.size();++i){
      indexMap[firstPetalStroke[i]]=i;
      temp.push_back(false);
    }
  for(int i=0;i<firstPetalStroke.size();++i)adjMatrix.push_back(temp);

  if(ui->printAllPoints->isChecked()){
    QString xStr,yStr;
    for(int i=0;i<firstPetalStroke.size();++i){
        qDebug()<<firstPetalStroke[i]<<",";
        xStr+=QString("%1,").arg(firstPetalStroke[i].x());
        yStr+=QString("%1,").arg(firstPetalStroke[i].y());
      }
    xStr=QString("px=[")+xStr+QString("];");
    yStr=QString("py=[")+yStr+QString("];");
    qDebug()<<xStr;
    qDebug()<<yStr;
  }


  TriangleNet S=delaunayTriangulation(firstPetalStroke,indexMap,adjMatrix);
  qDebug()<<"edges size: "<<S.edges.size();
  auto triangle=getTriangleFromNet(S,indexMap,adjMatrix);
  qDebug()<<"total triangles num: "<<triangle.size();

  ui->triangulationView->S=S;
  ui->triangulationView->update();

}

void MainWindow::on_petalTex_currentIndexChanged(int index)
{
  ui->openGLWidget->changeTexture(0,index);
  ui->openGLWidget->update();
}

void MainWindow::on_branchTex_currentIndexChanged(int index)
{
  ui->openGLWidget->changeTexture(2,index);
  ui->openGLWidget->update();
}

void MainWindow::on_leafTex_currentIndexChanged(int index)
{
  ui->openGLWidget->changeTexture(1,index);
  ui->openGLWidget->update();
}


void MainWindow::on_actionLoad_Sketch_triggered()
{
  QString path=QFileDialog::getOpenFileName(this,QString("Select sketch file to load..."),QString("."),"txt files(*.txt)");
  if(path.isNull())return;
  std::vector<Stroke> stroke;
  std::vector<int> history;
  std::ifstream file(path.toStdString());
  Stroke temp;
  while(!file.eof()){
      int x,y;
      file>>x;
      if(x==-10086){
          file>>x;
          history.push_back(x);
          if(!temp.empty()){
              stroke.push_back(temp);
              temp.clear();
            }
        }else{
          file>>y;
          temp.push_back(QPoint(x,y));
        }
    }
  ui->widget->loadSketch(stroke,history);
}

void MainWindow::on_actionExport_Sketch_triggered()
{
  QString path=QFileDialog::getSaveFileName(nullptr,QString("save sketch at..."),QString("sketch.txt"));
  if(path.isNull())return;
  std::ofstream file(path.toStdString());
  std::vector<Stroke> originalStroke=ui->widget->getOriginal();
  std::vector<Stroke> profileStroke=ui->widget->getProfiles();
  std::vector<int> history=ui->widget->getHistroy();
  int originalIndex=0;
  int profileIndex=0;
  for(auto& h:history){
      Stroke* stroke;
      int tag;
      if(h==PainterView::SKELETON){
          stroke=&originalStroke[originalIndex++];
          tag=PainterView::SKELETON;
        }else if(h==PainterView::PETAL){
          stroke=&originalStroke[originalIndex++];
          tag=PainterView::PETAL;
        }else if(h==PainterView::BRANCH_PROFILE){
          stroke=&profileStroke[profileIndex++];
          tag=PainterView::BRANCH_PROFILE;
        }else if(h==PainterView::LEAF_PROFILE){
          stroke=&profileStroke[profileIndex++];
          tag=PainterView::LEAF_PROFILE;
        }
      for(auto& p:*stroke){
          file<<p.x()<<" "<<p.y()<<"\n";
        }
      file<<"-10086 "<<tag<<"\n";
    }
}

void MainWindow::on_actionExport_Obj_triggered()
{
    ui->openGLWidget->exportOBJ();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->widget->undo();
}

void MainWindow::on_actionAdd_Ref_triggered()
{
  QString path=QFileDialog::getOpenFileName(this,QString("Select a reference image..."),QString("."),"JPEG Files(*.jpg);;PNG Files(*.png)");
  if(path.isNull())return;
  QPixmap pixmap(path);
  if(!pixmap.isNull())ui->widget->setReferenceImage(pixmap);
}

void MainWindow::on_actionClearAll_triggered()
{
  ui->openGLWidget->clearCurrentScene();
  ui->widget->clear();
  ui->reducedWidget->clear();
}

void MainWindow::on_actionFrame_triggered()
{
  ui->openGLWidget->frameMode=!ui->openGLWidget->frameMode;
  ui->openGLWidget->update();
}

void MainWindow::on_actionClearModel_triggered()
{
    ui->openGLWidget->clearCurrentScene();
}

void MainWindow::on_actionConstruct_triggered()
{

  auto start=std::chrono::high_resolution_clock::now();
  //----------------fetch original 2d point data----------------
  std::vector<Skeleton> skeletons=ui->widget->getSkeletons(ui->thresholdSlider->value());
  std::vector<Floret> manyFlorets=meanShiftClassify(ui->widget->getLeaves(ui->thresholdSlider->value()),10);
  std::vector<Stroke> leafProfiles=ui->widget->getLeafProfiles(40);
  std::vector<Stroke> branchProfiles=ui->widget->getBranchProfiles(40);
  std::vector<Floret3D> manyFlorets3D;
  std::vector<PetalStroke> leaves;
  //----------------add leaves and flowers----------------
  for(auto& aFloret2D:manyFlorets){
      if(aFloret2D.size()<5){
          //collect leaves
          for(auto& p:aFloret2D){
              leaves.push_back(p);
            }
          continue;
        }
      //collect floret and create floret3D
      Floret3D floret3D(aFloret2D,QPointF(ui->widget->width()/2.0,ui->widget->height()/2.0),mapScale);
      manyFlorets3D.push_back(floret3D);
    }
  //----------------add branch----------------
  auto nodeInfos=branchAnalyse(skeletons,8);//construct branch hierarchy
  std::vector<Branch*> branchModels;

  qDebug()<<"root branch num: "<<nodeInfos.size();
  std::vector<PetalStroke> leafLib=leaves;
  for(int i=0;i<nodeInfos.size();++i){
      auto currentNode=nodeInfos[i];
//      nodeInfos[i]->generateSubBranch(profiles);
      //-----------NEW LEAF----------
      std::vector<PetalStroke> newLeavesThisRound;
      std::vector<std::pair<QPoint,QPoint>> newLeafInfo;
      nodeInfos[i]->generateSubBranch(branchProfiles,ui->branchGrowthPointGap->value(),ui->branchGrowthRate->value());
      nodeInfos[i]->generateLeaves(leafProfiles,newLeafInfo,ui->leafGrowthPointGap->value(),ui->leafGrowthRate->value());
      copyAndPasteLeaves(leafLib,newLeavesThisRound,newLeafInfo);
      leaves.insert(leaves.end(),newLeavesThisRound.begin(),newLeavesThisRound.end());


      nodeInfos[i]->constructSkeleton3D(glm::vec3(0),glm::vec3(0),0,0,true);
      nodeInfos[i]->translate(ui->widget->width()/2.0,ui->widget->height()/2.0,mapScale);
      nodeInfos[i]->updateRadius();
      nodeInfos[i]->generateBranchModel(branchModels,ui->baseRadius->value());
    }


  //----------------create branch model----------------
  for(auto& bm:branchModels){
      ui->openGLWidget->addBranch(bm);
    }


  //----------------match floret to corresponding branch and create floret model----------------
  for(auto& floret3D:manyFlorets3D){
//      floret3D.bendPetal();
      floret3D.bendAndCavePetal(ui->bendFactor->text().toFloat(),ui->caveFactor->text().toFloat());
      if(ui->rotateToXZ->isChecked())floret3D.adjustOrient();
      //floret&branch match
      for(auto& n:nodeInfos){
          if(floretBranchMatch(floret3D,n))break;
        }
      //now create petal to 3D world
      ui->openGLWidget->addFloret(floret3D,0.02);
    }



  //----------------match leaf to corresponding branch----------------
  for(auto& leaf:leaves){
      PetalStroke3D leaf3D=constructLeaf(leaf,QPointF(ui->widget->width()/2.0,ui->widget->height()/2.0),mapScale);
      for(auto& n:nodeInfos){
          if(leafBranchMatch(leaf3D,n)){
              break;
            }
        }
      ui->openGLWidget->addLeaf(leaf,leaf3D,false,0.02);
    }
  ui->openGLWidget->update();
  auto end=std::chrono::high_resolution_clock::now();
  auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  qDebug()<<"time used: "<<duration.count();
}

void MainWindow::on_showDebug_toggled(bool checked)
{
  QRect rect=this->geometry();
  if(checked){
      rect.setHeight(DEBUG_HEIGHT);
    }
  else {
      rect.setHeight(NORMAL_HEIGHT);
    }
  this->setGeometry(rect);
}

void MainWindow::on_actiontest_triggered()
{
  qDebug()<<"hello";
  QPixmap image(ui->widget->size());
  std::vector<Skeleton> original=ui->widget->getSkeletons(ui->thresholdSlider->value());
  QPainter painter(&image);
  painter.fillRect(this->rect(),Qt::white);
  QPen pen;
  pen.setWidth(1);
  pen.setColor(Qt::black);

  QPen dotPen;
  dotPen.setWidth(4);
  dotPen.setColor(Qt::red);
  //------------------------draw line--------------------------
  painter.setPen(pen);
  for(auto& s:original){
      drawStrokeLine(s,painter);
    }
  //-------------------------draw point-------------------------
  for(int i=0;i<original.size();++i){
      painter.setPen(dotPen);
      for(int j=0;j<original[i].size();++j){
             painter.drawPoint(original[i][j]);
        }
    }

  image.save("/Users/hezhenbang/Desktop/save.png","PNG");
}
