// RpcCheck.h: interface for the CRpcCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPCCHECK_H__40C821C3_E246_40DB_A789_69CE8F76C488__INCLUDED_)
#define AFX_RPCCHECK_H__40C821C3_E246_40DB_A789_69CE8F76C488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define PLUGIN_RPC_CHECK  14

#define RPC_CHECK_PROG 1
#define RPC_CHECK_VERS 2
#define RPC_CHECK_PROC 4


class CRpcCheck : public CManipulate  
{
public:
	void SetupRpcCheck();
	void RpcCheckInit(char *data, OptTreeNode *otn, int protocol);
	void ParseRpc(char *data, OptTreeNode *otn);
	int CheckRpc(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	CRpcCheck();
	virtual ~CRpcCheck();

};

#endif // !defined(AFX_RPCCHECK_H__40C821C3_E246_40DB_A789_69CE8F76C488__INCLUDED_)
