//****************************************************************************//
// springsystem.cpp                                                           //
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
#include "springsystem.h"
#include "coremodel.h"
#include "model.h"
#include "mesh.h"
#include "submesh.h"
#include "coresubmesh.h"
#include "vector.h"

 /*****************************************************************************/
/** Constructs the spring system instance.
  *
  * This function is the default constructor of the spring system instance.
  *****************************************************************************/

CalSpringSystem::CalSpringSystem()
{
  m_pModel = 0;
}

 /*****************************************************************************/
/** Destructs the spring system instance.
  *
  * This function is the destructor of the spring system instance.
  *****************************************************************************/

CalSpringSystem::~CalSpringSystem()
{
}

 /*****************************************************************************/
/** Calculates the forces on each unbound vertex.
  *
  * This function calculates the forces on each unbound vertex of a specific
  * submesh.
  *
  * @param pSubmesh A pointer to the submesh from which the forces should be
  *                 calculated.
  * @param deltaTime The elapsed time in seconds since the last calculation.
  *****************************************************************************/

void CalSpringSystem::calculateForces(CalSubmesh *pSubmesh, float deltaTime)
{
  // get the vertex vector of the submesh
	rde::vector<CalSubmesh::Vertex>& vectorVertex = pSubmesh->getVectorVertex();

  // get the vertex vector of the submesh
  rde::vector<CalSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getVectorPhysicalProperty();

  // get the physical property vector of the core submesh
  rde::vector<CalCoreSubmesh::PhysicalProperty>& vectorCorePhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // loop through all the vertices
  int vertexId;
  for(vertexId = 0; vertexId < (int)vectorVertex.size(); vertexId++)
  {
    // get the physical property of the vertex
    CalSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

    // get the physical property of the core vertex
    CalCoreSubmesh::PhysicalProperty& corePhysicalProperty = vectorCorePhysicalProperty[vertexId];

    // only take vertices with a weight > 0 into account
    if(corePhysicalProperty.weight > 0.0f)
    {
//      physicalProperty.force.set(0.0f, 0.0f /*0.5f*/, corePhysicalProperty.weight * -98.1f);
      physicalProperty.force.set(0.0f, 0.5f, corePhysicalProperty.weight * -98.1f);
    }
  }
}

 /*****************************************************************************/
/** Calculates the vertices influenced by the spring system instance.
  *
  * This function calculates the vertices influenced by the spring system
  * instance.
  *
  * @param pSubmesh A pointer to the submesh from which the vertices should be
  *                 calculated.
  * @param deltaTime The elapsed time in seconds since the last calculation.
  *****************************************************************************/

