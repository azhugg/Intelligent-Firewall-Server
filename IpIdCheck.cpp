// IpIdCheck.cpp: implementation of the CIpIdCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IpIdCheck.h"
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

CIpIdCheck::CIpIdCheck()
{

}

CIpIdCheck::~CIpIdCheck()
{

}

int CIpIdCheck::IpIdCheckEq(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
	if (((IpIdData *)otn->ds_list[PLUGIN_IP_ID_CHECK])->ip_id == ntohs(p->iph->ip_id))
    {
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

void CIpIdCheck::ParseIpId(char *data, OptTreeNode *otn)
{
	IpIdData *ds_ptr;  
    ds_ptr = (IpIdData*) otn->ds_list[PLUGIN_IP_ID_CHECK];
    while (isspace((int)*data)){
        data++;
    }
    ds_ptr->ip_id = atoi(data);
}

void CIpIdCheck::IpIdCheckInit(char *data, OptTreeNode *otn, int protocol)
{
	CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
    otn->ds_list[PLUGIN_IP_ID_CHECK] = (IpIdData *) calloc(sizeof(IpIdData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseIpId(data, otn);
    CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::IpIdCheckEq;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CIpIdCheck::SetupIpIdCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::IpIdCheckInit;
	objPrep->RegisterPlugin("id",ptrfun);
}
