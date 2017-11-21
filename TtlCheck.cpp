// TtlCheck.cpp: implementation of the CTtlCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "TtlCheck.h"
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

CTtlCheck::CTtlCheck()
{

}

CTtlCheck::~CTtlCheck()
{

}

void CTtlCheck::ParseTtl(char *data, OptTreeNode *otn)
{
	TtlCheckData *ds_ptr;  
    ds_ptr = (TtlCheckData*) otn->ds_list[PLUGIN_TTL_CHECK];
    while (isspace((int)*data)) data++;
    ds_ptr->ttl = atoi(data);
}

int CTtlCheck::CheckTtlEq(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (((TtlCheckData *)otn->ds_list[PLUGIN_TTL_CHECK])->ttl == p->iph->ip_ttl){
		CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

void CTtlCheck::TtlCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
    otn->ds_list[PLUGIN_TTL_CHECK] = (TtlCheckData *) calloc(sizeof(TtlCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseTtl(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::CheckTtlEq;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CTtlCheck::SetupTtlCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::TtlCheckInit;
	objPrep->RegisterPlugin("ttl",ptrfun);
}

