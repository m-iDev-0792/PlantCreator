#include "petal.h"
HJGraphics::Texture2D* Petal::petalCommonTexture=nullptr;
HJGraphics::Texture2D* Petal::leafCommonTexture=nullptr;
Petal::Petal(const PetalStroke &_petal2D, const PetalStroke3D &_petal3D,bool isFloretPetal,float _thickness)
{
  hasShadow=true;

  petal2D=_petal2D;
  petal3D=_petal3D;
  thickness=_thickness;

  if(isFloretPetal){
    material.diffuseColor=material.ambientColor=glm::vec3(255/255.0, 105/255.0, 180/255.0);
    if(petalCommonTexture==nullptr)petalCommonTexture=new HJGraphics::Texture2D(GlobalTexturePath.toStdString()+std::string("PlantCreatorTexture/Petal/default.jpg"));
    material.diffuseMaps.push_back(*petalCommonTexture);
    }
  else{
    material.diffuseColor=material.ambientColor=glm::vec3(112/255.0, 158/255.0, 58/255.0);
    if(leafCommonTexture==nullptr)leafCommonTexture=new HJGraphics::Texture2D(GlobalTexturePath.toStdString()+std::string("PlantCreatorTexture/Leaf/leaf1.jpg"));
    material.diffuseMaps.push_back(*leafCommonTexture);
    }

  material.specularStrength=glm::vec3(0.15);
  material.diffuseStrength=glm::vec3(0.9);
  hasTexture=true;

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
glm::vec3 getNormalSimple(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3){
  glm::vec3 v1=glm::normalize(p2-p1);
  glm::vec3 v2=glm::normalize(p3-p1);
  glm::vec3 normal=glm::normalize(glm::cross(v1,v2));
  if(normal.z>0)return normal;else return -normal;
}
glm::vec3 getNormal(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,QPoint qp1,QPoint qp2,QPoint qp3){

  glm::vec3 v1=glm::normalize(p2-p1);
  glm::vec3 v2=glm::normalize(p3-p1);
  glm::vec2 v11(qp2.x()-qp1.x(),qp2.y()-qp1.y());
  glm::vec2 v22(qp3.x()-qp1.x(),qp3.y()-qp1.y());
  float dir=v11.x*v22.y-v11.y*v22.x;
  if(dir<0)return glm::normalize(glm::cross(v1,v2));
  else return -glm::normalize(glm::cross(v1,v2));
}
glm::vec3 getNormal(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,glm::vec3 DirBaseVec=glm::vec3(1,0,0)){
  //calculate normal
  std::vector<glm::vec3> fragPoints{p1,p2,p3};

  std::sort(fragPoints.begin(),fragPoints.end(),[](const glm::vec3& _p1,const glm::vec3& _p2)->bool{
      if(_p1.y==_p2.y)return _p1.x<_p2.x;
      else return _p1.y<_p2.y;
    });

  glm::vec3 v1=glm::normalize(fragPoints[1]-fragPoints[0]);
  glm::vec3 v2=glm::normalize(fragPoints[2]-fragPoints[0]);
  if(glm::dot(v2,DirBaseVec)>glm::dot(v1,DirBaseVec)){
      //v2 is closer to DirBaseVec
      return glm::normalize(glm::cross(v2,v1));
    }else{
      //v1 is closer to DirBaseVec
      return glm::normalize(glm::cross(v1,v2));
    }
}
void Petal::writeVerticesData(){
  Stroke &points2D=petal2D.stroke;
  Stroke3D &points3D=petal3D.stroke;
//  qDebug()<<"mainOrient: "<<petal3D.mainOrient.x<<" "<<petal3D.mainOrient.y<<" "<<petal3D.mainOrient.z;

  useLocalCoordinate=true;
  if(useLocalCoordinate){
      for(auto& p:points3D){
          p-=petal3D.worldOffset;
        }
      model=glm::translate(model,petal3D.worldOffset)*petal3D.mat;
    }
  indexMap.clear();
  adjMatrix.clear();
  std::vector<bool> temp;
  map2Dto3D.clear();
  petal2D.generateUVCoord();
  map2DtoUV.clear();
  normals.clear();
  for(int i=0;i<points2D.size();++i){
      map2Dto3D[points2D[i]]=points3D[i];
      map2DtoUV[points2D[i]]=petal2D.uvs[i];
    }

  sortPointsAsXAscend(points2D);//points2D lost its order,but we ensure that points2D keeps its order in following codes
  //init indexMap and adjMatrix
  for(int i=0;i<points2D.size();++i){
      indexMap[points2D[i]]=i;
      temp.push_back(false);
    }
  for(int i=0;i<points2D.size();++i)adjMatrix.push_back(temp);

  TriangleNet S=delaunayTriangulation(points2D,indexMap,adjMatrix);//points2D won't change its order
  triangles.clear();
  triangles=getTriangleFromNet(S,indexMap,adjMatrix);




  if(thickness>0)triangleNum=2*triangles.size()+2*petal2D.contour.size();
  else triangleNum=triangles.size();

  const glm::vec3 frontVec(0,0,1);
  float *data=new float[triangleNum*3*(3+3+2)];
  float *dataBackup=data;

  for(int i=0;i<triangles.size();++i){
      const Triangle& triangle=triangles[i];
      setFacePositionData(data,
                          map2Dto3D[points2D[triangle.pointIndices[0]]],
          map2Dto3D[points2D[triangle.pointIndices[1]]],
          map2Dto3D[points2D[triangle.pointIndices[2]]]);

      glm::vec3 normal=getNormalSimple(map2Dto3D[points2D[triangle.pointIndices[0]]],
          map2Dto3D[points2D[triangle.pointIndices[1]]],
          map2Dto3D[points2D[triangle.pointIndices[2]]]);//,points2D[triangle.pointIndices[0]],points2D[triangle.pointIndices[1]],points2D[triangle.pointIndices[2]]);

      if(isVecNan(normal)){
          qDebug()<<"a normal is nan in must-have face";
          debugVec3(map2Dto3D[points2D[triangle.pointIndices[0]]],"point 1");
          debugVec3(map2Dto3D[points2D[triangle.pointIndices[1]]],"point 2");
          debugVec3(map2Dto3D[points2D[triangle.pointIndices[2]]],"point 3");
          debugQPoint(points2D[triangle.pointIndices[0]],"qpoint 1");
          debugQPoint(points2D[triangle.pointIndices[1]],"qpoint 2");
          debugQPoint(points2D[triangle.pointIndices[2]],"qpoint 3");
          qDebug()<<" index 0-2:"<<triangle.pointIndices[0]<<" "<<triangle.pointIndices[1]<<" "<<triangle.pointIndices[2]<<endl;
        }
      //if(glm::dot(normal,petal3D.mainOrient)<0)normal=-normal;
      normals.push_back(normal);
      setFaceNormalData(data,normal);

      setFaceUVData(data,map2DtoUV[points2D[triangle.pointIndices[0]]],
          map2DtoUV[points2D[triangle.pointIndices[1]]],
          map2DtoUV[points2D[triangle.pointIndices[2]]]);

//      glm::vec3 normal=getFaceNormal(map2Dto3D[points2D[triangle.pointIndices[0]]],
//          map2Dto3D[points2D[triangle.pointIndices[1]]],
//          map2Dto3D[points2D[triangle.pointIndices[2]]]);
//      setFaceNormalData(data,glm::dot(normal,frontVec)>0?normal:-normal);
      data+=FACE_STEP;
    }
  //add another face and side face
  if(thickness>0){
      const glm::vec3 offset(0,0,-thickness);
      //---------------------another face----------------------
      for(int i=0;i<triangles.size();++i){
          const Triangle& triangle=triangles[i];
          setFacePositionData(data,map2Dto3D[points2D[triangle.pointIndices[0]]]+offset,
              map2Dto3D[points2D[triangle.pointIndices[1]]]+offset,
              map2Dto3D[points2D[triangle.pointIndices[2]]]+offset);

          glm::vec3 normal=-getNormalSimple(map2Dto3D[points2D[triangle.pointIndices[0]]]+offset,
              map2Dto3D[points2D[triangle.pointIndices[1]]]+offset,
              map2Dto3D[points2D[triangle.pointIndices[2]]]+offset);//,glm::vec3(-1,0,0));
          //if(glm::dot(normal,petal3D.mainOrient)>0)normal=-normal;
          setFaceNormalData(data,normal);
          normals.push_back(normal);

          setFaceUVData(data,map2DtoUV[points2D[triangle.pointIndices[0]]],
              map2DtoUV[points2D[triangle.pointIndices[1]]],
              map2DtoUV[points2D[triangle.pointIndices[2]]]);

          data+=FACE_STEP;
        }
      //---------------------side face----------------------
      const Stroke& contour=petal2D.contour;
      const int contourSize=contour.size();
      for(int i=0;i<contourSize;++i){
          glm::vec3 p1,p2,p3;
          p1=map2Dto3D[contour[i]];p2=map2Dto3D[contour[(i+1)%contourSize]];p3=map2Dto3D[contour[i]]+offset;
          setFacePositionData(data,p1,p2,p3);
          glm::vec3 normal=getNormal(p1,p2,p3,glm::vec3(0,0,-1));
          normals.push_back(normal);
          setFaceNormalData(data,normal);

          setFaceUVData(data,map2DtoUV[contour[i]],
              map2DtoUV[contour[(i+1)%contourSize]],
              map2DtoUV[contour[i]]);
          data+=FACE_STEP;

          p1=map2Dto3D[contour[i]]+offset;p2=map2Dto3D[contour[(i+1)%contourSize]];p3=map2Dto3D[contour[(i+1)%contourSize]]+offset;
          setFacePositionData(data,p1,p2,p3);
          normal=getNormal(p1,p2,p3,glm::vec3(0,0,-1));
          normals.push_back(normal);
          setFaceNormalData(data,normal);

          setFaceUVData(data,map2DtoUV[contour[i]],
              map2DtoUV[contour[(i+1)%contourSize]],
              map2DtoUV[contour[(i+1)%contourSize]]);
          data+=FACE_STEP;
        }
    }
  loadVBOData(dataBackup,sizeof(float)*(triangleNum*3*(3+3+2)));
  delete [] dataBackup;

  //-------------------------create data for btRigidBody-------------------

  for(auto &p:points2D){
      btVertices.push_back(map2Dto3D[p].x);
      btVertices.push_back(map2Dto3D[p].y);
      btVertices.push_back(map2Dto3D[p].z);
    }
  if(thickness>0){
      //add another layer of vertices
      for(auto &p:points2D){
          btVertices.push_back(map2Dto3D[p].x);
          btVertices.push_back(map2Dto3D[p].y);
          btVertices.push_back(map2Dto3D[p].z-thickness);
        }
    }

  for(auto &tri:triangles){
      btTriangles.push_back(tri.pointIndices[0]);
      btTriangles.push_back(tri.pointIndices[1]);
      btTriangles.push_back(tri.pointIndices[2]);
    }
  if(thickness>0){
      //another layer
      int petalFaceVerticesNum=points2D.size();
      for(auto &tri:triangles){
          btTriangles.push_back(tri.pointIndices[0]+petalFaceVerticesNum);
          btTriangles.push_back(tri.pointIndices[2]+petalFaceVerticesNum);
          btTriangles.push_back(tri.pointIndices[1]+petalFaceVerticesNum);
        }
      const Stroke& contour=petal2D.contour;
      const int contourSize=contour.size();
      for(int i=0;i<contourSize;++i){
          btTriangles.push_back(indexMap[contour[i]]);
          btTriangles.push_back(indexMap[contour[(i+1)%contourSize]]);
          btTriangles.push_back(indexMap[contour[i]]+petalFaceVerticesNum);

          btTriangles.push_back(indexMap[contour[i]]+petalFaceVerticesNum);
          btTriangles.push_back(indexMap[contour[(i+1)%contourSize]]);
          btTriangles.push_back(indexMap[contour[(i+1)%contourSize]]+petalFaceVerticesNum);
        }
    }

}
void Petal::writeObjectPropertyUniform(HJGraphics::Shader *shader){
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
void Petal::draw(){
  writeObjectPropertyUniform(defaultShader);
  if(hasTexture){
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D,material.diffuseMaps[0].id);
  }
  draw(*defaultShader);
}
void Petal::draw(HJGraphics::Shader shader){
  shader.use();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES,0,3*triangleNum);
  glBindVertexArray(0);
}

