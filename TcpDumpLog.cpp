// TcpDumpLog.cpp: implementation of the CTcpDumpLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "TcpDumpLog.h"
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

CTcpDumpLog::CTcpDumpLog()
{

}

CTcpDumpLog::~CTcpDumpLog()
{

}

void CTcpDumpLog::TcpdumpInitLogFile(LogTcpdumpData *data)
{
	time_t curr_time;      
    struct tm *loc_time;   
    char timebuf[10];
    char logdir[STD_BUF];
    bzero(logdir, STD_BUF);
    bzero(timebuf, 10);
    curr_time = time(NULL);
    loc_time = localtime(&curr_time);
    strftime(timebuf,91,"%m%d@%H%M",loc_time);
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if ((strlen(ptrMainApp->TcpdumpLogInfo->filename)+2+strlen(ptrMainApp->pv.log_dir)+strlen(timebuf)) > STD_BUF){
//        FatalError("ERROR: log file logging path and file name are too long, aborting!\n");
    }

    sprintf(logdir,"%s/%s-%s", ptrMainApp->pv.log_dir, timebuf, ptrMainApp->TcpdumpLogInfo->filename);

    if ((ptrMainApp->TcpdumpLogInfo->dumpd=pcap_dump_open(ptrMainApp->pd,logdir)) == NULL){
//        FatalError("log_tcpdump TcpdumpInitLogFile(): %s", strerror(errno));
    }
    return;
}

void CTcpDumpLog::LogTcpdump(Packet *p, char *msg, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    pcap_dump((u_char *)ptrMainApp->TcpdumpLogInfo->dumpd,p->pkth,p->pkt);
    fflush((FILE *)ptrMainApp->TcpdumpLogInfo->dumpd);
}

LogTcpdumpData* CTcpDumpLog::ParseTcpdumpArgs(char *args)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	ptrMainApp->TcpdumpLogInfo = (LogTcpdumpData *)malloc(sizeof(LogTcpdumpData));
    if (args != NULL){
        while (isspace((int)*args)) args++;
        ptrMainApp->TcpdumpLogInfo->filename = (char *) calloc(strlen(args) + 1, sizeof(char));  
        if (ptrMainApp->TcpdumpLogInfo->filename == NULL){
//            FatalError("ParseTcpdumpArgs() filename calloc failed: %s\n", strerror(errno));
        }
        strncpy(ptrMainApp->TcpdumpLogInfo->filename, args, strlen(args));
    }else{
        ptrMainApp->TcpdumpLogInfo->filename = (char *) calloc(strlen("ifs.log")+1, sizeof(char));
        strncpy(ptrMainApp->TcpdumpLogInfo->filename, "ifs.log", strlen("ifs.log"));
    }
       return ptrMainApp->TcpdumpLogInfo;
}

void CTcpDumpLog::SpoLogTcpdumpCleanExitFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	pcap_dump_close(ptrMainApp->TcpdumpLogInfo->dumpd);
    free(ptrMainApp->TcpdumpLogInfo->filename);
    free(ptrMainApp->TcpdumpLogInfo);
}


void CTcpDumpLog::SpoLogTcpdumpRestartFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	pcap_dump_close(ptrMainApp->TcpdumpLogInfo->dumpd);
    free(ptrMainApp->TcpdumpLogInfo->filename);
    free(ptrMainApp->TcpdumpLogInfo->dumpd);
    free(ptrMainApp->TcpdumpLogInfo);
}

void CTcpDumpLog::LogTcpdumpInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    ptrMainApp->pv.log_plugin_active = 1;
    ParseTcpdumpArgs((char *)args);
    TcpdumpInitLogFile(ptrMainApp->TcpdumpLogInfo);
	CRules *objRule;
	objRule = new CRules;
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
	ptrfun = &CManipulate::LogTcpdump;
    objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_LOG, ptrMainApp->TcpdumpLogInfo);
	void (CManipulate::*ptrfun2)(int,void*);
	ptrfun2 = &CManipulate::SpoLogTcpdumpCleanExitFunc;
	objPrep->AddFuncToCleanExitList(ptrfun2, ptrMainApp->TcpdumpLogInfo);
	ptrfun2 = &CManipulate::SpoLogTcpdumpRestartFunc;
	objPrep->AddFuncToRestartList(ptrfun2, ptrMainApp->TcpdumpLogInfo);
}

void CTcpDumpLog::SetupTcpDumpLog()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *);
	ptrfun = &CManipulate::LogTcpdumpInit;
	objPrep->RegisterOutputPlugin("log_tcpdump",NT_OUTPUT_LOG,ptrfun);
}
