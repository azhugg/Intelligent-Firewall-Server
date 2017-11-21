// IcmpCodeCheck.cpp: implementation of the CIcmpCodeCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IcmpCodeCheck.h"
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

CIcmpCodeCheck::CIcmpCodeCheck()
{

}

CIcmpCodeCheck::~CIcmpCodeCheck()
{

}

int CIcmpCodeCheck::IcmpCodeCheck(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (((IcmpCodeCheckData *) otn->ds_list[PLUGIN_ICMP_CODE])->icmp_code == p->icmph->code){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

void CIcmpCodeCheck::ParseIcmpCode(char *data, OptTreeNode *otn)
{
	char *code;
    IcmpCodeCheckData *ds_ptr;
    ds_ptr = (IcmpCodeCheckData *)otn->ds_list[PLUGIN_ICMP_CODE];
    code = data;
    while (isspace((int)*data))
        data++;
    if (isdigit((int)*data)){
        ds_ptr->icmp_code = atoi(data);
        return;
    }
}

void CIcmpCodeCheck::IcmpCodeCheckInit(char *data, OptTreeNode *otn, int protocol)
{
	CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	if (protocol != IPPROTO_ICMP){
      //  FatalError( "ERROR Line %s (%d): ICMP Options on non-ICMP rule\n", file_name, file_line);
    }
    otn->ds_list[PLUGIN_ICMP_CODE] = (IcmpCodeCheckData *) calloc(sizeof(IcmpCodeCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseIcmpCode(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::IcmpCodeCheck;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CIcmpCodeCheck::SetupIcmpCodeCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::IcmpCodeCheckInit;
	objPrep->RegisterPlugin("icmp_code",ptrfun);
}
