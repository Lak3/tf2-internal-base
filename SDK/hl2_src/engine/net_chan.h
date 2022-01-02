//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: net_chan.h
//
//=============================================================================//
#ifndef NET_CHAN_H
#define NET_CHAN_H
#ifdef _WIN32
#pragma once
#endif

#include "net.h"
#include "../public/tier1/bitbuf.h"
#include "../public/inetmessage.h"
#include "../public/tier1/utlvector.h"
#include "../public/tier1/utlbuffer.h"
#include "../public/const.h"
#include "../public/inetchannel.h"

// How fast to converge flow estimates
#define FLOW_AVG ( 3.0 / 4.0 )
 // Don't compute more often than this
#define FLOW_INTERVAL 0.25

#define NET_FRAMES_BACKUP	64		// must be power of 2
#define NET_FRAMES_MASK		(NET_FRAMES_BACKUP-1)
#define MAX_SUBCHANNELS		8		// we have 8 alternative send&wait bits

#define SUBCHANNEL_FREE		0	// subchannel is free to use
#define SUBCHANNEL_TOSEND	1	// subchannel has data, but not send yet
#define SUBCHANNEL_WAITING	2   // sbuchannel sent data, waiting for ACK
#define SUBCHANNEL_DIRTY	3	// subchannel is marked as dirty during changelevel

class CNetChan : public INetChannel
{
public:

	typedef struct dataFragments_s
	{
		int	file;			// open file handle
		char			filename[260]; // filename
		char* buffer;			// if NULL it's a file
		unsigned int	bytes;			// size in bytes
		unsigned int	bits;			// size in bits
		unsigned int	transferID;		// only for files
		bool			isCompressed;	// true if data is bzip compressed
		unsigned int	nUncompressedSize; // full size in bytes
		bool			asTCP;			// send as TCP stream
		int				numFragments;	// number of total fragments
		int				ackedFragments; // number of fragments send & acknowledged
		int				pendingFragments; // number of fragments send, but not acknowledged yet
	} dataFragments_t;

	struct subChannel_s
	{
		int				startFraggment[MAX_STREAMS];
		int				numFragments[MAX_STREAMS];
		int				sendSeqNr;
		int				state; // 0 = free, 1 = scheduled to send, 2 = send & waiting, 3 = dirty
		int				index; // index in m_SubChannels[]

		void Free()
		{
			state = SUBCHANNEL_FREE;
			sendSeqNr = -1;
			for (int i = 0; i < MAX_STREAMS; i++)
			{
				numFragments[i] = 0;
				startFraggment[i] = -1;
			}
		}
	};

	// Client's now store the command they sent to the server and the entire results set of
	//  that command. 
	typedef struct netframe_s
	{
		// Data received from server
		float			time;			// net_time received/send
		int				size;			// total size in bytes
		float			latency;		// raw ping for this packet, not cleaned. set when acknowledged otherwise -1.
		float			avg_latency;	// averaged ping for this packet
		bool			valid;			// false if dropped, lost, flushed
		int				choked;			// number of previously chocked packets
		int				dropped;
		float			m_flInterpolationAmount;
		unsigned short	msggroups[INetChannelInfo::TOTAL];	// received bytes for each message group
	} netframe_t;

	typedef struct
	{
		float		nextcompute;	// Time when we should recompute k/sec data
		float		avgbytespersec;	// average bytes/sec
		float		avgpacketspersec;// average packets/sec
		float		avgloss;		// average packet loss [0..1]
		float		avgchoke;		// average packet choke [0..1]
		float		avglatency;		// average ping, not cleaned
		float		latency;		// current ping, more accurate also more jittering
		int			totalpackets;	// total processed packets
		int			totalbytes;		// total processed bytes
		int			currentindex;		// current frame index
		netframe_t	frames[NET_FRAMES_BACKUP]; // frame history
		netframe_t* currentframe;	// current frame
	} netflow_t;

public:
	bool		m_bProcessingMessages;
	bool		m_bClearedDuringProcessing;
	bool		m_bShouldDelete;

	// last send outgoing sequence number
	int			m_nOutSequenceNr;
	// last received incoming sequnec number
	int			m_nInSequenceNr;
	// last received acknowledge outgoing sequnce number
	int			m_nOutSequenceNrAck;

	// state of outgoing reliable data (0/1) flip flop used for loss detection
	int			m_nOutReliableState;
	// state of incoming reliable data
	int			m_nInReliableState;

	int			m_nChokedPackets;	//number of choked packets


	// Reliable data buffer, send which each packet (or put in waiting list)
	bf_write	m_StreamReliable;
	CUtlMemory<byte> m_ReliableDataBuffer;

	// unreliable message buffer, cleared which each packet
	bf_write	m_StreamUnreliable;
	CUtlMemory<byte> m_UnreliableDataBuffer;

	bf_write	m_StreamVoice;
	CUtlMemory<byte> m_VoiceDataBuffer;

	// don't use any vars below this (only in net_ws.cpp)

	int			m_Socket;   // NS_SERVER or NS_CLIENT index, depending on channel.
	int			m_StreamSocket;	// TCP socket handle

	unsigned int m_MaxReliablePayloadSize;	// max size of reliable payload in a single packet	
};

#endif // NET_CHAN_H