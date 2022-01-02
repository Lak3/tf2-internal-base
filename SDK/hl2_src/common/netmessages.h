//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#ifndef NETMESSAGES_H
#define NETMESSAGES_H
#ifdef _WIN32
#pragma once
#pragma warning(disable : 4100)	// unreferenced formal parameter
#endif

#include "qlimits.h"
#include "protocol.h"
#include "../engine/net.h"
#include "../public/inetmessage.h"
#include "../public/inetchannelinfo.h"
#include "../public/inetmsghandler.h"

#if !defined( _X360 )
#include "xbox/xboxstubs.h"
#endif

class SendTable;
class KeyValue;
class KeyValues;
class INetMessageHandler;
class IServerMessageHandler;

#define DECLARE_BASE_MESSAGE( msgtype )						\
	public:													\
		bool			ReadFromBuffer( bf_read &buffer );	\
		bool			WriteToBuffer( bf_write &buffer );	\
		const char		*ToString() const;					\
		int				GetType() const { return msgtype; } \
		const char		*GetName() const { return #msgtype;}\

#define DECLARE_NET_MESSAGE( name )			\
	DECLARE_BASE_MESSAGE( net_##name );		\
	INetMessageHandler *m_pMessageHandler;	\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

#define DECLARE_SVC_MESSAGE( name )		\
	DECLARE_BASE_MESSAGE( svc_##name );	\
	IServerMessageHandler *m_pMessageHandler;\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

#define DECLARE_CLC_MESSAGE( name )		\
	DECLARE_BASE_MESSAGE( clc_##name );	\
	IClientMessageHandler *m_pMessageHandler;\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

#define DECLARE_MM_MESSAGE( name )		\
	DECLARE_BASE_MESSAGE( mm_##name );	\
	IMatchmakingMessageHandler *m_pMessageHandler;\
	bool Process() { return m_pMessageHandler->Process##name( this ); }\

//TODO: this is gay
class CNetMessageRatelimitPolicyBase
{
public:
	//no virtual destructor here
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue) = 0;
};

class CNetMessageRatelimitPolicyNone : public CNetMessageRatelimitPolicyBase
{
public:
	CNetMessageRatelimitPolicyNone()
	{
		m_iUnk1 = 0;
		m_iUnk2 = 0;
		m_dUnk0 = 0.0;
		m_iUnk3 = 0;
		m_iUnk4 = 0;
		m_dUnk1 = 0.0;
		m_iUnk5 = 0;
		m_iUnk6 = 0;
	}

	//no virtual destructor here
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue);

public:
	int m_iUnk1; //4
	int m_iUnk2; //8
	double m_dUnk0; //12
	int m_iUnk3; //20
	int m_iUnk4; //24
	double m_dUnk1; //28
	int m_iUnk5; //36
	int m_iUnk6; //40
};

//idk why but this inherits from base, not from none
class CNetMessageRatelimitPolicy : public CNetMessageRatelimitPolicyBase
{
public:
	CNetMessageRatelimitPolicy()
	{
		m_iUnk1 = 0;
		m_iUnk2 = 0;
		m_dUnk0 = 0.0;
		m_iUnk3 = 0;
		m_iUnk4 = 0;
		m_dUnk1 = 0.0;
		m_iUnk5 = 0;
		m_iUnk6 = 0;
	}

	//no virtual destructor here
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue);

public:
	int m_iUnk1; //4
	int m_iUnk2; //8
	double m_dUnk0; //12
	int m_iUnk3; //20
	int m_iUnk4; //24
	double m_dUnk1; //28
	int m_iUnk5; //36
	int m_iUnk6; //40

	//additional here
	double m_dUnk2; //44 (set to 0.001 by default)
	int m_iUnk7; //52
	int m_iUnk8; //56
};

class CNetMessage : public INetMessage
{
public:
	CNetMessage() {
		m_bReliable = true;
		m_NetChannel = NULL;
		m_pRateLimitPolicy = NULL;
	}

	virtual ~CNetMessage() { if (m_pRateLimitPolicy) delete m_pRateLimitPolicy; };

	virtual int		GetGroup() const { return INetChannelInfo::GENERIC; }
	INetChannel* GetNetChannel() const { return m_NetChannel; }

	virtual bool ReadFromBuffer(bf_read& buffer) { return true; }
	virtual bool WriteToBuffer(bf_write& buffer) { return true; }

	virtual void	SetReliable(bool state) { m_bReliable = state; };
	virtual bool	IsReliable() const { return m_bReliable; };
	virtual void    SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
	virtual bool	Process() { assert(0); return false; };	// no handler set

public: //additional
	virtual bool BIncomingMessageForProcessing(double dValue, int iValue);
	virtual void SetRatePolicy();

protected:
	bool				            m_bReliable;	// true if message should be send reliable
	INetChannel*                    m_NetChannel;	// netchannel this message is from/for
	CNetMessageRatelimitPolicyBase* m_pRateLimitPolicy;
};

class CLC_Move : public CNetMessage
{
	DECLARE_CLC_MESSAGE(Move);

	int	GetGroup() const { return INetChannelInfo::MOVE; }

	CLC_Move() { m_bReliable = false; }

public:
	int				m_nBackupCommands;
	int				m_nNewCommands;
	int				m_nLength;
	bf_read			m_DataIn;
	bf_write		m_DataOut;
};

#endif //NETMESSAGES_H