// Preprocessor.cpp: implementation of the CPreprocessor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Preprocessor.h"
#include "Message.h"
#include "HttpDecode.h"
#include "Minfrag.h"
#include "PortScan.h"
#include "Defrag.h"
#include "PatternMatch.h"
#include "TcpFlagCheck.h"
#include "IcmpTypeCheck.h"
#include "IcmpCodeCheck.h"
#include "TtlCheck.h"
#include "IpIdCheck.h"
#include "TcpAckCheck.h"
#include "TcpSeqCheck.h"
#include "DSizeCheck.h"
#include "IpOptionCheck.h"
#include "RpcCheck.h"
#include "IcmpIdCheck.h"
#include "IcmpSeqCheck.h"
#include "Session.h"
#include "AlertSysLog.h"
#include "TcpDumpLog.h"
#include "DatabaseLog.h"
#include "FastAlert.h"
#include "FullAlert.h"
#include "AlertSMB.h"
#include "AlertUnixSocket.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreprocessor::CPreprocessor()
{

}

CPreprocessor::~CPreprocessor()
{

}

void CPreprocessor::InitPreprocessors()
{
	CMessage msg;

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CHttpDecode objHDecode;
	objHDecode.SetupHttpDecode();
	CMinFrag objMinfrag;
	objMinfrag.SetupMinfrag();
	CPortScan objPortScan;
	objPortScan.SetupPortscan(); 
	objPortScan.SetupPortscanIgnoreHosts();
	CDefrag objDefrag;
	objDefrag.SetupDefrag();
}

void CPreprocessor::RegisterPreprocessor(char *keyword, void (CManipulate::*func)(u_char *))
{
	PreprocessKeywordList *idx;
	static void (CManipulate::*ptrfun)(u_char *);
	ptrfun = func;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	idx = ptrMainApp->PreprocessKeywords;
	if (idx == NULL){
		ptrMainApp->PreprocessKeywords  = (PreprocessKeywordList *) calloc(sizeof(PreprocessKeywordList),sizeof(char));
		ptrMainApp->PreprocessKeywords->entry.keyword = (char *) calloc(strlen(keyword)+1, sizeof(char));
        strncpy(ptrMainApp->PreprocessKeywords->entry.keyword, keyword, strlen(keyword));
		ptrMainApp->PreprocessKeywords->entry.func = ptrfun;
	}else{
		while(idx->next != NULL){
			if (!strncasecmp(idx->entry.keyword, keyword, strlen(keyword)));
			idx = idx->next;
		}
		idx->next = (PreprocessKeywordList *) calloc(sizeof(PreprocessKeywordList), sizeof(char));
		idx = idx->next;
		idx->entry.keyword = (char *) calloc(strlen(keyword)+1, sizeof(char));
		strncpy(idx->entry.keyword, keyword, strlen(keyword));
        idx->entry.func = ptrfun;
		strncpy(idx->entry.keyword, keyword, strlen(keyword));
	}
}

int CPreprocessor::PacketIsTCP(Packet *p)
{
	if (p->iph != NULL && p->tcph != NULL)
        return 1;
    return 0;
}



int CPreprocessor::PacketIsUDP(Packet *p)
{
	if (p->iph != NULL && p->udph != NULL)
        return 1;
    return 0;
}



int CPreprocessor::PacketIsICMP(Packet *p)
{
	if (p->iph != NULL && p->icmph != NULL)
        return 1;
    return 0;
}