void Petal::drawLight(HJGraphics::Light *light){
  HJGraphics::Shader *lightShader;
  if(light->type==HJGraphics::LightType::ParallelLightType)lightShader=parallelLightShader;
  else if(light->type==HJGraphics::LightType::SpotLightType)lightShader=spotLightShader;
  else if(light->type==HJGraphics::LightType::PointLightType)lightShader=pointLightShader;
  else return;
  writeObjectPropertyUniform(lightShader);
  if(hasTexture){
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D,material.diffuseMaps[0].id);
  }
  light->writeLightInfoUniform(lightShader);
  draw(*lightShader);
}

void Petal::exportVertices(int& _vertexStartIndex,std::ofstream& file){
  vertexStartIndex=_vertexStartIndex;
  const Stroke& points2D=petal2D.stroke;
  const Stroke3D& points3D=petal3D.stroke;
  file<<"#"<<tag<<" vertices, No."<<vertexStartIndex<<"\n";

  for(int i=0;i<points2D.size();++i){
      glm::vec3 v=glm::vec3(model*glm::vec4(map2Dto3D[points2D[i]],1.0f));
//      if(useLocalCoordinate)v+=petal3D.worldOffset;
      file<<"v "<<v.x<<" "<<v.y<<" "<<v.z<<"\n";
    }
  _vertexStartIndex+=points2D.size();

  if(thickness>0){
      glm::vec3 offset(0,0,-thickness);
      for(int i=0;i<points2D.size();++i){
          glm::vec3 v=map2Dto3D[points2D[i]]+offset;
          v=glm::vec3(model*glm::vec4(v,1.0f));
//          if(useLocalCoordinate)v+=petal3D.worldOffset;
          file<<"v "<<v.x<<" "<<v.y<<" "<<v.z<<"\n";
        }
      _vertexStartIndex+=points2D.size();
    }
}

