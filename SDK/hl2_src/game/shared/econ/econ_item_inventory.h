//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Container that allows client & server access to data in player inventories & loadouts
//
//=============================================================================
#ifndef ITEM_INVENTORY_H
#define ITEM_INVENTORY_H
#ifdef _WIN32
#pragma once
#endif

#include "../../../public/gcsdk/gcclient_sharedobjectcache.h"

class CPlayerInventory;
class CEconItem;
struct baseitemcriteria_t;
class CEconItemViewHandle;
class ITexture;

// A class that wants notifications when an inventory is updated
class IInventoryUpdateListener : public GCSDK::ISharedObjectListener
{
public:
	virtual void InventoryUpdated(CPlayerInventory* pInventory) = 0;
	virtual void SOCreated(const CSteamID& steamIDOwner, const GCSDK::CSharedObject* pObject, GCSDK::ESOCacheEvent eEvent) = 0;
	virtual void PreSOUpdate(const CSteamID& steamIDOwner, GCSDK::ESOCacheEvent eEvent) = 0;
	virtual void SOUpdated(const CSteamID& steamIDOwner, const GCSDK::CSharedObject* pObject, GCSDK::ESOCacheEvent eEvent) = 0;
	virtual void PostSOUpdate(const CSteamID& steamIDOwner, GCSDK::ESOCacheEvent eEvent) = 0;
	virtual void SODestroyed(const CSteamID& steamIDOwner, const GCSDK::CSharedObject* pObject, GCSDK::ESOCacheEvent eEvent) = 0;
	virtual void SOCacheSubscribed(const CSteamID& steamIDOwner, GCSDK::ESOCacheEvent eEvent) = 0;
	virtual void SOCacheUnsubscribed(const CSteamID& steamIDOwner, GCSDK::ESOCacheEvent eEvent) = 0;
};

#endif // ITEM_INVENTORY_H