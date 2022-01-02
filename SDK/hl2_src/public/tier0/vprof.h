//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Real-Time Hierarchical Profiling
//
// $NoKeywords: $
//=============================================================================//
#ifndef VPROF_H
#define VPROF_H

#include "fasttimer.h"
#include "vprof_telemetry.h"

// VProf is enabled by default in all configurations -except- X360 Retail.
#if !( defined( _X360 ) && defined( _CERT ) )
//#define VPROF_ENABLED
#endif

#if defined(_X360) && defined(VPROF_ENABLED)
#include "tier0/pmc360.h"
#ifndef USE_PIX
#define VPROF_UNDO_PIX
#undef _PIX_H_
#undef PIXBeginNamedEvent
#undef PIXEndNamedEvent
#undef PIXSetMarker
#undef PIXNameThread
#define USE_PIX
#include <pix.h>
#undef USE_PIX
#else
#include <pix.h>
#endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

// enable this to get detailed nodes beneath budget
// #define VPROF_LEVEL 1

// enable this to use pix (360 only)
// #define VPROF_PIX 1

#if defined(VPROF_PIX)
#pragma comment( lib, "Xapilibi" )
#endif

//-----------------------------------------------------------------------------
//
// Profiling instrumentation macros
//

#define MAXCOUNTERS 256

#ifdef VPROF_ENABLED

#define VPROF_VTUNE_GROUP

#define	VPROF( name )						VPROF_(name, 1, VPROF_BUDGETGROUP_OTHER_UNACCOUNTED, false, 0)
#define	VPROF_ASSERT_ACCOUNTED( name )		VPROF_(name, 1, VPROF_BUDGETGROUP_OTHER_UNACCOUNTED, true, 0)
#define	VPROF_( name, detail, group, bAssertAccounted, budgetFlags )		VPROF_##detail(name,group, bAssertAccounted, budgetFlags)

#define VPROF_BUDGET( name, group )					VPROF_BUDGET_FLAGS(name, group, BUDGETFLAG_OTHER)
#define VPROF_BUDGET_FLAGS( name, group, flags )	VPROF_(name, 0, group, false, flags)

#define VPROF_SCOPE_BEGIN( tag )	do { VPROF( tag )
#define VPROF_SCOPE_END()			} while (0)

#define VPROF_ONLY( expression )	expression

#define VPROF_ENTER_SCOPE( name )			g_VProfCurrentProfile.EnterScope( name, 1, VPROF_BUDGETGROUP_OTHER_UNACCOUNTED, false, 0 )
#define VPROF_EXIT_SCOPE()					g_VProfCurrentProfile.ExitScope()

#define VPROF_BUDGET_GROUP_ID_UNACCOUNTED 0


// Budgetgroup flags. These are used with VPROF_BUDGET_FLAGS.
// These control which budget panels the groups show up in.
// If a budget group uses VPROF_BUDGET, it gets the default 
// which is BUDGETFLAG_OTHER.
#define BUDGETFLAG_CLIENT	(1<<0)		// Shows up in the client panel.
#define BUDGETFLAG_SERVER	(1<<1)		// Shows up in the server panel.
#define BUDGETFLAG_OTHER	(1<<2)		// Unclassified (the client shows these but the dedicated server doesn't).
#define BUDGETFLAG_HIDDEN	(1<<15)
#define BUDGETFLAG_ALL		0xFFFF


