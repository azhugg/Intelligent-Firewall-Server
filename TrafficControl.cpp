// TrafficControl.cpp: implementation of the CTrafficControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "TrafficControl.h"
#include "Log.h"
#include "AlertScreenDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FAILED		1
#define SUCCESS		2

#include "iphlpapi.h"


VOID CALLBACK BProcessorProc(HWND hwnd,UINT message,UINT idTimer,DWORD dwTime);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrafficControl::CTrafficControl()
{
	m_ptrMainApp = PTRAPP;
}

CTrafficControl::~CTrafficControl()
{

}

short CTrafficControl::GetOutputWidth()
{
	HANDLE Stdout_Handle;
	CONSOLE_SCREEN_BUFFER_INFO Screen_Infos;
	BOOL status;
	short width = 80; 
	Stdout_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Stdout_Handle == INVALID_HANDLE_VALUE)
		return width;
	status = GetConsoleScreenBufferInfo(Stdout_Handle, &Screen_Infos);
	if (status == 0)
		return width;
	return Screen_Infos.dwSize.X;
}

void CTrafficControl::DisplaySynError(char *filter, unsigned char pos)
{
	unsigned char pad_width = pos;
	unsigned short msg_width;
	unsigned short output_width;
	unsigned short chars_before;
	char *one_line_format_str = "%%s%%s%\n%%%ds\n";
	char *multi_lines_format_str = "%%s%%.%ds\n%%%ds\n%%s\n";
	char format_str_expanded[42];
	output_width = GetOutputWidth();
	msg_width = SYN_ERROR_MSG_LEN + strlen(filter);
	if (output_width > msg_width) {
		_snprintf(format_str_expanded, 42,
				one_line_format_str,
				pad_width +
				SYN_ERROR_MSG_LEN + 1);
		printf(format_str_expanded,
				SYN_ERROR_MSG, filter,
				"^");
	}else {
		chars_before = pad_width +
			(output_width -
			 ((SYN_ERROR_MSG_LEN +
			   pad_width) % output_width));
		_snprintf(format_str_expanded, 42,
				multi_lines_format_str,
				chars_before, 
				output_width - 
				(chars_before -
				 pad_width) +1);
		if (chars_before < strlen(filter)) 
			printf(format_str_expanded,
					SYN_ERROR_MSG,
					filter, "^", filter +
					chars_before);
		else
			printf(format_str_expanded,
					SYN_ERROR_MSG,
					filter, "^", "");
	}
}

IP_ADAPTER_INFO* CTrafficControl::ListIpInterfaces()
{
	IP_ADAPTER_INFO *adapt_info;
	IP_ADAPTER_INFO *adapt_return;
	DWORD status;
	ULONG size;
	UCHAR count = 0;
	size = sizeof(IP_ADAPTER_INFO);
	adapt_info = (IP_ADAPTER_INFO *) malloc(size);
	status = GetAdaptersInfo(adapt_info, &size);
	adapt_return = adapt_info;
	if (status != ERROR_SUCCESS) {
		free(adapt_info);
		adapt_info = (IP_ADAPTER_INFO *) malloc(size);
		status = GetAdaptersInfo(adapt_info, &size);
		adapt_return = adapt_info;
	}
	while (adapt_info){
		adapt_info = adapt_info->Next;
	}
	return adapt_return;
}

