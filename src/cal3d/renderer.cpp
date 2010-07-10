//****************************************************************************//
// renderer.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "error.h"
#include "renderer.h"
#include "coremodel.h"
#include "model.h"
#include "coremesh.h"
#include "mesh.h"
#include "submesh.h"
#include "skeleton.h"
#include "bone.h"
#include "corematerial.h"
#include "coresubmesh.h"
#include "skiner.h"

 /*****************************************************************************/
/** Constructs the renderer instance.
  *
  * This function is the default constructor of the renderer instance.
  *****************************************************************************/

CalRenderer::CalRenderer()
{
  m_pSelectedSubmesh = 0;
}

 /*****************************************************************************/
/** Destructs the renderer instance.
  *
  * This function is the destructor of the renderer instance.
  *****************************************************************************/

CalRenderer::~CalRenderer()
{
}

 /*****************************************************************************/
/** Initializes the rendering query phase.
  *
  * This function initializes the rendering query phase. It must be called
  * before any rendering queries are executed.
  *****************************************************************************/

bool CalRenderer::beginRendering()
{
  // get the attached meshes vector
  std::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  // check if there are any meshes attached to the model
  if(vectorMesh.size() == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // select the default submesh
  m_pSelectedSubmesh = vectorMesh[0]->getSubmesh(0);
  if(m_pSelectedSubmesh == 0) return false;

  return true;
}

 /*****************************************************************************/
/** Creates the renderer instance.
  *
  * This function creates the renderer instance.
  *
  * @param pModel A pointer to the model that should be managed with this
  *               renderer instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalRenderer::create(CalModel *pModel)
{
  if(pModel == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pModel = pModel;

  return true;
}

 /*****************************************************************************/
/** Destroys the renderer instance.
  *
  * This function destroys all data stored in the renderer instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalRenderer::destroy()
{
  m_pModel = 0;
}

 /*****************************************************************************/
/** Finishes the rendering query phase.
  *
  * This function finishes the rendering query phase. It must be called
  * after all rendering queries have been executed.
  *****************************************************************************/

void CalRenderer::endRendering()
{
  // clear selected submesh
  m_pSelectedSubmesh = 0;
}

 /*****************************************************************************/
/** Provides access to the ambient color.
  *
  * This function returns the ambient color of the material of the selected
  * mesh/submesh.
  *
  * @param pColorBuffer A pointer to the user-provided buffer where the color
  *                     data is written to.
  *****************************************************************************/

void CalRenderer::getAmbientColor(unsigned char *pColorBuffer)
{
  // get the core material
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = m_pModel->getCoreModel()->getCoreMaterial(m_pSelectedSubmesh->getCoreMaterialId());
  if(pCoreMaterial == 0)
  {
    // write default values to the color buffer
    pColorBuffer[0] = 0;
    pColorBuffer[1] = 0;
    pColorBuffer[2] = 0;
    pColorBuffer[3] = 0;

    return;
  }

  // get the ambient color of the material
  CalCoreMaterial::Color& color = pCoreMaterial->getAmbientColor();

  // write it to the color buffer
  pColorBuffer[0] = color.red;
  pColorBuffer[1] = color.green;
  pColorBuffer[2] = color.blue;
  pColorBuffer[3] = color.alpha;
}

 /*****************************************************************************/
/** Provides access to the diffuse color.
  *
  * This function returns the diffuse color of the material of the selected
  * mesh/submesh.
  *
  * @param pColorBuffer A pointer to the user-provided buffer where the color
  *                     data is written to.
  *****************************************************************************/

void CalRenderer::getDiffuseColor(unsigned char *pColorBuffer)
{
  // get the core material
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = m_pModel->getCoreModel()->getCoreMaterial(m_pSelectedSubmesh->getCoreMaterialId());
  if(pCoreMaterial == 0)
  {
    // write default values to the color buffer
    pColorBuffer[0] = 192;
    pColorBuffer[1] = 192;
    pColorBuffer[2] = 192;
    pColorBuffer[3] = 192;

    return;
  }

  // get the diffuse color of the material
  CalCoreMaterial::Color& color = pCoreMaterial->getDiffuseColor();

  // write it to the color buffer
  pColorBuffer[0] = color.red;
  pColorBuffer[1] = color.green;
  pColorBuffer[2] = color.blue;
  pColorBuffer[3] = color.alpha;
}

 /*****************************************************************************/
/** Returns the number of faces.
  *
  * This function returns the number of faces in the selected mesh/submesh.
  *
  * @return The number of faces.
  *****************************************************************************/

int CalRenderer::getFaceCount()
{
  return m_pSelectedSubmesh->getFaceCount();
}

 /*****************************************************************************/
/** Provides access to the face data.
  *
  * This function returns the face data (vertex indices) of the selected
  * mesh/submesh. The LOD setting is taken into account.
  *
  * @param pFaceBuffer A pointer to the user-provided buffer where the face
  *                    data is written to.
  *
  * @return The number of faces written to the buffer.
  *****************************************************************************/

int CalRenderer::getFaces(int *pFaceBuffer)
{
  return m_pSelectedSubmesh->getFaces(pFaceBuffer);
}

 /*****************************************************************************/
/** Returns the number of maps.
  *
  * This function returns the number of maps in the selected mesh/submesh.
  *
  * @return The number of maps.
  *****************************************************************************/

int CalRenderer::getMapCount()
{
  // get the core material
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = m_pModel->getCoreModel()->getCoreMaterial(m_pSelectedSubmesh->getCoreMaterialId());
  if(pCoreMaterial == 0) return 0;

  return pCoreMaterial->getMapCount();
}

 /*****************************************************************************/
/** Provides access to a specified map user data.
  *
  * This function returns the user data stored in the specified map of the
  * material of the selected mesh/submesh.
  *
  * @param mapId The ID of the map.
  *
  * @return One of the following values:
  *         \li the user data stored in the specified map
  *         \li \b 0 if an error happend
  *****************************************************************************/

Cal::UserData CalRenderer::getMapUserData(int mapId)
{
  // get the core material
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = m_pModel->getCoreModel()->getCoreMaterial(m_pSelectedSubmesh->getCoreMaterialId());
  if(pCoreMaterial == 0) return 0;

  // get the map vector
  std::vector<CalCoreMaterial::Map>& vectorMap = pCoreMaterial->getVectorMap();

  // check if the map id is valid
  if((mapId < 0) || (mapId >= (int)vectorMap.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return vectorMap[mapId].userData;
}

 /*****************************************************************************/
/** Returns the number of attached meshes.
  *
  * This function returns the number of meshes attached to the renderer
  * instance.
  *
  * @return The number of attached meshes.
  *****************************************************************************/

int CalRenderer::getMeshCount()
{
  // get the attached meshes vector
  std::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  return vectorMesh.size();
}


 /*****************************************************************************/
/** Returns the shininess factor.
  *
  * This function returns the shininess factor of the material of the selected
  * mesh/submesh..
  *
  * @return The shininess factor.
  *****************************************************************************/

float CalRenderer::getShininess()
{
  // get the core material
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = m_pModel->getCoreModel()->getCoreMaterial(m_pSelectedSubmesh->getCoreMaterialId());
  if(pCoreMaterial == 0) return 50.0f;

  return pCoreMaterial->getShininess();
}

 /*****************************************************************************/
/** Provides access to the specular color.
  *
  * This function returns the specular color of the material of the selected
  * mesh/submesh.
  *
  * @param pColorBuffer A pointer to the user-provided buffer where the color
  *                     data is written to.
  *****************************************************************************/

void CalRenderer::getSpecularColor(unsigned char *pColorBuffer)
{
  // get the core material
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = m_pModel->getCoreModel()->getCoreMaterial(m_pSelectedSubmesh->getCoreMaterialId());
  if(pCoreMaterial == 0)
  {
    // write default values to the color buffer
    pColorBuffer[0] = 255;
    pColorBuffer[1] = 255;
    pColorBuffer[2] = 255;
    pColorBuffer[3] = 0;

    return;
  }

  // get the specular color of the material
  CalCoreMaterial::Color& color = pCoreMaterial->getSpecularColor();

  // write it to the color buffer
  pColorBuffer[0] = color.red;
  pColorBuffer[1] = color.green;
  pColorBuffer[2] = color.blue;
  pColorBuffer[3] = color.alpha;
}

 /*****************************************************************************/
/** Returns the number of submeshes.
  *
  * This function returns the number of submeshes in a given mesh.
  *
  * @param meshId The ID of the mesh for which the number of submeshes should
  *               be returned..
  *
  * @return The number of submeshes.
  *****************************************************************************/

int CalRenderer::getSubmeshCount(int meshId)
{
  // get the attached meshes vector
  std::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  // check if the mesh id is valid
  if((meshId < 0) || (meshId >= (int)vectorMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return vectorMesh[meshId]->getSubmeshCount();
}

 /*****************************************************************************/
/** Returns the number of vertices.
  *
  * This function returns the number of vertices in the selected mesh/submesh.
  *
  * @return The number of vertices.
  *****************************************************************************/

int CalRenderer::getVertexCount()
{
  return m_pSelectedSubmesh->getVertexCount();
}

 /*****************************************************************************/
/** Provides access to the vertex data.
  *
  * This function returns the vertex data of the selected mesh/submesh.
  *
  * @param pVertexBuffer A pointer to the user-provided buffer where the vertex
  *                      data is written to.
  *
  * @return The number of vertices written to the buffer.
  *****************************************************************************/

int CalRenderer::getVertices(void *pVertexBuffer, int iVertexFormat)
{
  // submesh does not handle the vertex data internally, so let the physique calculate it now
  return m_pModel->getSkiner()->calculateVertices(m_pSelectedSubmesh, (float*)pVertexBuffer, iVertexFormat);
}

 /*****************************************************************************/
/** Selects a mesh/submesh for rendering data queries.
  *
  * This function selects a mesh/submesh for further rendering data queries.
  *
  * @param meshId The ID of the mesh that should be used for further rendering
  *               data queries.
  * @param submeshId The ID of the submesh that should be used for further
  *                  rendering data queries.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalRenderer::selectMeshSubmesh(int meshId, int submeshId)
{
  // get the attached meshes vector
  std::vector<CalMesh *>& vectorMesh = m_pModel->getVectorMesh();

  // check if the mesh id is valid
  if((meshId < 0) || (meshId >= (int)vectorMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the core submesh
  m_pSelectedSubmesh = vectorMesh[meshId]->getSubmesh(submeshId);
  if(m_pSelectedSubmesh == 0) return false;

  return true;
}

//****************************************************************************//
