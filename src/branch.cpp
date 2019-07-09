#include "branch.h"
HJGraphics::Texture2D* Branch::branchCommonTexture=nullptr;
Branch::Branch(const Stroke3D& _branchPoints,int _n,float _r,int _rootIndex)
{
  if(branchCommonTexture==nullptr){
      branchCommonTexture=new HJGraphics::Texture2D(GlobalTexturePath.toStdString()+std::string("PlantCreatorTexture/Branch/greenBranch1.jpg"));
    }
  hasShadow=true;
  //index: 0->size-1 ; position: top->bottom
  if(_branchPoints.front().y>_branchPoints.back().y)
    branchPoints=_branchPoints;
  else
    for(int i=_branchPoints.size()-1;i>=0;--i)
      branchPoints.push_back(_branchPoints[i]);

  branchPoints.erase(std::unique(branchPoints.begin(),branchPoints.end()),branchPoints.end());
  strokeFilter(branchPoints);
  n=_n;r=_r;rootIndex=_rootIndex;
  material.diffuseColor=material.ambientColor=glm::vec3(0,238/255.0,118/255.0);
  material.diffuseMaps.push_back(*branchCommonTexture);
  writeVerticesData();
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8* sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8* sizeof(GLfloat), (void*)(3* sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8* sizeof(GLfloat), (void*)(6* sizeof(GLfloat)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);

}

void Branch::writeVerticesData(){

  points.clear();
  points.resize(branchPoints.size());

  glm::vec3 up(0,1.1415925,0);
  const glm::vec3 front(0,0,1);
  float radian=glm::radians(360.0/n);

  std::vector<glm::vec3> extendedBranchPoints;
  extendedBranchPoints.push_back(up+branchPoints.front());
  for(auto &p:branchPoints)extendedBranchPoints.push_back(p);
  extendedBranchPoints.push_back(branchPoints.back()-up);
  //generate points
  for(int i=1;i<=branchPoints.size();++i){
      glm::vec3 upDir=glm::normalize(extendedBranchPoints[i-1]-extendedBranchPoints[i]);
      glm::vec3 downDir=glm::normalize(extendedBranchPoints[i+1]-extendedBranchPoints[i]);
      glm::vec3 bisector=(upDir+downDir);

      glm::vec3 axis;

      if(glm::distance(bisector,glm::vec3(0))<0.001)axis=upDir;
      else axis=glm::normalize(glm::cross(glm::cross(downDir,upDir),bisector));

      if(std::isnan(axis.x)||std::isnan(axis.y)||std::isnan(axis.z))axis=upDir;
      else if(glm::dot(upDir,axis)<0)axis=-axis;

      glm::mat4 rotateMat(1.0f);
      rotateMat=glm::rotate(rotateMat,radian,axis);
      glm::vec3 dir=front*r*branchAttenuation(std::abs(rootIndex-i)/(float)branchPoints.size());
      for(int j=0;j<n;++j){
          glm::vec3 p=extendedBranchPoints[i]+dir;
          if(std::isnan(p.x)||std::isnan(p.y)||std::isnan(p.z)){
              qDebug()<<"a point is nan at j="<<j<<"/i="<<i;
              qDebug()<<"axis: "<<axis.x<<" "<<axis.y<<" "<<axis.z;
              qDebug()<<"dir: "<<dir.x<<" "<<dir.y<<" "<<dir.z;
              qDebug()<<"distance:"<<glm::distance(dir,glm::vec3(0));
              qDebug()<<"bisector: "<<bisector.x<<" "<<bisector.y<<" "<<bisector.z;
              qDebug()<<"upDir: "<<upDir.x<<" "<<upDir.y<<" "<<upDir.z;
              qDebug()<<"downDir: "<<downDir.x<<" "<<downDir.y<<" "<<downDir.z;
              qDebug()<<"extendedBranchPoints[i-1]: "<<extendedBranchPoints[i-1].x<<" "<<extendedBranchPoints[i-1].y<<" "<<extendedBranchPoints[i-1].z;
              qDebug()<<"extendedBranchPoints[i]: "<<extendedBranchPoints[i].x<<" "<<extendedBranchPoints[i].y<<" "<<extendedBranchPoints[i].z;
              qDebug()<<"extendedBranchPoints[i+1]: "<<extendedBranchPoints[i+1].x<<" "<<extendedBranchPoints[i+1].y<<" "<<extendedBranchPoints[i+1].z<<endl;
            }
//          if(std::abs(glm::dot(up,axis))<0.001){
//              qDebug()<<"an axis is perpendicular to up(0,1,0) at j="<<j<<"/i="<<i;
//              qDebug()<<"p: "<<p.x<<" "<<p.y<<" "<<p.z;
//              qDebug()<<"axis: "<<axis.x<<" "<<axis.y<<" "<<axis.z;
//              qDebug()<<"dir: "<<dir.x<<" "<<dir.y<<" "<<dir.z;
//              qDebug()<<"distance:"<<glm::distance(dir,glm::vec3(0));
//              qDebug()<<"bisector: "<<bisector.x<<" "<<bisector.y<<" "<<bisector.z;
//              qDebug()<<"upDir: "<<upDir.x<<" "<<upDir.y<<" "<<upDir.z;
//              qDebug()<<"downDir: "<<downDir.x<<" "<<downDir.y<<" "<<downDir.z;
//              qDebug()<<"extendedBranchPoints[i-1]: "<<extendedBranchPoints[i-1].x<<" "<<extendedBranchPoints[i-1].y<<" "<<extendedBranchPoints[i-1].z;
//              qDebug()<<"extendedBranchPoints[i]: "<<extendedBranchPoints[i].x<<" "<<extendedBranchPoints[i].y<<" "<<extendedBranchPoints[i].z;
//              qDebug()<<"extendedBranchPoints[i+1]: "<<extendedBranchPoints[i+1].x<<" "<<extendedBranchPoints[i+1].y<<" "<<extendedBranchPoints[i+1].z<<endl;
//            }
          points[i-1].push_back(p);
          dir=glm::vec3(rotateMat*glm::vec4(dir,1));
        }
    }


  float *data=new float[(3+3+2)*3*n*2*(branchPoints.size()-1)];//(positionFloatNum_normalFloatNum+UVFloatNum)*pointNumPerFace*n*2*segmentNum
  float *dataBackup=data;

  //connect points as triangle face
  float unitU=1.0/n;
  uvs.clear();
  uvs.resize(2);
  for(int i=0;i<=n;++i){
      uvs[0].push_back(glm::vec2(i*unitU,1));
      uvs[1].push_back(glm::vec2(i*unitU,0));
    }
  for(int i=0;i<branchPoints.size()-1;++i){
      for(int j=0;j<n;++j){
          setFacePositionData(data,points[i][j],points[i+1][j],points[i][(j+1)%n]);
          glm::vec3 normal=getFaceNormal(points[i][j],points[i+1][j],points[i][(j+1)%n]);
          setFaceNormalData(data,normal);
          normals.push_back(normal);
          //uv1=glm::vec2(j*unitU,1);
          //uv2=glm::vec2(j*unitU,0);
          //uv3=glm::vec2((j+1)*unitU,1);
          setFaceUVData(data,uvs[0][j],uvs[1][j],uvs[0][j+1]);
          data+=FACE_STEP;

          setFacePositionData(data,points[i+1][(j+1)%n],points[i][(j+1)%n],points[i+1][j]);
          normal=getFaceNormal(points[i+1][(j+1)%n],points[i][(j+1)%n],points[i+1][j]);
          setFaceNormalData(data,normal);
          normals.push_back(normal);
          //uv1=glm::vec2((j+1)*unitU,0);
          //uv2=glm::vec2((j+1)*unitU,1);
          //uv3=glm::vec2(j*unitU,0);
          setFaceUVData(data,uvs[1][j+1],uvs[0][j+1],uvs[1][j]);
          data+=FACE_STEP;
        }
    }

  loadVBOData(dataBackup,(3+3+2)*3*n*2*(branchPoints.size()-1)*sizeof(float));

  delete [] dataBackup;
}

void Branch::writeObjectPropertyUniform(HJGraphics::Shader *shader){
  //-----------------------------------
  // Set Shader Value
  //-----------------------------------
  shader->use();
  shader->set4fm("model",model);
  shader->setInt("material.diffuseMapNum",material.diffuseMaps.size());
  shader->setInt("material.diffuseMap",0);
  shader->setInt("material.specularMapNum",material.specularMaps.size());
  shader->setInt("material.normalMapNum",material.normalMaps.size());
  shader->setInt("material.heightMapNum",material.heightMaps.size());

  shader->set3fv("material.ambientStrength",material.ambientStrength);
  shader->set3fv("material.diffuseStrength",material.diffuseStrength);
  shader->set3fv("material.specularStrength",material.specularStrength);

  shader->set3fv("material.ambientColor",material.ambientColor);
  shader->set3fv("material.diffuseColor",material.diffuseColor);
  shader->set3fv("material.specularColor",material.specularColor);

  shader->setFloat("material.shininess",material.shininess);
  shader->setFloat("material.alpha",material.alpha);
  shader->setFloat("material.reflective",material.reflective);
  shader->setFloat("material.reflective",material.refractive);

  shader->bindBlock("sharedMatrices",sharedBindPoint);
}
void Branch::draw(){
  writeObjectPropertyUniform(defaultShader);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,material.diffuseMaps[0].id);
  draw(*defaultShader);
}
void Branch::draw(HJGraphics::Shader shader){
  shader.use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES,0,6*n*(branchPoints.size()-1));
  glBindVertexArray(0);
}

