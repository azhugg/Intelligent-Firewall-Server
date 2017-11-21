// DSizeCheck.cpp: implementation of the CDSizeCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "DSizeCheck.h"
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

CDSizeCheck::CDSizeCheck()
{

}

CDSizeCheck::~CDSizeCheck()
{

}

int CDSizeCheck::CheckDsizeEq(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (((DsizeCheckData *)otn->ds_list[PLUGIN_DSIZE_CHECK])->dsize == p->dsize)
    {
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

int CDSizeCheck::CheckDsizeGT(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
	if (((DsizeCheckData *)otn->ds_list[PLUGIN_DSIZE_CHECK])->dsize < p->dsize){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
	}
    return 0;
}

int CDSizeCheck::CheckDsizeLT(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
	if (((DsizeCheckData *)otn->ds_list[PLUGIN_DSIZE_CHECK])->dsize > p->dsize){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn, fp_list->next);
    }
    return 0;
}

void CDSizeCheck::ParseDsize(char *data, OptTreeNode *otn)
{
	DsizeCheckData *ds_ptr;
    ds_ptr = (DsizeCheckData *)otn->ds_list[PLUGIN_DSIZE_CHECK];
    while (isspace((int)*data)) data++;
    CRules *objRule;
	objRule = new CRules;
    if (*data == '>'){
        data++;
		int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
		ptrfun = &CManipulate::CheckDsizeGT;
		objRule->AddOptFuncToList(ptrfun, otn);
    }else if(*data == '<'){
        data++;
		int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
		ptrfun = &CManipulate::CheckDsizeLT;
		objRule->AddOptFuncToList(ptrfun, otn);
    }else{
		int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
		ptrfun = &CManipulate::CheckDsizeEq;
		objRule->AddOptFuncToList(ptrfun, otn);
    }
    while (isspace((int)*data)) data++;
    ds_ptr->dsize = atoi(data);
}

void CDSizeCheck::DsizeCheckInit(char *data, _OptTreeNode *otn, int protocol)
{
	CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
    otn->ds_list[PLUGIN_DSIZE_CHECK] = (DsizeCheckData *) calloc(sizeof(DsizeCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseDsize(data, otn);
}

void CDSizeCheck::SetupDsizeCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::DsizeCheckInit;
	objPrep->RegisterPlugin("dsize",ptrfun);
}
