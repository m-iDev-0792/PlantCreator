#ifndef PLANTOBJECT_H
#define PLANTOBJECT_H
#include "ElementObjects.h"
#include <fstream>

class PlantObject:public HJGraphics::GeometryObject
{
public:

  int vertexStartIndex;
  int uvStartIndex;
  int normalStartIndex;
  std::string tag;
  PlantObject();

  virtual void exportVertices(int& _vertexStartIndex,std::ofstream& file);

  virtual void exportUVs(int& _uvStartIndex,std::ofstream& file);

  virtual void exportNormals(int& _normalStartIndex,std::ofstream& file);

  virtual void exportFaces(std::ofstream& file);

  virtual void exportMaterial(std::ofstream& file);
};

#endif // PLANTOBJECT_H