char CTrafficControl::WriteFilter(char *filter)
{
	DWORD read;
	DWORD written;
	BOOL bool;
	HANDLE my_pipe;
	char status_code; 
	char syntax_error;
	u_char *msg_buf = NULL;
	u_int msg_buf_size;
	CString string;
	
	bool = WaitNamedPipe(THE_PIPE, NMPWAIT_USE_DEFAULT_WAIT);
	if (!bool) {
		fprintf(stderr, "error: unable to connect to %s\n", THE_PIPE);
		fprintf(stderr, "IFS Packet Filtering service is probably not running\n");
		exit(1);
	}
	my_pipe = CreateFile(THE_PIPE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (my_pipe == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "error: unable to read from %s\n", THE_PIPE);
		exit(1);
	}

	bool = WriteFile(my_pipe, filter, strlen(filter), &written, NULL);
	if (!bool) {
		fprintf(stderr, "error: unable to write to %s\n", THE_PIPE);
		fprintf(stderr, "IFS Packet Filtering service is probably not running\n");
		exit(1);
	}
	bool = ReadFile(my_pipe, &status_code, 1, &read, NULL);
	if (!bool || read != 1) {
		fprintf(stderr, "error: unable to read from %s\n", THE_PIPE);
		exit(1);
	}

	if (status_code == FILTER_FAILURE) {
		bool = ReadFile(my_pipe, &syntax_error, 1, &read, NULL);
		if (!bool || read != 1) {
			fprintf(stderr, "error: unable to read from %s\n", THE_PIPE);
			exit(1);
		}
		DisplaySynError(filter, syntax_error);
		m_ptrMainApp->pv.flush_failure = FAILED;
		return FILTER_FAILURE;
	}
	if (status_code == FILTER_MESSAGE) {
		bool = ReadFile(my_pipe, &msg_buf_size, sizeof(msg_buf_size),
				&read, NULL);
		if (!bool || read != sizeof(msg_buf_size)) {
			fprintf(stderr, "error: unable to read from %s\n", THE_PIPE);
			exit(1);
		}
		msg_buf = (u_char *) malloc((msg_buf_size + 1)* sizeof(char));
		memset(msg_buf, 0, (msg_buf_size + 1));
		bool = ReadFile(my_pipe, msg_buf, msg_buf_size, &read, NULL);
		if (!bool || read != msg_buf_size) {
			fprintf(stderr, "error: unable to read from %s\n", THE_PIPE);
			exit(1);
		}
		printf("%s", msg_buf);
		string = filter;
		if(string.Find("Stats on",0) != -1)
		{
			m_ptrMainApp->m_DenyStatus = msg_buf; 
		}
		if(string.Find("List on",0) != -1)
		{
			string = msg_buf;
			m_ptrMainApp->m_DenyList = string;
		}
	
		return FILTER_MESSAGE;
	}

	if (status_code != FILTER_SUCCESS) 
		fprintf(stderr, "warning: unknown status code\n");
	return FILTER_SUCCESS;
}

void CTrafficControl::BriefStatsOnInterface(char *iface)
{
	char stats_msg[32] = "Stats on ";
	
	strncat(stats_msg, iface, 32 - strlen(stats_msg));	
	
	WriteFilter(stats_msg);
}

void CTrafficControl::DetailedStatsOnInterface(char *iface)
{
	char stats_msg[32] = "Stats on ";
	
	strncat(stats_msg, iface, 32 - strlen(stats_msg));	
	
	WriteFilter(stats_msg);
}

void CTrafficControl::FlushInterface(char *iface)
{
	char flush_msg[32] = "flush on ";
	
	strncat(flush_msg, iface, 32 - strlen(flush_msg));	

	if(WriteFilter(flush_msg) != FILTER_FAILURE)
	{
		m_ptrMainApp->pv.flush_failure = SUCCESS;
	}

}

void CTrafficControl::ListRulesOnInterface(char *iface)
{
	char list_msg[32] = "list on ";

	strncat(list_msg, iface, 32 - strlen(list_msg));	

	WriteFilter(list_msg);
}

void CTrafficControl::NumberedListRulesOnInterface(char *iface)
{
	char list_msg[32] = "List on ";

	strncat(list_msg, iface, 32 - strlen(list_msg));	

	WriteFilter(list_msg);
}

char CTrafficControl::ReadFiltersFromFile(char *filename)
{
	FILE *filters;
	char line[MAX_RULE_LENGTH]; 
	char success;
	
	filters = fopen(filename, "r");
	if (filters == NULL) {
		fprintf(stderr,"error: could not open filters file %s\n",
				filename);
		return -1;
	}
	while (fgets(line, MAX_RULE_LENGTH, filters) != NULL) 
	{
		if (*line != '#') 
			success = WriteFilter(line);
		else 
			continue;
		if (success == FILTER_FAILURE)
			break;
	}
	fclose(filters);
	return success;
}

