// IcmpSeqCheck.cpp: implementation of the CIcmpSeqCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IcmpSeqCheck.h"
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

CIcmpSeqCheck::CIcmpSeqCheck()
{

}

CIcmpSeqCheck::~CIcmpSeqCheck()
{

}

void CIcmpSeqCheck::ParseIcmpSeq(char *data, OptTreeNode *otn)
{
	IcmpSeqData *ds_ptr;  
    ds_ptr = (IcmpSeqData*) otn->ds_list[PLUGIN_ICMP_SEQ_CHECK];
    while (isspace((int)*data)) data++;
    ds_ptr->icmpseq = atoi(data);
}

int CIcmpSeqCheck::IcmpSeqCheck(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (p->icmph->type == ICMP_ECHO || p->icmph->type == ICMP_ECHOREPLY)
    {
        if (((IcmpSeqData *) otn->ds_list[PLUGIN_ICMP_SEQ_CHECK])->icmpseq == p->icmph->ih_seq)
        {
	        CManipulate objManip;
			int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
			ptrfun = fp_list->next->OptTestFunc;
			return (objManip.*ptrfun)(p, otn, fp_list->next);
        }
    }
    return 0;
}


void CIcmpSeqCheck::IcmpSeqCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;

	if (protocol != IPPROTO_ICMP)
    {
//        FatalError("ERROR Line %s (%d): ICMP Options on non-ICMP rule\n", file_name, file_line);
    }

    otn->ds_list[PLUGIN_ICMP_SEQ_CHECK] = (IcmpSeqData *) \
                                          calloc(sizeof(IcmpSeqData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseIcmpSeq(data, otn);

	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::IcmpSeqCheck;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CIcmpSeqCheck::SetupIcmpSeqCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::IcmpSeqCheckInit;
	objPrep->RegisterPlugin("icmp_seq",ptrfun);
}

