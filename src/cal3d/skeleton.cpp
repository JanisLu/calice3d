//****************************************************************************//
// calskeleton.cpp                                                            //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "skeleton.h"
#include "error.h"
#include "bone.h"
#include "coreskeleton.h"
#include "corebone.h" // DEBUG

 /*****************************************************************************/
/** Constructs the skeleton instance.
  *
  * This function is the default constructor of the skeleton instance.
  *****************************************************************************/

CalSkeleton::CalSkeleton()
{
  m_pCoreSkeleton = 0;
}

 /*****************************************************************************/
/** Destructs the skeleton instance.
  *
  * This function is the destructor of the skeleton instance.
  *****************************************************************************/

CalSkeleton::~CalSkeleton()
{
  assert(m_vectorBone.empty());
}

 /*****************************************************************************/
/** Calculates the state of the skeleton instance.
  *
  * This function calculates the state of the skeleton instance by recursively
  * calculating the states of its bones.
  *****************************************************************************/

void CalSkeleton::calculateState()
{
  // calculate all bone states of the skeleton
  rde::vector<int>& vectorRootCoreBoneId = m_pCoreSkeleton->getVectorRootCoreBoneId();

  rde::vector<int>::iterator iteratorRootBoneId;
  for(iteratorRootBoneId = vectorRootCoreBoneId.begin(); iteratorRootBoneId != vectorRootCoreBoneId.end(); ++iteratorRootBoneId)
  {
    m_vectorBone[*iteratorRootBoneId]->calculateState();
  }
}

 /*****************************************************************************/
/** Clears the state of the skeleton instance.
  *
  * This function clears the state of the skeleton instance by recursively
  * clearing the states of its bones.
  *****************************************************************************/

void CalSkeleton::clearState()
{
  // clear all bone states of the skeleton
  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    (*iteratorBone)->clearState();
  }
}

 /*****************************************************************************/
/** Creates the skeleton instance.
  *
  * This function creates the skeleton instance based on a core skeleton.
  *
  * @param pCoreSkeleton A pointer to the core skeleton on which this skeleton
  *                      instance should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSkeleton::create(CalCoreSkeleton *pCoreSkeleton)
{
  if(pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreSkeleton = pCoreSkeleton;

  // clone the skeleton structure of the core skeleton
  rde::vector<CalCoreBone *>& vectorCoreBone = pCoreSkeleton->getVectorCoreBone();

  // get the number of bones
  int boneCount;
  boneCount = vectorCoreBone.size();

  // reserve space in the bone vector
  m_vectorBone.reserve(boneCount);

  // clone every core bone
  int boneId;
  for(boneId = 0; boneId < boneCount; boneId++)
  {
    CalBone *pBone;
    pBone = new CalBone();
    if(pBone == 0)
    {
      CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
      return false;
    }

    // create a bone for every core bone
    if(!pBone->create(vectorCoreBone[boneId]))
    {
      delete pBone;
      return false;
    }

    // set skeleton in the bone instance
    pBone->setSkeleton(this);

    // insert bone into bone vector
    m_vectorBone.push_back(pBone);
  }

  return true;
}

 /*****************************************************************************/
/** Destroys the skeleton instance.
  *
  * This function destroys all data stored in the skeleton instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalSkeleton::destroy()
{
  // destroy all bones
  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    (*iteratorBone)->destroy();
    delete (*iteratorBone);
  }
  m_vectorBone.clear();

  m_pCoreSkeleton = 0;
}

 /*****************************************************************************/
/** Provides access to a bone.
  *
  * This function returns the bone with the given ID.
  *
  * @param boneId The ID of the bone that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the bone
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalBone *CalSkeleton::getBone(int boneId)
{
  return m_vectorBone[boneId];
}

 /*****************************************************************************/
/** Provides access to the core skeleton.
  *
  * This function returns the core skeleton on which this skeleton instance is
  * based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSkeleton *CalSkeleton::getCoreSkeleton()
{
  return m_pCoreSkeleton;
}

 /*****************************************************************************/
/** Returns the bone vector.
  *
  * This function returns the vector that contains all bones of the skeleton
  * instance.
  *
  * @return A reference to the bone vector.
  *****************************************************************************/

rde::vector<CalBone *>& CalSkeleton::getVectorBone()
{
  return m_vectorBone;
}

 /*****************************************************************************/
/** Locks the state of the skeleton instance.
  *
  * This function locks the state of the skeleton instance by recursively
  * locking the states of its bones.
  *****************************************************************************/

void CalSkeleton::lockState()
{
  // lock all bone states of the skeleton
  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    (*iteratorBone)->lockState();
  }
}

//****************************************************************************//



//****************************************************************************//
//****************************************************************************//
//****************************************************************************//
// DEBUG-/TEST-CODE                                                           //
//****************************************************************************//
//****************************************************************************//
//****************************************************************************//

int CalSkeleton::getBonePoints(float *pPoints)
{
  int nrPoints;
  nrPoints = 0;

  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    const CalVector& translation = (*iteratorBone)->getTranslationAbsolute();

    *pPoints++ = translation[0];
    *pPoints++ = translation[1];
    *pPoints++ = translation[2];

    nrPoints++;
  }

  return nrPoints;
}

int CalSkeleton::getBonePointsStatic(float *pPoints)
{
  int nrPoints;
  nrPoints = 0;

  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    const CalVector& translation = (*iteratorBone)->getCoreBone()->getTranslationAbsolute();

    *pPoints++ = translation[0];
    *pPoints++ = translation[1];
    *pPoints++ = translation[2];

    nrPoints++;
  }

  return nrPoints;
}

int CalSkeleton::getBoneLines(float *pLines)
{
  int nrLines;
  nrLines = 0;

  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    int parentId;
    parentId = (*iteratorBone)->getCoreBone()->getParentId();

    if(parentId != -1)
    {
      CalBone *pParent;
      pParent = m_vectorBone[parentId];

      const CalVector& translation = (*iteratorBone)->getTranslationAbsolute();
      const CalVector& translationParent = pParent->getTranslationAbsolute();

      *pLines++ = translationParent[0];
      *pLines++ = translationParent[1];
      *pLines++ = translationParent[2];

      *pLines++ = translation[0];
      *pLines++ = translation[1];
      *pLines++ = translation[2];

      nrLines++;
    }
  }

  return nrLines;
}

int CalSkeleton::getBoneLinesStatic(float *pLines)
{
  int nrLines;
  nrLines = 0;

  rde::vector<CalBone *>::iterator iteratorBone;
  for(iteratorBone = m_vectorBone.begin(); iteratorBone != m_vectorBone.end(); ++iteratorBone)
  {
    int parentId;
    parentId = (*iteratorBone)->getCoreBone()->getParentId();

    if(parentId != -1)
    {
      CalBone *pParent;
      pParent = m_vectorBone[parentId];

      const CalVector& translation = (*iteratorBone)->getCoreBone()->getTranslationAbsolute();
      const CalVector& translationParent = pParent->getCoreBone()->getTranslationAbsolute();

      *pLines++ = translationParent[0];
      *pLines++ = translationParent[1];
      *pLines++ = translationParent[2];

      *pLines++ = translation[0];
      *pLines++ = translation[1];
      *pLines++ = translation[2];

      nrLines++;
    }
  }

  return nrLines;
}

//****************************************************************************//
//****************************************************************************//
//****************************************************************************//
// END DEBUG-/TEST-CODE                                                       //
//****************************************************************************//
//****************************************************************************//
//****************************************************************************//