// NOTE: You can use strings instead of these defines. . they are defined here and added
// in vprof.cpp so that they are always in the same order.
#define VPROF_BUDGETGROUP_OTHER_UNACCOUNTED			_T("Unaccounted")
#define VPROF_BUDGETGROUP_WORLD_RENDERING			_T("World Rendering")
#define VPROF_BUDGETGROUP_DISPLACEMENT_RENDERING	_T("Displacement_Rendering")
#define VPROF_BUDGETGROUP_GAME						_T("Game")
#define VPROF_BUDGETGROUP_NPCS						_T("NPCs")
#define VPROF_BUDGETGROUP_SERVER_ANIM				_T("Server Animation")
#define VPROF_BUDGETGROUP_PHYSICS					_T("Physics")
#define VPROF_BUDGETGROUP_STATICPROP_RENDERING		_T("Static_Prop_Rendering")
#define VPROF_BUDGETGROUP_MODEL_RENDERING			_T("Other_Model_Rendering")
#define VPROF_BUDGETGROUP_MODEL_FAST_PATH_RENDERING _T("Fast Path Model Rendering")
#define VPROF_BUDGETGROUP_BRUSHMODEL_RENDERING		_T("Brush_Model_Rendering")
#define VPROF_BUDGETGROUP_SHADOW_RENDERING			_T("Shadow_Rendering")
#define VPROF_BUDGETGROUP_DETAILPROP_RENDERING		_T("Detail_Prop_Rendering")
#define VPROF_BUDGETGROUP_PARTICLE_RENDERING		_T("Particle/Effect_Rendering")
#define VPROF_BUDGETGROUP_ROPES						_T("Ropes")
#define VPROF_BUDGETGROUP_DLIGHT_RENDERING			_T("Dynamic_Light_Rendering")
#define VPROF_BUDGETGROUP_OTHER_NETWORKING			_T("Networking")
#define VPROF_BUDGETGROUP_CLIENT_ANIMATION			_T("Client_Animation")
#define VPROF_BUDGETGROUP_OTHER_SOUND				_T("Sound")
#define VPROF_BUDGETGROUP_OTHER_VGUI				_T("VGUI")
#define VPROF_BUDGETGROUP_OTHER_FILESYSTEM			_T("FileSystem")
#define VPROF_BUDGETGROUP_PREDICTION				_T("Prediction")
#define VPROF_BUDGETGROUP_INTERPOLATION				_T("Interpolation")
#define VPROF_BUDGETGROUP_SWAP_BUFFERS				_T("Swap_Buffers")
#define VPROF_BUDGETGROUP_PLAYER					_T("Player")
#define VPROF_BUDGETGROUP_OCCLUSION					_T("Occlusion")
#define VPROF_BUDGETGROUP_OVERLAYS					_T("Overlays")
#define VPROF_BUDGETGROUP_TOOLS						_T("Tools")
#define VPROF_BUDGETGROUP_LIGHTCACHE				_T("Light_Cache")
#define VPROF_BUDGETGROUP_DISP_HULLTRACES			_T("Displacement_Hull_Traces")
#define VPROF_BUDGETGROUP_TEXTURE_CACHE				_T("Texture_Cache")
#define VPROF_BUDGETGROUP_REPLAY					_T("Replay")
#define VPROF_BUDGETGROUP_PARTICLE_SIMULATION		_T("Particle Simulation")
#define VPROF_BUDGETGROUP_SHADOW_DEPTH_TEXTURING	_T("Flashlight Shadows")
#define VPROF_BUDGETGROUP_CLIENT_SIM				_T("Client Simulation") // think functions, tempents, etc.
#define VPROF_BUDGETGROUP_STEAM						_T("Steam") 
#define VPROF_BUDGETGROUP_CVAR_FIND					_T("Cvar_Find") 
#define VPROF_BUDGETGROUP_CLIENTLEAFSYSTEM			_T("ClientLeafSystem")
#define VPROF_BUDGETGROUP_JOBS_COROUTINES			_T("Jobs/Coroutines")
#define VPROF_BUDGETGROUP_SLEEPING					_T("Sleeping")
#define VPROF_BUDGETGROUP_THREADINGMAIN				_T("ThreadingMain")
#define VPROF_BUDGETGROUP_HTMLSURFACE				_T("HTMLSurface")
#define VPROF_BUDGETGROUP_VGUI						VPROF_BUDGETGROUP_HTMLSURFACE
#define VPROF_BUDGETGROUP_TENFOOT					VPROF_BUDGETGROUP_HTMLSURFACE
#define VPROF_BUDGETGROUP_STEAMUI					VPROF_BUDGETGROUP_HTMLSURFACE
#define VPROF_BUDGETGROUP_ATTRIBUTES				_T("Attributes")
#define VPROF_BUDGETGROUP_FINDATTRIBUTE				_T("FindAttribute")
#define VPROF_BUDGETGROUP_FINDATTRIBUTEUNSAFE		_T("FindAttributeUnsafe")

#ifdef _X360
// update flags
#define VPROF_UPDATE_BUDGET				0x01	// send budget data every frame
#define VPROF_UPDATE_TEXTURE_GLOBAL		0x02	// send global texture data every frame
#define VPROF_UPDATE_TEXTURE_PERFRAME	0x04	// send perframe texture data every frame
#endif

