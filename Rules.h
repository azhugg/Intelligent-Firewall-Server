// Rules.h: interface for the CRules class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULES_H__86F70F08_DA25_451E_92FB_FF1538EC8873__INCLUDED_)
#define AFX_RULES_H__86F70F08_DA25_451E_92FB_FF1538EC8873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RULE_LOG     0
#define RULE_PASS    1
#define RULE_ALERT   2
#define RULE_VAR     3
#define RULE_INCLUDE 4
#define RULE_PREPROCESS 5
#define RULE_OUTPUT 6
#define EXCEPT_SRC_IP  0x01
#define EXCEPT_DST_IP  0x02
#define ANY_SRC_PORT   0x04
#define ANY_DST_PORT   0x08
#define ANY_FLAGS      0x10
#define EXCEPT_SRC_PORT 0x20
#define EXCEPT_DST_PORT 0x40
#define BIDIRECTIONAL   0x80
#define R_FIN          0x01
#define R_SYN          0x02
#define R_RST          0x04
#define R_PSH          0x08
#define R_ACK          0x10
#define R_URG          0x20
#define R_RES2         0x40
#define R_RES1         0x80
#define MODE_EXIT_ON_MATCH   0
#define MODE_FULL_SEARCH     1
#define CHECK_SRC            0x01
#define CHECK_DST            0x02
#define INVERSE              0x04
#define SESSION_PRINTABLE    1
#define SESSION_ALL          2
#define RESP_RST_SND         0x01
#define RESP_RST_RCV         0x02
#define RESP_BAD_NET         0x04
#define RESP_BAD_HOST        0x08
#define RESP_BAD_PORT        0x10
#define MODE_EXIT_ON_MATCH   0
#define MODE_FULL_SEARCH     1
#define SRC                  0
#define DST                  1
#ifndef NT_OUTPUT_ALERT
#define NT_OUTPUT_ALERT  0   
#define NT_OUTPUT_LOG    1   
#endif

class CRules:public CManipulate
{
public:
	char* VarGet(char *name);
	int ParseIP(char *paddr, u_long *ip_addr, u_long *netmask);
	int CheckAddrPort(u_long addr, u_long mask, u_short hi_port, u_short lo_port, Packet *p, char flags, int mode);
	void CallLogPlugins(Packet *p, char *message);
	void CallAlertPlugins(Packet *p, char *message);
	void AddFuncToPreprocList(void (CManipulate::*func)(Packet *));
	CRules();
	virtual ~CRules();
public:
	char* CreateRule(char *rule, char *buf, char *rule_buf);
	void ParseLogto(char *filename);
	void ParseMessage(char *msg);
	void ParseListFile(char *rule, char *file);
	int OptListEnd(Packet *p, struct _OptTreeNode *otn_idx, OptFpList *fp_list);
	int CheckDstPortEqual(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckSrcPortEqual(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckDstPortNotEq(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckSrcPortNotEq(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckDstIPEqual(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckSrcIPEqual(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckDstIPNotEq(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckSrcIPNotEq(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int RuleListEnd(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	int CheckBidirectional(Packet *p, struct _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	void AddRuleFuncToList(int (CManipulate::*func)(Packet *,struct _RuleTreeNode*,struct _RuleFpList*), RuleTreeNode *rtn);
	void ParseRuleOptions(char *rule, int rule_type, int protocol);
	void SetOutputList(void (CManipulate::*func)(Packet *, char *, void *), char node_type, void *arg);
	struct VarEntry* VarAlloc();
	int TestHeader(RuleTreeNode *rule, RuleTreeNode *rtn);
	void XferHeader(RuleTreeNode *rule, RuleTreeNode *rtn);
	void PortToFunc(RuleTreeNode *rtn, int any_flag, int except_flag, int mode);
	void AddrToFunc(RuleTreeNode *rtn, u_long ip, u_long mask, int exception_flag, int mode);
	void SetupRTNFuncList(RuleTreeNode *rtn);
	void ProcessHeadNode(RuleTreeNode *test_node, ListHead *list, int protocol);
	int ConvPort(char *port, char *proto);
	int ParsePort(char *prule_port, u_short *hi_port, u_short *lo_port, char *proto, int *not_flag);
	int WhichProto(char *proto_str);
	void ParseOutputPlugin(char *rule);
	void ParsePreprocessor(char *rule);
	void VarDefine(char *name, char *value);
	char* ExpandVars(char *string);
	int RuleType(char *func);
	void ParseRule(char *prule, int inclevel);
	void DumpChain(RuleTreeNode *rtn_head, char *name);
	void IntegrityCheck(RuleTreeNode *rtn_head, char *listname);
	void ParseRulesFile(char *file, int inclevel);
	char* ProcessFileOption(char *filespec);
	void AddFuncToOutputList(void (CManipulate::*func)(Packet *, char *, void *), char node_type, void *arg);
	void AddOptFuncToList(int (CManipulate::*func)(Packet *,struct _OptTreeNode*,struct _OptFpList*), OptTreeNode *otn);
	int EvalOpts(OptTreeNode *List, Packet *p);
	int EvalHeader(RuleTreeNode *rtn_idx, Packet *p);
	int EvalPacket(ListHead *List, int mode, Packet *p);
	int Detect(Packet *p);
	void Preprocess(Packet *p);
	ListHead *Alert;      
	ListHead *Log;        
	ListHead *Pass;       
	RuleTreeNode *rtn_tmp;  
	int list_file_line; 
};

#endif // !defined(AFX_RULES_H__86F70F08_DA25_451E_92FB_FF1538EC8873__INCLUDED_)
