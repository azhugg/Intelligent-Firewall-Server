// IcmpIdCheck.cpp: implementation of the CIcmpIdCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IcmpIdCheck.h"
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

CIcmpIdCheck::CIcmpIdCheck()
{

}

CIcmpIdCheck::~CIcmpIdCheck()
{

}

void CIcmpIdCheck::ParseIcmpId(char *data, OptTreeNode *otn)
{
	IcmpIdData *ds_ptr;
    ds_ptr = (IcmpIdData*) otn->ds_list[PLUGIN_ICMP_ID_CHECK];
    while (isspace((int)*data)) data++;
    ds_ptr->icmpid = atoi(data);
    ds_ptr->icmpid = htons(ds_ptr->icmpid);
}


int CIcmpIdCheck::IcmpIdCheck(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
	if (p->icmph->type == ICMP_ECHO || p->icmph->type == ICMP_ECHOREPLY){
        if (((IcmpIdData *) otn->ds_list[PLUGIN_ICMP_ID_CHECK])->icmpid == p->icmph->ih_id){
	        CManipulate objManip;
			int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
			ptrfun = fp_list->next->OptTestFunc;
			return (objManip.*ptrfun)(p, otn, fp_list->next);
        }
    }
    return 0;
}

void CIcmpIdCheck::IcmpIdCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	if (protocol != IPPROTO_ICMP){
//        FatalError("ERROR Line %s (%d): ICMP Options on non-ICMP rule\n", file_name, file_line);
    }
	otn->ds_list[PLUGIN_ICMP_ID_CHECK] = (IcmpIdData *)calloc(sizeof(IcmpIdData), sizeof(char));
    ptrMainApp->StripQuotes(data);

    ParseIcmpId(data, otn);

   	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::IcmpIdCheck;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CIcmpIdCheck::SetupIcmpIdCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::IcmpIdCheckInit;
	objPrep->RegisterPlugin("icmp_id",ptrfun);
}