int CPreprocessor::DestinationIpIsHomenet(Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	if ((p->iph->ip_dst.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
        return 1;
    }
    return 0;
}



int CPreprocessor::SourceIpIsHomenet(Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	if ((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
        return 1;
    }
    return 0;
}

int CPreprocessor::IsTcpSessionTraffic(Packet *p)
{
	if (p->tcph == NULL)
        return 0;
    if (p->tcph->th_flags != (TH_PUSH | TH_ACK)){
        return 0;
    }
    return 1;
}


int CPreprocessor::CheckNet(in_addr *compare, in_addr *compare2)
{
	if (compare->s_addr == compare2->s_addr){
        return 1;
    }
    return 0;
}

int CPreprocessor::PacketIsIP(Packet *p)
{
	if (p->iph != NULL)
        return 1;
    return 0;
}

void CPreprocessor::RegisterPlugin(char *keyword, void (CManipulate::*func)(char *,OptTreeNode *,int))
{

		CIFServerApp* ptrMainApp;
		ptrMainApp =PTRAPP;
		KeywordXlateList *idx;
        idx = ptrMainApp->KeywordList;
    if (idx == NULL){
        ptrMainApp->KeywordList = (KeywordXlateList *) calloc(sizeof(KeywordXlateList), sizeof(char));
        ptrMainApp->KeywordList->entry.keyword = (char *) calloc(strlen(keyword)+1, sizeof(char));
        strncpy(ptrMainApp->KeywordList->entry.keyword, keyword, strlen(keyword));
        ptrMainApp->KeywordList->entry.func = func;
    }else{
        while (idx->next != NULL){
            if (!strncasecmp(idx->entry.keyword, keyword, strlen(keyword))){
//                FatalError( "ERROR %s (%d) => Duplicate detection plugin keyword!\n", file_name, file_line);
            }
            idx = idx->next;
        }
        idx->next = (KeywordXlateList *) calloc(sizeof(KeywordXlateList), sizeof(char));
        idx = idx->next;
        idx->entry.keyword = (char *) calloc(strlen(keyword)+1, sizeof(char));
        strncpy(idx->entry.keyword, keyword, strlen(keyword));
        idx->entry.func = func;
    }
}

void CPreprocessor::InitPlugIns()
{
	CMessage msg;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CPatternMatch *objPattern;
	objPattern = new CPatternMatch;
	objPattern->SetupPatternMatch();
	CTcpFlagCheck *objTcpFlag;
	objTcpFlag = new CTcpFlagCheck;
	objTcpFlag->SetupTCPFlagCheck();
	CIcmpTypeCheck *objIcmpType;
	objIcmpType = new CIcmpTypeCheck;
	objIcmpType->SetupIcmpTypeCheck();
	CIcmpCodeCheck *objIcmpCode;
	objIcmpCode = new CIcmpCodeCheck;
	objIcmpCode->SetupIcmpCodeCheck();
    CTtlCheck *objTtlCheck;
	objTtlCheck = new CTtlCheck;
	objTtlCheck->SetupTtlCheck();
    CIpIdCheck *objIpIdCheck;
	objIpIdCheck = new CIpIdCheck;
	objIpIdCheck->SetupIpIdCheck();
    CTcpAckCheck *objTcpAck;
	objTcpAck = new CTcpAckCheck;
	objTcpAck->SetupTcpAckCheck();
    CTcpSeqCheck *objTcpSeq;
	objTcpSeq = new CTcpSeqCheck;
	objTcpSeq->SetupTcpSeqCheck();
    CDSizeCheck *objDsize;
	objDsize = new CDSizeCheck;
	objDsize->SetupDsizeCheck();
    CIpOptionCheck *objIpOption;
	objIpOption = new CIpOptionCheck;
	objIpOption->SetupIpOptionCheck();
    CRpcCheck *objRpcCheck;
	objRpcCheck = new CRpcCheck;
	objRpcCheck->SetupRpcCheck();
    CIcmpIdCheck *objIcmpId;
	objIcmpId = new CIcmpIdCheck;
	objIcmpId->SetupIcmpIdCheck();
    CIcmpSeqCheck *objIcmpSeq;
	objIcmpSeq = new CIcmpSeqCheck;
	objIcmpSeq->SetupIcmpSeqCheck();
    CSession *objSession;
	objSession = new CSession;
	objSession->SetupSession();
}

void CPreprocessor::RegisterOutputPlugin(char *keyword, int type, void (CManipulate::*func)(u_char *))
{

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	OutputKeywordList *idx;
    idx = ptrMainApp->OutputKeywords;
    if (idx == NULL){
        ptrMainApp->OutputKeywords = (OutputKeywordList *) calloc(sizeof(OutputKeywordList), sizeof(char));
        idx = ptrMainApp->OutputKeywords;
    }else{
        while (idx->next != NULL){
            if (!strncasecmp(idx->entry.keyword, keyword, strlen(keyword))){
//                FatalError( "ERROR %s (%d) => Duplicate output keyword!\n", file_name, file_line);
            }
            idx = idx->next;
        }
        idx->next = (OutputKeywordList *) calloc(sizeof(OutputKeywordList), sizeof(char));
        idx = idx->next;
    }
    idx->entry.keyword = (char *) calloc(strlen(keyword)+1, sizeof(char));
    strncpy(idx->entry.keyword, keyword, strlen(keyword));
    idx->entry.node_type = type;
    idx->entry.func =  func;
}

void CPreprocessor::AddFuncToRestartList(void (CManipulate::*func)(int,void *), void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	ptrMainApp->PluginRestartList = AddFuncToSignalList(func, arg, ptrMainApp->PluginRestartList);
}

void CPreprocessor::AddFuncToCleanExitList(void (CManipulate::*func)(int,void *), void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	ptrMainApp->PluginCleanExitList = AddFuncToSignalList(func, arg, ptrMainApp->PluginCleanExitList);
}


PluginSignalFuncNode* CPreprocessor::AddFuncToSignalList(void (CManipulate::*func)(int,void *), void *arg, PluginSignalFuncNode *list)
{
	PluginSignalFuncNode *idx;
    idx = list;
    if (idx == NULL){
        idx = (PluginSignalFuncNode *) calloc(sizeof(PluginSignalFuncNode), sizeof(char));
        idx->func = func;
        idx->arg = arg;
        list = idx;
    }else{
        while (idx->next != NULL)
            idx = idx->next;
        idx->next = (PluginSignalFuncNode *) calloc(sizeof(PluginSignalFuncNode), sizeof(char));
        idx = idx->next;
        idx->func = func;
        idx->arg = arg;
    }
    idx->next = NULL;
    return list;
}


void CPreprocessor::InitOutputPlugins()
{
	CMessage msg;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CAlertSysLog *objAlert;
	objAlert = new CAlertSysLog;
	objAlert->SetupAlertSyslog();
    CTcpDumpLog *objTcpDump;
	objTcpDump = new CTcpDumpLog;
	objTcpDump->SetupTcpDumpLog();
    CDatabaseLog *objDBLog;
	objDBLog = new CDatabaseLog;
	objDBLog->SetupLogDatabase();
    CFastAlert *objFast;
	objFast = new CFastAlert;
	objFast->SetupFastAlert();
    CFullAlert *objFull;
	objFull = new CFullAlert;
	objFull->SetupFullAlert();
#ifdef ENABLE_SMB_ALERTS
    CAlertSMB *objSMb;
	objSMb = new CAlertSMB;
	objSMb->SetupAlertSMB();
#endif
    CAlertUnixSocket *objUnix;
	objUnix = new CAlertUnixSocket;
	objUnix->SetupAlertUnixSocket();
}
