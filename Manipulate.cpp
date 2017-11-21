// Manipulate.cpp: implementation of the CManipulate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Manipulate.h"
#include "Structure.h"
#include "Minfrag.h"
#include "Rules.h"
#include "HttpDecode.h"
#include "PortScan.h"
#include "Defrag.h"
#include "PatternMatch.h"
#include "TcpFlagCheck.h"
#include "IcmpTypeCheck.h"
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
#include "IcmpCodeCheck.h"
#include "TcpDumpLog.h"
#include "Log.h"
#include "StringMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CManipulate::CManipulate()
{

}

CManipulate::~CManipulate()
{

}

void CManipulate::CallAlertPlugins(Packet *p, char *c)
{
	CRules *objRule;
	objRule = new CRules;
	objRule->CallAlertPlugins(p,c);
}

void CManipulate::HttpDecodeInit(u_char* args)
{
	CHttpDecode *objHttpDecode;
	objHttpDecode = new CHttpDecode;
	objHttpDecode->HttpDecodeInit(args);
}

void CManipulate::PreprocUrlDecode(Packet* p)
{
	CHttpDecode *objHttpDecode;
	objHttpDecode = new CHttpDecode;
	objHttpDecode->PreprocUrlDecode(p);
}

void CManipulate::CallLogPlugins(Packet *p, char *message)
{
	CRules *objRule;
	objRule = new CRules;
	objRule->CallLogPlugins(p,message);
}


void CManipulate::CheckMinfrag(Packet *p)
{
	CMinFrag *objMinFrag;
	objMinFrag = new CMinFrag;
	objMinFrag->CheckMinfrag(p);
}

void CManipulate::MinfragInit(u_char *uc)
{
	CMinFrag *objminfrag;
	objminfrag = new CMinFrag;
	objminfrag->MinfragInit(uc);
}

void CManipulate::PortscanPreprocFunction(Packet *p)
{
	CPortScan *objPortScan;
	objPortScan = new CPortScan;
	objPortScan->PortscanPreprocFunction(p);
}

void CManipulate::PortscanInit(u_char *uc)
{
	CPortScan *objPortScan;
	objPortScan = new CPortScan;
	objPortScan->PortscanInit(uc);
}

void CManipulate::PortscanIgnoreHostsInit(u_char *args)
{
	CPortScan *objPortScan;
	objPortScan = new CPortScan;
	objPortScan->PortscanIgnoreHostsInit(args);
}

void CManipulate::PreprocDefrag(Packet *p)
{
	CDefrag *objDefrag;
	objDefrag = new CDefrag;
	objDefrag->PreprocDefrag(p);
}

void CManipulate::DefragInit(u_char *uc)
{
	CDefrag *objDefrag;
	objDefrag = new CDefrag;
	objDefrag->DefragInit(uc);
}

int CManipulate::CheckPatternMatch(Packet *p, _OptTreeNode *otn_idx, _OptFpList *fp_list)
{
	CPatternMatch *objPattern;
	objPattern = new CPatternMatch;
	return objPattern->CheckPatternMatch(p,otn_idx,fp_list);
}

void CManipulate::PayloadSearchInit(char *data, _OptTreeNode *otn, int protocol)
{
	CPatternMatch *objPattern;
	objPattern = new CPatternMatch;
	objPattern->PayloadSearchInit(data,otn,protocol);
}

void CManipulate::PayloadSearchOffset(char *data, _OptTreeNode *otn, int protocol)
{
	CPatternMatch *objPattern;
	objPattern = new CPatternMatch;
	objPattern->PayloadSearchOffset(data,otn,protocol);
}

void CManipulate::PayloadSearchDepth(char *data, _OptTreeNode *otn, int protocol)
{
	CPatternMatch *objPattern;
	objPattern = new CPatternMatch;
	objPattern->PayloadSearchDepth(data,otn,protocol);
}

void CManipulate::PayloadSearchNocase(char *data, _OptTreeNode *otn, int protocol)
{
	CPatternMatch *objPattern;
	objPattern = new CPatternMatch;
	objPattern->PayloadSearchNocase(data,otn,protocol);
}

void CManipulate::TCPFlagCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CTcpFlagCheck *objTcpFlag;
	objTcpFlag = new CTcpFlagCheck;
	objTcpFlag->TCPFlagCheckInit(data,otn,protocol);
}

int CManipulate::CheckTcpFlagsEq(Packet *p, _OptTreeNode *otn_idx, _OptFpList *fp_list)
{
	CTcpFlagCheck *objTcpFlag;
	objTcpFlag = new CTcpFlagCheck;
	return objTcpFlag->CheckTcpFlagsEq(p,otn_idx,fp_list);
}