//-------------------------------------

#ifndef VPROF_LEVEL
#define VPROF_LEVEL 0
#endif

//these macros exist to create VProf_<line number> variables. This is important because it avoids /analyze warnings about variable aliasing when VPROF's are nested within each other, and allows
//for multiple VPROF's to exist within the same scope. Three macros must be used to force the __LINE__ to be resolved prior to the token concatenation, but just ignore the _INTERNAL macros and use
//the VPROF_VAR_NAME
#define VPROF_VAR_NAME_INTERNAL_CAT(a, b)	a##b
#define VPROF_VAR_NAME_INTERNAL( a, b )		VPROF_VAR_NAME_INTERNAL_CAT( a, b )
#define VPROF_VAR_NAME( a )					VPROF_VAR_NAME_INTERNAL( a, __LINE__ )

#define	VPROF_0(name,group,assertAccounted,budgetFlags)	tmZone( TELEMETRY_LEVEL2, TMZF_NONE, "(%s)%s", group, name ); CVProfScope VPROF_VAR_NAME( VProf_ )(name, 0, group, assertAccounted, budgetFlags);

#if VPROF_LEVEL > 0 
#define	VPROF_1(name,group,assertAccounted,budgetFlags)	tmZone( TELEMETRY_LEVEL3, TMZF_NONE, "(%s)%s", group, name ); CVProfScope VPROF_VAR_NAME( VProf_ )(name, 1, group, assertAccounted, budgetFlags);
#else
#define	VPROF_1(name,group,assertAccounted,budgetFlags)	((void)0)
#endif

#if VPROF_LEVEL > 1 
#define	VPROF_2(name,group,assertAccounted,budgetFlags)	CVProfScope VPROF_VAR_NAME( VProf_ )(name, 2, group, assertAccounted, budgetFlags);
#else
#define	VPROF_2(name,group,assertAccounted,budgetFlags)	((void)0)
#endif

#if VPROF_LEVEL > 2 
#define	VPROF_3(name,group,assertAccounted,budgetFlags)	CVProfScope VPROF_VAR_NAME( VProf_ )(name, 3, group, assertAccounted, budgetFlags);
#else
#define	VPROF_3(name,group,assertAccounted,budgetFlags)	((void)0)
#endif

#if VPROF_LEVEL > 3 
#define	VPROF_4(name,group,assertAccounted,budgetFlags)	CVProfScope VPROF_VAR_NAME( VProf_ )(name, 4, group, assertAccounted, budgetFlags);
#else
#define	VPROF_4(name,group,assertAccounted,budgetFlags)	((void)0)
#endif

//-------------------------------------

#ifdef _MSC_VER
#define VProfCode( code ) \
	if ( 0 ) \
		; \
	else \
	{ \
	VPROF( __FUNCTION__ ": " #code ); \
		code; \
	}
#else
#define VProfCode( code ) \
	if ( 0 ) \
		; \
	else \
	{ \
		VPROF( #code ); \
		code; \
	} 
#endif


//-------------------------------------

#define VPROF_INCREMENT_COUNTER(name,amount)			do { static CVProfCounter _counter( name ); _counter.Increment( amount ); } while( 0 )
#define VPROF_INCREMENT_GROUP_COUNTER(name,group,amount)			do { static CVProfCounter _counter( name, group ); _counter.Increment( amount ); } while( 0 )

#else

#define	VPROF( name )									((void)0)
#define	VPROF_ASSERT_ACCOUNTED( name )					((void)0)
#define	VPROF_( name, detail, group, bAssertAccounted, budgetFlags )	((void)0)
#define VPROF_BUDGET( name, group )						((void)0)
#define VPROF_BUDGET_FLAGS( name, group, flags )		((void)0)

#define VPROF_SCOPE_BEGIN( tag )	do {
#define VPROF_SCOPE_END()			} while (0)

#define VPROF_ONLY( expression )	((void)0)

#define VPROF_ENTER_SCOPE( name )
#define VPROF_EXIT_SCOPE()

#define VPROF_INCREMENT_COUNTER(name,amount)			((void)0)
#define VPROF_INCREMENT_GROUP_COUNTER(name,group,amount)	((void)0)

#define VPROF_TEST_SPIKE( msec )	((void)0)

#define VProfCode( code ) code

#endif

#endif