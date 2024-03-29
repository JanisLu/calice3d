//****************************************************************************//
// mesh.h                                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_MESH_H
#define CAL_MESH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalModel;
class CalCoreMesh;
class CalSubmesh;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The mesh class.
  *****************************************************************************/

class CAL3D_API CalMesh
{
// member variables
protected:
  CalModel *m_pModel;
  CalCoreMesh *m_pCoreMesh;
  rde::vector<CalSubmesh *> m_vectorSubmesh;

// constructors/destructor
public:
  CalMesh();
  virtual ~CalMesh();

// member functions	
public:
  bool create(CalCoreMesh *pCoreMesh);
  void destroy();
  CalCoreMesh *getCoreMesh();
  CalSubmesh *getSubmesh(int id);
  int getSubmeshCount();
  rde::vector<CalSubmesh *>& getVectorSubmesh();
  void setLodLevel(float lodLevel);
  void setModel(CalModel *pModel);
};

#endif

//****************************************************************************//
