#include "leafedge.h"
#include "DebugUtility.h"
HJGraphics::Shader* LeafEdge::defaultShader=nullptr;
LeafEdge::LeafEdge(const std::vector<PetalStroke3D>& leafstroke3ds)
{
  if(defaultShader==nullptr)defaultShader=new HJGraphics::Shader("/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Shaders/gridVertex.glsl","/Users/hezhenbang/Documents/C++/PlantCreator/HJGraphics/Shaders/gridFragment.glsl");

  model=glm::mat4(1.0f);
  lineColor=glm::vec3(0.8f,0.0f,0.0f);
  int totalPointNum=0;
  for(auto& s:leafstroke3ds){
      totalPointNum+=s.stroke.size();
    }
  float* data=new float[totalPointNum*3];
  int index=0;
  for(auto& s:leafstroke3ds){
      leafPointNum.push_back(s.stroke.size());
      for(auto& p:s.stroke){
          data[index]=p.x;
          data[index+1]=p.y;
          data[index+2]=p.z;
          index+=3;
        }
    }
  loadVBOData(data, sizeof(data));
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(GLfloat), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);
  qDebug()<<" data size "<<sizeof(data);
  loadVBOData(data,sizeof(float)*totalPointNum*3);
  delete[] data;
}
void LeafEdge::writeObjectPropertyUniform(HJGraphics::Shader *shader){
  shader->use();
  shader->set4fm("model",model);
  shader->set3fv("lineColor",lineColor);
  shader->bindBlock("sharedMatrices",sharedBindPoint);
}

void LeafEdge::draw(){
  writeObjectPropertyUniform(defaultShader);
  draw(*defaultShader);
}
void LeafEdge::draw(HJGraphics::Shader shader){
  shader.use();
  glBindVertexArray(VAO);
  int startIndex=0;
  for(auto& n:leafPointNum){
      glDrawArrays(GL_LINE_LOOP, startIndex, n);
      startIndex+=n;
    }
}
HJGraphics::Shader* LeafEdge::getDefaultShader(){
  return defaultShader;
}
