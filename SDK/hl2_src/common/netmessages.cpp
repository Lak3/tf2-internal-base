#pragma once

#include "netmessages.h"

#include "../public/inetchannel.h"
#include "../public/tier1/bitbuf.h"
#include "../public/tier1/strtools.h"

static char s_text[1024];

const char* CLC_Move::ToString(void) const
{
	Q_snprintf(s_text, sizeof(s_text), "%s: backup %i, new %i, bytes %i",
		GetName(), m_nNewCommands, m_nBackupCommands, Bits2Bytes(m_nLength));

	return s_text;
}

bool CLC_Move::WriteToBuffer(bf_write& buffer)
{
	buffer.WriteUBitLong(GetType(), NETMSG_TYPE_BITS);
	m_nLength = m_DataOut.GetNumBitsWritten();

	buffer.WriteUBitLong(m_nNewCommands, NUM_NEW_COMMAND_BITS);
	buffer.WriteUBitLong(m_nBackupCommands, NUM_BACKUP_COMMAND_BITS);

	buffer.WriteWord(m_nLength);

	return buffer.WriteBits(m_DataOut.GetData(), m_nLength);
}

bool CLC_Move::ReadFromBuffer(bf_read& buffer)
{
	//VPROF("CLC_Move::ReadFromBuffer");

	m_nNewCommands = buffer.ReadUBitLong(NUM_NEW_COMMAND_BITS);
	m_nBackupCommands = buffer.ReadUBitLong(NUM_BACKUP_COMMAND_BITS);
	m_nLength = buffer.ReadWord();
	m_DataIn = buffer;
	return buffer.SeekRelative(m_nLength);
}

bool CNetMessage::BIncomingMessageForProcessing(double dValue, int iValue)
{
	if (!m_pRateLimitPolicy)
		SetRatePolicy();

	return m_pRateLimitPolicy->BIncomingMessageForProcessing(dValue, iValue);
}

void CNetMessage::SetRatePolicy()
{
	m_pRateLimitPolicy = new CNetMessageRatelimitPolicyNone();
}

bool CNetMessageRatelimitPolicyNone::BIncomingMessageForProcessing(double dValue, int iValue)
{
	m_iUnk1++;
	m_iUnk2 += iValue;

	if (m_dUnk1 == dValue)
	{
		m_iUnk6 += iValue;
		m_iUnk5++;
	}
	else
	{
		m_dUnk0 = m_dUnk1;
		m_dUnk1 = dValue;
		m_iUnk3 = m_iUnk5;
		m_iUnk5 = 1;
		m_iUnk4 = m_iUnk6;
		m_iUnk6 = iValue;
	}

	//this always returns true, what the point in the code above?
	return true;
}

bool CNetMessageRatelimitPolicy::BIncomingMessageForProcessing(double dValue, int iValue)
{
	int iTmp = 0;

	if (m_dUnk1 == 0.0 || dValue - m_dUnk1 > m_dUnk2 || -m_dUnk2 > dValue - m_dUnk1)
	{
		m_iUnk3 = m_iUnk5;
		m_dUnk0 = m_dUnk1;
		m_dUnk1 = dValue;
		m_iUnk4 = m_iUnk6;
		m_iUnk5 = 1;
	}
	else
	{
		iTmp = m_iUnk6;
		m_iUnk5++;
	}

	m_iUnk1++;
	m_iUnk2 += iValue;
	iTmp += iValue;
	m_iUnk6 = iTmp;

	if (m_iUnk7 <= 0 || m_iUnk7 >= m_iUnk5)
		return (m_iUnk8 <= 0 || m_iUnk8 >= iTmp);

	return false;
}