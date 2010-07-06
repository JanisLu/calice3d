//****************************************************************************//
// loader.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_LOADER_H
#define CAL_LOADER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreModel;
class CalCoreSkeleton;
class CalCoreBone;
class CalCoreAnimation;
class CalCoreTrack;
class CalCoreKeyframe;
class CalCoreMesh;
class CalCoreSubmesh;
class CalCoreMaterial;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The loader class.
  *****************************************************************************/

class CAL3D_API CalLoader
{
// constructors/destructor
public:
  CalLoader();
  virtual ~CalLoader();

// member functions
public:
  CalCoreAnimation *loadCoreAnimation(const char* strFilename);
  CalCoreMaterial *loadCoreMaterial(const char* strFilename);
  CalCoreMesh *loadCoreMesh(const char* strFilename);
  CalCoreSkeleton *loadCoreSkeleton(const char* strFilename);

protected:
  CalCoreBone *loadCoreBones(std::ifstream& file, const char* strFilename);
  CalCoreKeyframe *loadCoreKeyframe(std::ifstream& file, const char* strFilename);
  CalCoreSubmesh *loadCoreSubmesh(std::ifstream& file, const char* strFilename);
  CalCoreTrack *loadCoreTrack(std::ifstream& file, const char* strFilename);
};

#endif

//****************************************************************************//
