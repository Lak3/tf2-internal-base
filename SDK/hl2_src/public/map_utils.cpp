//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

//from bspfile.h
#define	ANGLE_UP	-1
#define	ANGLE_DOWN	-2

#include "map_utils.h"

void SetupLightNormalFromProps(const QAngle& angles, float angle, float pitch, Vector& output)
{
	if (angle == ANGLE_UP)
	{
		output[0] = output[1] = 0.0f;
		output[2] = 1.0f;
	}
	else if (angle == ANGLE_DOWN)
	{
		output[0] = output[1] = 0.0f;
		output[2] = -1.0f;
	}
	else
	{
		// if we don't have a specific "angle" use the "angles" YAW
		if (!angle)
		{
			angle = angles[1];
		}

		output[2] = 0.0f;
		output[0] = cosf(angle / 180.0f * M_PI);
		output[1] = sinf(angle / 180.0f * M_PI);
	}

	if (!pitch)
	{
		// if we don't have a specific "pitch" use the "angles" PITCH
		pitch = angles[0];
	}

	output[2]  = sinf(pitch / 180.0f * M_PI);
	output[0] *= cosf(pitch / 180.0f * M_PI);
	output[1] *= cosf(pitch / 180.0f * M_PI);
}