char CTrafficControl::StartUpdateListShell()
{
	char rule_buf[MAX_RULE_LENGTH];
	char *word;
	char *p;
	char *ret;
	
	while (1){
		memset(rule_buf, 0, MAX_RULE_LENGTH);
		ret = fgets(rule_buf, MAX_RULE_LENGTH, stdin);
		if (ret == NULL){
			fprintf(stderr, "error: rule too long\n");
			continue;
		}
		if ((strncmp(rule_buf, "help", 4) == 0)|| (strncmp(rule_buf, "h\n", 2) == 0)){
			continue;
		}
		if (strncmp(rule_buf, "source", 6) == 0){
			word = rule_buf + 6;
			while ((word < (rule_buf + MAX_RULE_LENGTH)) && (*word == ' '))
				word++;
			p = word;
			while ((word < (rule_buf + MAX_RULE_LENGTH)) && (*word != '\n'))
				word++;
			*word = 0;
			ReadFiltersFromFile(p);
			continue;
		}
		if (strncmp(rule_buf, "reload", 6) == 0) {
			FlushInterface("all");
			word = rule_buf + 6;
			while ((word < (rule_buf + MAX_RULE_LENGTH)) && (*word == ' '))
				word++;
			p = word;
			while ((word < (rule_buf + MAX_RULE_LENGTH)) && (*word != '\n'))
				word++;
			*word = 0;
			ReadFiltersFromFile(p);
			continue;
		}
		if (strncmp(rule_buf, "q\n", 2) == 0)
			return 1;
		if (strncmp(rule_buf, "quit", 4) == 0)
		       return 1;
		if (strncmp(rule_buf, "exit", 4) == 0)
		       return 1;
		if (strncmp(rule_buf, "\004\n", 2) == 0)
			return 1;
		if (strncmp(rule_buf, "x\n", 2) == 0)
			return 1;
		if (strcmp(rule_buf, "\n") == 0) 
			continue;
		WriteFilter(rule_buf);
	}
}


void CTrafficControl::ActivateBlockingProcessor()
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	ptrMainApp->nResult = ::SetTimer(NULL,0,5000,(TIMERPROC) BProcessorProc);
}

VOID CALLBACK BProcessorProc(HWND hwnd,UINT message,UINT idTimer,DWORD dwTime)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	AlertNode *alert_idx;
	CriticalClientList *idx;
	AccessList *access_idx;
	CTrafficControl *ptrTControl;
	ptrTControl = new CTrafficControl;
	access_idx = ptrMainApp->accesslist;
if(access_idx != NULL)
	for(int loop = 0;loop <= MAX_CLIENTS; loop++){
		if(access_idx->TCPList->ClientQueue[loop] != NULL){
			alert_idx  =  access_idx->TCPList->ClientQueue[loop];
			while(alert_idx != NULL){
				if(alert_idx->next != NULL)
					alert_idx = alert_idx->next;
				else
					break;
			}
			if(alert_idx->Weight >= ptrMainApp->pv.critical_sensitivity)
				if(ptrTControl->AddToDenyList(IPPROTO_TCP, loop))
					ptrTControl->FreeNode(access_idx->TCPList->ClientQueue[loop],loop,IPPROTO_TCP);
							
		}
		if(access_idx->UDPList->ClientQueue[loop] != NULL){
			alert_idx  =  access_idx->UDPList->ClientQueue[loop];
			while(alert_idx != NULL){
				if(alert_idx->next != NULL)
					alert_idx = alert_idx->next;
				else
					break;
			}		
			if(alert_idx->Weight >= ptrMainApp->pv.critical_sensitivity)
				if(ptrTControl->AddToDenyList(IPPROTO_UDP, loop))
					ptrTControl->FreeNode(access_idx->UDPList->ClientQueue[loop],loop,IPPROTO_UDP);
		}
		if(access_idx->ICMPList->ClientQueue[loop] != NULL){
			alert_idx  =  access_idx->ICMPList->ClientQueue[loop];
			while(alert_idx != NULL){
				if(alert_idx->next != NULL)
					alert_idx = alert_idx->next;
				else
					break;
			}

			if(alert_idx->Weight >= ptrMainApp->pv.critical_sensitivity)
				if(ptrTControl->AddToDenyList(IPPROTO_ICMP, loop))
					ptrTControl->FreeNode(access_idx->ICMPList->ClientQueue[loop],loop,IPPROTO_ICMP);
		}

	}
	free(ptrTControl);
}

