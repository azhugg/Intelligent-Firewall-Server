// PortScan.h: interface for the CPortScan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTSCAN_H__FBDC94A6_FC38_4460_962B_635134FF3A0E__INCLUDED_)
#define AFX_PORTSCAN_H__FBDC94A6_FC38_4460_962B_635134FF3A0E__INCLUDED_

#include "Structure.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MODNAME "spp_portscan"


class CPortScan : public CManipulate  
{
public:
	void ExpireConnections(ScanList *scanList, struct spp_timeval watchPeriod, struct spp_timeval currentTime);
	int IsServer(Packet *p);
	ScanType CheckTCPFlags(u_char th_flags);
	void PortscanPreprocFunction(Packet *p);
	CPortScan();
	virtual ~CPortScan();

public:
	void SetupPortscanIgnoreHosts(void);
	void PortscanIgnoreHostsInit(u_char *args);
	void CreateServerList(u_char *servers);
	void SetupPortscan(void);
	void PortscanInit(u_char *args);
	ScanList* CreateScanList(void);
	void ParsePortscanArgs(u_char *args);
	ConnectionInfo* NewConnection(Packet *p, ScanType scanType);
	SourceInfo* AddSource(SourceInfo *currentSource, Packet *p, ScanType scanType);
	ConnectionInfo* AddConnection(ConnectionInfo *currentConnection, Packet *p, ScanType scanType);
	DestinationInfo* AddDestination(DestinationInfo* currentDestination, Packet *p, ScanType scanType);
	DestinationInfo* NewDestination(Packet *p, ScanType scanType);
	void ExtractHeaderInfo(Packet *p, struct in_addr *saddr, struct in_addr *daddr, u_short *sport, u_short *dport);
	SourceInfo* NewSource(Packet *p, ScanType scanType);
	void RemoveSource(SourceInfo *delSource);
	void RemoveDestination(DestinationInfo *delDestination);
	void RemoveConnection(ConnectionInfo *delConnection);
	void ClearConnectionInfoFromSource(SourceInfo *currentSource);
	void AlertIntermediateInfo(SourceInfo* currentSource);
	void LogScanInfoToSeparateFile(SourceInfo *currentSource);
	int NewScan(ScanList *scanList, Packet *p, ScanType scanType);
	u_long homeNet,homeNetMask;
	ScanType scansToWatch;
	ScanList *scanList;
	struct spp_timeval maxTime;
	long maxPorts;
	LogLevel logLevel;
	FILE *logFile;
	int packetLogSize;  
	ServerNode *serverList;
	enum _timeFormat{
		tLOCAL, tGMT 
	} timeFormat; 
};

#endif // !defined(AFX_PORTSCAN_H__FBDC94A6_FC38_4460_962B_635134FF3A0E__INCLUDED_)
