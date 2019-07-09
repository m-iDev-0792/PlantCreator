#ifndef LEAFEDGE_H
#define LEAFEDGE_H
#include "BasicGLObject.h"
#include "painterview.h"
#include <vector>
class LeafEdge:public HJGraphics::BasicGLObject
{
public:
  std::vector<int> leafPointNum;
  glm::vec3 lineColor;

  LeafEdge(const std::vector<PetalStroke3D>& leafstroke3ds);

  static HJGraphics::Shader* defaultShader;

  virtual HJGraphics::Shader* getDefaultShader();

  virtual void draw();

  virtual void draw(HJGraphics::Shader shader);

  void writeObjectPropertyUniform(HJGraphics::Shader *shader);
};

#endif // LEAFEDGE_H
