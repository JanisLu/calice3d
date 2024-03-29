//****************************************************************************//
// coresubmesh.h                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_CORESUBMESH_H
#define CAL_CORESUBMESH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"
#include "vector.h"

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core submesh class.
  *****************************************************************************/

class CAL3D_API CalCoreSubmesh
{
// misc
public:
  /// The core submesh TextureCoordinate.
  typedef struct
  {
    float u, v;
  } TextureCoordinate;

  /// The core submesh Influence.
  typedef struct
  {
    int boneId;
    float weight;
  } Influence;

  /// The core submesh PhysicalProperty.
  typedef struct
  {
    float weight;
  } PhysicalProperty;

  /// The core submesh Vertex.
  typedef struct
  {
    CalVector position;
    CalVector normal;
    rde::vector<Influence> vectorInfluence;
    int collapseId;
    int faceCollapseCount;
  } Vertex;

  /// The core submesh Face.
  typedef struct
  {
    int vertexId[3];
  } Face;

  /// The core submesh Spring.
  typedef struct
  {
    int vertexId[2];
    float springCoefficient;
    float idleLength;
  } Spring;

// member variables
protected:
  rde::vector<Vertex> m_vectorVertex;
  rde::vector<rde::vector<TextureCoordinate> > m_vectorvectorTextureCoordinate;
  rde::vector<PhysicalProperty> m_vectorPhysicalProperty;
  rde::vector<Face> m_vectorFace;
  rde::vector<Spring> m_vectorSpring;
  int m_coreMaterialThreadId;
  int m_lodCount;

// constructors/destructor
public:
  CalCoreSubmesh();
  virtual ~CalCoreSubmesh();

// member functions	
public:
  bool create();
  void destroy();
  int getCoreMaterialThreadId();
  int getFaceCount();
  int getLodCount();
  int getSpringCount();
  rde::vector<Face>& getVectorFace();
  rde::vector<PhysicalProperty>& getVectorPhysicalProperty();
  rde::vector<Spring>& getVectorSpring();
  rde::vector<rde::vector<TextureCoordinate> >& getVectorVectorTextureCoordinate();
  rde::vector<Vertex>& getVectorVertex();
  int getVertexCount();
  bool reserve(int vertexCount, int textureCoordinateCount, int faceCount, int springCount);
  void setCoreMaterialThreadId(int coreMaterialThreadId);
  void setFace(int faceId, const Face& face);
  void setLodCount(int lodCount);
  void setPhysicalProperty(int vertexId, const PhysicalProperty& physicalProperty);
  void setSpring(int springId, const Spring& spring);
  void setTextureCoordinate(int vertexId, int textureCoordinateId, const TextureCoordinate& textureCoordinate);
  void setVertex(int vertexId, const Vertex& vertex);
};

#endif

//****************************************************************************//
