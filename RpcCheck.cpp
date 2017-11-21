// RpcCheck.cpp: implementation of the CRpcCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "RpcCheck.h"
#include <rpc.h>
#include "Rules.h"
#include "Preprocessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRpcCheck::CRpcCheck()
{

}

CRpcCheck::~CRpcCheck()
{

}

int CRpcCheck::CheckRpc(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    RpcCheckData *ds_ptr;  
    unsigned char* c=(unsigned char*)p->data;
    u_long xid, rpcvers, prog, vers, proc;
    enum msg_type direction;
    if (p->iph->ip_proto == IPPROTO_TCP)
    {
        c+=4;
        if (p->dsize<28){
            return 0;
        }
    }else{
        if (p->dsize<24){
            return 0;
        }
    }
    xid = IXDR_GET_LONG (c);
    direction = IXDR_GET_ENUM (c, enum msg_type);
    if (direction != CALL){
        return 0;
    }
    rpcvers = IXDR_GET_LONG (c);
    if (rpcvers != RPC_MSG_VERSION){
        return 0;
    }
    prog = IXDR_GET_LONG (c);
    vers = IXDR_GET_LONG (c);
    proc = IXDR_GET_LONG (c);
    ds_ptr=(RpcCheckData *)otn->ds_list[PLUGIN_RPC_CHECK];
    if (!(ds_ptr->flags & RPC_CHECK_PROG) || ds_ptr->program == prog){
        if (!(ds_ptr->flags & RPC_CHECK_VERS) || ds_ptr->vers == vers){
            if (!(ds_ptr->flags & RPC_CHECK_PROC) || ds_ptr->proc == proc){
		        CManipulate objManip;
				int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
				ptrfun = fp_list->next->OptTestFunc;
				return (objManip.*ptrfun)(p, otn, fp_list->next);
            }
        }
    }
    return 0;
}

void CRpcCheck::ParseRpc(char *data, OptTreeNode *otn)
{
	RpcCheckData *ds_ptr;
    char *tmp;
    ds_ptr = (RpcCheckData*) otn->ds_list[PLUGIN_RPC_CHECK];
    ds_ptr->flags=0;
    while (isspace((int)*data)) data++;
    if (*data != '*'){
        ds_ptr->program = strtoul(data,&tmp,0);
        ds_ptr->flags|=RPC_CHECK_PROG;
    }
    if (*tmp == '\0') return;
    data=++tmp;
    if (*data != '*'){
        ds_ptr->vers = strtoul(data,&tmp,0);
        ds_ptr->flags|=RPC_CHECK_VERS;
    }else{
        tmp++;
    }
    if (*tmp == '\0') return;
    data=++tmp;
    if (*data != '*'){
        ds_ptr->proc = strtoul(data,&tmp,0);
        ds_ptr->flags|=RPC_CHECK_PROC;
    }
}

void CRpcCheck::RpcCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
    otn->ds_list[PLUGIN_RPC_CHECK] = (RpcCheckData *) calloc(sizeof(RpcCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseRpc(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::CheckRpc;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CRpcCheck::SetupRpcCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::RpcCheckInit;
	objPrep->RegisterPlugin("rpc",ptrfun);
}
