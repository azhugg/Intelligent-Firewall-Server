// TcpAckCheck.cpp: implementation of the CTcpAckCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "TcpAckCheck.h"
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

CTcpAckCheck::CTcpAckCheck()
{

}

CTcpAckCheck::~CTcpAckCheck()
{

}

void CTcpAckCheck::ParseTcpAck(char *data, OptTreeNode *otn)
{
	TcpAckCheckData *ds_ptr;  
    ds_ptr = (TcpAckCheckData*) otn->ds_list[PLUGIN_TCP_ACK_CHECK];
    while (isspace((int)*data)){
        data++;
    }
    ds_ptr->tcp_ack = atoi(data);
}


int CTcpAckCheck::CheckTcpAckEq(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (((TcpAckCheckData *)otn->ds_list[PLUGIN_TCP_ACK_CHECK])->tcp_ack == ntohl(p->tcph->th_ack)){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

void CTcpAckCheck::TcpAckCheckInit(char *data, OptTreeNode *otn, int protocol)
{
	CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	if (protocol != IPPROTO_TCP){
//        FatalError( "ERROR Line %s (%d): TCP Options on non-TCP rule\n", file_name, file_line);
    }
    otn->ds_list[PLUGIN_TCP_ACK_CHECK] = (TcpAckCheckData *) calloc(sizeof(TcpAckCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseTcpAck(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::CheckTcpAckEq;
	objRule->AddOptFuncToList(ptrfun, otn);
}


void CTcpAckCheck::SetupTcpAckCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::TcpAckCheckInit;
	objPrep->RegisterPlugin("ack",ptrfun);
}
