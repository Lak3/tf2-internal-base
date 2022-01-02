//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef VIEW_SCENE_H
#define VIEW_SCENE_H
#ifdef _WIN32
#pragma once
#endif

#include "iviewrender.h"
#include "../../public/materialsystem/itexture.h"

// Transform into view space (translate and rotate the camera into the origin).
//void ViewTransform(const Vector& worldSpace, Vector& viewSpace);

// Transform a world point into normalized screen space (X and Y from -1 to 1).
// Returns 0 if the point is behind the viewer.
int ScreenTransform(const Vector& point, Vector& screen);
int HudTransform(const Vector& point, Vector& screen);

extern int g_viewscene_refractUpdateFrame;
extern bool g_bAllowMultipleRefractUpdatesPerScenePerFrame;

#endif // VIEW_SCENE_H