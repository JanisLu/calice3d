//----------------------------------------------------------------------------//
// Exporter.cpp                                                               //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "Exporter.h"
#include "BaseInterface.h"
#include "SkeletonCandidate.h"
#include "BoneCandidate.h"
#include "BaseNode.h"
#include "win32/SkeletonExportSheet.h"
#include "win32/AnimationExportSheet.h"
#include "win32/MeshExportSheet.h"
#include "win32/MaterialExportSheet.h"
#include "MeshCandidate.h"
#include "SubmeshCandidate.h"
#include "VertexCandidate.h"
#include "MaterialLibraryCandidate.h"
#include "MaterialCandidate.h"

//----------------------------------------------------------------------------//
// Debug                                                                      //
//----------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------//
// The one and only exporter instance                                         //
//----------------------------------------------------------------------------//

CExporter theExporter;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CExporter::CExporter()
{
	m_pInterface = 0;
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CExporter::~CExporter()
{
}

//----------------------------------------------------------------------------//
// Create an exporter instance for a given interface                          //
//----------------------------------------------------------------------------//

bool CExporter::Create(CBaseInterface *pInterface)
{
	// check if a valid interface is set
	if(pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	m_pInterface = pInterface;

	return true;
}

//----------------------------------------------------------------------------//
// Export the animation to a given file                                       //
//----------------------------------------------------------------------------//

bool CExporter::ExportAnimation(const std::string& strFilename)
{
	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// build a skeleton candidate
	CSkeletonCandidate skeletonCandidate;

	// show export wizard sheet
	CAnimationExportSheet sheet("Cal3D Animation Export", m_pInterface->GetMainWnd());
	sheet.SetSkeletonCandidate(&skeletonCandidate);
	sheet.SetAnimationTime(m_pInterface->GetStartFrame(), m_pInterface->GetEndFrame(), m_pInterface->GetCurrentFrame(), m_pInterface->GetFps());
	sheet.SetWizardMode();
	if(sheet.DoModal() != ID_WIZFINISH) return true;

	// get the number of selected bone candidates
	int selectedCount;
	selectedCount = skeletonCandidate.GetSelectedCount();
	if(selectedCount == 0)
	{
		SetLastError("No bones selected to export.", __FILE__, __LINE__);
		return false;
	}

	// create the core animation instance
	CalCoreAnimation coreAnimation;
	if(!coreAnimation.create())
	{
		SetLastError("Creation of core animation instance failed.", __FILE__, __LINE__);
		return false;
	}

	// set the duration of the animation
	float duration;
	duration = (float)(sheet.GetEndFrame() - sheet.GetStartFrame()) / (float)m_pInterface->GetFps();
	coreAnimation.setDuration(duration);

	// get bone candidate vector
	std::vector<CBoneCandidate *>& vectorBoneCandidate = skeletonCandidate.GetVectorBoneCandidate();

	unsigned int boneCandidateId;
	for(boneCandidateId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
	{
		// get the bone candidate
		CBoneCandidate *pBoneCandidate;
		pBoneCandidate = vectorBoneCandidate[boneCandidateId];

		// only create tracks for the selected bone candidates
		if(pBoneCandidate->IsSelected())
		{
			// allocate new core track instance
			CalCoreTrack *pCoreTrack;
			pCoreTrack = new CalCoreTrack();
			if(pCoreTrack == 0)
			{
				SetLastError("Memory allocation failed.", __FILE__, __LINE__);
				coreAnimation.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			// create the core track instance
			if(!pCoreTrack->create())
			{
				SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
				delete pCoreTrack;
				coreAnimation.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			// set the core bone id
			pCoreTrack->setCoreBoneId(boneCandidateId);

			// add the core track to the core animation instance
			if(!coreAnimation.addCoreTrack(pCoreTrack))
			{
				SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
				delete pCoreTrack;
				coreAnimation.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}
		}
	}

	// start the progress info
	m_pInterface->StartProgressInfo("Exporting to animation file...");

	// calculate the end frame
	int endFrame;
	endFrame = (int)(duration * (float)sheet.GetFps() + 0.5f);

	// calculate the displaced frame
  int displacedFrame;
  displacedFrame = (int)(((float)sheet.GetDisplacement() / (float)m_pInterface->GetFps()) * (float)sheet.GetFps() + 0.5f) % endFrame;

	// calculate the possible wrap frame
  int wrapFrame;
  wrapFrame = (displacedFrame > 0) ? 1 : 0;
  float wrapTime;
  wrapTime = 0.0f;

  int frame;
  int outputFrame;
  for(frame = 0,  outputFrame = 0; frame <= (endFrame + wrapFrame); frame++)
	{
		// update the progress info
		m_pInterface->SetProgressInfo(100.0f * (float)frame / (float)(endFrame + wrapFrame + 1));

		// calculate the time in seconds
		float time;
		time = (float)sheet.GetStartFrame() / (float)m_pInterface->GetFps() + (float)displacedFrame / (float)sheet.GetFps();

/* DEBUG
CString str;
str.Format("frame=%d, endframe=%d, disframe=%d, ouputFrame=%d (%f), time=%f\n", frame, endFrame, displacedFrame, outputFrame, (float)outputFrame / (float)sheet.GetFps() + wrapTime, time);
OutputDebugString(str);
*/

		for(boneCandidateId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
		{
			// get the bone candidate
			CBoneCandidate *pBoneCandidate;
			pBoneCandidate = vectorBoneCandidate[boneCandidateId];

			// only export keyframes for the selected bone candidates
			if(pBoneCandidate->IsSelected())
			{
				// allocate new core keyframe instance
				CalCoreKeyframe *pCoreKeyframe;
				pCoreKeyframe = new CalCoreKeyframe();
				if(pCoreKeyframe == 0)
				{
					SetLastError("Memory allocation failed.", __FILE__, __LINE__);
					coreAnimation.destroy();
					m_pInterface->StopProgressInfo();
					return false;
				}

				// create the core keyframe instance
				if(!pCoreKeyframe->create())
				{
					SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
					delete pCoreKeyframe;
					coreAnimation.destroy();
					m_pInterface->StopProgressInfo();
					return false;
				}

				// set the frame time
				pCoreKeyframe->setTime((float)outputFrame / (float)sheet.GetFps() + wrapTime);

				// get the translation and the rotation of the bone candidate
				CalVector translation;
				CalQuaternion rotation;
				skeletonCandidate.GetTranslationAndRotation(boneCandidateId, time, translation, rotation);

				// set the translation and rotation
				pCoreKeyframe->setTranslation(translation);
				pCoreKeyframe->setRotation(rotation);

				// get the core track for this bone candidate
				CalCoreTrack *pCoreTrack;
				pCoreTrack = coreAnimation.getCoreTrack(pBoneCandidate->GetId());
				if(pCoreTrack == 0)
				{
					SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
					delete pCoreKeyframe;
					coreAnimation.destroy();
					m_pInterface->StopProgressInfo();
					return false;
				}

				// add this core keyframe to the core track
				pCoreTrack->addCoreKeyframe(pCoreKeyframe);
			}
		}

    // calculate the next displaced frame and its frame time
    if(wrapFrame > 0)
    {
      if(displacedFrame == endFrame)
      {
        wrapTime = 0.0001f;
        displacedFrame = 0;
      }
      else
      {
        wrapTime = 0.0f;
        outputFrame++;
        displacedFrame++;
      }
    }
    else
    {
      outputFrame++;
      displacedFrame++;
   }
	}

	// stop the progress info
	m_pInterface->StopProgressInfo();

	// save core animation to the file
	CalSaver saver;
	if(!saver.saveCoreAnimation(strFilename.c_str(), &coreAnimation))
	{
		SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
		coreAnimation.destroy();
		return false;
	}

	// destroy the core animation
	coreAnimation.destroy();

	return true;
}

//----------------------------------------------------------------------------//
// Export the material to a given file                                        //
//----------------------------------------------------------------------------//

bool CExporter::ExportMaterial(const std::string& strFilename)
{
	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// build a material library candidate
	CMaterialLibraryCandidate materialLibraryCandidate;
	if(!materialLibraryCandidate.CreateFromInterface()) return false;

	// show export wizard sheet
	CMaterialExportSheet sheet("Cal3D Material Export", m_pInterface->GetMainWnd());
	sheet.SetMaterialLibraryCandidate(&materialLibraryCandidate);
	sheet.SetWizardMode();
	if(sheet.DoModal() != ID_WIZFINISH) return true;

	// get selected material candidate
	CMaterialCandidate *pMaterialCandidate;
	pMaterialCandidate = materialLibraryCandidate.GetSelectedMaterialCandidate();
	if(pMaterialCandidate == 0)
	{
		SetLastError("No material selected.", __FILE__, __LINE__);
		return false;
	}

	// create the core material instance
	CalCoreMaterial coreMaterial;
	if(!coreMaterial.create())
	{
		SetLastError("Creation of core material instance failed.", __FILE__, __LINE__);
		return false;
	}

	// set the ambient color
	CalCoreMaterial::Color coreColor;
	float color[4];
	pMaterialCandidate->GetAmbientColor(&color[0]);
	coreColor.red = (unsigned char)(255.0f * color[0]);
	coreColor.green = (unsigned char)(255.0f * color[1]);
	coreColor.blue = (unsigned char)(255.0f * color[2]);
	coreColor.alpha = (unsigned char)(255.0f * color[3]);
	coreMaterial.setAmbientColor(coreColor);


	// set the diffuse color
	pMaterialCandidate->GetDiffuseColor(&color[0]);
	coreColor.red = (unsigned char)(255.0f * color[0]);
	coreColor.green = (unsigned char)(255.0f * color[1]);
	coreColor.blue = (unsigned char)(255.0f * color[2]);
	coreColor.alpha = (unsigned char)(255.0f * color[3]);
	coreMaterial.setDiffuseColor(coreColor);

	// set the specular color
	pMaterialCandidate->GetSpecularColor(&color[0]);
	coreColor.red = (unsigned char)(255.0f * color[0]);
	coreColor.green = (unsigned char)(255.0f * color[1]);
	coreColor.blue = (unsigned char)(255.0f * color[2]);
	coreColor.alpha = (unsigned char)(255.0f * color[3]);
	coreMaterial.setSpecularColor(coreColor);

	// set the shininess factor
	coreMaterial.setShininess(pMaterialCandidate->GetShininess());

	// get the map vector of the material candidate
	std::vector<CMaterialCandidate::Map>& vectorMap = pMaterialCandidate->GetVectorMap();

	// reserve memory for all the material data
	if(!coreMaterial.reserve(vectorMap.size()))
	{
		SetLastError("Memory reservation for maps failed.", __FILE__, __LINE__);
		return false;
	}

	// load all maps
	unsigned int mapId;
	for(mapId = 0; mapId < vectorMap.size(); mapId++)
	{
		CalCoreMaterial::Map map;

		// set map data
		strcpy_s(map.strFilename, vectorMap[mapId].strFilename.c_str());

		// set map in the core material instance
		coreMaterial.setMap(mapId, map);
	}

	// save core mesh to the file
	CalSaver saver;
	if(!saver.saveCoreMaterial(strFilename.c_str(), &coreMaterial))
	{
		SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
		coreMaterial.destroy();
		return false;
	}

	// destroy the core mesh
	coreMaterial.destroy();

	return true;
}

//----------------------------------------------------------------------------//
// Export the mesh to a given file                                            //
//----------------------------------------------------------------------------//

bool CExporter::ExportMesh(const std::string& strFilename)
{
	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// build a mesh candidate
	CMeshCandidate meshCandidate;

	// build a skeleton candidate
	CSkeletonCandidate skeletonCandidate;

	// show export wizard sheet
	CMeshExportSheet sheet("Cal3D Mesh Export", m_pInterface->GetMainWnd());
	sheet.SetSkeletonCandidate(&skeletonCandidate);
	sheet.SetMeshCandidate(&meshCandidate);
	sheet.SetWizardMode();
	if(sheet.DoModal() != ID_WIZFINISH) return true;

	// create the core mesh instance
	CalCoreMesh coreMesh;
	if(!coreMesh.create())
	{
		SetLastError("Creation of core mesh instance failed.", __FILE__, __LINE__);
		return false;
	}

	// get the submesh candidate vector
	std::vector<CSubmeshCandidate *>& vectorSubmeshCandidate = meshCandidate.GetVectorSubmeshCandidate();

	// start the progress info
	m_pInterface->StartProgressInfo("Exporting to mesh file...");

	unsigned int submeshCandidateId;
	for(submeshCandidateId = 0; submeshCandidateId < vectorSubmeshCandidate.size(); submeshCandidateId++)
	{
		// update the progress info
		m_pInterface->SetProgressInfo(100.0f * (float)submeshCandidateId / (float)vectorSubmeshCandidate.size());

		// get the submesh candidate
		CSubmeshCandidate *pSubmeshCandidate;
		pSubmeshCandidate = vectorSubmeshCandidate[submeshCandidateId];

		// get the face vector
		std::vector<CSubmeshCandidate::Face>& vectorFace = pSubmeshCandidate->GetVectorFace();

		// check if the submesh actually contains faces
		if(vectorFace.size() > 0)
		{
			// allocate new core submesh instance
			CalCoreSubmesh *pCoreSubmesh;
			pCoreSubmesh = new CalCoreSubmesh();
			if(pCoreSubmesh == 0)
			{
				SetLastError("Memory allocation failed.", __FILE__, __LINE__);
				coreMesh.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			// create the core submesh instance
			if(!pCoreSubmesh->create())
			{
				SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
				delete pCoreSubmesh;
				coreMesh.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			// set the core material id
			//pCoreSubmesh->setCoreMaterialThreadId(pSubmeshCandidate->GetMaterialThreadId());

			// get the vertex candidate vector
			std::vector<CVertexCandidate *>& vectorVertexCandidate = pSubmeshCandidate->GetVectorVertexCandidate();

			// get the spring vector
			std::vector<CSubmeshCandidate::Spring>& vectorSpring = pSubmeshCandidate->GetVectorSpring();

			// reserve memory for all the submesh data
			if(!pCoreSubmesh->reserve(vectorVertexCandidate.size(), pSubmeshCandidate->GetMapCount(), vectorFace.size(), vectorSpring.size()))
			{
				SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
				delete pCoreSubmesh;
				coreMesh.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			unsigned int vertexCandidateId;
			for(vertexCandidateId = 0; vertexCandidateId < vectorVertexCandidate.size(); vertexCandidateId++)
			{
				// get the vertex candidate
				CVertexCandidate *pVertexCandidate;
				pVertexCandidate = vectorVertexCandidate[vertexCandidateId];

				CalCoreSubmesh::Vertex vertex;

				// set the vertex position
				CalVector position;
				pVertexCandidate->GetPosition(vertex.position);

				// set the vertex normal
				CalVector normal;
				pVertexCandidate->GetNormal(vertex.normal);

				// set the collapse id
				vertex.collapseId = pVertexCandidate->GetCollapseId();

				// set the face collapse count
				vertex.faceCollapseCount = pVertexCandidate->GetFaceCollapseCount();

				// get the texture coordinate vector
				std::vector<CVertexCandidate::TextureCoordinate>& vectorTextureCoordinate = pVertexCandidate->GetVectorTextureCoordinate();

				// set all texture coordinates
				unsigned int textureCoordinateId;
				for(textureCoordinateId = 0; textureCoordinateId < vectorTextureCoordinate.size(); textureCoordinateId++)
				{
					CalCoreSubmesh::TextureCoordinate textureCoordinate;
					textureCoordinate.u = vectorTextureCoordinate[textureCoordinateId].u;
					textureCoordinate.v = vectorTextureCoordinate[textureCoordinateId].v;

					// set texture coordinate
					pCoreSubmesh->setTextureCoordinate(pVertexCandidate->GetLodId(), textureCoordinateId, textureCoordinate);
				}

				// get the influence vector
				std::vector<CVertexCandidate::Influence>& vectorInfluence = pVertexCandidate->GetVectorInfluence();

				// reserve memory for the influences in the vertex
				vertex.vectorInfluence.reserve(vectorInfluence.size());
				vertex.vectorInfluence.resize(vectorInfluence.size());

				// set all influences
				unsigned int influenceId;
				for(influenceId = 0; influenceId < vectorInfluence.size(); influenceId++)
				{
					vertex.vectorInfluence[influenceId].boneId = vectorInfluence[influenceId].boneId;
					vertex.vectorInfluence[influenceId].weight = vectorInfluence[influenceId].weight;
				}

				// set vertex in the core submesh instance
				pCoreSubmesh->setVertex(pVertexCandidate->GetLodId(), vertex);

				// set the physical property if there is a spring system
				if(vectorSpring.size() > 0)
				{
					// get the physical property vector
					CVertexCandidate::PhysicalProperty physicalPropertyCandidate;
					pVertexCandidate->GetPhysicalProperty(physicalPropertyCandidate);

					CalCoreSubmesh::PhysicalProperty physicalProperty;
					physicalProperty.weight = physicalPropertyCandidate.weight;

					// set the physical property in the core submesh instance
					pCoreSubmesh->setPhysicalProperty(vertexCandidateId, physicalProperty);

				}
			}

			unsigned int faceId;
			for(faceId = 0; faceId < vectorFace.size(); faceId++)
			{
				CalCoreSubmesh::Face face;

				// set the vertex ids
				face.vertexId[0] = vectorFace[faceId].vertexLodId[0];
				face.vertexId[1] = vectorFace[faceId].vertexLodId[1];
				face.vertexId[2] = vectorFace[faceId].vertexLodId[2];

				// set face in the core submesh instance
				pCoreSubmesh->setFace(vectorFace[faceId].lodId, face);
			}

			unsigned int springId;
			for(springId = 0; springId < vectorSpring.size(); springId++)
			{
				CalCoreSubmesh::Spring spring;

				// set the vertex ids
				spring.vertexId[0] = vectorSpring[springId].vertexId[0];
				spring.vertexId[1] = vectorSpring[springId].vertexId[1];
				spring.springCoefficient = vectorSpring[springId].springCoefficient;
				spring.idleLength = vectorSpring[springId].idleLength;

				// set face in the core submesh instance
				pCoreSubmesh->setSpring(springId, spring);
			}

			// set the LOD step count
			pCoreSubmesh->setLodCount(pSubmeshCandidate->GetLodCount());

			// add the core submesh to the core mesh instance
			coreMesh.addCoreSubmesh(pCoreSubmesh);
		}
	}

	// stop the progress info
	m_pInterface->StopProgressInfo();

	// save core mesh to the file
	CalSaver saver;
	if(!saver.saveCoreMesh(strFilename.c_str(), &coreMesh))
	{
		SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
		coreMesh.destroy();
		return false;
	}

	// destroy the core mesh
	coreMesh.destroy();

	return true;
}

//----------------------------------------------------------------------------//
// Export the skeleton to a given file                                        //
//----------------------------------------------------------------------------//

bool CExporter::ExportSkeleton(const std::string& strFilename)
{
	// check if a valid interface is set
	if(m_pInterface == 0)
	{
		SetLastError("Invalid handle.", __FILE__, __LINE__);
		return false;
	}

	// build a skeleton candidate
	CSkeletonCandidate skeletonCandidate;
	if(!skeletonCandidate.CreateFromInterface()) return false;

	// show export wizard sheet
	CSkeletonExportSheet sheet("Cal3D Skeleton Export", m_pInterface->GetMainWnd());
	sheet.SetSkeletonCandidate(&skeletonCandidate);
	sheet.SetWizardMode();
	if(sheet.DoModal() != ID_WIZFINISH) return true;

	// build the selected ids of the bone candidates
	int selectedCount;
	selectedCount = skeletonCandidate.BuildSelectedId();
	if(selectedCount == 0)
	{
		SetLastError("No bones selected to export.", __FILE__, __LINE__);
		return false;
	}

	// create the core skeleton instance
	CalCoreSkeleton coreSkeleton;
	if(!coreSkeleton.create())
	{
		SetLastError("Creation of core skeleton instance failed.", __FILE__, __LINE__);
		return false;
	}

	// get bone candidate vector
	std::vector<CBoneCandidate *>& vectorBoneCandidate = skeletonCandidate.GetVectorBoneCandidate();

	// start the progress info
	m_pInterface->StartProgressInfo("Exporting to skeleton file...");

	int boneCandidateId;
	unsigned int selectedId;
	for(boneCandidateId = 0, selectedId = 0; boneCandidateId < vectorBoneCandidate.size(); boneCandidateId++)
	{
		// get the bone candidate
		CBoneCandidate *pBoneCandidate;
		pBoneCandidate = vectorBoneCandidate[boneCandidateId];

		// only export selected bone candidates
		if(pBoneCandidate->IsSelected())
		{
			// update the progress info
			m_pInterface->SetProgressInfo(100.0f * (selectedId + 1) / selectedCount);
			selectedId++;

			// allocate new core bone instance
			CalCoreBone *pCoreBone;
			pCoreBone = new CalCoreBone();
			if(pCoreBone == 0)
			{
				SetLastError("Memory allocation failed.", __FILE__, __LINE__);
				coreSkeleton.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			// create the core bone instance
			if(!pCoreBone->create(pBoneCandidate->GetNode()->GetName().c_str()))
			{
				SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
				delete pCoreBone;
				coreSkeleton.destroy();
				m_pInterface->StopProgressInfo();
				return false;
			}

			// get the parent id of the bone candidate
			int parentId;
			parentId = skeletonCandidate.GetParentSelectedId(boneCandidateId);

			// set the parentId
			pCoreBone->setParentId(parentId);

			// get the translation and the rotation of the bone candidate
			CalVector translation;
			CalQuaternion rotation;
			skeletonCandidate.GetTranslationAndRotation(boneCandidateId, -1.0f, translation, rotation);

			// set the translation and rotation
			pCoreBone->setTranslation(translation);
			pCoreBone->setRotation(rotation);

			// get the bone space translation and the rotation of the bone candidate
			CalVector translationBoneSpace;
			CalQuaternion rotationBoneSpace;
			skeletonCandidate.GetTranslationAndRotationBoneSpace(boneCandidateId, -1.0f, translationBoneSpace, rotationBoneSpace);

			// set the bone space translation and rotation
			pCoreBone->setTranslationBoneSpace(translationBoneSpace);
			pCoreBone->setRotationBoneSpace(rotationBoneSpace);

			// set the core skeleton of the core bone instance
			pCoreBone->setCoreSkeleton(&coreSkeleton);

			// add the core bone to the core skeleton instance
			int boneId;
			boneId = coreSkeleton.addCoreBone(pCoreBone);

			// adjust child list of parent bone
			if(parentId != -1)
			{
				// get parent core bone
				CalCoreBone *pParentCoreBone;
				pParentCoreBone = coreSkeleton.getCoreBone(parentId);
				if(pParentCoreBone == 0)
				{
					SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
					delete pCoreBone;
					coreSkeleton.destroy();
					m_pInterface->StopProgressInfo();
					return false;
				}

				// add this core bone to the child list of the parent bone
				pParentCoreBone->addChildId(boneId);
			}
		}
	}

	// stop the progress info
	m_pInterface->StopProgressInfo();

	// save core skeleton to the file
	CalSaver saver;
	if(!saver.saveCoreSkeleton(strFilename.c_str(), &coreSkeleton))
	{
		SetLastError(CalError::getLastErrorText(), __FILE__, __LINE__);
		coreSkeleton.destroy();
		return false;
	}

	// destroy core skeleton instance
	coreSkeleton.destroy();

	return true;
}

//----------------------------------------------------------------------------//
// Get the last error message                                                 //
//----------------------------------------------------------------------------//

CBaseInterface *CExporter::GetInterface()
{
	return m_pInterface;
}

//----------------------------------------------------------------------------//
// Get the last error message                                                 //
//----------------------------------------------------------------------------//

const std::string& CExporter::GetLastError()
{
	return m_strLastError;
}

//----------------------------------------------------------------------------//
// Set the last error message                                                 //
//----------------------------------------------------------------------------//

void CExporter::SetLastError(const std::string& strText, const std::string& strFilename, int line)
{
	std::stringstream strstrError;
	strstrError << strText << "\n(" << strFilename << " " << line << ")" << std::ends;

	m_strLastError = strstrError.str();
}

//----------------------------------------------------------------------------//
// Set the last error message from the cal3d library                          //
//----------------------------------------------------------------------------//

void CExporter::SetLastErrorFromCal(const std::string& strFilename, int line)
{
	std::stringstream strstrError;
	strstrError << "cal3d : " << CalError::getLastErrorDescription();

	if(CalError::getLastErrorText() != NULL)
	{
		strstrError << " '" << CalError::getLastErrorText() << "'";
	}

	strstrError << " in " << CalError::getLastErrorFile()
				<< "(" << CalError::getLastErrorLine() << ")";

	strstrError << "\n(" << strFilename << " " << line << ")" << std::ends;

	m_strLastError = strstrError.str();
}

//----------------------------------------------------------------------------//