void Branch::drawLight(HJGraphics::Light *light){
  HJGraphics::Shader *lightShader;
  if(light->type==HJGraphics::LightType::ParallelLightType)lightShader=parallelLightShader;
  else if(light->type==HJGraphics::LightType::SpotLightType)lightShader=spotLightShader;
  else if(light->type==HJGraphics::LightType::PointLightType)lightShader=pointLightShader;
  else return;
  writeObjectPropertyUniform(lightShader);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,material.diffuseMaps[0].id);

  light->writeLightInfoUniform(lightShader);
  draw(*lightShader);
}


void Branch::exportVertices(int& _vertexStartIndex,std::ofstream& file){
  vertexStartIndex=_vertexStartIndex;
  file<<"#"<<tag<<" vertices, No."<<vertexStartIndex<<"\n";

  for(auto& ps:points){
    for(auto& p:ps){
      file<<"v "<<p.x<<" "<<p.y<<" "<<p.z<<"\n";
      }
    }
  _vertexStartIndex+=points.size()*n;
}

void Branch::exportUVs(int& _uvStartIndex,std::ofstream& file){
  uvStartIndex=_uvStartIndex;
  file<<"#"<<tag<<" uvs, No."<<uvStartIndex<<"\n";
  for(auto& ps:uvs)
    for(auto& p:ps)
      file<<"vt "<<p.x<<" "<<p.y<<"\n";
  _uvStartIndex+=(n+1)*2;
}

