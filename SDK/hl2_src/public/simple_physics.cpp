//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#include "simple_physics.h"
#include "tier0/dbg.h"

CSimplePhysics::CSimplePhysics()
{
	Init(1.0f / 30.0f); // default is 30 steps per second
}

void CSimplePhysics::Init(float flTimeStep)
{
	m_flPredictedTime = 0;
	m_iCurTimeStep = 0;
	m_flTimeStep = flTimeStep;
	m_flTimeStepMul = m_flTimeStep * m_flTimeStep * 0.5f;
}

void CSimplePhysics::Simulate(CSimplePhysics::CNode* pNodes, int nNodes, CSimplePhysics::IHelper* pHelper, float dt, float flDamp)
{
	// Figure out how many time steps to run.
	m_flPredictedTime += dt;

	const int newTimeStep = static_cast<int>(ceilf(m_flPredictedTime / m_flTimeStep));
	const int nTimeSteps = newTimeStep - m_iCurTimeStep;

	for (int iTimeStep = 0; iTimeStep < nTimeSteps; iTimeStep++)
	{
		// Simulate everything..
		for (int iNode = 0; iNode < nNodes; iNode++)
		{
			CSimplePhysics::CNode* pNode = &pNodes[iNode];

			// Apply forces.
			Vector vAccel;
			pHelper->GetNodeForces(pNodes, iNode, &vAccel);
			assert(vAccel.IsValid());

			Vector vPrevPos = pNode->m_vPos;
			pNode->m_vPos = pNode->m_vPos + (pNode->m_vPos - pNode->m_vPrevPos) * flDamp + vAccel * m_flTimeStepMul;
			pNode->m_vPrevPos = vPrevPos;
		}

		// Apply constraints.
		pHelper->ApplyConstraints(pNodes, nNodes);
	}

	m_iCurTimeStep = newTimeStep;

	// Setup predicted positions.
	const float flInterpolant = static_cast<float>((m_flPredictedTime - (GetCurTime() - static_cast<double>(m_flTimeStep))) / static_cast<double>(m_flTimeStep));

	for (int iNode = 0; iNode < nNodes; iNode++)
	{
		CSimplePhysics::CNode* pNode = &pNodes[iNode];
		VectorLerp(pNode->m_vPrevPos, pNode->m_vPos, flInterpolant, pNode->m_vPredicted);
	}
}