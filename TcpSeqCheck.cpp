// TcpSeqCheck.cpp: implementation of the CTcpSeqCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "TcpSeqCheck.h"
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

CTcpSeqCheck::CTcpSeqCheck()
{

}

CTcpSeqCheck::~CTcpSeqCheck()
{

}

void CTcpSeqCheck::ParseTcpSeq(char *data, OptTreeNode *otn)
{
	TcpSeqCheckData *ds_ptr;  
    ds_ptr = (TcpSeqCheckData*) otn->ds_list[PLUGIN_TCP_SEQ_CHECK];
    while (isspace((int)*data)){
        data++;
    }
    ds_ptr->tcp_seq = atoi(data);
}

int CTcpSeqCheck::CheckTcpSeqEq(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
	if (((TcpSeqCheckData *)otn->ds_list[PLUGIN_TCP_SEQ_CHECK])->tcp_seq == ntohl(p->tcph->th_seq)){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}


void CTcpSeqCheck::TcpSeqCheckInit(char *data, OptTreeNode *otn, int protocol)
{
	CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	if (protocol != IPPROTO_TCP){
        fprintf(stderr, "ERROR Line %s (%d): TCP Options on non-TCP rule\n", ptrMainApp->file_name, ptrMainApp->file_line);
        exit(1);
    }

    otn->ds_list[PLUGIN_TCP_SEQ_CHECK] = (TcpSeqCheckData *) calloc(sizeof(TcpAckCheckData), sizeof(char));
	ptrMainApp->StripQuotes(data);
    ParseTcpSeq(data, otn);
    CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::CheckTcpSeqEq;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CTcpSeqCheck::SetupTcpSeqCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::TcpSeqCheckInit;
	objPrep->RegisterPlugin("seq",ptrfun);
}
