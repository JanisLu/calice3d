//****************************************************************************//
// coremesh.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMESH_H
#define CAL_COREMESH_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreSubmesh;
class CalLoader;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core mesh class.
  *****************************************************************************/

class CAL3D_API CalCoreMesh
{
  friend class CalLoader;
// member variables
protected:
  rde::vector<CalCoreSubmesh *> m_vectorCoreSubmesh;

// constructors/destructor
public:
  CalCoreMesh();
  virtual ~CalCoreMesh();

// member functions	
public:
  int addCoreSubmesh(CalCoreSubmesh *pCoreSubmesh);
  bool create();
  void destroy();
  CalCoreSubmesh *getCoreSubmesh(int id);
  int getCoreSubmeshCount();
  rde::vector<CalCoreSubmesh *>& getVectorCoreSubmesh();
};

#endif

//****************************************************************************//
