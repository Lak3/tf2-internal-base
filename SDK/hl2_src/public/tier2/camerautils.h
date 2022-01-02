//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: A set of utilities to deal with camera transforms
//
//===========================================================================//
#ifndef CAMERAUTILS_H
#define CAMERAUTILS_H
#ifdef _WIN32
#pragma once
#endif

#include "../mathlib/vector.h"
#include "../Color.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class Vector;
class QAngle;
class IMaterial;
struct matrix3x4_t;
struct Camera_t;
class VMatrix;

//-----------------------------------------------------------------------------
// accessors for generated matrices
//-----------------------------------------------------------------------------
void ComputeViewMatrix(VMatrix* pWorldToCamera, const Camera_t& camera);
void ComputeViewMatrix(matrix3x4_t* pWorldToCamera, const Camera_t& camera);

//-----------------------------------------------------------------------------
// Computes the screen space position given a screen size
//-----------------------------------------------------------------------------
void ComputeScreenSpacePosition(Vector2D* pScreenPosition, const Vector& vecWorldPosition,
	const Camera_t& camera, int width, int height);


#endif // CAMERAUTILS_H