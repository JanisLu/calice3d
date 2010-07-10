//****************************************************************************//
// skiner.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SKINER_H
#define CAL_SKINER_H

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
/** The skiner class.
  *****************************************************************************/

class CAL3D_API CalSkiner
{
// member variables
public:
  CalModel *m_pModel;

// constructors/destructor
public:
  CalSkiner();
  virtual ~CalSkiner();

// member functions	
public:
  int calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer, int iVertexFormat);
  bool create(CalModel *pModel);
  void destroy();
  void update();
};

#endif

//****************************************************************************//