void CalSpringSystem::calculateVertices(CalSubmesh *pSubmesh, float deltaTime)
{
  // get the vertex vector of the submesh
  rde::vector<CalSubmesh::Vertex>& vectorVertex = pSubmesh->getVectorVertex();

  // get the physical property vector of the submesh
  rde::vector<CalSubmesh::PhysicalProperty>& vectorPhysicalProperty = pSubmesh->getVectorPhysicalProperty();

  // get the physical property vector of the core submesh
  rde::vector<CalCoreSubmesh::PhysicalProperty>& vectorCorePhysicalProperty = pSubmesh->getCoreSubmesh()->getVectorPhysicalProperty();

  // loop through all the vertices
  int vertexId;
  for(vertexId = 0; vertexId < (int)vectorVertex.size(); vertexId++)
  {
    // get the vertex
	CalSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // get the physical property of the vertex
    CalSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

    // get the physical property of the core vertex
    CalCoreSubmesh::PhysicalProperty& corePhysicalProperty = vectorCorePhysicalProperty[vertexId];

    // store current position for later use
    CalVector position;
    position = physicalProperty.position;

    // only take vertices with a weight > 0 into account
    if(corePhysicalProperty.weight > 0.0f)
    {
      // do the Verlet step
      physicalProperty.position += (position - physicalProperty.positionOld) * 0.99f + physicalProperty.force / corePhysicalProperty.weight * deltaTime * deltaTime;
    }
    else
    {
      physicalProperty.position = vectorVertex[vertexId].position;
    }

    // make the current position the old one
    physicalProperty.positionOld = position;

    // set the new position of the vertex
    vertex.position = physicalProperty.position;

    // clear the accumulated force on the vertex
    physicalProperty.force.clear();
  }

  // get the spring vector of the core submesh
  rde::vector<CalCoreSubmesh::Spring>& vectorSpring = pSubmesh->getCoreSubmesh()->getVectorSpring();

  // iterate a few times to relax the constraints
  int iterationCount;
#define ITERATION_COUNT 2
  for(iterationCount = 0; iterationCount < ITERATION_COUNT; iterationCount++)
  {
    // loop through all the springs
    rde::vector<CalCoreSubmesh::Spring>::iterator iteratorSpring;
    for(iteratorSpring = vectorSpring.begin(); iteratorSpring != vectorSpring.end(); ++iteratorSpring)
    {
      // get the spring
      CalCoreSubmesh::Spring& spring = *iteratorSpring;

      // compute the difference between the two spring vertices
      CalVector distance;
      distance = vectorVertex[spring.vertexId[1]].position - vectorVertex[spring.vertexId[0]].position;

      // get the current length of the spring
      float length;
      length = distance.length();

      if(length > 0.0f)
      {
        float factor[2];
        factor[0] = (length - spring.idleLength) / length;
        factor[1] = factor[0];

        if(vectorCorePhysicalProperty[spring.vertexId[0]].weight > 0.0f)
        {
          factor[0] /= 2.0f;
          factor[1] /= 2.0f;
        }
        else
        {
          factor[0] = 0.0f;
        }

        if(vectorCorePhysicalProperty[spring.vertexId[1]].weight <= 0.0f)
        {
          factor[0] *= 2.0f;
          factor[1] = 0.0f;
        }

        vectorVertex[spring.vertexId[0]].position += distance * factor[0];
        vectorPhysicalProperty[spring.vertexId[0]].position = vectorVertex[spring.vertexId[0]].position;

        vectorVertex[spring.vertexId[1]].position -= distance * factor[1];
        vectorPhysicalProperty[spring.vertexId[1]].position = vectorVertex[spring.vertexId[1]].position;
      }
    }
  }
/* DEBUG-CODE ********************
  CalVector spherePosition(Sphere.x, Sphere.y, Sphere.z);
  float sphereRadius = Sphere.radius;

  // loop through all the vertices
  for(vertexId = 0; vertexId < (int)vectorVertex.size(); vertexId++)
  {
    // get the vertex
    CalVector& vertex = vectorVertex[vertexId];

    // get the physical property of the vertex
    CalSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

    // get the physical property of the core vertex
    CalCoreSubmesh::PhysicalProperty& corePhysicalProperty = vectorCorePhysicalProperty[vertexId];

    // only take vertices with a weight > 0 into account
    if(corePhysicalProperty.weight > 0.0f)
    {
      CalVector position;
      position = physicalProperty.position;
      position -= spherePosition;

      float length;
      length = position.normalize();

      if(length < sphereRadius)
      {
        position *= sphereRadius;
        position += spherePosition;

        physicalProperty.position = position;
        physicalProperty.positionOld = position;
        vertex = physicalProperty.position;
      }
    }
  }
*********************************/
}

 /*****************************************************************************/
/** Creates the spring system instance.
  *
  * This function creates the spring system instance.
  *
  * @param pModel A pointer to the model that should be managed with this
  *               spring system instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSpringSystem::create(CalModel *pModel)
{
  if(pModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pModel = pModel;

  CalCoreModel *pCoreModel;
  pCoreModel = m_pModel->getCoreModel();
  if(pCoreModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Destroys the spring system instance.
  *
  * This function destroys all data stored in the spring system instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalSpringSystem::destroy()
{
  m_pModel = 0;
}

 /*****************************************************************************/
/** Updates all the spring systems in the attached meshes.
  *
  * This functon updates all the spring systems in the attached meshes.
  *****************************************************************************/

void CalSpringSystem::update(float deltaTime)
{
  // get the attached meshes vector
  rde::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  // loop through all the attached meshes
  rde::vector<CalMesh *>::iterator iteratorMesh;
  for(iteratorMesh = vectorMesh.begin(); iteratorMesh != vectorMesh.end(); ++iteratorMesh)
  {
    // get the ssubmesh vector of the mesh
    rde::vector<CalSubmesh *>& vectorSubmesh = (*iteratorMesh)->getVectorSubmesh();

    // loop through all the submeshes of the mesh
    rde::vector<CalSubmesh *>::iterator iteratorSubmesh;
    for(iteratorSubmesh = vectorSubmesh.begin(); iteratorSubmesh != vectorSubmesh.end(); ++iteratorSubmesh)
    {
      // check if the submesh contains a spring system
      if((*iteratorSubmesh)->getCoreSubmesh()->getSpringCount() > 0)
      {
        // calculate the new forces on each unbound vertex
        calculateForces(*iteratorSubmesh, deltaTime);

        // calculate the vertices influenced by the spring system
        calculateVertices(*iteratorSubmesh, deltaTime);
      }
    }
  }
}

//****************************************************************************//
