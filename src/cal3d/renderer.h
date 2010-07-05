//****************************************************************************//
// renderer.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_RENDERER_H
#define CAL_RENDERER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalModel;
class CalSubmesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The renderer class.
  *****************************************************************************/
#define VERTEX_XYZ              0x002
#define VERTEX_NORMAL           0x010
#define VERTEX_TEX0             0x000
#define VERTEX_TEX1             0x100
#define VERTEX_TEX2             0x200
#define VERTEX_TEX3             0x300
#define VERTEX_TEX4             0x400
#define VERTEX_TEX5             0x500
#define VERTEX_TEX6             0x600
#define VERTEX_TEX7             0x700
#define VERTEX_TEX8             0x800

class CAL3D_API CalRenderer
{
// member variables
public:
  CalModel *m_pModel;
  CalSubmesh *m_pSelectedSubmesh;

// constructors/destructor
public:
  CalRenderer();
  virtual ~CalRenderer();

// member functions	
public:
  bool beginRendering();
  bool create(CalModel *pModel);
  void destroy();
  void endRendering();
  void getAmbientColor(unsigned char *pColorBuffer);
  void getDiffuseColor(unsigned char *pColorBuffer);
  int getFaceCount();
  int getFaces(int *pFaceBuffer);
  int getMapCount();
  Cal::UserData getMapUserData(int mapId);
  int getMeshCount();
  float getShininess();
  void getSpecularColor(unsigned char *pColorBuffer);
  int getSubmeshCount(int meshId);
  int getVertexCount();
  int getVertices(void *pVertexBuffer, int iVertexFormat);
  bool selectMeshSubmesh(int meshId, int submeshId);
};

#endif

//****************************************************************************//
