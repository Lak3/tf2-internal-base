//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Matchmaking stuff shared between GC and gameserver / client
//
//=============================================================================
#ifndef TF_MATCHMAKING_SHARED_H
#define TF_MATCHMAKING_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "../../../public/tier0/platform.h"

class IMatchGroupDescription;

#define NEXT_MAP_VOTE_OPTIONS 3

// Replace this hard-coded value concept in order to support all bracket types (i.e. anything higher than 6v6, etc)
// This increases a number of hard-coded data structures and so on, so beware
#define MATCH_SIZE_MAX 24

// Similarly, some hot path MM structures use this for fixed arrays. It should be the maximum number of players that
// will ever be in a party and no larger.
#define MAX_PARTY_SIZE 6

// Range clients are allowed to pass up for custom ping tolerance
// Currently matches CS:GO
#define CUSTOM_PING_TOLERANCE_MIN 25
#define CUSTOM_PING_TOLERANCE_MAX 350

// XXX(JohnS): Before we can actually use other rating backends for matchmaking or display purposes, there are remaining
//             hard coded assumptions about where the primary rating is, and issues with e.g. Match_Result assuming the
//             backend in use *now* is what the match was created for, etc.  I've sprinkled this around at all the
//             landmines I found while implementing the new rating backend, so... comment this out and fix everything
//             that breaks.
static inline void FixmeMMRatingBackendSwapping() { }

// Backend-agnostic storage type for rating data, so we're not manually passing pairs around to every rating-specific
// interface when we inevitably decide to add a third.
//
// Keep in mind that it is much easier to query well structured data for reports & otherwise, so we should prefer
// e.g. adding another value to packing two 16-bit ints in RatingSecondary for a new backend that needs more 16-bit
// values.  (The calculus may change if we end up with a backend that has eight 8-bit values, however)
//
// Schema objects that embed rating data: RatingHistory, RatingData
// Proto objects that embed rating data:  CSOTFRatingData
struct MMRatingData_t {
	uint32_t unRatingPrimary;
	uint32_t unRatingSecondary;
	uint32_t unRatingTertiary;

	inline bool operator==(const MMRatingData_t& b) const
	{
		return this->unRatingPrimary == b.unRatingPrimary &&
			this->unRatingSecondary == b.unRatingSecondary &&
			this->unRatingTertiary == b.unRatingTertiary;
	}
};

// Stored value, don't re-order
enum EMatchGroup
{
	k_nMatchGroup_Invalid = -1,
	k_nMatchGroup_First = 0,

	k_nMatchGroup_MvM_Practice = 0,
	k_nMatchGroup_MvM_MannUp,

	k_nMatchGroup_Ladder_6v6,
	k_nMatchGroup_Ladder_9v9,
	k_nMatchGroup_Ladder_12v12,

	k_nMatchGroup_Casual_6v6,
	k_nMatchGroup_Casual_9v9,
	k_nMatchGroup_Casual_12v12,

	k_nMatchGroup_Count,
	// When adding a new matchgroup, add case handling to GetMatchSizeForMatchGroup(), GetMatchGroupName(), GetServerPoolName(), GetMaxLobbySizeForMatchGroup(), YldWebAPIServersByDataCenter()
};

// Stored value, don't re-order
//
// If you add a new backend, see ITFMMRatingBackend::GetRatingBackend -- you need to at least provide a GetDefault()
enum EMMRating
{
	k_nMMRating_LowestValue = -1,
	k_nMMRating_Invalid = -1,

	k_nMMRating_First = 0,
	k_nMMRating_6v6_DRILLO = 0,
	k_nMMRating_6v6_DRILLO_PlayerAcknowledged = 1,
	k_nMMRating_6v6_GLICKO = 2,
	k_nMMRating_12v12_DRILLO = 3,
	k_nMMRating_12v12_GLICKO = 4,
	k_nMMRating_Last = 4,
};

// Stored value, don't re-order
enum EMMRatingSource
{
	k_nMMRatingSource_LowestValue = -1,
	k_nMMRatingSource_Invalid = -1,

