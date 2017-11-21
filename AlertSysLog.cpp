// AlertSysLog.cpp: implementation of the CAlertSysLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "AlertSysLog.h"
#include "StringMan.h"
#include "SysLog.h"
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

CAlertSysLog::CAlertSysLog()
{

}

CAlertSysLog::~CAlertSysLog()
{

}

SyslogData* CAlertSysLog::ParseSyslogArgs(char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	
	char **toks;
    int num_toks;
    int i = 0;

    ptrMainApp->syslog_alert_data = (SyslogData *)malloc(sizeof(SyslogData));

    ptrMainApp->syslog_alert_data->options = 0;
    ptrMainApp->syslog_alert_data->facility = LOG_AUTH;
    ptrMainApp->syslog_alert_data->priority = LOG_INFO;

    if (args == NULL)
    {
        fprintf(stderr, "ERROR %s (%d)=> No arguments to alert_syslog preprocessor!\n", ptrMainApp->file_name, ptrMainApp->file_line);
        exit(1);
    }

	CStringMan *objString;
	objString = new CStringMan;
    toks = objString->mSplit(args, " |", 31, &num_toks, '\\');

    for (i = 0; i < num_toks; i++)
    {

#ifdef LOG_CONS 
        if (!strncasecmp("LOG_CONS", toks[i], 8))
        {
            ptrMainApp->syslog_alert_data->options |= LOG_CONS;
        }
#endif
#ifdef LOG_NDELAY 
        else if (!strncasecmp("LOG_NDELAY", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->options |= LOG_NDELAY;
        }
#endif
#ifdef LOG_PERROR 
        else if (!strncasecmp("LOG_PERROR", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->options |= LOG_PERROR;
        }
#endif
#ifdef LOG_PID 
        else if (!strncasecmp("LOG_PID", toks[i], 7))
        {
            ptrMainApp->syslog_alert_data->options |= LOG_PID;
        }
#endif


#ifdef LOG_AUTH 
        else if (!strncasecmp("LOG_AUTH", toks[i], 8))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_AUTH;
        }
#endif
#ifdef LOG_AUTHPRIV 
        else if (!strncasecmp("LOG_AUTHPRIV", toks[i], 12))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_AUTHPRIV;
        }
#endif
#ifdef LOG_DAEMON 
        else if (!strncasecmp("LOG_DAEMON", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_DAEMON;
        }
#endif
#ifdef LOG_LOCAL0 
        else if (!strncasecmp("LOG_LOCAL0", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL0;
        }
#endif
#ifdef LOG_LOCAL1 
        else if (!strncasecmp("LOG_LOCAL1", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL1;
        }
#endif
#ifdef LOG_LOCAL2 
        else if (!strncasecmp("LOG_LOCAL2", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL2;
        }
#endif
#ifdef LOG_LOCAL3 
        else if (!strncasecmp("LOG_LOCAL3", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL3;
        }
#endif
#ifdef LOG_LOCAL4 
        else if (!strncasecmp("LOG_LOCAL4", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL4;
        }
#endif
#ifdef LOG_LOCAL5 
        else if (!strncasecmp("LOG_LOCAL5", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL5;
        }
#endif
#ifdef LOG_LOCAL6 
        else if (!strncasecmp("LOG_LOCAL6", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL6;
        }
#endif
#ifdef LOG_LOCAL7 
        else if (!strncasecmp("LOG_LOCAL7", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_LOCAL7;
        }
#endif
#ifdef LOG_USER 
        else if (!strncasecmp("LOG_USER", toks[i], 8))
        {
            ptrMainApp->syslog_alert_data->facility = LOG_USER;
        }
#endif


#ifdef LOG_EMERG 
        else if (!strncasecmp("LOG_EMERG", toks[i], 9))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_EMERG;
        }
#endif
#ifdef LOG_ALERT 
        else if (!strncasecmp("LOG_ALERT", toks[i], 9))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_ALERT;
        }
#endif
#ifdef LOG_CRIT 
        else if (!strncasecmp("LOG_CRIT", toks[i], 8))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_CRIT;
        }
#endif
#ifdef LOG_ERR 
        else if (!strncasecmp("LOG_ERR", toks[i], 7))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_ERR;
        }
#endif
#ifdef LOG_WARNING 
        else if (!strncasecmp("LOG_WARNING", toks[i], 11))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_WARNING;
        }
#endif
#ifdef LOG_NOTICE 
        else if (!strncasecmp("LOG_NOTICE", toks[i], 10))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_NOTICE;
        }
#endif
#ifdef LOG_INFO 
        else if (!strncasecmp("LOG_INFO", toks[i], 8))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_INFO;
        }
#endif
#ifdef LOG_DEBUG 
        else if (!strncasecmp("LOG_DEBUG", toks[i], 9))
        {
            ptrMainApp->syslog_alert_data->priority = LOG_DEBUG;
        }
#endif
        else
        {
            fprintf(stderr, "WARNING %s (%d) => Unrecognized syslog facility/priority: %s\n", ptrMainApp->file_name, ptrMainApp->file_line, toks[i]);
        }
    }
       return ptrMainApp->syslog_alert_data;
}

void CAlertSysLog::AlertSyslog(Packet *p, char *msg, void *arg)
{
	char sip[16];
    char dip[16];
	
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;

	CSysLog *objLog;
	objLog = new CSysLog;
       
    if (p == NULL)
    {

		objLog->SysLog(ptrMainApp->syslog_alert_data->priority, "%s",msg==NULL?"ALERT!":msg);
        return;
    }

    strncpy(sip, inet_ntoa(p->iph->ip_src), 16);
    strncpy(dip, inet_ntoa(p->iph->ip_dst), 16);

    if (p->iph->ip_proto == IPPROTO_ICMP || p->frag_flag)
    {
        if (msg != NULL)
        {
            objLog->SysLog(ptrMainApp->syslog_alert_data->priority, "%s: %s -> %s", msg, sip, dip);
        }
        else
        {
			objLog->SysLog(ptrMainApp->syslog_alert_data->priority, "ALERT: %s -> %s", sip, dip);
        }
    }
    else
    {
        if (msg != NULL)
        {
			objLog->SysLog(ptrMainApp->syslog_alert_data->priority, "%s: %s:%i -> %s:%i", msg,
                   sip, p->sp, dip, p->dp);
        }
        else
        {
			objLog->SysLog(ptrMainApp->syslog_alert_data->priority, "ALERT: %s:%i -> %s:%i",
                   sip, p->sp, dip, p->dp);
        }
    }
    return;
}

void CAlertSysLog::AlertSyslogInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CSysLog *objSysLog;
	objSysLog = new CSysLog;

    ptrMainApp->pv.alert_plugin_active = 1;

    ParseSyslogArgs((char *)args);

    objSysLog->OpenLog("ifs", ptrMainApp->syslog_alert_data->options, ptrMainApp->syslog_alert_data->facility);


	CRules *objRule;
	objRule = new CRules;
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
	ptrfun = &CManipulate::AlertSyslog;
    objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_ALERT, ptrMainApp->syslog_alert_data);
	void (CManipulate::*ptrfun2)(int,void*);
	ptrfun2 = &CManipulate::AlertSyslogCleanExitFunc;
	objPrep->AddFuncToCleanExitList(ptrfun2, ptrMainApp->syslog_alert_data);
	ptrfun2 = &CManipulate::AlertSyslogRestartFunc;
	objPrep->AddFuncToRestartList(ptrfun2, ptrMainApp->syslog_alert_data);
}

void CAlertSysLog::AlertSyslogCleanExitFunc(int signal, void *arg)
{
	   CIFServerApp *ptrMainApp;
	   ptrMainApp = PTRAPP;
       free(ptrMainApp->syslog_alert_data);
}

void CAlertSysLog::AlertSyslogRestartFunc(int signal, void *arg)
{
	   CIFServerApp *ptrMainApp;
	   ptrMainApp = PTRAPP;
       free(ptrMainApp->syslog_alert_data);
}

void CAlertSysLog::SetupAlertSyslog()
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	CPreprocessor *objPrep;	
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *args);
	ptrfun = &CManipulate::AlertSyslogInit;
	ptrMainApp->OutputKeywords = NULL;
	objPrep->RegisterOutputPlugin("alert_syslog",NT_OUTPUT_ALERT,ptrfun);
}
