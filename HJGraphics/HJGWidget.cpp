#include "HJGWidget.h"
#include "leafedge.h"
using namespace HJGraphics;
using namespace std;
using namespace glm;
HJGraphics::Box* physicsBox;
HJGWidget::HJGWidget(QWidget *parent):QOpenGLWidget(parent){
  fov = 45.0f;
  firstMouse = true;
  mouseDown = false;
  mouseSensitivity = 0.2;
  lastX = static_cast<float>(width()) / 2;
  lastY = static_cast<float>(height()) / 2;
  yaw = 0.0f;
  pitch = 0.0f;
  moveSpeed=0.2;
  fps=60;
  frameMode=false;
  floretNum=0;
}
void HJGWidget::addBranch(Branch *branch){
  currentScene->addObject(*branch);
  plantObjects.push_back(branch);
  branches.push_back(branch);
  branch->tag=QString("Branch_%1").arg(branches.size()).toStdString();
  update();
}

void HJGWidget::addFloret(const Floret3D floret3D,float thickness){
  ++floretNum;
  for(int i=0;i<floret3D.petals2D.size();++i){
      Petal* petal=new Petal(floret3D.petals2D[i],floret3D.petals3D[i],true,0.02);
      petal->tag=QString("Floret_%1_petal_%2").arg(floretNum).arg(i+1).toStdString();
      petals.push_back(petal);
      currentScene->addObject(*petal);
      plantObjects.push_back(petal);
    }
  update();
}

void HJGWidget::addLeaf(const PetalStroke& points2D,const PetalStroke3D& points3D,bool isFloretPetal,float thickness){
  Petal* petal=new Petal(points2D,points3D,isFloretPetal,thickness);
  if(isFloretPetal){
      petals.push_back(petal);
    }
  else{
      leaves.push_back(petal);
      petal->tag=QString("Leaf_%1").arg(leaves.size()).toStdString();
    }

  currentScene->addObject(*petal);

  plantObjects.push_back(petal);
  update();
}

void HJGWidget::addLeafStrokes(const std::vector<PetalStroke3D> &leafstroke3ds){
  LeafEdge* leafedge=new LeafEdge(leafstroke3ds);
  currentScene->addObject(*leafedge);
  update();
}
HJGraphics::Scene* HJGWidget::getCurrentScene(){
  return currentScene;
}

