//****************************************************************************//
// coreskeleton.cpp                                                           //
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

#include "error.h"
#include "coreskeleton.h"
#include "corebone.h"

 /*****************************************************************************/
/** Constructs the core skeleton instance.
  *
  * This function is the default constructor of the core skeleton instance.
  *****************************************************************************/

CalCoreSkeleton::CalCoreSkeleton()
{
}

 /*****************************************************************************/
/** Destructs the core skeleton instance.
  *
  * This function is the destructor of the core skeleton instance.
  *****************************************************************************/

CalCoreSkeleton::~CalCoreSkeleton()
{
  assert(m_vectorRootCoreBoneId.empty());
  assert(m_vectorCoreBone.empty());
}

 /*****************************************************************************/
/** Adds a core bone.
  *
  * This function adds a core bone to the core skeleton instance.
  *
  * @param pCoreBone A pointer to the core bone that should be added.
  *
  * @return One of the following values:
  *         \li the assigned bone \b ID of the added core bone
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreSkeleton::addCoreBone(CalCoreBone *pCoreBone)
{
  // get next bone id
  int boneId;
  boneId = m_vectorCoreBone.size();

  m_vectorCoreBone.push_back(pCoreBone);

  // if necessary, add the core bone to the root bone list
  if(pCoreBone->getParentId() == -1)
  {
    m_vectorRootCoreBoneId.push_back(boneId);
  }

  return boneId;
}

 /*****************************************************************************/
/** Calculates the current state.
  *
  * This function calculates the current state of the core skeleton instance by
  * calculating all the core bone states.
  *****************************************************************************/

void CalCoreSkeleton::calculateState()
{
  // calculate all bone states of the skeleton
  rde::vector<int>::iterator iteratorRootCoreBoneId;
  for(iteratorRootCoreBoneId = m_vectorRootCoreBoneId.begin(); iteratorRootCoreBoneId != m_vectorRootCoreBoneId.end(); ++iteratorRootCoreBoneId)
  {
    m_vectorCoreBone[*iteratorRootCoreBoneId]->calculateState();
  }
}

 /*****************************************************************************/
/** Creates the core skeleton instance.
  *
  * This function creates the core skeleton instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreSkeleton::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core skeleton instance.
  *
  * This function destroys all data stored in the core skeleton instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreSkeleton::destroy()
{
  // destroy all core animations
  rde::vector<CalCoreBone *>::iterator iteratorCoreBone;
  for(iteratorCoreBone = m_vectorCoreBone.begin(); iteratorCoreBone != m_vectorCoreBone.end(); ++iteratorCoreBone)
  {
    (*iteratorCoreBone)->destroy();
    delete (*iteratorCoreBone);
  }

  m_vectorCoreBone.clear();

  // clear root bone id list
  m_vectorRootCoreBoneId.clear();
}

 /*****************************************************************************/
/** Provides access to a core bone.
  *
  * This function returns the core bone with the given ID.
  *
  * @param coreBoneId The ID of the core bone that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreBone *CalCoreSkeleton::getCoreBone(int coreBoneId)
{
  if((coreBoneId < 0) || (coreBoneId >= (int)m_vectorCoreBone.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreBone[coreBoneId];
}

 /*****************************************************************************/
/** Returns the ID of a specified core bone.
  *
  * This function returns the ID of a specified core bone.
  *
  * @param strName The name of the core bone that should be returned.
  *
  * @return One of the following values:
  *         \li the \b ID of the core bone
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreSkeleton::getCoreBoneId(const char* strName)
{
  int boneId;
  for(boneId = 0; boneId < (int)m_vectorCoreBone.size(); boneId++)
  {
    if(strncmp(m_vectorCoreBone[boneId]->getName(), strName, 32) == 0)
		return boneId;
  }

  return -1;
}

 /*****************************************************************************/
/** Returns the root core bone id vector.
  *
  * This function returns the vector that contains all root core bone IDs of the
  * core skeleton instance.
  *
  * @return A reference to the root core bone id vector.
  *****************************************************************************/

rde::vector<int>& CalCoreSkeleton::getVectorRootCoreBoneId()
{
  return m_vectorRootCoreBoneId;
}

 /*****************************************************************************/
/** Returns the core bone vector.
  *
  * This function returns the vector that contains all core bones of the core
  * skeleton instance.
  *
  * @return A reference to the core bone vector.
  *****************************************************************************/

rde::vector<CalCoreBone *>& CalCoreSkeleton::getVectorCoreBone()
{
  return m_vectorCoreBone;
}

//****************************************************************************//
