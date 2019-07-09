#ifndef BRANCH_H
#define BRANCH_H
#include "ElementObjects.h"
#include "plantobject.h"
#include <vector>
#include "utility.h"
#include "commondatatype.h"
class Branch:public PlantObject
{
public:
  Stroke3D branchPoints;
  std::vector<std::vector<glm::vec3>> points;
  std::vector<glm::vec3> normals;
  std::vector<std::vector<glm::vec2>> uvs;// 0:top then 1:buttom
  int n;
  float r;
  int rootIndex;
  HJGraphics::Material material;
  static HJGraphics::Texture2D* branchCommonTexture;
  Branch(const Stroke3D& branchPoints,int _n,float _r,int _rootIndex);

  void draw() override;

  void draw(HJGraphics::Shader shader) override;

  void drawLight(HJGraphics::Light *light) override;

  void writeVerticesData() override;

  void writeObjectPropertyUniform(HJGraphics::Shader *shader) override;

  //override for plantobject functions
  void exportVertices(int& _vertexStartIndex,std::ofstream& file) override;

  void exportUVs(int& _uvStartIndex,std::ofstream& file) override;

  void exportNormals(int& _normalStartIndex,std::ofstream& file) override;

  void exportFaces(std::ofstream& file) override;

  void exportMaterial(std::ofstream& file) override;
};

#endif // BRANCH_H