void Petal::exportUVs(int& _uvStartIndex,std::ofstream& file){
  uvStartIndex=_uvStartIndex;
  const Stroke& points2D=petal2D.stroke;
  const Stroke3D& points3D=petal3D.stroke;
  file<<"#"<<tag<<" uvs, No."<<uvStartIndex<<"\n";
  for(int i=0;i<points2D.size();++i){
      glm::vec2 v=map2DtoUV[points2D[i]];
      file<<"vt "<<v.x<<" "<<(1-v.y)<<"\n";
    }
  _uvStartIndex+=points2D.size();
}

void Petal::exportNormals(int& _normalStartIndex,std::ofstream& file){
  normalStartIndex=_normalStartIndex;
  file<<"#"<<tag<<" normals, No."<<normalStartIndex<<"\n";
  for(int i=0;i<normals.size();++i){
      glm::vec3 norm=normals[i];
      norm=glm::vec3(model*glm::vec4(norm,0.0f));
      file<<"vn "<<norm.x<<" "<<norm.y<<" "<<norm.z<<"\n";
    }
  _normalStartIndex+=normals.size();
}
void Petal::exportMaterial(std::ofstream &file){
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

void Petal::exportFaces(std::ofstream& file){
  const Stroke& points2D=petal2D.stroke;
  int normalIndex=0;
  file<<"g "<<tag<<"\n";
  file<<"usemtl mtl_"<<tag<<"\n";
  for(int i=0;i<triangles.size();++i){
      const Triangle& triangle=triangles[i];
      int v1,v2,v3,n,uv1,uv2,uv3;
      v1=vertexStartIndex+triangle.pointIndices[0];
      v2=vertexStartIndex+triangle.pointIndices[1];
      v3=vertexStartIndex+triangle.pointIndices[2];
      n=normalStartIndex+(normalIndex++);
      uv1=uvStartIndex+triangle.pointIndices[0];
      uv2=uvStartIndex+triangle.pointIndices[1];
      uv3=uvStartIndex+triangle.pointIndices[2];

      file<<"f "<<v1<<"/"<<uv1<<"/"<<n<<" "<<v2<<"/"<<uv2<<"/"<<n<<" "<<v3<<"/"<<uv3<<"/"<<n<<"\n";
    }
  if(thickness>0){
      for(int i=0;i<triangles.size();++i){
          const Triangle& triangle=triangles[i];
          int v1,v2,v3,n,uv1,uv2,uv3;
          v1=vertexStartIndex+triangle.pointIndices[0]+points2D.size();
          v2=vertexStartIndex+triangle.pointIndices[1]+points2D.size();
          v3=vertexStartIndex+triangle.pointIndices[2]+points2D.size();
          n=normalStartIndex+(normalIndex++);
          uv1=uvStartIndex+triangle.pointIndices[0];
          uv2=uvStartIndex+triangle.pointIndices[1];
          uv3=uvStartIndex+triangle.pointIndices[2];

          file<<"f "<<v1<<"/"<<uv1<<"/"<<n<<" "<<v2<<"/"<<uv2<<"/"<<n<<" "<<v3<<"/"<<uv3<<"/"<<n<<"\n";
        }

      const Stroke& contour=petal2D.contour;
      const int contourSize=contour.size();
      for(int i=0;i<contourSize;++i){
          int v1,v2,v3,n,uv1,uv2,uv3;
          v1=vertexStartIndex+indexMap[contour[i]];
          v2=vertexStartIndex+indexMap[contour[(i+1)%contourSize]];
          v3=vertexStartIndex+indexMap[contour[i]]+points2D.size();
          n=normalStartIndex+(normalIndex++);
          uv1=uvStartIndex+indexMap[contour[i]];
          uv2=uvStartIndex+indexMap[contour[(i+1)%contourSize]];
          uv3=uvStartIndex+indexMap[contour[i]];
          file<<"f "<<v1<<"/"<<uv1<<"/"<<n<<" "<<v2<<"/"<<uv2<<"/"<<n<<" "<<v3<<"/"<<uv3<<"/"<<n<<"\n";

          v1=vertexStartIndex+indexMap[contour[i]]+points2D.size();
          v2=vertexStartIndex+indexMap[contour[(i+1)%contourSize]];
          v3=vertexStartIndex+indexMap[contour[(i+1)%contourSize]]+points2D.size();
          n=normalStartIndex+(normalIndex++);
          uv1=uvStartIndex+indexMap[contour[i]];
          uv2=uvStartIndex+indexMap[contour[(i+1)%contourSize]];
          uv3=uvStartIndex+indexMap[contour[(i+1)%contourSize]];
          file<<"f "<<v1<<"/"<<uv1<<"/"<<n<<" "<<v2<<"/"<<uv2<<"/"<<n<<" "<<v3<<"/"<<uv3<<"/"<<n<<"\n";
        }

    }
}
