#ifndef PETAL_H
#define PETAL_H
#include "ElementObjects.h"
#include "plantcreator.h"
#include "plantobject.h"
#include <vector>
class Petal:public PlantObject
{
public:
  bool hasTexture;
  static HJGraphics::Texture2D* petalCommonTexture;
  static HJGraphics::Texture2D* leafCommonTexture;
  HJGraphics::Material material;
  PetalStroke petal2D;
  PetalStroke3D petal3D;
  int triangleNum;
  float thickness;
  //for vertices generation
  std::map<QPoint,int> indexMap;
  std::vector<std::vector<bool>> adjMatrix;
  std::map<QPoint,glm::vec3> map2Dto3D;
  std::map<QPoint,glm::vec2> map2DtoUV;
  std::vector<Triangle> triangles;
  std::vector<glm::vec3> normals;
  Petal(const PetalStroke &_petal2D, const PetalStroke3D &_petal3D,bool isFloretPetal=true,float _thickness=0.0);

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

  void exportMaterial(std::ofstream &file) override;
private:
  std::vector<int> btTriangles;
  std::vector<float> btVertices;
  bool useLocalCoordinate;
};

#endif // PETAL_H
