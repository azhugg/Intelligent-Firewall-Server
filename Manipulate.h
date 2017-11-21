// Manipulate.h: interface for the CManipulate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANIPULATE_H__EB0E4B12_E755_4375_90EF_73AB66D03860__INCLUDED_)
#define AFX_MANIPULATE_H__EB0E4B12_E755_4375_90EF_73AB66D03860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//struct _OptTreeNode

class CManipulate  
{
	 
public:
	virtual int mSearch(char *, int, char *, int, int *, int *);
	virtual int mSearchCI(char *, int, char *, int, int *, int *);
	virtual int OptListEnd(Packet *p, struct _OptTreeNode *otn_idx, struct _OptFpList *fp_list);
	virtual int CheckDstPortEqual(Packet *p, struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckSrcPortEqual(Packet *p,struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckDstPortNotEq(Packet *p,struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckSrcPortNotEq(Packet *p, struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckDstIPEqual(Packet *p, struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckSrcIPEqual(Packet *p, struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckDstIPNotEq(Packet *p,struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int CheckSrcIPNotEq(Packet *p, struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual int RuleListEnd(Packet *p,struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	int CheckBidirectional(Packet *p,struct _RuleTreeNode *rtn_idx, struct _RuleFpList *fp_list);
	virtual void NoLog(Packet* p,char* msg, void* arg);
	virtual void LogPkt(Packet* p,char* msg, void* arg);
	virtual void LogBin(Packet*, char*, void*);
	virtual void NoAlert(Packet *p, char *msg, void *arg);
	virtual void UnixSockAlert(Packet *p, char *msg, void *arg);
	virtual void FullAlert(Packet *p, char *msg, void *arg);
	virtual void FastAlert(Packet *p, char *msg, void *arg);
	virtual void SyslogAlert(Packet *p, char *msg, void *arg);
	virtual void LogTcpdumpInit(u_char *args);
	virtual void IcmpCodeCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual int IcmpCodeCheck(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void AlertUnixSockInit(u_char *args);
	virtual void SpoAlertUnixSock(Packet *p, char *msg, void *arg);
	virtual void SpoAlertSmb(Packet *p, char *msg, void *arg);
	virtual void SpoAlertSmbCleanExitFunc(int signal, void *arg);
	virtual void SpoAlertSmbRestartFunc(int signal, void *arg);
	virtual void AlertSmbInit(u_char *args);
	virtual void FullAlertRestartFunc(int signal, void *arg);
	virtual void FullAlertCleanExitFunc(int signal, void *arg);
	virtual void FullAlertInit(u_char *args);
	virtual void SpoAlertFull(Packet *p, char *msg, void *arg);
	virtual void FastAlertInit(u_char *args);
	virtual void FastAlertRestartFunc(int signal, void *arg);
	virtual void FastAlertCleanExitFunc(int signal, void *arg);
	virtual void SpoAlertFast(Packet *p, char *msg, void *arg);
	virtual void LogDatabaseInit(u_char *args);
	virtual void AlertSyslogInit(u_char *args);
	virtual void LogDatabase(Packet *p, char *msg, void *arg);
	virtual void SpoLogTcpdumpRestartFunc(int signal, void *arg);
	virtual void SpoLogTcpdumpCleanExitFunc(int signal, void *arg);
	virtual void LogTcpdump(Packet *p, char *msg, void *arg);
	virtual void AlertSyslogRestartFunc(int signal, void *arg);
	virtual void AlertSyslogCleanExitFunc(int signal, void *arg);
	virtual void AlertSyslog(Packet *p, char *msg, void *arg);
	virtual int LogSessionData(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void SessionInit(char *data,struct _OptTreeNode *otn, int protocol);
	virtual int IcmpSeqCheck(Packet *p, struct _OptTreeNode *otn,struct _OptFpList *fp_list);
	virtual void IcmpSeqCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual	void IcmpIdCheckInit(char *data,struct _OptTreeNode *otn, int protocol);
	virtual int IcmpIdCheck(Packet *p,struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual int CheckRpc(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void RpcCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual void IpOptionInit(char *data,struct _OptTreeNode *otn, int protocol);
	virtual int CheckIpOptions(Packet *p,struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void DsizeCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual int CheckDsizeLT(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual int CheckDsizeGT(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual int CheckDsizeEq(Packet *p,struct _OptTreeNode *otn,struct _OptFpList *fp_list);
	virtual void TcpSeqCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual int CheckTcpSeqEq(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual int CheckTcpAckEq(Packet *p,struct  _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void TcpAckCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual void IpIdCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual int IpIdCheckEq(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void TtlCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual int CheckTtlEq(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual void IcmpTypeCheckInit(char *data,struct _OptTreeNode *otn, int protocol);
	virtual int IcmpTypeCheck(Packet *p, struct _OptTreeNode *otn, struct _OptFpList *fp_list);
	virtual int CheckTcpFlagsEq(Packet *p, struct _OptTreeNode *otn_idx, struct _OptFpList *fp_list);
	virtual void TCPFlagCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	virtual void PayloadSearchNocase(char *data, struct _OptTreeNode *otn, int protocol);
	virtual void PayloadSearchDepth(char *data, struct _OptTreeNode *otn, int protocol);
	virtual void PayloadSearchOffset(char *data,struct _OptTreeNode *otn, int protocol);
	virtual void PayloadSearchInit(char *data,struct _OptTreeNode *otn, int protocol);
	virtual int CheckPatternMatch(Packet *p, struct _OptTreeNode *otn_idx,struct _OptFpList *fp_list);
	virtual void DefragInit(u_char*);
	virtual void PreprocDefrag(Packet*);
	virtual void PortscanIgnoreHostsInit(u_char *args);
	virtual void PortscanInit(u_char*);
	virtual void PortscanPreprocFunction(Packet *p);
	virtual void MinfragInit(u_char*);
	virtual void CheckMinfrag(Packet*);
	virtual void CallLogPlugins(Packet *p, char *message);
	virtual void CallAlertPlugins(Packet *, char *);
	virtual void HttpDecodeInit(u_char* args);
	virtual void PreprocUrlDecode(Packet*);
	CManipulate();
	virtual ~CManipulate();
};

#endif // !defined(AFX_MANIPULATE_H__EB0E4B12_E755_4375_90EF_73AB66D03860__INCLUDED_)