void HJGWidget::initializeGL(){
  initializeOpenGLFunctions();

  //load tex source
  {
    QDir dir(GlobalTexturePath+QString("PlantCreatorTexture/Petal"));
    QStringList nameFilters;
    nameFilters << "*.jpg";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(auto f:files){
//        qDebug()<<(dir.path()+QString("/")+f);
        petalTex.push_back(HJGraphics::Texture2D((dir.path()+QString("/")+f).toStdString()));
      }
  }
  {
    QDir dir(GlobalTexturePath+QString("PlantCreatorTexture/Leaf"));
    QStringList nameFilters;
    nameFilters << "*.jpg";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(auto f:files){
//        qDebug()<<(dir.path()+QString("/")+f);
        leafTex.push_back(HJGraphics::Texture2D((dir.path()+QString("/")+f).toStdString()));
      }
  }
  {
    QDir dir(GlobalTexturePath+QString("PlantCreatorTexture/Branch"));
    QStringList nameFilters;
    nameFilters << "*.jpg";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(auto f:files){
//        qDebug()<<(dir.path()+QString("/")+f);
        branchTex.push_back(HJGraphics::Texture2D((dir.path()+QString("/")+f).toStdString()));
      }
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_MULTISAMPLE);
//  glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE );
//  glEnable(GL_CULL_FACE);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

  Coordinate* coord=new Coordinate;
  Grid* grid=new Grid(1.0f,5,GRIDMODE::XY);//GRIDMODE::YZ|
  Skybox* skybox=new Skybox(30,
                              string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/envmap_miramar/miramar_rt.tga"),
                                                          string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/envmap_miramar/miramar_lf.tga"),
                                                          string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/envmap_miramar/miramar_up.tga"),
                                                          string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/envmap_miramar/miramar_dn.tga"),
                                                          string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/envmap_miramar/miramar_bk.tga"),
                                                          string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/envmap_miramar/miramar_ft.tga"));

//                            string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/grass/grass_right.jpg"),
//                            string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/grass/grass_left.jpg"),
//                            string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/grass/grass_up.jpg"),
//                            string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/grass/grass_down.jpg"),
//                            string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/grass/grass_back.jpg"),
//                            string("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/grass/grass_front.jpg"));


  Plane* plane=new Plane(16,16,"/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Textures/chessboard.jpg",2);
  plane->model=glm::translate(plane->model,glm::vec3(0,-7,0));
  Box* box=new Box(2,2,2);
  physicsBox=box;
  box->model=translate(box->model,vec3(0.0f,-5.0f,0.0f));
  //-----------------------Direction------------------------Position-----------------------Color
  PointLight* pointLight=new PointLight(glm::vec3(0.0f,4.0f,1.0f));
  ParallelLight* paraLight=new ParallelLight(glm::vec3(0,-1,-1),glm::vec3(0,15,15));
  glm::vec3 cameraPos=glm::vec3(5.0f,5.0f,10.0f);
  glm::vec3 cameraDirection=glm::vec3(0.0f, 0.0f, 0.0f)-cameraPos;
  Camera* camera=new Camera(cameraPos,cameraDirection,4 / 3.0f, 45.0f);
  Scene* scene=new Scene(this->width()*this->devicePixelRatioF(),this->height()*this->devicePixelRatioF(),0.2f,glm::vec3(1.0f,1.0f,1.0f));
  scene->addCamera(*camera);
  //scene->addObject(*grid);
  //scene->addObject(*coord);
  scene->addObject(*plane);
  //scene->addObject(*skybox);
//  scene->addObject(*box);
  scene->addLight(*paraLight);
  addScene(*scene);
}
void HJGWidget::resizeGL(int w, int h){
  glViewport(0,0,w,h);
}
void HJGWidget::paintGL(){
  if(frameMode){
      glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE );
    }else{
      glPolygonMode(GL_FRONT_AND_BACK ,GL_FILL );
    }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  auto cc=currentScene->clearColor;
  glClearColor(cc.x, cc.y, cc.z, 1);
  if(currentScene==nullptr)return;
  currentScene->writeSharedUBOData();
  currentScene->setDefaultFramebuffer(this->defaultFramebufferObject());
  currentScene->draw();
}
void HJGWidget::mousePressEvent(QMouseEvent *event){
  this->setFocus();
  mouseDown = true;
  originalDirection=currentScene->getMainCamera()->direction;
  update();
}

void HJGWidget::mouseReleaseEvent(QMouseEvent *event){
  mouseDown = false;
  firstMouse = true;
  yaw=0;
  pitch=0;
  update();
}

void HJGWidget::mouseMoveEvent(QMouseEvent *event){
  double xpos=event->x();
  double ypos=event->y();
  if (!mouseDown)return;
  if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  glm::mat4 yawMat(1.0f);
  glm::mat4 pitchMat(1.0f);
  glm::vec3 cameraRight=glm::normalize(glm::cross(originalDirection,glm::vec3(0.0f,1.0f,0.0f)));
  glm::vec3 cameraUp=glm::normalize(glm::cross(cameraRight,originalDirection));
  yawMat=glm::rotate(yawMat,glm::radians(yaw),-cameraUp);
  pitchMat=glm::rotate(pitchMat,glm::radians(pitch),cameraRight);
  glm::vec4 newDir=yawMat*glm::vec4(originalDirection,0.0f);
  newDir=pitchMat*newDir;
  currentScene->getMainCamera()->direction=newDir;
  update();
}
void HJGWidget::wheelEvent(QWheelEvent *event){
  if (fov >= 1 && fov <= 60)fov -= event->delta()*0.01;
  if (fov <= 1)fov = 1.0f;
  else if (fov >= 60)fov = 60.0f;
  currentScene->getMainCamera()->fov=fov;
  update();
}

