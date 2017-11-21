// TrafficControl.h: interface for the CTrafficControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAFFICCONTROL_H__7B760863_80A3_497D_9943_653782D06954__INCLUDED_)
#define AFX_TRAFFICCONTROL_H__7B760863_80A3_497D_9943_653782D06954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "iptypes.h"
#include "IFServer.h"	// Added by ClassView

#define THE_PIPE "\\\\.\\pipe\\IFSServicePipe"	
#define MAX_RULE_LENGTH 256

#define SYN_ERROR_MSG "Syntax error: "
#define SYN_ERROR_MSG_LEN 14

#define FILTER_FAILURE 0 /* filter was correctly added */
#define FILTER_SUCCESS 1 /* filter had a syntax error */
#define FILTER_MESSAGE 2 /* informative message returned to the client */
#define MAX_NODE	   25
#define MAX_CLIENTS	   25


class CTrafficControl
{
public:
	void AddFilterToList(char *filter);
	void DeleteFilter(char* filter);
	CIFServerApp *m_ptrMainApp;
	void FreeNode(AlertNode *node, int NodeNumber, int protocol);
	BOOL AddToDenyList(int protocol, int Node);
	void ActivateBlockingProcessor();
	char StartUpdateListShell(void);
	char ReadFiltersFromFile(char *filename);
	void NumberedListRulesOnInterface(char *iface);
	void ListRulesOnInterface(char *iface);
	void FlushInterface(char *iface);
	void DetailedStatsOnInterface(char *iface);
	void BriefStatsOnInterface(char *iface);
	char WriteFilter(char *filter);
	IP_ADAPTER_INFO* ListIpInterfaces(void);
	void DisplaySynError(char *filter, unsigned char pos);
	short GetOutputWidth(void);
	CTrafficControl();
	virtual ~CTrafficControl();
	
	static CRITICAL_SECTION m_csAcessLock;

};

#endif // !defined(AFX_TRAFFICCONTROL_H__7B760863_80A3_497D_9943_653782D06954__INCLUDED_)