BOOL CTrafficControl::AddToDenyList(int protocol, int Node)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	char *net_address;
	char *rule_string;
	rule_string = new char[STD_BUF];
	CAlertScreenDlg ptrscreen;
	switch(protocol){
		case IPPROTO_TCP:
			net_address = inet_ntoa(ptrMainApp->accesslist->TCPList->ClientQueue[Node]->Pkt->iph->ip_src);
			sprintf(rule_string,"block in on eth0 proto any from %s to any",net_address);
			WriteFilter(rule_string);
			if(ptrMainApp->AlertStatus !=1){
				ptrMainApp->AlertStatus = 1;
				ptrscreen.DoModal();
			}
			break;
		case IPPROTO_UDP:
			net_address = inet_ntoa(ptrMainApp->accesslist->UDPList->ClientQueue[Node]->Pkt->iph->ip_src);
			sprintf(rule_string,"block in on eth0 proto any from %s to any",net_address);
			WriteFilter(rule_string);
			ptrscreen.DoModal();
			break;
		case IPPROTO_ICMP:
			net_address = inet_ntoa(ptrMainApp->accesslist->ICMPList->ClientQueue[Node]->Pkt->iph->ip_src);
			sprintf(rule_string,"block in on eth0 proto any from %s to any",net_address);
			WriteFilter(rule_string);
			ptrscreen.DoModal();
			break;
	}
	
	return TRUE;	
}

void CTrafficControl::FreeNode(AlertNode *node, int NodeNumber, int protocol)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	CSingleLock objCrit(&ptrMainApp->m_CriticalSection);
	AlertNode *ptrNode;
	int loop = NULL;
	objCrit.Lock();
		while(node != NULL){
			if(node->next != NULL){
				ptrNode = node->next;
				free(node);
				node = NULL;
				node = ptrNode;
			}else{
				ptrNode = node;
				node = NULL;
			}
		}
		free(ptrNode);
		switch(protocol){
			case IPPROTO_TCP:
				ptrMainApp->accesslist->TCPList->ClientQueue[NodeNumber] = NULL;
				break;
			case IPPROTO_UDP:
				ptrMainApp->accesslist->UDPList->ClientQueue[NodeNumber] = NULL;
				break;
			case IPPROTO_ICMP:
				ptrMainApp->accesslist->ICMPList->ClientQueue[NodeNumber] = NULL;
				break;
		}
	objCrit.Unlock();
}

void CTrafficControl::DeleteFilter(char* filter)
{
	FILE *thefp;       
    char *buf;		
	char *Index;
	int ptrpos, ele = 0, putele = 0;
	fpos_t filepos, startpos;
	CString string;
	CString strfilter;
	CString filebuffer[STD_BUF];
	buf = new char[STD_BUF];
	Index = new char[STD_BUF];
    if ((thefp = fopen("e:/temp/denylist/filter.dlt","r")) == NULL)
    {
	}
    bzero((char *)buf, STD_BUF);
	   while ((fgets(buf, STD_BUF, thefp)) != NULL){
		      Index = buf;
	          if ((*Index != '#') && (*Index!= 0x0a) && (*Index!= ';') && (Index != SEEK_SET)){
					while (isspace((int) *Index)) Index++;
					m_ptrMainApp->Strip(Index);
					m_ptrMainApp->Strip(filter);
					string = Index;
					strfilter = filter;
					strfilter.TrimRight();
					strfilter.TrimLeft();
					ptrpos = string.Find(strfilter,0);
					if( ptrpos == -1 && ptrpos != 0){
						filebuffer[ele]	= Index;
						filebuffer[ele] += "\n";ele++;
					}
			  }
        bzero((char *)buf, STD_BUF);
    }
    fclose(thefp);
	if ((thefp = fopen("e:/temp/denylist/filter.dlt","w+")) == NULL);
		while(ele != putele){
			fputs(filebuffer[putele],thefp);putele++;
		}
	fclose(thefp);
    return;	
}

void CTrafficControl::AddFilterToList(char *filter)
{
	FILE *thefp;
	if((thefp = fopen("e:/temp/denylist/filter.dlt","a")) == NULL){
		return;	
	}

	if(!fputs(filter,thefp)){
		fclose(thefp);
		return;
	}
	fclose(thefp);
}