void Branch::exportNormals(int& _normalStartIndex,std::ofstream& file){
  normalStartIndex=_normalStartIndex;
  file<<"#"<<tag<<" normals, No."<<normalStartIndex<<"\n";
  for(auto& p:normals)
    file<<"vn "<<p.x<<" "<<p.y<<" "<<p.z<<"\n";
  _normalStartIndex+=normals.size();
}
void Branch::exportMaterial(std::ofstream &file){
  file<<"newmtl mtl_"<<tag<<"\n";
  file<<"Ka "<<material.ambientColor.r<<" "<<material.ambientColor.g<<" "<<material.ambientColor.b<<"\n";
  file<<"Kd "<<material.diffuseColor.r<<" "<<material.diffuseColor.g<<" "<<material.diffuseColor.b<<"\n";
  file<<"Ks "<<material.specularColor.r<<" "<<material.specularColor.g<<" "<<material.specularColor.b<<"\n";
  file<<"Ns "<<material.shininess<<"\n";
  if(!material.diffuseMaps.empty()){
      file<<"map_Ka "<<material.diffuseMaps.front().path<<"\n";
      file<<"map_Kd "<<material.diffuseMaps.front().path<<"\n";
    }
}

void Branch::exportFaces(std::ofstream& file){
  auto vID=[=](int i,int j)->int{return i*n+j;};
  int normalIndex=0;
  file<<"# vertice index start at "<<vertexStartIndex<<" end at "<<(vertexStartIndex+points.size()*n-1)<<"\n";
  file<<"g "<<tag<<"\n";
  file<<"usemtl mtl_"<<tag<<"\n";
  for(int i=0;i<branchPoints.size()-1;++i){
      for(int j=0;j<n;++j){
          int v1,v2,v3,norm,uv1,uv2,uv3;
          v1=vertexStartIndex+vID(i,j);
          v2=vertexStartIndex+vID(i+1,j);
          v3=vertexStartIndex+vID(i,(j+1)%n);
          norm=normalStartIndex+(normalIndex++);
          uv1=uvStartIndex+vID(0,j);
          uv2=uvStartIndex+vID(1,j);
          uv3=uvStartIndex+vID(0,j+1);
          file<<"f "<<v1<<"/"<<uv1<<"/"<<norm<<" "<<v2<<"/"<<uv2<<"/"<<norm<<" "<<v3<<"/"<<uv3<<"/"<<norm<<"\n";

          v1=vertexStartIndex+vID(i+1,(j+1)%n);
          v2=vertexStartIndex+vID(i,(j+1)%n);
          v3=vertexStartIndex+vID(i+1,j);
          if(v1==v3||v1==v3||v1==v2)qDebug()<<"i="<<i<<" j="<<j<<" n="<<n;

          norm=normalStartIndex+(normalIndex++);
          uv1=uvStartIndex+vID(1,j+1);
          uv2=uvStartIndex+vID(0,j+1);
          uv3=uvStartIndex+vID(1,j);
          file<<"f "<<v1<<"/"<<uv1<<"/"<<norm<<" "<<v2<<"/"<<uv2<<"/"<<norm<<" "<<v3<<"/"<<uv3<<"/"<<norm<<"\n";

        }
    }
}