int CManipulate::IcmpTypeCheck(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CIcmpTypeCheck *objIcmpType;
	objIcmpType = new CIcmpTypeCheck;
	return (objIcmpType->IcmpTypeCheck(p,otn,fp_list));
}

void CManipulate::IcmpTypeCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIcmpTypeCheck *objIcmpType;
	objIcmpType = new CIcmpTypeCheck;
	objIcmpType->IcmpTypeCheckInit(data,otn,protocol);
}

int CManipulate::CheckTtlEq(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CTtlCheck *objTtl;
	objTtl = new CTtlCheck;
	return objTtl->CheckTtlEq(p,otn,fp_list);
}

void CManipulate::TtlCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CTtlCheck *objTtl;
	objTtl = new CTtlCheck;
	objTtl->TtlCheckInit(data,otn,protocol);
}

int CManipulate::IpIdCheckEq(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CIpIdCheck *objIpId;
	objIpId = new CIpIdCheck;
	return objIpId->IpIdCheckEq(p,otn,fp_list);
}

void CManipulate::IpIdCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIpIdCheck *objIpId;
	objIpId = new CIpIdCheck;
	objIpId->IpIdCheckInit(data,otn,protocol);
}

void CManipulate::TcpAckCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CTcpAckCheck *objTcpAck;
	objTcpAck = new CTcpAckCheck;
	objTcpAck->TcpAckCheckInit(data,otn,protocol);
}

int CManipulate::CheckTcpAckEq(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CTcpAckCheck *objTcpAck;
	objTcpAck = new CTcpAckCheck;
	return objTcpAck->CheckTcpAckEq(p,otn,fp_list);
}

int CManipulate::CheckTcpSeqEq(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CTcpSeqCheck *objTcpSeq;
	objTcpSeq = new CTcpSeqCheck;
	return objTcpSeq->CheckTcpSeqEq(p,otn,fp_list);
}

void CManipulate::TcpSeqCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CTcpSeqCheck *objTcpSeq;
	objTcpSeq = new CTcpSeqCheck;
	objTcpSeq->TcpSeqCheckInit(data,otn,protocol);
}

int CManipulate::CheckDsizeEq(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CDSizeCheck *objDSize;
	objDSize = new CDSizeCheck;
	return objDSize->CheckDsizeEq(p,otn,fp_list);
}

int CManipulate::CheckDsizeGT(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CDSizeCheck *objDSize;
	objDSize = new CDSizeCheck;
	return objDSize->CheckDsizeGT(p,otn,fp_list);
}

int CManipulate::CheckDsizeLT(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CDSizeCheck *objDSize;
	objDSize = new CDSizeCheck;
	return objDSize->CheckDsizeLT(p,otn,fp_list);
}

void CManipulate::DsizeCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CDSizeCheck *objDSize;
	objDSize = new CDSizeCheck;
	objDSize->DsizeCheckInit(data,otn,protocol);
}

int CManipulate::CheckIpOptions(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CIpOptionCheck *objIpOption;
	objIpOption = new CIpOptionCheck;
	return objIpOption->CheckIpOptions(p,otn,fp_list);
}

void CManipulate::IpOptionInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIpOptionCheck *objIpOption;
	objIpOption = new CIpOptionCheck;
	objIpOption->IpOptionInit(data,otn,protocol);
}

void CManipulate::RpcCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CRpcCheck *objRpc;
	objRpc = new CRpcCheck;
	objRpc->RpcCheckInit(data,otn,protocol);
}

int CManipulate::CheckRpc(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CRpcCheck *objRpc;
	objRpc = new CRpcCheck;
	return objRpc->CheckRpc(p,otn,fp_list);
}

int CManipulate::IcmpIdCheck(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CIcmpIdCheck *objIcmpId;
	objIcmpId = new CIcmpIdCheck;
	return objIcmpId->IcmpIdCheck(p,otn,fp_list);
}

void CManipulate::IcmpIdCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIcmpIdCheck *objIcmpId;
	objIcmpId = new CIcmpIdCheck;
	objIcmpId->IcmpIdCheckInit(data,otn,protocol);
}

void CManipulate::IcmpSeqCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIcmpSeqCheck *objIcmpSeq;
	objIcmpSeq = new CIcmpSeqCheck;
	objIcmpSeq->IcmpSeqCheckInit(data,otn,protocol);
}

int CManipulate::IcmpSeqCheck(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CIcmpSeqCheck *objIcmpSeq;
	objIcmpSeq = new CIcmpSeqCheck;
	return objIcmpSeq->IcmpSeqCheck(p,otn,fp_list);
}

