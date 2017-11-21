// MinFrag.cpp: implementation of the CMinFrag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "MinFrag.h"
#include "Rules.h"
#include "StringMan.h"
#include "Preprocessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMinFrag::CMinFrag()
{

}

CMinFrag::~CMinFrag()
{

}



void CMinFrag::ProcessMinfragArgs(u_char *args)
{
	char **toks;
    int num_toks;
    int i;

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;

    if (args == NULL)
    {
		CMessage msg;
		msg.uMsg =  "ERROR ";
		msg.uMsg +=	ptrMainApp->file_name;
		msg.uMsg += "(";
		msg.uMsg += ptrMainApp->file_line;
		msg.uMsg += ")=> No Arguments To Minfrag Preprocessor!";
		msg.FatalErrorMessage(msg.uMsg);
    }
	CStringMan objString;
    toks = objString.mSplit((char *)args, " ", 3, &num_toks, '\\');
    for (i = 0; i < num_toks; i++){
        switch (isdigit((int)toks[i][0])){
            case 0: 
                if (!strncasecmp(toks[i], "home", 4)){
					   ptrMainApp->FragData.home_net_only = 1;
                }else if (!strncasecmp(toks[i], "any", 3)){
                    ptrMainApp->FragData.home_net_only = 0;
                }else{
                    CMessage msg;
					msg.uMsg =  "ERROR ";
					msg.uMsg +=	ptrMainApp->file_name;
					msg.uMsg += "(";
					msg.uMsg += ptrMainApp->file_line;
					msg.uMsg += ")=> Unknown minfrag argument: ";
					msg.uMsg += toks[i];
					msg.FatalErrorMessage(msg.uMsg);
                }
                break;
            default:
                ptrMainApp->FragData.minfrag = atoi(toks[i]);
                break;
        } 
    }
}

void CMinFrag::CheckMinfrag(Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CPreprocessor objPre;
	if (!p->frag_flag)
    {
		return;
    }
	ptrMainApp->do_detect = 0;
    if (ptrMainApp->FragData.home_net_only){
        if (!objPre.DestinationIpIsHomenet(p)){
            return;
        }
    }

    if (p->frag_offset == 0){
        if (p->dsize <= ptrMainApp->FragData.minfrag){   
			void (CManipulate::*ptrFunAlert)(Packet*,char*);
			void (CManipulate::*ptrFunLog)(Packet*,char*);
			ptrFunAlert = ptrMainApp->AlertFunc;
			ptrFunLog = ptrMainApp->LogFunc;
			CManipulate objManip;
			(objManip.*ptrFunAlert)(p, MINFRAG_ALERT_MESSAGE);
			(objManip.*ptrFunLog)(p, MINFRAG_ALERT_MESSAGE);
        }
    }
}

void CMinFrag::MinfragInit(u_char *args)
{
	ProcessMinfragArgs(args);
	void (CManipulate::*ptrfun)(Packet*);
	ptrfun = &CManipulate::CheckMinfrag;
	CRules objRule;
	objRule.AddFuncToPreprocList(ptrfun);
}

void CMinFrag::SetupMinfrag()
{
	CPreprocessor objPrep;
	void (CManipulate::*ptrfun)(u_char*);
	ptrfun = &CManipulate::MinfragInit;
	objPrep.RegisterPreprocessor("minfrag",ptrfun);
}

