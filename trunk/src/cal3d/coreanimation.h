//****************************************************************************//
// coreanimation.h                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREANIMATION_H
#define CAL_COREANIMATION_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreTrack;
class CalLoader;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core animation class.
  *****************************************************************************/

class CAL3D_API CalCoreAnimation
{
  friend class CalLoader;
// member variables
protected:
  float m_duration;
  rde::vector<CalCoreTrack *> m_arrayCoreTrack;

// constructors/destructor
public:
  CalCoreAnimation();
  virtual ~CalCoreAnimation();

// member functions	
public:
  bool addCoreTrack(CalCoreTrack *pCoreTrack);
  bool create();
  void destroy();
  CalCoreTrack *getCoreTrack(int coreBoneId);
  float getDuration();
  rde::vector<CalCoreTrack *>& getCoreTracks();
  void setDuration(float duration);
};

#endif

//****************************************************************************//