void HJGWidget::keyPressEvent(QKeyEvent *event){
  if(event->key()==Qt::Key_A){
      //left
      glm::vec3 cameraRight=glm::normalize(glm::cross(currentScene->getMainCamera()->direction,glm::vec3(0.0f,1.0f,0.0f)));
      currentScene->getMainCamera()->position-=cameraRight*moveSpeed;
    }
  if(event->key()==Qt::Key_D){
      //right
      glm::vec3 cameraRight=glm::normalize(glm::cross(currentScene->getMainCamera()->direction,glm::vec3(0.0f,1.0f,0.0f)));
      currentScene->getMainCamera()->position+=cameraRight*moveSpeed;
    }
  if(event->key()==Qt::Key_W){
      //front
      glm::vec3 cameraRight=glm::normalize(glm::cross(currentScene->getMainCamera()->direction,glm::vec3(0.0f,1.0f,0.0f)));
      glm::vec3 cameraFront=glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),cameraRight));
      currentScene->getMainCamera()->position+=cameraFront*moveSpeed;
    }
  if(event->key()==Qt::Key_S){
      //back
      glm::vec3 cameraRight=glm::normalize(glm::cross(currentScene->getMainCamera()->direction,glm::vec3(0.0f,1.0f,0.0f)));
      glm::vec3 cameraFront=glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),cameraRight));
      currentScene->getMainCamera()->position-=cameraFront*moveSpeed;
    }
  if(event->key()==Qt::Key_Q){
      //up
      currentScene->getMainCamera()->position+=glm::vec3(0,1,0)*moveSpeed;
    }
  if(event->key()==Qt::Key_E){
      //down
      currentScene->getMainCamera()->position+=glm::vec3(0,-1,0)*moveSpeed;
    }
  update();
}
void HJGWidget::switchScene(int index) {
  if(index<0){
      std::cout<<"WARNING @ Window::switchScene(int) : index is less than 0"<<std::endl;
      return;
    }
  if(index>=scenes.size()){
      std::cout<<"WARNING @ Window::switchScene(int) : index is more than scenes size"<<std::endl;
      return;
    }
  currentScene=scenes.at(index);
}
void HJGWidget::addScene(Scene &_scene) {
  scenes.push_back(&_scene);
  if(scenes.size() == 1)currentScene=&_scene;
}
void HJGWidget::clearCurrentScene(){
  if(plantObjects.empty())return;
  currentScene->removeObjectAfter(plantObjects.front());
  plantObjects.clear();
  petals.clear();
  leaves.clear();
  branches.clear();
  floretNum=0;
  update();
}
void HJGWidget::changeTexture(int type, int index){
  if(index<0)return;
  if(type==0){//petal
      if(index>=petalTex.size())return;
      for(auto& p:petals){
          p->material.diffuseMaps[0]=petalTex.at(index);
        }
    }else if(type==1){//leaf
      if(index>=leafTex.size())return;
      for(auto& l:leaves){
          l->material.diffuseMaps[0]=leafTex.at(index);
        }
    }else if(type==2){//branch
      if(index>=branchTex.size())return;
      for(auto& b:branches){
          b->material.diffuseMaps[0]=branchTex.at(index);
        }
    }
}
void HJGWidget::exportOBJ(){
  QString path=QFileDialog::getSaveFileName(nullptr,QString("save model at..."),QString("plant.obj"));
  if(path.isNull())return;
  //write material file
  QString mtlPath=path;
  mtlPath.replace(".obj",".mtl");

  qDebug()<<" material: "<<mtlPath<<" obj: "<<path;
  std::ofstream mtl(mtlPath.toStdString());
  for(auto& p:plantObjects){
      p->exportMaterial(mtl);
    }
  mtl.close();

  //write model file
  std::ofstream file(path.toStdString());
  int vertexStartIndex,uvStartIndex,normalStartIndex;
  vertexStartIndex=uvStartIndex=normalStartIndex=1;

  file<<"mtllib "<<QFileInfo(mtlPath).fileName().toStdString()<<"\n";
  for(auto& p:plantObjects){
      p->exportVertices(vertexStartIndex,file);
    }
  for(auto& p:plantObjects){
      p->exportUVs(uvStartIndex,file);
    }
  for(auto& p:plantObjects){
      p->exportNormals(normalStartIndex,file);
    }
  for(auto& p:plantObjects){
      p->exportFaces(file);
    }
}
