//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Additional shared object cache functionality for the GC
//
//=============================================================================
#ifndef GCCLIENT_SHAREDOBJECTCACHE_H
#define GCCLIENT_SHAREDOBJECTCACHE_H
#ifdef _WIN32
#pragma once
#endif

#include "sharedobjectcache.h"

class CSteamID;
class CMsgSOCacheSubscribed;
class CMsgSOCacheSubscribed_SubscribedType;

namespace GCSDK
{
	/// Enumerate different events that might trigger a callback to an ISharedObjectListener
	enum ESOCacheEvent
	{
		/// Dummy sentinel value
		eSOCacheEvent_None = 0,

		/// We received a our first update from the GC and are subscribed
		eSOCacheEvent_Subscribed = 1,

		/// We lost connection to GC or GC notified us that we are no longer subscribed.
		/// Objects stay in the cache, but we no longer receive updates
		eSOCacheEvent_Unsubscribed = 2,

		/// We received a full update from the GC on a cache for which we were already subscribed.
		/// This can happen if connectivity is lost, and then restored before we realized it was lost.
		eSOCacheEvent_Resubscribed = 3,

		/// We received an incremental update from the GC about specific object(s) being
		/// added, updated, or removed from the cache
		eSOCacheEvent_Incremental = 4,

		/// A lister was added to the cache
		/// @see CGCClientSharedObjectCache::AddListener
		eSOCacheEvent_ListenerAdded = 5,

		/// A lister was removed from the cache
		/// @see CGCClientSharedObjectCache::RemoveListener
		eSOCacheEvent_ListenerRemoved = 6,
	};

	//----------------------------------------------------------------------------
	// Purpose: Allow game components to register themselves to hear about inventory
	//			changes when they are received from the server
	//----------------------------------------------------------------------------
	class ISharedObjectListener
	{
	public:

		/// Called when a new object is created in a cache we are currently subscribed to, or when we are added
		/// as a listener to a cache which already has objects in it
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Subscribed
		/// - eSOCacheEvent_Resubscribed
		/// - eSOCacheEvent_Incremental
		/// - eSOCacheEvent_ListenerAdded
		virtual void SOCreated(const CSteamID& steamIDOwner, const CSharedObject* pObject, ESOCacheEvent eEvent) = 0;

		/// Called when we are about to update objects in our cache but haven't done any of the work yet.
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Resubscribed
		/// - eSOCacheEvent_Incremental
		virtual void PreSOUpdate(const CSteamID& steamIDOwner, ESOCacheEvent eEvent) = 0;

		/// Called when an object is updated in a cache we are currently subscribed to.
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Resubscribed
		/// - eSOCacheEvent_Incremental
		virtual void SOUpdated(const CSteamID& steamIDOwner, const CSharedObject* pObject, ESOCacheEvent eEvent) = 0;

		/// Called when we're finished updating objects in our cache for this batch.
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Resubscribed
		/// - eSOCacheEvent_Incremental
		virtual void PostSOUpdate(const CSteamID& steamIDOwner, ESOCacheEvent eEvent) = 0;

		/// Called when an object is about to be deleted in a cache we are currently subscribed to.
		/// The object will have already been removed from the cache, but is still valid.
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Incremental
		/// - eSOCacheEvent_Resubscribed
		virtual void SODestroyed(const CSteamID& steamIDOwner, const CSharedObject* pObject, ESOCacheEvent eEvent) = 0;

		/// Called to notify a listener that he is subscribed to the cache.
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Subscribed
		/// - eSOCacheEvent_Resubscribed
		/// - eSOCacheEvent_ListenerAdded
		///
		/// A listener is guaranteed that it will not receive incremental updates (SOCreated,
		/// SOUpdated, SODestroyed) while not subscribed.  (Before the SOCacheSubscribed or
		/// after SOCacheUnsubscribed.)  However, note that it may be possible to receive
		/// an SOCacheSubscribed message while already subscribed.  This can happen if the
		/// GC loses and restores connection, or otherwise decides that a full update is
		/// necessary.
		virtual void SOCacheSubscribed(const CSteamID& steamIDOwner, ESOCacheEvent eEvent) = 0;

		/// Called to notify a listener that he is no longer subscribed to the cache.
		/// if he is being removed as a listener, then he will no longer receive
		/// updates.  Otherwise, he might receive another SOCacheSubscribed
		/// message, followed by further update notifications.
		///
		/// eEvent will be one of:
		/// - eSOCacheEvent_Unsubscribed
		/// - eSOCacheEvent_ListenerRemoved
		virtual void SOCacheUnsubscribed(const CSteamID& steamIDOwner, ESOCacheEvent eEvent) = 0;
	};
} // namespace GCSDK

#endif //GCCLIENT_SHAREDOBJECTCACHE_H