void CManipulate::SessionInit(char *data, _OptTreeNode *otn, int protocol)
{
	CSession *objSession;
	objSession = new CSession;
	objSession->SessionInit(data,otn,protocol);
}


int CManipulate::LogSessionData(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CSession *objSession;
	objSession = new CSession;
	return objSession->LogSessionData(p,otn,fp_list);
}

void CManipulate::AlertSyslog(Packet *p, char *msg, void *arg)
{
	CAlertSysLog *objAlertSys;
	objAlertSys = new CAlertSysLog;
	objAlertSys->AlertSyslog(p,msg,arg);
}

void CManipulate::AlertSyslogCleanExitFunc(int signal, void *arg)
{
	CAlertSysLog *objAlert;
	objAlert = new CAlertSysLog;
	objAlert->AlertSyslogCleanExitFunc(signal,arg);
}

void CManipulate::AlertSyslogRestartFunc(int signal, void *arg)
{
	CAlertSysLog *objAlert;
	objAlert = new CAlertSysLog;
	objAlert->AlertSyslogRestartFunc(signal,arg);
}

void CManipulate::LogTcpdump(Packet *p, char *msg, void *arg)
{
	CTcpDumpLog *objTcpDump;
	objTcpDump = new CTcpDumpLog;
	objTcpDump->LogTcpdump(p,msg,arg);
}

void CManipulate::SpoLogTcpdumpCleanExitFunc(int signal, void *arg)
{
	CTcpDumpLog *objTcpDump;
	objTcpDump = new CTcpDumpLog;
	objTcpDump->SpoLogTcpdumpCleanExitFunc(signal,arg);
}

void CManipulate::SpoLogTcpdumpRestartFunc(int signal, void *arg)
{
	CTcpDumpLog *objTcpDump;
	objTcpDump = new CTcpDumpLog;
	objTcpDump->SpoLogTcpdumpRestartFunc(signal,arg);
}

void CManipulate::LogDatabase(Packet *p, char *msg, void *arg)
{	
	CDatabaseLog *objDLog;
	objDLog = new CDatabaseLog;
	objDLog->LogDatabase(p,msg,arg);
}

void CManipulate::AlertSyslogInit(u_char *args)
{
	CAlertSysLog *objAlert;
	objAlert = new CAlertSysLog;
	objAlert->AlertSyslogInit(args);
}

void CManipulate::LogDatabaseInit(u_char *args)
{	
	CDatabaseLog *objDatabase;
	objDatabase = new CDatabaseLog;
	objDatabase->LogDatabaseInit(args);
}

void CManipulate::SpoAlertFast(Packet *p, char *msg, void *arg)
{
	CFastAlert *objAlert;
	objAlert = new CFastAlert;
	objAlert->SpoAlertFast(p,msg,arg);
}

void CManipulate::FastAlertCleanExitFunc(int signal, void *arg)
{
	CFastAlert *objAlert;
	objAlert = new CFastAlert;
	objAlert->FastAlertCleanExitFunc(signal,arg);
}

void CManipulate::FastAlertRestartFunc(int signal, void *arg)
{
	CFastAlert *objAlert;
	objAlert = new CFastAlert;
	objAlert->FastAlertRestartFunc(signal,arg);
}

void CManipulate::FastAlertInit(u_char *args)
{
	CFastAlert *objAlert;
	objAlert = new CFastAlert;
	objAlert->FastAlertInit(args);
}

void CManipulate::SpoAlertFull(Packet *p, char *msg, void *arg)
{
	CFullAlert *objFull;
	objFull = new CFullAlert;
	objFull->SpoAlertFull(p,msg,arg);
}

void CManipulate::FullAlertInit(u_char *args)
{
	CFullAlert *objFull;
	objFull = new CFullAlert;
	objFull->FullAlertInit(args);
}

void CManipulate::FullAlertCleanExitFunc(int signal, void *arg)
{
	CFullAlert *objFull;
	objFull = new CFullAlert;
	objFull->FullAlertCleanExitFunc(signal,arg);
}

void CManipulate::FullAlertRestartFunc(int signal, void *arg)
{
	CFullAlert *objFull;
	objFull = new CFullAlert;
	objFull->FullAlertRestartFunc(signal,arg);
}

void CManipulate::AlertSmbInit(u_char *args)
{
	CAlertSMB *objAlert;
	objAlert = new CAlertSMB;
	objAlert->AlertSmbInit(args);
}

