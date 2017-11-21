// IpOptionCheck.cpp: implementation of the CIpOptionCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IpOptionCheck.h"
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

CIpOptionCheck::CIpOptionCheck()
{

}

CIpOptionCheck::~CIpOptionCheck()
{

}

void CIpOptionCheck::ParseIpOptionData(char *data, OptTreeNode *otn)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	IpOptionData *ds_ptr;  
    ds_ptr = (IpOptionData*) otn->ds_list[PLUGIN_IPOPTION_CHECK];
    if (data == NULL){
        fprintf(stderr, "ERROR %s (%d) => IP Option keyword missing argument!\n", ptrMainApp->file_name, ptrMainApp->file_line);
        exit(1);
    }
    while (isspace((u_char)*data))
        data++; 
    if (!strncasecmp(data, "rr", 2)){
        ds_ptr->ip_option = IPOPT_RR;
        return;
    }
    else if (!strncasecmp(data, "eol", 3)){
        ds_ptr->ip_option = IPOPT_EOL;
        return;
    }else if (!strncasecmp(data, "nop", 3)){
        ds_ptr->ip_option = IPOPT_NOP;
        return;
    }else if (!strncasecmp(data, "ts", 2)){
        ds_ptr->ip_option = IPOPT_TS;
        return;
    }else if (!strncasecmp(data, "sec", 3)){
        ds_ptr->ip_option = IPOPT_SECURITY;
        return;
    }else if (!strncasecmp(data, "lsrr", 4)){
        ds_ptr->ip_option = IPOPT_LSRR;
        return;
    }else if (!strncasecmp(data, "lsrre", 5)){
        ds_ptr->ip_option = IPOPT_LSRR_E;
        return;
    }else if (!strncasecmp(data, "satid", 5)){
        ds_ptr->ip_option = IPOPT_SATID;
        return;
    }else if (!strncasecmp(data, "ssrr", 3)){
        ds_ptr->ip_option = IPOPT_SSRR;
        return;
    }else{
        fprintf(stderr, "ERROR %s (%d) => Unknown IP option argument: %s!\n", ptrMainApp->file_name, ptrMainApp->file_line, data);
        exit(1);
    }
}

int CIpOptionCheck::CheckIpOptions(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
	int i;
    for (i=0; i<p->ip_option_count; i++){
        if (((IpOptionData *)otn->ds_list[PLUGIN_IPOPTION_CHECK])->ip_option == p->ip_options[i].code)
        {
	        CManipulate objManip;
			int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
			ptrfun = fp_list->next->OptTestFunc;
			return (objManip.*ptrfun)(p, otn, fp_list->next);
        }
    }
    return 0;
}

void CIpOptionCheck::IpOptionInit(char *data, OptTreeNode *otn, int protocol)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    otn->ds_list[PLUGIN_IPOPTION_CHECK] = (IpOptionData *) calloc(sizeof(IpOptionData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseIpOptionData(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::CheckIpOptions;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CIpOptionCheck::SetupIpOptionCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::IpOptionInit;
	objPrep->RegisterPlugin("ipopts",ptrfun);
}