	k_nMMRatingSource_Match = 0, // Match result. Source ID is match ID
	k_nMMRatingSource_Admin = 1, // Admin command/manual adjustment. Source ID is probably 0 or something.
	k_nMMRatingSource_PlayerAcknowledge = 2, // For 'acknowledge' type ratings, the player acknowledged the value
	k_nMMRatingSource_ImportedOldSystem = 3, // For pre-history ratings, this source is used once for 'initial rating from old system'
	k_nMMRatingSource_Last = 3,
};

// Probably a better place for this...
enum ELadderLeaderboardTypes
{
	LADDER_LEADERBOARDS_6V6 = 0,
	LADDER_LEADERBOARDS_PUBLIC,
	LADDER_LEADERBOARDS_9V9,
	LADDER_LEADERBOARDS_12V12,
	LADDER_LEADERBOARDS_MAX
};

// Late join modes
enum EMatchMode
{
	// Uninitialized/unknown
	eMatchMode_Invalid,
	// Not late join / don't use late join
	eMatchMode_MatchMaker_CompleteFromQueue,
	// The add-one-player-at-a-time mode that doesn't work with the new scoring system, but still used for MvM and other
	// old-scoring-system stuff.
	eMatchMode_MatchMaker_LateJoinDropIn,
	// The new late join mode that re-evaulates complete matches with the missing spot(s) filled.
	eMatchMode_MatchMaker_LateJoinMatchBased,
	// A match that is being manually crafted
	eMatchMode_Manual,
};

constexpr EMatchGroup k_nMatchGroup_Ladder_First = k_nMatchGroup_Ladder_6v6;
constexpr EMatchGroup k_nMatchGroup_Ladder_Last = k_nMatchGroup_Ladder_12v12;

constexpr EMatchGroup k_nMatchGroup_Casual_First = k_nMatchGroup_Casual_6v6;
constexpr EMatchGroup k_nMatchGroup_Casual_Last = k_nMatchGroup_Casual_12v12;

inline bool IsMvMMatchGroup(EMatchGroup eMatchGroup)
{
	return (eMatchGroup == k_nMatchGroup_MvM_Practice) || (eMatchGroup == k_nMatchGroup_MvM_MannUp);
}

inline bool IsLadderGroup(EMatchGroup eMatchGroup)
{
	return (eMatchGroup >= k_nMatchGroup_Ladder_First && eMatchGroup <= k_nMatchGroup_Ladder_Last)
		|| (eMatchGroup >= k_nMatchGroup_Casual_First && eMatchGroup <= k_nMatchGroup_Casual_Last);
}

inline bool IsCasualGroup(EMatchGroup eMatchGroup)
{
	return (eMatchGroup >= k_nMatchGroup_Casual_First) && (eMatchGroup <= k_nMatchGroup_Casual_Last);
}

inline bool IsMannUpGroup(EMatchGroup eMatchGroup)
{
	switch (eMatchGroup)
	{
	case k_nMatchGroup_MvM_Practice:
		return false;
	case k_nMatchGroup_MvM_MannUp:
		return true;
	case k_nMatchGroup_Ladder_6v6:
	case k_nMatchGroup_Ladder_9v9:
	case k_nMatchGroup_Ladder_12v12:
		return false;
	case k_nMatchGroup_Invalid:
	default:
		Error("IsMannUpGroup called with invalid match group");
		return false;
	}

	return false;
}

enum EMMServerMode
{
	eMMServerMode_Idle,
	eMMServerMode_Incomplete_Match,
	eMMServerMode_Full,
	eMMServerMode_Count
};

// Separate penalty pools (and rules) for different classes of modes
enum EMMPenaltyPool
{
	eMMPenaltyPool_Invalid,
	eMMPenaltyPool_Casual, // Pool with lenient penalties for most casual/mainstream gamemodes
	eMMPenaltyPool_Ranked  // Pool with strict and cumulative penalties for ranked gamemodes where abandons tank matches
};

enum
{
	// !! This should match up with GetServerPoolIndex to map these between match groups and server pools
	// eMMServerMode_NotParticipating
	k_nGameServerPool_NotParticipating = -1,

