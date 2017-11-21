// FileManagement.cpp: implementation of the CFileManagement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "FileManagement.h"
#include "Log.h"
#include "Configd.h"
#include "Confige.h" 
#include <stdio.h>
#include "IFServerSubDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileManagement::CFileManagement()
{

}

CFileManagement::~CFileManagement()
{
	
}

FILE* CFileManagement::OpenAlertFile()
{
	CLog *objLog;
	objLog = new CLog;
	return objLog->OpenAlertFile(NULL);
}

void CFileManagement::OpenIDSRulesFile(char *file, CIFServerSubDlg *m_subDlg)
{
	FILE *thefp;       
    char *buf;		
    char *index;    
    CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	char *stored_file_name = ptrMainApp->rules_file;
    int  stored_file_line = ptrMainApp->file_line;
	char *Index;
	CConfigD *ptrDlg;
	CString string;
	ptrDlg = (CConfigD*)m_subDlg;
	buf = new char[STD_BUF];
	Index = new char[STD_BUF];
        ptrMainApp->rules_file = strdup(file);
 
    if ((thefp = fopen(file,"r")) == NULL)
    {
//        FatalError( "ERROR: Unable to open rules file: %s\n", file);
    }

    bzero((char *)buf, STD_BUF);
    stored_file_line = ptrMainApp->file_line;
    stored_file_name = ptrMainApp->rules_file;
    ptrMainApp->rules_file = strdup(file);
    ptrMainApp->file_line = 0;
	ptrDlg->m_List.BringWindowToTop();
    while ((fgets(buf, STD_BUF, thefp)) != NULL){
        Index = buf;
        if ((*Index != '#') && (*Index!= 0x0a) && (*Index!= ';') && (Index != NULL)){
            while (isspace((int) *Index)) Index++;
				ptrMainApp->Strip(Index);
				if(strncmp(Index,"include",7))
					ptrDlg->m_List.AddString(Index);
				else{
					string = Index;
					string = string.Right(string.GetLength()-7);
					string.TrimLeft();
					string.TrimRight();
					bzero((char *)Index, STD_BUF);
					strncpy(Index,string,string.GetLength());
					ptrDlg->m_IncludeList.AddString(Index);
					OpenIncludeFileToVeiw(Index,ptrDlg);
				}
        }
        bzero((char *)buf, STD_BUF);
    }
    fclose(thefp);
    return;
}

void CFileManagement::OpenIncludeFileToVeiw(char *file, CIFServerSubDlg *ptrDlg)
{
	OpenIDSRulesFile(file,ptrDlg);
}

void CFileManagement::LoadAlertFile(char *fp, CIFServerSubDlg *m_subDlg)
{
	FILE *thefp;       
    char *buf;		
    char *index;    
    CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	CConfigE *ptrDlg;
	ptrDlg = (CConfigE*)m_subDlg;
	int ptrpos;
	buf = new char[STD_BUF];
	char *Index;
	Index = new char[STD_BUF];
	CString string;
    if ((thefp = fopen("e:/temp/alert.ids","r")) == NULL)
    {
//        FatalError( "ERROR: Unable to open rules file: %s\n", file);
	}
    bzero((char *)buf, STD_BUF);
	ptrDlg->m_lstAlert.BringWindowToTop();
	ptrDlg->m_lstAlert.ResetContent();
	fseek(thefp,STD_BUF,SEEK_END);
	long curpos= ftell(thefp);
	if (curpos >= 15000){
		fpos_t filepos;
		filepos = curpos - 15000;
		fsetpos(thefp,&filepos);
		fgets(buf,STD_BUF+1024,thefp);
		string = buf;
		while (ptrpos = string.Find("[**]",0)){
				filepos = filepos + 1;
				fsetpos(thefp,&filepos);
				bzero((char *)buf, STD_BUF);
				fgets(buf,STD_BUF,thefp);
				string = buf;
		}if (ptrpos != -1){	
					 filepos = ftell(thefp) - string.GetLength() - 1;
					 fsetpos(thefp,&filepos);
					 ftell(thefp);
				}
	}else fseek(thefp,STD_BUF,SEEK_SET);
    while ((fgets(buf, STD_BUF, thefp)) != NULL){
        Index = buf;
        if ((*Index != '#') && (*Index!= 0x0a) && (*Index!= ';') && (Index != SEEK_SET)){
            while (isspace((int) *Index)) Index++;
				ptrMainApp->Strip(Index);
					ptrDlg->m_lstAlert.AddString(Index);
        }
        bzero((char *)buf, STD_BUF);
    }
    fclose(thefp);
    return;	
}

BOOL CFileManagement::LoadConfigurationFile()
{
	FILE* thefp;
	char* buffer;
	buffer = new char[STD_BUF];
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	CString string;
	char *Index;
	Index = new char[STD_BUF];
		
	if ((thefp = fopen("e:/temp/configuration.dat","r")) == NULL)
    {
//        FatalError( "ERROR: Unable to open rules file: %s\n", file);
	}

    while ((fgets(buffer, STD_BUF, thefp)) != NULL)
    {
        Index = buffer;
		ptrMainApp->Strip(Index);
		string = Index;
        if ((*Index != '#') && (*Index!= 0x0a) && (*Index!= ';') && (Index != SEEK_SET)){
            while (isspace((int) *Index)) Index++;
			if(!strncmp(Index,"[BloodHoundSen]",15)){
					string = string.Right(string.GetLength()-16);
					ptrMainApp->pv.critical_sensitivity  = atoi(string);
			}else if (!strncmp(Index,"[RulesDirectory]",16)){
					string = string.Right(string.GetLength()-17);
					sprintf(ptrMainApp->pv.rules_directory, "%s",string);
			}else if (!strncmp(Index,"[RulesFile]",11)){
					string = string.Right(string.GetLength()-12);
					sprintf(ptrMainApp->pv.main_rules_file, "%s",string);					
			}else if (!strncmp(Index,"[LogDirectory]",14)){
					string = string.Right(string.GetLength()-15);
			}else if (!strncmp(Index,"[DenyListFile]",14)){
					string = string.Right(string.GetLength()-15);
					sprintf(ptrMainApp->pv.deny_list_file, "%s",string);
			}
        }
        bzero((char *)buffer, STD_BUF);
    }
    fclose(thefp);
return TRUE;
}

BOOL CFileManagement::SaveConfigurationFile()
{
	FILE* thefp;
	char* buffer;
	buffer = new char[STD_BUF];
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	if ((thefp = fopen("e:/temp/configuration.dat","w+")) == NULL)
    {
//        FatalError( "ERROR: Unable to open configuration file: %s\n", file);
	}
	sprintf(buffer,"[BloodHoundSen]=%d\n",ptrMainApp->pv.critical_sensitivity);
	fputs(buffer,thefp);
	sprintf(buffer,"[RulesDirectory]=%s\n",ptrMainApp->pv.rules_directory);
	fputs(buffer,thefp);
	sprintf(buffer,"[RulesFile]=%s\n",ptrMainApp->pv.main_rules_file);
	fputs(buffer,thefp);
	sprintf(buffer,"[LogDirectory]=%s\n",ptrMainApp->pv.log_dir);
	fputs(buffer,thefp);
	sprintf(buffer,"[DenyListFile]=%s\n",ptrMainApp->pv.deny_list_file);
	fputs(buffer,thefp);
	fclose(thefp);
	delete(buffer);
return TRUE;
}