void CManipulate::SpoAlertSmbRestartFunc(int signal, void *arg)
{
	CAlertSMB *objAlert;
	objAlert = new CAlertSMB;
	objAlert->SpoAlertSmbRestartFunc(signal,arg);
}

void CManipulate::SpoAlertSmbCleanExitFunc(int signal, void *arg)
{
	CAlertSMB *objAlert;
	objAlert = new CAlertSMB;
	objAlert->SpoAlertSmbCleanExitFunc(signal,arg);
}

void CManipulate::SpoAlertSmb(Packet *p, char *msg, void *arg)
{
	CAlertSMB *objAlert;
	objAlert = new CAlertSMB;
	objAlert->SpoAlertSmb(p,msg,arg);
}

void CManipulate::SpoAlertUnixSock(Packet *p, char *msg, void *arg)
{
	CAlertUnixSocket *objAlert;
	objAlert = new CAlertUnixSocket;
	objAlert->SpoAlertUnixSock(p,msg,arg);
}

void CManipulate::AlertUnixSockInit(u_char *args)
{
	CAlertUnixSocket *objAlert;
	objAlert = new CAlertUnixSocket;
	objAlert->AlertUnixSockInit(args);
}

int CManipulate::IcmpCodeCheck(Packet *p, _OptTreeNode *otn, _OptFpList *fp_list)
{
	CIcmpCodeCheck *objCode;
	objCode = new CIcmpCodeCheck;
	return objCode->IcmpCodeCheck(p, otn, fp_list);
}

void CManipulate::IcmpCodeCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIcmpCodeCheck *objCode;
	objCode = new CIcmpCodeCheck;
	objCode->IcmpCodeCheckInit(data,otn,protocol);
}

void CManipulate::LogTcpdumpInit(u_char *args)
{
	CTcpDumpLog *objTcpDump;
	objTcpDump = new CTcpDumpLog;
	objTcpDump->LogTcpdumpInit(args);
}

void CManipulate::SyslogAlert(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->SyslogAlert(p,msg,arg);
}

void CManipulate::FastAlert(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->FastAlert(p,msg,arg);
}

void CManipulate::FullAlert(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->FullAlert(p,msg,arg);
}

void CManipulate::UnixSockAlert(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->UnixSockAlert(p,msg,arg);
}

void CManipulate::NoAlert(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->NoAlert(p,msg,arg);
}

void CManipulate::LogBin(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->LogBin(p,msg,arg);
}

void CManipulate::LogPkt(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->LogPkt(p,msg,arg);
}

void CManipulate::NoLog(Packet *p, char *msg, void *arg)
{
	CLog *objLog;
	objLog = new CLog;
	objLog->NoLog(p,msg,arg);
}

int CManipulate::CheckBidirectional(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckBidirectional(p,rtn_idx,fp_list);
}

int CManipulate::RuleListEnd(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->RuleListEnd(p,rtn_idx,fp_list);
}

int CManipulate::CheckSrcIPNotEq(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckSrcIPNotEq(p,rtn_idx,fp_list);
}

int CManipulate::CheckDstIPNotEq(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckDstIPNotEq(p,rtn_idx,fp_list);
}

int CManipulate::CheckSrcIPEqual(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckSrcIPEqual(p,rtn_idx,fp_list);
}

int CManipulate::CheckDstIPEqual(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckDstIPEqual(p,rtn_idx,fp_list);
}

int CManipulate::CheckSrcPortNotEq(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckSrcPortNotEq(p,rtn_idx,fp_list);
}

int CManipulate::CheckDstPortNotEq(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckDstPortNotEq(p,rtn_idx,fp_list);
}

int CManipulate::CheckSrcPortEqual(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckSrcPortEqual(p,rtn_idx,fp_list);
}

int CManipulate::CheckDstPortEqual(Packet *p, _RuleTreeNode *rtn_idx, _RuleFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->CheckDstPortEqual(p,rtn_idx,fp_list);
}

int CManipulate::OptListEnd(Packet *p, _OptTreeNode *otn_idx, _OptFpList *fp_list)
{
	CRules *objRule;
	objRule = new CRules;
	return objRule->OptListEnd(p,otn_idx,fp_list);
}

int CManipulate::mSearchCI(char *buf, int blen, char *ptrn, int plen, int *skip, int *shift)
{
	CStringMan *objString;
	objString = new CStringMan;
	return objString->mSearchCI(buf,blen,ptrn,plen,skip,shift);
}

int CManipulate::mSearch(char *buf, int blen, char *ptrn, int plen, int *skip, int *shift)
{
	CStringMan *objString;
	objString = new CStringMan;
	return	objString->mSearch(buf,blen,ptrn,plen,skip,shift);
}