	// eMMServerMode_Incomplete_Match
	k_nGameServerPool_MvM_Practice_Incomplete_Match = 0,
	k_nGameServerPool_MvM_MannUp_Incomplete_Match,
	k_nGameServerPool_Ladder_6v6_Incomplete_Match,
	k_nGameServerPool_Ladder_9v9_Incomplete_Match,
	k_nGameServerPool_Ladder_12v12_Incomplete_Match,
	k_nGameServerPool_Casual_6v6_Incomplete_Match,
	k_nGameServerPool_Casual_9v9_Incomplete_Match,
	k_nGameServerPool_Casual_12v12_Incomplete_Match,

	// eMMServerMode_Full
	k_nGameServerPool_MvM_Practice_Full,
	k_nGameServerPool_MvM_MannUp_Full,
	k_nGameServerPool_Ladder_6v6_Full,
	k_nGameServerPool_Ladder_9v9_Full,
	k_nGameServerPool_Ladder_12v12_Full,
	k_nGameServerPool_Casual_6v6_Full,
	k_nGameServerPool_Casual_9v9_Full,
	k_nGameServerPool_Casual_12v12_Full,

	// eMMServerMode_Idle
	k_nGameServerPool_Idle,
	// When adding a new matchgroup, add case handling to GetMatchSizeForMatchGroup(), GetMatchGroupName(), GetServerPoolName(), GetMaxLobbySizeForMatchGroup(), YldWebAPIServersByDataCenter()

	k_nGameServerPoolCountTotal,
};

constexpr int k_nGameServerPool_Incomplete_Match_First = k_nGameServerPool_MvM_Practice_Incomplete_Match;
constexpr int k_nGameServerPool_Incomplete_Match_Last = k_nGameServerPool_Casual_12v12_Incomplete_Match;
constexpr int k_nGameServerPool_Full_First = k_nGameServerPool_MvM_Practice_Full;
constexpr int k_nGameServerPool_Full_Last = k_nGameServerPool_Casual_12v12_Full;

inline bool IsIncompleteMatchPool(int nGameServerPool)
{
	return nGameServerPool >= k_nGameServerPool_Incomplete_Match_First && nGameServerPool <= k_nGameServerPool_Incomplete_Match_Last;
}

// Stuff is simpler if we can set a max number of challenges in the schema
#define MAX_MVM_CHALLENGES 64

// Player Skill Ratings
constexpr int k_nDrilloRating_MinRatingAdjust = 1;
constexpr int k_nDrilloRating_MaxRatingAdjust = 100;
constexpr int k_nDrilloRating_Ladder_MaxRatingAdjust = 500;
constexpr int k_nDrilloRating_Ladder_MaxLossAdjust_LowRank = 100;
constexpr uint32 k_unDrilloRating_MaxDifference = 25000;
constexpr uint32 k_unDrilloRating_Min = 1;
constexpr uint32 k_unDrilloRating_Ladder_Min = 10000;
constexpr uint32 k_unDrilloRating_Max = 50000;
constexpr uint32 k_unDrilloRating_Avg = 20000;
constexpr uint32 k_unDrilloRating_Ladder_Start = 10000;
constexpr uint32 k_unDrilloRating_Ladder_LowSkill = 19500; // First 6 ranks ceiling
constexpr uint32 k_unDrilloRating_Ladder_HighSkill = 33001; // Last 6 ranks floor

// CSOTFLobby flags
#define LOBBY_FLAG_LOWPRIORITY	( 1 << 0 )
#define LOBBY_FLAG_REMATCH		( 1 << 1 )

// CMsgGC_Match_Result match flags
#define MATCH_FLAG_LOWPRIORITY ( 1 << 0 )
#define MATCH_FLAG_REMATCH ( 1 << 1 )

// CMsgGC_Match_Result player flags
#define MATCH_FLAG_PLAYER_LEAVER    ( 1 << 0 )
#define MATCH_FLAG_PLAYER_LATEJOIN  ( 1 << 1 )
// Separate from LEAVER - was marked as an abandon and issued a penalty.  You can be a leaver without being an
// abandoner.
#define MATCH_FLAG_PLAYER_ABANDONER ( 1 << 2 )
#define MATCH_FLAG_PLAYER_PLAYED	( 1 << 3 )	// Did they stay long enough for the game to start?

#endif // #ifndef TF_MATCHMAKING_SHARED_H