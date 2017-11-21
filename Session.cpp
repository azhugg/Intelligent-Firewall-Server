// Session.cpp: implementation of the CSession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Session.h"
#include "Rules.h"
#include "Preprocessor.h"
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef mkdir
	#define mkdir(x, y) _mkdir(x)
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSession::CSession()
{

}

CSession::~CSession()
{

}

void CSession::ParseSession(char *data, OptTreeNode *otn)
{
	SessionData *ds_ptr;  
    ds_ptr = (SessionData*) otn->ds_list[PLUGIN_SESSION];
    while (isspace((int)*data))
        data++;
    if (!strncasecmp(data, "printable", 9)){
        ds_ptr->session_flag = SESSION_PRINTABLE;
        return;
    }
    if (!strncasecmp(data, "all", 3)){
        ds_ptr->session_flag = SESSION_ALL;
        return;
    }
}

int CSession::LogSessionData(Packet *p, _OptTreeNode *otn, OptFpList *fp_list)
{
    if (p->dsize != 0 && p->data != NULL){
        OpenSessionFile(p);
        if(session != NULL){
           DumpSessionData(session, p, otn);
           fclose(session);
        }else{
//            ErrorMessage("[!] ERROR: Unable to open session file!\n");
        }
    }
	CManipulate objManip;
	int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
	ptrfun = fp_list->next->OptTestFunc;
	return (objManip.*ptrfun)(p, otn, fp_list->next);
}


void CSession::OpenSessionFile(Packet *p)
{
	char log_path[STD_BUF];
    char session_file[STD_BUF]; /* name of session file */
	CIFServerApp* ptrMainApp;
    ptrMainApp =PTRAPP;
    if ((p->iph->ip_proto != IPPROTO_TCP) || (p->frag_flag)){
        session = NULL;
        return;
    }
    bzero((char *)session_file, STD_BUF);
    bzero((char *)log_path, STD_BUF);
    if ((p->iph->ip_dst.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
        if ((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) != ptrMainApp->pv.homenet){
            sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_src));
        }else{
            if ( p->sp >= p->dp ){
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_src));
            }else{
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_dst));
            }
        }
    }else{
        if ((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
            sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_dst));
        }else{
            if (p->sp >= p->dp){
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_src));
            }else{
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_dst));
            }
        }
    }
    if (p->sp >= p->dp){
#ifdef WIN32
		sprintf(session_file, "%s/SESSION_%d-%d", log_path, p->sp, p->dp);
#else
        sprintf(session_file, "%s/SESSION:%d-%d", log_path, p->sp, p->dp);
#endif
	}else{
#ifdef WIN32
        sprintf(session_file, "%s/SESSION_%d-%d", log_path, p->dp, p->sp);
#else
        sprintf(session_file, "%s/SESSION:%d-%d", log_path, p->dp, p->sp);
#endif
    }
    if ((session = fopen(session_file, "a")) == NULL){
//        FatalError("ERROR: OpenSessionFile() => fopen(%s) session file: %s\n", session_file, strerror(errno));
    }
}

void CSession::DumpSessionData(FILE *fp, Packet *p, _OptTreeNode *otn)
{
	u_char *idx;
    u_char *end;
    char conv[] = "0123456789ABCDEF"; 
    if (p->dsize == 0 || p->data == NULL)
        return;
    idx = p->data;
    end = idx + p->dsize;
    if (((SessionData *) otn->ds_list[PLUGIN_SESSION])->session_flag == SESSION_PRINTABLE){
        while (idx != end){
            if ((*idx > 0x1f && *idx < 0x7f) || *idx == 0x0a || *idx == 0x0d){
                fputc(*idx, fp);
            }
            idx++;
        }
    }else{
        while (idx != end){
            if ((*idx > 0x1f && *idx < 0x7f) || *idx == 0x0a || *idx == 0x0d){
                fputc(*idx, fp);
            }else{
                fputc('\\', fp);
                fputc(conv[((*idx&0xFF) >> 4)], fp);
                fputc(conv[((*idx&0xFF)&0x0F)], fp);
            }   idx++;
        }
    }
}


void CSession::SessionInit(char *data, OptTreeNode *otn, int protocol)
{
	otn->ds_list[PLUGIN_SESSION] = (SessionData *) calloc(sizeof(SessionData), sizeof(char));
    ParseSession(data, otn);
	CRules *objRule;
	objRule = new CRules;
	int (CManipulate::*ptrfun)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
	ptrfun = &CManipulate::LogSessionData;
	objRule->AddOptFuncToList(ptrfun, otn);
}

void CSession::SetupSession()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	ptrfun = &CManipulate::SessionInit;
	objPrep->RegisterPlugin("session",ptrfun);
}
