// AlertSMB.cpp: implementation of the CAlertSMB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "AlertSMB.h"
#include "Rules.h"
#include "Preprocessor.h"
#include "StringMan.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlertSMB::CAlertSMB()
{

}

CAlertSMB::~CAlertSMB()
{
//	SpoAlertSmbData *smb_alert_data;
}

void CAlertSMB::SpoAlertSmb(Packet *p, char *msg, void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;

    char command_line[2048];
    FILE *output;
    FILE *workstations;
    char *tempmsg;
    char tempwork[STD_BUF];
    char timestamp[23];
    int msg_str_size;
/*    SpoAlertSmbData *data = (SpoAlertSmbData *)arg;*/

    bzero((char *)timestamp, 23);

    ptrMainApp->Ts_Print((struct timeval *)(p==NULL?NULL:&p->pkth->ts), timestamp);

	
    msg_str_size = strlen(msg) + 32 + 10 + 150;

    if((tempmsg = (char *) calloc(msg_str_size, sizeof(char))) == NULL)
    {
//        FatalError("[!] ERROR: SmbAlert() unable to allocate space for tempmsg: %s\n", strerror(errno));
    }


    if ((workstations = fopen(ptrMainApp->smb_alert_data->filename,"r")) != NULL)
    {
        if(p != NULL)
        {
            if(p->frag_flag || p->iph->ip_proto)
            {
                sprintf(tempmsg, "IFS ALERT - Possible Network Attack or Probe:\n [**] %s [**]\n%s %s->%s", msg, timestamp, inet_ntoa(p->iph->ip_src), inet_ntoa(p->iph->ip_dst));
            }
            else
            {
                sprintf(tempmsg, "IFS ALERT - Possible Network Attack or Probe:\n [**] %s [**]\n%s %s:%d->%s:%d", msg, timestamp, inet_ntoa(p->iph->ip_src), p-> sp, inet_ntoa(p->iph->ip_dst), p->dp);
            }
        }
        else
        {

            sprintf(tempmsg, "IFS ALERT - Possible Network Attack or Probe:\n [**] %s [**]\n", msg);
        }

        bzero((char *)tempwork, STD_BUF);
        bzero((char *)command_line, 2048);


        while ((fgets(tempwork, STD_BUF-1, workstations)) != NULL)
        {
            if (tempwork[0] != 0x0A)
            {
                ptrMainApp->Strip(tempwork);

                if(strlen(tempmsg) + strlen(tempwork) + 50 < 2048)
                {
                   sprintf(command_line, "echo \"%s\" | smbclient -U IFS -M %s", tempmsg, tempwork);

                   output = _popen(command_line,"r");

                   _pclose(output);
                }
                else
                {
//                   ErrorMessage("[!] WARNING: Unable to send alert to %s, command buffer size exceeded!\n", tempwork);
                }

                bzero((char *)tempwork, STD_BUF);
                bzero((char *)command_line, 2048);
            }
        }

        fclose(workstations);
    }

    free(tempmsg);
}

void CAlertSMB::SpoAlertSmbCleanExitFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
       free(ptrMainApp->smb_alert_data->filename);
       free(ptrMainApp->smb_alert_data);
}

void CAlertSMB::SpoAlertSmbRestartFunc(int signal, void *arg)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
       free(ptrMainApp->smb_alert_data->filename);
       free(ptrMainApp->smb_alert_data);
}


SpoAlertSmbData* CAlertSMB::ParseAlertSmbArgs(char *args)
{
    CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	
	ptrMainApp->smb_alert_data = (SpoAlertSmbData *) malloc(sizeof(SpoAlertSmbData));

	if (args != NULL)
	{
		while(isspace((int)*args)) args++;
              ptrMainApp->smb_alert_data->filename = (char *) calloc(strlen(args) + 1, sizeof(char));
		if(ptrMainApp->smb_alert_data->filename == NULL)
		{
//			FatalError("ParseAlertSmbArgs() filename calloc failed: %s\n",
//					strerror(errno));
		}
		
		strncpy(ptrMainApp->smb_alert_data->filename, args, strlen(args));
	}
	else
	{
//		FatalError("Error line %s (%d): No filename for AlertSmb\n",
//				file_name, file_line);
	}

    return ptrMainApp->smb_alert_data;
}

void CAlertSMB::AlertSmbInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;

	ptrMainApp->pv.alert_plugin_active = 1;

    ParseAlertSmbArgs((char *)args);

	CRules *objRule;
	objRule = new CRules;
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
	ptrfun = &CManipulate::SpoAlertSmb;
    objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_ALERT, ptrMainApp->smb_alert_data);
	void (CManipulate::*ptrfun2)(int,void*);
	ptrfun2 = &CManipulate::SpoAlertSmbCleanExitFunc;
	objPrep->AddFuncToCleanExitList(ptrfun2, ptrMainApp->smb_alert_data);
	ptrfun2 = &CManipulate::SpoAlertSmbRestartFunc;
    objPrep->AddFuncToRestartList(ptrfun2, ptrMainApp->smb_alert_data);
}

void CAlertSMB::SetupAlertSMB()
{
	CPreprocessor *objPrep;	
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *args);
	ptrfun = &CManipulate::AlertSmbInit;
	objPrep->RegisterOutputPlugin("alert_smb", NT_OUTPUT_ALERT,ptrfun);
}
