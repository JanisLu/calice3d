//****************************************************************************//
// corebone.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREBONE_H
#define CAL_COREBONE_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"
#include "vector.h"
#include "quaternion.h"

#define CAL3D_MAX_BONE_NAME 512

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreSkeleton;
class CalLoader;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The core bone class.
  *****************************************************************************/

class CAL3D_API CalCoreBone
{
  friend class CalLoader;
// member variables
protected:
  char m_strName[CAL3D_MAX_BONE_NAME];
  CalCoreSkeleton *m_pCoreSkeleton;
  int m_parentId;
  rde::vector<int> m_arrayChildId;
  CalVector m_translation;
  CalQuaternion m_rotation;
  CalVector m_translationAbsolute;
  CalQuaternion m_rotationAbsolute;
  CalVector m_translationBoneSpace;
  CalQuaternion m_rotationBoneSpace;
  Cal::UserData m_userData;

// constructors/destructor
public:
  CalCoreBone();
  virtual ~CalCoreBone();

// member functions
public:
  bool addChildId(int childId);
  void calculateState();
  bool create(const char* strName);
  void destroy();
  rde::vector<int>& getChildIds();
  const char* getName();
  int getParentId();
  const CalQuaternion& getRotation();
  const CalQuaternion& getRotationAbsolute();
  const CalQuaternion& getRotationBoneSpace();
  const CalVector& getTranslation();
  const CalVector& getTranslationAbsolute();
  const CalVector& getTranslationBoneSpace();
  Cal::UserData getUserData();
  void setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton);
  void setParentId(int parentId);
  void setRotation(const CalQuaternion& rotation);
  void setRotationBoneSpace(const CalQuaternion& rotation);
  void setTranslation(const CalVector& translation);
  void setTranslationBoneSpace(const CalVector& translation);
  void setUserData(Cal::UserData userData);
};

#endif

//****************************************************************************//
