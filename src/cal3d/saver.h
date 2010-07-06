//****************************************************************************//
// saver.h                                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SAVER_H
#define CAL_SAVER_H

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

class CAL3D_API CalSaver
{
// constructors/destructor
public:
  CalSaver();
  virtual ~CalSaver();

// member functions
public:
  bool saveCoreAnimation(const char* strFilename, CalCoreAnimation *pCoreAnimation);
  bool saveCoreMaterial(const char* strFilename, CalCoreMaterial *pCoreMaterial);
  bool saveCoreMesh(const char* strFilename, CalCoreMesh *pCoreMesh);
  bool saveCoreSkeleton(const char* strFilename, CalCoreSkeleton *pCoreSkeleton);

protected:
  bool saveCoreBones(std::ofstream& file, const char* strFilename, CalCoreBone *pCoreBone);
  bool saveCoreKeyframe(std::ofstream& file, const char* strFilename, CalCoreKeyframe *pCoreKeyframe);
  bool saveCoreSubmesh(std::ofstream& file, const char* strFilename, CalCoreSubmesh *pCoreSubmesh);
  bool saveCoreTrack(std::ofstream& file, const char* strFilename, CalCoreTrack *pCoreTrack);
};

#endif

//****************************************************************************//
