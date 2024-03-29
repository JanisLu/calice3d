//----------------------------------------------------------------------------//
// BaseMesh.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef BASE_MESH_H
#define BASE_MESH_H

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CSkeletonCandidate;
class CVertexCandidate;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CBaseMesh
{
// member variables
protected:

// constructors/destructor
protected:
	CBaseMesh();
public:
	virtual ~CBaseMesh();

// member functions
public:
	virtual int GetFaceCount() = 0;
	virtual int GetMaterialCount() = 0;
	virtual int GetFaceMaterialId(unsigned int faceId) = 0;
	virtual int GetSubmeshMapCount(unsigned int submeshId) = 0;
	virtual int GetSubmeshMaterialThreadId(unsigned int submeshId) = 0;
	virtual CVertexCandidate *GetVertexCandidate(CSkeletonCandidate *pSkeletonCandidate, int faceId, int faceVertexId) = 0;
};

#endif

//----------------------------------------------------------------------------//
