//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#include "../public/tier2/camerautils.h"
#include "../public/tier0/dbg.h"
#include "../public/mathlib/vmatrix.h"
#include "../public/mathlib/camera.h"

//-----------------------------------------------------------------------------
// accessors for generated matrices
//-----------------------------------------------------------------------------
void ComputeViewMatrix(matrix3x4_t* pWorldToCamera, const Camera_t& camera)
{
	matrix3x4_t transform;
	AngleMatrix(camera.m_angles, camera.m_origin, transform);

	VMatrix matRotate(transform);
	VMatrix matRotateZ;
	MatrixBuildRotationAboutAxis(matRotateZ, Vector(0, 0, 1), -90);
	MatrixMultiply(matRotate, matRotateZ, matRotate);

	VMatrix matRotateX;
	MatrixBuildRotationAboutAxis(matRotateX, Vector(1, 0, 0), 90);
	MatrixMultiply(matRotate, matRotateX, matRotate);
	transform = matRotate.As3x4();

	MatrixInvert(transform, *pWorldToCamera);
}

void ComputeViewMatrix(VMatrix* pWorldToCamera, const Camera_t& camera)
{
	matrix3x4_t transform, invTransform;
	AngleMatrix(camera.m_angles, camera.m_origin, transform);

	VMatrix matRotate(transform);
	VMatrix matRotateZ;
	MatrixBuildRotationAboutAxis(matRotateZ, Vector(0, 0, 1), -90);
	MatrixMultiply(matRotate, matRotateZ, matRotate);

	VMatrix matRotateX;
	MatrixBuildRotationAboutAxis(matRotateX, Vector(1, 0, 0), 90);
	MatrixMultiply(matRotate, matRotateX, matRotate);
	transform = matRotate.As3x4();

	MatrixInvert(transform, invTransform);
	*pWorldToCamera = invTransform;
}

//-----------------------------------------------------------------------------
// Computes the screen space position given a screen size
//-----------------------------------------------------------------------------
void ComputeScreenSpacePosition(Vector2D* pScreenPosition, const Vector& vecWorldPosition,
	const Camera_t& camera, int width, int height)
{
	VMatrix view, proj, viewproj;
	ComputeViewMatrix(&view, camera);
	ComputeProjectionMatrix(&proj, camera, width, height);
	MatrixMultiply(proj, view, viewproj);

	Vector vecScreenPos;
	Vector3DMultiplyPositionProjective(viewproj, vecWorldPosition, vecScreenPos);

	pScreenPosition->x = (vecScreenPos.x + 1.0f) * width / 2.0f;
	pScreenPosition->y = (-vecScreenPos.y + 1.0f) * height / 2.0f;
}