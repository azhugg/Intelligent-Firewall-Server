// IcmpTypeCheck.cpp: implementation of the CIcmpTypeCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IcmpTypeCheck.h"
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

CIcmpTypeCheck::CIcmpTypeCheck()
{

}

CIcmpTypeCheck::~CIcmpTypeCheck()
{

}

int CIcmpTypeCheck::IcmpTypeCheck(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (((IcmpTypeCheckData *) otn->ds_list[PLUGIN_ICMP_TYPE])->icmp_type == p->icmph->type){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

void CIcmpTypeCheck::ParseIcmpType(char *data, OptTreeNode *otn)
{
	char *type;
    IcmpTypeCheckData *ds_ptr;  
    ds_ptr = (IcmpTypeCheckData*) otn->ds_list[PLUGIN_ICMP_TYPE];
    type = data;
    while (isspace((int)*data))
        data++;
    if (isdigit((int)*data)){
        ds_ptr->icmp_type = atoi(data);
        return;
    }else{
//        FatalError( "ERROR Line %s (%d): Bad ICMP type: %s\n", file_name, 
//                    file_line, data);
    }  
}

void CIcmpTypeCheck::IcmpTypeCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	if (protocol != IPPROTO_ICMP)
    {
//        FatalError( "ERROR Line %s (%d): ICMP Options on non-ICMP rule\n", file_name, file_line);
    }
    otn->ds_list[PLUGIN_ICMP_TYPE] = (IcmpTypeCheckData *) calloc(sizeof(IcmpTypeCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseIcmpType(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::IcmpTypeCheck;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CIcmpTypeCheck::SetupIcmpTypeCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::IcmpTypeCheckInit;
	objPrep->RegisterPlugin("itype",ptrfun);
}
