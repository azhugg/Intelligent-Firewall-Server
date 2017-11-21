// TcpFlagCheck.cpp: implementation of the CTcpFlagCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "TcpFlagCheck.h"
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

CTcpFlagCheck::CTcpFlagCheck()
{

}

CTcpFlagCheck::~CTcpFlagCheck()
{

}



void CTcpFlagCheck::ParseTCPFlags(char *rule, OptTreeNode *otn)
{
	char *fptr;
    char *fend;
    TCPFlagCheckData *idx;
    idx = (TCPFlagCheckData*) otn->ds_list[PLUGIN_TCP_FLAG_CHECK];
    fptr = rule;
    while (!isalnum((u_char) *fptr))
        fptr++;
    fend = fptr + strlen(fptr); 
    while (fptr < fend){
        switch ((*fptr&0xFF)){
            case 'f':
            case 'F':
                idx->tcp_flags = (idx->tcp_flags | R_FIN);
                break;
            case 's':
            case 'S':
                idx->tcp_flags = (idx->tcp_flags | R_SYN);
                break;
            case 'r':
            case 'R':
                idx->tcp_flags = (idx->tcp_flags | R_RST);
                break;
            case 'p':
            case 'P':
                idx->tcp_flags = (idx->tcp_flags | R_PSH);
                break;
            case 'a':
            case 'A':
                idx->tcp_flags = (idx->tcp_flags | R_ACK);
                break;
            case 'u':
            case 'U':
                idx->tcp_flags = (idx->tcp_flags | R_URG);
                break;
            case '0':
                idx->tcp_flags = 0;
                break;
            case '1': 
                idx->tcp_flags = (idx->tcp_flags | R_RES1);
                break;
            case '2': 
                idx->tcp_flags = (idx->tcp_flags | R_RES2);
                break;
            default:
                CIFServerApp* ptrMainApp;
				ptrMainApp =PTRAPP;
				fprintf(stderr, "ERROR Line %s (%d): bad TCP flag = \"%c\"\n", ptrMainApp->file_name, ptrMainApp->file_line, *fptr);
                fprintf(stderr, " Valid otions: UAPRSF12 or 0 for NO flags (e.g. NULL scan)\n");
                exit(1);
        }

        fptr++;
    }
}

int CTcpFlagCheck::CheckTcpFlagsEq(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list)
{
    TCPFlagCheckData *flagptr;
    if(p->tcph == NULL)
       return 0;
    flagptr = (TCPFlagCheckData*) otn_idx->ds_list[PLUGIN_TCP_FLAG_CHECK];
    if (flagptr->tcp_flags == p->tcph->th_flags){
        CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
		return (objManip.*ptrfun)(p, otn_idx, fp_list->next);
    }
    return 0;
}


void CTcpFlagCheck::TCPFlagCheckInit(char *data, OptTreeNode *otn, int protocol)
{
    CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	if (protocol != IPPROTO_TCP){
        fprintf(stderr, "ERROR Line %s (%d): TCP Options on non-TCP rule\n", ptrMainApp->file_name, ptrMainApp->file_line);
		char *str;
		str = new char[256];
		int lineno = ptrMainApp->file_line;
		sprintf(str,"Problem in Rules File Line No: %i TCP Options on non-TCP rule",lineno);
		AfxMessageBox(str);
        exit(1);
    }
    otn->ds_list[PLUGIN_TCP_FLAG_CHECK] = (TCPFlagCheckData *)calloc(sizeof(TCPFlagCheckData), sizeof(char));
    ptrMainApp->StripQuotes(data);
    ParseTCPFlags(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::CheckTcpFlagsEq;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CTcpFlagCheck::SetupTCPFlagCheck()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::TCPFlagCheckInit;
	objPrep->RegisterPlugin("flags",ptrfun);
}

